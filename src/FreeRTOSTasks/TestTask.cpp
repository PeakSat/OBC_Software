#include "TestTask.hpp"
#include "TypeDefinitions.hpp"
#include "MemoryManager.hpp"
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

// req_set_time request_time;
// res_set_time response_time;
//
// req_get_ldd_telemetries request_gpo;
// res_get_ldd_telemetries response_gpo;

void TestTask::execute() {
    vTaskDelay(pdMS_TO_TICKS(1000));

    // ParameterId param = PeakSatParameters::EPS_UNIX_SECOND_ID;
    // uint8_t temp = 0;
    // MemoryManager::getParameter(param, static_cast<void*>(&temp));
    //
    // request_time.timestamp = 1738070674;
    //
    // if(PayloadGatekeeperTask->sendrecvPayload(request_time.req_code, static_cast<void*>(&request_time), static_cast<void*>(&response_time))){
    //     LOG_DEBUG<<"Payload Responded with time: "<<response_time.timestamp<<" Status: "<<response_time.status;
    // }

    while (true) {

        // if(PayloadGatekeeperTask->sendrecvPayload(request_gpo.req_code, static_cast<void*>(&request_gpo), static_cast<void*>(&response_gpo))){
        //     LOG_DEBUG<<"LDD TM Amp out pow  :"<<response_gpo.amplifier_output_power;
        //     LOG_DEBUG<<"LDD TM ld temp      :"<<response_gpo.ld_temperature;
        //     LOG_DEBUG<<"LDD TM Time (s)     :"<<response_gpo.time;
        //
        // }



        // LOG_DEBUG<<"commit_hashID"<<MemoryManager::getParameterAsUINT64(PeakSatParameters::commit_hashID);
        //
        // LOG_DEBUG<<"UseCANID: "<<MemoryManager::getParameterAsUINT64(PeakSatParameters::UseCANID);

        // vTaskDelay(5000);
        //
        // String<64> logString = "The value for parameter with ID ";
        // etl::to_string(param, logString, true);
        // logString.append(" was ");
        // temp+=5;
        // etl::to_string(MemoryManager::getParameterAsUINT64(param), logString, true);
        // MemoryManager::setParameter(param, static_cast<void*>(&temp));
        // logString.append(" and is now ");
        // etl::to_string(MemoryManager::getParameterAsUINT64(param), logString, true);


        // LOG_DEBUG << logString.c_str();
        //monitorAllTasks();
        vTaskDelay(pdMS_TO_TICKS(3000));
    }
}