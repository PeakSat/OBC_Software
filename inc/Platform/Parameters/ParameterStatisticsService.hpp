#pragma once

#include "Services/ParameterStatisticsService.hpp"
#include "PeakSatParameters.hpp"

namespace ParameterStatistics {
    /**
     * The number of Statistic instances that will be created
     */
    const uint8_t NumOfStatistics = 2;

    /**
     * An array of parameter IDs. Statistics will be logged for each of these parameters
     */
    inline const etl::array<uint16_t, NumOfStatistics> statisticParameterIDs = {PeakSatParameters::OBDH_PCB_TEMPERATURE_1_ID, PeakSatParameters::OBDH_PCB_TEMPERATURE_2_ID};
    /**
     * An array holding Statistic instances. Each Statistic will correspond to the similarly-numbered parameter from statisticParameterIDs
     * This is used in the initializeStatisticsMap function, which is called in the constructor of the ST04
     * Service
     */
    inline etl::array<Statistic, NumOfStatistics> statistics = {};
} // namespace ParameterStatistics