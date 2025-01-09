#include "OnBoardMonitoringTask.hpp"

void OnBoardMonitoringTask::execute() {

    // LOG_DEBUG << "Runtime init: " << this->TaskName;
    auto& onBoardMonitoring = Services.onBoardMonitoringService;


    while (true) {
        onBoardMonitoring.checkAll();
        if (onBoardMonitoring.getPMONDefinition(AcubeSATParameters::OBCPCBTemperature1).get().checkingStatus != PMON::CheckingStatus::WithinLimits) {
            auto status = onBoardMonitoring.getPMONDefinition(AcubeSATParameters::OBCPCBTemperature1).get().checkingStatus;
            if (status == PMON::CheckingStatus::BelowLowLimit) {}
            // LOG_INFO << "parameter status: BelowLowLimit ";
        }
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}