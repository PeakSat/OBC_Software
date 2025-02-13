#include "CAN/TPProtocol.hpp"

#include <HeartbeatTask.hpp>

#include "CANGatekeeperTask.hpp"

using namespace CAN;

void TPProtocol::parseMessage(TPMessage& message) {
    uint8_t messageType = static_cast<Application::MessageIDs>(message.data[0]);
    switch (messageType) {
        case CAN::Application::SendParameters:
            CAN::Application::parseSendParametersMessage(message);
            break;
        case CAN::Application::RequestParameters:
            CAN::Application::parseRequestParametersMessage(message);
            break;
        case CAN::Application::PerformFunction:
            break; //todo: use ST[08] to execute the perform function command
        case CAN::Application::EventReport:
            break; //todo: use the Event Report service
        case CAN::Application::TMPacket:
            CAN::Application::parseTMMessage(message);
            break;
        case CAN::Application::TCPacket:
            CAN::Application::parseTCMessage(message);
            break;
        case CAN::Application::CCSDSPacket:
            break; //todo send this to comms? idk
        case CAN::Application::Ping: {
            auto senderID = static_cast<CAN::NodeIDs>(message.idInfo.sourceAddress);
            auto senderName = CAN::Application::nodeIdToString.at(senderID);
            LOG_DEBUG << "Received ping from " << senderName.c_str();
            CAN::Application::sendPongMessage();
        } break;
        case CAN::Application::Pong: {
            auto senderID = static_cast<CAN::NodeIDs>(message.idInfo.sourceAddress);
            auto senderName = CAN::Application::nodeIdToString.at(senderID);
            LOG_DEBUG << "Received pong from " << senderName.c_str();
        } break;
        case CAN::Application::Heartbeat: {
            auto senderID = static_cast<CAN::NodeIDs>(message.idInfo.sourceAddress);
            auto senderName = CAN::Application::nodeIdToString.at(senderID);
            LOG_DEBUG << "Received heartbeat from " << senderName.c_str();
            heartbeatReceived = true;
        } break;
        case CAN::Application::LogMessage: {
            auto senderID = static_cast<CAN::NodeIDs>(message.idInfo.sourceAddress);
            auto senderName = CAN::Application::nodeIdToString.at(senderID);
            String<ECSSMaxMessageSize> logSource = "Incoming Log from ";
            logSource.append(senderName);
            logSource.append(": ");
            auto logData = String<ECSSMaxMessageSize>(message.data.begin() + 1, message.dataSize - 1);
            LOG_DEBUG << logSource.c_str() << logData.c_str();
        } break;
        default:
            LOG_INFO << "CAN Message of Unknown type";
            // ErrorHandler::reportInternalError(ErrorHandler::UnknownMessageType);
            break;
    }
}

bool TPProtocol::createCANTPMessage(const TPMessage& message, bool isISR) {
    if (!createCANTPMessageWithRetry(message, isISR, 2)) {
        return 0;
    } else {
        //Change CAN bus
        Application::switchBus();

        if (!createCANTPMessageWithRetry(message, isISR, 2)) {
            return 0;
        } else {
            uint32_t error = MCAN1_ErrorGet();
            LOG_ERROR << "Resetting CAN. Error code: " << error;
            //reset CAN
            MCAN0_Initialize();
            MCAN1_Initialize();
            vTaskDelay(1);
            CAN::Driver::initialize();
            vTaskDelay(1);
            uint8_t ActiveCANBus = PeakSatParameters::Main;
            MemManTask::setParameter(PeakSatParameters::CAN_BUS_ACTIVEID, &ActiveCANBus);
            if (!createCANTPMessageWithRetry(message, isISR, 2)) {
                return 0;
            } else {
                ActiveCANBus = PeakSatParameters::Redundant;
                MemManTask::setParameter(PeakSatParameters::CAN_BUS_ACTIVEID, &ActiveCANBus);
                if (!createCANTPMessageWithRetry(message, isISR, 2)) {
                    return 0;
                } else {
                    //Packet transmit fialure
                    LOG_ERROR << "Packet transmit failure";
                    return 1;
                }
            }
        }
    }
}

