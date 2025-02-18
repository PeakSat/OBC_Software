//
// Created by tats on 10/7/2024.
//

#include "PayloadGatekeeperTask.hpp"

#include <binary_sw1.hpp>
#include <etl/vector.h>


void responseTimerCallback(TC_TIMER_STATUS status, uintptr_t context){
    PayloadGatekeeperTask->setPayloadError(static_cast<uint8_t>(ATLAS_Driver_Error::TIMEOUT), true);
    TC0_CH2_TimerStop();
}


void printError(uint8_t error){
    switch (error) {
        case static_cast<uint8_t>(ATLAS_Driver_Error::NONE):
            LOG_DEBUG<<"No Error";
            break;
        case static_cast<uint8_t>(ATLAS_Driver_Error::TIMEOUT):
            LOG_DEBUG<<"PAYLOAD TIMEOUT";
            break;
        case static_cast<uint8_t>(ATLAS_Driver_Error::SIZE_OVERFLOW):
            LOG_DEBUG<<"PAYLOAD SIZE_OVERFLOW";
            break;
        case static_cast<uint8_t>(ATLAS_Driver_Error::RECEIVE_ERROR):
            LOG_DEBUG<<"PAYLOAD RECEIVE_ERROR";
            break;
        case static_cast<uint8_t>(ATLAS_Driver_Error::TRANSMIT_ERROR):
            LOG_DEBUG<<"PAYLOAD TRANSMIT_ERROR";
            break;
        case static_cast<uint8_t>(ATLAS_Driver_Error::CRC_MISMATCH):
            LOG_DEBUG<<"PAYLOAD CRC_MISMATCH";
            break;
        default:
            LOG_DEBUG<<"Unknown payload error code: "<< error;
            break;
    }
}


bool PayloadGatekeeperTask::handlePayloadResponse(uint8_t command_code, uint8_t* payload, void* response_struct){
    switch (command_code) {
        case GET_MODE:
            return handleResGetMode(payload, response_struct);
        case SET_MODE:
            return handleResSetMode(payload, response_struct);
        case GET_TIME:
            return handleResGetTime(payload, response_struct);
        case SET_TIME:
            return handleResSetTime(payload, response_struct);
        case GET_MAIN_TELEMETRIES:
            return handleResGetMainTelemetries(payload, response_struct);
        case GET_SECONDARY_TELEMETRIES:
            return handleResGetSecondaryTelemetries(payload, response_struct);
        case GET_LDD_TELEMETRIES:
            return handleResGetLddTelemetries(payload, response_struct);
        case GET_FPGA_TELEMETRIES:
            return handleResGetFpgaTelemetries(payload, response_struct);
        case GET_POINTING_OFFSET:
            return handleResGetPointingOffset(payload, response_struct);
        case FILE_WRITE:
            return handleResFileWrite(payload, response_struct);
        case FILE_READ:
            return handleResFileRead(payload, response_struct);
        case FILE_GET_SIZE:
            return handleResFileGetSize(payload, response_struct);
        case FILE_DELETE:
            return handleResFileDelete(payload, response_struct);
        default:
            break;
    }
    return false;
}

uint16_t PayloadGatekeeperTask::handlePayloadRequest(uint8_t command_code, void *request_struct, uint8_t* buffer, uint16_t &wr_delay) {
    switch (command_code) {
        case GET_MODE:
            return handleReqGetMode(request_struct, buffer, wr_delay);
        case SET_MODE:
            return handleReqSetMode(request_struct, buffer, wr_delay);
        case GET_TIME:
            return handleReqGetTime(request_struct, buffer, wr_delay);
        case SET_TIME:
            return handleReqSetTime(request_struct, buffer, wr_delay);
        case GET_MAIN_TELEMETRIES:
            return handleReqGetMainTelemetries(request_struct, buffer, wr_delay);
        case GET_SECONDARY_TELEMETRIES:
            return handleReqGetSecondaryTelemetries(request_struct, buffer, wr_delay);
        case GET_LDD_TELEMETRIES:
            return handleReqGetLddTelemetries(request_struct, buffer, wr_delay);
        case GET_FPGA_TELEMETRIES:
            return handleReqGetFpgaTelemetries(request_struct, buffer, wr_delay);
        case GET_POINTING_OFFSET:
            return handleReqGetPointingOffset(request_struct, buffer, wr_delay);
        case FILE_WRITE:
            return handleReqFileWrite(request_struct, buffer, wr_delay);
        case FILE_READ:
            return handleReqFileRead(request_struct, buffer, wr_delay);
        case FILE_GET_SIZE:
            return handleReqFileGetSize(request_struct, buffer, wr_delay);
        case FILE_DELETE:
            return handleReqFileDelete(request_struct, buffer, wr_delay);
        default:
            break;
    }
    return 0;
}

