//
// Created by tats on 10/7/2024.
//

#pragma once

#include "FreeRTOS.h"
#include "TaskConfigs.hpp"
#include "task.h"
#include "queue.h"
#include "Logger.hpp"
#include "plib_tc0.h"
#include "general_definitions.hpp"
#include "ATLAS_Frame_Definitions.hpp"
#include "ATLAS_payload.hpp"
#include "ATLAS_Response_Structs.hpp"
#include "ATLAS_Request_Structs.hpp"
#include "ATLAS_Response_Handlers.hpp"
#include "ATLAS_Request_Handlers.hpp"

class PayloadGatekeeperTask : public Task {
private:

    StackType_t taskStack[PayloadGatekeeperTaskStack];

    static const uint8_t maxFrameQueueSize = 5;
    static const uint16_t maxReadDelayms = 2000;

    StaticQueue_t xPayloadSendFrameQueue;
    QueueHandle_t xFrameSendQueueHandle;
    uint8_t payloadSendFrameQueueStorage[maxFrameQueueSize*(payload_size_size+max_payload_size)];

    StaticQueue_t xPayloadReceiveFrameQueue;
    QueueHandle_t xFrameReceiveQueueHandle;
    uint8_t payloadReceiveFrameQueueStorage[maxFrameQueueSize*(payload_size_size+max_payload_size)];

    uint8_t internal_buffer[payload_size_size+max_payload_size] = {};

    uint8_t error = 0;

public:


    void execute();

    PayloadGatekeeperTask();

    /**
     * Create freeRTOS Task
     */
    void createTask() {
        taskHandle = xTaskCreateStatic(vClassTask<PayloadGatekeeperTask>, this->TaskName, PayloadGatekeeperTaskStack, this,
                          PayloadGatekeeperTaskPriority, this->taskStack, &(this->taskBuffer));
    }

    bool addPayloadSendQueue(uint8_t* payload, uint16_t size, bool isISR);

    bool addPayloadReceiveQueue(uint8_t* payload, uint16_t size, bool isISR);

    void setPayloadError(uint8_t error_code, bool isISR);

    bool handlePayloadResponse(uint8_t command_code, uint8_t* payload, void* response_struct);

    uint16_t handlePayloadRequest(uint8_t command_code, void* request_struct, uint8_t* buffer, uint16_t &delay);

    bool sendrecvPayload(uint8_t command_code, void* request_struct, void* response_struct);

    bool uploadPayloadFile(uint8_t command_code, void* request_struct, void* response_struct);

};

inline std::optional<PayloadGatekeeperTask> PayloadGatekeeperTask;
