#pragma once

#include "OBC_Definitions.hpp"
#include "PeakSatParameters.hpp"

constexpr uint16_t YES = 1;

namespace CommonParameters {
    inline auto& boardTemperature1 = PeakSatParameters::PCBTemperature1ID;
    inline auto& boardTemperature2 = PeakSatParameters::PCBTemperature2ID;
    inline auto& mcuTemperature = PeakSatParameters::MCUTemperatureID;
    inline auto& time = PeakSatParameters::OnBoardTimeID;
    inline auto& useRTT = PeakSatParameters::UseRTTID;
    inline auto& useUART = YES;
    inline auto& useCAN = PeakSatParameters::UseCANID;
} // namespace CommonParameters