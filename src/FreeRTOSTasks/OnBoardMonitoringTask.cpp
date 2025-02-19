#include "OnBoardMonitoringTask.hpp"

#include <EPS_Driver.hpp>
#include "adm_driver.hpp"




using namespace EPSConfiguration;
using namespace EPSParameters;
using namespace EPSParameters::ParameterDescriptors;


MRAM mram2(SMC::NCS0);

void mramTest() {
    uint8_t testMramByte = 123;
    uint32_t MramA7Id       = 0b00000000000010000000;
    uint32_t MramA9Id       = 0b00000000001000000000;
    uint32_t MramA7or9Id    = 0b00000000001010000000;
    uint8_t readTestByte = 0;
    uint8_t readTestA9 = 0;
    uint8_t readTest97 = 0;

    mram2.mramReadByte(MramA9Id, readTestA9);
    LOG_DEBUG << "Mram read test byte A9 init: " << readTestA9;
    mram2.mramWriteByte(MramA7Id, testMramByte);
    mram2.mramReadByte(MramA7Id, readTestByte);
    LOG_DEBUG << "MramA7Id:" << readTestByte;
    mram2.mramReadByte(MramA9Id, readTestA9);
    LOG_DEBUG << "Mram read test byte A9 after A7: " << readTestA9;
    mram2.mramWriteByte(MramA9Id, testMramByte);
    mram2.mramReadByte(MramA9Id, readTestByte);
    LOG_DEBUG << "MramA9Id:" << readTestByte;
    mram2.mramReadByte(MramA7Id, readTestByte);
    LOG_DEBUG << "MramA7Id after A9 write:" << readTestByte;
    mram2.mramReadByte(MramA7or9Id, readTest97);
    LOG_DEBUG << "MramA7 or A9 Id after A9 write:" << readTest97;
    MemoryManager::setParameter(789, &readTest97);
}
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