#ifndef ECSS_SERVICES_HOUSEKEEPINGPARAMETERS_HPP
#define ECSS_SERVICES_HOUSEKEEPINGPARAMETERS_HPP

#include "PeakSatParameters.hpp"
#include "Services/HousekeepingService.hpp"

namespace HousekeepingStructures {

    static etl::array<HousekeepingStructure, ECSSMaxHousekeepingStructures> structures = {
        {
            // OBC Parameters that will be monitored in the campaign
            {0, 1000, true, {OBDHParameters::PCBTemperature1ID, OBDHParameters::PCBTemperature2ID, OBDHParameters::MCUTemperatureID, OBDHParameters::MCUBootCounterID, OBDHParameters::MemoryPartition, OBDHParameters::MCUSystick, OBDHParameters::CANBUSLoad1ID, OBDHParameters::CANBUSLoad2ID, OBDHParameters::CANBUSActiveID, OBDHParameters::NANDFLASHLCLThresholdID, OBDHParameters::MRAMLCLThresholdID, OBDHParameters::NANDFLASHONID, OBDHParameters::MRAMONID, OBDHParameters::UseCAN, OBDHParameters::UseUART, OBDHParameters::UseRTT, OBDHParameters::OnBoardTimeID}},
            // OBC Mission Database parameters monitored every 3 sec
            {1, 3000, true, {OBDHParameters::PCBTemperature1ID, OBDHParameters::PCBTemperature2ID, OBDHParameters::SpacecraftTimeRef, OBDHParameters::OperationalMode, OBDHParameters::MemoryPartition, OBDHParameters::ReconfigurationTimer, OBDHParameters::LastFailedEvent, OBDHParameters::MCUSystick}},
            // OBC Mission Database parameters not sampled
            {2, 5000, false, {OBDHParameters::MCUInputVoltage, OBDHParameters::FlashInt, OBDHParameters::SRAMInt, OBDHParameters::AvailableMRAM, OBDHParameters::AvailableNAND, OBDHParameters::CANBUSLoad1, OBDHParameters::CANBUSLoad2, OBDHParameters::CANBUSActive, OBDHParameters::MCUFDIR, OBDHParameters::MCURestartSafeModeThreshold, OBDHParameters::NANDFLASHLCLThreshold, OBDHParameters::MRAMLCLThreshold, OBDHParameters::NANDFLASHON, OBDHParameters::NANDFLASHScrubbingFrequency, OBDHParameters::ΜRAMScrubbingFrequency, OBDHParameters::ProgramFlashScrubbingFrequency}},
        }};
} // namespace HousekeepingStructures

#endif
