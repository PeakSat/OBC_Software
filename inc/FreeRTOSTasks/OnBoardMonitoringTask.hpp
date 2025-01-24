#pragma once

#include "TaskConfigs.hpp"

/**
 * FreeRTOS task implementing the ST[03] Housekeeping Service.
 */
class OnBoardMonitoringTask : public Task {
public:
    StackType_t taskStack[OnBoardMonitoringTaskStack];

    /**
      * @see HousekeepingService
      */
    void execute();

    OnBoardMonitoringTask() : Task("OnBoardMonitoring") { }

    /**
     * Create freeRTOS Task
     */
    void createTask() {
        xTaskCreateStatic(vClassTask<OnBoardMonitoringTask>, this->TaskName, OnBoardMonitoringTaskStack,
                          this, OnBoardMonitoringTaskPriority, this->taskStack,
                          &(this->taskBuffer));
    }
};

inline std::optional<OnBoardMonitoringTask> onBoardMonitoringTask;
