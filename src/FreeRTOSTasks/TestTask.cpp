#include "TestTask.hpp"
#include "task.h"

TestTask::TestTask() : Task("TestTask") {
    // LOG_INFO << "Initialised instance of TestTask";
}

// void bytes_to_hex_string(unsigned char byte1, unsigned char byte2, unsigned char byte3, unsigned char byte4, unsigned char byte5) {
//     // Allocate enough space for 4 "0xFF " patterns + 1 null terminator
//     if (!result);  // Check if allocation was successful

//     // Format each byte into "0xXX " and concatenate
//     snprintf(result, 5 * 5 + 1, "0x%02X 0x%02X 0x%02X 0x%02X 0x%02X", byte1, byte2, byte3, byte4, byte5);
// }

void TestTask::execute() {
    vTaskDelay(pdMS_TO_TICKS(this->delayMs));
    LOG_INFO << "TestTask started";

    while (true) {
        LOG_DEBUG << "Test Task executed";
        vTaskDelay(pdMS_TO_TICKS(this->delayMs));
    }
}