bool PayloadGatekeeperTask::sendrecvPayload(uint8_t command_code, void* request_struct, void* response_struct){
    LOG_DEBUG<<"Sending payload msg, CC: "<<command_code;

    uint8_t payload_buffer[max_payload_size] = {};
    uint16_t payload_size = 0;
    uint16_t write_read_delay = 500;

    payload_size = handlePayloadRequest(command_code, request_struct, payload_buffer, write_read_delay);

    sendPayloadMessage(payload_buffer, payload_size);
    vTaskDelay(pdMS_TO_TICKS(write_read_delay));

    if(xQueueReceive(xFrameReceiveQueueHandle, internal_buffer, pdMS_TO_TICKS(maxReadDelayms)) == pdTRUE){
        uint16_t rcv_size = (static_cast<uint16_t>(internal_buffer[1]) <<8 | internal_buffer[0]);    // LSB first
        LOG_DEBUG<<"Received response size: "<<rcv_size;
        if(handlePayloadResponse(command_code, &internal_buffer[payload_size_size], response_struct)){
            for (int i = 0 ; i<payload_size_size; i++) {
                internal_buffer[i] = 0;
            }
            return true;
        }
        setPayloadError(static_cast<uint8_t>(ATLAS_Driver_Error::ANSWER_MISMATCH), false);
        return false;
    }
    setPayloadError(static_cast<uint8_t>(ATLAS_Driver_Error::TIMEOUT), false);
    return false;
}


bool PayloadGatekeeperTask::addPayloadSendQueue(uint8_t *payload, uint16_t size, bool isISR) {
    internal_buffer[0] = size & 0xFF;
    internal_buffer[1] = (size>>8) & 0xFF;
    memcpy(&internal_buffer[2], payload, size);

    if(isISR){
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        xQueueSendToBackFromISR(xFrameSendQueueHandle, internal_buffer, &xHigherPriorityTaskWoken);
        xTaskNotifyFromISR(taskHandle, PAYLOAD_SND, eSetValueWithOverwrite, &xHigherPriorityTaskWoken);
        return true;
    }

    if(xQueueSendToBack(xFrameSendQueueHandle, internal_buffer, 0) == pdTRUE){
        xTaskNotify(taskHandle, PAYLOAD_SND, eSetValueWithOverwrite);
        return true;
    }
    // xTaskNotify(this->taskHandle, PAYLOAD_SND, eNoAction);
    return false;
}

bool PayloadGatekeeperTask::addPayloadReceiveQueue(uint8_t *payload, uint16_t size, bool isISR) {
    internal_buffer[0] = size & 0xFF;
    internal_buffer[1] = (size>>8) & 0xFF;
    memcpy(&internal_buffer[2], payload, size);

    if(isISR){
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        xQueueSendToBackFromISR(xFrameReceiveQueueHandle, internal_buffer, &xHigherPriorityTaskWoken);
//        xTaskNotifyFromISR(taskHandle, PAYLOAD_RCV, eSetValueWithOverwrite, &xHigherPriorityTaskWoken);
        return true;
    }

    if(xQueueSendToBack(xFrameReceiveQueueHandle, internal_buffer, 0) == pdTRUE){
//        xTaskNotify(taskHandle, PAYLOAD_RCV, eSetValueWithOverwrite);
        return true;
    }
    // xTaskNotify(taskHandle, PAYLOAD_RCV, eNoAction);
    return false;
}

