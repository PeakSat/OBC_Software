//
// Created by tats on 10/7/2024.
//

#include "PayloadGatekeeperTask.hpp"

#include <binary_sw1.hpp>
#include <etl/vector.h>
#include <sys/_intsup.h>


void responseTimerCallback(TC_TIMER_STATUS status, uintptr_t context) {
    PayloadGatekeeperTask->setPayloadError(static_cast<uint8_t>(ATLAS_Driver_Error::TIMEOUT), true);
    TC0_CH2_TimerStop();
}


void printError(uint8_t error) {
    switch (error) {
        case static_cast<uint8_t>(ATLAS_Driver_Error::NONE):
            LOG_DEBUG << "No Error";
            break;
        case static_cast<uint8_t>(ATLAS_Driver_Error::TIMEOUT):
            LOG_DEBUG << "PAYLOAD TIMEOUT";
            break;
        case static_cast<uint8_t>(ATLAS_Driver_Error::SIZE_OVERFLOW):
            LOG_DEBUG << "PAYLOAD SIZE_OVERFLOW";
            break;
        case static_cast<uint8_t>(ATLAS_Driver_Error::RECEIVE_ERROR):
            LOG_DEBUG << "PAYLOAD RECEIVE_ERROR";
            break;
        case static_cast<uint8_t>(ATLAS_Driver_Error::TRANSMIT_ERROR):
            LOG_DEBUG << "PAYLOAD TRANSMIT_ERROR";
            break;
        case static_cast<uint8_t>(ATLAS_Driver_Error::CRC_MISMATCH):
            LOG_DEBUG << "PAYLOAD CRC_MISMATCH";
            break;
        default:
            LOG_DEBUG << "Unknown payload error code: " << error;
            break;
    }
}


bool PayloadGatekeeperTask::handlePayloadResponse(uint8_t command_code, uint8_t* payload, void* response_struct) {
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
        case FILE_GET_REGION_CRC:
            return handleResFileGetRegionCRC(payload, response_struct);
        case FILE_DELETE:
            return handleResFileDelete(payload, response_struct);
        case CAPTURE_IMAGES:
            return handleResCaptureImages(payload, response_struct);
        case CAPTURE_IMAGES_STATUS:
            return handleResCaptureImagesStatus(payload, response_struct);
        case PREPARE_IMAGES_FOR_DOWNLOAD:
            return handleResPrepareImagesForDownload(payload, response_struct);
        case PREPARE_IMAGES_FOR_DOWNLOAD_STATUS:
            return handleResPrepareImagesForDownloadStatus(payload, response_struct);
        default:
            break;
    }
    return false;
}

uint16_t PayloadGatekeeperTask::handlePayloadRequest(uint8_t command_code, void* request_struct, uint8_t* buffer, uint16_t& wr_delay) {
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
        case FILE_GET_REGION_CRC:
            return handleReqFileGetRegionCRC(request_struct, buffer, wr_delay);
        case FILE_DELETE:
            return handleReqFileDelete(request_struct, buffer, wr_delay);
        case CAPTURE_IMAGES:
            return handleReqCaptureImage(request_struct, buffer, wr_delay);
        case CAPTURE_IMAGES_STATUS:
            return handleReqCaptureImageStatus(request_struct, buffer, wr_delay);
        case PREPARE_IMAGES_FOR_DOWNLOAD:
            return handleReqPrepareImagesForDownload(request_struct, buffer, wr_delay);
        case PREPARE_IMAGES_FOR_DOWNLOAD_STATUS:
            return handleReqPrepareImagesForDownloadStatus(request_struct, buffer, wr_delay);
        default:
            break;
    }
    return 0;
}

