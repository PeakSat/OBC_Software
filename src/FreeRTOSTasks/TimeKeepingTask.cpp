#include "TimeKeepingTask.hpp"
#include "TimeStamp.tpp"

#include <MemoryManager.hpp>

Time::DefaultCUC _onBoardTimeKeeper(Time::DefaultCUC(0));

void TimeKeepingTask::GNSS_PPS_Callback(PIO_PIN pin, uintptr_t context) {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    auto* instance = reinterpret_cast<TimeKeepingTask*>(context);
    configASSERT(instance != nullptr);
    vTaskNotifyGiveFromISR(instance->timeKeepingTaskHandle, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

int TimeKeepingTask::timeToSeconds(const tm& time) {
    return (time.tm_hour * 3600) + (time.tm_min * 60) + time.tm_sec;
}

void TimeKeepingTask::correctDriftTime(const tm& ppsTime, tm& rtcTime) {
    const int rtcSeconds = timeToSeconds(rtcTime);
    const int ppsSeconds = timeToSeconds(ppsTime);

    const int drift = rtcSeconds - ppsSeconds; // Drift in seconds
    uint8_t RTCOffsetThreshold = 0U;
    if (abs(drift) > MemoryManager::getParameter(PeakSatParameters::OBDH_RTC_OFFSET_THRESHOLD_ID, static_cast<void*>(&RTCOffsetThreshold))) {
        LOG_DEBUG << "Clock drift detected: " << drift << "s. Correcting...";
        rtcTime = ppsTime; // Align RTC to PPS time
        RTC_TimeSet(&rtcTime);
    }
}

TimeKeepingTask::TimeKeepingTask() : Task("Timekeeping") {
    PIO_PinInterruptCallbackRegister(GNSS_PPS_PIN, GNSS_PPS_Callback, reinterpret_cast<uintptr_t>(this));
    PIO_PinInterruptEnable(GNSS_PPS_PIN);
}


Time::DefaultCUC TimeKeepingTask::getSavedTime() {
    return _onBoardTimeKeeper;
}

void TimeKeepingTask::execute() {
    static tm dateTime;
    setEpoch(dateTime);
    RTC_TimeSet(&dateTime);
    tm ppsTime = dateTime;
    MemoryManager::setParameter(PeakSatParameters::OBDH_RTC_OFFSET_THRESHOLD_ID, static_cast<void*>(&DRIFT_THRESHOLD));
    // Clear any pending notifications
    ulTaskNotifyTake(pdTRUE, 0);
    while (true) {
        if (ulTaskNotifyTake(pdTRUE, 1200) != pdTRUE) {
            // TODO: Check GNSS
            LOG_ERROR << "GNSS_PPS timed out";
        }

        RTC_TimeGet(&dateTime);
        ppsTime.tm_sec += 1;
        (void) mktime(&ppsTime);

        correctDriftTime(ppsTime, dateTime);
        setTimePlatformParameters(dateTime);
        printOnBoardTime();
    }
}

void TimeKeepingTask::printOnBoardTime() {
    UTCTimestamp timestamp = _onBoardTimeKeeper.toUTCtimestamp();
    etl::string<29> printTime = "Time:";
    etl::to_string(timestamp.hour, printTime, true);
    printTime += "-";
    etl::to_string(timestamp.minute, printTime, true);
    printTime += "-";
    etl::to_string(timestamp.second, printTime, true);
    printTime += " -- ";
    etl::to_string(timestamp.day, printTime, true);
    printTime += "/";
    etl::to_string(timestamp.month, printTime, true);
    printTime += "/";
    etl::to_string(timestamp.year, printTime, true);
    LOG_DEBUG << printTime.data();
}

void TimeKeepingTask::setTimePlatformParameters(const tm& dateTime) const {
    const UTCTimestamp timeUTC(dateTime.tm_year + yearBase, dateTime.tm_mon + 1, dateTime.tm_mday, dateTime.tm_hour, dateTime.tm_min, dateTime.tm_sec);
    const Time::DefaultCUC timeCUC(timeUTC);
    _onBoardTimeKeeper = timeCUC;
}

void TimeKeepingTask::setEpoch(tm& dateTime) const {
    dateTime.tm_sec = EpochTime.tm_sec;
    dateTime.tm_min = EpochTime.tm_min;
    dateTime.tm_hour = EpochTime.tm_hour;
    dateTime.tm_mday = EpochTime.tm_mday;
    dateTime.tm_mon = EpochTime.tm_mon;
    dateTime.tm_year = EpochTime.tm_year - yearBase;
}
