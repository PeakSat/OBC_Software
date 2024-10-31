#pragma once

#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h>                     // Defines EXIT_FAILURE
#include <inttypes.h>

// I2C Clock speed 400kHz

const uint16_t ADM_ADDRESS = 0x33;

/**
 * @brief          READ COMMAND
 *                  First byte
 * | MSB | 6  | 5  | 4  | 3 | 2 | 1  | LSB |
 * | D4  | D3 | D2 | D1 | 0 | M | S2 | S1  |
 * 
 * D1-4: Logical state of door      - [0: Closed, 1: Opened]
 * M   : Operation mode             - [0: Normal operation, 1: Test mode] 
 * S1-2: Current antenna algorithm  - [00: Alg_OFF, 01: Alg_1, 10: Alg_2]
 * 
 *                  Second byte
 * | MSB | 6  | 5  | 4  | 3  | 2  | 1  | LSB |
 * | A4  | A3 | A2 | A1 | B4 | B3 | B2 | B1  |
 * 
 * A1-4: Currently engaged main heaters     - [0: Heater OFF, 1: Heater ON]  
 * B1-4: Currently engaged back-up heaters  - [0: Heater OFF, 1: Heater ON]
 * 
 *                  Third byte
 *  Currently elapsed time in seconds
 * 
 *                  Fourth byte
 * |  MSB  |  6  |   5   |  4  |   3   |  2  |   1   | LSB |
 * | SW4.1 | SW4 | SW3.1 | SW3 | SW2.1 | SW2 | SW1.1 | SW1 |
 * 
 * SWx & SWx.1 represent the two feedback switches of antenna x rod
 * 
 * | SWx.1 | SWx | Logical State of Door Dx |
 * |   0   |  0  |           0              |
 * |   0   |  1  |           0              |
 * |   1   |  0  |           0              |
 * |   1   |  1  |           1              |
 * 
 */

const uint8_t DOOR_4_STATUS         =   7;      // MSB of first response byte
const uint8_t DOOR_3_STATUS         =   6;
const uint8_t DOOR_2_STATUS         =   5;
const uint8_t DOOR_1_STATUS         =   4;
const uint8_t RESERVED_BYTE_FB_1    =   3;      // Should be zero
const uint8_t OPERATION_MODE        =   2;      
const uint8_t CURR_ANT_ALGORITHM_S2 =   1;
const uint8_t CURR_ANT_ALGORITHM_S1 =   0;      // LSB of first response byte


const uint8_t MAIN_HEATER_4_STATUS      =   7;      // MSB of second response byte
const uint8_t MAIN_HEATER_3_STATUS      =   6;
const uint8_t MAIN_HEATER_2_STATUS      =   5;
const uint8_t MAIN_HEATER_1_STATUS      =   4;
const uint8_t BACKUP_HEATER_4_STATUS    =   3;      
const uint8_t BACKUP_HEATER_3_STATUS    =   2;      
const uint8_t BACKUP_HEATER_2_STATUS    =   1;
const uint8_t BACKUP_HEATER_1_STATUS    =   0;      // LSB of second response byte


const uint8_t SW_FB_41      =   7;      // MSB of fourth response byte
const uint8_t SW_FB_4       =   6;
const uint8_t SW_FB_31      =   5;
const uint8_t SW_FB_3       =   4;
const uint8_t SW_FB_21      =   3;      
const uint8_t SW_FB_2       =   2;      
const uint8_t SW_FB_11      =   1;
const uint8_t SW_FB_1       =   0;      // LSB of fourth response byte


/**
 * @brief          WRITE COMMAND
 *                  First byte
 * | MSB | 6 | 5  | 4  |  3   |  2   |  1   | LSB |
 * |  0  | 0 | S2 | S1 | Ant4 | Ant3 | Ant2 | Ant1|
 * 
 * Ant1-4: Select heaters to engage     - [0: Heater OFF, 1: Heater ON]
 * S1-2: Define deployment algorithms
 * | S2 | S1 | Command Name  |
 * | 0  | 0  | ALGORITHM_OFF |
 * | 0  | 1  | ALGORITHM_1   |
 * | 1  | 0  | ALGORITHM_2   | 
 * 
 */

const uint8_t RESERVED_BYTE_WC_2 =   7;      // MSB of first response byte (Should be zero)
const uint8_t RESERVED_BYTE_WC_3 =   6;      // Should be zero
const uint8_t DEPL_ALG_S2        =   5;
const uint8_t DEPL_ALG_S1        =   4;
const uint8_t ENGAGE_ANT_4       =   3;      
const uint8_t ENGAGE_ANT_3       =   2;      
const uint8_t ENGAGE_ANT_2       =   1;
const uint8_t ENGAGE_ANT_1       =   0;      // LSB of first response byte

/**
 * @brief Examples for write command, from the datasheet
 * Examples:
    0x1F - Execute Algorithm 1 for all four antenna rods
    0x2F - Execute Algorithm 2 for all four antenna rods
    0х21 - Execute Algorithm 2 only for the first antenna rod
    0x16 - Execute Algorithm 1 for the third and the second antenna rod
    0x00 - Clear and interrupt all recent commands
 * 
 */