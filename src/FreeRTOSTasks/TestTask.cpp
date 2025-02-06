#include "TestTask.hpp"
#include "TypeDefinitions.hpp"
#include "MemoryManagementTask.hpp"
#include "task.h"

void monitorAllTasks() {
//    unsigned long ulTotalRunTime, ulStatsAsPercentage;
//    UBaseType_t tasks_num = uxTaskGetNumberOfTasks();
//    TaskStatus_t taskStatusArray[tasks_num]; // Adjust size to number of tasks
//    UBaseType_t taskCount = uxTaskGetSystemState(taskStatusArray, tasks_num, &ulTotalRunTime);
//    etl::vector<std::string, 15> log_entries; // If tasks are more than 15 adjust the size appropriately
//
//    for (UBaseType_t i = 0; i < taskCount; i++) {
//        ulStatsAsPercentage = taskStatusArray[i].ulRunTimeCounter * 100 /ulTotalRunTime;
//        std::ostringstream entry;
//        entry << "Task: " << taskStatusArray[i].pcTaskName
//              << "\tRem stack: " << taskStatusArray[i].usStackHighWaterMark
//              << "\tState: " << taskStatusArray[i].eCurrentState
//              << "\tRun Time (%): " << ulStatsAsPercentage;
//        log_entries.push_back(entry.str());
//    }

//    for(uint i=0;i<tasks_num;i++){
//        LOG_DEBUG<<log_entries[i].c_str();
//    }
}

TestTask::TestTask() : Task("TestTask") {
    // LOG_INFO << "Initialised instance of TestTask";
}

void TestTask::execute() {
    vTaskDelay(pdMS_TO_TICKS(this->delayMs));
    ParameterId param = PeaksatParameters::EPS_UNIX_SECONDID;
    uint8_t temp = 0;
    memManTask->getParameter(param, static_cast<void*>(&temp));

    while (true) {

        String<64> logString = "The value for parameter with ID ";
        etl::to_string(param, logString, true);
        logString.append(" was ");
        temp+=5;
        etl::to_string(memManTask->getParameterAsUINT64(param), logString, true);
        memManTask->setParameter(param, static_cast<void*>(&temp));
        logString.append(" and is now ");
        etl::to_string(memManTask->getParameterAsUINT64(param), logString, true);


        LOG_DEBUG << logString.c_str();
        //monitorAllTasks();
        vTaskDelay(pdMS_TO_TICKS(this->delayMs));
    }
}