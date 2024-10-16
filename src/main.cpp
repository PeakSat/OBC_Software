#include "main.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "list.h"
#include "task.h"
#include "definitions.h"
#include "OBC_Definitions.hpp"

// Task Header files start
#include "UARTGatekeeperTask.hpp"
#include "TimeKeepingTask.hpp"
#include "AmbientTemperatureTask.hpp"
#include "WatchdogTask.hpp"
#include "MCUTemperatureTask.hpp"
#include "HousekeepingTask.hpp"
#include "TimeBasedSchedulingTask.hpp"
#include "StatisticsReportingTask.hpp"
#include "CANGatekeeperTask.hpp"
#include "CANTestTask.hpp"
#include "TCHandlingTask.hpp"
#include "NANDTask.hpp"
#include "MRAMTask.hpp"
#include "PayloadTestTask.hpp"
// Task Header files end

#define IDLE_TASK_SIZE 1400

#if configSUPPORT_STATIC_ALLOCATION
/* static memory allocation for the IDLE task */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[IDLE_TASK_SIZE];

extern "C" void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer,
                                              uint32_t *pulIdleTaskStackSize) {
    *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
    *ppxIdleTaskStackBuffer = &xIdleStack[0];
    *pulIdleTaskStackSize = IDLE_TASK_SIZE;
}

#endif

extern "C" void main_cpp() {
    SYS_Initialize(NULL);

    uartGatekeeperTask.emplace();
    timeKeepingTask.emplace();
    ambientTemperatureTask.emplace();
    watchdogTask.emplace();
    mcuTemperatureTask.emplace();
    tcHandlingTask.emplace();
    housekeepingTask.emplace();
    canGatekeeperTask.emplace();
    canTestTask.emplace();
    payloadTestTask.emplace();
    nandTask.emplace();
    mramTask.emplace();


    ambientTemperatureTask->createTask();
    mcuTemperatureTask->createTask();
    timeKeepingTask->createTask();
    uartGatekeeperTask->createTask();
    watchdogTask->createTask();
    tcHandlingTask->createTask();
    housekeepingTask->createTask();
    canGatekeeperTask->createTask();
    canTestTask->createTask();
    payloadTestTask->createTask();
    nandTask->createTask();
    mramTask->createTask();

    vTaskStartScheduler();

    while (true) {
        /* Maintain state machines of all polled MPLAB Harmony modules. */
        SYS_Tasks();
    }

    return;
}

