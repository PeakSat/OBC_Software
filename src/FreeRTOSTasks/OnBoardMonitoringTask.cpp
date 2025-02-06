#include "OnBoardMonitoringTask.hpp"

#include <EPS_Driver.hpp>


using namespace EPSConfiguration;
using namespace EPSParameters;
using namespace EPSParameters::ParameterDescriptors;


void OnBoardMonitoringTask::execute() {

    EPS eps;

    auto get = eps.getConfigurationParameter<getTypeSize(EPS_CH_STARTUP_ENA_BF_DESC.type)>(EPS_CH_STARTUP_ENA_BF_DESC);
    if (get != EPS::ErrorCode::None) {
        LOG_ERROR<<"EPS_CH_STARTUP_ENA_BF_DESC not set, error:" << static_cast<EPS::ErrorCode_t>(get);
    }

    auto& onBoardMonitoring = Services.onBoardMonitoringService;
    get = eps.outputBusChannelOn(EPS::EPSChannels::COMMS_12V);
    // get = eps.setConfigParamWithKey<>()
    while (true) {
        vTaskDelay(10);
        get = eps.getSystemStatus();
        if (get != EPS::ErrorCode::None) {
            LOG_ERROR<<"EPS getStatus failed, error:" << static_cast<EPS::ErrorCode_t>(get);
        }
        LOG_INFO<< "EPS Time: "<<PeakSatParameters::epsUNIX_MINUTE.getValue() << " : " << PeakSatParameters::epsUNIX_SECOND.getValue();
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