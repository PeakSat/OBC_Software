#include "CAN/ApplicationLayer.hpp"
#include "CAN/Driver.hpp"
#include "CAN/TPMessage.hpp"
#include "CAN/TPProtocol.hpp"
#include "CANGatekeeperTask.hpp"

#include <ServicePool.hpp>
#include <TimeGetter.hpp>

namespace CAN::Application {
    void switchBus() {
        uint8_t readActiveCANbus = 2;
        MemoryManager::getParameter(PeakSatParameters::OBDH_CAN_BUS_ACTIVE_ID, static_cast<void*>(&readActiveCANbus));
        if (readActiveCANbus == PeakSatParameters::Main) {
            readActiveCANbus = PeakSatParameters::Redundant;
            MemoryManager::setParameter(PeakSatParameters::OBDH_CAN_BUS_ACTIVE_ID, static_cast<void*>(&readActiveCANbus));
        } else {
            readActiveCANbus = PeakSatParameters::Main;
            MemoryManager::setParameter(PeakSatParameters::OBDH_CAN_BUS_ACTIVE_ID, static_cast<void*>(&readActiveCANbus));
        }
    }

    void sendPingMessage(NodeIDs destinationAddress, bool isMulticast) {
        TPMessage message = {{NodeID, destinationAddress, isMulticast}};

        message.appendUint8(Ping);

        CAN::TPProtocol::createCANTPMessage(message, false);
    }

    void sendPongMessage() {
        TPMessage message = {{NodeID, COMMS, false}};

        message.appendUint8(Pong);

        CAN::TPProtocol::createCANTPMessage(message, true);
    }

    void sendHeartbeatMessage() {
        TPMessage message = {{NodeID, COMMS, false}};

        message.appendUint8(Heartbeat);

        CAN::TPProtocol::createCANTPMessage(message, true);
    }

    void sendUTCTimeMessageWithElapsedTicks() {
        auto now = TimeGetter::getCurrentTimeDefaultCUC();

        const std::chrono::duration<uint64_t, std::milli> msOfDay = now.asDuration() % millisecondsPerDay;

        uint32_t ticksOfDay = static_cast<uint32_t>(msOfDay.count() / 100);

        UTCTimestamp utc = now.toUTCtimestamp();
        etl::array<uint8_t, CAN::MaxPayloadLength> data = {0, 0, static_cast<uint8_t>(ticksOfDay),
                                                           static_cast<uint8_t>(ticksOfDay >> 8),
                                                           static_cast<uint8_t>(ticksOfDay >> 16),
                                                           static_cast<uint8_t>(ticksOfDay >> 24),
                                                           static_cast<uint8_t>(utc.day),
                                                           static_cast<uint8_t>(utc.day >> 8)};

        canGatekeeperTask->send({MessageIDs::UTCTime + CAN::NodeID, data}, false);
    }

    void createSendParametersMessage(NodeIDs destinationAddress, bool isMulticast,
                                     const etl::array<uint16_t, TPMessageMaximumArguments>& parameterIDs, bool isISR) {
        TPMessage message = {{CAN::NodeID, destinationAddress, isMulticast}};

        message.appendUint8(MessageIDs::SendParameters);
        message.appendUint16(parameterIDs.size());
        for (auto parameterID: parameterIDs) {
            if (Services.parameterManagement.parameterExists(parameterID)) {
                message.append(parameterID);
                Services.parameterManagement.appendParameterToMessage(message, parameterID);
                LOG_DEBUG << "param: " << MemoryManager::getParameterAsUINT64(parameterID);
                // LOG_DEBUG<< AcubeSATParameters::obcPCBTemperature1;
            } else if (parameterID == 0) {
                continue;
            } else {
                LOG_ERROR << "Requested parameter that doesn't exist! ID: " << parameterID;
            }
        }

        CAN::TPProtocol::createCANTPMessage(message, isISR);
    }

    void createRequestParametersMessage(NodeIDs destinationAddress, bool isMulticast,
                                        const etl::array<uint16_t, TPMessageMaximumArguments>& parameterIDs,
                                        bool isISR) {
        TPMessage message = {{CAN::NodeID, destinationAddress, isMulticast}};

        message.appendUint8(MessageIDs::RequestParameters);
        message.appendUint16(parameterIDs.size());

        if constexpr (Logger::isLogged(Logger::debug)) {
            String<128> logString = "Requesting parameters with ID: ";
            for (auto parameterID: parameterIDs) {
                if (parameterID == 0) {
                    continue;
                }
                etl::to_string(parameterID, logString, true);
                message.append(parameterID);
            }
            LOG_DEBUG << logString.c_str();
        } else {
            for (auto parameterID: parameterIDs) {
                message.append(parameterID);
            }
        }

        CAN::TPProtocol::createCANTPMessage(message, isISR);
    }

    void createPerformFunctionMessage(NodeIDs destinationAddress, bool isMulticast,
                                      const etl::string<FunctionIdSize>& functionId,
                                      const etl::map<uint8_t, uint64_t, TPMessageMaximumArguments>& arguments,
                                      bool isISR) {
        TPMessage message = {{CAN::NodeID, destinationAddress, isMulticast}};

        message.appendUint8(MessageIDs::PerformFunction);

        message.appendString(functionId);

        message.appendUint16(arguments.size());

        for (auto argument: arguments) {
            message.appendUint8(argument.first);
            message.appendUint64(argument.second);
        }

        CAN::TPProtocol::createCANTPMessage(message, isISR);
    }

