#pragma once

#include "OBC_Definitions.hpp"
#include "PeakSatParameters.hpp"

namespace CommonParameters {
    inline auto& boardTemperature1 = PeakSatParameters::obcPCBTemperature1;
    inline auto& boardTemperature2 = PeakSatParameters::obcPCBTemperature2;
    inline auto& mcuTemperature = PeakSatParameters::obcMCUTemperature;
    inline auto& time = PeakSatParameters::obcOnBoardTime;
    inline auto& useRTT = PeakSatParameters::obcUseRTT;
    inline auto& useUART = PeakSatParameters::obcUseUART;
    inline auto& useCAN = PeakSatParameters::obcUseCAN;
} // namespace CommonParameters
