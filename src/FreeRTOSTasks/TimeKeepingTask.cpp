#include "TimeKeepingTask.hpp"
#include "TimeStamp.tpp"

Time::DefaultCUC _onBoardTimeKeeper(Time::DefaultCUC(0));

PIO_PIN_CALLBACK TimeKeepingTask::GNSS_PPS_Callback(uintptr_t context) {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    configASSERT(timeKeepingTaskHandle != nullptr);
    vTaskNotifyGiveFromISR(timeKeepingTaskHandle, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

int TimeKeepingTask::timeToSeconds(const tm& time) {
    return (time.tm_hour * 3600) + (time.tm_min * 60) + time.tm_sec;
}

void TimeKeepingTask::correctDriftTime(const tm& ppsTime, tm& rtcTime) {
    const int rtcSeconds = timeToSeconds(rtcTime);
    const int ppsSeconds = timeToSeconds(ppsTime);

    const int drift = rtcSeconds - ppsSeconds;  // Drift in seconds

    if (abs(drift) > DRIFT_THRESHOLD) {  // Only correct if drift is significant
        LOG_DEBUG << "Clock drift detected: " << drift << "s. Correcting...";
        rtcTime = ppsTime;  // Align RTC to PPS time
        RTC_TimeSet(&rtcTime);
    }
}

TimeKeepingTask::TimeKeepingTask() : Task("Timekeeping") {
    PIO_PinInterruptCallbackRegister(GNSS_PPS_PIN, reinterpret_cast<PIO_PIN_CALLBACK>(GNSS_PPS_Callback), reinterpret_cast<uintptr_t>(nullptr));
}


Time::DefaultCUC TimeKeepingTask::getSavedTime(){
    return _onBoardTimeKeeper;
}

void TimeKeepingTask::execute() {
    static tm dateTime;
    setEpoch(dateTime);
    RTC_TimeSet(&dateTime);

    // Clear any pending notifications
    ulTaskNotifyTake(pdTRUE, 0);
    while (true) {
        const BaseType_t notifyResult = ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        if (notifyResult == 0) {
            continue;  // No valid notification, skip this cycle
        }

        RTC_TimeGet(&dateTime);

        tm ppsTime = dateTime;
        ppsTime.tm_sec += 1;
        (void)mktime(&ppsTime);

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
