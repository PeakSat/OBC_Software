#pragma once

#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "etl/array.h"

#include "OnBoardMonitoringTask.hpp"

namespace TimerManagement{

    enum class TimerID:uint32_t {
        TEN_SEC,
        ONE_MIN,
        FIVE_MIN,
        TEN_MIN,
        // Add more as needed
    };
    using TimerID_t = std::underlying_type_t<TimerID>;;

    constexpr uint8_t MAX_TIMERS = 10;

    class TimerManager {
    public:
        TimerManager() {
            timers_.fill(nullptr);
        }

        void createTimer(TimerID id, TickType_t period) {
            auto timerIndex = static_cast<size_t>(id);
            TimerHandle_t xTimer = xTimerCreate(
                "Timer",            // Timer name
                period,             // Timer period in ticks
                pdTRUE,             // Auto-reload
                reinterpret_cast<void*>(static_cast<uintptr_t>(timerIndex)), // Store TimerID as void*,
                TimerCallback       // Callback function
            );


            if (xTimer == nullptr) {
                // Handle error: Timer creation failed
                return;
            }
            timers_[timerIndex] = xTimer;
            xTimerStart(xTimer, period);
        }

        static void TimerCallback(TimerHandle_t xTimer) {
            auto timerIndex = reinterpret_cast<size_t>(pvTimerGetTimerID(xTimer));
            auto timerID = static_cast<TimerID>(timerIndex);

            switch (timerID) {
                case TimerID::TEN_SEC:
                        xTaskNotify(OnBoardMonitoringTask::onBoardMonitoringTaskHandle, static_cast<TimerID_t>(timerID), eSetValueWithOverwrite);
                        break;
                case TimerID::ONE_MIN:
                    // Notify or perform actions for Task 2
                        break;
                case TimerID::TEN_MIN:
                    // Notify or perform actions for Task 3
                        break;
                // Add more cases as needed
                default:
                    // Handle unexpected timer IDs
                        break;
            }
        }

    private:
        etl::array<TimerHandle_t, MAX_TIMERS> timers_{};
    };

}