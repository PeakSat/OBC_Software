#pragma once

#include "CAN/ApplicationLayer.hpp"
#include "CAN/Frame.hpp"
#include "TaskConfigs.hpp"
#include "queue.h"
#include "Platform/Peripheral_Definitions.hpp"
#include "CAN/TPProtocol.hpp"
#ifdef OBC_EQM_LCL
#include "LCLDefinitions.hpp"
#endif
/**
* Every variable needed to control the incoming frames' fifo buffer
* will be stored in this struct.
 */
struct incomingFIFO {
    uint8_t* buffer;
    uint32_t NOfItems;
    uint32_t lastItemPointer;
    incomingFIFO() : buffer(nullptr), NOfItems(0), lastItemPointer(0) {}
    incomingFIFO(uint8_t* externalBuffer, uint32_t NOfItems) : buffer(externalBuffer), NOfItems(NOfItems), lastItemPointer(0) {}
};
extern incomingFIFO incomingFIFO;
/**
 * Contains functionality of a Gatekeeper Task for the CAN Bus. It has the sole access to CAN, to avoid any
 * deadlocks that might be caused by simultaneous requests of access to the same resource. It works by having anyone
 * needing to access CAN, send the data in a queue. Then this task receives queue elements and sends them via CAN.
 *
 * @example @code
 * uint32_t id = 0x4; // Specify the sending Node ID.
 * etl::vector<uint8_t, 8> data = {0,1,2,3,4,5,6,7}; // Specify an array of data, up to 64 bytes.
 * CAN::Frame message = {id, data}; // Create a CAN::Frame object.
 * canGatekeeperTask->addToQueue(message); // Add the message to the outgoing queue.
 * @endcode
 */
class CANGatekeeperTask : public Task {
public:
    /**
     * A freeRTOS queue to handle outgoing messages, to keep order in case tasks interrupt each other.
     */
    QueueHandle_t outgoingQueue;

    /**
     * The variable used to hold the queue's data structure.
     */
    static inline StaticQueue_t outgoingQueueBuffer;

    /**
     * Storage area given to freeRTOS to manage the queue items.
     */
    static inline uint8_t outgoingQueueStorageArea[CAN::FrameQueueSize * sizeof(CAN::Frame)];

    /**
* A freeRTOS queue to handle incoming CAN frames.
*/
    QueueHandle_t incomingFrameQueue;
    /**
   * The variable used to hold the queue's data structure.
   */
    static inline StaticQueue_t incomingFrameQueueBuffer;

    /**
   * Storage area given to freeRTOS to manage the queue items.
   */
    static inline uint8_t incomingFrameQueueStorageArea[sizeOfIncommingFrameBuffer * sizeof(CAN::Frame)];

    StackType_t taskStack[CANGatekeeperTaskStack]{};

    CAN::Driver::ActiveBus ActiveBus = CAN::Driver::ActiveBus::Main;

public:
    TickType_t lastTransmissionTime = 0;

    void execute();

    /**
     * The constructor calls the initialize() function of the CAN::Driver. It also initializes the FreeRTOS queues for
     * incoming/outgoing messages.
     */
    CANGatekeeperTask();

    /**
     * Adds an CAN::Frame to the CAN Gatekeeper's queue.
     *
     * This function was added as an extra abstraction layer to house the `xQueueSendToBack` function.
     * It can be used from anywhere in the code to get access to the CAN queue/CAN Gatekeeper task, without having to
     * know the low level details of the queue.
     *
     * If the queue is full, the message is not added to the queue and an error is logged.
     *
     * @param message the CAN::Frame to be added in the queue of the CAN Gatekeeper task.
     * @param isISR indicating if the message is a response to another CAN Message, thus composed through an ISR
     */
    inline void send(const CAN::Packet& message, bool isISR = false) {
        BaseType_t status;

        if (isISR) {
            BaseType_t taskShouldYield = pdFALSE;

            status = xQueueSendToBackFromISR(outgoingQueue, &message, &taskShouldYield);

            if (taskShouldYield) {
                taskYIELD();
            }
        } else {
            status = xQueueSendToBack(outgoingQueue, &message, 0);
        }

        if (status == errQUEUE_FULL) {
            // LOG_ERROR << "Tried sending CAN Message while outgoing queue is full!";
        }
    }

