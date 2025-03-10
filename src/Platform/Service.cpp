#include <etl/String.hpp>
#include <Logger.hpp>
#include "OBC_Definitions.hpp"
#include "Service.hpp"

#include <ApplicationLayer.hpp>

#include "MessageParser.hpp"

// #define LOG_HOUSEKEEPING_TO_SERIAL

void Service::storeMessage(Message& message) {

    // appends the remaining bits to complete a byte
    message.finalize();

#ifdef LOG_HOUSEKEEPING_TO_SERIAL
    etl::format_spec formatSpec;
    auto serviceType = String<MaxTypeIDStringSize>("");
    auto messageType = String<MaxTypeIDStringSize>("");

    etl::to_string(message.serviceType, serviceType, formatSpec, false);
    etl::to_string(message.messageType, messageType, formatSpec, false);


    auto output = String<ECSSMaxMessageSize>("New ");
    (message.packetType == Message::TM) ? output.append("TM[") : output.append("TC[");
    output.append(serviceType);
    output.append(",");
    output.append(messageType);
    output.append("] message! ");

    auto data = String<CCSDSMaxMessageSize>("");
    String<CCSDSMaxMessageSize> createdPacket = MessageParser::compose(message);
    for (unsigned int i = 0; i < createdPacket.size(); i++) {
        etl::to_string(createdPacket[i], data, formatSpec, true);
        data.append(" ");
    }

    output.append(data.c_str());
    LOG_DEBUG << output.c_str();
#endif

    CAN::Application::createCCSDSPacketMessage(CAN::NodeIDs::COMMS, false, message, false);
    LOG_DEBUG<<"Sent Housekeeping via CAN, size: "<<message.dataSize;

    if (message.packetType == Message::TM) {
        LOG_DEBUG<<"[STORAGE] Storing TM to Filesystem";
        auto status = MemoryManager::storeTMToFile(message);
        if (!status.has_value()) {
            MemoryManager::printMemoryError(status.error());
        }
    }

}
