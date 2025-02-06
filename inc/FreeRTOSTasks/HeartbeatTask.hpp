#pragma once

#include "FreeRTOS.h"
#include <queue.h>
#include <etl/vector.h>
#include <etl/optional.h>
#include "TaskConfigs.hpp"

inline bool heartbeatReceived = true;

class HeartbeatTask : public Task {
private:
    StackType_t taskStack[HeartbeatTaskStack];

public:
    void execute();

    HeartbeatTask() : Task("HEARTBEAT") {}

    static inline TaskHandle_t TestTaskHandle;

    void createTask() {
        this->taskHandle = xTaskCreateStatic(vClassTask<HeartbeatTask>, this->TaskName,
                                             HeartbeatTaskStack, this, tskIDLE_PRIORITY + 1,
                                             this->taskStack, &(this->taskBuffer));
    }
};

inline etl::optional<HeartbeatTask> heartbeatTask;
