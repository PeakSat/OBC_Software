#pragma once

#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "etl/array.h"
#include "etl/vector.h"
#include "OnBoardMonitoringTask.hpp"


/**
 * @brief FreeRTOS timer management system with support for multiple task notifications
 *
 * This class implements a singleton pattern for managing FreeRTOS software timers.
 * It provides functionality to create, start, stop, and delete timers, as well as
 * register multiple tasks to be notified when timers expire.
 *
 * @example
 * // Create a ten-second timer
 * auto& timerMgr = TimerManagement::TimerManager::getInstance();
 * timerMgr.createTimer(TimerManagement::TimerID::TIMER_10_SEC, pdMS_TO_TICKS(10000));
 *
 * // Register current task to be notified
 * timerMgr.registerTaskForTimer(TimerManagement::TimerID::TIMER_10_SEC, xTaskGetCurrentTaskHandle());
 */

namespace TimerManagement {

    /**
     * @brief Error codes returned by TimerManager methods
     */
    enum class ErrorCode : uint8_t {
        SUCCESS,                    ///< Operation completed successfully
        TIMER_CREATION_FAILED,      ///< Failed to create timer
        TIMER_START_FAILED,         ///< Failed to start timer
        TIMER_STOP_FAILED,          ///< Failed to stop timer
        TIMER_DELETE_FAILED,        ///< Failed to delete timer
        TIMER_PERIOD_CHANGE_FAILED, ///< Failed to change timer period
        TIMER_RESET_FAILED,         ///< Failed to reset timer
        TIMER_NOT_FOUND,            ///< Timer with specified ID not found
        TASK_REGISTRATION_FAILED,   ///< Failed to register task
        TASK_NOT_FOUND,             ///< Task not found for unregistration
        INVALID_PARAMETER,          ///< Invalid parameter provided
        MAX_TASKS_REACHED           ///< Maximum number of tasks per timer reached
    };

    /**
     * @brief Timer identifier enumeration
     *
     * Defines the available timer types in the system.
     */
    enum class TimerID : uint32_t {
        TIMER_NOT_INITIALIZED = 0,
        TIMER_10_SEC,
        TIMER_1_MIN,
        TIMER_5_MIN,
        TIMER_10_MIN,
        // Add more as needed
        TIMER_COUNT ///< Must be the last entry, used to validate indices
    };

    /** @brief Underlying type of TimerID for conversion purposes */
    using TimerID_t = std::underlying_type_t<TimerID>;

    /** @brief Maximum number of timers that can be managed */
    constexpr uint8_t MAX_TIMERS = 10U;

    /** @brief Maximum number of tasks that can be registered per timer */
    static constexpr uint8_t MAX_TASKS_PER_TIMER = 5U;

    /**
     * @brief Structure to store task notification information
     */
    struct TaskNotification {
        TaskHandle_t taskHandle;
        uint32_t notificationValue;
        bool active;
    };

    /**
     * @brief Notification bits for different timer events
     */
    static constexpr uint32_t NOTIFICATION_10_SEC = (1UL << 0);
    static constexpr uint32_t NOTIFICATION_1_MIN = (1UL << 1);
    static constexpr uint32_t NOTIFICATION_5_MIN = (1UL << 2);
    static constexpr uint32_t NOTIFICATION_10_MIN = (1UL << 3);

    /**
     * @brief Notification bits for different timer events
     */
    enum class NotificationType : uint8_t {
        NOTIFICATION_NONE = 0,
        NOTIFICATION_10_SEC_TYPE = 1,
        NOTIFICATION_1_MIN_TYPE = 2,
        NOTIFICATION_5_MIN_TYPE = 3,
        NOTIFICATION_10_MIN_TYPE = 4 ,
        NOTIFICATION_TYPE_UNKNOWN = 5,
    };

    /** @brief Underlying type of TimerID for conversion purposes */
    using NotificationType_t = std::underlying_type_t<NotificationType>;