bool TPProtocol::createCANTPMessageWithRetry(const TPMessage& message, bool isISR, uint32_t NoOfRetries) {
    for (uint32_t i = 0; i < NoOfRetries; i++) {
        if (!createCANTPMessageNoRetransmit(message, isISR)) {
            return 0;
        }
        if (i > 0) {
            //number of retransmits ++
            LOG_ERROR << "Retranmitted CAN packet";
        }
    }
    return 1;
}

bool TPProtocol::createCANTPMessageNoRetransmit(const TPMessage& messageToBeSent, bool isISR) {
    TPMessage message = messageToBeSent;
    size_t messageSize = message.dataSize;
    uint32_t id = OBC_CAN_ID; //

    if (message.data[0] == Application::ACK) {
        etl::array<uint8_t, CAN::MaxPayloadLength> data = {
            static_cast<uint8_t>(((Single << 6) & 0xFF) | (messageSize & 0b111111))};
        for (size_t idx = 0; idx < messageSize; idx++) {
            data.at(idx + 1) = message.data[idx];
        }
        canGatekeeperTask->send({id, data}, isISR);
        return false;
    }

    //Add a dummy byte for single byte messages so that the gatekeeper can distinguish it from trash
    if (messageSize == 1) {
        messageSize = 2;
        message.appendUint8(0xAA);
    }

    // First Frame
    xSemaphoreTake(CAN_TRANSMIT_Handler.CAN_TRANSMIT_SEMAPHORE, portMAX_DELAY);
    {
        // 4 MSB bits is the Frame Type identifier and the 4 LSB are the leftmost 4 bits of the data length.
        uint8_t firstByte = (First << 6) | ((messageSize >> 8) & 0b111111);
        // Rest of the data length.
        uint8_t secondByte = messageSize & 0xFF;

        etl::array<uint8_t, CAN::MaxPayloadLength> firstFrame = {firstByte, secondByte};

        canGatekeeperTask->send({id, firstFrame}, isISR);
        xTaskNotifyGive(canGatekeeperTask->taskHandle);
    }

    // Consecutive Frames
    uint8_t totalConsecutiveFramesNeeded = ceil(static_cast<float>(messageSize) / UsableDataLength);
    for (uint8_t currentConsecutiveFrameCount = 1;
         currentConsecutiveFrameCount <= totalConsecutiveFramesNeeded; currentConsecutiveFrameCount++) {

        uint8_t firstByte = (Consecutive << 6);
        if (currentConsecutiveFrameCount == totalConsecutiveFramesNeeded) {
            firstByte = (Final << 6);
        }
        etl::array<uint8_t, CAN::MaxPayloadLength> consecutiveFrame = {firstByte};
        consecutiveFrame.at(1) = currentConsecutiveFrameCount;

        for (uint8_t idx = 0; idx < UsableDataLength; idx++) {
            consecutiveFrame.at(idx + 2) = message.data[idx + UsableDataLength * (currentConsecutiveFrameCount - 1)];
        }

        if (currentConsecutiveFrameCount % 4 == 3) { // Make sure the output buffers do not overflow
            vTaskDelay(1);
        }

        canGatekeeperTask->send({id, consecutiveFrame}, isISR);
        xTaskNotifyGive(canGatekeeperTask->taskHandle);
    }

    if (xSemaphoreTake(can_ack_handler.CAN_ACK_SEMAPHORE, pdMS_TO_TICKS(can_ack_handler.TIMEOUT)) == pdTRUE) {
        LOG_DEBUG << "CAN ACK received!";
        xSemaphoreGive(CAN_TRANSMIT_Handler.CAN_TRANSMIT_SEMAPHORE);
        return false;
    }
    LOG_ERROR << "Timeout waiting for CAN ACK";
    xSemaphoreGive(CAN_TRANSMIT_Handler.CAN_TRANSMIT_SEMAPHORE);
    return true;
}
