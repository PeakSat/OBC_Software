#include "OnBoardMonitoringTask.hpp"
#include "EPS_Driver.hpp"

void OnBoardMonitoringTask::execute() {
    auto &onBoardMonitoring = Services.onBoardMonitoringService;
    EPS eps;
    constexpr uint32_t OBC_RED_COMMSbitMaskBin = 0b00000000000000000100000011000000;
    constexpr uint32_t OBC_RED_COMMSbitMaskHex = 0x40C0;

    auto status = eps.getSystemStatus();
    SYSTICK_DelayMs(30);
    auto powerOn = eps.outputBusGroupOn({
        EPS::EPSChannels::OBC_3V3, EPS::EPSChannels::COMMS_12V, EPS::EPSChannels::RED_3V3
    });
    SYSTICK_DelayMs(30);
    const auto epsChannelConfig = eps.setConfigurationParameter(
        EPSParameters::ParameterDescriptors::EPS_CH_STARTUP_ENA_BF_DESC, OBC_RED_COMMSbitMaskHex);
    SYSTICK_DelayMs(30);
    auto epsConfigCheck = eps.getConfigurationParameter(
        EPSParameters::ParameterDescriptors::EPS_CH_STARTUP_ENA_BF_DESC);
    if (epsChannelConfig == OBC_RED_COMMSbitMaskHex) {
        LOG_INFO << "EPS Channels configured successfully, saving config...";
        SYSTICK_DelayMs(30);
        status = eps.saveConfiguration();
        if (status.value() == EPSConfiguration::STAT::ACCEPTED || status.value() == EPSConfiguration::STAT::NEW) {
            LOG_INFO << "EPS Channels configuration saved successfully";
        } else {
            LOG_ERROR << "Failed to save configuration, with error code:" << status.error();
        }
    }
    auto status2 = onBoardMonitoring.getPMONDefinition(PeakSatParameters::OBCPCBTemperature1).get().checkingStatus;
    while (true) {
        status = eps.getSystemStatus();
        onBoardMonitoring.checkAll();
        if (onBoardMonitoring.getPMONDefinition(PeakSatParameters::OBCPCBTemperature1).get().
                              checkingStatus != PMON::CheckingStatus::WithinLimits) {
            auto status = onBoardMonitoring.getPMONDefinition(PeakSatParameters::OBCPCBTemperature1).get().checkingStatus;
            if (status == PMON::CheckingStatus::BelowLowLimit) {}
                          LOG_INFO << "parameter status: BelowLowLimit ";
            checkingStatus != PMON::CheckingStatus::WithinLimits) {
            status2 = onBoardMonitoring.getPMONDefinition(PeakSatParameters::OBCPCBTemperature1).get().
                    checkingStatus; //TODO: ADD MORE
            if (status2 == PMON::CheckingStatus::BelowLowLimit) {
                LOG_INFO << "parameter status: BelowLowLimit ";
            }
            if (status2 == PMON::CheckingStatus::AboveHighLimit) {
                LOG_INFO << "parameter status: AboveHighLimit ";
            }
        }
        LOG_INFO << "EPS Day: " << PeakSatParameters::epsUNIX_DAY << " Time: " << PeakSatParameters::epsUNIX_TIME;
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