    inline NotificationType getHighestPriorityNotification(uint32_t notifiedValue) {
        if (notifiedValue & NOTIFICATION_10_SEC) return NotificationType::NOTIFICATION_10_SEC_TYPE;
        if (notifiedValue & NOTIFICATION_1_MIN) return NotificationType::NOTIFICATION_1_MIN_TYPE;
        if (notifiedValue & NOTIFICATION_5_MIN) return NotificationType::NOTIFICATION_5_MIN_TYPE;
        if (notifiedValue & NOTIFICATION_10_MIN) return NotificationType::NOTIFICATION_10_MIN_TYPE;
        if (notifiedValue != 0) return NotificationType::NOTIFICATION_TYPE_UNKNOWN;
        return NotificationType::NOTIFICATION_NONE;
    }

    /**
     * @brief Timer management class for FreeRTOS
     *
     * Provides a centralized system for creating and managing FreeRTOS software timers,
     * with the ability to notify multiple tasks when timers expire.
     */
    class TimerManager {
    public:
        /**
         * @brief Get the singleton instance of TimerManager
         * @return Reference to the TimerManager singleton
         */
        static TimerManager& getInstance() {
            static TimerManager instance;
            return instance;
        }

        /**
         * @brief Creates a new timer with the specified ID and period
         *
         * @param id The unique identifier for the timer
         * @param period The timer period in ticks
         * @return true if the timer was created and started successfully
         * @return ErrorCode indicating success or failure reason
         */
        ErrorCode createTimer(TimerID id, TickType_t period) {
            // Validate parameters
            if (period == 0) {
                return ErrorCode::INVALID_PARAMETER;
            }

            if (id >= TimerID::TIMER_COUNT) {
                return ErrorCode::INVALID_PARAMETER;
            }

            auto timerIndex = static_cast<size_t>(id);

            // Check if timer already exists
            if (timers_[timerIndex] != nullptr) {
                // Clean up existing timer first
                if (xTimerDelete(timers_[timerIndex], portMAX_DELAY) != pdPASS) {
                    return ErrorCode::TIMER_DELETE_FAILED;
                }
                timers_[timerIndex] = nullptr;
            }
            char timerName[12];
            snprintf(timerName, sizeof(timerName), "Timer_%u", static_cast<unsigned>(id));

            TimerHandle_t xTimer = xTimerCreate(
                timerName,                                                   // Timer name
                period,                                                      // Timer period in ticks
                pdTRUE,                                                      // Auto-reload
                reinterpret_cast<void*>(static_cast<uintptr_t>(timerIndex)), // Store TimerID as void*,
                &TimerCallback                                               // Callback function
            );

            if (xTimer == nullptr) {
                // Handle error: Timer creation failed
                return ErrorCode::TIMER_CREATION_FAILED;
            }

            timers_[timerIndex] = xTimer;
            timerPeriods_[timerIndex] = period;

            if (xTimerStart(xTimer, 0U) != pdPASS) {
                // Failed to start timer
                (void) xTimerDelete(xTimer, 0U);
                timers_[timerIndex] = nullptr;
                return ErrorCode::TIMER_START_FAILED;
            }
            return ErrorCode::SUCCESS;
        }

        /**
         * @brief Register a task to be notified when a specific timer expires, one timer can be registered to up to 5 task Notifications
         *
         * @param id The timer ID
         * @param taskHandle Handle of the task to be notified
         * @param notificationValue Value to be used in the notification (default: timer ID)
         * @return ErrorCode indicating success or failure reason.
         */
        ErrorCode registerTaskForTimer(TimerID id, TaskHandle_t taskHandle, uint32_t notificationValue = 0) {
            // Validate parameters
            if (id >= TimerID::TIMER_COUNT) {
                return ErrorCode::INVALID_PARAMETER;
            }

            if (taskHandle == nullptr) {
                return ErrorCode::INVALID_PARAMETER;
            }
            const auto timerIndex = static_cast<size_t>(id);

            // Timer must exist to register for it
            if (timers_[timerIndex] == nullptr) {
                return ErrorCode::TIMER_NOT_FOUND;
            }
            // If no specific notification value provided, use the timer ID
            if (notificationValue == 0U) {
                notificationValue = static_cast<TimerID_t>(id);
            }

            // Enter critical section to protect task registration
            taskENTER_CRITICAL();

            // Check if this task is already registered
            for (auto& task: timerToTasksMap_[timerIndex]) {
                if (task.taskHandle == taskHandle) {
                    // Update the existing entry
                    task.notificationValue = notificationValue;
                    task.active = true;
                    taskEXIT_CRITICAL();
                    return ErrorCode::SUCCESS;
                }
            }

            // Add new task if there is available space
            if (timerToTasksMap_[timerIndex].size() < MAX_TASKS_PER_TIMER) {
                timerToTasksMap_[timerIndex].push_back({taskHandle, notificationValue, true});
                taskEXIT_CRITICAL();
                return ErrorCode::SUCCESS;
            }

            taskEXIT_CRITICAL();
            return ErrorCode::MAX_TASKS_REACHED;
        }

