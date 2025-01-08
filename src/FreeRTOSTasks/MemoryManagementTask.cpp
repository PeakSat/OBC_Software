//
// Created by tats on 2/12/2024.
//
#include "MemoryManagementTask.hpp"

MRAM mram(SMC::NCS0);
MT29F mt29f_part_a(SMC::NCS3, MEM_NAND_BUSY_1_PIN, MEM_NAND_WR_ENABLE_PIN);
MT29F mt29f_part_b(SMC::NCS1, MEM_NAND_BUSY_2_PIN, MEM_NAND_WR_ENABLE_PIN);

// Static buffer allocations (aligned to requirements)
static uint8_t nand_a_read_buffer[MaxMemoryElementByteSize];        // Must match read_size
static uint8_t nand_a_prog_buffer[MaxMemoryElementByteSize];        // Must match prog_size
static uint8_t nand_a_lookahead_buffer[MaxMemoryLookaheadByteSize]; // Must match lookahead_size

static uint8_t nand_b_read_buffer[MaxMemoryElementByteSize];        // Must match read_size
static uint8_t nand_b_prog_buffer[MaxMemoryElementByteSize];        // Must match prog_size
static uint8_t nand_b_lookahead_buffer[MaxMemoryLookaheadByteSize]; // Must match lookahead_size

// Global LFS instances

lfs_t lfs_nand_a;
lfs_t lfs_nand_b;

// Error Translating functions

void printNANDError(MT29F_Errno error) {
    switch (error) {
        case MT29F_Errno::TIMEOUT:
            LOG_DEBUG << "TIMEOUT";
            break;
        case MT29F_Errno::ADDRESS_OUT_OF_BOUNDS:
            LOG_DEBUG << "ADDRESS OUT OF BOUNDS";
            break;
        case MT29F_Errno::BUSY_IO:
            LOG_DEBUG << "MODULE BUSY (I/O)";
            break;
        case MT29F_Errno::BUSY_ARRAY:
            LOG_DEBUG << "MODULE BUSY (ARRAY)";
            break;
        case MT29F_Errno::FAIL_PREVIOUS:
            LOG_DEBUG << "PREVIOUS OPERATION FAILED";
            break;
        case MT29F_Errno::FAIL_RECENT:
            LOG_DEBUG << "RECENT OPERATION FAILED";
            break;
        case MT29F_Errno::NOT_READY:
            LOG_DEBUG << "MODULE NOT READY";
            break;
        default:
            LOG_DEBUG << "UNKNOWN ERROR";
            break;
    }
}

void printMRAMErrno(MRAM_Errno error) {
    switch (error) {
        case MRAM_Errno::MRAM_TIMEOUT:
            LOG_DEBUG << "MRAM Error Timeout";
            break;
        case MRAM_Errno::MRAM_NOT_READY:
            LOG_DEBUG << "MRAM Error Not Ready";
            break;
        case MRAM_Errno::MRAM_ADDRESS_OUT_OF_BOUNDS:
            LOG_DEBUG << "MRAM Address Out Of Bounds";
            break;
        default:
            LOG_DEBUG << "Unknown MRAM Errno: " << (uint8_t) error;
            break;
    }
}

// LFS functions for NAND part A

/**
 * @Description  Low level read function, controlled by LittleFS
 *               not meant to be called by the user
 *
 * @param c      : Specific configurations of the memory module
 * @param block  : Block to read
 * @param off    : Offset inside the block
 * @param buffer : Read data storage
 * @param size   : Size of the data to read
 * @return       : 0 on success, negative error code on failure
 */
int lfs_nand_a_read(const struct lfs_config* c, lfs_block_t block, lfs_off_t off, void* buffer, lfs_size_t size) {
    // Ensure the input parameters are valid
    if (buffer == NULL || size == 0) {
        return LFS_ERR_INVAL; // Invalid input
    }

    // Calculate the physical address in NAND flash
    uint64_t position = (block * c->block_size) + off;

    // Use the LUN (assumed to always be 0)
    uint8_t LUN = 0;

    // Wrap the output buffer in a span for the `readNAND` function
    etl::span<uint8_t> data(reinterpret_cast<uint8_t*>(buffer), size);

    // Call the `readNAND` function
    MT29F_Errno result = mt29f_part_a.readNAND(LUN, position, data);

    // Handle the result of the read operation
    if (result != MT29F_Errno ::NONE) {
        //        printNANDError(result);
        return LFS_ERR_IO;
    }
    return LFS_ERR_OK;
}

