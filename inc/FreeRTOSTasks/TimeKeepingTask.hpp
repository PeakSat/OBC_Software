#pragma once

#include "TaskConfigs.hpp"
#include "TimeStamp.hpp"
#include "UTCTimestamp.hpp"
#include "Time.hpp"
#include "SoftwareTimers.hpp"



class TimeKeepingTask : public Task {
private:
    const uint16_t DelayMs = 5000;

    StackType_t taskStack[TimeKeepingTaskStack]{};

    inline static TaskHandle_t timeKeepingTaskHandle = nullptr;

    static void GNSS_PPS_Callback(PIO_PIN pin, uintptr_t context);

    static void RTT_InterruptHandler(RTT_INTERRUPT_TYPE type, uintptr_t context);

    static int timeToSeconds(const tm& time);

    static void correctDriftTime(const tm& ppsTime, tm& rtcTime);

    /**
     * @brief Initialize all commonly used timers
     *
     * Creates standard system timers with predefined periods.
     *
     * @return TimerManagement::ErrorCode indicating success or the first failure encountered
     */
    static TimerManagement::ErrorCode initializeStandardTimers();

    /**
     * @brief Register this task for timer notifications
     *
     * Registers the TimeKeepingTask to receive notifications for standard timers
     *
     * @return true if all registrations succeeded, false otherwise
     */
    static bool registerForTimerNotifications(TaskHandle_t taskToNotify);


public:
    void execute();

    /**
     * Sets the year base, used to acquire the current time from a tm structure.
     */
    uint16_t yearBase = 1900;

    uint8_t DRIFT_THRESHOLD = 2;

    /**
     * This function sets the epoch time.
     * @param dateTime is a tm struct witch keeps the time from MCU.
     */
    void setEpoch(tm& dateTime) const;

    /**
     * This function sets the AcubeSAT's time parameters using a tm struct.
     * @param dateTime is a tm struct witch keeps the time from MCU.
     */
    void setTimePlatformParameters(const tm& dateTime) const;

    /**
     * This function prints the on-board time.
     */
    void printOnBoardTime();

    static void getGNSSTimestamp();

    Time::DefaultCUC getSavedTime();

    TimeKeepingTask();

    void createTask() {
        timeKeepingTaskHandle = xTaskCreateStatic(vClassTask<TimeKeepingTask>, this->TaskName, TimeKeepingTaskStack, this,
                                                  TimeKeepingTaskPriority, this->taskStack, &(this->taskBuffer));
    }
};

inline std::optional<TimeKeepingTask> timeKeepingTask;
