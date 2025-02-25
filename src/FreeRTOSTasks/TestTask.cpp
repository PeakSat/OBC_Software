#include "TestTask.hpp"
#include "TypeDefinitions.hpp"
#include "MemoryManagementTask.hpp"
#include "task.h"

#include <binary_sw1.hpp>

void TestTask::changePayloadMode(ATLAS_mode mode) {
    req_set_mode request_set_mode;
    res_set_mode response_set_mode;

    request_set_mode.mode = static_cast<ATLAS_mode_t>(mode);
    if (PayloadGatekeeperTask->sendrecvPayload(request_set_mode.req_code, static_cast<void*>(&request_set_mode), static_cast<void*>(&response_set_mode))) {
        LOG_INFO<<"ATLAS mode set to: "<<response_set_mode.mode;
    } else {
        LOG_ERROR << "error at: request_set_mode ";
    }
}


void TestTask::getPayloadTelemetries() {
    req_set_time request_time;
    res_set_time response_time;

    req_get_ldd_telemetries request_gpo;
    res_get_ldd_telemetries response_gpo;

    req_get_mode request_get_mode;
    res_get_mode response_get_mode;

    req_get_main_telemetries request_get_main_telemetry;
    res_get_main_telemetries response_get_main_telemetry;

    request_time.timestamp = 1738070674;

    if (PayloadGatekeeperTask->sendrecvPayload(request_gpo.req_code, static_cast<void*>(&request_gpo), static_cast<void*>(&response_gpo))) {
        LOG_DEBUG << "LDD TM Amp out pow  :" << response_gpo.amplifier_output_power;
        LOG_DEBUG << "LDD TM ld temp      :" << response_gpo.ld_temperature;
        LOG_DEBUG << "LDD TM Time (s)     :" << response_gpo.time;
    } else {
        LOG_ERROR << "error at: request_gpo ";
    }

    if (PayloadGatekeeperTask->sendrecvPayload(request_time.req_code, static_cast<void*>(&request_time), static_cast<void*>(&response_time))) {
        LOG_DEBUG << "Payload Responded with time: " << response_time.timestamp << " Status: " << response_time.status;
    } else {
        LOG_ERROR << "error at: request_time ";
    }

    if (PayloadGatekeeperTask->sendrecvPayload(request_get_mode.req_code, static_cast<void*>(&request_get_mode), static_cast<void*>(&response_get_mode))) {
        LOG_INFO << "GET MODE : " << response_get_mode.mode;
    } else {
        LOG_ERROR << "error at: response_get_mode ";
    }

    if (PayloadGatekeeperTask->sendrecvPayload(request_get_main_telemetry.req_code, static_cast<void*>(&request_get_main_telemetry), static_cast<void*>(&response_get_main_telemetry))) {
        LOG_INFO << "Get uptime_LSB : " << response_get_main_telemetry.uptime_LSB;
        LOG_INFO << "Get uptime_MSB : " << response_get_main_telemetry.uptime_MSB;
        LOG_INFO << "Get time : " << response_get_main_telemetry.time;
        LOG_INFO << "Get psu_12V : " << response_get_main_telemetry.psu_12V;
        LOG_INFO << "Get psu_5V : " << response_get_main_telemetry.psu_5V;
        LOG_INFO << "Get psu_33V : " << response_get_main_telemetry.psu_33V;
        LOG_INFO << "Get MCU_Die_Temp : " << response_get_main_telemetry.MCU_Die_Temp;
        LOG_INFO << "Get Main_Board_Temp : " << response_get_main_telemetry.Main_Board_Temp;
        LOG_INFO << "Get Seed_ld_output_pow : " << response_get_main_telemetry.Seed_ld_output_pow;
        LOG_INFO << "Get FSM_Chamber_Temp : " << response_get_main_telemetry.FSM_Chamber_Temp;
        LOG_INFO << "Get FSM_Chamber_Pressure : " << response_get_main_telemetry.FSM_Chamber_Pressure;
        LOG_INFO << "Get FSMD_Voltage_Converter_Temp : " << response_get_main_telemetry.FSMD_Voltage_Converter_Temp;
        LOG_INFO << "Get FSMD_Drivers_Temp : " << response_get_main_telemetry.FSMD_Drivers_Temp;
        LOG_INFO << "Get Camera_PCB_Temp : " << response_get_main_telemetry.Camera_PCB_Temp;
        LOG_INFO << "Get FSO_AUX_Temp_1 : " << response_get_main_telemetry.FSO_AUX_Temp_1;
        LOG_INFO << "Get FSO_AUX_Temp_2 : " << response_get_main_telemetry.FSO_AUX_Temp_2;
        LOG_INFO << "Get ldd_12V_Curr : " << response_get_main_telemetry.ldd_12V_Curr;
        LOG_INFO << "Get FSM_Driver_12V_Curr : " << response_get_main_telemetry.FSM_Driver_12V_Curr;
        LOG_INFO << "Get Flashes_33v_Curr : " << response_get_main_telemetry.Flashes_33v_Curr;
        LOG_INFO << "Get FPGA_5V_Curr : " << response_get_main_telemetry.FPGA_5V_Curr;
        LOG_INFO << "Get SDD_33V_Curr : " << response_get_main_telemetry.SDD_33V_Curr;
        LOG_INFO << "Get MCU_33V_Curr : " << response_get_main_telemetry.MCU_33V_Curr;
        LOG_INFO << "Get Seed_Diode_Bias_Curr : " << response_get_main_telemetry.Seed_Diode_Bias_Curr;
        LOG_INFO << "Get Tec_Curr : " << response_get_main_telemetry.Tec_Curr;
        LOG_INFO << "Get Sd_Temp_Violations : " << response_get_main_telemetry.Sd_Temp_Violations;
        LOG_INFO << "Get end_uptime_LSB : " << response_get_main_telemetry.end_uptime_LSB;
        LOG_INFO << "Get end_uptime_MSB : " << response_get_main_telemetry.end_uptime_MSB;
    } else {
        LOG_ERROR << "error at: request_get_main_telemetry ";
    }
}
void TestTask::testPayload() {

    req_file_write request_file_write;
    res_file_write response_file_write;

    req_file_read request_file_read;
    res_file_read response_file_read;

    req_file_get_size request_file_get_size;
    res_file_get_size response_file_get_size;

    req_file_delete request_file_delete;
    res_file_delete response_file_delete;


    request_file_write.file_descriptor = 0;
    request_file_write.offset = 0;
    request_file_write.size = 2030;

    request_file_read.file_descriptor = 0;
    request_file_read.offset = 11;
    request_file_read.size = 21;

    request_file_delete.file_descriptor = 0;

    request_file_get_size.file_descriptor = 0;


    // getPayloadTelemetries();

    if (PayloadGatekeeperTask->sendrecvPayload(request_file_delete.req_code, static_cast<void*>(&request_file_delete), static_cast<void*>(&response_file_delete))) {
        LOG_DEBUG << "Delete file: " << response_file_delete.file_descriptor;
        LOG_DEBUG << "Delete file status: " << response_file_delete.status;
    } else {
        LOG_ERROR << "error at: request_file_delete ";
    }

    vTaskDelay(pdMS_TO_TICKS(100));

    PayloadGatekeeperTask->uploadPayloadFile(request_file_write.req_code, request_file_write, response_file_write);

    vTaskDelay(pdMS_TO_TICKS(100));
    if (PayloadGatekeeperTask->sendrecvPayload(request_file_read.req_code, static_cast<void*>(&request_file_read), static_cast<void*>(&response_file_read))) {
        LOG_DEBUG << "READ FD    : " << response_file_read.file_descriptor;
        LOG_DEBUG << "READ offset: " << response_file_read.offset;
        LOG_DEBUG << "READ size  : " << response_file_read.size;
    } else {
        LOG_ERROR << "error at: request_file_read ";
    }
    vTaskDelay(pdMS_TO_TICKS(1000));


    if (PayloadGatekeeperTask->sendrecvPayload(request_file_get_size.req_code, static_cast<void*>(&request_file_get_size), static_cast<void*>(&response_file_get_size))) {
        LOG_DEBUG << "size" << response_file_get_size.size;
        LOG_DEBUG << "FD: " << response_file_get_size.file_descriptor;
    } else {
        LOG_ERROR << "error at: request_file_get_size ";
    }

    vTaskDelay(pdMS_TO_TICKS(100));

    request_file_read.file_descriptor = 0;
    request_file_read.offset=0;
    request_file_read.size=0;
    PayloadGatekeeperTask->downloadPayloadFile(request_file_read.req_code, request_file_read, response_file_read);
}

