#pragma once

#include "OBC_Definitions.hpp"
#include "PeakSatParameters.hpp"

namespace CommonParameters {
    inline auto& boardTemperature1 = PCBTemperature1;
    inline auto& boardTemperature2 = PCBTemperature2;
    inline auto& mcuTemperature = MCUTemperature;
    inline auto& time = OnBoardTime;
    inline auto& useRTT = pUseRTT;
    inline auto& useUART = pUseUART;
    inline auto& useCAN = pUseCAN;
} // namespace CommonParameters
