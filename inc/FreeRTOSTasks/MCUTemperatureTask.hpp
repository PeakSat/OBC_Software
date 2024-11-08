#pragma once

#include "TaskConfigs.hpp"

/**
 * FreeRTOS task for periodically printing the value of the internal temperature sensor.
 */
class MCUTemperatureTask : public Task {
private:
    const uint16_t delayMs = 7000;

    StackType_t taskStack[MCUTemperatureTaskStack];

public:
    void execute();

    MCUTemperatureTask() : Task("MCUTemperatureSensor") {}

    void createTask() {
        xTaskCreateStatic(vClassTask<MCUTemperatureTask>, this->TaskName, MCUTemperatureTask::TaskStackDepth, this,
                          MCUTemperatureTaskPriority, this->taskStack, &(this->taskBuffer));
    }
};

inline std::optional<MCUTemperatureTask> mcuTemperatureTask;
