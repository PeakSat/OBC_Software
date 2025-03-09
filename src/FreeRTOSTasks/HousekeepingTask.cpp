#include "FreeRTOSTasks/HousekeepingTask.hpp"

void HousekeepingTask::execute() {
    vTaskDelay(pdMS_TO_TICKS(3000));
    auto &housekeeping = Services.housekeeping;
    Time::DefaultCUC nextCollection(0);
    Time::DefaultCUC timeBeforeDelay(0);
    TickType_t xLastWakeTime = xTaskGetTickCount();

    while (true) {
        Time::DefaultCUC TaskGetTickCountCUC(pdTICKS_TO_MS(xTaskGetTickCount()));
        nextCollection = housekeeping.reportPendingStructures(TaskGetTickCountCUC, timeBeforeDelay, nextCollection);
        timeBeforeDelay = TaskGetTickCountCUC;
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(nextCollection.formatAsBytes()));
    }
}

