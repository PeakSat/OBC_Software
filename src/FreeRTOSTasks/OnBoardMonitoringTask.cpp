#include "OnBoardMonitoringTask.hpp"

#include <EPS_Driver.hpp>
#include <HAL_I2C.hpp>

using namespace EPSConfiguration;
using namespace EPSParameters;
using namespace EPSParameters::ParameterDescriptors;
void OnBoardMonitoringTask::execute() {

    EPS eps;
    auto get = eps.getConfigurationParameter<getTypeSize(EPS_CH_STARTUP_ENA_BF_DESC.type)>(EPSParameters::ParameterDescriptors::EPS_CH_STARTUP_ENA_BF_DESC);
    StaticSemaphore_t DelayBuffer;

    SemaphoreHandle_t DelaySemaphore = xSemaphoreCreateBinaryStatic(&DelayBuffer);
    // LOG_DEBUG << "Runtime init: " << this->TaskName;
    auto& onBoardMonitoring = Services.onBoardMonitoringService;

auto s = eps.getSystemStatus();
    while (true) {
        xTaskNotifyWait(0, 0, nullptr, portMAX_DELAY);

        onBoardMonitoring.checkAll();


        LOG_INFO << "start ";
        xSemaphoreTake(DelaySemaphore, portMAX_DELAY);
        LOG_INFO << "finished ";
        if (onBoardMonitoring.getPMONDefinition(PeakSatParameters::OBCPCBTemperature1).get().checkingStatus != PMON::CheckingStatus::WithinLimits) {
            auto status = onBoardMonitoring.getPMONDefinition(PeakSatParameters::OBCPCBTemperature1).get().checkingStatus;
            if (status == PMON::CheckingStatus::BelowLowLimit) {}
            LOG_INFO << "parameter status: BelowLowLimit ";
        }
    }
}