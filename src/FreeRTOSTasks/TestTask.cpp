#include "TestTask.hpp"

#include <ApplicationLayer.hpp>

#include "task.h"
etl::array<uint16_t, CAN::TPMessageMaximumArguments> COMMSUHFBandPATemperature = {2000};
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

    while (true) {
        //monitorAllTasks();
        CAN::Application::createRequestParametersMessage(CAN::OBC, false, COMMSUHFBandPATemperature, false);
        vTaskDelay(pdMS_TO_TICKS(this->delayMs));
    }
}