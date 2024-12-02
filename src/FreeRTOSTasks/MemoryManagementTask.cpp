//
// Created by tats on 2/12/2024.
//
#include "MemoryManagementTask.hpp"

MRAM mram(SMC::NCS0);
MT29F mt29f_part_a(SMC::NCS3, MEM_NAND_BUSY_1_PIN, MEM_NAND_WR_ENABLE_PIN);
MT29F mt29f_part_b(SMC::NCS1, MEM_NAND_BUSY_2_PIN, MEM_NAND_WR_ENABLE_PIN);

// Static buffer allocations (aligned to requirements)
static uint8_t nand_a_read_buffer[MaxMemoryElementByteSize];    // Must match read_size
static uint8_t nand_a_prog_buffer[MaxMemoryElementByteSize];    // Must match prog_size
static uint8_t nand_a_lookahead_buffer[MaxMemoryLookaheadByteSize]; // Must match lookahead_size

static uint8_t nand_b_read_buffer[MaxMemoryElementByteSize];    // Must match read_size
static uint8_t nand_b_prog_buffer[MaxMemoryElementByteSize];    // Must match prog_size
static uint8_t nand_b_lookahead_buffer[MaxMemoryLookaheadByteSize]; // Must match lookahead_size

// Global LFS instances

lfs_t lfs_nand_a;
lfs_t lfs_nand_b;
lfs_t lfs_mram;

// Error Translating functions

void printNANDError(MT29F_Errno error){
    switch (error) {
        case TIMEOUT:
            LOG_DEBUG<<"TIMEOUT";
            break;
        case ADDRESS_OUT_OF_BOUNDS:
            LOG_DEBUG<<"ADDRESS OUT OF BOUNDS";
            break;
        case BUSY_IO:
            LOG_DEBUG<<"MODULE BUSY (I/O)";
            break;
        case BUSY_ARRAY:
            LOG_DEBUG<<"MODULE BUSY (ARRAY)";
            break;
        case FAIL_PREVIOUS:
            LOG_DEBUG<<"PREVIOUS OPERATION FAILED";
            break;
        case FAIL_RECENT:
            LOG_DEBUG<<"RECENT OPERATION FAILED";
            break;
        case NOT_READY:
            LOG_DEBUG<<"MODULE NOT READY";
            break;
        default:
            LOG_DEBUG<<"UNKNOWN ERROR";
            break;
    }
}

void printMRAMErrno(MRAM_Errno error){
    switch (error) {
        case MRAM_Errno::MRAM_TIMEOUT:
            LOG_DEBUG<<"MRAM Error Timeout";
            break;
        case MRAM_Errno::MRAM_NOT_READY:
            LOG_DEBUG<<"MRAM Error Not Ready";
            break;
        case MRAM_Errno::MRAM_ADDRESS_OUT_OF_BOUNDS:
            LOG_DEBUG<<"MRAM Address Out Of Bounds";
            break;
        default:
            LOG_DEBUG<<"Unknown MRAM Errno: "<<error;
            break;
    }
}

// LFS functions for NAND part A

int lfs_nand_a_read(const struct lfs_config *c, lfs_block_t block, lfs_off_t off, void *buffer, lfs_size_t size) {
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
    if (result != MT29F_Errno ::NONE){
        printNANDError(result);
        return -1;
    }
    return 0;
}

int lfs_nand_a_prog(const struct lfs_config *c, lfs_block_t block, lfs_off_t off, const void *buffer, lfs_size_t size) {
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
    MT29F_Errno result =mt29f_part_a.writeNAND(LUN, position, data);

    // Handle the result of the write operation
    if (result != MT29F_Errno ::NONE){
        printNANDError(result);
        return -1;
    }
    return 0;
}

int lfs_nand_a_erase(const struct lfs_config *c, lfs_block_t block) {

    // LUN always 0
    uint8_t LUN = 0;

    MT29F_Errno result = mt29f_part_a.eraseBlock(LUN, block);

    // Handle the result of the erase block operation
    if (result != MT29F_Errno ::NONE){
        printNANDError(result);
        return -1;
    }

    return 0;  // Success
}

int lfs_nand_a_sync(const struct lfs_config *c) {
    // Perform any necessary synchronization
    if (!mt29f_part_a.getRDYstatus()) {
        return LFS_ERR_IO;  // Return an error if the sync fails
    }

    return 0;  // Success
}

// LFS functions for NAND part B

int lfs_nand_b_read(const struct lfs_config *c, lfs_block_t block, lfs_off_t off, void *buffer, lfs_size_t size) {
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
    if (result != MT29F_Errno ::NONE){
        printNANDError(result);
        return -1;
    }
    return 0;
}

