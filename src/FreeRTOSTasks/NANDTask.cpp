#include "NANDTask.hpp"
#include <etl/to_string.h>
#include <etl/String.hpp>
#include "LCLDefinitions.hpp"
#include "MRAMTask.hpp"
#include "mutex_Handler.h"

etl::array<uint8_t, 500> test_data = {
    76, 111, 114, 101, 109, 32, 105, 112, 115, 117, 109, 32, 100, 111, 108, 111, 114, 32, 115, 105, 116, 32, 97, 109, 101, 116, 44, 32, 99, 111, 110, 115, 101, 99, 116, 101, 116, 117, 114, 32,
    97, 100, 105, 112, 105, 115, 99, 105, 110, 103, 32, 101, 108, 105, 116, 46, 32, 65, 101, 110, 101, 97, 110, 32, 118, 105, 118, 101, 114, 114, 97, 44, 32, 110, 105, 98, 104, 32, 118, 101, 108,
    32, 99, 111, 110, 115, 101, 113, 117, 97, 116, 32, 115, 117, 115, 99, 105, 112, 105, 116, 44, 32, 101, 120, 32, 110, 105, 115, 108, 32, 102, 101, 114, 109, 101, 110, 116, 117, 109, 32, 110, 
    105, 115, 105, 44, 32, 101, 103, 101, 116, 32, 108, 117, 99, 116, 117, 115, 32, 102, 101, 108, 105, 115, 32, 112, 117, 114, 117, 115, 32, 110, 111, 110, 32, 109, 97, 117, 114, 105, 115, 46, 
    32, 68, 111, 110, 101, 99, 32, 110, 117, 110, 99, 32, 116, 111, 114, 116, 111, 114, 44, 32, 98, 108, 97, 110, 100, 105, 116, 32, 115, 101, 100, 32, 115, 117, 115, 99, 105, 112, 105, 116, 32,
    118, 101, 108, 44, 32, 103, 114, 97, 118, 105, 100, 97, 32, 113, 117, 105, 115, 32, 110, 101, 113, 117, 101, 46, 32, 85, 116, 32, 118, 101, 108, 105, 116, 32, 101, 115, 116, 44, 32, 103, 114,
    97, 118, 105, 100, 97, 32, 97, 32, 118, 97, 114, 105, 117, 115, 32, 97, 116, 44, 32, 103, 114, 97, 118, 105, 100, 97, 32, 105, 110, 32, 108, 111, 114, 101, 109, 46, 32, 78, 117, 110, 99, 32,
    99, 117, 114, 115, 117, 115, 32, 105, 109, 112, 101, 114, 100, 105, 101, 116, 32, 117, 114, 110, 97, 44, 32, 117, 116, 32, 112, 111, 114, 116, 97, 32, 108, 105, 98, 101, 114, 111, 32, 97, 
    99, 99, 117, 109, 115, 97, 110, 32, 110, 101, 99, 46, 32, 77, 111, 114, 98, 105, 32, 104, 101, 110, 100, 114, 101, 114, 105, 116, 32, 118, 111, 108, 117, 116, 112, 97, 116, 32, 97, 117, 103, 
    117, 101, 44, 32, 110, 101, 99, 32, 99, 111, 110, 103, 117, 101, 32, 116, 101, 108, 108, 117, 115, 32, 112, 104, 97, 114, 101, 116, 114, 97, 32, 118, 101, 108, 46, 32, 83, 101, 100, 32, 117,
    116, 32, 110, 105, 98, 104, 32, 116, 114, 105, 115, 116, 105, 113, 117, 101, 44, 32, 114, 104, 111, 110, 99, 117, 115, 32, 114, 105, 115, 117, 115, 32, 102, 97, 117, 99, 105, 98, 117, 115, 
    44, 32, 101, 108, 101, 105, 102, 101, 110, 100, 32, 109, 97, 103, 110, 97, 46, 32, 80, 104, 97, 115, 101, 108, 108, 117, 115, 32, 114, 104, 111, 110, 99, 117, 115, 32, 112, 117, 108, 118, 
    105, 110, 97, 114, 32, 99, 111, 110, 115, 101, 113, 117, 97};

