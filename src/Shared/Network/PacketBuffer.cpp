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

#include "PacketBuffer.h"

namespace SteerStone
{
    /// Constructor
   /// @p_InitializeSize : Reserve size for our m_Storage
    PacketBuffer::PacketBuffer(uint32 p_InitializeSize) : m_WritePosition(0), m_ReadPosition(0), m_Buffer(p_InitializeSize, 0)
    {
    }

    /// Read - Read the packet
    /// @p_Buffer : Buffer which holds the data
    /// @p_Length : The length of the data
    void PacketBuffer::Read(char* p_Buffer, std::size_t const& p_Length)
    {
        assert(ReadLengthRemaining() >= p_Length);

        if (p_Buffer)
            memcpy(p_Buffer, &m_Buffer[m_ReadPosition], p_Length);

        m_ReadPosition += p_Length;
    }

    /// Write - Write the data to be sent
    /// @p_Buffer : Buffer which holds the data
    /// @p_Length : The length of the data
    void PacketBuffer::Write(char const* p_Buffer, std::size_t const& p_Length)
    {
        const size_t l_NewLength = m_WritePosition + p_Length;

        if (m_Buffer.size() < l_NewLength)
            m_Buffer.resize(l_NewLength);

        memcpy(&m_Buffer[m_WritePosition], p_Buffer, p_Length);

        m_WritePosition += p_Length;
    }

    /// ReadLength - Get the total read length of the packet
    std::size_t const PacketBuffer::ReadLength()
    {
        return m_ReadPosition;
    }

    /// ProcessIncomingData - Get the current read position
    std::size_t const PacketBuffer::ReadPosition()
    {
        return m_Buffer[m_ReadPosition];
    }

    /// ReadLengthRemaining - Get the total read length of the packet
    std::size_t const PacketBuffer::ReadLengthRemaining()
    {
        return m_WritePosition - m_ReadPosition;
    }
} ///< NAMESPACE STEERSTONE