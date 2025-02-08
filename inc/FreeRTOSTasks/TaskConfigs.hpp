#pragma once

#include "Task.hpp"

constexpr BaseType_t AmbientTemperatureTaskPriority = tskIDLE_PRIORITY + 1;
constexpr BaseType_t CANGatekeeperTaskPriority = tskIDLE_PRIORITY + 2;
constexpr BaseType_t CANParserTaskPriority = tskIDLE_PRIORITY + 1;
constexpr BaseType_t HousekeepingTaskPriority = tskIDLE_PRIORITY + 2;
constexpr BaseType_t MCUTemperatureTaskPriority = tskIDLE_PRIORITY + 1;
//const BaseType_t MRAMTaskPriority = tskIDLE_PRIORITY + 2;
const BaseType_t NANDTaskPriority = tskIDLE_PRIORITY + 2;
const BaseType_t MemoryManagementTaskPriority = tskIDLE_PRIORITY + 2;
const BaseType_t PayloadGatekeeperTaskPriority = tskIDLE_PRIORITY + 1;
const BaseType_t StatisticsReportingTaskPriority = tskIDLE_PRIORITY + 1;
const BaseType_t TCHandlingTaskPriority = tskIDLE_PRIORITY + 1;
const BaseType_t TimeBasedSchedulingTaskPriority = tskIDLE_PRIORITY + 2;
const BaseType_t TimeKeepingTaskPriority = tskIDLE_PRIORITY + 2;
const BaseType_t UARTGatekeeperTaskPriority = tskIDLE_PRIORITY + 3;
const BaseType_t WatchdogTaskPriority = configMAX_PRIORITIES - 1;

constexpr BaseType_t TestTaskPriority = tskIDLE_PRIORITY + 2;
constexpr BaseType_t OnBoardMonitoringTaskPriority = tskIDLE_PRIORITY + 2;

/**
* The stack depth of each FreeRTOS task, defined as the number of words the stack can hold. For example, in an
* architecture with 4 byte stack, assigning 100 to the usStackDepth argument, will allocate 4x100=400 bytes.
*/
constexpr uint16_t AmbientTemperatureTaskStack = 1200;
constexpr uint16_t CANGatekeeperTaskStack = 4500;
constexpr uint16_t CANParserTaskStack = 5300;
constexpr uint16_t HousekeepingTaskStack = 3300;
constexpr uint16_t MCUTemperatureTaskStack = 500;
//const uint16_t MRAMTaskStack = 5000;

constexpr uint16_t NANDTaskStack = 100;
constexpr uint16_t MemoryManagementTaskStack = 1200;
constexpr uint16_t PayloadTestTaskStack = 500;
constexpr uint16_t StatisticsReportingTaskStack = 2000;
constexpr uint16_t TCHandlingTaskStack = 1000;
constexpr uint16_t TimeBasedSchedulingTaskStack = 500;
constexpr uint16_t TimeKeepingTaskStack = 1000;
constexpr uint16_t PayloadGatekeeperTaskStack = 3000;
constexpr uint16_t UARTGatekeeperTaskStack = 1000;
constexpr uint16_t WatchdogTaskStack = 1000;
constexpr uint16_t HeartbeatTaskStack = 1800;
constexpr uint16_t OnBoardMonitoringTaskStack = 3500;


constexpr uint16_t TestTaskStack = 1000;
