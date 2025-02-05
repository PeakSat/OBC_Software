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
                    OBCPCBTemperature1, OBCPCBTemperature2, OBCMCUTemperature, OBCMCUBootCounter, OBCMemoryPartition,
                    OBCMCUSystick, OBCCANBUSLoad1, OBCCANBUSLoad2, OBCCANBUSActive, OBCNANDFLASHLCLThreshold,
                    OBCMRAMLCLThreshold, OBCNANDFLASHON, OBCMRAMON, OBCUseCAN, OBCUseUART, OBCUseRTT, OBCOnBoardTime
                }
            },
            // OBC Mission Database parameters monitored every 3 sec
            {
                1, 3000, true,
                {
                    OBCPCBTemperature1, OBCPCBTemperature2, OBCSpacecraftTimeRef, OBCOperationalMode,
                    OBCMemoryPartition, OBCReconfigurationTimer, OBCLastFailedEvent, OBCMCUSystick
                }
            },
            // OBC Mission Database parameters not sampled
            {
                2, 5000, false,
                {
                    OBCMCUInputVoltage, OBCFlashInt, OBCSRAMInt, OBCAvailableMRAM, OBCAvailableNAND, OBCCANBUSLoad1,
                    OBCCANBUSLoad2, OBCCANBUSActive, OBCMCUFDIR, OBCMCURestartSafeModeThreshold,
                    OBCNANDFLASHLCLThreshold, OBCMRAMLCLThreshold, OBCNANDFLASHON, OBCNANDFLASHScrubbingFrequency,
                    OBCRAMScrubbingFrequency, OBCProgramFlashScrubbingFrequency
                }
            },
            {
                3, 5000, false,
                {
                    EPS_MODE, EPS_CONF, EPS_RESET_CAUSE, EPS_UPTIME, EPS_ERROR, EPS_RC_CNT_PWRON, EPS_RC_CNT_WDG,
                    EPS_RC_CNT_CMD, EPS_RC_CNT_MCU, EPS_RC_CNT_EMLOPO, EPS_PREVCMD_ELAPSED, EPS_UNIX_TIME,
                    EPS_UNIX_YEAR, EPS_UNIX_MONTH, EPS_UNIX_DAY, EPS_UNIX_HOUR, EPS_UNIX_MINUTE, EPS_UNIX_SECOND
                }
            },

            {
                4, 5000, false,
                {
                    COMMS_uhf_power_amp_tempID, COMMS_pcb_tempID, COMMS_gnss_tempID, COMMS_antenna_deployment_statusID,
                    COMMS_gnss_latID, COMMS_gnss_longID, COMMS_gnss_altID, COMMS_gnss_timeID, COMMS_gnss_ack_timeoutID,
                    COMMS_gnss_cmd_retriesID, COMMS_gnss_error_timeoutID, COMMS_error_timeout_cnt_thrhdID,
                    COMMS_gnss_delay_cmdsID, COMMS_satellites_trackedID, COMMS_gnss_fix_qualityID, COMMS_commit_hashID
                }
            },
            {
                5, 5000, false,
                {
                    PAY_xID, PAY_yID, PAY_zID, PAY_uptimeID, PAY_timeID, PAY_psu_12vID, PAY_psu_5vID, PAY_psu_33vID,
                    PAY_mcu_die_temperatureID, PAY_main_board_temperatureID, PAY_seed_ld_output_powerID,
                    PAY_fsm_chamber_temperatureID, PAY_fsm_chamber_pressureID, PAY_fsmd_voltage_converter_temperatureID,
                    PAY_fsmd_drivers_temperatureID, PAY_camera_pcb_temperatureID, PAY_camera_sensor_temperatureID,
                    PAY_fso_aux_temperature_1ID, PAY_fso_aux_temperature_2ID, PAY_oad_temperatureID,
                    PAY_optical_amplifier_combiner_heater_temperatureID,
                    PAY_optical_amplifier_fiber_mirror_temperatureID,
                    PAY_optical_amplifier_circulator_heater_temperatureID, PAY_ldd_temperatureID, PAY_ld_temperatureID,
                    PAY_ldd_12v_currentID, PAY_ldd_psu_12vID, PAY_ldd_converter_input_currentID, PAY_ldd_ld_vID,
                    PAY_ldd_ld_dac_set_vID, PAY_fsm_driver_12v_currentID, PAY_flashes_33v_currentID,
                    PAY_fpga_5v_currentID, PAY_sdd_33v_currentID, PAY_pump_ld_powerID, PAY_mcu_33v_currentID,
                    PAY_fpga_die_temperatureID, PAY_fpga_vdd1_voltageID, PAY_fpga_vdd18_voltageID,
                    PAY_fpga_vdd25_voltageID, PAY_seed_ld_incoming_powerID, PAY_amplifier_output_powerID,
                    PAY_amplifier_output_reflected_powerID, PAY_seed_diode_bias_currentID, PAY_tec_currentID,
                    PAY_ldd_output_currentID, PAY_sd_temperature_violationsID, PAY_end_uptimeID, PAY_responseID,
                    PAY_deviceID, PAY_firmwareID, PAY_firmware_is_confirmedID, PAY_bitstreamID,
                    PAY_softcpu_1_firmwareID, PAY_softcpu_2_firmwareID, PAY_softcpu_3_firmwareID,
                    PAY_softcpu_4_firmwareID, PAY_softcpu_5_firmwareID, PAY_softcpu_6_firmwareID,
                    PAY_softcpu_7_firmwareID, PAY_softcpu_8_firmwareID, PAY_boot_countID, PAY_transmission_countID,
                    PAY_hw_detID, PAY_storage0ID, PAY_storage1ID, PAY_storage2ID, PAY_storage3ID, PAY_ldd_faultID,
                    PAY_fsm_faultID, PAY_fpga_faultID, PAY_v_cam_faultID, PAY_sdd_faultID
                }
            },

        }
    };
} // namespace HousekeepingStructures

#endif
