#include "TestTask.hpp"
#include "task.h"



void monitorAllTasks() {
    unsigned long ulTotalRunTime, ulStatsAsPercentage;
    UBaseType_t tasks_num = uxTaskGetNumberOfTasks();
    TaskStatus_t taskStatusArray[tasks_num]; // Adjust size to number of tasks
    UBaseType_t taskCount = uxTaskGetSystemState(taskStatusArray, tasks_num, &ulTotalRunTime);

    for (UBaseType_t i = 0; i < taskCount; i++) {
        if(ulTotalRunTime>0){
            ulStatsAsPercentage = taskStatusArray[i].ulRunTimeCounter/ulTotalRunTime;
            if(ulStatsAsPercentage>0UL){
                LOG_DEBUG<<"Task: "<<taskStatusArray[i].pcTaskName<<"\tRem stack: "<<taskStatusArray[i].usStackHighWaterMark<<"\tState: "<<taskStatusArray[i].eCurrentState<<"\tRun Time (%): "<<ulStatsAsPercentage;
            }else {
                LOG_DEBUG << "Task: " << taskStatusArray[i].pcTaskName << "\tRem stack: " << taskStatusArray[i].usStackHighWaterMark << "\tState: " << taskStatusArray[i].eCurrentState << "\tRun Time (%): 0";
            }
        }

    }
}

TestTask::TestTask() : Task("TestTask") {
    // LOG_INFO << "Initialised instance of TestTask";
}

void TestTask::execute() {
    vTaskDelay(pdMS_TO_TICKS(this->delayMs));

    while (true) {
        monitorAllTasks();
        // PIO_PinToggle(USR_GPIO_PIN);
        vTaskDelay(pdMS_TO_TICKS(this->delayMs));
    }
}