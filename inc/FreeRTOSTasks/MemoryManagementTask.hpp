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
#include "filenames.hpp"
#include "general_definitions.hpp"
#include "git_version.h"
#include "lfs.h"

typedef struct {
    uint boot_count;
    uint8_t last_reset_cause;
    uint8_t fw_ver[3];
    char git_hash_id[10];
}bios_file_content;

enum class FILE_RW_FLAGS: uint8_t{
    APPEND = 0,
    OVERWRITE = 1
};

inline const lfs_size_t MaxMemoryElementByteSize = 256;         // Static Read/Write Buffer sizes
inline const lfs_size_t MaxMemoryLookaheadByteSize = 256;       // Lookahead buffer size

inline const lfs_size_t NAND_partition_size_bytes = 1048576;    // 1MB (128 pages)
inline const lfs_size_t NAND_partition_blocks = 4096;           // Total number of blocks in the LUN
inline const lfs_size_t NAND_wear_leveling_cycles = 500;        // Erase cycles for wear leveling

class MemManTask : public Task{
private:
    StackType_t taskStack[MemoryManagementTaskStack];

    bool writeNANDFile(lfs *lfs, const char* filename, etl::span<const uint8_t> &data, FILE_RW_FLAGS flag);
    bool writeMRAM_File(const char* filename, etl::span<const uint8_t> &data, FILE_RW_FLAGS flag);

    bool readNANDFile(lfs *lfs, const char* filename, etl::span<uint8_t> &data);
    bool readMRAM_File(const char* filename, etl::span<uint8_t> &data);

    bool eraseMRAMFile(const char* filename);

    size_t getNANDFileSize(lfs *lfs, const char* filename);
    size_t getMRAMFileSize(const char* filename);

    bool updateBiosFile();

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

    bool writeToFile(const char* filename, etl::span<const uint8_t> &data, FILE_RW_FLAGS flags);
    bool readFromFile(const char* filename, etl::span<uint8_t> &data);
    bool eraseFile(const char* filename);
    bool getFileSize(const char* filename, size_t &filesize);
    float getUsedSpace(lfs *lfs);
    void printAvailableFiles(lfs *lfs);

};

inline std::optional<MemManTask> memManTask;
