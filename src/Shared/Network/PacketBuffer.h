/*
* Liam Ashdown
* Copyright (C) 2019
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _NETWORK_PACKETBUFFER_h
#define _NETWORK_PACKETBUFFER_h
#include "../Common/SharedDefines.h"
#endif /* _NETWORK_PACKETBUFFER_h */

#define STORAGE_INITIAL_SIZE 4096

namespace SteerStone
{
    /// Buffer class to send/recieve packets
    class PacketBuffer
    {
    public:
        friend class Socket;

    public:
        /// Constructor
        /// @p_InitializeSize : Reserve size for our m_Storage
        explicit PacketBuffer(uint32 p_InitializeSize = STORAGE_INITIAL_SIZE);

        /// Read - Read the packet
        /// @p_Buffer : Buffer which holds the data
        /// @p_Length : The length of the data
        void Read(char* p_Buffer, const std::size_t& p_Length);

        /// Write - Write the data to be sent
        /// @p_Buffer : Buffer which holds the data
        /// @p_Length : The length of the data
        void Write(char const* p_Buffer, std::size_t const& p_Length);

        /// ReadLength - Get the total read length of the packet
        std::size_t const ReadLength();

        /// ReadLengthRemaining - Get the total read length of the packet
        std::size_t const ReadLengthRemaining();

        /// ProcessIncomingData - Get the current read position
        std::size_t const ReadPosition();

    private:
        /// Storage
        std::size_t m_WritePosition;                                                    ///< Write position in our storage
        std::size_t m_ReadPosition;                                                     ///< Read position in our storage
        std::vector<uint8> m_Buffer;                                                    ///< Vector Storage
    };
} ///< NAMESPACE STEERSTONE
