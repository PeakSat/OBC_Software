#include "main.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "list.h"
#include "task.h"
#include "definitions.h"
#include "general_definitions.hpp"
#include "OBC_Definitions.hpp"
#include "FreeRTOSHandlers.hpp"


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
#include "CANTestTask.hpp"
#include "TCHandlingTask.hpp"
//#include "NANDTask.hpp"
//#include "MRAMTask.hpp"
#include "MemoryManagementTask.hpp"
#include "PayloadTestTask.hpp"
#include "TestTask.hpp"
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
    DWT->CYCCNT = 0;                               // Reset the cycle counter
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;           // Enable the cycle counter
}

uint32_t getTimerValue(void){
    return (DWT->CYCCNT);
}
#endif


extern "C" void main_cpp() {
    SYS_Initialize(NULL);

    uartGatekeeperTask.emplace();
//    nandTask.emplace();
     payloadTestTask.emplace();
     canGatekeeperTask.emplace();
     canTestTask.emplace();
     housekeepingTask.emplace();
     tcHandlingTask.emplace();
     mcuTemperatureTask.emplace();
    // ambientTemperatureTask.emplace();
//     mramTask.emplace();
     memManTask.emplace();
     timeKeepingTask.emplace();
     TestTask.emplace();
    watchdogTask.emplace();


    __disable_irq();
    uartGatekeeperTask->createTask();
//    nandTask->createTask();
     payloadTestTask->createTask();
     canGatekeeperTask->createTask();
     canTestTask->createTask();
     housekeepingTask->createTask();
//     tcHandlingTask->createTask();
     mcuTemperatureTask->createTask();
    // ambientTemperatureTask->createTask();
    // mramTask->createTask();
    memManTask->createTask();
     timeKeepingTask->createTask();
     TestTask->createTask();
    watchdogTask->createTask();

    __enable_irq();

    vTaskStartScheduler();

    while (true) {
        /* Maintain state machines of all polled MPLAB Harmony modules. */
        SYS_Tasks();
    }

    return;
}
