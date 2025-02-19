#pragma once

#include "FreeRTOS.h"
#include <queue.h>
#include "PayloadGatekeeperTask.hpp"
#include <etl/vector.h>
#include "TaskConfigs.hpp"

class TestTask : public Task {
private:
    StackType_t taskStack[TestTaskStack];

    const uint16_t delayMs = 3000;

    static req_set_time request_time;
    static res_set_time response_time;

    static req_get_ldd_telemetries request_gpo;
    static res_get_ldd_telemetries response_gpo;

    static req_file_write request_file_write;
    static res_file_write response_file_write;

    static req_file_read request_file_read;
    static res_file_read response_file_read;

    static req_file_get_size request_file_get_size;
    static res_file_get_size response_file_get_size;

    static req_file_delete request_file_delete;
    static res_file_delete response_file_delete;

    static req_get_mode request_get_mode;
    static res_get_mode response_get_mode;

    static req_get_main_telemetries request_get_main_telemetry;
    static res_get_main_telemetries response_get_main_telemetry;

    static req_set_mode request_set_mode;
    static res_set_mode response_set_mode;


public:
    void execute();

    TestTask();

    static inline TaskHandle_t TestTaskHandle;

    static void changePayloadMode(ATLAS_mode mode);

    static void testPayload();

    static void getPayloadTelemetries();

    void createTask() {
        TestTaskHandle = xTaskCreateStatic(vClassTask<TestTask>, this->TaskName,
                                           TestTaskStack, this,
                                           TestTaskPriority, this->taskStack,
                                           &(this->taskBuffer));
    }
};

inline std::optional<TestTask> TestTask;