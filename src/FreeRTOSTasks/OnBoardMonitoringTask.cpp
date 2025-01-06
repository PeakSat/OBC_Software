#include "OnBoardMonitoringTask.hpp"

void OnBoardMonitoringTask::execute() {

    // LOG_DEBUG << "Runtime init: " << this->TaskName;
    auto& onBoardMonitoring = Services.onBoardMonitoringService;


    while (true) {
        onBoardMonitoring.checkAll();
        if (onBoardMonitoring.getPMONDefinition(AcubeSATParameters::OBCPCBTemperature1).get().
                              checkingStatus != PMON::CheckingStatus::WithinLimits) {
            LOG_INFO << "parameter status: " << Services.onBoardMonitoringService.getPMONDefinition(
                AcubeSATParameters::OBCPCBTemperature1).get().checkingStatus;
        }
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}