// #include "MRAMTask.hpp"
// #include "LCLDefinitions.hpp"
// #include "MRAMTestData.hpp"
//
// #include <MemoryManagementTask.hpp>
//
// void printMRAMErrno(MRAM_Errno error) {
//     switch (error) {
//         case MRAM_Errno::MRAM_TIMEOUT:
//             LOG_DEBUG << "MRAM Error Timeout";
//             break;
//         case MRAM_Errno::MRAM_NOT_READY:
//             LOG_DEBUG << "MRAM Error Not Ready";
//             break;
//         case MRAM_Errno::MRAM_ADDRESS_OUT_OF_BOUNDS:
//             LOG_DEBUG << "MRAM Address Out Of Bounds";
//             break;
//         default:
//             LOG_DEBUG << "Unknown MRAM Errno: " << (uint8_t) error;
//             break;
//     }
// }
//
// bool MRAMsingleByteRWTest(MRAM mram_module) {
//     uint8_t data = 'A';
//     uint32_t randomAddress = (uint32_t) rand() % (0x1FFFFD);
//     LOG_DEBUG << "Testing Address: " << randomAddress;
//     MRAM_Errno error = mram_module.mramWriteByte(randomAddress, data);
//     if (error != MRAM_Errno ::MRAM_NONE) {
//         printMRAMErrno(error);
//         return false;
//     }
//     data = 0;
//     error = mram_module.mramReadByte(randomAddress, data);
//     if (error != MRAM_Errno ::MRAM_NONE) {
//         printMRAMErrno(error);
//         return false;
//     }
//     if (data != 'A') {
//         LOG_DEBUG << "Read unexpected value: " << data;
//         return false;
//     }
//     return true;
// }
//
// bool MRAMmultipleBytesRWTest(MRAM mram_module) {
//     uint32_t randomAddress = (uint32_t) rand() % (0x1FFFFD - mram_test_data.size());
//     // Prevent overflow by subtracting from the maximum address the length of the array to be written
//     LOG_DEBUG << "Testing Address: " << randomAddress;
//
//     etl::span<const uint8_t> data_span_write(mram_test_data.data(), mram_test_data.size());
//     MRAM_Errno error = mram_module.mramWriteData(randomAddress, data_span_write);
//     if (error != MRAM_Errno ::MRAM_NONE) {
//         printMRAMErrno(error);
//         return false;
//     }
//
//     etl::array<uint8_t, 200> read_data{};
//     etl::span<uint8_t> data_span_read(read_data.data(), read_data.size());
//
//     error = mram_module.mramReadData(randomAddress, data_span_read);
//     if (error != MRAM_Errno ::MRAM_NONE) {
//         printMRAMErrno(error);
//         return false;
//     }
//
//     if ((data_span_read[0] != mram_test_data[0]) || (data_span_read[data_span_read.size() - 1] != mram_test_data[mram_test_data.size() - 1])) {
//         LOG_DEBUG << "Read unexpected data";
//         LOG_DEBUG << "First read byte: " << data_span_read[0];
//         LOG_DEBUG << "Last read byte: " << data_span_read[data_span_read.size() - 1];
//         return false;
//     }
//
//     return true;
// }
//
// void testMRAMmodule(MRAM mram_module) {
//
//     if (!MRAMsingleByteRWTest(mram_module)) {
//         LOG_DEBUG << "MRAM SINGLE BYTE RW TEST FAILED";
//     } else {
//         LOG_DEBUG << "MRAM SINGLE BYTE RW TEST SUCCEEDED";
//     }
//
//     if (!MRAMmultipleBytesRWTest(mram_module)) {
//         LOG_DEBUG << "MRAM MULTIPLE BYTE RW TEST FAILED";
//     } else {
//         LOG_DEBUG << "MRAM MULTIPLE BYTE RW TEST SUCCEEDED";
//     }
// }
//
//
// void MRAMTask::execute() {
//     LOG_DEBUG << "Runtime init: " << this->TaskName;
//
//     MRAM mram(SMC::NCS0);
//
//     LCL& mramLCL = LCLDefinitions::lclArray[LCLDefinitions::MRAM];
//     mramLCL.enableLCL();
//
//     MRAM_Errno error;
//     error = mram.isMRAMAlive();
//     if(error!=MRAM_Errno ::MRAM_READY){
//         printMRAMErrno(error);
//         mram.errorHandler(error);
//         vTaskSuspend(NULL);
//     }
//     LOG_DEBUG<<"MRAM Init Success";
//
//     vTaskDelay(pdMS_TO_TICKS(this->DelayMs));
//
//     while (true) {
//         LOG_DEBUG << "Runtime entered: " << this->TaskName;
//         testMRAMmodule(mram);
//         vTaskDelay(pdMS_TO_TICKS(this->DelayMs));
//     }
// }