        /**
         * @brief Unregister a task from a timer's notification list
         *
         * @param id The timer ID
         * @param taskHandle Handle of the task to unregister
         * @return true if the task was found and unregistered
         * @return ErrorCode indicating success or failure reason
         */
        ErrorCode unregisterTaskFromTimer(TimerID id, TaskHandle_t taskHandle) {
            // Validate parameters
            if (id >= TimerID::TIMER_COUNT) {
                return ErrorCode::INVALID_PARAMETER;
            }

            if (taskHandle == nullptr) {
                return ErrorCode::INVALID_PARAMETER;
            }

            const auto timerIndex = static_cast<size_t>(id);

            // Enter critical section to protect task list
            taskENTER_CRITICAL();

            for (auto& task: timerToTasksMap_[timerIndex]) {
                if (task.taskHandle == taskHandle) {
                    // Mark as inactive
                    task.active = false;
                    taskEXIT_CRITICAL();
                    return ErrorCode::SUCCESS;
                }
            }

            taskEXIT_CRITICAL();
            return ErrorCode::TASK_NOT_FOUND;
        }

        /**
         * @brief Stop a running timer
         *
         * @param id The timer ID
         * @return ErrorCode indicating success or failure reason
         */
        ErrorCode stopTimer(TimerID id) {
            // Validate parameters
            if (id >= TimerID::TIMER_COUNT) {
                return ErrorCode::INVALID_PARAMETER;
            }
            const auto timerIndex = static_cast<size_t>(id);
            if (timers_[timerIndex] == nullptr) {
                return ErrorCode::TIMER_NOT_FOUND;
            }

            if (xTimerStop(timers_[timerIndex], portMAX_DELAY) != pdPASS) {
                return ErrorCode::TIMER_STOP_FAILED;
            }

            return ErrorCode::SUCCESS;
        }

        /**
         * @brief Delete a timer
         *
         * @param id The timer ID
         * @return ErrorCode indicating success or failure reason
         */
        ErrorCode deleteTimer(TimerID id) {
            // Validate parameters
            if (id >= TimerID::TIMER_COUNT) {
                return ErrorCode::INVALID_PARAMETER;
            }

            const auto timerIndex = static_cast<size_t>(id);
            if (timers_[timerIndex] == nullptr) {
                return ErrorCode::TIMER_NOT_FOUND;
            }

            if (xTimerDelete(timers_[timerIndex], portMAX_DELAY) != pdPASS) {
                return ErrorCode::TIMER_DELETE_FAILED;
            }

            // Clear associated data structures
            timers_[timerIndex] = nullptr;
            timerPeriods_[timerIndex] = 0;

            // Clear task notifications for this timer
            taskENTER_CRITICAL();
            timerToTasksMap_[timerIndex].clear();
            taskEXIT_CRITICAL();

            return ErrorCode::SUCCESS;
        }

        /**
         * @brief Get the period of a timer
         *
         * @param id The timer ID
         * @return TickType_t The timer period in ticks or ErrorCode indicating failure reason
         */
        [[nodiscard]] etl::expected<TickType_t, ErrorCode> getTimerPeriod(TimerID id) const {
            // Validate parameters
            if (id >= TimerID::TIMER_COUNT) {
                return etl::unexpected(ErrorCode::INVALID_PARAMETER);
            }

            const auto timerIndex = static_cast<size_t>(id);
            if (timers_[timerIndex] == nullptr) {
                return etl::unexpected(ErrorCode::TIMER_NOT_FOUND);
            }
            return timerPeriods_[timerIndex];
        }

