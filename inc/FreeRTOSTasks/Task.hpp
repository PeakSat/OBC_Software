#pragma once

#include "FreeRTOS.h"
#include "task.h"
#include "optional"
//#include "ServicePool.hpp"
#include "BootCounter.hpp"
#include "definitions.h"
#include "Logger.hpp"
#include "PeakSatParameters.hpp"
#include "Definitions.hpp"

/**
 * Base class, whose method 'execute' is meant to be inherited by each and every individual FreeRTOS task.
 */
class Task {
public:
    /**
     * taskBuffer parameter of each FreeRTOS task that holds the new task's data structures 
     */
    StaticTask_t taskBuffer;

    /**
     * A native FreeRTOS task that calls the `->execute()` function of a C++ Task.
     *
     * Useful for converting between native C-style tasks, and tasks defined as C++ classes.
     *
     * @tparam T A class with an `execute()` function that represent the state of a FreeRTOS task
     * @param pvParameters This argument is passed by FreeRTOS and should contain a pointer to the Task object
     */
    template <class T>
    static void vClassTask(void* pvParameters) {
        (static_cast<T*>(pvParameters))->execute();
    }

public:
    /**
     * Name of each task.
     */
    const char* TaskName;

    /**
     * Handle of each FreeRTOS task.
     */
    TaskHandle_t taskHandle;


    Task(const char* TaskName) : TaskName(TaskName) {}
};
