/*
* This file is part of the CPriston Project. See AUTHORS file for Copyright information
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/
//-----------------------------------------------//
#include "PacketBuffer.h"
//-----------------------------------------------//
namespace Quad
{
    //-----------------------------------------------//
    void PacketBuffer::Read(char* buffer, const std::size_t& length)
    {
        assert(ReadLengthRemaining() >= length);

        if (buffer)
            memcpy(buffer, &mBuffer[mReadPosition], length);

        mReadPosition += length;
    }
    //-----------------------------------------------//
    void PacketBuffer::Write(const char* buffer, const std::size_t& length)
    {
        const size_t newLength = mWritePosition + length;

        if (mBuffer.size() < newLength)
            mBuffer.resize(newLength);

        memcpy(&mBuffer[mWritePosition], buffer, length);

        mWritePosition += length;
    }
    //-----------------------------------------------//
    std::size_t PacketBuffer::ReadLength() const
    {
        return mReadPosition;
    }
    //-----------------------------------------------//
    std::size_t PacketBuffer::ReadPosition() const
    {
        return mBuffer[mReadPosition];
    }
    //-----------------------------------------------//
    std::size_t PacketBuffer::ReadLengthRemaining() const
    {
        return mWritePosition - mReadPosition;
    }
    //-----------------------------------------------//
}