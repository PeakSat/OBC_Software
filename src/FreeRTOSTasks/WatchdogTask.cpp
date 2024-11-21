#include "WatchdogTask.hpp"
#include "UARTGatekeeperTask.hpp"

void WatchdogTask::execute() {

    vTaskDelay(2000);

    while (true) {

        UBaseType_t highWatermarkWDT = uxTaskGetStackHighWaterMark(NULL);
        UBaseType_t highWatermarkUGK = uxTaskGetStackHighWaterMark(uartGatekeeperTask->taskHandle);

        LOG_DEBUG<<"WDT Watermark: "<<highWatermarkWDT;
        LOG_DEBUG<<"UARTGatekeeper Watermark: "<<highWatermarkUGK;

        //        LOG_DEBUG << "Runtime entered: " << this->TaskName;
        WDT_Clear();
        LOG_DEBUG << "Watchdog reset";

        //        LOG_DEBUG << "Runtime exit: " << this->TaskName;
        vTaskDelay(pdMS_TO_TICKS(WatchdogWindow));
    }
}
