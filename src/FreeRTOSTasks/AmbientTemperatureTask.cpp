#include "AmbientTemperatureTask.hpp"


void AmbientTemperatureTask::execute() {
    uint8_t numberOfDisconnectedSensors = 0;

    for (auto& sensor: sensors) {
        if (not sensor.isDeviceConnected()) {
            LOG_ERROR << "Temperature sensor with address " << sensor.getI2CUserAddress()
                      << " was disconnected";
            numberOfDisconnectedSensors++;
        }
    }

    if (numberOfDisconnectedSensors == NumberOfTemperatureSensors) {
        LOG_ERROR << "Suspending ambient temperature task";
        //        vTaskSuspend(taskHandle);
    }


    while (true) {
        for (uint8_t sensorCounter = 0; sensorCounter < NumberOfTemperatureSensors; sensorCounter++) {
            if (not sensors[sensorCounter].isDeviceConnected()) {
                LOG_ERROR << "Temperature sensor with address " << sensors[sensorCounter].getI2CUserAddress()
                          << " was disconnected";
                continue;
            }
            ambientTemperature[sensorCounter] = sensors[sensorCounter].getTemperature();
            // LOG_INFO << "Sensor with address" << sensors[sensorCounter].getI2CUserAddress() << " responded with ambient temperature = " << ambientTemperature[sensorCounter];
        }

        MemoryManager::setParameter(PeakSatParameters::PCB_TEMPERATURE_1ID, static_cast<void*>(&ambientTemperature[0]));
        MemoryManager::setParameter(PeakSatParameters::PCB_TEMPERATURE_2ID, static_cast<void*>(&ambientTemperature[1]));

        MemoryManager::getParameter(PeakSatParameters::PCB_TEMPERATURE_1ID, static_cast<void*>(&ambientTemperature[0]));
        MemoryManager::getParameter(PeakSatParameters::PCB_TEMPERATURE_2ID, static_cast<void*>(&ambientTemperature[1]));
        LOG_INFO << "Sensor with address 0 responded with ambient temperature = " << ambientTemperature[0];
        LOG_INFO << "Sensor with address 1 responded with ambient temperature = " << ambientTemperature[1];


        // Check if the operation was successful

        //        LOG_DEBUG << "Runtime is exiting: " << this->TaskName;
        vTaskDelay(pdMS_TO_TICKS(DelayMs));
    }
}
