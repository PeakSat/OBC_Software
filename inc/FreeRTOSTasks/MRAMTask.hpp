//#pragma once
//
//#include "TaskConfigs.hpp"
//#include "MR4A08BUYS45.hpp"
//
//class MRAMTask : public Task {
//private:
//    const uint16_t DelayMs = 15000;
//
//    StackType_t taskStack[MRAMTaskStack];
//
//public:
//    void execute();
//
//    MRAMTask() : Task("MRAMTask") {}
//
//    static inline TaskHandle_t mramTaskHandle;
//
//    void createTask() {
//        mramTaskHandle = xTaskCreateStatic(vClassTask<MRAMTask>, this->TaskName,
//                                           MRAMTaskStack, this, MRAMTaskPriority, this->taskStack,
//                                           &(this->taskBuffer));
//    }
//};
//
//inline std::optional<MRAMTask> mramTask;