    void createEventReportMessage(NodeIDs destinationAddress, bool isMulticast, EventReportType type, uint16_t eventID,
                                  const Message& eventData, bool isISR) {
        TPMessage message = {{CAN::NodeID, destinationAddress, isMulticast}};

        message.appendUint8(MessageIDs::EventReport);
        message.appendEnum8(type);
        message.appendUint16(eventID);
        message.appendMessage(eventData, eventData.dataSize);

        CAN::TPProtocol::createCANTPMessage(message, isISR);
    }

    void createPacketMessage(NodeIDs destinationAddress, bool isMulticast, const etl::string<128>& incomingMessage, Message::PacketType packetType, bool isISR) {
        TPMessage message = {{CAN::NodeID, destinationAddress, isMulticast}};

        if (packetType == Message::TM) {
            message.appendUint8(MessageIDs::TMPacket);
        } else {
            message.appendUint8(MessageIDs::TCPacket);
        }

        message.appendString(incomingMessage);

        CAN::TPProtocol::createCANTPMessage(message, isISR);
    }

    void
    createCCSDSPacketMessage(NodeIDs destinationAddress, bool isMulticast, const Message& incomingMessage, bool isISR) {
        TPMessage message = {{CAN::NodeID, destinationAddress, isMulticast}};

        auto ccsdsMessage = MessageParser::compose(incomingMessage);

        message.appendUint8(MessageIDs::CCSDSPacket);
        message.appendString(ccsdsMessage);

        CAN::TPProtocol::createCANTPMessage(message, isISR);
    }

    bool createLogMessage(NodeIDs destinationAddress, bool isMulticast, const String<ECSSMaxMessageSize>& log,
                          bool isISR) {
        TPMessage message = {{CAN::NodeID, destinationAddress, isMulticast}};

        message.appendUint8(MessageIDs::LogMessage);
        message.appendString(log);

        return CAN::TPProtocol::createCANTPMessage(message, isISR);
    }

    void parseMessage(const CAN::Packet& message) {
        uint32_t id = filterMessageID(message.id);
        if (id == Heartbeat) {
            //            registerHeartbeat();
        } else if (id == BusSwitchover) {
            switchBus();
        } else if (id == UTCTime) {
            //            registerUTCTime();
        }
    }

    void parseSendParametersMessage(TPMessage& message) {
        uint8_t messageType = message.readUint8();
        if (not ErrorHandler::assertInternal(messageType == SendParameters, ErrorHandler::UnknownMessageType)) {
            return;
        }
        uint16_t parameterCount = message.readUint16();

        for (uint16_t idx = 0; idx < parameterCount; idx++) {
            uint16_t parameterID = message.readUint16();
            if (Services.parameterManagement.parameterExists(parameterID)) {
                if constexpr (Logger::isLogged(Logger::debug)) {
                    String<64> logString = "The value for parameter with ID ";
                    etl::to_string(parameterID, logString, true);
                    logString.append(" was ");

                    etl::to_string(MemoryManager::getParameterAsUINT64(parameterID), logString, true);
                    Services.parameterManagement.updateParameterFromMessage(message, parameterID);
                    logString.append(" and is now ");
                    etl::to_string(MemoryManager::getParameterAsUINT64(parameterID), logString, true);

                    LOG_DEBUG << logString.c_str();
                } else {
                    Services.parameterManagement.updateParameterFromMessage(message, parameterID);
                }
            }
        }
    }

    void parseRequestParametersMessage(TPMessage& message) {
        uint8_t messageType = message.readUint8();
        if (not ErrorHandler::assertInternal(messageType == RequestParameters, ErrorHandler::UnknownMessageType)) {
            return;
        }
        uint16_t parameterCount = message.readUint16();
        etl::array<uint16_t, TPMessageMaximumArguments> parameterIDs = {};

        for (uint16_t idx = 0; idx < parameterCount; idx++) {
            parameterIDs[idx] = message.readUint16();
        }
        createSendParametersMessage(message.idInfo.sourceAddress, message.idInfo.isMulticast, parameterIDs, false);
    }

    void parseTMMessage(TPMessage& message) {
        uint8_t messageType = message.readUint8();
        if (not ErrorHandler::assertInternal(messageType == TMPacket, ErrorHandler::UnknownMessageType)) {
            return;
        }

        String<ECSSMaxMessageSize> logString = message.data.begin() + 1;

        LOG_DEBUG << logString.c_str();
    }

    void parseTCMessage(TPMessage& message) {
        uint8_t messageType = message.readUint8();
        if (not ErrorHandler::assertInternal(messageType == TCPacket, ErrorHandler::UnknownMessageType)) {
            return;
        }

        Message teleCommand = MessageParser::parseECSSTC(message.data.begin() + 1);

        MessageParser::execute(teleCommand);
    }
} // namespace CAN::Application
