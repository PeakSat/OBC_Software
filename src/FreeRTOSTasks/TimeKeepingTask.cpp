#include "TimeKeepingTask.hpp"
#include "TimeStamp.tpp"

#include <ApplicationLayer.hpp>
#include <MemoryManager.hpp>
#include <OnBoardMonitoringTask.hpp>

using namespace TimerManagement;
Time::DefaultCUC _onBoardTimeKeeper(Time::DefaultCUC(0));

void TimeKeepingTask::GNSS_PPS_Callback(PIO_PIN pin, uintptr_t context) {
    // Ignore unused parameters
    (void) pin;

    if (timeKeepingTaskHandle != nullptr) {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        vTaskNotifyGiveFromISR(timeKeepingTaskHandle, &xHigherPriorityTaskWoken);
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
}

void TimeKeepingTask::RTT_InterruptHandler(RTT_INTERRUPT_TYPE type, uintptr_t context) {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    configASSERT(context != nullptr);
    auto taskHandle = static_cast<TaskHandle_t>((void*) (context));
    switch (type) {
        case RTT_PERIODIC:
            LOG_INFO << "RTT_PERIODIC";
            break;
        case RTT_ALARM:
            LOG_INFO << "RTT_ALARM";
            // vTaskNotifyGiveFromISR(taskHandle, &xHigherPriorityTaskWoken);
            break;
        case RTT_INTERRUPT_INVALID:
            LOG_INFO << "RTT_INTERRUPT_INVALID";
            break;
        default:
            LOG_INFO << "ERROR";
    }
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
    MemoryManager::getParameter(PeakSatParameters::OBDH_RTC_OFFSET_THRESHOLD_ID, static_cast<void*>(&RTCOffsetThreshold));
    if (abs(drift) > RTCOffsetThreshold) {
        LOG_DEBUG << "Clock drift detected: " << drift << "s. Correcting...";
        rtcTime = ppsTime; // Align RTC to PPS time
        RTC_TimeSet(&rtcTime);
    }
}

TimeKeepingTask::TimeKeepingTask() : Task("Timekeeping") {
    PIO_PinInterruptCallbackRegister(GNSS_PPS_PIN, GNSS_PPS_Callback, NULL);
    PIO_PinInterruptEnable(GNSS_PPS_PIN);
    RTT_CallbackRegister(RTT_InterruptHandler, reinterpret_cast<uintptr_t>(OnBoardMonitoringTask::onBoardMonitoringTaskHandle));
    RTT_Enable();
    RTT_EnableInterrupt(RTT_ALARM);
    RTT_EnableInterrupt(RTT_PERIODIC);
}


Time::DefaultCUC TimeKeepingTask::getSavedTime() {
    return _onBoardTimeKeeper;
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

void TimeKeepingTask::getGNSSTimestamp() {
    constexpr etl::array<uint16_t, CAN::TPMessageMaximumArguments> parameterIDArray = {PeakSatParameters::COMMS_GNSS_TIME_ID};
    CAN::Application::createRequestParametersMessage(CAN::COMMS, true, parameterIDArray, false);
    //TODO: parse GNSS time
    //TODO: Update RTC time with new GNSS time
}

TimerManagement::ErrorCode TimeKeepingTask::initializeStandardTimers() {
    auto& timerMgr = TimerManagement::TimerManager::getInstance();

    // Define timer configurations as a direct struct array
    struct TimerConfig {
        TimerManagement::TimerID id = TimerID::TIMER_NOT_INITIALIZED;
        TickType_t period = 0U;
    };

    TimerConfig timerConfigs[] = {
        {TimerManagement::TimerID::TIMER_10_SEC, pdMS_TO_TICKS(TimerPeriods::TEN_SECONDS)},
        {TimerManagement::TimerID::TIMER_1_MIN, pdMS_TO_TICKS(TimerPeriods::ONE_MINUTE)},
        {TimerManagement::TimerID::TIMER_5_MIN, pdMS_TO_TICKS(TimerPeriods::FIVE_MINUTES)},
        {TimerManagement::TimerID::TIMER_10_MIN, pdMS_TO_TICKS(TimerPeriods::TEN_MINUTES)}};

    // Create all timers in a loop
    for (const auto& config: timerConfigs) {
        TimerManagement::ErrorCode result = timerMgr.createTimer(config.id, config.period);
        if (result != TimerManagement::ErrorCode::SUCCESS) {
            return result; // Return first error encountered
        }
    }

    return TimerManagement::ErrorCode::SUCCESS;
}

bool TimeKeepingTask::registerForTimerNotifications(TaskHandle_t taskToNotify) {
    auto& timerMgr = TimerManagement::TimerManager::getInstance();

    // Define notification configurations as a direct struct array
    struct NotifConfig {
        TimerManagement::TimerID id = TimerID::TIMER_NOT_INITIALIZED;
        uint32_t notification = 0U;
    };

    NotifConfig notifConfigs[] = {
        {TimerManagement::TimerID::TIMER_10_SEC, NOTIFICATION_10_SEC},
        {TimerManagement::TimerID::TIMER_1_MIN, NOTIFICATION_1_MIN},
        {TimerManagement::TimerID::TIMER_5_MIN, NOTIFICATION_5_MIN},
        {TimerManagement::TimerID::TIMER_10_MIN, NOTIFICATION_10_MIN}};

    // Register for all timer notifications in a loop
    for (const auto& config: notifConfigs) {
        if (timerMgr.registerTaskForTimer(config.id, taskToNotify, config.notification) != TimerManagement::ErrorCode::SUCCESS) {
            return false; // Return false on first failure
        }
    }

    return true;
}
void TimeKeepingTask::execute() {
    // Initialize standard timers and register for notifications
    const TimerManagement::ErrorCode timerInitResult = initializeStandardTimers();
    if (timerInitResult != TimerManagement::ErrorCode::SUCCESS) {
        LOG_ERROR << "Timer initialization failed: ";
    }

    // Register for timer notifications
    bool registrationResult = registerForTimerNotifications(OnBoardMonitoringTask::onBoardMonitoringTaskHandle);
    if (!registrationResult) {
        LOG_ERROR << "Timer notification registration failed";
    }

    // Original time-keeping initialization
    static tm dateTime;
    bool useGNSS = true;

    setEpoch(dateTime);
    RTC_TimeSet(&dateTime);
    tm ppsTime = dateTime;

    MemoryManager::setParameter(PeakSatParameters::OBDH_RTC_OFFSET_THRESHOLD_ID, static_cast<void*>(&DRIFT_THRESHOLD));
    // MemoryManager::getParameter(PeakSatParameters::OBDH_USE_GNSS_PPS_ID, static_cast<void*>(&useGNSS));

    ulTaskNotifyTake(pdTRUE, 0);
    while (true) {
        bool ppsReceived = false;

        if (useGNSS) {
            ppsReceived = (ulTaskNotifyTake(pdTRUE, pdMS_TO_TICKS(1100)) == pdTRUE);
            if (not ppsReceived) {
                // TODO: Check GNSS
                LOG_ERROR << "GNSS_PPS timed out";
                if (++gnssTimeouts >= GNSS_MAX_TIMEOUTS) {
                    LOG_WARNING << "Disabling GNSS time sync after " << static_cast<int>(gnssTimeouts) << " timeouts";
                    useGNSS = false;
                }
            } else {
                // Reset timeout counter on successful PPS
                gnssTimeouts = 0;
            }
        }

        RTC_TimeGet(&dateTime);
        if (useGNSS && ppsReceived) {
            ppsTime.tm_sec += 1;
            (void) mktime(&ppsTime); // Normalize time (handle wrapping)
            correctDriftTime(ppsTime, dateTime);
        }

        setTimePlatformParameters(dateTime);
        printOnBoardTime();

        if (!useGNSS || !ppsReceived) {
            vTaskDelay(pdMS_TO_TICKS(1000));
        }
    }
}
