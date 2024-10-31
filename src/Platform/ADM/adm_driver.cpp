#include "FreeRTOS.h"
#include "task.h"
#include "adm_driver.hpp"
#include "adm_configs.hpp"

char _msg[20]; 

void storeMsgADM(char* msg, size_t len){
    if(len>20){
        return;
    }
    memcpy(_msg, msg, len);
}

char* getMsgADM(){
    return _msg;
}

void byteContToString(uint8_t byte, char* content){
   for(uint8_t i=7; i>=0; i--){
    if((byte>>i) & 1){
        content[7-i]='1';
    }else{
        content[7-i]='0';
    }
   } 
}

uint8_t get_ADM_write_byte(DEPLOYMENT_ALGORITHMS alg, bool en_h1, bool en_h2, bool en_h3, bool en_h4){
    uint8_t _generated_byte;
    switch (alg)
    {
    case DEPLOYMENT_ALGORITHMS::ALGORITHM_OFF:
        _generated_byte |= ( 0 << DEPL_ALG_S2 );
        _generated_byte |= ( 0 << DEPL_ALG_S1 );
        break;
    case DEPLOYMENT_ALGORITHMS::ALGORITHM_1:
        _generated_byte |= ( 0 << DEPL_ALG_S2 );
        _generated_byte |= ( 1 << DEPL_ALG_S1 );
        break;
    case DEPLOYMENT_ALGORITHMS::ALGORITHM_2:
        _generated_byte |= ( 1 << DEPL_ALG_S2 );
        _generated_byte |= ( 0 << DEPL_ALG_S1 );
        break;
    default:
        _generated_byte |= ( 1 << DEPL_ALG_S2 );
        _generated_byte |= ( 0 << DEPL_ALG_S1 );
        break;
    }

    _generated_byte |= en_h4 << ENGAGE_ANT_4;
    _generated_byte |= en_h3 << ENGAGE_ANT_3;
    _generated_byte |= en_h2 << ENGAGE_ANT_2;
    _generated_byte |= en_h1 << ENGAGE_ANT_1;

    return _generated_byte;
}

inline void waitForResponse() {
        auto start = xTaskGetTickCount();
        while (TWIHS0_IsBusy()) {
            if (xTaskGetTickCount() - start > 100) {
                TWIHS0_Initialize();
            }
            taskYIELD();
        }
};

ADM_ERRNO read_command_ADM(ADM_READ_BYTES *response){
    response->first_byte  = 0;
    response->second_byte = 0;
    response->third_byte  = 0;
    response->fourth_byte = 0;

    // if(TWIHS0_IsBusy()){
    //     return ADM_ERRNO::I2C_BUS_BUSY;
    // }

    uint8_t _res[4] = {0xFF};
    uint8_t _send_data = 0x00;

    // waitForResponse();
    // if(!TWIHS0_Write(ADM_ADDRESS, &_send_data, 1)){
    //     return ADM_ERRNO::I2C_W_FAILED;
    // }

    
    if(TWIHS0_Read(ADM_ADDRESS, _res, sizeof(_res))){
        waitForResponse();
        response->first_byte=_res[0];
        response->second_byte=_res[1];
        response->third_byte=_res[2];
        response->fourth_byte=_res[3];
        TWIHS_ERROR err = TWIHS0_ErrorGet();
        return (ADM_ERRNO) err;
    }
    TWIHS_ERROR err = TWIHS0_ErrorGet();
    return ADM_ERRNO::I2C_R_FAILED;
}



ADM_ERRNO write_command_ADM(uint8_t writeByte){
    if(!TWIHS0_Write(ADM_ADDRESS, &writeByte, 1)){
        return ADM_ERRNO::I2C_RW_FAILED;
    }
    return ADM_ERRNO::NO_ERROR;
}