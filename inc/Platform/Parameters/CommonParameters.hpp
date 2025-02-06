#pragma once

#include "OBC_Definitions.hpp"
#include "PeakSatParameters.hpp"

const uint16_t YES = 1;

namespace CommonParameters {
    inline auto& boardTemperature1 = PeaksatParameters::PCBTemperature1ID;
    inline auto& boardTemperature2 = PeaksatParameters::PCBTemperature2ID;
    inline auto& mcuTemperature = PeaksatParameters::MCUTemperatureID;
    inline auto& time = PeaksatParameters::OnBoardTimeID;
    inline auto& useRTT = PeaksatParameters::UseRTTID;
    inline auto& useUART = YES;
    inline auto& useCAN = PeaksatParameters::UseCANID;
} // namespace CommonParameters
