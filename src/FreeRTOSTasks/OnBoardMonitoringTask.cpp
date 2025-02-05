#include "OnBoardMonitoringTask.hpp"

#include <EPS_Driver.hpp>


using namespace EPSConfiguration;
using namespace EPSParameters;
using namespace EPSParameters::ParameterDescriptors;


void OnBoardMonitoringTask::execute() {

    EPS eps;
    auto get = eps.getConfigurationParameter<getTypeSize(EPS_CH_STARTUP_ENA_BF_DESC.type)>(EPSParameters::ParameterDescriptors::EPS_CH_STARTUP_ENA_BF_DESC);

    auto& onBoardMonitoring = Services.onBoardMonitoringService;

auto s = eps.getSystemStatus();
    while (true) {
        onBoardMonitoring.checkAll();
        if (onBoardMonitoring.getPMONDefinition(PeakSatParameters::OBCPCBTemperature1).get().checkingStatus != PMON::CheckingStatus::WithinLimits) {
            auto status = onBoardMonitoring.getPMONDefinition(PeakSatParameters::OBCPCBTemperature1).get().checkingStatus;
            if (status == PMON::CheckingStatus::BelowLowLimit) {
                LOG_INFO << "parameter status: BelowLowLimit ";
            }
        }
        vTaskDelay(1000);
    }
}