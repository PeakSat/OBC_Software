#include "TestTask.hpp"
#include "task.h"
#include "adm_driver.hpp"
#include "UARTGatekeeperTask.hpp"

TestTask::TestTask() : Task("TestTask") {
    LOG_INFO << "Initialised instance of TestTask";
}

void handlePayloadMessage(char* msg, size_t len){
    LOG_DEBUG<<"MSG["<<msg<<"]";
    if(len < 7){
        LOG_DEBUG<<"Unknown command";
        return;
    }
    char id = *(msg+0);
    char cmd = *(msg+1);
    char da = *(msg+2);
    char h1 = *(msg+3);
    char h2 = *(msg+4);
    char h3 = *(msg+5);
    char h4 = *(msg+6);

    bool h1_s = false;
    bool h2_s = false;
    bool h3_s = false;
    bool h4_s = false;

    uint8_t _b = 0xFF;

    if(id == 'A'){
        if(cmd == 'R'){
            LOG_DEBUG << "Send read command";
            return;
        }else if(cmd == 'W'){
            if(h1 == '1'){
                h1_s = true;
            }
            if(h2 == '1'){
                h2_s = true;
            }
            if(h3 == '1'){
                h3_s = true;
            }
            if(h4 == '1'){
                h4_s = true;
            }
            if(da == '0'){
                _b = get_ADM_write_byte(DEPLOYMENT_ALGORITHMS::ALGORITHM_OFF, h1_s, h2_s, h3_s, h4_s);
            }else if(da == '1'){
                _b = get_ADM_write_byte(DEPLOYMENT_ALGORITHMS::ALGORITHM_1, h1_s, h2_s, h3_s, h4_s);
            }else if(da == '2'){
                _b = get_ADM_write_byte(DEPLOYMENT_ALGORITHMS::ALGORITHM_2, h1_s, h2_s, h3_s, h4_s);
            }
            return;
        }
    }
    LOG_DEBUG<<"Unknown command";
}

char result[26] = {'A'};

void bytes_to_hex_string(unsigned char byte1, unsigned char byte2, unsigned char byte3, unsigned char byte4, unsigned char byte5) {
    // Allocate enough space for 4 "0xFF " patterns + 1 null terminator
    if (!result);  // Check if allocation was successful

    // Format each byte into "0xXX " and concatenate
    snprintf(result, 5 * 5 + 1, "0x%02X 0x%02X 0x%02X 0x%02X 0x%02X", byte1, byte2, byte3, byte4, byte5);
}

void TestTask::execute() {
    // uint32_t ulNotifiedValue;
    LOG_INFO << "TestTask started";
    while (true) {
        // xTaskNotifyWait(0, 0, &ulNotifiedValue, portMAX_DELAY);
        // handlePayloadMessage(getMsgADM(), 20);
        vTaskDelay(pdMS_TO_TICKS(3000));
        for(uint8_t i=0;i<50;i++){
            ADM_READ_BYTES res;
            LOG_DEBUG<<"BFR_RES:"<<result;
            ADM_ERRNO adm_err = read_command_ADM (&res);
            
            bytes_to_hex_string(adm_err, res.first_byte, res.second_byte, res.third_byte, res.fourth_byte);
            LOG_DEBUG<<"RES:"<<result;
            result[0]='F';
            result[1]='\0';

            LOG_DEBUG<<"Z_RES:"<<result;
        }
        
        // LOG_DEBUG<<"RES B2:"<<res.second_byte;
        // LOG_DEBUG<<"RES B3:"<<res.third_byte;
        // LOG_DEBUG<<"RES B4:"<<res.fourth_byte;

        // vTaskSuspend(NULL);


        LOG_INFO << "TestTask finished";
    }
}