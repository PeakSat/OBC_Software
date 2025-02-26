#pragma once

#include "FreeRTOS.h"
#include <queue.h>
#include "PayloadGatekeeperTask.hpp"
#include <etl/vector.h>
#include "TaskConfigs.hpp"
#include "MemoryManager.hpp"

class TestTask : public Task {
private:
    StackType_t taskStack[TestTaskStack];

    const uint16_t delayMs = 200;

public:
    void execute();

    TestTask();

    static inline TaskHandle_t TestTaskHandle;

    void createTask() {
        TestTaskHandle = xTaskCreateStatic(vClassTask<TestTask>, this->TaskName,
                                           TestTaskStack, this,
                                           TestTaskPriority, this->taskStack,
                                           &(this->taskBuffer));
    }
};

inline std::optional<TestTask> TestTask;