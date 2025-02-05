#ifndef ECSS_SERVICES_HOUSEKEEPINGPARAMETERS_HPP
#define ECSS_SERVICES_HOUSEKEEPINGPARAMETERS_HPP

#include "PeakSatParameters.hpp"
#include "Services/HousekeepingService.hpp"

namespace HousekeepingStructures {

    static etl::array<HousekeepingStructure, ECSSMaxHousekeepingStructures> structures = {
        {
            // OBC Parameters that will be monitored in the campaign
            {0, 1000, true, {OBDHParameters::PCBTemperature1ID, OBDHParameters::PCBTemperature2ID, OBDHParameters::MCUTemperatureID, OBDHParameters::MCUBootCounterID, OBDHParameters::MemoryPartitionID, OBDHParameters::MCUSystickID, OBDHParameters::CANBUSLoad1ID, OBDHParameters::CANBUSLoad2ID, OBDHParameters::CANBUSActiveID, OBDHParameters::NANDFLASHLCLThresholdID, OBDHParameters::MRAMLCLThresholdID, OBDHParameters::NANDFLASHONID, OBDHParameters::MRAMONID, OBDHParameters::UseCANID, OBDHParameters::UseUARTID, OBDHParameters::UseRTTID, OBDHParameters::OnBoardTimeID}},
            // OBC Mission Database parameters monitored every 3 sec
            {1, 3000, true, {OBDHParameters::PCBTemperature1ID, OBDHParameters::PCBTemperature2ID, OBDHParameters::SpacecraftTimeRefID, OBDHParameters::OperationalModeID, OBDHParameters::MemoryPartitionID, OBDHParameters::ReconfigurationTimerID, OBDHParameters::LastFailedEventID, OBDHParameters::MCUSystickID}},
            // OBC Mission Database parameters not sampled
            {2, 5000, false, {OBDHParameters::MCUInputVoltageID, OBDHParameters::FlashIntID, OBDHParameters::SRAMIntID, OBDHParameters::AvailableMRAMID, OBDHParameters::AvailableNANDID, OBDHParameters::CANBUSLoad1ID, OBDHParameters::CANBUSLoad2ID, OBDHParameters::CANBUSActiveID, OBDHParameters::MCUFDIRID, OBDHParameters::MCURestartSafeModeThresholdID, OBDHParameters::NANDFLASHLCLThresholdID, OBDHParameters::MRAMLCLThresholdID, OBDHParameters::NANDFLASHONID, OBDHParameters::NANDFLASHScrubbingFrequencyID, OBDHParameters::ΜRAMScrubbingFrequencyID, OBDHParameters::ProgramFlashScrubbingFrequencyID}},
        }};
} // namespace HousekeepingStructures

#endif