/**
 * @Description  Low level write function, controlled by LittleFS
 *               not meant to be called by the user
 *
 * @param c      : Specific configurations of the memory module
 * @param block  : Block to read
 * @param off    : Offset inside the block
 * @param buffer : Read data storage
 * @param size   : Size of the data to read
 * @return       : 0 on success, negative error code on failure
 */
int lfs_nand_a_prog(const struct lfs_config* c, lfs_block_t block, lfs_off_t off, const void* buffer, lfs_size_t size) {
    // Ensure the input parameters are valid
    if (buffer == NULL || size == 0) {
        return LFS_ERR_INVAL; // Invalid input
    }

    // Calculate the physical address in NAND flash
    uint64_t position = (block * c->block_size) + off;

    // LUN always 0
    uint8_t LUN = 0;

    // Wrap the input buffer in a span for the `writeNAND` function
    etl::span<const uint8_t> data(reinterpret_cast<const uint8_t*>(buffer), size);

    // Call the `writeNAND` function
    MT29F_Errno result = mt29f_part_a.writeNAND(LUN, position, data);

    // Handle the result of the write operation
    if (result != MT29F_Errno ::NONE) {
        //        printNANDError(result);
        return LFS_ERR_IO;
    }
    return LFS_ERR_OK;
}

/**
 * @Description  Low level erase function, controlled by LittleFS
 *               not meant to be called by the user
 *
 * @param c      : Specific configurations of the memory module
 * @param block  : Block to read
 * @return       : 0 on success, negative error code on failure
 */
int lfs_nand_a_erase(const struct lfs_config* c, lfs_block_t block) {
    uint8_t LUN = 0; // LUN always 0
    MT29F_Errno result = mt29f_part_a.eraseBlock(LUN, block);
    // Handle the result of the erase block operation
    if (result != MT29F_Errno ::NONE) {
        //        printNANDError(result);
        return LFS_ERR_IO;
    }

    return LFS_ERR_OK; // Success
}

/**
 * @Description  Low level sync function, controlled by LittleFS
 *               not meant to be called by the user
 *
 * @param c : Specific configurations of the memory module
 * @return  : 0 on success, negative error code on failure
 */
int lfs_nand_a_sync(const struct lfs_config* c) {
    // Perform any necessary synchronization
    if (!mt29f_part_a.getRDYstatus()) {
        return LFS_ERR_IO; // Return an error if the sync fails
    }

    return LFS_ERR_OK; // Success
}

// LFS functions for NAND part B

/**
 * @Description  Low level read function, controlled by LittleFS
 *               not meant to be called by the user
 *
 * @param c      : Specific configurations of the memory module
 * @param block  : Block to read
 * @param off    : Offset inside the block
 * @param buffer : Read data storage
 * @param size   : Size of the data to read
 * @return       : 0 on success, negative error code on failure
 */
int lfs_nand_b_read(const struct lfs_config* c, lfs_block_t block, lfs_off_t off, void* buffer, lfs_size_t size) {
    // Ensure the input parameters are valid
    if (buffer == NULL || size == 0) {
        return LFS_ERR_INVAL; // Invalid input
    }

    // Calculate the physical address in NAND flash
    uint64_t position = (block * c->block_size) + off;

    // Use the LUN (assumed to always be 0)
    uint8_t LUN = 0;

    // Wrap the output buffer in a span for the `readNAND` function
    etl::span<uint8_t> data(reinterpret_cast<uint8_t*>(buffer), size);

    // Call the `readNAND` function
    MT29F_Errno result = mt29f_part_b.readNAND(LUN, position, data);

    // Handle the result of the read operation
    if (result != MT29F_Errno ::NONE) {
        //        printNANDError(result);
        return LFS_ERR_IO;
    }
    return LFS_ERR_OK;
}

