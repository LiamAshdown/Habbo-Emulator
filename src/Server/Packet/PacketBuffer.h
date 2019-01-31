/*
* Liam Ashdown
* Copyright (C) 2018
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

#ifndef _QuadEmu_PacketBuffer_h_
#define _QuadEmu_PacketBuffer_h_
#include "../../Common/SharedDefines.h"
#endif /* _QuadEmu_PacketBuffer_ */

class PacketBuffer
{
public:
    friend class Socket;

public:
    PacketBuffer(int initialSize = 4096) : mWritePosition(0), mReadPosition(0), mBuffer(initialSize, 0) {}

    void Read(char* buffer, const std::size_t& length);
    uint8 ReadPosition() const { return mBuffer[mReadPosition]; }
    void Write(const char *p_Buffer, const std::size_t p_Length);
    std::size_t ReadLengthRemaining() const { return mWritePosition - mReadPosition; }


    std::size_t mWritePosition;
    std::size_t mReadPosition;
    std::vector<uint8> mBuffer;
};