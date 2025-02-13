#include "OnBoardMonitoringTask.hpp"

#include <EPS_Driver.hpp>
#include "adm_driver.hpp"


using namespace EPSConfiguration;
using namespace EPSParameters;
using namespace EPSParameters::ParameterDescriptors;

void updatePayloadParameters() {
    req_get_main_telemetries req_main;
    res_get_main_telemetries res_main;
    if (!PayloadGatekeeperTask->sendrecvPayload(req_main.req_code, static_cast<void*>(&req_main), static_cast<void*>(&res_main))) {
        return;
    }
    MemManTask::setParameter(PeakSatParameters::timeID, static_cast<void*>(&res_main.time));
    MemManTask::setParameter(PeakSatParameters::psu_12vID, static_cast<void*>(&res_main.psu_12V));
    MemManTask::setParameter(PeakSatParameters::psu_5vID, static_cast<void*>(&res_main.psu_5V));
    MemManTask::setParameter(PeakSatParameters::psu_33vID, static_cast<void*>(&res_main.psu_33V));
    MemManTask::setParameter(PeakSatParameters::mcu_die_temperatureID, static_cast<void*>(&res_main.MCU_Die_Temp));
    MemManTask::setParameter(PeakSatParameters::main_board_temperatureID, static_cast<void*>(&res_main.Main_Board_Temp));
    MemManTask::setParameter(PeakSatParameters::seed_ld_output_powerID, static_cast<void*>(&res_main.Seed_ld_output_pow));

    MemManTask::setParameter(PeakSatParameters::fsm_chamber_temperatureID, static_cast<void*>(&res_main.FSM_Chamber_Temp));
    MemManTask::setParameter(PeakSatParameters::fsm_chamber_pressureID, static_cast<void*>(&res_main.FSM_Chamber_Pressure));
    MemManTask::setParameter(PeakSatParameters::fsmd_voltage_converter_temperatureID, static_cast<void*>(&res_main.FSMD_Voltage_Converter_Temp));
    MemManTask::setParameter(PeakSatParameters::fsmd_drivers_temperatureID, static_cast<void*>(&res_main.FSMD_Drivers_Temp));
    MemManTask::setParameter(PeakSatParameters::camera_pcb_temperatureID, static_cast<void*>(&res_main.Camera_PCB_Temp));
    MemManTask::setParameter(PeakSatParameters::fso_aux_temperature_1ID, static_cast<void*>(&res_main.FSO_AUX_Temp_1));
    MemManTask::setParameter(PeakSatParameters::fso_aux_temperature_2ID, static_cast<void*>(&res_main.FSO_AUX_Temp_2));

    MemManTask::setParameter(PeakSatParameters::ldd_12v_currentID, static_cast<void*>(&res_main.ldd_12V_Curr));
    MemManTask::setParameter(PeakSatParameters::fsm_driver_12v_currentID, static_cast<void*>(&res_main.FSM_Driver_12V_Curr));
    MemManTask::setParameter(PeakSatParameters::flashes_33v_currentID, static_cast<void*>(&res_main.Flashes_33v_Curr));
    MemManTask::setParameter(PeakSatParameters::fpga_5v_currentID, static_cast<void*>(&res_main.FPGA_5V_Curr));
    MemManTask::setParameter(PeakSatParameters::sdd_33v_currentID, static_cast<void*>(&res_main.SDD_33V_Curr));
    MemManTask::setParameter(PeakSatParameters::mcu_33v_currentID, static_cast<void*>(&res_main.MCU_33V_Curr));
    MemManTask::setParameter(PeakSatParameters::seed_diode_bias_currentID, static_cast<void*>(&res_main.Seed_Diode_Bias_Curr));

    MemManTask::setParameter(PeakSatParameters::tec_currentID, static_cast<void*>(&res_main.Tec_Curr));
    MemManTask::setParameter(PeakSatParameters::sd_temperature_violationsID, static_cast<void*>(&res_main.Sd_Temp_Violations));

    LOG_DEBUG << "PAYLOAD Parameters Updated";
}



void OnBoardMonitoringTask::execute() {

    EPS eps;

    auto get = eps.getConfigurationParameter<getTypeSize(EPS_CH_STARTUP_ENA_BF_DESC.type)>(EPS_CH_STARTUP_ENA_BF_DESC);
    if (get != EPS::ErrorCode::None) {
        LOG_ERROR << "EPS_CH_STARTUP_ENA_BF_DESC not set, error:" << static_cast<EPS::ErrorCode_t>(get);
    }

    auto& housekeeping = Services.housekeeping;
    auto& onBoardMonitoring = Services.onBoardMonitoringService;
    get = eps.outputBusChannelOn(EPS::EPSChannels::COMMS_12V);
    vTaskDelay(10);
    get = eps.outputBusChannelOn(EPS::EPSChannels::ADM_5V_1);
    vTaskDelay(10);
    while (true) {
        vTaskDelay(10);
        get = eps.getSystemStatus();
        vTaskDelay(10);
        get = eps.outputBusChannelOn(EPS::EPSChannels::COMMS_12V);
        if (get != EPS::ErrorCode::None) {
            LOG_ERROR << "EPS getStatus failed, error:" << static_cast<EPS::ErrorCode_t>(get);
        }
        LOG_INFO << "EPS time: " << MemManTask::getParameterAsUINT64(PeakSatParameters::EPS_UNIX_MINUTEID) << " : " << MemManTask::getParameterAsUINT64(PeakSatParameters::EPS_UNIX_SECONDID);
        updatePayloadParameters();
        vTaskDelay(10);
        ADM::getADMParameters();
        LOG_INFO<<"ADM Status: "<< MemManTask::getParameterAsUINT64(PeakSatParameters::antenna_deployment_statusID);

        // LOG_DEBUG<<"OBC Telemetry:";
        // housekeeping.housekeepingStructureReport(0);
        // LOG_DEBUG<<"EPS telemetry:";
        // housekeeping.housekeepingStructureReport(2);
        // LOG_DEBUG<<"COMMS-ADM Telemetry:";
        // housekeeping.housekeepingStructureReport(5);
        // LOG_DEBUG<<"Payload Telemetry:";
        // housekeeping.housekeepingStructureReport(3);

        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}