/**
 * @Description  Low level write function, controlled by LittleFS
 *               not meant to be called by the user
 *
 * @param c      : Specific configurations of the memory module
 * @param block  : Block to read
 * @param off    : Offset inside the block
 * @param buffer : Read data storage
 * @param size   : Size of the data to read
 * @return       : 0 on success, negative error code on failure
 */
int lfs_nand_b_prog(const struct lfs_config* c, lfs_block_t block, lfs_off_t off, const void* buffer, lfs_size_t size) {
    // Ensure the input parameters are valid
    if (buffer == NULL || size == 0) {
        return LFS_ERR_INVAL; // Invalid input
    }

    // Calculate the physical address in NAND flash
    uint64_t position = (block * c->block_size) + off;

    // LUN always 0
    uint8_t LUN = 0;

    // Wrap the input buffer in a span for the `writeNAND` function
    etl::span<const uint8_t> data(reinterpret_cast<const uint8_t*>(buffer), size);

    // Call the `writeNAND` function
    MT29F_Errno result = mt29f_part_b.writeNAND(LUN, position, data);

    // Handle the result of the write operation
    if (result != MT29F_Errno ::NONE) {
        //        printNANDError(result);
        return LFS_ERR_IO;
    }
    return LFS_ERR_OK;
}

/**
 * @Description  Low level erase function, controlled by LittleFS
 *               not meant to be called by the user
 *
 * @param c      : Specific configurations of the memory module
 * @param block  : Block to read
 * @return       : 0 on success, negative error code on failure
 */
int lfs_nand_b_erase(const struct lfs_config* c, lfs_block_t block) {

    // LUN always 0
    uint8_t LUN = 0;

    MT29F_Errno result = mt29f_part_b.eraseBlock(LUN, block);

    // Handle the result of the erase block operation
    if (result != MT29F_Errno ::NONE) {
        //        printNANDError(result);
        return LFS_ERR_IO;
    }

    return LFS_ERR_OK; // Success
}

/**
 * @Description  Low level sync function, controlled by LittleFS
 *               not meant to be called by the user
 *
 * @param c : Specific configurations of the memory module
 * @return  : 0 on success, negative error code on failure
 */
int lfs_nand_b_sync(const struct lfs_config* c) {
    // Perform any necessary synchronization
    if (!mt29f_part_b.getRDYstatus()) {
        return LFS_ERR_IO; // Return an error if the sync fails
    }

    return LFS_ERR_OK; // Success
}

// LFS Configuration & Mounting Functions

const struct lfs_config nand_a_cfg = {
    // block device operations
    .read = lfs_nand_a_read,
    .prog = lfs_nand_a_prog,
    .erase = lfs_nand_a_erase,
    .sync = lfs_nand_a_sync,

    // block device configuration
    .read_size = MaxMemoryElementByteSize,
    .prog_size = MaxMemoryElementByteSize,
    .block_size = NAND_partition_size_bytes,
    .block_count = NAND_partition_blocks,
    .block_cycles = NAND_wear_leveling_cycles,
    .cache_size = MaxMemoryElementByteSize, // Must match read_size & prog_size
    .lookahead_size = MaxMemoryLookaheadByteSize,

    // Static memory buffers
    .read_buffer = nand_a_read_buffer,
    .prog_buffer = nand_a_prog_buffer,
    .lookahead_buffer = nand_a_lookahead_buffer,
};

const struct lfs_config nand_b_cfg = {
    // block device operations
    .read = lfs_nand_b_read,
    .prog = lfs_nand_b_prog,
    .erase = lfs_nand_b_erase,
    .sync = lfs_nand_b_sync,

    // block device configuration
    .read_size = MaxMemoryElementByteSize,
    .prog_size = MaxMemoryElementByteSize,
    .block_size = NAND_partition_size_bytes,
    .block_count = NAND_partition_blocks,
    .block_cycles = NAND_wear_leveling_cycles,
    .cache_size = MaxMemoryElementByteSize, // Must match read_size & prog_size
    .lookahead_size = MaxMemoryLookaheadByteSize,

    // Static memory buffers
    .read_buffer = nand_b_read_buffer,
    .prog_buffer = nand_b_prog_buffer,
    .lookahead_buffer = nand_b_lookahead_buffer,
};

