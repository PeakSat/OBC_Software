#ifndef ECSS_SERVICES_HOUSEKEEPINGPARAMETERS_HPP
#define ECSS_SERVICES_HOUSEKEEPINGPARAMETERS_HPP

#include "PeakSatParameters.hpp"
#include "Services/HousekeepingService.hpp"

namespace HousekeepingStructures {

    static etl::array<HousekeepingStructure, ECSSMaxHousekeepingStructures> structures = {
        {
            // OBC Parameters that will be monitored in the campaign
            {0, 1000, true, {PCBTemperature1, PCBTemperature2, MCUTemperature}},
            // OBC Mission Database parameters monitored every 3 sec
            {1, 3000, true, {PCBTemperature1, PCBTemperature2, MCUTemperature}},
            // OBC Mission Database parameters not sampled
            {2, 5000, true, {PCBTemperature1, PCBTemperature2, MCUTemperature}},
        }};
} // namespace HousekeepingStructures

#endif
