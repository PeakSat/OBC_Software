#pragma once

#include "TaskConfigs.hpp"
#include "UTCTimestamp.hpp"
#include "Time.hpp"

class TimeKeepingTask : public Task {
private:
    const uint16_t DelayMs = 5000;

    StackType_t taskStack[TimeKeepingTaskStack];

public:
    void execute();

    /**
     * Sets the year base, used to acquire the current time from a tm structure.
     */
    uint16_t yearBase = 1900;

    /**
     * This function sets the epoch time.
     * @param dateTime is a tm struct witch keeps the time from MCU.
     */
    void setEpoch(tm& dateTime);

    /**
     * This function sets the AcubeSAT's time parameters using a tm struct.
     * @param dateTime is a tm struct witch keeps the time from MCU.
     */
    void setTimePlatformParameters(tm& dateTime);

    /**
     * This function prints the on-board time.
     */
    void printOnBoardTime();

    TimeKeepingTask() : Task("Timekeeping") {}

    void createTask() {
        xTaskCreateStatic(vClassTask<TimeKeepingTask>, this->TaskName, TimeKeepingTaskStack, this,
                          TimeKeepingTaskPriority, this->taskStack, &(this->taskBuffer));
    }
};

inline std::optional<TimeKeepingTask> timeKeepingTask;
