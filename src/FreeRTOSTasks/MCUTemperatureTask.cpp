#include "MCUTemperatureTask.hpp"

void MCUTemperatureTask::execute() {
    // LOG_DEBUG << "Runtime init: " << this->TaskName;

    while (true) {
        //        LOG_DEBUG << "Runtime entered: " << this->TaskName;
        AFEC0_ConversionStart();
        vTaskDelay(pdMS_TO_TICKS(1));
        uint16_t ADCconversion = AFEC0_ChannelResultGet(AFEC_CH11);
        float voltageConversion = static_cast<float>(ADCconversion) * PositiveVoltageReference / MaxADCValue;
        float MCUtemperature = (voltageConversion - TypicalVoltageAt25) / TemperatureSensitivity + ReferenceTemperature;
        LOG_DEBUG << "The temperature of the MCU is: " << MCUtemperature << " degrees Celsius";

        memManTask->setParameter(PeaksatParameters::MCUTemperatureID, static_cast<void*>(&MCUtemperature));

        vTaskDelay(pdMS_TO_TICKS(delayMs));
    }
}
