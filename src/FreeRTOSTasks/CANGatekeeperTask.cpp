#include "CAN/Driver.hpp"
#include "CAN/Frame.hpp"
#include "CANGatekeeperTask.hpp"
#include <ApplicationLayer.hpp>

struct incomingFIFO incomingFIFO;
uint8_t incomingBuffer[CANMessageSize * sizeOfIncommingFrameBuffer];
struct localPacketHandler {
    uint8_t Buffer[1024];
    uint32_t TailPointer = 0;
    uint32_t PacketSize = 0;
    uint8_t PacketID = 0;
};
struct localPacketHandler CAN1PacketHandler;
struct localPacketHandler CAN2PacketHandler;

CANGatekeeperTask::CANGatekeeperTask() : Task("CANGatekeeperTask") {
    CAN::Driver::initialize();

    incomingFIFO.buffer = incomingBuffer;
    incomingFIFO.NOfItems = sizeOfIncommingFrameBuffer;

    outgoingQueue = xQueueCreateStatic(CAN::FrameQueueSize, sizeof(CAN::Packet), outgoingQueueStorageArea,
                                       &outgoingQueueBuffer);
    vQueueAddToRegistry(outgoingQueue, "CAN Outgoing");
    configASSERT(outgoingQueue);

    incomingSFQueue = xQueueCreateStatic(CAN::FrameQueueSize, sizeof(CAN::Packet), incomingSFQueueStorageArea,
                                         &incomingSFQueueBuffer);
    vQueueAddToRegistry(incomingSFQueue, "CAN Incoming SF");
    configASSERT(incomingSFQueue);

    incomingMFQueue = xQueueCreateStatic(CAN::FrameQueueSize, sizeof(CAN::Packet), incomingSFQueueStorageArea,
                                         &incomingMFQueueBuffer);
    vQueueAddToRegistry(incomingSFQueue, "CAN Incoming MF");
    configASSERT(incomingMFQueue);

    incomingFrameQueue = xQueueCreateStatic(sizeOfIncommingFrameBuffer, sizeof(CAN::Frame), incomingFrameQueueStorageArea,
                                            &incomingFrameQueueBuffer);
    vQueueAddToRegistry(incomingFrameQueue, "CAN Incoming Frame");
}

