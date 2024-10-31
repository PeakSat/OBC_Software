//
// Created by tats on 10/7/2024.
//

#include "RS422/RS422_Driver.hpp"
#include "TaskConfigs.hpp"
#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"


/*
 * ADM3491 module
 * RO -> Receiver Output -> Connected to UART2_RX
 * DI -> Driver Input -> Connected to UART2_TX
 * RE -> Receiver Output Enable, Active Low -> Connected to PD30 (Chip pad 34)
 * DE -> Driver Output Enable, Active High -> Connected to PD16 (Chip pad 78)
 *
 * ! If RE is High and DE is Low, the device enters a low power
 * shutdown mode
 *
 */

void enterLowPowerModeRS422(){
    PIO_PinWrite(PAYLOAD_RX_ENABLE_PIN, true);
    PIO_PinWrite(PAYLOAD_TX_ENABLE_PIN, false);
}

bool sendPayloadStatus(){
    return UART2_TransmitComplete();
}

void sendModeRS422(){
    PIO_PinWrite(PAYLOAD_RX_ENABLE_PIN, true);
    PIO_PinWrite(PAYLOAD_TX_ENABLE_PIN, true);
}

void receiveModeRS422(){
    PIO_PinWrite(PAYLOAD_RX_ENABLE_PIN, false);
    PIO_PinWrite(PAYLOAD_TX_ENABLE_PIN, false);
}

void transparentModeRS422(){
    // ! use only if the communication is betwen no more than two devices
    PIO_PinWrite(PAYLOAD_RX_ENABLE_PIN, false);
    PIO_PinWrite(PAYLOAD_TX_ENABLE_PIN, true);
}

bool sendPayloadMessage(uint8_t* msg, size_t msg_size){
    bool status = false;

    if(sendPayloadStatus()){
        status = UART2_Write(msg, msg_size);
    }
    return status;
}

