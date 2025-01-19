#ifndef ECSS_SERVICES_HOUSEKEEPINGPARAMETERS_HPP
#define ECSS_SERVICES_HOUSEKEEPINGPARAMETERS_HPP

#include "PeakSatParameters.hpp"
#include "Services/HousekeepingService.hpp"

namespace HousekeepingStructures {

    // static etl::array<HousekeepingStructure, ECSSMaxHousekeepingStructures> structures = {
    //     {
    //         // OBC Parameters that will be monitored in the campaign
    //         {0, 1000, true, {OBDHParameters::PCBTemperature1ID, OBDHParameters::PCBTemperature2ID, OBDHParameters::MCUTemperatureID, OBDHParameters::MCUBootCounterID, OBCMemoryPartition, OBCMCUSystick, OBDHParameters::CANBUSLoad1ID, OBDHParameters::CANBUSLoad2ID, OBDHParameters::CANBUSActiveID, OBDHParameters::NANDFLASHLCLThresholdID, OBDHParameters::MRAMLCLThresholdID, OBDHParameters::NANDFLASHONID, OBDHParameters::MRAMONID, OBCUseCAN, OBCUseUART, OBCUseRTT, OBDHParameters::OnBoardTimeID}},
    //         // OBC Mission Database parameters monitored every 3 sec
    //         {1, 3000, true, {OBDHParameters::PCBTemperature1ID, OBDHParameters::PCBTemperature2ID, OBCSpacecraftTimeRef, OBCOperationalMode, OBCMemoryPartition, OBCReconfigurationTimer, OBCLastFailedEvent, OBCMCUSystick}},
    //         // OBC Mission Database parameters not sampled
    //         {2, 5000, false, {OBCMCUInputVoltage, OBCFlashInt, OBCSRAMInt, OBCAvailableMRAM, OBCAvailableNAND, OBCCANBUSLoad1, OBCCANBUSLoad2, OBCCANBUSActive, OBCMCUFDIR, OBCMCURestartSafeModeThreshold, OBCNANDFLASHLCLThreshold, OBCMRAMLCLThreshold, OBCNANDFLASHON, OBCNANDFLASHScrubbingFrequency, OBCRAMScrubbingFrequency, OBCProgramFlashScrubbingFrequency}},
    //     }};
} // namespace HousekeepingStructures

#endif
