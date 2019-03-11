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

#ifndef _Quad_StringBuffer_h_
#define _Quad_StringBuffer_h_
#include "Common/SharedDefines.h"
namespace Quad
{

    class TempBuffer
    {
    public:
        TempBuffer() : mWritePosition(0), mReadPosition(0)
        {
            mStorage.reserve(4096);
        }
        ~TempBuffer() {}

    public:

        void AppendString(const std::string buffer)
        {
            if (std::size_t length = buffer.length())
                Append((uint8 const*)buffer.c_str(), length);
        }

        void AppendUint8(const uint8 value)
        {
            Append<uint8>(value);
        }

        void AppendInt8(const int8 value)
        {
            Append<int8>(value);
        }

        void AppendUint16(const uint16 value)
        {
            Append<uint16>(value);
        }

        void AppendInt16(const int16 value)
        {
            Append<int16>(value);
        }

        void AppendUint32(const uint32 value)
        {
            Append<uint32>(value);
        }

        void AppendInt32(const int32 value)
        {
            Append<uint8>(value);
        }

        void AppendUint64(const uint64 value)
        {
            Append<uint64>(value);
        }

        void AppendInt64(const int64 value)
        {
            Append<int64>(value);
        }

        void AppendBase64(const uint32 value)
        {
            std::string encoder = EncodeBase64(value);

            if (std::size_t length = encoder.length())
                Append((uint8 const*)encoder.c_str(), length);
        }

        void AppendWired(const int64 value)
        {
            std::string wired = EncodeWired(value);

            if (std::size_t length = wired.length())
                Append((uint8 const*)wired.c_str(), length);
        }

        void AppendSOH()
        {
            std::string SOH = "\u0001";
            if (std::size_t length = SOH.length())
                Append(SOH.c_str(), length);
        }

        template <typename T> void Append(T value)
        {
            Append((uint8*)&value, sizeof(value));
        }

        template<class T> void Append(const T* src, std::size_t cnt)
        {
            return Append((const uint8*)src, cnt * sizeof(T));
        }

        void Append(const char* buffer, const std::size_t& size)
        {
            return Append((const uint8*)buffer, size);
        }

        void Append(const uint8* buffer, const std::size_t& size)
        {
            if (!size)
                return;
            
            if (mStorage.size() < mWritePosition + size)
                mStorage.resize(mWritePosition + size);

            memcpy(&mStorage[mWritePosition], buffer, size);
            mWritePosition += size;
        }

        std::size_t GetSize() const
        {
            return mStorage.size();
        }

        uint8 const* GetContents() const
        {
            return &mStorage[0];
        }

        void Clear()
        {
            mWritePosition = 0;
            mReadPosition = 0;
            mStorage.clear();
        }

    private:
        std::size_t mWritePosition;
        std::size_t mReadPosition;
        std::vector<uint8> mStorage;
    };

    class StringBuffer
    {
    public:

        StringBuffer(const std::string& value) : mWritePosition(0), mReadPosition(0)
        {
            if (std::size_t l_Length = value.length())
                Append(value.c_str(), l_Length);
        }

        StringBuffer(const char* packetName, std::size_t packetSize) : mWritePosition(0), mReadPosition(0)
        {
            *this << packetName;
            mStorage.reserve(packetSize);
        }

        StringBuffer() : mWritePosition(0), mReadPosition(0)
        {
        }

        ~StringBuffer() {}

    public:
        StringBuffer& operator<<(const std::string& value)
        {
            if (std::size_t l_Length = value.length())
                Append(value.c_str(), l_Length);

            return *this;
        }

        StringBuffer& operator<<(int8 value)
        {
            std::string toString = std::to_string(value);
            if (std::size_t l_Length = toString.length())
                Append(toString.c_str(), l_Length);
            return *this;
        }

        StringBuffer& operator<<(int value)
        {
            std::string toString = std::to_string(value);
            if (std::size_t l_Length = toString.length())
                Append(toString.c_str(), l_Length);
            return *this;
        }

        StringBuffer& operator<<(uint8 value)
        {
            int newValue = (int)value;
            std::string toString = std::to_string(newValue);
            if (std::size_t l_Length = toString.length())
                Append(toString.c_str(), l_Length);
            return *this;
        }

        StringBuffer& operator<<(uint16 value)
        {
            std::string toString = std::to_string(value);
            if (std::size_t l_Length = toString.length())
                Append(toString.c_str(), l_Length);
            return *this;
        }

        StringBuffer& operator<<(uint32 value)
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
#pragma endregion
}

#endif /* _Quad_StringBuffer_ */