TestTask::TestTask() : Task("TestTask") {
    // LOG_INFO << "Initialised instance of TestTask";
}

void TestTask::execute() {
    req_capture_images request_capture_images;
    res_capture_images response_capture_images;

    req_file_read request_file_read;
    res_file_read response_file_read;

    vTaskDelay(pdMS_TO_TICKS(this->delayMs));

    // testPayload();

    request_capture_images.count=1;


    request_capture_images.size = 0x00; // full frame
    request_capture_images.type = 0x00; // raw

    request_capture_images.skip_count = 0x01;

    request_file_read.offset=0;
    request_file_read.size=0;


    // LOG_INFO<<"Set mode to IDLE";
    changePayloadMode(ATLAS_mode::IDLE);
    vTaskDelay(pdMS_TO_TICKS(20000));
    // LOG_INFO<<"Set mode to PREHEAT";
    // changePayloadMode(ATLAS_mode::PREHEAT);
    // vTaskDelay(pdMS_TO_TICKS(180000));
    // LOG_INFO<<"Set mode to TRANSIEVE";
    // changePayloadMode(ATLAS_mode::TRANSIEVE);

    // vTaskDelay(pdMS_TO_TICKS(10000));
    request_file_read.file_descriptor = 0x50;
    // const auto result = PayloadGatekeeperTask->takePayloadImage(request_capture_images.req_code, request_capture_images, response_capture_images);
    // LOG_INFO<<"takePayloadImage result: "<<result;
    // vTaskDelay(pdMS_TO_TICKS(60000));
    // LOG_INFO<<"Set mode to IDLE";
    // changePayloadMode(ATLAS_mode::IDLE);

    PayloadGatekeeperTask->downloadPayloadFile(request_file_read.req_code, request_file_read,response_file_read);



    while (true) {

        // getPayloadTelemetries();

        vTaskDelay(pdMS_TO_TICKS(this->delayMs));
    }
}