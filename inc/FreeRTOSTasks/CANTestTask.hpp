#pragma once

#include "CAN/Driver.hpp"
#include "TaskConfigs.hpp"

class CANTestTask : public Task {
private:
public:
    StackType_t taskStack[CANTestTaskStack];

    void execute();

    CANTestTask() : Task("CAN Test") {}

    /**
     * Create freeRTOS Task
     */
    void createTask() {
        xTaskCreateStatic(vClassTask<CANTestTask>, this->TaskName, CANTestTask::TaskStackDepth, this,
                          CANTestTaskPriority, this->taskStack, &(this->taskBuffer));
    }
};

inline std::optional<CANTestTask> canTestTask;
