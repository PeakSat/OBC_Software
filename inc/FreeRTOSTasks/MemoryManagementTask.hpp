//
// Created by tats on 2/12/2024.
//

#pragma once

#include "TaskConfigs.hpp"
#include "MR4A08BUYS45.hpp"
#include "NANDFlash.h"
#include "LCLDefinitions.hpp"
#include <etl/String.hpp>
#include "queue.h"
#include "lfs.h"

inline const lfs_size_t MaxMemoryElementByteSize = 256;         // Static Read/Write Buffer sizes
inline const lfs_size_t MaxMemoryLookaheadByteSize = 128;       // Lookahead buffer size

inline const lfs_size_t NAND_partition_size_bytes = 1048576;    // 1MB (128 pages)
inline const lfs_size_t NAND_partition_blocks = 4096;           // Total number of blocks in the LUN
inline const lfs_size_t NAND_wear_leveling_cycles = 500;        // Erase cycles for wear leveling


class MemManTask : public Task{
private:
    StackType_t taskStack[MemoryManagementTaskStack];

public:
    void execute();

    MemManTask() : Task("MemManTask"){}

    static inline TaskHandle_t memManHandle;

    void createTask(){
        memManHandle = xTaskCreateStatic(vClassTask<MemManTask>, this->TaskName,
                                         MemoryManagementTaskStack, this,
                                         MemoryManagementTaskPriority, this->taskStack,
                                         &(this->taskBuffer));
    }

    bool writeToFile();
    bool readFromFile();
    bool eraseFile();
    bool getFileSize();
    bool getAvailableFiles();

};

inline std::optional<MemManTask> memManTask;
