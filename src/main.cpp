#include "main.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "list.h"
#include "task.h"
#include "definitions.h"
#include "general_definitions.hpp"
#include "OBC_Definitions.hpp"
#include "FreeRTOSHandlers.hpp"
#include "HelperFunctions.hpp"
#include "mutex_Handler.h"

// ECSS Header Files
#include "ErrorHandler.hpp"
#include "Message.hpp"

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
#include "CANParserTask.hpp"
#include "TCHandlingTask.hpp"

#include "HeartbeatTask.hpp"
#include "PayloadGatekeeperTask.hpp"
#include "TestTask.hpp"
#include "OnBoardMonitoringTask.hpp"

// Task Header files end


#define IDLE_TASK_SIZE 1000
#if configSUPPORT_STATIC_ALLOCATION
/* static memory allocation for the IDLE task */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[IDLE_TASK_SIZE];

extern "C" void vApplicationGetIdleTaskMemory(StaticTask_t** ppxIdleTaskTCBBuffer, StackType_t** ppxIdleTaskStackBuffer,
                                              uint32_t* pulIdleTaskStackSize) {
    *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
    *ppxIdleTaskStackBuffer = &xIdleStack[0];
    *pulIdleTaskStackSize = IDLE_TASK_SIZE;
}

#endif

#if configGENERATE_RUN_TIME_STATS
void configureDWTForRunTimeStats(void) {
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk; // Enable DWT access
    DWT->CYCCNT = 0;                                // Reset the cycle counter
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;            // Enable the cycle counter
}

uint32_t getTimerValue(void) { return (DWT->CYCCNT); }
#endif

extern "C" void main_cpp() {
    SYS_Initialize(NULL);

    uartGatekeeperTask.emplace();
    PayloadGatekeeperTask.emplace();
    canGatekeeperTask.emplace();
    canParserTask.emplace();
    // housekeepingTask.emplace();
    onBoardMonitoringTask.emplace();
    // tcHandlingTask.emplace();
    mcuTemperatureTask.emplace();
    ambientTemperatureTask.emplace();
    timeKeepingTask.emplace();
    TestTask.emplace();
    watchdogTask.emplace();
    heartbeatTask.emplace();


    __disable_irq();
    uartGatekeeperTask->createTask();
    PayloadGatekeeperTask->createTask();
    canGatekeeperTask->createTask();
    canParserTask->createTask();
    // housekeepingTask->createTask();
    onBoardMonitoringTask->createTask();
    // tcHandlingTask->createTask();
    mcuTemperatureTask->createTask();
    ambientTemperatureTask->createTask();
    timeKeepingTask->createTask();
    TestTask->createTask();
    watchdogTask->createTask();
    heartbeatTask->createTask();

    __enable_irq();

    initializeSemaphores();
    HelperFunctions::resetChecks();             // get the last reason of reset
    HelperFunctions::initialiseMPURegions();    // setup MPU regions for external memories

    vTaskStartScheduler();

    while (true) {
        /* Maintain state machines of all polled MPLAB Harmony modules. */
        SYS_Tasks();
    }

    return;
}