bool PayloadGatekeeperTask::sendrecvPayload(uint8_t command_code, void* request_struct, void* response_struct) {
    LOG_DEBUG << "Sending payload msg, CC: " << command_code;

    uint8_t payload_buffer[max_payload_size] = {};
    uint16_t payload_size = 0;
    uint16_t write_read_delay = 500;

    payload_size = handlePayloadRequest(command_code, request_struct, payload_buffer, write_read_delay);

    sendPayloadMessage(payload_buffer, payload_size);
    vTaskDelay(pdMS_TO_TICKS(write_read_delay));

    if (xQueueReceive(xFrameReceiveQueueHandle, internal_buffer, pdMS_TO_TICKS(maxReadDelayms)) == pdTRUE) {
        uint16_t rcv_size = (static_cast<uint16_t>(internal_buffer[1]) << 8 | internal_buffer[0]); // LSB first
        LOG_DEBUG << "Received response size: " << rcv_size;
        if (handlePayloadResponse(command_code, &internal_buffer[payload_size_size], response_struct)) {
            return true;
        }
        setPayloadError(static_cast<uint8_t>(ATLAS_Driver_Error::ANSWER_MISMATCH), false);
        return false;
    }
    setPayloadError(static_cast<uint8_t>(ATLAS_Driver_Error::TIMEOUT), false);
    return false;
}


bool PayloadGatekeeperTask::addPayloadSendQueue(uint8_t* payload, uint16_t size, bool isISR) {
    internal_buffer[0] = size & 0xFF;
    internal_buffer[1] = (size >> 8) & 0xFF;
    memcpy(&internal_buffer[2], payload, size);

    if (isISR) {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        xQueueSendToBackFromISR(xFrameSendQueueHandle, internal_buffer, &xHigherPriorityTaskWoken);
        xTaskNotifyFromISR(taskHandle, PAYLOAD_SND, eSetValueWithOverwrite, &xHigherPriorityTaskWoken);
        return true;
    }

    if (xQueueSendToBack(xFrameSendQueueHandle, internal_buffer, 0) == pdTRUE) {
        xTaskNotify(taskHandle, PAYLOAD_SND, eSetValueWithOverwrite);
        return true;
    }
    // xTaskNotify(this->taskHandle, PAYLOAD_SND, eNoAction);
    return false;
}

bool PayloadGatekeeperTask::addPayloadReceiveQueue(uint8_t* payload, uint16_t size, bool isISR) {
    internal_buffer[0] = size & 0xFF;
    internal_buffer[1] = (size >> 8) & 0xFF;
    memcpy(&internal_buffer[2], payload, size);

    if (isISR) {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        xQueueSendToBackFromISR(xFrameReceiveQueueHandle, internal_buffer, &xHigherPriorityTaskWoken);
        //        xTaskNotifyFromISR(taskHandle, PAYLOAD_RCV, eSetValueWithOverwrite, &xHigherPriorityTaskWoken);
        return true;
    }

    if (xQueueSendToBack(xFrameReceiveQueueHandle, internal_buffer, 0) == pdTRUE) {
        //        xTaskNotify(taskHandle, PAYLOAD_RCV, eSetValueWithOverwrite);
        return true;
    }
    // xTaskNotify(taskHandle, PAYLOAD_RCV, eNoAction);
    return false;
}

void PayloadGatekeeperTask::setPayloadError(uint8_t error_code, bool isISR) {
    error = (uint8_t) error_code;
    if (isISR) {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        xTaskNotifyFromISR(taskHandle, PAYLOAD_ERR, eSetValueWithOverwrite, &xHigherPriorityTaskWoken);
        return;
    }
    xTaskNotify(taskHandle, PAYLOAD_ERR, eSetValueWithOverwrite);
}

