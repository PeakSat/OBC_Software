#pragma once

#include "adm_configs.hpp"
#include <cstring>
#include "peripheral/twihs/master/plib_twihs0_master.h"

enum DEPLOYMENT_ALGORITHMS{
    ALGORITHM_OFF,
    ALGORITHM_1,
    ALGORITHM_2
};

enum ADM_ERRNO{
    NO_ERROR,
    I2C_BUS_BUSY,
    I2C_RW_FAILED,
    I2C_R_FAILED,
    I2C_W_FAILED
};

typedef struct{
    uint8_t first_byte;
    uint8_t second_byte;
    uint8_t third_byte;
    uint8_t fourth_byte;
}ADM_READ_BYTES;

void storeMsgADM(char* msg, size_t len);

char* getMsgADM();

uint8_t get_ADM_write_byte(DEPLOYMENT_ALGORITHMS alg, bool en_h1, bool en_h2, bool en_h3, bool en_h4);

ADM_ERRNO read_command_ADM(ADM_READ_BYTES *response);

ADM_ERRNO write_command_ADM(uint8_t writeByte);