void PayloadGatekeeperTask::setPayloadError(uint8_t error_code, bool isISR){
    error = (uint8_t) error_code;
    if(isISR){
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        xTaskNotifyFromISR(taskHandle, PAYLOAD_ERR, eSetValueWithOverwrite, &xHigherPriorityTaskWoken);
        return;
    }
    xTaskNotify(taskHandle, PAYLOAD_ERR, eSetValueWithOverwrite);
}

bool PayloadGatekeeperTask::uploadPayloadFile(const uint8_t command_code, req_file_write request_struct, void* response_struct) {
    LOG_DEBUG<<"Sending payload msg, CC: "<< command_code;

    constexpr int32_t file_size = 74048;
    constexpr int32_t maxChunkSize = sizeof(request_struct.data);
    etl::vector<int32_t, 255> failedOffsets{};

    auto result = false;
    for (int32_t offset = 0; offset < file_size; offset += maxChunkSize) {
        const int32_t chunk_size = (file_size - offset > maxChunkSize) ? maxChunkSize : (file_size - offset);
        request_struct.offset = offset;
        memcpy(request_struct.data,  const_cast<const uint8_t* >(&firmware_data[offset]), chunk_size);

        if (not this->sendrecvPayload(request_struct.req_code, static_cast<void*>(&request_struct), static_cast<void*>(&response_struct))) {
            LOG_ERROR << "Retry failed at offset: " << offset;
            failedOffsets.push_back(offset);
            result = false;
        }
    }
    for (const int32_t offset : failedOffsets) {
        const int32_t chunk_size = (file_size - offset > maxChunkSize) ? maxChunkSize : (file_size - offset);
        request_struct.offset = offset;
        memcpy(request_struct.data,  const_cast<const uint8_t* >(&firmware_data[offset]), chunk_size);
        result =  this->sendrecvPayload(request_struct.req_code, static_cast<void*>(&request_struct), static_cast<void*>(&response_struct));
    }

    return result;
}



PayloadGatekeeperTask::PayloadGatekeeperTask() : Task("Payload Gatekeeper") {
    // Initialize send queue
    xFrameSendQueueHandle = xQueueCreateStatic(maxFrameQueueSize, (payload_size_size+max_payload_size), payloadSendFrameQueueStorage, &xPayloadSendFrameQueue);
    // Initialize receive queue
    xFrameReceiveQueueHandle = xQueueCreateStatic(maxFrameQueueSize, (payload_size_size+max_payload_size), payloadReceiveFrameQueueStorage, &xPayloadReceiveFrameQueue);
    // Initialize communications
    setupATLASCommunications();
    // Initialize response timer callback
    TC0_CH2_TimerCallbackRegister(responseTimerCallback, (uintptr_t) NULL);
}

void PayloadGatekeeperTask::execute() {

    LOG_DEBUG << "Runtime init: " << this->TaskName;
    vTaskDelay(pdMS_TO_TICKS(2000));

    uint32_t ulNotifiedValue = 0;

    while (true) {

        xTaskNotifyWait(0, 0, &ulNotifiedValue, portMAX_DELAY);

        switch (ulNotifiedValue) {
            case PAYLOAD_RCV:
                // Woken to receive
                // Stop any pending response timer
                TC0_CH2_TimerStop();
                while(xQueueReceive(xFrameReceiveQueueHandle, internal_buffer, 0) == pdTRUE){
                    uint16_t rcv_size = (static_cast<uint16_t>(internal_buffer[1]) <<8 | internal_buffer[0]);    // LSB first
                    LOG_DEBUG<<"Received payload msg, payload_size:"<<rcv_size;
                }
                break;
            case PAYLOAD_SND:
                // Woken to transmit
                while(xQueueReceive(xFrameSendQueueHandle, internal_buffer, 0) == pdTRUE){
                    uint16_t snd_size = (static_cast<uint16_t>(internal_buffer[1]) <<8 | internal_buffer[0]);    // LSB first
                    LOG_DEBUG<<"Sending payload msg, payload_size:"<<snd_size;
                    sendPayloadMessage(&internal_buffer[payload_size_size], snd_size);
                }
                // Start timer for callback
                TC0_CH2_TimerStart();
                break;
            case PAYLOAD_ERR:
                // Woken to handle Error
                printError(this->error);
                break;
            default:
                break;
        }
    }
}