void CANGatekeeperTask::execute() {
    LOG_DEBUG << "Runtime init: " << this->TaskName;
#ifdef OBC_EQM_LCL
    LCLDefinitions::lclArray[LCLDefinitions::CAN1].enableLCL();
    LCLDefinitions::lclArray[LCLDefinitions::CAN2].enableLCL();
#endif

    PIO_PinWrite(CAN::CAN_SILENT_1, false);
    PIO_PinWrite(CAN::CAN_SILENT_2, false);

    CAN::Packet out_message = {};
    CAN::Packet in_message = {};

    /*   struct localPacketHandler {
        uint8_t Buffer[1024];
        uint32_t TailPointer = 0;
        uint32_t PacketSize = 0;
        uint8_t PacketID = 0;
    };
    struct localPacketHandler CAN1PacketHandler;
    struct localPacketHandler CAN2PacketHandler;*/
    CAN::Frame in_frame_handler = {};

    uint32_t ulNotifiedValue;

    while (true) {

        //        LOG_DEBUG << "Runtime entered: " << this->TaskName;
        xTaskNotifyWait(0, 0, &ulNotifiedValue, 1000);

        if (xTaskGetTickCount() - lastTransmissionTime > 8000) {
            LOG_ERROR << "Resetting CAN LCLs";
            LCLDefinitions::lclArray[LCLDefinitions::CAN1].enableLCL();
            LCLDefinitions::lclArray[LCLDefinitions::CAN2].enableLCL();
            MCAN0_Initialize();
            MCAN1_Initialize();
        }

        while (uxQueueMessagesWaiting(incomingFrameQueue)) {
            // if (eMMCPacketTailPointer + 2 > eMMC::memoryMap[eMMC::CANMessages].size / 512) {
            //     eMMCPacketTailPointer = 0;
            // }


            // Get the message pointer from the queue
            xQueueReceive(incomingFrameQueue, &in_frame_handler, portMAX_DELAY);

            struct localPacketHandler* CANPacketHandler;
            if (in_frame_handler.bus == CAN::CAN1) {
                CANPacketHandler = &CAN1PacketHandler;
            } else if (in_frame_handler.bus == CAN::CAN2) {
                CANPacketHandler = &CAN2PacketHandler;
            }

            // Extract metadata
            uint8_t metadata = in_frame_handler.pointerToData[0];
            uint8_t frameType = metadata >> 6;
            uint8_t payloadLength = metadata & 0x3F;
            if (frameType == CAN::TPProtocol::Frame::Single) {
                if (in_frame_handler.pointerToData[1] == CAN::Application::MessageIDs::ACK) {
                    CAN_TRANSMIT_Handler.ACKReceived = true;
                }
                __NOP();
            } else if (frameType == CAN::TPProtocol::Frame::First) {
                // // debugCounter=0;
                CANPacketHandler->PacketSize = payloadLength << 8;
                CANPacketHandler->PacketSize = CANPacketHandler->PacketSize | in_frame_handler.pointerToData[1];
                CANPacketHandler->PacketSize -= 1; //compensate for ID byte
                // currenConsecutiveFrameCounter=0;
                CANPacketHandler->TailPointer = 0;
                __NOP();
            } else if (frameType == CAN::TPProtocol::Frame::Consecutive) {

                uint8_t FrameNumber = in_frame_handler.pointerToData[1] - 1;
                // Add frame to local buffer
                __NOP();

                for (uint32_t i = 0; i < (CAN::MaxPayloadLength - 2); i++) {
                    __NOP();
                    if (i + FrameNumber == 0) {
                        CANPacketHandler->PacketID = in_frame_handler.pointerToData[2];
                    } else {
                        CANPacketHandler->Buffer[(FrameNumber * (CAN::MaxPayloadLength - 2)) + i - 1] = in_frame_handler.pointerToData[i + 2];
                        CANPacketHandler->TailPointer = CANPacketHandler->TailPointer + 1;
                    }
                }
                __NOP();

            } else if (frameType == CAN::TPProtocol::Frame::Final) {
                if ((CANPacketHandler->PacketSize - CANPacketHandler->TailPointer) < (CAN::MaxPayloadLength - 2) && CANPacketHandler->PacketSize != 0) {
                    for (uint32_t i = 0; (CANPacketHandler->PacketSize > CANPacketHandler->TailPointer); i++) {

                        CANPacketHandler->Buffer[CANPacketHandler->TailPointer] = in_frame_handler.pointerToData[i + 2];
                        CANPacketHandler->TailPointer = CANPacketHandler->TailPointer + 1;
                    }
                    // Send ACK
                    CAN::TPMessage ACKmessage = {{CAN::NodeID, CAN::NodeIDs::OBC, false}};

                    ACKmessage.appendUint8(CAN::Application::MessageIDs::ACK);

                    CAN::TPProtocol::createCANTPMessage(ACKmessage, false);

                    // Parse message
                    // Add message to queue
                    CAN::TPMessage message;
                    message.appendUint8(CANPacketHandler->PacketID);
                    for (int i = 0; i < CANPacketHandler->PacketSize; i++) {
                        message.appendUint8(CANPacketHandler->Buffer[i]);
                    }
                    CAN::TPProtocol::parseMessage(message);
                    __NOP();

                    // xQueueSendToBack(storedPacketQueue, &PacketToBeStored, NULL);
                    // CAN::TPProtocol::parseMessage(message);
                } else {
                    // Message not received correctly
                    LOG_DEBUG << "DROPPED CAN MESSAGE";
                }
                __NOP();
                CANPacketHandler->TailPointer = 0;
            }


            // while (getIncomingSFMessagesCount()) {
            //     xQueueReceive(incomingSFQueue, &in_message, portMAX_DELAY);
            //     CAN::TPProtocol::processSingleFrame(in_message);
            // }
            CAN::TPProtocol::processMultipleFrames();


            //        LOG_DEBUG << "Runtime is exiting: " << this->TaskName;
        }
        while (uxQueueMessagesWaiting(outgoingQueue)) {
            vTaskDelay(1);
            xQueueReceive(outgoingQueue, &out_message, portMAX_DELAY);
            CAN::Driver::send(out_message);
        }
    }
}