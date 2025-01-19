#pragma once

#include "OBC_Definitions.hpp"
#include "PeakSatParameters.hpp"

namespace CommonParameters {
    inline auto& boardTemperature1 = OBDHParameters::PCBTemperature1;
    inline auto& boardTemperature2 = OBDHParameters::PCBTemperature2;
    inline auto& mcuTemperature = OBDHParameters::MCUTemperature;
    inline auto& time = OBDHParameters::OnBoardTime;
    inline auto& useRTT = PeakSatParameters::obcUseRTT;
    inline auto& useUART = PeakSatParameters::obcUseUART;
    inline auto& useCAN = PeakSatParameters::obcUseCAN;
} // namespace CommonParameters
