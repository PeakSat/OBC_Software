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
                0,
                1000,
                true,
                {
                    debugCounterID,
                    PCBTemperature1ID,
                    PCBTemperature2ID,
                    MCUTemperatureID,
                    MCUInputVoltageID,
                    MCUSystickID,
                    LastFailedEventID,
                    MCUBootCounterID,
                    OperationalModeID,
                }
            },
            // OBC Mission Database parameters monitored every 3 sec
            {
                1,
                3000,
                true,
                {
                    EPS_MODEID,
                    EPS_CONFID,
                    EPS_RESET_CAUSEID,
                    EPS_UPTIMEID,
                    EPS_ERRORID,
                    EPS_RC_CNT_PWRONID,
                    EPS_RC_CNT_WDGID,
                    EPS_RC_CNT_CMDID,
                    EPS_RC_CNT_MCUID,
                    EPS_RC_CNT_EMLOPOID,
                    EPS_PREVCMD_ELAPSEDID,
                    EPS_UNIX_TIMEID,
                    EPS_UNIX_YEARID,
                    EPS_UNIX_MONTHID,
                    EPS_UNIX_DAYID,
                    EPS_UNIX_HOURID,
                    EPS_UNIX_MINUTEID,
                    EPS_UNIX_SECONDID,
                }
            },
            {3, 5000, true, {
                cw_intervalID,
                bfsk_beacon_intervalID,
                uhf_tx_powerID,
                rssiID,
                gnss_latID,
                gnss_longID,
                gnss_altID,
                gnss_timeID,
                gnss_ack_timeoutID,
                gnss_cmd_retriesID,
                gnss_error_timeoutID,
                error_timeout_cnt_thrhdID,
                gnss_delay_cmdsID,
                satellites_trackedID,
                gnss_fix_qualityID,
                gnss_TM_data_periodID,
                commit_hashID,
                pointing_offset_xID,
                pointing_offset_yID,
                pointing_offset_zID,
                uptimeID,
                timeID, psu_12vID, psu_5vID, psu_33vID, mcu_die_temperatureID

                            }
            },
            //
            {
                4,
                5000,
                true,
                {
                    fsm_chamber_temperatureID,
                    fsm_chamber_pressureID,
                    fsmd_voltage_converter_temperatureID,
                    fsmd_drivers_temperatureID,
                    camera_pcb_temperatureID,
                    camera_sensor_temperatureID,
                    fso_aux_temperature_1ID,
                    fso_aux_temperature_2ID,
                    oad_temperatureID,
                    optical_amplifier_combiner_heater_temperatureID,
                    optical_amplifier_fiber_mirror_temperatureID,
                    optical_amplifier_circulator_heater_temperatureID,
                    ldd_temperatureID, ld_temperatureID,
                    ldd_12v_currentID, ldd_psu_12vID,
                    ldd_converter_input_currentID, ldd_ld_vID,
                    ldd_ld_dac_set_vID, fsm_driver_12v_currentID,
                    flashes_33v_currentID, fpga_5v_currentID, sdd_33v_currentID,
                    pump_ld_powerID, mcu_33v_currentID, fpga_die_temperatureID,
                    fpga_vdd1_voltageID, fpga_vdd18_voltageID, fpga_vdd25_voltageID,
                    seed_ld_incoming_powerID, amplifier_output_powerID,
                    amplifier_output_reflected_powerID, seed_diode_bias_currentID, tec_currentID,
                    ldd_output_currentID, sd_temperature_violationsID, end_uptimeID, responseID, deviceID,
                    firmwareID, firmware_is_confirmedID, bitstreamID, softcpu_1_firmwareID,
                    softcpu_2_firmwareID, softcpu_3_firmwareID, softcpu_4_firmwareID, softcpu_5_firmwareID,
                    softcpu_6_firmwareID, softcpu_7_firmwareID, softcpu_8_firmwareID, boot_countID,
                    transmission_countID, hw_detID, storage0ID, storage1ID, storage2ID, storage3ID,
                    ldd_faultID, fsm_faultID, fpga_faultID, v_cam_faultID, sdd_faultID, ResetTypeID
                }
            },
        }
    };
    // clang-format on
} // namespace HousekeepingStructures

#endif
