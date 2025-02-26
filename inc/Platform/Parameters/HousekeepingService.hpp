#ifndef ECSS_SERVICES_HOUSEKEEPINGPARAMETERS_HPP
#define ECSS_SERVICES_HOUSEKEEPINGPARAMETERS_HPP

#include "PeakSatParameters.hpp"
#include "Services/HousekeepingService.hpp"

namespace HousekeepingStructures {
    using namespace PeakSatParameters;
    // clang-format off
    static etl::array<HousekeepingStructure, ECSSMaxHousekeepingStructures> structures = {
        {
            // OBC Parameters that will be monitored in the campaign
            {
                1,
                1000,
                true,
                {
                    OBDH_PCB_TEMPERATURE_1_ID,
                    OBDH_PCB_TEMPERATURE_2_ID,
                    OBDH_SPACECRAFT_TIME_REF_ID,
                    OBDH_OPERATIONAL_MODE_ID,
                    OBDH_MemoryPartition_ID,
                    OBDH_RECONFIGURATION_TIMER_ID,
                    OBDH_LAST_FAILED_EVENT_ID,
                    OBDH_MCU_SYSTICK_ID,
                }
            },
            // OBC Mission Database parameters monitored every 3 sec
            {
                2,
                3000,
                true,
                {
                    EPS_MODE_ID,
                    EPS_CONF_ID,
                    EPS_RESET_CAUSE_ID,
                    EPS_UPTIME_ID,
                    EPS_ERROR_ID,
                    EPS_RC_CNT_PWRON_ID,
                    EPS_RC_CNT_WDG_ID,
                    EPS_RC_CNT_CMD_ID,
                    EPS_RC_CNT_MCU_ID,
                    EPS_RC_CNT_EMLOPO_ID,
                    EPS_PREVCMD_ELAPSED_ID,
                    EPS_UNIX_TIME_ID,
                    EPS_UNIX_YEAR_ID,
                    EPS_UNIX_MONTH_ID,
                    EPS_UNIX_DAY_ID,
                    EPS_UNIX_HOUR_ID,
                    EPS_UNIX_MINUTE_ID,
                    EPS_UNIX_SECOND_ID,
                }
            },

            {
                3, 4000, true,
                {
                    EPS_MODE_ID,
                    EPS_CONF_ID,
                    EPS_RESET_CAUSE_ID,
                    EPS_UPTIME_ID,
                    EPS_ERROR_ID,
                    EPS_RC_CNT_PWRON_ID,
                    EPS_RC_CNT_WDG_ID,
                    EPS_RC_CNT_CMD_ID,
                    EPS_RC_CNT_MCU_ID,
                    EPS_RC_CNT_EMLOPO_ID,
                    EPS_PREVCMD_ELAPSED_ID,
                    EPS_UNIX_TIME_ID,
                    EPS_UNIX_YEAR_ID,
                    EPS_UNIX_MONTH_ID,
                    EPS_UNIX_DAY_ID,
                    EPS_UNIX_HOUR_ID,
                    EPS_UNIX_MINUTE_ID,
                    EPS_UNIX_SECOND_ID,
                    EPS_VOLT_BRDSUP_RAW_ID,
                    EPS_TEMP_MCU_RAW_ID,
                    EPS_VIP_VOLT_VD0_RAW_ID,
                    EPS_VIP_VOLT_VD1_RAW_ID,
                    EPS_VIP_VOLT_VD2_RAW_ID,
                    EPS_VIP_VOLT_VD3_RAW_ID,
                    EPS_VIP_VOLT_VD4_RAW_ID,
                    EPS_VIP_VOLT_VD5_RAW_ID,
                    EPS_VIP_VOLT_VD6_RAW_ID,
                    EPS_VIP_CURR_VD0_RAW_ID,
                    EPS_VIP_CURR_VD1_RAW_ID,
                    EPS_VIP_CURR_VD2_RAW_ID,
                    EPS_VIP_CURR_VD3_RAW_ID,
                    EPS_VIP_CURR_VD4_RAW_ID,
                    EPS_VIP_CURR_VD5_RAW_ID,
                    EPS_VIP_CURR_VD6_RAW_ID,
                    EPS_VIP_POWE_VD0_RAW_ID,
                    EPS_VIP_POWE_VD1_RAW_ID,
                    EPS_VIP_POWE_VD2_RAW_ID,
                    EPS_VIP_POWE_VD3_RAW_ID,
                    EPS_VIP_POWE_VD4_RAW_ID,
                    EPS_VIP_POWE_VD5_RAW_ID,
                    EPS_VIP_POWE_VD6_RAW_ID,
                    EPS_VIP_CH00_VOLT_RAW_ID,
                    EPS_VIP_CH00_CURR_RAW_ID,
                    EPS_VIP_CH00_POWE_RAW_ID,
                    EPS_VIP_CH01_VOLT_RAW_ID,
                    EPS_VIP_CH01_CURR_RAW_ID,
                    EPS_VIP_CH01_POWE_RAW_ID,
                    // EPS_VIP_CH02_VOLT_RAW_ID,
                    // EPS_VIP_CH02_CURR_RAW_ID,
                    // EPS_VIP_CH02_POWE_RAW_ID,
                    // EPS_VIP_CH03_VOLT_RAW_ID,
                    // EPS_VIP_CH03_CURR_RAW_ID,
                    // EPS_VIP_CH03_POWE_RAW_ID,
                    // EPS_VIP_CH04_VOLT_RAW_ID,
                    // EPS_VIP_CH04_CURR_RAW_ID,
                    // EPS_VIP_CH04_POWE_RAW_ID,
                    // EPS_VIP_CH05_VOLT_RAW_ID,
                    // EPS_VIP_CH05_CURR_RAW_ID,
                    // EPS_VIP_CH05_POWE_RAW_ID,
                    // EPS_VIP_CH06_VOLT_RAW_ID,
                    // EPS_VIP_CH06_CURR_RAW_ID,
                    // EPS_VIP_CH06_POWE_RAW_ID,
                    // EPS_VIP_CH07_VOLT_RAW_ID,
                    // EPS_VIP_CH07_CURR_RAW_ID,
                    // EPS_VIP_CH07_POWE_RAW_ID,
                    // EPS_VIP_CH08_VOLT_RAW_ID,
                    // EPS_VIP_CH08_CURR_RAW_ID,
                    // EPS_VIP_CH08_POWE_RAW_ID,
                    // EPS_VIP_CH09_VOLT_RAW_ID,
                    // EPS_VIP_CH09_CURR_RAW_ID,
                    // EPS_VIP_CH09_POWE_RAW_ID,
                    // EPS_VIP_CH10_VOLT_RAW_ID,
                    // EPS_VIP_CH10_CURR_RAW_ID,
                    // EPS_VIP_CH10_POWE_RAW_ID,
                    // EPS_VIP_CH11_VOLT_RAW_ID,
                    // EPS_VIP_CH11_CURR_RAW_ID,
                    // EPS_VIP_CH11_POWE_RAW_ID,
                    // EPS_VIP_CH12_VOLT_RAW_ID,
                    // EPS_VIP_CH12_CURR_RAW_ID,
                    // EPS_VIP_CH12_POWE_RAW_ID,
                    // EPS_VIP_CH13_VOLT_RAW_ID,
                    // EPS_VIP_CH13_CURR_RAW_ID,
                    // EPS_VIP_CH13_POWE_RAW_ID,
                    // EPS_VIP_CH14_VOLT_RAW_ID,
                    // EPS_VIP_CH14_CURR_RAW_ID,
                    // EPS_VIP_CH14_POWE_RAW_ID
                }
            },

            //
            {
                3,
                5000,
                true,
                {

                }
            },
        }
    };
    // clang-format on
} // namespace HousekeepingStructures

#endif