bool PayloadGatekeeperTask::uploadPayloadFile(const uint8_t command_code, req_file_write request_struct, res_file_write response_struct) {
    LOG_DEBUG << "Sending payload msg, CC: " << command_code;

    constexpr int32_t file_size = 74048;
    constexpr int32_t maxChunkSize = sizeof(request_struct.data);
    etl::vector<int32_t, 255> failedOffsets{};

    req_file_get_region_crc requestGetFileRegionCRCStruct;
    res_file_get_region_crc responseGetFileRegionCRCStruct;

    requestGetFileRegionCRCStruct.file_descriptor = request_struct.file_descriptor;
    requestGetFileRegionCRCStruct.offset = request_struct.offset;
    requestGetFileRegionCRCStruct.size = file_size;

    bool result = false;
    for (int32_t offset = 0; offset < file_size; offset += maxChunkSize) {
        const int32_t chunk_size = (file_size - offset > maxChunkSize) ? maxChunkSize : (file_size - offset);
        request_struct.size = chunk_size;
        request_struct.offset = offset;
        memcpy(request_struct.data, const_cast<const uint8_t*>(&firmware_data[offset]), static_cast<size_t>(chunk_size));
        LOG_DEBUG << "Chunk #" << (offset / maxChunkSize) + 1 << " | Offset: " << offset << " | Chunk Size: " << chunk_size;
        if (not this->sendrecvPayload(request_struct.req_code, static_cast<void*>(&request_struct), static_cast<void*>(&response_struct))) {
            LOG_ERROR << "Writing failed at offset: " << offset;
            failedOffsets.push_back(offset);
        }
    }
    const auto localRegionChecksum = crc32(const_cast<uint8_t*>(&firmware_data[0]), file_size);

    // while (not failedOffsets.empty()) {
    for (etl::vector<int32_t, 255>::iterator it = failedOffsets.begin(); it != failedOffsets.end();) {
        const int32_t offset = *it;
        const int32_t chunk_size = (file_size - offset > maxChunkSize) ? maxChunkSize : (file_size - offset);
        request_struct.offset = offset;

        memcpy(request_struct.data, const_cast<const uint8_t*>(&firmware_data[offset]), static_cast<size_t>(chunk_size));

        if (this->sendrecvPayload(request_struct.req_code, static_cast<void*>(&request_struct), static_cast<void*>(&response_struct))) {
            it = failedOffsets.erase(it); // Move iterator to next valid position
        } else {
            LOG_ERROR << "Retry failed at offset: " << offset;
            ++it; // Only move forward if retry failed
        }
    }
    // }

    if (not this->sendrecvPayload(requestGetFileRegionCRCStruct.req_code, static_cast<void*>(&requestGetFileRegionCRCStruct),
                                  static_cast<void*>(&responseGetFileRegionCRCStruct))) {
        LOG_ERROR << "CRC failed with status: " << responseGetFileRegionCRCStruct.status;
    }

    LOG_INFO << "Local crc: " << localRegionChecksum;
    LOG_INFO << "Response crc: " << responseGetFileRegionCRCStruct.checksum;

    result = (responseGetFileRegionCRCStruct.checksum == localRegionChecksum);

    return result;
}