/**
 * @Description Mounts and if necessary format LFS on the NAND modules
 * @return true on success
 */
bool configureMountFS_NAND() {
    int err_nand_a = lfs_mount(&lfs_nand_a, &nand_a_cfg);
    if (err_nand_a) {
        // Format if we can't mount the filesystem
        // this should only happen on the first boot
        LOG_DEBUG << "Formatting LittleFS for NAND A";
        lfs_format(&lfs_nand_a, &nand_a_cfg);
        err_nand_a = lfs_mount(&lfs_nand_a, &nand_a_cfg);
    }

    int err_nand_b = lfs_mount(&lfs_nand_b, &nand_b_cfg);
    if (err_nand_b) {
        // Format if we can't mount the filesystem
        // this should only happen on the first boot
        LOG_DEBUG << "Formatting LittleFS for NAND B";
        lfs_format(&lfs_nand_b, &nand_b_cfg);
        err_nand_b = lfs_mount(&lfs_nand_b, &nand_b_cfg);
    }

    if (err_nand_a || err_nand_b) {
        return false;
    }
    return true; // Success
}

void initLCLs() {
    LCL& nandLCL = LCLDefinitions::lclArray[LCLDefinitions::NANDFlash];
    nandLCL.enableLCL();
    LCL& mramLCL = LCLDefinitions::lclArray[LCLDefinitions::MRAM];
    mramLCL.enableLCL();
}

/**
 * Initializes and performs health checks on the NAND modules
 * @return true on success
 */
bool initNAND() {
    bool flag = false;
    if (mt29f_part_a.resetNAND() != MT29F_Errno::NONE) {
        LOG_DEBUG << "Error reseting NAND A";
        flag = true;
    }

    if (not mt29f_part_a.isNANDAlive()) {
        LOG_DEBUG << "NAND ID Error (Die A)";
        flag = true;
    }

    if (mt29f_part_b.resetNAND() != MT29F_Errno::NONE) {
        LOG_DEBUG << "Error reseting NAND B";
        flag = true;
    }

    if (not mt29f_part_b.isNANDAlive()) {
        LOG_DEBUG << "NAND ID Error (Die B)";
        flag = true;
    }
    return !flag;
}

// Internal R/W functions

etl::expected<int, lfs_error> MemManTask::writeNANDFile(lfs* lfs, const char* filename, etl::span<const uint8_t>& data, FILE_RW_FLAGS flag) {
    lfs_file_t file;
    int flags = 0;
    switch (flag) {
        case FILE_RW_FLAGS::APPEND:
            flags = LFS_O_CREAT | LFS_O_APPEND | LFS_O_WRONLY;
            break;
        case FILE_RW_FLAGS::OVERWRITE:
            flags = LFS_O_CREAT | LFS_O_TRUNC | LFS_O_WRONLY;
            break;
        default:
            flags = LFS_O_WRONLY;
            break;
    }
    int error = lfs_file_open(lfs, &file, filename, flags);
    if (error < 0) {
        //        LOG_ERROR<<"Failed to open the selected file";
        return etl::unexpected((lfs_error) error);
    }
    error = lfs_file_write(lfs, &file, data.data(), data.size());
    if (error < 0) {
        //        LOG_ERROR<<"Failed writing the selected file";
        return etl::unexpected((lfs_error) error);
    }
    error = lfs_file_close(lfs, &file);
    if (error < 0) {
        //        LOG_ERROR<<"Failed syncing the selected file";
        return etl::unexpected((lfs_error) error);
    }
    return 0;
}

bool MemManTask::writeMRAM_File(const char* filename, etl::span<const uint8_t>& data, FILE_RW_FLAGS flag) {
    LOG_DEBUG << "MRAM Write Not implemented yet hehe";
    return true;
}