    /**
     * Adds a CAN::Frame to the incomingQueue.
     * If the queue is full the message is lost.
     *
     * @note This function is designed to be used from within the ISR of a CAN Message Receipt. Thus, it uses
     * freeRTOS's ISR-Specific functions.
     *
     * @param message The incoming CAN::Frame.
     */
    // inline void addSFToIncoming(const CAN::Packet& message) {
    //     BaseType_t taskShouldYield = pdFALSE;
    //
    //     // xQueueSendToBackFromISR(incomingSFQueue, &message, &taskShouldYield);
    //
    //     if (taskShouldYield) {
    //         taskYIELD();
    //     }
    // }

    /**
     * Adds a CAN::Frame to the incomingQueue.
     * If the queue is full the message is lost.
     *
     * @note This function is designed to be used from within the ISR of a CAN Message Receipt. Thus, it uses
     * freeRTOS's ISR-Specific functions.
     *
     * @param message The incoming CAN::Frame.
     */
    // inline void addMFToIncoming(const CAN::Packet& message) {
    //     BaseType_t taskShouldYield = pdFALSE;
    //
    //     // xQueueSendToBackFromISR(incomingMFQueue, &message, &taskShouldYield);
    //
    //     if (taskShouldYield) {
    //         taskYIELD();
    //     }
    // }

    /**
     * An abstraction layer over the freeRTOS queue API to get the number of messages in the incoming queue.
     * @return The number of messages in the queue.
     */
    // inline uint8_t getIncomingSFMessagesCount() {
    //     // return uxQueueMessagesWaiting(incomingSFQueue);
    //     return 0;
    // }

    /**
     * An abstraction layer over the freeRTOS queue API to get the number of messages in the incoming queue.
     * @return The number of messages in the queue.
     */
    // inline uint8_t getIncomingMFMessagesCount() {
    //     // return uxQueueMessagesWaiting(incomingMFQueue);
    //     return 0;
    // }

    /**
     * Receives a CAN::Frame from the CAN Gatekeeper's incoming queue.
     *
     * This function was added as an extra abstraction layer to house the `xQueueReceive` function.
     * It can be used from anywhere in the code to get access to the CAN queue/CAN Gatekeeper task, without having to
     * know the low level details of the queue.
     *
     * If the queue is empty, the returned message is empty.
     */
    // inline CAN::Frame getFromSFQueue() {
    //     CAN::Frame message;
    //     // xQueueReceive(incomingSFQueue, &message, 0);
    //     return message;
    // }

    inline void switchActiveBus(CAN::Driver::ActiveBus activeBus) {
        this->ActiveBus = activeBus;
    }


    /**
     * Deletes all items present in the incoming queue.
     */
    // void emptyIncomingSFQueue() {
    //     // xQueueReset(incomingSFQueue);
    // }

    // inline CAN::Packet getFromMFQueue() {
    //     CAN::Packet message;
    //     // xQueueReceive(incomingMFQueue, &message, 0);
    //     return message;
    // }

    /**
     * Deletes all items present in the incoming queue.
     */
    // void emptyIncomingMFQueue() {
    //     // xQueueReset(incomingMFQueue);
    // }

    void createTask() {
        taskHandle = xTaskCreateStatic(vClassTask<CANGatekeeperTask>, this->TaskName, CANGatekeeperTaskStack, this,
                                       CANGatekeeperTaskPriority, this->taskStack, &(this->taskBuffer));
    }
};

inline std::optional<CANGatekeeperTask> canGatekeeperTask;