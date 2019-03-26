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

#pragma once
#include "Common/SharedDefines.h"
#include "StringBuffer.h"

namespace SteerStone
{
    /// ServerPacket which handles encoding packets to be sent to the client
    class ServerPacket
    {
    public:
        /// Constructor
        ServerPacket(uint32 const&& p_HeaderId) 
        {
            m_Buffer.AppendBase64(p_HeaderId);
        }
        /// Deconstructor
        ~ServerPacket() {}

    public:
        /// GetBuffer - Return our buffer to insert data outside Server Packet class
        StringBuffer& GetBuffer()          { return m_Buffer;         }
        StringBuffer& GetSecondaryBuffer() {return m_SecondaryBuffer; }

    protected:
        StringBuffer m_Buffer;                  ///< String buffer which holds our data to be sent to the client
        StringBuffer m_SecondaryBuffer;         ///< Use Secondary buffer if we are pushing data into buffer outside the class packet
                                                ///< And want to append the secondary buffer after the first buffer
    };
} ///< NAMESPACE STEERSTONE