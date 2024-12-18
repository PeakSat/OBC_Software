#pragma once

#include "etl/array.h"
#include "Peripheral_Definitions.hpp"

#include <peripheral/mcan/plib_mcan_common.h>

/**
 * A CAN::Packet is part of a CAN message. The driver can only handle packets of 1024 bytes maximum length.
 * Any bigger messages need to be dissected into packets by the application layer.
 * Packets are then divided into frames by the driver.
 */
namespace CAN {
    /**
 * The maximum data length that is currently configured in the peripheral.
 */
    static constexpr uint8_t MaxPayloadLength = 8;
    static constexpr uint8_t MaxPacketLength = 1024;

    enum CANInstance {
        CAN1,
        CAN2
    };

    class Frame {
    public:
        /**
     * Pointer to the bus from which the frame came from
     */
        CANInstance bus;

        /**
     * The header contains information about the frame,
     * such as sender ID
      */
        MCAN_RX_BUFFER header;

        /**
      *Pointer to the 64 byte buffer of data
      */
        uint8_t* pointerToData;

        Frame()
            : pointerToData(0) {
        }
    };


    /**
* A class to handle the incoming frames and combine them into a packet (1024 bytes max).
* The application layer does not deal with frames, only packets.
 */
    class Packet {
    public:
        // static constexpr uint8_t MaxDataLength = 8;

        /**
         * The right aligned ID of the message to be sent. Since the protocol doesn't make use of extended IDs,
         * they should be at most 11 bits long.
         *
         * @note The ID here must match one of the IDs found in DDJF_OBDH.
         */
        uint32_t id = 0;

        /**
         * A array containing the message payload.
         *
         * @note Users should use data.push_back() instead of data[i] while adding items to avoid errors caused by
         * copying the array to the gatekeeper queue.
         */
        etl::array<uint8_t, MaxPayloadLength> data = {};

        Packet() = default;

        Packet(uint32_t id) : id(id){};

        Packet(uint32_t id, const etl::array<uint8_t, MaxPayloadLength>& data) : id(id), data(data){};

        /**
         * Zeroes out the current frame. Use this if you're using a single static object in a recurring function.
         */
        inline void empty() {
            id = 0;
            data.fill(0);
        }
    };
} // namespace CAN
