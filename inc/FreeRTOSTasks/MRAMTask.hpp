#pragma once

#include "TaskConfigs.hpp"
#include "MR4A08BUYS45.hpp"
#include "LCLDefinitions.hpp"

class MRAMTask : public Task {
private:
    const uint16_t DelayMs = 15000;

    StackType_t taskStack[MRAMTaskStack];

    MRAM mram{SMC::NCS0};

    LCL& mramLCL = LCLDefinitions::lclArray[LCLDefinitions::MRAM];

    uint32_t areYouAliveAddress = 0;

    uint8_t areYouAliveValue = 27;

public:
    void execute();

    MRAMTask() : Task("MRAMTask") {}

    static inline TaskHandle_t mramTaskHandle;

    void createTask() {
        mramTaskHandle = xTaskCreateStatic(vClassTask<MRAMTask>, this->TaskName,
                                           MRAMTask::TaskStackDepth, this, MRAMTaskPriority, this->taskStack,
                                           &(this->taskBuffer));
    }

    bool isMRAMAlive();
};

inline std::optional<MRAMTask> mramTask;
