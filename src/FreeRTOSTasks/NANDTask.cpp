#include "NANDTask.hpp"
#include "NANDTestData.hpp"
#include <etl/to_string.h>
#include <etl/String.hpp>
#include "LCLDefinitions.hpp"
#include "MRAMTask.hpp"
#include "mutex_Handler.h"

#include "lfs.h"

MT29F mt29f(SMC::NCS3, MEM_NAND_BUSY_1_PIN, MEM_NAND_WR_ENABLE_PIN);
MT29F mt29f_b(SMC::NCS1, MEM_NAND_BUSY_2_PIN, MEM_NAND_WR_ENABLE_PIN);

// Static buffer allocations (aligned to requirements)
static uint8_t read_buffer[256];    // Must match read_size
static uint8_t prog_buffer[256];    // Must match prog_size
static uint8_t lookahead_buffer[128]; // Must match lookahead_size / 8

void printError(MT29F_Errno error){
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

bool singleByteRWTest(MT29F nand_module){
//    LOG_DEBUG<<"NAND Single byte R/W test";
    uint8_t data = 'A';
    uint64_t address_pos = (uint64_t) rand()%4529848319;
//     LOG_DEBUG<<"Writing to: "<<address_pos;
    MT29F_Errno error = nand_module.writeNAND(0, address_pos, data);
    if(error != MT29F_Errno::NONE){
        LOG_DEBUG<<"Write operation returned error-code: "<<error;
        return false;
    }
    data = 0;
    //LOG_DEBUG<<"Reading from the same address";
    error = nand_module.readNAND(0, address_pos, data);
    if(error != MT29F_Errno::NONE){
        printError(error);
        return false;
    }
    if(data != 'A'){
        LOG_DEBUG<<"Read unexpected data: "<<data;
        return false;
    }
    return true;
}

bool singlePageRWTest(MT29F nand_module){
    //LOG_DEBUG<<"NAND 200 byte R/W test (single page)";
    uint8_t selected_page = (uint8_t) rand()%128;
    uint16_t selected_block = (uint16_t) rand()%4095; // One block before end to avoid checking if the selected page is the last available
    uint64_t address_pos = (8640*selected_page) + (1105920*selected_block) +10;
    // Address position is 10 bytes after the start of the page so 200 bytes of data
    // will fit in a single page
    if(address_pos > 4529848319){
        // Kati paei lathos me tis prakseis panw kai ginetai paded me f's kapoies fores
        address_pos = address_pos & 0x00000000ffffffff;
    }

    LOG_DEBUG<<"Testing block: "<<selected_block;
    LOG_DEBUG<<"Testing page: "<<selected_page;
    etl::span<uint8_t> data_span(nand_test_data.data(), nand_test_data.size());
    MT29F_Errno error = nand_module.writeNAND((uint8_t) 0, address_pos, data_span);
    if(error != MT29F_Errno::NONE){
        printError(error);
        return false;
    }

    etl::array<uint8_t, 200> read_data;
    etl::span<uint8_t> data_span_read(read_data.data(), read_data.size());
    // LOG_DEBUG<<"Reading from: "<<address_pos;
    error = nand_module.readNAND(0, address_pos, data_span_read);
    if(error != MT29F_Errno::NONE){
        printError(error);
        return false;
    }
    if( (data_span_read[0] != nand_test_data[0]) ||  (data_span_read[data_span_read.size()-1]!=nand_test_data[nand_test_data.size()-1] ) ){
        LOG_DEBUG<<"Read unexpected data";
        LOG_DEBUG<<"First read byte: "<<data_span_read[0];
        LOG_DEBUG<<"Last read byte: "<<data_span_read[data_span_read.size()-1];
        return false;
    }
    return true;
}

bool crossPageRWTest(MT29F nand_module){
    //LOG_DEBUG<<"NAND 200 byte R/W test (cross page)";
    // Page = 8640 bytes
    uint8_t selected_page = ((uint8_t)rand()%127) + 1; // // One page after begin to avoid checking for cross block
    uint16_t selected_block = (uint16_t)rand()%4096;
    uint64_t address_pos = (8640*selected_page) + (1105920*selected_block) - 50;
    if(address_pos > 4529848319){
        // Kati paei lathos me tis prakseis panw kai ginetai paded me f's kapoies fores
        address_pos = address_pos & 0x00000000ffffffff;
    }
    // Address position is 50 bytes before the end of the page so 200 bytes of data
    // won't fit in a single page

    LOG_DEBUG<<"Testing block: "<<selected_block;
    LOG_DEBUG<<"Testing pages: "<<selected_page-1 <<", "<<selected_page;
    etl::span<uint8_t> data_span(nand_test_data.data(), nand_test_data.size());
    MT29F_Errno error = nand_module.writeNAND((uint8_t) 0, address_pos, data_span);
    if(error != MT29F_Errno::NONE){
        printError(error);
        return false;
    }
    uint8_t data_s, data_e;
    error = nand_module.readNAND(0, address_pos, data_s);
    if(error != MT29F_Errno::NONE){
        printError(error);
        return false;
    }
    error = nand_module.readNAND(0, (address_pos+nand_test_data.size()-1), data_e);
    if(error != MT29F_Errno::NONE){
        printError(error);
        return false;
    }
    if((data_s!= nand_test_data[0]) || (data_e!=nand_test_data[nand_test_data.size()-1])){
        LOG_DEBUG<<"Write confirm operation failed";
        LOG_DEBUG<<"First read byte: "<<data_s;
        LOG_DEBUG<<"Last read byte: "<<data_e;
        return false;
    }

    etl::array<uint8_t, 200> read_data;
    etl::span<uint8_t> data_span_read(read_data.data(), read_data.size());
    // LOG_DEBUG<<"Reading from: "<<address_pos;
    error = nand_module.readNAND(0, address_pos, data_span_read);
    if(error != MT29F_Errno::NONE){
        printError(error);
        return false;
    }
    if( (data_span_read[0] != nand_test_data[0]) ||  (data_span_read[data_span_read.size()-1]!=nand_test_data[nand_test_data.size()-1] ) ){
        LOG_DEBUG<<"Cross page read operation failed";
        LOG_DEBUG<<"First read byte: "<<data_span_read[0];
        LOG_DEBUG<<"Last read byte: "<<data_span_read[data_span_read.size()-1];
        return false;
    }
    return true;
}

bool blockEraseTest(MT29F nand_module){
    // LOG_DEBUG<<"NAND Block Erase test";
    // Block = PageSize*128
    uint16_t selected_block = rand()%4095; // One block before end to avoid checking if the selected page is the last available
    uint64_t block_start_address = 1105920 * selected_block;
    uint64_t next_block_start_address = 1105920 * (selected_block+1);
    // Write to 10 addresses inside the selected block, then erase the block and check if the data is erased

    LOG_DEBUG<<"Testing block: "<<selected_block;
    uint64_t addresses[10];
    uint8_t data = 'A';
    for(uint8_t i=0; i<10; i++){
        addresses[i] = (block_start_address) + (rand() % (next_block_start_address - block_start_address + 1));
        if(addresses[i] > 4529848319){
            // Kati paei lathos me tis prakseis panw kai ginetai paded me f's kapoies fores
            addresses[i] = addresses[i] & 0x00000000ffffffff;
        }
        MT29F_Errno error = nand_module.writeNAND(0, addresses[i], data);
        if(error != MT29F_Errno::NONE){
            printError(error);
            return false;
        }
    }
    MT29F_Errno error = nand_module.eraseBlock(0, selected_block);
    if(error != MT29F_Errno::NONE){
        printError(error);
        return false;
    }
    for(uint8_t i=0; i<10; i++){
        error = nand_module.readNAND(0, addresses[i], data);
        if(error != MT29F_Errno::NONE){
            printError(error);
            return false;
        }
        if(data == 'A'){
            LOG_DEBUG<<"Error on address: "<<addresses[i];
            return false;
        }
    }
    return true;
}

void testNANDmodule(MT29F nand_module){
    if(!singleByteRWTest(nand_module)){
        LOG_DEBUG<<"NAND Single byte RW test FAILED";
    }else{
         LOG_DEBUG<<"NAND Single byte RW test SUCCEDED";
    }

    if(!singlePageRWTest(nand_module)){
         LOG_DEBUG<<"NAND 500 byte RW (single page) test FAILED";
    }else{
         LOG_DEBUG<<"NAND 500 byte RW (single page) test SUCCEDED";
    }

    if(!crossPageRWTest(nand_module)){
         LOG_DEBUG<<"NAND 500 byte RW (cross page) test FAILED";
    }else{
         LOG_DEBUG<<"NAND 500 byte RW (cross page) test SUCCEDED";
    }

    if(!blockEraseTest(nand_module)){
         LOG_DEBUG<<"NAND Block Erase test FAILED";
    }else{
         LOG_DEBUG<<"NAND Block Erase test SUCCEDED";
    }

}


int lfs_read(const struct lfs_config *c, lfs_block_t block, lfs_off_t off, void *buffer, lfs_size_t size) {
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
    MT29F_Errno result = mt29f.readNAND(LUN, position, data);

    // Handle the result of the read operation
    if (result != MT29F_Errno ::NONE){
        printError(result);
        return -1;
    }
    return 0;
}

int lfs_prog(const struct lfs_config *c, lfs_block_t block, lfs_off_t off, const void *buffer, lfs_size_t size) {
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
    MT29F_Errno result =mt29f.writeNAND(LUN, position, data);

    // Handle the result of the write operation
    if (result != MT29F_Errno ::NONE){
        printError(result);
        return -1;
    }
    return 0;
}

int lfs_erase(const struct lfs_config *c, lfs_block_t block) {

    // LUN always 0
    uint8_t LUN = 0;

    MT29F_Errno result = mt29f.eraseBlock(LUN, block);

    // Handle the result of the erase block operation
    if (result != MT29F_Errno ::NONE){
        printError(result);
        return -1;
    }

    return 0;  // Success
}

int lfs_sync(const struct lfs_config *c) {
    // Perform any necessary synchronization
    if (!mt29f.getRDYstatus()) {
        return LFS_ERR_IO;  // Return an error if the sync fails
    }

    return 0;  // Success
}


void NANDTask::execute() {

    LCL& nandLCL = LCLDefinitions::lclArray[LCLDefinitions::NANDFlash];
    nandLCL.enableLCL();

    if(mt29f.resetNAND()!=MT29F_Errno::NONE){
        LOG_DEBUG<<"Error reseting NAND";
        mt29f.errorHandler(mt29f.resetNAND());
    }

    if(mt29f.isNANDAlive()){
        LOG_DEBUG<<"NAND ID correct";
    }else{
        LOG_DEBUG<<"NAND ID Error";
        // Error handler logic
    }

     if(mt29f_b.resetNAND()!=MT29F_Errno::NONE){
         LOG_DEBUG<<"Error reseting NAND";
         mt29f_b.errorHandler(mt29f_b.resetNAND());
     }


     if(mt29f_b.isNANDAlive()){
         LOG_DEBUG<<"NAND ID correct";
     }else{
         LOG_DEBUG<<"NAND ID Error";
         // Error handler logic
     }

     vTaskDelay(pdMS_TO_TICKS(500));

     lfs_t lfs;
     lfs_file_t file;

     // configuration of the filesystem is provided by this struct
     const struct lfs_config cfg = {
         // block device operations
         .read  = lfs_read,
         .prog  = lfs_prog,
         .erase = lfs_erase,
         .sync  = lfs_sync,

         // block device configuration
         .read_size = 256,        // 8KB
         .prog_size = 256,        // 8KB
         .block_size = 1048576,    // 1MB (128 pages)
         .block_count = 4096,      // Total number of blocks in the LUN
         .block_cycles = 500,      // Erase cycles for wear leveling
         .cache_size = 8192,       // Same as page size
         .lookahead_size = 128,    // Lookahead buffer size

         // Static memory buffers
         .read_buffer = read_buffer,
         .prog_buffer = prog_buffer,
         .lookahead_buffer = lookahead_buffer,
     };



    while (true) {
//        LOG_DEBUG<<"Testing NAND Die A";
//        testNANDmodule(mt29f);
//        LOG_DEBUG<<"\nTesting NAND Die B";
//        testNANDmodule(mt29f_b);

        int err = lfs_mount(&lfs, &cfg);
        if (err) {
            // reformat if we can't mount the filesystem
            // this should only happen on the first boot
            LOG_DEBUG<<"Formatting LittleFS";
            lfs_format(&lfs, &cfg);
            lfs_mount(&lfs, &cfg);
        }

        // read current count
        uint32_t boot_count = 0;
        lfs_file_open(&lfs, &file, "boot_count", LFS_O_RDWR | LFS_O_CREAT);
        lfs_file_read(&lfs, &file, &boot_count, sizeof(boot_count));

        // update boot count
        boot_count += 1;
        lfs_file_rewind(&lfs, &file);
        lfs_file_write(&lfs, &file, &boot_count, sizeof(boot_count));

        // remember the storage is not updated until the file is closed successfully
        lfs_file_close(&lfs, &file);

        // release any resources we were using
        lfs_unmount(&lfs);

        // print the boot count
        LOG_DEBUG<<"Boot count: "<<boot_count;


        // LOG_DEBUG << "Runtime is exiting: " << this->TaskName;

        // vTaskResume(MRAMTask::mramTaskHandle);
         vTaskSuspend(NULL);
//        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}