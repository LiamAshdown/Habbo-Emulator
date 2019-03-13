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
    class StringBuffer
    {
    public:
        explicit StringBuffer(std::size_t reverseSize = 4096) : mWritePosition(0), mReadPosition(0)
        {
            mStorage.reserve(reverseSize);
        }
        ~StringBuffer() {}

    public:
        void AppendWiredBool(bool value)
        {
            if (value)
                AppendWired(1);
            else
                AppendWired(0);
        }

        void AppendString(const std::string buffer)
        {
            if (std::size_t length = buffer.length())
                Append((uint8 const*)buffer.c_str(), length);
            AppendSOT();
        }

        void AppendString(const std::string buffer, const std::string delimeter)
        {
            if (std::size_t length = buffer.length())
                Append((uint8 const*)buffer.c_str(), length);
            AppendString(delimeter);
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

        void AppendSOT()
        {
            std::string SOH = "\u0002";
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

            assert(GetSize() < 10000000);

            if (mStorage.size() < mWritePosition + size)
                mStorage.resize(mWritePosition + size);

            memcpy(&mStorage[mWritePosition], buffer, size);
            mWritePosition += size;
        }

        void Append(const StringBuffer& buffer)
        {
            if (buffer.GetWritePosition())
                Append(buffer.GetContents(), buffer.GetWritePosition());
        }

        void Resize(const std::size_t newSize)
        {
            mStorage.reserve(newSize);
            mReadPosition = 0;
            mWritePosition = 0;
        }

        void Reserve(const std::size_t resize)
        {
            if (resize > GetSize())
                mStorage.reserve(resize);
        }

        std::size_t GetWritePosition() const
        {
            return mWritePosition;
        }

        std::size_t GetReadPositino()
        {
            return mReadPosition;
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
}

#endif /* _Quad_StringBuffer_ */