bool PayloadGatekeeperTask::downloadPayloadFile(const uint8_t command_code, req_file_read request_struct, res_file_read response_struct) {
    constexpr int32_t maxChunkSize = sizeof(response_struct.data);
    // etl::vector<int32_t, 255> failedOffsets{};
    bool result = false;

    req_file_get_size requestGetFileSize;
    res_file_get_size responseGetFileSize;
    req_file_get_region_crc requestGetFileRegionCRCStruct;
    res_file_get_region_crc responseGetFileRegionCRCStruct;

    requestGetFileRegionCRCStruct.file_descriptor = request_struct.file_descriptor;
    requestGetFileRegionCRCStruct.offset = request_struct.offset;
    requestGetFileRegionCRCStruct.size = request_struct.size;

    int32_t file_size = 0U;

    requestGetFileSize.file_descriptor = request_struct.file_descriptor;

    LOG_DEBUG << "Sending payload msg, CC: " << command_code;
    // acquire file size
    if (this->sendrecvPayload(requestGetFileSize.req_code, static_cast<void*>(&requestGetFileSize), static_cast<void*>(&responseGetFileSize))) {
        if (responseGetFileSize.size >= 0) {
            LOG_DEBUG << "Request File Size: " <<  responseGetFileSize.size;
            file_size = responseGetFileSize.size;
        } else {
            LOG_DEBUG << "Request File Size Error at file: " << requestGetFileSize.file_descriptor;
            return false;
        }

    } else {
        LOG_ERROR << "Request File Size failed";
        return false;
    }

    for (int32_t offset = 0; offset < file_size; offset += maxChunkSize) {
        const int32_t chunk_size = (file_size - offset > maxChunkSize) ? maxChunkSize : (file_size - offset);
        request_struct.offset = offset;
        requestGetFileRegionCRCStruct.offset = offset;
        requestGetFileRegionCRCStruct.size = chunk_size;
        request_struct.size = chunk_size;
        LOG_DEBUG << "Chunk #" << (offset / maxChunkSize) + 1 << " | Offset: " << offset << " | Chunk Size: " << chunk_size;
        if (this->sendrecvPayload(request_struct.req_code, static_cast<void*>(&request_struct), static_cast<void*>(&response_struct))) {
            LOG_DEBUG << "Data acquired from offset: " << offset;
            // memcpy(response_struct.data, const_cast<const uint8_t*>(&firmware_data[offset]), static_cast<size_t>(chunk_size));

            LOG_DEBUG << "----DATA FRAME START----" ;
            for(int j = 0; j < chunk_size; j++) {
                LOG_DEBUG<<response_struct.data[j];
            }
            LOG_DEBUG << "----DATA FRAME END----" ;

            const auto localRegionChecksum = crc32(const_cast<uint8_t*>(&response_struct.data[0]), chunk_size);

            //request region CRC and compare with local CRC
            if (not this->sendrecvPayload(requestGetFileRegionCRCStruct.req_code, static_cast<void*>(&requestGetFileRegionCRCStruct),
                                  static_cast<void*>(&responseGetFileRegionCRCStruct))) {
                LOG_ERROR << "CRC failed with status: " << responseGetFileRegionCRCStruct.status;
                                  }
            LOG_INFO << "Local crc: " << localRegionChecksum;
            LOG_INFO << "Response crc: " << responseGetFileRegionCRCStruct.checksum;
            result = (responseGetFileRegionCRCStruct.checksum == localRegionChecksum);

        } else {
            LOG_ERROR << "Reading failed at offset: " << offset;
            // failedOffsets.push_back(offset);
            result = false;
        }
    }
    return result;
}

