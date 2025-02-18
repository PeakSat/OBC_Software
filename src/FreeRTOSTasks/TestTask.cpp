#include "TestTask.hpp"
#include "TypeDefinitions.hpp"
#include "MemoryManagementTask.hpp"
#include "task.h"

#include <binary_sw1.hpp>

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

req_set_time request_time;
res_set_time response_time;

req_get_ldd_telemetries request_gpo;
res_get_ldd_telemetries response_gpo;

req_file_write request_file_write;
res_file_write response_file_write;

req_file_read request_file_read;
res_file_read response_file_read;

req_file_get_size request_file_get_size;
res_file_get_size response_file_get_size;

req_file_delete request_file_delete;
res_file_delete response_file_delete;

void TestTask::execute() {
    vTaskDelay(pdMS_TO_TICKS(this->delayMs));


    request_time.timestamp = 1738070674;

    // if (PayloadGatekeeperTask->sendrecvPayload(request_time.req_code, static_cast<void*>(&request_time), static_cast<void*>(&response_time))) {
    //     LOG_DEBUG << "Payload Responded with time: " << response_time.timestamp << " Status: " << response_time.status;
    // }

    request_file_write.file_descriptor = 32;
    request_file_write.offset = 11;
    request_file_write.size = 21;

    request_file_read.file_descriptor = 32;
    request_file_read.offset = 11;
    request_file_read.size = 21;

    request_file_delete.file_descriptor= 32;





    if (PayloadGatekeeperTask->sendrecvPayload(request_file_delete.req_code, static_cast<void*>(&request_file_delete), static_cast<void*>(&response_file_delete))) {
        LOG_DEBUG << "Delete file: " << response_file_delete.file_descriptor;
        LOG_DEBUG << "Delete file status: " << response_file_delete.status;
    }

    vTaskDelay(pdMS_TO_TICKS(100));

    // uploadPayloadFile();

    if (PayloadGatekeeperTask->sendrecvPayload(request_file_write.req_code, static_cast<void*>(&request_file_write), static_cast<void*>(&response_file_write))) {
            LOG_DEBUG << "Write file FD: " << response_file_write.file_descriptor;
            LOG_DEBUG << "Write file ofset: " << response_file_write.offset;
            LOG_DEBUG << "Write file size: " << response_file_write.size;
            LOG_DEBUG<< "------------------------------------------------------------";
        }
    vTaskDelay(pdMS_TO_TICKS(100));
    if (PayloadGatekeeperTask->sendrecvPayload(request_file_read.req_code, static_cast<void*>(&request_file_read), static_cast<void*>(&response_file_read))) {
        LOG_DEBUG << "READ FD    : " << response_file_read.file_descriptor;
        LOG_DEBUG << "READ offset: " << response_file_read.offset;
        LOG_DEBUG << "READ size  : " << response_file_read.size;
    }
    vTaskDelay(pdMS_TO_TICKS(1000));


    if (PayloadGatekeeperTask->sendrecvPayload(request_file_get_size.req_code, static_cast<void*>(&request_file_get_size), static_cast<void*>(&response_file_get_size))) {
        LOG_DEBUG << "size" << response_file_get_size.size;
        LOG_DEBUG << "FD: " << response_file_get_size.file_descriptor;
    }

    vTaskDelay(pdMS_TO_TICKS(100));

    for (int i = 0; i < 20; i++) {
        LOG_DEBUG << response_file_read.data[i];
    }


    while (true) {


        if (PayloadGatekeeperTask->sendrecvPayload(request_gpo.req_code, static_cast<void*>(&request_gpo), static_cast<void*>(&response_gpo))) {
            LOG_DEBUG << "LDD TM Amp out pow  :" << response_gpo.amplifier_output_power;
            LOG_DEBUG << "LDD TM ld temp      :" << response_gpo.ld_temperature;
            LOG_DEBUG << "LDD TM Time (s)     :" << response_gpo.time;
        }

        vTaskDelay(pdMS_TO_TICKS(this->delayMs));
    }
}