#include "ECSS_Configuration.hpp"
#include "AcubeSATParameters.hpp"

#ifdef SERVICE_ONBOARDMONITORING

#include "Services/OnBoardMonitoringService.hpp"

void OnBoardMonitoringService::initializeParameterMonitoringMap() {
    static etl::array<PMONLimitCheck, MaximumNumberOfChecksLimitCheck> limitChecksArray{
        PMONLimitCheck(AcubeSATParameters::OBCPCBTemperature1, 5, 10, 0, 40, 1)
    };
    for (auto& limitCheck: limitChecksArray) {
        addPMONLimitCheck(limitCheck.monitoredParameterId, limitCheck);
        limitCheck.monitoringEnabled = true;
    }
    static etl::array<PMONExpectedValueCheck, MaximumNumberOfChecksDeltaCheck> expectedValueChecksArray{
        PMONExpectedValueCheck(AcubeSATParameters::OBCPCBTemperature1, 5, 28, 4, 0)
    };
    for (auto& expectedValueCheck: expectedValueChecksArray) {
        addPMONExpectedValueCheck(expectedValueCheck.monitoredParameterId, expectedValueCheck);
    }
    static etl::array<PMONDeltaCheck, MaximumNumberOfChecksDeltaCheck> deltaChecksArray{
        PMONDeltaCheck(AcubeSATParameters::OBCPCBTemperature1, 5, 2, -3, 0, +3, 0)
    };
    for (auto& deltaCheck: deltaChecksArray) {
        addPMONDeltaCheck(deltaCheck.monitoredParameterId, deltaCheck);
    }
}

#endif