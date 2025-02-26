#include "OnBoardMonitoringTask.hpp"
#include <EPS_Driver.hpp>
#include "adm_driver.hpp"


using namespace EPSConfiguration;
using namespace EPSParameters;
using namespace EPSParameters::ParameterDescriptors;


void OnBoardMonitoringTask::getMCUTemperature() {
    uint32_t adc_conversion = 0U; // Variable to store ADC result

    AFEC0_ConversionStart();
    xTaskNotifyWait(0U, UINT32_MAX, &adc_conversion, portMAX_DELAY);
    const float voltageConversion = static_cast<float>(adc_conversion) * PositiveVoltageReference / MaxADCValue;
    float mcuTemperature = (voltageConversion - TypicalVoltageAt25) / TemperatureSensitivity + ReferenceTemperature;

    LOG_DEBUG << "The temperature of the MCU is: " << mcuTemperature << " degrees Celsius";
    MemoryManager::setParameter(PeakSatParameters::OBDH_MCU_TEMPERATURE_ID, static_cast<void*>(&mcuTemperature));
}

void OnBoardMonitoringTask::checkAmbientSensors() {
    uint8_t numberOfDisconnectedSensors = 0;

    for (auto& sensor: sensors) {
        if (not sensor.isDeviceConnected().value()) {
            LOG_ERROR << "Temperature sensor with address " << sensor.getI2CUserAddress()
                      << " was disconnected";
            LOG_ERROR << "Error " << sensor.isDeviceConnected().value();
            numberOfDisconnectedSensors++;
        }
    }
    if (numberOfDisconnectedSensors == NumberOfTemperatureSensors) {
        LOG_ERROR << "Suspending ambient temperature task";
    }
}

void OnBoardMonitoringTask::getAmbientTemperature() {
    for (uint8_t sensorCounter = 0; sensorCounter < NumberOfTemperatureSensors; sensorCounter++) {
        if (not sensors[sensorCounter].isDeviceConnected().value()) {
            LOG_ERROR << "Temperature sensor with address " << sensors[sensorCounter].getI2CUserAddress()
                      << " was disconnected";
            continue;
        }
        ambientTemperature[sensorCounter] = sensors[sensorCounter].getTemperature().value();
    }

    MemoryManager::setParameter(PeakSatParameters::OBDH_PCB_TEMPERATURE_1_ID, static_cast<void*>(&ambientTemperature[0]));
    MemoryManager::setParameter(PeakSatParameters::OBDH_PCB_TEMPERATURE_2_ID, static_cast<void*>(&ambientTemperature[1]));

    MemoryManager::getParameter(PeakSatParameters::OBDH_PCB_TEMPERATURE_1_ID, static_cast<void*>(&ambientTemperature[0]));
    MemoryManager::getParameter(PeakSatParameters::OBDH_PCB_TEMPERATURE_2_ID, static_cast<void*>(&ambientTemperature[1]));
    LOG_INFO << "Sensor with address 0 responded with ambient temperature = " << ambientTemperature[0];
    LOG_INFO << "Sensor with address 1 responded with ambient temperature = " << ambientTemperature[1];
}


OnBoardMonitoringTask::OnBoardMonitoringTask() : Task("OnBoardMonitoring") {
    AFEC0_CallbackRegister(
        [](uint32_t status, uintptr_t context) -> void {
            BaseType_t higher_priority_task_woken = pdFALSE;
            // Ensure valid task handle
            if (onBoardMonitoringTaskHandle != nullptr) {
                const uint16_t adc_conversion = AFEC0_ChannelResultGet(AFEC_CH11);
                xTaskNotifyFromISR(onBoardMonitoringTaskHandle, adc_conversion, eSetValueWithOverwrite, &higher_priority_task_woken);

                portYIELD_FROM_ISR(higher_priority_task_woken);
            } else {
                // Log error if ISR is called without a valid task handle
                LOG_ERROR << "ISR called with null task handle!";
            }
        },
        reinterpret_cast<uintptr_t>(nullptr));
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
    checkAmbientSensors();

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
        LOG_INFO << "ADM Status: " << MemoryManager::getParameterAsUINT64(PeakSatParameters::COMMS_ANTENNA_DEPLOYMENT_STATUS_ID);
        vTaskDelay(10);
        getAmbientTemperature();
        vTaskDelay(10);
        getMCUTemperature();

        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}