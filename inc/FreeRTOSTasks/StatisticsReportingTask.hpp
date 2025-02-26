#pragma once

#include "TaskConfigs.hpp"
#include "ServicePool.hpp"

class StatisticsReportingTask : public Task {
public:
    /**
     * @see ParameterStatisticsService
     */
    void execute();

    StackType_t taskStack[StatisticsReportingTaskStack];

    StatisticsReportingTask() : Task("StatisticsReporting") {}

    /**
     * Create freeRTOS Task
     */
    void createTask() {
        xTaskCreateStatic(vClassTask<StatisticsReportingTask>, this->TaskName,
                          StatisticsReportingTaskStack, this, StatisticsReportingTaskPriority,
                          this->taskStack, &(this->taskBuffer));
    }
};

inline std::optional<StatisticsReportingTask> statisticsReportingTask;
