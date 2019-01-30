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

#ifndef _QuadEmu_ByteBuffer_h_
#define _QuadEmu_ByteBuffer_h_
#include "../../Common/SharedDefines.h"
#endif /* _QuadEmu_ByteBuffer_ */

class ByteBuffer
{
public:

    ByteBuffer(const std::string& value) : mWritePosition(0), mReadPosition(0)
    {
        if (std::size_t l_Length = value.length())
            Append(value.c_str(), l_Length);
    }

    ByteBuffer(const char* packetName, std::size_t packetSize) : mWritePosition(0), mReadPosition(0)
    {
        *this << packetName;
        mStorage.reserve(packetSize);
    }

    ByteBuffer() : mWritePosition(0), mReadPosition(0)
    {
    }

    ~ByteBuffer() {}

public:
    ByteBuffer& operator<<(const std::string& value)
    {
        if (std::size_t l_Length = value.length())
            Append(value.c_str(), l_Length);

        return *this;
    }

    ByteBuffer& operator<<(int8 value)
    {
        std::string toString = std::to_string(value);
        if (std::size_t l_Length = toString.length())
            Append(toString.c_str(), l_Length);
        return *this;
    }

    ByteBuffer& operator<<(int value)
    {
        std::string toString = std::to_string(value);
        if (std::size_t l_Length = toString.length())
            Append(toString.c_str(), l_Length);
        return *this;
    }

    ByteBuffer& operator<<(uint8 value)
    {
        int newValue = (int)value;
        std::string toString = std::to_string(newValue);
        if (std::size_t l_Length = toString.length())
            Append(toString.c_str(), l_Length);
        return *this;
    }

    ByteBuffer& operator<<(uint16 value)
    {
        std::string toString = std::to_string(value);
        if (std::size_t l_Length = toString.length())
            Append(toString.c_str(), l_Length);
        return *this;
    }

    ByteBuffer& operator<<(uint32 value)
    {
        std::string toString = std::to_string(value);
        if (std::size_t l_Length = toString.length())
            Append(toString.c_str(), l_Length);
        return *this;
    }

    void Append(const char* p_Char, std::size_t p_Size)
    {
        return Append((const uint8*)p_Char, p_Size);
    }

    void Append(const uint8* p_Char, std::size_t p_Size)
    {
        mStorage.insert(mStorage.begin() + mWritePosition, p_Char, p_Char + p_Size);
        mWritePosition += p_Size;
    }

    void AppendSpace()
    {
        std::string space = " ";
        if (std::size_t l_Length = space.length())
            Append(space.c_str(), l_Length);
    }

    void AppendComma()
    {
        std::string space = ",";
        if (std::size_t l_Length = space.length())
            Append(space.c_str(), l_Length);
    }

    void AppendTab()
    {
        std::string space = "\t";
        if (std::size_t l_Length = space.length())
            Append(space.c_str(), l_Length);
    }

    void AppendEndCarriage()
    {
        std::string end = "\r##";
        if (std::size_t l_Length = end.length())
            Append(end.c_str(), l_Length);
    }

    void AppendForwardSlash()
    {
        std::string end = "/";
        if (std::size_t l_Length = end.length())
            Append(end.c_str(), l_Length);
    }

    void AppendMove()
    {
        std::string move = "/mv ";
        if (std::size_t l_Length = move.length())
            Append(move.c_str(), l_Length);
    }

    void AppendCarriage()
    {
        std::string end = "\r";
        if (std::size_t l_Length = end.length())
            Append(end.c_str(), l_Length);
    }

    // limited for internal use because can "append" any unexpected type (like pointer and etc) with hard detection problem
    template <typename T> void Append(T value)
    {
        Append((uint8*)&value, sizeof(value));
    }

    uint8 const* GetContents() const
    {
        return &mStorage[0];
    }

    std::size_t const GetSize() const
    {
        return mStorage.size();
    }

    void Clear()
    {
        mWritePosition = 0;
        mReadPosition = 0;
        mStorage.clear();
    }

    std::vector<uint8> GetStorage()
    {
        return mStorage;
    }

protected:
    std::size_t mWritePosition;
    std::size_t mReadPosition;
    std::vector<uint8> mStorage;
};