bool singleByteRWTest(MT29F nand_module){
    //LOG_DEBUG<<"NAND Single byte R/W test";
    uint8_t data = 'A';
    uint32_t address_pos = (uint32_t) rand();
    // LOG_DEBUG<<"Writing to: "<<address_pos;
    MT29F_Errno error = nand_module.writeNAND(0, address_pos, data);
    if(error != MT29F_Errno::NONE){
        LOG_DEBUG<<"Write operation returned error-code: "<<error;
        return false;
    }
    data = 0;
    //LOG_DEBUG<<"Reading from the same address";
    error = nand_module.readNAND(0, &data, address_pos);
    if(error != MT29F_Errno::NONE){
        LOG_DEBUG<<"Read operation returned error-code: "<<error;
        return false;
    }
    if(data != 'A'){
        LOG_DEBUG<<"Read unexpected data: "<<data;
        return false; 
    }
    return true;
}

bool singlePageRWTest(MT29F nand_module){
    //LOG_DEBUG<<"NAND 500 byte R/W test (single page)";
    // Page = 8640 bytes
    uint32_t selected_page = rand()%(4096*128);
    uint32_t address_pos = (8640 * selected_page) + 10;
    // Address position is 10 bytes after the start of the page so 500 bytes of data
    // will fit in a single page
    
    LOG_DEBUG<<"Testing page: "<<selected_page;
    etl::span<uint8_t> data_span(test_data.data(), test_data.size());
    MT29F_Errno error = nand_module.writeNAND((uint8_t) 0, address_pos, data_span);
    if(error != MT29F_Errno::NONE){
        LOG_DEBUG<<"Write operation returned error-code: "<<error;
        return false;
    }

    etl::array<uint8_t, 500> read_data;
    etl::span<uint8_t> data_span_read(read_data.data(), read_data.size()); 
    // LOG_DEBUG<<"Reading from: "<<address_pos;
    error = nand_module.readNAND(0, address_pos, data_span_read);
    if(error != MT29F_Errno::NONE){
        LOG_DEBUG<<"Read operation returned error-code: "<<error;
        return false;
    }
    if( (data_span_read[0] != test_data[0]) ||  (data_span_read[data_span_read.size()-1]!=test_data[test_data.size()-1] ) ){
        LOG_DEBUG<<"Read unexpected data";
        LOG_DEBUG<<"First read byte: "<<data_span_read[0];
        LOG_DEBUG<<"Last read byte: "<<data_span_read[data_span_read.size()-1];
        return false;
    }
    return true;
}

bool crossPageRWTest(MT29F nand_module){
    //LOG_DEBUG<<"NAND 500 byte R/W test (cross page)";
    // Page = 8640 bytes
    uint32_t selected_page = rand()%(4096*128);
    uint32_t address_pos = (8640 * selected_page) - 200;
    // Address position is 200 bytes before the end of the page so 500 bytes of data
    // wont fit in a single page
    
    LOG_DEBUG<<"Testing pages: "<<selected_page<<", "<<selected_page+1;
    etl::span<uint8_t> data_span(test_data.data(), test_data.size());
    MT29F_Errno error = nand_module.writeNAND((uint8_t) 0, address_pos, data_span);
    if(error != MT29F_Errno::NONE){
        LOG_DEBUG<<"Write operation returned error-code: "<<error;
        return false;
    }

    etl::array<uint8_t, 500> read_data;
    etl::span<uint8_t> data_span_read(read_data.data(), read_data.size()); 
    // LOG_DEBUG<<"Reading from: "<<address_pos;
    error = nand_module.readNAND(0, address_pos, data_span_read);
    if(error != MT29F_Errno::NONE){
        LOG_DEBUG<<"Read operation returned error-code: "<<error;
        return false;
    }
    if( (data_span_read[0] != test_data[0]) ||  (data_span_read[data_span_read.size()-1]!=test_data[test_data.size()-1] ) ){
        LOG_DEBUG<<"Read unexpected data";
        LOG_DEBUG<<"First read byte: "<<data_span_read[0];
        LOG_DEBUG<<"Last read byte: "<<data_span_read[data_span_read.size()-1];
        return false;
    }
    return true;
}

