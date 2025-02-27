#ifndef ECSS_SERVICES_TimeBasedSchedulingTask_HPP
#define ECSS_SERVICES_TimeBasedSchedulingTask_HPP

#include "TaskConfigs.hpp"
#include "ServicePool.hpp"

class TimeBasedSchedulingTask : public Task {
public:
    StackType_t taskStack[TimeBasedSchedulingTaskStack];

    void execute();

    TimeBasedSchedulingTask() : Task("TimeBasedSchedulingTask") {}

    /**
     * Create freeRTOS Task
     */
    void createTask() {
        this->taskHandle = xTaskCreateStatic(vClassTask<TimeBasedSchedulingTask>,
                                             this->TaskName,
                                             TimeBasedSchedulingTaskStack,
                                             this, TimeBasedSchedulingTaskPriority,
                                             this->taskStack,
                                             &(this->taskBuffer));
    }
};

inline std::optional<TimeBasedSchedulingTask> timeBasedSchedulingTask;

#endif
