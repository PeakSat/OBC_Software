#include "CANParserTask.hpp"
#include "CANGatekeeperTask.hpp"


void CANParserTask::execute() {
    LOG_DEBUG << "Runtime init: " << this->TaskName;

    // String<ECSSMaxMessageSize> testPayload1("a");
    //
    // String<ECSSMaxMessageSize> testPayload2("bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb");
    uint32_t ulNotifiedValue;
    while (true) {
        xTaskNotifyWait(0, 0, &ulNotifiedValue, portMAX_DELAY);
        while (uxQueueMessagesWaiting(canGatekeeperTask->incomingPacketQueue)) {
            // vTaskDelay(1);
            struct localPacketHandler* CANPacketHandler;
            xQueueReceive(canGatekeeperTask->incomingPacketQueue, &CANPacketHandler, portMAX_DELAY);

            // construct message
            CAN::TPMessage message;
            message.appendUint8(CANPacketHandler->PacketID);
            for (int i = 0; i < CANPacketHandler->PacketSize; i++) {
                message.appendUint8(CANPacketHandler->Buffer[i]);
            }
            message.idInfo.sourceAddress = CAN::COMMS;

            CAN::TPProtocol::parseMessage(message);
            //Send ACK
            CAN::TPMessage ACKmessage = {{CAN::NodeID, CAN::NodeIDs::OBC, false}};
            ACKmessage.appendUint8(CAN::Application::MessageIDs::ACK);
            CAN::TPProtocol::createCANTPMessageNoRetransmit(ACKmessage, false);
            xTaskNotifyGive(canGatekeeperTask->taskHandle);
        }


        // LOG_DEBUG << "Runtime entered: " << this->TaskName;
        // if (PeakSatParameters::obcCANBUSActive.getValue() == CAN::Driver::ActiveBus::Redundant) {
        //     PeakSatParameters::obcCANBUSActive.setValue(CAN::Driver::ActiveBus::Main);
        //     if (CAN::Application::createLogMessage(CAN::NodeIDs::COMMS, false, testPayload1.data(), false)) {
        //         LOG_ERROR << "CAN FAILURE";
        //     }
        // } else {
        //     PeakSatParameters::obcCANBUSActive.setValue(CAN::Driver::ActiveBus::Redundant);
        //     if (CAN::Application::createLogMessage(CAN::NodeIDs::COMMS, false, testPayload2.data(), false)) {
        //         LOG_ERROR << "CAN FAILURE";
        //     }
        // }
        //
        // xTaskNotify(canGatekeeperTask->taskHandle, 0, eNoAction);
        //
        // //        LOG_DEBUG << "Runtime exit: " << this->TaskName;
        // vTaskDelay(pdMS_TO_TICKS(5000));
    }
}