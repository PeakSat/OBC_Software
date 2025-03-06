#pragma once

#include "FreeRTOS.h"
#include "Logger.hpp"

extern "C" void vApplicationStackOverflowHook(TaskHandle_t xTask, char* pcTaskName) {
    LOG_DEBUG << "Stack overflow on task " << pcTaskName;
}

extern "C" void vApplicationGetTimerTaskMemory( StaticTask_t **ppxTimerTaskTCBBuffer,
                                      StackType_t **ppxTimerTaskStackBuffer,
                                      uint32_t *pulTimerTaskStackSize )
{
    /* Allocate memory for the Timer task's TCB and stack. */
    static StaticTask_t xTimerTaskTCB;
    static StackType_t uxTimerTaskStack[ configTIMER_TASK_STACK_DEPTH ];

    /* Set the buffer pointers to the allocated memory. */
    *ppxTimerTaskTCBBuffer = &xTimerTaskTCB;
    *ppxTimerTaskStackBuffer = uxTimerTaskStack;
    *pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
}