etl::expected<int, lfs_error> MemManTask::readNANDFile(lfs* lfs, const char* filename, etl::span<uint8_t>& data) {
    lfs_file_t file;
    int error = lfs_file_open(lfs, &file, filename, LFS_O_RDONLY);
    if (error < 0) {
        //        LOG_ERROR<<"Failed to open the selected file";
        return etl::unexpected((lfs_error) error);
    }
    error = lfs_file_read(lfs, &file, data.data(), data.size());
    if (error < 0) {
        //        LOG_ERROR<<"Failed reading the selected file";
        return etl::unexpected((lfs_error) error);
    }
    error = lfs_file_close(lfs, &file);
    if (error < 0) {
        //        LOG_ERROR<<"Failed syncing the selected file";
        return etl::unexpected((lfs_error) error);
    }
    return 0;
}

bool MemManTask::readMRAM_File(const char* filename, etl::span<uint8_t>& data) {
    LOG_DEBUG << "MRAM Read Not implemented yet hehe";
    return true;
}

bool MemManTask::eraseMRAMFile(const char* filename) {
    LOG_DEBUG << "MRAM Erase function not implemented yet, hehe";
    return true;
}

etl::expected<size_t, lfs_error> MemManTask::getNANDFileSize(lfs* lfs, const char* filename) {
    lfs_file_t file;
    int error = lfs_file_open(lfs, &file, filename, LFS_O_RDONLY);
    if (error < 0) {
        //        LOG_ERROR<<"Failed to open the selected file";
        return etl::unexpected((lfs_error) error);
    }
    size_t file_size = lfs_file_seek(lfs, &file, 0, LFS_SEEK_END);
    error = lfs_file_close(lfs, &file);
    if (error < 0) {
        //        LOG_ERROR<<"Failed syncing the selected file";
        return etl::unexpected((lfs_error) error);
    }
    return file_size;
}

size_t MemManTask::getMRAMFileSize(const char* filename) {
    LOG_DEBUG << "MRAM get file size function not implemented yet, hehe";
    return 0;
}

etl::expected<float, lfs_error> MemManTask::getUsedSpace(lfs* lfs) {
    lfs_ssize_t used_blocks = lfs_fs_size(lfs);
    if (used_blocks < 0) {
        //        LOG_ERROR<<"Failed retrieving FS used blocks";
        return etl::unexpected((lfs_error) used_blocks);
    }
    size_t used_size = used_blocks * NAND_partition_size_bytes;
    uint64_t total_size = (uint64_t) NAND_partition_blocks * NAND_partition_size_bytes;
    float percentage = ((float) used_size / total_size) * 100;
    //    LOG_DEBUG<<"Used space: "<<percentage<<"%";
    return percentage;
}

void MemManTask::printAvailableFiles(lfs* lfs) {
    lfs_dir_t dir;
    struct lfs_info info;

    int error = lfs_dir_open(lfs, &dir, "/");
    if (error < 0) {
        LOG_ERROR << "Failed opening root directory";
        return;
    }
    error = 1;
    while (error != 0) { // Error = 0 end of directory reached
        error = lfs_dir_read(lfs, &dir, &info);
        if (error < 0) {
            LOG_ERROR << "Failed to read directory";
            return;
        }
        if (info.type == LFS_TYPE_REG) {
            LOG_DEBUG << "File: " << info.name << " Size: " << (long long) info.size << " bytes";
        }
    }
    lfs_dir_close(lfs, &dir);
}

// Exposed R/W functions

etl::expected<int, lfs_error> MemManTask::writeToFile(const char* filename, etl::span<const uint8_t>& data, FILE_RW_FLAGS flags) {
    char id_byte = filename[FILE_ID_POS];
    switch (id_byte) {
        case 'A': {
            auto result = writeNANDFile(&lfs_nand_a, filename, data, flags);
            if (!result.has_value()) {
                return etl::unexpected(result.error());
            }
            break;
        }
        case 'B': {
            auto result = writeNANDFile(&lfs_nand_b, filename, data, flags);
            if (!result.has_value()) {
                return etl::unexpected(result.error());
            }
            break;
        }
        case 'C':
            return writeMRAM_File(filename, data, flags);
        default:
            //            LOG_DEBUG<<"Unknown File ID";
            return etl::unexpected(lfs_error::LFS_ERR_INVAL);
    }
    return 0; // Success
}

