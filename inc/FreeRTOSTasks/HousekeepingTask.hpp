#ifndef OBC_SOFTWARE_HOUSEKEEPINGTASK_HPP
#define OBC_SOFTWARE_HOUSEKEEPINGTASK_HPP

#include "TaskConfigs.hpp"

/**
 * FreeRTOS task implementing the ST[03] Housekeeping Service.
 */
class HousekeepingTask : public Task {
public:
    StackType_t taskStack[HousekeepingTaskStack];

    /**
      * @see HousekeepingService
      */
    void execute();

    HousekeepingTask() : Task("Housekeeping") {}

    /**
     * Create freeRTOS Task
     */
    void createTask() {
        xTaskCreateStatic(vClassTask<HousekeepingTask>, this->TaskName, HousekeepingTask::TaskStackDepth,
                          this, HousekeepingTaskPriority, this->taskStack,
                          &(this->taskBuffer));
    }
};

inline std::optional<HousekeepingTask> housekeepingTask;

#endif
