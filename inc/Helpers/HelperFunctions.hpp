#pragma once
#include "MemoryManager.hpp"
namespace HelperFunctions {
    inline void resetChecks() {
        const RSTC_RESET_CAUSE resetCause = RSTC_ResetCauseGet();
        uint32_t resetCauseValue = resetCause;
        if (resetCause == RSTC_GENERAL_RESET) {
            LOG_DEBUG << "Last reset was: General reset.";
        } else if (resetCause == RSTC_BACKUP_RESET) {
            LOG_DEBUG << "Last reset was: Backup reset.";
        } else if (resetCause == RSTC_WATCHDOG_RESET) {
            LOG_DEBUG << "Last reset was: Watchdog reset.";
        } else if (resetCause == RSTC_USER_RESET) {
            LOG_DEBUG << "Last reset was: User reset.";
        } else if (resetCause == RSTC_SOFTWARE_RESET) {
            LOG_DEBUG << "Last reset was: Software reset.";
        } else {
            LOG_DEBUG << "Last reset was: Undefined reset.";
        }
        MemoryManager::setParameter(PeakSatParameters::OBDH_LAST_RESET_CAUSE_ID, static_cast<void*>(&resetCauseValue));
    }

    inline void initialiseMPURegions(void) {

        const uint32_t REGION_NAND_A = 0;
        const uint32_t REGION_NAND_B = 1;
        const uint32_t REGION_MRAM   = 2;

        const uint32_t NAND_A_BASE_ADDRESS = 0x61000000;
        const uint32_t NAND_B_BASE_ADDRESS = 0x63000000;
        const uint32_t MRAM_BASE_ADDRESS   = 0x60000000;

        ARM_MPU_Disable();
        // Configure NAND Flash regions as Device memory
        ARM_MPU_SetRegionEx(REGION_NAND_A, ARM_MPU_RBAR(REGION_NAND_A, NAND_A_BASE_ADDRESS),
                                        ARM_MPU_RASR(   1,                  /* DisableExec: Disable instruction fetches */
                                                        ARM_MPU_AP_FULL,    /* AccessPermission: full read/write access */
                                                        0,                  /* TypeExtField: 0 for Device (adjust if needed) */
                                                        1,                  /* IsShareable: Yes (1) for device/peripheral */
                                                        0,                  /* IsCacheable: No (0) */
                                                        1,                  /* IsBufferable: Yes (1) */
                                                        0,                  /* SubRegionDisable: All subregions enabled */
                                                        ARM_MPU_REGION_SIZE_16MB  /* Size: 16MB region */
                                                        ));

        ARM_MPU_SetRegionEx(REGION_NAND_B, ARM_MPU_RBAR(REGION_NAND_B, NAND_B_BASE_ADDRESS),
                                        ARM_MPU_RASR(   1,                  /* DisableExec: Disable instruction fetches */
                                                        ARM_MPU_AP_FULL,    /* AccessPermission: full read/write access */
                                                        0,                  /* TypeExtField: 0 for Device (adjust if needed) */
                                                        1,                  /* IsShareable: Yes (1) for device/peripheral */
                                                        0,                  /* IsCacheable: No (0) */
                                                        1,                  /* IsBufferable: Yes (1) */
                                                        0,                  /* SubRegionDisable: All subregions enabled */
                                                        ARM_MPU_REGION_SIZE_256MB  /* Size: 256MB region */
                                                        ));

        ARM_MPU_SetRegionEx(REGION_MRAM, ARM_MPU_RBAR(REGION_MRAM, MRAM_BASE_ADDRESS),
                                        ARM_MPU_RASR(   1,                  /* DisableExec: Disable instruction fetches */
                                                        ARM_MPU_AP_FULL,    /* AccessPermission: full read/write access */
                                                        0,                  /* TypeExtField: 0 for Device (adjust if needed) */
                                                        1,                  /* IsShareable: Yes (1) for device/peripheral */
                                                        0,                  /* IsCacheable: No (0) */
                                                        1,                  /* IsBufferable: Yes (1) */
                                                        0,                  /* SubRegionDisable: All subregions enabled */
                                                        ARM_MPU_REGION_SIZE_16MB  /* Size: 16MB region */
                                                        ));

        // Enable MPU with default memory map
        ARM_MPU_Enable(MPU_CTRL_PRIVDEFENA_Msk);
    }

} //namespace HelperFunctions
