#pragma once

#include "TaskConfigs.hpp"
#include <OBC_Definitions.hpp>
#include "PayloadGatekeeperTask.hpp"
#include "ServicePool.hpp"
#include <MemoryManager.hpp>
#include "MCP9808.hpp"


class OnBoardMonitoringTask : public Task {
public:
    /**
     * Number of sensors on the PCB
     */
    static inline constexpr uint8_t NumberOfTemperatureSensors = 2;

    /**
     * The I2C addresses of the sensors based on the pin configuration of the physical devices
     */
    inline static constexpr etl::array<uint8_t, NumberOfTemperatureSensors>
        SensorI2CAddress = {0, 1};

    /**
     * The driver for the MCP9808 temperature sensor
     */
    etl::array<MCP9808, NumberOfTemperatureSensors> sensors = {MCP9808(SensorI2CAddress[0]),
                                                               MCP9808(SensorI2CAddress[1])};

    /**
     * The value of the temperature, measured in Celsius
     */
    etl::array<float, NumberOfTemperatureSensors> ambientTemperature = {0, 0};

    StackType_t taskStack[OnBoardMonitoringTaskStack]{};

    inline static TaskHandle_t onBoardMonitoringTaskHandle = nullptr;

    void execute();

    OnBoardMonitoringTask();

    /**
     * Create freeRTOS Task
     */
    void createTask() {
        onBoardMonitoringTaskHandle = xTaskCreateStatic(vClassTask<OnBoardMonitoringTask>, this->TaskName, OnBoardMonitoringTaskStack,
                          this, OnBoardMonitoringTaskPriority, this->taskStack,
                          &(this->taskBuffer));
    }
private:

    static void getMCUTemperature();
    void getAmbientTemperature();
    void checkAmbientSensors();
};

inline std::optional<OnBoardMonitoringTask> onBoardMonitoringTask;
