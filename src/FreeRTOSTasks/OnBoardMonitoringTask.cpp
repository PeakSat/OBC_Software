#include "OnBoardMonitoringTask.hpp"

#include <EPS_Driver.hpp>
#include "adm_driver.hpp"




using namespace EPSConfiguration;
using namespace EPSParameters;
using namespace EPSParameters::ParameterDescriptors;



void OnBoardMonitoringTask::execute() {

    EPS eps;
    // ADM::admI2CdrvHandle = DRV_I2C_Open(
    //             DRV_I2C_INDEX_2, static_cast<DRV_IO_INTENT>(DRV_IO_INTENT_READWRITE | DRV_IO_INTENT_BLOCKING));
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
        if (get != EPS::ErrorCode::None) {
            LOG_ERROR << "EPS getStatus failed, error:" << static_cast<EPS::ErrorCode_t>(get);
        }
        LOG_INFO << "EPS time: " << MemoryManager::getParameterAsUINT64(PeakSatParameters::EPS_UNIX_MINUTE_ID) << " : " << MemoryManager::getParameterAsUINT64(PeakSatParameters::EPS_UNIX_SECOND_ID);
        get = eps.getPIUHousekeepingDataRaw();
        updatePayloadParameters();
        vTaskDelay(10);
        ADM::getADMParameters();
        LOG_INFO<<"ADM Status: "<< MemoryManager::getParameterAsUINT64(PeakSatParameters::COMMS_ANTENNA_DEPLOYMENT_STATUS_ID);
        vTaskDelay(10);

        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}