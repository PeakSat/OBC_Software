#pragma once

#include "CAN/Driver.hpp"
#include "TaskConfigs.hpp"

#include <optional>

class CANParserTask : public Task {
private:
public:
    StackType_t taskStack[CANParserTaskStack];

    void execute();

    CANParserTask() : Task("CAN Test") {}

    /**
   * Create freeRTOS Task
   */
    void createTask() {
        this->taskHandle = xTaskCreateStatic(vClassTask<CANParserTask>, this->TaskName, CANParserTaskStack, this,
                                             CANParserTaskPriority, this->taskStack, &(this->taskBuffer));
    }
};

inline std::optional<CANParserTask> canParserTask;
