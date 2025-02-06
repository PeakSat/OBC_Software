#include "CANTestTask.hpp"
#include "CANGatekeeperTask.hpp"

void CANTestTask::execute() {

    LOG_DEBUG << "Runtime init: " << this->TaskName;
    CAN::Frame frame = {CAN::NodeID};
    for (auto i = 0; i < CAN::Frame::MaxDataLength; i++) {
        frame.data.at(i) = i;
    }

    String<ECSSMaxMessageSize> testPayload1("WHO LIVES IN A PINEAPPLE UNDER THE SEA?");

    String<ECSSMaxMessageSize> testPayload2("Giati?");

    uint8_t activeBus = CAN::Driver::ActiveBus::Main;

    while (true) {

        if (activeBus == CAN::Driver::ActiveBus::Main) {
            activeBus = CAN::Driver::ActiveBus::Redundant;
            CAN::Application::createLogMessage(CAN::NodeIDs::COMMS, false, testPayload1.data(), false);
            LOG_DEBUG << "Sent CAN message to main CAN bus";
        } else {
            activeBus = CAN::Driver::ActiveBus::Main;
            CAN::Application::createLogMessage(CAN::NodeIDs::COMMS, false, testPayload2.data(), false);
            LOG_DEBUG << "Sent CAN message to redundant CAN bus";
        }
        memManTask->setParameter(PeaksatParameters::CANBUSActiveID, static_cast<void*>(&activeBus));
        xTaskNotify(canGatekeeperTask->taskHandle, 0, eNoAction);

        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}