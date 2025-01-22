#include "CAN/Driver.hpp"
#include "CAN/Frame.hpp"
#include "CANGatekeeperTask.hpp"
#include <ApplicationLayer.hpp>
#include <CANParserTask.hpp>
#include <TestTask.hpp>

struct incomingFIFO incomingFIFO;
uint8_t incomingBuffer[CAN::MaxPayloadLength * sizeOfIncommingFrameBuffer];

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

    incomingPacketQueue = xQueueCreateStatic(1, sizeof(localPacketHandler*), incomingPacketStorageArea,
                                             &incomingPacketBuffer);
    vQueueAddToRegistry(incomingPacketQueue, "CAN Outgoing");
    configASSERT(incomingPacketQueue);

    incomingFrameQueue = xQueueCreateStatic(sizeOfIncommingFrameBuffer, sizeof(CAN::Frame), incomingFrameQueueStorageArea,
                                            &incomingFrameQueueBuffer);
    vQueueAddToRegistry(incomingFrameQueue, "CAN Incoming Frame");
    configASSERT(incomingFrameQueue);
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

    CAN::Frame in_frame_handler = {};

    uint32_t ulNotifiedValue;

    while (true) {
        //        LOG_DEBUG << "Runtime entered: " << this->TaskName;
        xTaskNotifyWait(0, 0, &ulNotifiedValue, 1000);

        while (uxQueueMessagesWaiting(incomingFrameQueue)) {

            // Get the message pointer from the queue
            xQueueReceive(incomingFrameQueue, &in_frame_handler, portMAX_DELAY);

            if (in_frame_handler.header.id >> 18 == COMMS_CAN_ID) { // Check if the message came from the COMMS

                struct localPacketHandler* CANPacketHandler;
                if (in_frame_handler.bus == CAN::CAN1) {
                    CANPacketHandler = &CAN1PacketHandler;
                } else {
                    CANPacketHandler = &CAN2PacketHandler;
                }

                // Extract metadata
                uint8_t metadata = in_frame_handler.pointerToData[0];
                uint8_t frameType = metadata >> 6;
                uint8_t payloadLength = metadata & 0x3F;
                if (frameType == CAN::TPProtocol::Frame::Single) {
                    if (in_frame_handler.pointerToData[1] == CAN::Application::MessageIDs::ACK) {
                        xSemaphoreGive(can_ack_handler.CAN_ACK_SEMAPHORE);
                    }
                } else if (frameType == CAN::TPProtocol::Frame::First) {
                    // // debugCounter=0;
                    CANPacketHandler->PacketSize = payloadLength << 8;
                    CANPacketHandler->PacketSize = CANPacketHandler->PacketSize | in_frame_handler.pointerToData[1];
                    CANPacketHandler->PacketSize -= 1; //compensate for ID byte
                    // currenConsecutiveFrameCounter=0;
                    CANPacketHandler->TailPointer = 0;
                } else if (frameType == CAN::TPProtocol::Frame::Consecutive) {

                    uint8_t FrameNumber = in_frame_handler.pointerToData[1] - 1;
                    // Add frame to local buffer

                    for (uint32_t i = 0; i < (CAN::MaxPayloadLength - 2); i++) {
                        if (i + FrameNumber == 0) {
                            // first consecutive frame has the message ID in its first byte
                            CANPacketHandler->PacketID = in_frame_handler.pointerToData[2];
                        } else {
                            if (sizeof(CANPacketHandler->Buffer) / sizeof(CANPacketHandler->Buffer[0]) > (FrameNumber * (CAN::MaxPayloadLength - 2)) + i - 1) {
                                //add the rest of the bytes to the local buffer
                                CANPacketHandler->Buffer[(FrameNumber * (CAN::MaxPayloadLength - 2)) + i - 1] = in_frame_handler.pointerToData[i + 2];
                                CANPacketHandler->TailPointer = CANPacketHandler->TailPointer + 1;
                            } else {
                                // buffer size exceeded
                                CANPacketHandler->TailPointer = 0;
                            }
                        }
                    }

                } else if (frameType == CAN::TPProtocol::Frame::Final) {
                    // check if the number of received bytes matches the message length
                    if ((CANPacketHandler->PacketSize - CANPacketHandler->TailPointer) <= (CAN::MaxPayloadLength - 2) && CANPacketHandler->PacketSize != 0) {
                        // add the last bytes to the local buffer
                        for (uint32_t i = 0; (CANPacketHandler->PacketSize > CANPacketHandler->TailPointer); i++) {
                            uint8_t FrameNumber = in_frame_handler.pointerToData[1] - 1;
                            if (i + FrameNumber == 0) {
                                // if there are no consecutive frames, the message ID is the first byte of this frame
                                CANPacketHandler->PacketID = in_frame_handler.pointerToData[2];
                            } else if (sizeof(CANPacketHandler->Buffer) / sizeof(CANPacketHandler->Buffer[0]) > CANPacketHandler->TailPointer) {
                                //add the rest of the bytes in the local buffer
                                CANPacketHandler->Buffer[CANPacketHandler->TailPointer] = in_frame_handler.pointerToData[i + 2];
                                CANPacketHandler->TailPointer = CANPacketHandler->TailPointer + 1;
                            } else {
                                // buffer size exceeded
                                CANPacketHandler->TailPointer = 0;
                            }
                        }
                        xQueueSendToBack(incomingPacketQueue, (void*) &CANPacketHandler, NULL);
                        xTaskNotify(canParserTask->taskHandle, 0, eNoAction);
                    } else {
                        // Message not received correctly
                        LOG_ERROR << "DROPPED CAN MESSAGE";
                    }
                    CANPacketHandler->TailPointer = 0;
                }
            } // else logic for the ADCS
        }
        while (uxQueueMessagesWaiting(outgoingQueue)) {
            // vTaskDelay(1);
            xQueueReceive(outgoingQueue, &out_message, portMAX_DELAY);
            CAN::Driver::send(out_message);
        }
        //        LOG_DEBUG << "Runtime is exiting: " << this->TaskName;
    }
}
