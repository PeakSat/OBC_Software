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
} //namespace HelperFunctions
