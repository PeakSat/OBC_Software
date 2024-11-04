//
// Created by tats on 10/7/2024.
//

#pragma once

#include "RS422/RS422_Driver.hpp"
#include "FreeRTOS.h"
#include <queue.h>
#include "etl/String.hpp"
#include "TaskConfigs.hpp"

class PayloadTestTask : public Task {
private:
    /**
     * A buffer to save externally incoming bytes using usart. Queued for later use.
     */
    etl::string<20> savedMessage;

    /**
     * A buffer containing UART bytes coming out of the queue.
     */
    etl::string<20> messageOut;

    /**
     * Incoming byte
     */
    uint8_t byteIn = 0;

    uint32_t uart_err = 0;

    /**
     * The value that signals a message is done being transmitted.
     */
    const inline static uint8_t MessageEndDelimiter = 0x24; // $ symbol

    /**
     * Saves incoming bytes by inserting them into a queue.
     */
    uint8_t messageQueueStorageArea[TCQueueCapacity * sizeof(etl::string<20>)];
    StaticQueue_t messageQueue;
    QueueHandle_t messageQueueHandle;

public:
    StackType_t taskStack[PayloadTestTaskStack];

    void execute();

    void resetInput();

    void ingress();

    PayloadTestTask();

    /**
     * Create freeRTOS Task
     */
    void createTask() {
        xTaskCreateStatic(vClassTask<PayloadTestTask>, this->TaskName, PayloadTestTask::TaskStackDepth, this,
                          PayloadTestTaskPriority, this->taskStack, &(this->taskBuffer));
    }
};

inline std::optional<PayloadTestTask> payloadTestTask;
