#pragma once

#include "TaskConfigs.hpp"
#include "MemoryManagementTask.hpp"

/**
 * FreeRTOS task for periodically printing the value of the internal temperature sensor.
 */
class MCUTemperatureTask : public Task {
private:
    const uint16_t delayMs = 7000;

    StackType_t taskStack[MCUTemperatureTaskStack];

    inline static TaskHandle_t AFEC0HandlingTaskHandle = nullptr;

public:
    void execute() const;

    MCUTemperatureTask();

    void createTask() {
        AFEC0HandlingTaskHandle = xTaskCreateStatic(vClassTask<MCUTemperatureTask>, this->TaskName,
                                                    MCUTemperatureTaskStack, this,
                                                    MCUTemperatureTaskPriority, this->taskStack, &(this->taskBuffer));
    }
};

inline std::optional<MCUTemperatureTask> mcuTemperatureTask;