bool blockEraseTest(MT29F nand_module){
    // LOG_DEBUG<<"NAND Block Erase test";
    // Block = PageSize*128
    uint16_t selected_block = rand()%4096;
    uint32_t block_start_address = 8640 * 128 * selected_block;
    uint32_t next_block_start_address = 8640 * 128 * (selected_block+1);
    // Write to 10 addresses inside the selected block, then erase the block and check if the data is erased
    // LOG_DEBUG<<"Writing to random addresses inside a block";

    // LOG_DEBUG<<"Testing block: "<<selected_block;
    uint32_t addresses[10];
    uint8_t data = 'A';
    for(uint8_t i=0; i<10; i++){
        addresses[i] = (block_start_address) + (rand() % (next_block_start_address - block_start_address + 1));
        MT29F_Errno error = nand_module.writeNAND(0, addresses[i], data);
        if(error != MT29F_Errno::NONE){
           LOG_DEBUG<<"Write operation returned code: "<<error; 
           return false;
        }
    }
    MT29F_Errno error = nand_module.eraseBlock(0, selected_block);
    if(error != MT29F_Errno::NONE){
        LOG_DEBUG<<"Erase block operation returned code: "<<error;
        return false; 
    }
    for(uint8_t i=0; i<10; i++){
        MT29F_Errno error = nand_module.readNAND(0, &data, addresses[i]);
        if(error != MT29F_Errno::NONE){
           LOG_DEBUG<<"Read operation returned code: "<<error;
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
        //LOG_DEBUG<<"NAND Single byte RW test FAILED";
    }else{
        // LOG_DEBUG<<"NAND Single byte RW test SUCCEDED";
    }

    if(!singlePageRWTest(nand_module)){
        // LOG_DEBUG<<"NAND 500 byte RW (single page) test FAILED";
    }else{
        // LOG_DEBUG<<"NAND 500 byte RW (single page) test SUCCEDED";
    }

    if(!crossPageRWTest(nand_module)){
        // LOG_DEBUG<<"NAND 500 byte RW (cross page) test FAILED";
    }else{
        // LOG_DEBUG<<"NAND 500 byte RW (cross page) test SUCCEDED";
    }

    if(!blockEraseTest(nand_module)){
        // LOG_DEBUG<<"NAND Block Erase test FAILED";
    }else{
        // LOG_DEBUG<<"NAND Block Erase test SUCCEDED";
    }

}

void NANDTask::execute() {
    
    MT29F mt29f(SMC::NCS3, MEM_NAND_BUSY_1_PIN, MEM_NAND_WR_ENABLE_PIN);
    // MT29F mt29f_b(SMC::NCS1, MEM_NAND_BUSY_2_PIN, MEM_NAND_WR_ENABLE_PIN);

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

    // if(mt29f_b.resetNAND()!=MT29F_Errno::NONE){
    //     LOG_DEBUG<<"Error reseting NAND";
    //     mt29f_b.errorHandler(mt29f_b.resetNAND());
    // }


    // if(mt29f_b.isNANDAlive()){
    //     LOG_DEBUG<<"NAND ID correct";
    // }else{
    //     LOG_DEBUG<<"NAND ID Error";
    //     // Error handler logic
    // }

    while (true) {
        testNANDmodule(mt29f);
        // testNANDmodule(mt29f_b);
        // LOG_DEBUG << "Runtime is exiting: " << this->TaskName;

        UBaseType_t highWatermarkNAD = uxTaskGetStackHighWaterMark(NULL);
        LOG_DEBUG<<"NAND Watermark: "<<highWatermarkNAD;

        // vTaskResume(MRAMTask::mramTaskHandle);
        // vTaskSuspend(NULL);
        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}