etl::expected<int, lfs_error> MemManTask::readFromFile(const char* filename, etl::span<uint8_t>& data) {
    char id_byte = filename[FILE_ID_POS];
    switch (id_byte) {
        case 'A': {
            auto result = readNANDFile(&lfs_nand_a, filename, data);
            if (!result.has_value()) {
                return etl::unexpected(result.error());
            }
            break;
        }
        case 'B': {
            auto result = readNANDFile(&lfs_nand_b, filename, data);
            if (!result.has_value()) {
                return etl::unexpected(result.error());
            }
            break;
        }
        case 'C':
            return readMRAM_File(filename, data);
        default:
            //            LOG_DEBUG<<"Unknown File ID";
            return etl::unexpected(lfs_error::LFS_ERR_INVAL);
    }
    return 0; // Success
}

etl::expected<int, lfs_error> MemManTask::eraseFile(const char* filename) {
    char id_byte = filename[FILE_ID_POS];
    int error = lfs_error::LFS_ERR_OK;
    switch (id_byte) {
        case 'A':
            error = lfs_remove(&lfs_nand_a, filename);
            break;
        case 'B':
            error = lfs_remove(&lfs_nand_b, filename);
            break;
        case 'C':
            return eraseMRAMFile(filename);
        default:
            //            LOG_ERROR<<"Unknown fileID";
            return etl::unexpected(lfs_error::LFS_ERR_INVAL);
    }
    if (error < 0) {
        //        LOG_DEBUG<<"File erase operation failed";
        return etl::unexpected((lfs_error) error);
    }
    return 0; //Success
}

etl::expected<size_t, lfs_error> MemManTask::getFileSize(const char* filename) {
    char id_byte = filename[FILE_ID_POS];

    switch (id_byte) {
        case 'A': {
            auto result = getNANDFileSize(&lfs_nand_a, filename);
            if (!result.has_value()) {
                return etl::unexpected(result.error());
            }
            return result.value();
        }
        case 'B': {
            auto result = getNANDFileSize(&lfs_nand_b, filename);
            if (!result.has_value()) {
                return etl::unexpected(result.error());
            }
            return result.value();
        }
        case 'C':
            return getMRAMFileSize(filename);
        default:
            //            LOG_ERROR<<"Unknown fileID";
            return etl::unexpected(lfs_error::LFS_ERR_INVAL);
    }
    // Should not reach here
    return 0;
}

bool MemManTask::formatNANDmodules() {
    bool err = lfs_unmount(&lfs_nand_a);
    err = lfs_format(&lfs_nand_a, &nand_a_cfg);
    err = lfs_unmount(&lfs_nand_b);
    err = lfs_format(&lfs_nand_b, &nand_b_cfg);
    err = !configureMountFS_NAND();
    return !err;
}

etl::expected<float, lfs_error> MemManTask::getUsedMemory() {
    auto nand_a_fill = getUsedSpace(&lfs_nand_a);
    auto nand_b_fill = getUsedSpace(&lfs_nand_b);
    // @Todo Implement function to get MRAM remaining space
    if (!nand_a_fill.has_value() || !nand_b_fill.has_value()) {
        return etl::unexpected(lfs_error::LFS_ERR_IO);
    }
    return (nand_a_fill.value() + nand_b_fill.value());
}


/**
 * Check if the stored version of the HASH ID is correct
 * @param hash_id the hash id read from Memory
 * @return true on correct match
 */
bool checkGitHash(char* hash_id) {
    if (hash_id[0] == 0) {
        return false;
    }
    for (uint8_t i = 0; i < 7; i++) {
        if (hash_id[i] != kGitHash[i]) {
            return false;
        }
    }
    return true;
}