int lfs_nand_b_prog(const struct lfs_config *c, lfs_block_t block, lfs_off_t off, const void *buffer, lfs_size_t size) {
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
    MT29F_Errno result =mt29f_part_b.writeNAND(LUN, position, data);

    // Handle the result of the write operation
    if (result != MT29F_Errno ::NONE){
        printNANDError(result);
        return -1;
    }
    return 0;
}

int lfs_nand_b_erase(const struct lfs_config *c, lfs_block_t block) {

    // LUN always 0
    uint8_t LUN = 0;

    MT29F_Errno result = mt29f_part_b.eraseBlock(LUN, block);

    // Handle the result of the erase block operation
    if (result != MT29F_Errno ::NONE){
        printNANDError(result);
        return -1;
    }

    return 0;  // Success
}

int lfs_nand_b_sync(const struct lfs_config *c) {
    // Perform any necessary synchronization
    if (!mt29f_part_b.getRDYstatus()) {
        return LFS_ERR_IO;  // Return an error if the sync fails
    }

    return 0;  // Success
}

// LFS Configuration & Mounting Functions

bool configureMountFS_NAND_A(){
    const struct lfs_config nand_cfg = {
        // block device operations
        .read  = lfs_nand_a_read,
        .prog  = lfs_nand_a_prog,
        .erase = lfs_nand_a_erase,
        .sync  = lfs_nand_a_sync,

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

    int err = lfs_mount(&lfs_nand_a, &nand_cfg);
    if (err) {
        // reformat if we can't mount the filesystem
        // this should only happen on the first boot
        LOG_DEBUG<<"Formatting LittleFS for NAND A";
        lfs_format(&lfs_nand_a, &nand_cfg);
        err = lfs_mount(&lfs_nand_a, &nand_cfg);
        if(err){
            return false;
        }
    }
    return true;
}

bool configureMountFS_NAND_B(){
    const struct lfs_config nand_cfg = {
        // block device operations
        .read  = lfs_nand_b_read,
        .prog  = lfs_nand_b_prog,
        .erase = lfs_nand_b_erase,
        .sync  = lfs_nand_b_sync,

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

    int err = lfs_mount(&lfs_nand_b, &nand_cfg);
    if (err) {
        // reformat if we can't mount the filesystem
        // this should only happen on the first boot
        LOG_DEBUG<<"Formatting LittleFS for NAND B";
        lfs_format(&lfs_nand_b, &nand_cfg);
        err = lfs_mount(&lfs_nand_b, &nand_cfg);
        if(err){
            return false;
        }
    }
    return true;
}

void initLCLs(){
    LCL& nandLCL = LCLDefinitions::lclArray[LCLDefinitions::NANDFlash];
    nandLCL.enableLCL();
    LCL& mramLCL = LCLDefinitions::lclArray[LCLDefinitions::MRAM];
    mramLCL.enableLCL();
}

bool initNANDinstance(MT29F nand){
    if(nand.resetNAND()!=MT29F_Errno::NONE){
        LOG_DEBUG<<"Error reseting NAND";
        return false;
//        mt29f.errorHandler(mt29f.resetNAND());
    }

    if(not nand.isNANDAlive()){
        LOG_DEBUG<<"NAND ID Error (Die A)";
        return false;
        // Error handler logic
    }
    return true;
}


void MemManTask::execute() {
    // Enable LCLs
    initLCLs();

    // Init memories
    if(not initNANDinstance(mt29f_part_a)){
        LOG_DEBUG << "Error initialising NAND part a";
        vTaskSuspend(NULL);
    }
    LOG_DEBUG << "NAND part a initialised";

    if(not initNANDinstance(mt29f_part_b)){
        LOG_DEBUG << "Error initialising NAND part b";
        vTaskSuspend(NULL);
    }
    LOG_DEBUG << "NAND part b initialised";

    MRAM_Errno error = mram.isMRAMAlive();
    if( error != MRAM_Errno ::MRAM_READY){
        printMRAMErrno(error);
        LOG_DEBUG << "Failed to initialise MRAM";
        vTaskSuspend(NULL);
    }
    LOG_DEBUG << "MRAM initialised";

    // Mount LFS
    if(!configureMountFS_NAND_A()){
        LOG_DEBUG << "Error mounting FS on NAND part a";
        vTaskSuspend(NULL);
    }
    LOG_DEBUG << "NAND part a FS mounted";

    if(!configureMountFS_NAND_B()){
        LOG_DEBUG << "Error mounting FS on NAND part b";
        vTaskSuspend(NULL);
    }
    LOG_DEBUG << "NAND part b FS mounted";

    // Check memories
    // Print memories info

    while(true){
        vTaskSuspend(NULL);
    }
}