int8_t PayloadGatekeeperTask::takePayloadImage(uint8_t command_code, req_capture_images request_struct, res_capture_images response_struct) {

    bool ImageCaptureResult = false;
    bool ImageDownloadCommandResult = false;
    bool imageDownloadComplete = false;

    constexpr int maxAttempts = 20;

    int8_t stored_image_fd = 127;

    req_get_mode request_get_mode;
    res_get_mode response_get_mode;

    req_capture_images_status requestGetImageStatus;
    res_capture_images_status responseGetImageStatus;

    req_prepare_images_for_download request_prepare_images_for_download;
    res_prepare_images_for_download response_prepare_images_for_download;
    request_prepare_images_for_download.image_number=0;

    req_prepare_images_for_download_status request_prepare_images_for_download_status;
    res_prepare_images_for_download_status response_prepare_images_for_download_status;
    request_prepare_images_for_download_status.image_number = 0;

    // check if in correct mode
    if (this->sendrecvPayload(request_get_mode.req_code, static_cast<void*>(&request_get_mode), static_cast<void*>(&response_get_mode))) {
        LOG_INFO << "GET MODE : " << response_get_mode.mode;
        if (response_get_mode.mode!= static_cast<ATLAS_mode_t>(ATLAS_mode::TRANSIEVE)) {
            LOG_DEBUG<< "Attempted to issue capture image command, but ATLAS is not in TRANSIEVE mode";
            return false;
        }
    } else {
        LOG_ERROR << "error at: response_get_mode ";
    }

    response_struct.status = 0;
    // issue command to capture image
    if (not this->sendrecvPayload(request_struct.req_code, static_cast<void*>(&request_struct), static_cast<void*>(&response_struct))) {
        LOG_ERROR << "Capture Image command failed with error: " << response_struct.status;
    }

    // poll image capturing status
   for (int i  =  0; i < maxAttempts; i++) {
        if (not this->sendrecvPayload(requestGetImageStatus.req_code,
                                      static_cast<void*>(&requestGetImageStatus), static_cast<void*>(&responseGetImageStatus))) {
            LOG_ERROR << "responseGetImageStatus command failed";
        }
        LOG_DEBUG << "Image capture progress: " << responseGetImageStatus.progress ;
        LOG_DEBUG << "Image capture status: " << responseGetImageStatus.status ;
        if (responseGetImageStatus.progress == response_struct.count) {
            LOG_DEBUG << "Image has been captured, prepare for download...";
            ImageCaptureResult = true;
            break;
        }
        vTaskDelay(pdMS_TO_TICKS(100));
    }

    // issue command to download image to ATLAS
    if (ImageCaptureResult) {
        if (not this->sendrecvPayload(request_prepare_images_for_download.req_code,
                                      static_cast<void*>(&request_prepare_images_for_download), static_cast<void*>(&response_prepare_images_for_download))) {
            LOG_ERROR << "request_prepare_images_for_download command failed";
        } else {
            ImageDownloadCommandResult = true;
        }
    }
    if (ImageDownloadCommandResult) {
        // poll image download status
        while (not imageDownloadComplete){
            if (not this->sendrecvPayload(request_prepare_images_for_download_status.req_code,
                                          static_cast<void*>(&request_prepare_images_for_download_status), static_cast<void*>(&response_prepare_images_for_download_status))) {
                LOG_ERROR << "responseGetImageStatus command failed";
            }
            LOG_INFO<< "Download at: "<< response_prepare_images_for_download_status.status <<"%";
            if (response_prepare_images_for_download_status.status == 100||response_prepare_images_for_download_status.status == 99) {
                LOG_DEBUG << "Image has downloaded at File: "<< response_prepare_images_for_download_status.file_descriptor;
                stored_image_fd = response_prepare_images_for_download_status.file_descriptor;
                imageDownloadComplete= true;
                // break;
            }
            vTaskDelay(pdMS_TO_TICKS(100));
        }
        /** placeholder for read downloaded file **/
    }

    return stored_image_fd;
}

PayloadGatekeeperTask::PayloadGatekeeperTask() : Task("Payload Gatekeeper") {
    // Initialize send queue
    xFrameSendQueueHandle = xQueueCreateStatic(maxFrameQueueSize, (payload_size_size + max_payload_size), payloadSendFrameQueueStorage, &xPayloadSendFrameQueue);
    // Initialize receive queue
    xFrameReceiveQueueHandle = xQueueCreateStatic(maxFrameQueueSize, (payload_size_size + max_payload_size), payloadReceiveFrameQueueStorage, &xPayloadReceiveFrameQueue);
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
                while (xQueueReceive(xFrameReceiveQueueHandle, internal_buffer, 0) == pdTRUE) {
                    uint16_t rcv_size = (static_cast<uint16_t>(internal_buffer[1]) << 8 | internal_buffer[0]); // LSB first
                    LOG_DEBUG << "Received payload msg, payload_size:" << rcv_size;
                }
                break;
            case PAYLOAD_SND:
                // Woken to transmit
                while (xQueueReceive(xFrameSendQueueHandle, internal_buffer, 0) == pdTRUE) {
                    uint16_t snd_size = (static_cast<uint16_t>(internal_buffer[1]) << 8 | internal_buffer[0]); // LSB first
                    LOG_DEBUG << "Sending payload msg, payload_size:" << snd_size;
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