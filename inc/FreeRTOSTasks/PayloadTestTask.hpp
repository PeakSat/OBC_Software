//
// Created by tats on 10/7/2024.
//

#pragma once

#include "RS422/RS422_Driver.hpp"
#include "TaskConfigs.hpp"

class PayloadTestTask : public Task {
private:
public:
    StackType_t taskStack[PayloadTestTaskStack];

    void execute();

    PayloadTestTask() : Task("Payload Test") {}

    /**
     * Create freeRTOS Task
     */
    void createTask() {
        xTaskCreateStatic(vClassTask<PayloadTestTask>, this->TaskName, PayloadTestTask::TaskStackDepth, this,
                          PayloadTestTaskPriority, this->taskStack, &(this->taskBuffer));
    }
};

inline std::optional<PayloadTestTask> payloadTestTask;
