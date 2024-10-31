//
// Created by tats on 10/7/2024.
//

#include "PayloadTestTask.hpp"
#include <definitions.h>
#include "task.h"
#include "Logger.hpp"
#include "RS422/Payload_Message.hpp"
#include "adm_driver.hpp"
#include "TestTask.hpp"

PayloadTestTask::PayloadTestTask() : Task("Payload Test") {
    messageQueueHandle = xQueueCreateStatic(TCQueueCapacity, sizeof(etl::string<20>),
                                            messageQueueStorageArea,
                                            &messageQueue);
    configASSERT(messageQueueHandle);

    UART2_ReadCallbackRegister([](uintptr_t object) -> void {
        PayloadTestTask *task = reinterpret_cast<PayloadTestTask * >(object);

        if (UART2_ReadCountGet() == 0) {
            task->uart_err = UART2_ErrorGet();
        } else {
            task->ingress();
        }

        UART2_Read(&(task->byteIn), sizeof(this->byteIn));
    }, reinterpret_cast<uintptr_t>(this));

    UART2_Read(&byteIn, sizeof(byteIn));
}

void PayloadTestTask::resetInput() {
    new(&(PayloadTestTask::savedMessage)) etl::string<20>;
}

void PayloadTestTask::ingress() {
    BaseType_t higherPriorityTaskWoken = pdFALSE;

    if (savedMessage.full()) {
        resetInput();
    }

    if (byteIn == MessageEndDelimiter) {
        xQueueSendToBackFromISR(messageQueueHandle, static_cast<void *>(&savedMessage), &higherPriorityTaskWoken);
        resetInput();
    } else {
        savedMessage.push_back(byteIn);
    }

    if (higherPriorityTaskWoken) {
        portYIELD_FROM_ISR(higherPriorityTaskWoken);
    }
}


void PayloadTestTask::execute() {

    LOG_DEBUG << "Runtime init: " << this->TaskName;
    transparentModeRS422();

    while (true) {

//        LOG_DEBUG << "Runtime entered: " << this->TaskName;
        if(sendPayloadMessage(payloadMessage, 512)){
            LOG_DEBUG<<"Sent 512 bytes payload message";
        }else{
            LOG_DEBUG<<"Unable to send payload message";
        }

        if(xQueueReceive(messageQueueHandle, static_cast<void *>(&messageOut), 0)==pdTRUE){
            messageOut.repair();
            storeMsgADM((char*) messageOut.c_str(), messageOut.length());
            xTaskNotify(TestTask->TestTaskHandle, 0, eNoAction);
            LOG_DEBUG<<"Notified TestTask for incoming message";
        }
        

//        LOG_DEBUG << "Runtime exit: " << this->TaskName;
        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}