        /**
         * @brief Change the period of an existing timer
         *
         * @param id The timer ID
         * @param newPeriod The new period in ticks
         * @return ErrorCode indicating success or failure reason
         */
        ErrorCode changeTimerPeriod(TimerID id, TickType_t newPeriod) {
            // Validate parameters
            if (id >= TimerID::TIMER_COUNT) {
                return ErrorCode::INVALID_PARAMETER;
            }

            if (newPeriod == 0) {
                return ErrorCode::INVALID_PARAMETER;
            }

            const auto timerIndex = static_cast<size_t>(id);
            if (timers_[timerIndex] == nullptr) {
                return ErrorCode::TIMER_NOT_FOUND;
            }

            if (xTimerChangePeriod(timers_[timerIndex], newPeriod, portMAX_DELAY) != pdPASS) {
                return ErrorCode::TIMER_PERIOD_CHANGE_FAILED;
            }

            timerPeriods_[timerIndex] = newPeriod;
            return ErrorCode::SUCCESS;
        }

        /**
         * @brief Reset a timer
         *
         * @param id The timer ID
         * @return ErrorCode indicating success or failure reason
         */
        ErrorCode resetTimer(TimerID id) {
            // Validate parameters
            if (id >= TimerID::TIMER_COUNT) {
                return ErrorCode::INVALID_PARAMETER;
            }

            const auto timerIndex = static_cast<size_t>(id);
            if (timers_[timerIndex] == nullptr) {
                return ErrorCode::TIMER_NOT_FOUND;
            }

            if (xTimerReset(timers_[timerIndex], portMAX_DELAY) != pdPASS) {
                return ErrorCode::TIMER_RESET_FAILED;
            }

            return ErrorCode::SUCCESS;
        }

        /** @brief Deleted copy constructor to enforce singleton pattern */
        TimerManager(const TimerManager&) = delete;

        /** @brief Deleted assignment operator to enforce singleton pattern */
        TimerManager& operator=(const TimerManager&) = delete;

    private:
        /**
         * @brief Default constructor
         *
         * Initializes the internal arrays
         */
        TimerManager() {
            timers_.fill(nullptr);
            timerPeriods_.fill(0U);
        }

        /** @brief Array of timer handles, indexed by TimerID */
        etl::array<TimerHandle_t, MAX_TIMERS> timers_{};

        /** @brief Array of timer periods, indexed by TimerID */
        etl::array<TickType_t, MAX_TIMERS> timerPeriods_{};

        /** @brief Array of task notification vectors, one vector per timer */
        etl::array<etl::vector<TaskNotification, 5>, MAX_TIMERS> timerToTasksMap_{};

        static void TimerCallback(TimerHandle_t xTimer) {
            if (xTimer == nullptr) {
                return;
            }

            const auto timerIndex = reinterpret_cast<size_t>(pvTimerGetTimerID(xTimer));
            if (timerIndex >= MAX_TIMERS) {
                return; // Invalid timer index
            }

            // Get instance to access non-static members
            TimerManager& instance = getInstance();

            // Enter critical section to protect task list access
            taskENTER_CRITICAL();

            // Notify all active registered tasks
            for (const auto& taskNotif: instance.timerToTasksMap_[timerIndex]) {
                if (taskNotif.active && taskNotif.taskHandle != nullptr) {
                    // Use ulTaskNotifyValueClear to preserve other bits in notification value
                    xTaskNotify(taskNotif.taskHandle, taskNotif.notificationValue, eSetBits);
                }
            }

            taskEXIT_CRITICAL();
        }
    };

} // namespace TimerManagement

/**
 * @brief Standard system timer periods in milliseconds
 */
namespace TimerPeriods {
    constexpr uint32_t TEN_SECONDS = 10000;
    constexpr uint32_t ONE_MINUTE = 60000;
    constexpr uint32_t FIVE_MINUTES = 300000;
    constexpr uint32_t TEN_MINUTES = 600000;
} // namespace TimerPeriods