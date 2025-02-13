//
// Created by tats on 2/12/2024.
//

#pragma once

#include "TaskConfigs.hpp"
#include "MR4A08BUYS45.hpp"
#include "NANDFlash.h"
#include "LCLDefinitions.hpp"
#include <etl/String.hpp>
#include <etl/expected.h>
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
} bios_file_content;

enum class FILE_RW_FLAGS : uint8_t {
    APPEND = 0,
    OVERWRITE = 1
};

inline const lfs_size_t MaxMemoryElementByteSize = 256;   // Static Read/Write Buffer sizes
inline const lfs_size_t MaxMemoryLookaheadByteSize = 256; // Lookahead buffer size

inline const lfs_size_t NAND_partition_size_bytes = 1048576; // 1MB (128 pages)
inline const lfs_size_t NAND_partition_blocks = 4096;        // Total number of blocks in the LUN
inline const lfs_size_t NAND_wear_leveling_cycles = 500;     // Erase cycles for wear leveling

class MemManTask : public Task {
private:
    StackType_t taskStack[MemoryManagementTaskStack];

    /**
     * @Description Writes the provided data to the specified file in the appropriate NAND module,
     *              this function is meant to be used only by the MemoryManagementTask and not
     *              to be called directly by the user.
     * @param lfs       LFS instance to be used
     * @param filename  Filename according to the filename convention (see filenames.hpp)
     * @param data      Data to be written
     * @param flag      APPEND or OVERWRITE
     * @return          0 on success, negative error code on failure
     */
    static etl::expected<int, lfs_error> writeNANDFile(lfs* lfs, const char* filename, etl::span<const uint8_t>& data, FILE_RW_FLAGS flag);

    /**
     * @Description Writes the provided data to the specified file to the MRAM module,
     *              this function is meant to be used only by the MemoryManagementTask and not
     *              to be called directly by the user.
     * @param filename  Filename according to the filename convention (see filenames.hpp)
     * @param data      Data to be written
     * @param flag      APPEND or OVERWRITE
     * @return          0 on success, 1 on failure [TBD]
     */
    static bool writeMRAM_File(const char* filename, etl::span<const uint8_t>& data, FILE_RW_FLAGS flag);

    /**
     * @Description Reads the requested data from the specified file from the appropriate NAND module,
     *              this function is meant to be used only by the MemoryManagementTask and not
     *              to be called directly by the user.
     * @param lfs       LFS instance to be used
     * @param filename  Filename according to the filename convention (see filenames.hpp)
     * @param data      Buffer to read data
     * @return          0 on success, negative error code on failure
     */
    static etl::expected<int, lfs_error> readNANDFile(lfs* lfs, const char* filename, etl::span<uint8_t>& data);

    /**
     * @Description Reads the requested data from the specified file from the MRAM module,
     *              this function is meant to be used only by the MemoryManagementTask and not
     *              to be called directly by the user.
     * @param data      Buffer to read data
     * @return          0 on success, 1 on failure [TBD]
     */
    static bool readMRAM_File(const char* filename, etl::span<uint8_t>& data);

    /**
     * @Description Erases the specified file from the MRAM module,
     *              this function is meant to be used only by the MemoryManagementTask and not
     *              to be called directly by the user.
     * @param filename  Filename according to the filename convention (see filenames.hpp)
     * @return          0 on success, 1 on failure [TBD]
     */
    static bool eraseMRAMFile(const char* filename);

    /**
     * @Description Returns the size of the file requested in bytes from the appropriate NAND module,
     *              this function is meant to be used only by the MemoryManagementTask and not
     *              to be called directly by the user.
     * @param lfs       LFS instance to be used
     * @param filename  Filename according to the filename convention (see filenames.hpp)
     * @return          The size of the file on success, negative error code on failure
     */
    static etl::expected<size_t, lfs_error> getNANDFileSize(lfs* lfs, const char* filename);

    /**
     * @Description Returns the size of the file requested in bytes from the MRAM module,
     *              this function is meant to be used only by the MemoryManagementTask and not
     *              to be called directly by the user.
     * @param filename  Filename according to the filename convention (see filenames.hpp)
     * @return          The size of the file on success, negative error code on failure [TBD]
     */
    static size_t getMRAMFileSize(const char* filename);

    /**
     * @Description     Returns the used space in the mounted LFS. Not meant to be used
     *                  by the user.
     * @param lfs       LFS instance to be used.
     * @return          float percentage in success, negative error code on failure.
     */
    static etl::expected<float, lfs_error> getUsedSpace(lfs* lfs);

    /**
     * @Description     Prints the filenames stored in the specified LFS instance.
     * @param lfs       LFS instance to be used.
     */
    static void printAvailableFiles(lfs* lfs);

    static bool updateBiosFile();

public:
    void execute();

    MemManTask() : Task("MemManTask") {}

    static inline TaskHandle_t memManHandle;

    void createTask() {
        memManHandle = xTaskCreateStatic(vClassTask<MemManTask>, this->TaskName,
                                         MemoryManagementTaskStack, this,
                                         MemoryManagementTaskPriority, this->taskStack,
                                         &(this->taskBuffer));
    }

    /**
     * @Description Write the provided data to the specified file,
     *              this function handles internally the selection of the appropriate
     *              memory module.
     * @param filename  Filename according to the filename convention (see filenames.hpp)
     * @param data      Data to be written
     * @param flag      APPEND or OVERWRITE
     * @return          0 on success, negative error code on failure.
     */
    static etl::expected<int, lfs_error> writeToFile(const char* filename, etl::span<const uint8_t>& data, FILE_RW_FLAGS flags);

    /**
     * @Description Reads the requested data from the specified file,
     *              this function handles internally the selection of the appropriate
     *              memory module.
     * @param filename  Filename according to the filename convention (see filenames.hpp)
     * @param data      Data to be written
     * @return          0 on success, negative error code on failure.
     */
    static etl::expected<int, lfs_error> readFromFile(const char* filename, etl::span<uint8_t>& data);

    /**
     * @Description Erases the specified file, this function handles internally the
     *              selection of the appropriate memory module.
     * @param filename  Filename according to the filename convention (see filenames.hpp)
     * @return          0 on success, negative error code on failure.
     */
    static etl::expected<int, lfs_error> eraseFile(const char* filename);

    /**
     * @Description Returns the size of the selected file in bytes.
     * @param filename  Filename according to the filename convention (see filenames.hpp)
     * @return size on success, negative error code on failure.
     */
    static etl::expected<size_t, lfs_error> getFileSize(const char* filename);

    /**
     * @Description Get the total memory use percentage of all memory modules available.
     * @return float percent on success, negative error code on failure.
     */
    static etl::expected<float, lfs_error> getUsedMemory();

    static bool formatNANDmodules();

    static bool setParameter(ParameterId parameter, void* value);

    static bool getParameter(ParameterId parameter, void* value);

    static uint64_t getParameterAsUINT64(ParameterId parameter);
};

inline std::optional<MemManTask> memManTask;