bool MemManTask::updateBiosFile() {
    bios_file_content bios;
    //
    // etl::span<uint8_t> dataSpan(reinterpret_cast<uint8_t*>(&bios), sizeof(bios));
    // auto result = readFromFile(FILE_BIOS, dataSpan);
    // if(!result.has_value()){
    //     LOG_DEBUG<<"Unable to read BIOS file";
    //     // Continue since on the first boot it may not exist, the write function will create it
    //     bios.boot_count = 0;
    // }
    // // Check for updated FW version
    // if(bios.fw_ver[0]!=FW_VER_MAJOR ||  bios.fw_ver[1]!=FW_VER_MINOR || bios.fw_ver[2]!=FW_VER_PATCH){
    //     LOG_DEBUG<<"New FW version detected";
    //     bios.fw_ver[0]=FW_VER_MAJOR;
    //     bios.fw_ver[1]=FW_VER_MINOR;
    //     bios.fw_ver[2]=FW_VER_PATCH;
    // }
    // // TODO: Add last reset reason get function
    // // Check Git commit hash
    // if(!checkGitHash(bios.git_hash_id)){
    //     LOG_DEBUG<<"New Git Hash detected";
    //     memccpy(bios.git_hash_id, kGitHash, '\0', 10);
    // }
    //
    // bios.boot_count+=1;
    // LOG_DEBUG<<"FW_VERSION: "<<bios.fw_ver[0]<<"."<<bios.fw_ver[1]<<"."<<bios.fw_ver[2];
    // LOG_DEBUG<<"BOOT_COUNT: "<<bios.boot_count;
    // LOG_DEBUG<<"RESET_RSN : "<<bios.last_reset_cause;
    // LOG_DEBUG<<"GIT_HASH  : "<<bios.git_hash_id;
    //
    // etl::span<const uint8_t> writeSpan(reinterpret_cast<const uint8_t*>(&bios), sizeof(bios));
    // result = writeToFile(FILE_BIOS, writeSpan, FILE_RW_FLAGS::OVERWRITE);
    // if(!result.has_value()){
    //     LOG_DEBUG<<"Unable to write BIOS file";
    //     return false;
    // }
    return true;
}


void MemManTask::execute() {
    // Enable LCLs
    initLCLs();

    // // Init memories
    // if(not initNAND()){
    //     LOG_ERROR << "Error initialising NAND";
    //     // Error Handling?
    //     vTaskSuspend(NULL);
    // }
    // LOG_DEBUG << "NAND part initialised";
    //
    // MRAM_Errno error = mram.isMRAMAlive();
    // if( error != MRAM_Errno ::MRAM_READY){
    //     printMRAMErrno(error);
    //     LOG_ERROR << "Failed to initialise MRAM";
    //     // Error Handling?
    //     vTaskSuspend(NULL);
    // }
    // LOG_DEBUG << "MRAM initialised";
    //
    // vTaskDelay(pdMS_TO_TICKS(1000));
    //
    // // Mount LFS
    // if(!configureMountFS_NAND()){
    //     LOG_ERROR << "Error mounting FS on NAND";
    //     // Error Handling?
    //     vTaskSuspend(NULL);
    // }
    // LOG_DEBUG << "NAND FS mounted";
    //
    // vTaskDelay(pdMS_TO_TICKS(1000));
    //
    // // Print memory
    // auto result = getUsedMemory();
    // if(!result.has_value()){
    //     LOG_DEBUG<<"Unable to retrieve used memory, error code: "<<result.error();
    // }else{
    //     LOG_DEBUG<<"Total used space (%): "<<result.value();
    // }
    // LOG_DEBUG<<"Files stored in filesystem";
    // printAvailableFiles(&lfs_nand_a);
    // printAvailableFiles(&lfs_nand_b);
    // //@Todo add logic for MRAM
    //
    // //Update BIOS parameters
    // if(not updateBiosFile()){
    //     LOG_DEBUG<<"BIOS file update ERROR";
    // }else{
    //     LOG_DEBUG<<"BIOS file updated";
    // }

    while (true) {
        vTaskSuspend(NULL);
    }
}
