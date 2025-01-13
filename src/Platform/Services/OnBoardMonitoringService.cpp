#include "ECSS_Configuration.hpp"
#include "PeakSatParameters.hpp"

#ifdef SERVICE_ONBOARDMONITORING

#include "Services/OnBoardMonitoringService.hpp"

void OnBoardMonitoringService::initializeParameterMonitoringMap() {
    static etl::array<PMONLimitCheck, MaximumNumberOfChecksLimitCheck> limitChecksArray{
        PMONLimitCheck(PeakSatParameters::OBCPCBTemperature1, 5, 10, 0, 40, 1)
    };
    for (auto& limitCheck: limitChecksArray) {
        limitCheck.monitoringEnabled = true;
        addPMONLimitCheck(limitCheck.monitoredParameterId, limitCheck);
    }
    static etl::array<PMONExpectedValueCheck, MaximumNumberOfChecksDeltaCheck> expectedValueChecksArray{
        PMONExpectedValueCheck(PeakSatParameters::OBCPCBTemperature1, 5, 28, 4, 0)
    };
    for (auto& expectedValueCheck: expectedValueChecksArray) {
        addPMONExpectedValueCheck(expectedValueCheck.monitoredParameterId, expectedValueCheck);
    }
    static etl::array<PMONDeltaCheck, MaximumNumberOfChecksDeltaCheck> deltaChecksArray{
        PMONDeltaCheck(PeakSatParameters::OBCPCBTemperature1, 5, 2, -3, 0, +3, 0)
    };
    for (auto& deltaCheck: deltaChecksArray) {
        addPMONDeltaCheck(deltaCheck.monitoredParameterId, deltaCheck);
    }
}

#endif