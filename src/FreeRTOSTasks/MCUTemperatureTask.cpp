#include "MCUTemperatureTask.hpp"

MCUTemperatureTask::MCUTemperatureTask(): Task("MCUTemperatureTask") {
    AFEC0_CallbackRegister(
        [](uint32_t status, uintptr_t context) -> void {
            BaseType_t higher_priority_task_woken = pdFALSE;
            // Ensure valid task handle
            if (AFEC0HandlingTaskHandle != nullptr) {
                const uint16_t adc_conversion = AFEC0_ChannelResultGet(AFEC_CH11);
                xTaskNotifyFromISR(AFEC0HandlingTaskHandle, adc_conversion, eSetValueWithOverwrite, &higher_priority_task_woken);
                portYIELD_FROM_ISR(higher_priority_task_woken);
            } else {
                // Log error if ISR is called without a valid task handle
                LOG_ERROR << "ISR called with null task handle!";
            }
        },reinterpret_cast<uintptr_t>(nullptr)
    );
}


void MCUTemperatureTask::execute() const {
    uint32_t adc_conversion = 0U;  // Variable to store ADC result

    while (true) {

        AFEC0_ConversionStart();
        xTaskNotifyWait(0U, UINT32_MAX, &adc_conversion, portMAX_DELAY);
        const float voltageConversion = static_cast<float>(adc_conversion) * PositiveVoltageReference / MaxADCValue;
        const float mcuTemperature =
                (voltageConversion - TypicalVoltageAt25) / TemperatureSensitivity + ReferenceTemperature;
        LOG_DEBUG << "The temperature of the MCU is: " << mcuTemperature << " degrees Celsius";
        CommonParameters::mcuTemperature.setValue(mcuTemperature);
        vTaskDelay(pdMS_TO_TICKS(delayMs));
    }
}
