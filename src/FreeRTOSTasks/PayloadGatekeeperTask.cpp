//
// Created by tats on 10/7/2024.
//

#include "PayloadGatekeeperTask.hpp"

void printError(uint8_t error){
    switch (error) {
        case (uint8_t) ATLAS_Driver_Error::NONE:
            LOG_DEBUG<<"No Error";
            break;
        case (uint8_t) ATLAS_Driver_Error::TIMEOUT:
            LOG_DEBUG<<"TIMEOUT";
            break;
        case (uint8_t) ATLAS_Driver_Error::SIZE_OVERFLOW:
            LOG_DEBUG<<"SIZE_OVERFLOW";
            break;
        case (uint8_t) ATLAS_Driver_Error::RECEIVE_ERROR:
            LOG_DEBUG<<"RECEIVE_ERROR";
            break;
        case (uint8_t) ATLAS_Driver_Error::TRANSMIT_ERROR:
            LOG_DEBUG<<"TRANSMIT_ERROR";
            break;
        case (uint8_t) ATLAS_Driver_Error::CRC_MISMATCH:
            LOG_DEBUG<<"CRC_MISMATCH";
            break;
        default:
            LOG_DEBUG<<"Unknown payload error code: "<< error;
            break;
    }
}


PayloadGatekeeperTask::PayloadGatekeeperTask() : Task("Payload Gatekeeper") {
    // Initialize send queue
    xFrameSendQueueHandle = xQueueCreateStatic(maxFrameQueueSize, (payload_size_size+max_payload_size), payloadSendFrameQueueStorage, &xPayloadSendFrameQueue);
    // Initialize receive queue
    xFrameReceiveQueueHandle = xQueueCreateStatic(maxFrameQueueSize, (payload_size_size+max_payload_size), payloadReceiveFrameQueueStorage, &xPayloadReceiveFrameQueue);
    // Initialize communications
    setupATLASCommunications();
}

void PayloadGatekeeperTask::execute() {

    LOG_DEBUG << "Runtime init: " << this->TaskName;
    vTaskDelay(pdMS_TO_TICKS(2000));

    uint32_t ulNotifiedValue;

    while (true) {

        xTaskNotifyWait(0, 0, &ulNotifiedValue, portMAX_DELAY);

        switch (ulNotifiedValue) {
            case PAYLOAD_RCV:
                // Woken to receive
                while(xQueueReceive(xFrameReceiveQueueHandle, internal_buffer, 0) == pdTRUE){
                    uint16_t rcv_size = ((uint16_t) internal_buffer[1]<<8 | internal_buffer[0]);    // LSB first
                    LOG_DEBUG<<"Received payload msg, payload_size:"<<rcv_size;
                }
                break;
            case PAYLOAD_SND:
                // Woken to transmit
                while(xQueueReceive(xFrameSendQueueHandle, internal_buffer, 0) == pdTRUE){
                    uint16_t snd_size = ((uint16_t) internal_buffer[1]<<8 | internal_buffer[0]);    // LSB first
                    LOG_DEBUG<<"Sending payload msg, payload_size:"<<snd_size;
                    sendPayloadMessage(&internal_buffer[2], snd_size);
                }
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