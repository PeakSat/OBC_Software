#pragma once 

#include "FreeRTOS.h"
#include <queue.h>
#include "TaskConfigs.hpp"

class TestTask : public Task {
private:
    StackType_t taskStack[TestTaskStack];
    
    const uint16_t delayMs = 5000;

public:
    void execute();

    TestTask();

    static inline TaskHandle_t TestTaskHandle;

    void createTask() {
        TestTaskHandle = xTaskCreateStatic(vClassTask < TestTask > , this->TaskName,
                                                     TestTask::TaskStackDepth, this,
                                                     TestTaskPriority, this->taskStack, &(this->taskBuffer));
    }
};

inline std::optional<TestTask> TestTask;