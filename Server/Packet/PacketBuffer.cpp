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

#include "PacketBuffer.h"
//-----------------------------------------------//
void PacketBuffer::Read(char* buffer, const std::size_t& length)
{
    if (buffer)
        memcpy(buffer, &mBuffer[mReadPosition], length);

    mReadPosition += length;
}
//-----------------------------------------------//
void PacketBuffer::Write(const char * p_Buffer, const std::size_t p_Length)
{
    const size_t newLength = mWritePosition + p_Length;

    if (mBuffer.size() < newLength)
        mBuffer.resize(newLength);

    memcpy(&mBuffer[mWritePosition], p_Buffer, p_Length);

    mWritePosition += p_Length;
}
//-----------------------------------------------//
