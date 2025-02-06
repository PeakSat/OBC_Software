#ifndef ECSS_SERVICES_HOUSEKEEPINGPARAMETERS_HPP
#define ECSS_SERVICES_HOUSEKEEPINGPARAMETERS_HPP

#include "PeakSatParameters.hpp"
#include "Services/HousekeepingService.hpp"

namespace HousekeepingStructures {
    using namespace PeakSatParameters;

    static etl::array<HousekeepingStructure, ECSSMaxHousekeepingStructures> structures = {
        {
            // OBC Parameters that will be monitored in the campaign
            {
                0, 1000, true,
                {

                }
            },
            // OBC Mission Database parameters monitored every 3 sec
            {
                1, 3000, true,
                {

                }
            },
            {
                3, 5000, true,
                {

                }
            },

            {
                4, 5000, true,
                {

                }
            },
            {
                5, 5000, true,
                {


                }
            },

        }
    };
} // namespace HousekeepingStructures

#endif
