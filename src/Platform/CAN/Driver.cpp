#include "CAN/Driver.hpp"
#include "CAN/TPProtocol.hpp"
#include "CANGatekeeperTask.hpp"
#include "Logger.hpp"
#include "FreeRTOS.h"
#include <semphr.h>
#include <TPProtocol.hpp>

uint8_t CAN::Driver::convertDlcToLength(uint8_t dlc) {
    return dataLengthCode;
}

uint8_t CAN::Driver::convertLengthToDLC(uint8_t length) {
    uint8_t dlc;

    if (length <= 8U) {
        dlc = length;
    } else if (length <= 12U) {
        dlc = 0x9U;
    } else if (length <= 16U) {
        dlc = 0xAU;
    } else if (length <= 20U) {
        dlc = 0xBU;
    } else if (length <= 24U) {
        dlc = 0xCU;
    } else if (length <= 32U) {
        dlc = 0xDU;
    } else if (length <= 48U) {
        dlc = 0xEU;
    } else {
        dlc = 0xFU;
    }
    return dlc;
}

void CAN::Driver::mcan0TxFifoCallback(uintptr_t context) {
    uint32_t status = MCAN0_ErrorGet() & MCAN_PSR_LEC_Msk;
    bool isStatusOk = (status == MCAN_ERROR_NONE) || (status == MCAN_ERROR_LEC_NO_CHANGE);
    auto appState = static_cast<AppStates>(context);

    if (appState == Transmit && not isStatusOk) {
        LOG_ERROR << "Could not transmit CAN message. The status is " << status;
        return;
    }

    canGatekeeperTask->lastTransmissionTime = xTaskGetTickCount();
}

void CAN::Driver::mcan0RxFifo0Callback(uint8_t numberOfMessages, uintptr_t context) {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;


    uint32_t status = MCAN1_ErrorGet() & MCAN_PSR_LEC_Msk;
    bool isStatusOk = (status == MCAN_ERROR_NONE) || (status == MCAN_ERROR_LEC_NO_CHANGE);
    auto appState = static_cast<AppStates>(context);

    if (not(isStatusOk && appState == Receive)) {
        return;
    }

    for (size_t messageNumber = 0; messageNumber < numberOfMessages; messageNumber++) {
        /* Retrieve Rx messages from RX FIFO0 */
        CAN::Frame newFrame;

        // Find the next part of the buffer to store the incoming message and set the pointer to it
        if (incomingFIFO.lastItemPointer >= sizeOfIncommingFrameBuffer) {
            incomingFIFO.lastItemPointer = 0;
        }
        newFrame.pointerToData = &incomingFIFO.buffer[CANMessageSize * (incomingFIFO.lastItemPointer)];

        // get the frame from the peripheral
        if (not(MCAN0_MessageReceiveFifo(MCAN_RX_FIFO_0, 1, &newFrame.header))) {
            // ERROR
        }
        if (newFrame.header.id >> 18 == COMMS_CAN_ID) { // Check if the message came from the COMMS

            // Add data to the buffer
            for (int i = 0; i < MaxPayloadLength; i++) {
                newFrame.pointerToData[i] = newFrame.header.data[i];
            }

            newFrame.bus = CAN::CAN2;

            // Notify the gatekeeper
            if (xQueueIsQueueFullFromISR(canGatekeeperTask->incomingFrameQueue) == pdTRUE) {
                // Queue is full. Handle the error
                // todo
            } else {
                xQueueSendToBackFromISR(canGatekeeperTask->incomingFrameQueue, &newFrame, NULL);
                xTaskNotifyFromISR(canGatekeeperTask->taskHandle, 0, eNoAction, &xHigherPriorityTaskWoken);
                portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
            }
        } else if (true) { // else logic for ADCS
        }
    }
}

void CAN::Driver::mcan0RxFifo1Callback(uint8_t numberOfMessages, uintptr_t context) {
    // Unused
    __NOP();
}

void CAN::Driver::mcan1TxFifoCallback(uintptr_t context) {
    uint32_t status = MCAN1_ErrorGet() & MCAN_PSR_LEC_Msk;
    bool isStatusOk = (status == MCAN_ERROR_NONE) || (status == MCAN_ERROR_LEC_NO_CHANGE);
    auto appState = static_cast<AppStates>(context);

    if (appState == Transmit && not isStatusOk) {
        LOG_ERROR << "Could not transmit CAN message. The status is " << status;
        return;
    }

    canGatekeeperTask->lastTransmissionTime = xTaskGetTickCount();
}

