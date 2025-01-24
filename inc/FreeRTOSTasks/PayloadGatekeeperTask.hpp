//
// Created by tats on 10/7/2024.
//

#pragma once

#include "FreeRTOS.h"
#include "TaskConfigs.hpp"
#include "task.h"
#include "queue.h"
#include "Logger.hpp"
#include "plib_tc0.h"
#include "general_definitions.hpp"
#include "ATLAS_Frame_Definitions.hpp"
#include "ATLAS_payload.hpp"

class PayloadGatekeeperTask : public Task {
private:

    StackType_t taskStack[PayloadGatekeeperTaskStack];

    static const uint8_t maxFrameQueueSize = 5;

    StaticQueue_t xPayloadSendFrameQueue;
    QueueHandle_t xFrameSendQueueHandle;
    uint8_t payloadSendFrameQueueStorage[maxFrameQueueSize*(payload_size_size+max_payload_size)];

    StaticQueue_t xPayloadReceiveFrameQueue;
    QueueHandle_t xFrameReceiveQueueHandle;
    uint8_t payloadReceiveFrameQueueStorage[maxFrameQueueSize*(payload_size_size+max_payload_size)];

    uint8_t internal_buffer[payload_size_size+max_payload_size];

    uint8_t error = 0;

public:

    void execute();

    PayloadGatekeeperTask();

    /**
     * Create freeRTOS Task
     */
    void createTask() {
        taskHandle = xTaskCreateStatic(vClassTask<PayloadGatekeeperTask>, this->TaskName, PayloadGatekeeperTaskStack, this,
                          PayloadGatekeeperTaskPriority, this->taskStack, &(this->taskBuffer));
    }

    bool addPayloadSendQueue(uint8_t* payload, uint16_t size, bool isISR){
        internal_buffer[0] = size & 0xFF;
        internal_buffer[1] = (size>>8) & 0xFF;
        memcpy(&internal_buffer[2], payload, size);

        if(isISR){
            BaseType_t xHigherPriorityTaskWoken = pdFALSE;
            xQueueSendToBackFromISR(xFrameSendQueueHandle, internal_buffer, &xHigherPriorityTaskWoken);
            xTaskNotifyFromISR(taskHandle, PAYLOAD_SND, eSetValueWithOverwrite, &xHigherPriorityTaskWoken);
            return true;
        }

        if(xQueueSendToBack(xFrameSendQueueHandle, internal_buffer, 0) == pdTRUE){
            xTaskNotify(taskHandle, PAYLOAD_SND, eSetValueWithOverwrite);
            return true;
        }
        // xTaskNotify(this->taskHandle, PAYLOAD_SND, eNoAction);
        return false;
    }

    bool addPayloadReceiveQueue(uint8_t* payload, uint16_t size, bool isISR){
        internal_buffer[0] = size & 0xFF;
        internal_buffer[1] = (size>>8) & 0xFF;
        memcpy(&internal_buffer[2], payload, size);

        if(isISR){
            BaseType_t xHigherPriorityTaskWoken = pdFALSE;
            xQueueSendToBackFromISR(xFrameReceiveQueueHandle, internal_buffer, &xHigherPriorityTaskWoken);
            xTaskNotifyFromISR(taskHandle, PAYLOAD_RCV, eSetValueWithOverwrite, &xHigherPriorityTaskWoken);
            return true;
        }

        if(xQueueSendToBack(xFrameReceiveQueueHandle, internal_buffer, 0) == pdTRUE){
            xTaskNotify(taskHandle, PAYLOAD_RCV, eSetValueWithOverwrite);
            return true;
        }
        // xTaskNotify(taskHandle, PAYLOAD_RCV, eNoAction);
        return false;
    }

    void setPayloadError(uint8_t error_code, bool isISR){
        error = (uint8_t) error_code;
        if(isISR){
            BaseType_t xHigherPriorityTaskWoken = pdFALSE;
            xTaskNotifyFromISR(taskHandle, PAYLOAD_ERR, eSetValueWithOverwrite, &xHigherPriorityTaskWoken);
            return;
        }
        xTaskNotify(taskHandle, PAYLOAD_ERR, eSetValueWithOverwrite);
    }

};

inline std::optional<PayloadGatekeeperTask> PayloadGatekeeperTask;
