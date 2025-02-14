#include "OnBoardMonitoringTask.hpp"

#include <EPS_Driver.hpp>
#include "adm_driver.hpp"


using namespace EPSConfiguration;
using namespace EPSParameters;
using namespace EPSParameters::ParameterDescriptors;


void OnBoardMonitoringTask::execute() {

    EPS eps;

    auto get = eps.getConfigurationParameter<getTypeSize(EPS_CH_STARTUP_ENA_BF_DESC.type)>(EPS_CH_STARTUP_ENA_BF_DESC);
    if (get != EPS::ErrorCode::None) {
        LOG_ERROR << "EPS_CH_STARTUP_ENA_BF_DESC not set, error:" << static_cast<EPS::ErrorCode_t>(get);
    }

    auto& housekeeping = Services.housekeeping;
    auto& onBoardMonitoring = Services.onBoardMonitoringService;
    get = eps.outputBusChannelOn(EPS::EPSChannels::COMMS_12V);
    vTaskDelay(10);
    get = eps.outputBusChannelOn(EPS::EPSChannels::ADM_5V_1);
    vTaskDelay(10);
    while (true) {
        vTaskDelay(10);
        get = eps.getSystemStatus();
        vTaskDelay(10);
        get = eps.outputBusChannelOn(EPS::EPSChannels::COMMS_12V);
        if (get != EPS::ErrorCode::None) {
            LOG_ERROR << "EPS getStatus failed, error:" << static_cast<EPS::ErrorCode_t>(get);
        }
        LOG_INFO << "EPS time: " << MemManTask::getParameterAsUINT64(PeakSatParameters::EPS_UNIX_MINUTEID) << " : " << MemManTask::getParameterAsUINT64(PeakSatParameters::EPS_UNIX_SECONDID);
        updatePayloadParameters();
        vTaskDelay(10);
        ADM::getADMParameters();
        LOG_INFO<<"ADM Status: "<< MemManTask::getParameterAsUINT64(PeakSatParameters::antenna_deployment_statusID);
        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}