void CAN::Driver::mcan1RxFifo0Callback(uint8_t numberOfMessages, uintptr_t context) {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;


    uint32_t status = MCAN1_ErrorGet() & MCAN_PSR_LEC_Msk;
    bool isStatusOk = (status == MCAN_ERROR_NONE) || (status == MCAN_ERROR_LEC_NO_CHANGE);
    auto appState = static_cast<AppStates>(context);

    if (not(isStatusOk && appState == Receive)) {
        return;
    }

    for (size_t messageNumber = 0; messageNumber < numberOfMessages; messageNumber++) {
        /* Retrieve Rx messages from RX FIFO0 */
        CAN::Frame newFrame;

        // Find the next part of the buffer to store the incoming message and set the pointer to it
        if (incomingFIFO.lastItemPointer >= sizeOfIncommingFrameBuffer) {
            incomingFIFO.lastItemPointer = 0;
        }
        newFrame.pointerToData = &incomingFIFO.buffer[CANMessageSize * (incomingFIFO.lastItemPointer)];

        if (not(MCAN1_MessageReceiveFifo(MCAN_RX_FIFO_0, 1, &newFrame.header))) {
            // ERROR
        }

        if (newFrame.header.id >> 18 == COMMS_CAN_ID) { // Check if the message came from the COMMS

            // Add data to the buffer
            for (int i = 0; i < MaxPayloadLength; i++) {
                newFrame.pointerToData[i] = newFrame.header.data[i];
            }

            newFrame.bus = CAN::CAN2;

            // Notify the gatekeeper
            if (xQueueIsQueueFullFromISR(canGatekeeperTask->incomingFrameQueue) == pdTRUE) {
                // Queue is full. Handle the error
                // todo
            } else {
                xQueueSendToBackFromISR(canGatekeeperTask->incomingFrameQueue, &newFrame, NULL);
                xTaskNotifyFromISR(canGatekeeperTask->taskHandle, 0, eNoAction, &xHigherPriorityTaskWoken);
                portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
                __NOP();
            }
        } else if (true) { // else logic for ADCS
        }
    }
}

void CAN::Driver::mcan1RxFifo1Callback(uint8_t numberOfMessages, uintptr_t context) {
    // Unused
    __NOP();
}

void CAN::Driver::send(const CAN::Packet& message) {
    using namespace CAN;

    memset(&Driver::txFifo, 0, MCAN1_TX_FIFO_BUFFER_ELEMENT_SIZE);

    Driver::txFifo.brs = 0;
    Driver::txFifo.fdf = 1;
    Driver::txFifo.xtd = 0;
    Driver::txFifo.id = Driver::writeId(message.id);
    Driver::txFifo.dlc = Driver::convertLengthToDLC(message.data.size());

    etl::copy(message.data.begin(), message.data.end(), Driver::txFifo.data);

    uint8_t getActiveCANBus = 2;
    MemoryManager::getParameter(PeakSatParameters::OBDH_CAN_BUS_ACTIVE_ID, static_cast<void*>(&getActiveCANBus));
    if (getActiveCANBus == PeakSatParameters::Main) {
        if (MCAN0_TxFifoFreeLevelGet() < 1) {
            // LOG_ERROR << "CAN0 Tx FIFO full";

        } else {
            MCAN0_MessageTransmitFifo(1, &Driver::txFifo);
        }
    } else {
        if (MCAN1_TxFifoFreeLevelGet() < 1) {
            // LOG_ERROR << "CAN1 Tx FIFO full";
        } else {
            MCAN1_MessageTransmitFifo(1, &Driver::txFifo);
        }
    }
}

void CAN::Driver::logMessage(const MCAN_RX_BUFFER& rxBuf, ActiveBus incomingBus) {
    auto message = String<ECSSMaxStringSize>("CAN Message: ");
    if (incomingBus == Main) {
        message.append("MCAN0 ");
    } else {
        message.append("MCAN1 ");
    }
    uint32_t id = readId(rxBuf.id);
    const uint8_t msgLength = convertDlcToLength(rxBuf.dlc);
    message.append("ID : ");
    etl::to_string(id, message, etl::format_spec().hex(), true);
    message.append(" Length : ");
    etl::to_string(msgLength, message, true);
    message.append(" Data : ");
    for (uint8_t idx = 0; idx < msgLength; idx++) {
        etl::to_string(rxBuf.data[idx], message, true);
        message.append(" ");
    }
    LOG_INFO << message.c_str();
}

CAN::Packet CAN::Driver::getFrame(const MCAN_RX_BUFFER& rxBuffer) {
    CAN::Packet frame;
    const uint8_t messageLength = convertDlcToLength(rxBuffer.dlc);

    frame.id = readId(rxBuffer.id);
    for (uint8_t idx = 0; idx < messageLength; idx++) {
        frame.data.at(idx) = rxBuffer.data[idx];
    }

    return frame;
}

void CAN::Driver::initialize() {
    MCAN0_MessageRAMConfigSet(CAN::Driver::mcan0MessageRAM);
    MCAN1_MessageRAMConfigSet(CAN::Driver::mcan1MessageRAM);

    MCAN0_TxFifoCallbackRegister(CAN::Driver::mcan0TxFifoCallback, CAN::Driver::Transmit);
    MCAN0_RxFifoCallbackRegister(MCAN_RX_FIFO_0, CAN::Driver::mcan0RxFifo0Callback, CAN::Driver::Receive);
    MCAN0_RxFifoCallbackRegister(MCAN_RX_FIFO_1, CAN::Driver::mcan0RxFifo1Callback, CAN::Driver::Receive);

    MCAN1_TxFifoCallbackRegister(CAN::Driver::mcan1TxFifoCallback, CAN::Driver::Transmit);
    MCAN1_RxFifoCallbackRegister(MCAN_RX_FIFO_0, CAN::Driver::mcan1RxFifo0Callback, CAN::Driver::Receive);
    MCAN1_RxFifoCallbackRegister(MCAN_RX_FIFO_1, CAN::Driver::mcan1RxFifo1Callback, CAN::Driver::Receive);
}
