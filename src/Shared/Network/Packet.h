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

#ifndef _Quad_Packet_h_
#define _Quad_Packet_h_
#include "Common/SharedDefines.h"
#endif /* _Quad_Packet_h_ */

enum ApproveNameError
{
    NAME_VALID                      = 0,
    NAME_TOO_LONG                   = 1,
    NAME_UNACCEPTABLE_TO_STAFF      = 2,
    NAME_UNACCEPTABLE_TO_STAFF_2    = 3,
    NAME_TAKEN                      = 4,
};

enum ApprovePasswordError
{
    PASSWORD_VALID                  = 0,
    PASSWORD_TOO_SHORT              = 1,
    PASSWORD_TOO_LONG               = 2,
    PASSWORD_INVALID_CHARS          = 3,
    PASSWORD_REQUIRES_NUMBERS       = 4,
    PASSWORD_USER_NAME_SIMILIAR     = 5,
};

class Packet
{
public:
    Packet() : mReadPosition(0), mHeader(0), mContent("") {}

    ~Packet() {}

public:
    void Parse(std::string& buffer)
    {
        mHeader = DecodeBase64(buffer.substr(0, 2));
        mContent = buffer.substr(2);
    }

    std::string ReadString()
    {
        std::size_t length = DecodeBase64(mContent.substr(mReadPosition, 2));
        std::string temp = mContent.substr(2 + mReadPosition, length);
        mReadPosition += (length + 2);

        return temp;
    }

    int32 ReadBase64Int()
    {
        int32 i = DecodeBase64(mContent.substr(mReadPosition, 2));
        mReadPosition += 2;

        return i;
    }

    int32 ReadWiredInt()
    {
        int32 i = DecodeWired(mContent.substr(mReadPosition));
        mReadPosition += EncodeWired(i).length();

        return i;
    }

    uint32 ReadBase64Uint()
    {
        uint32 i = DecodeBase64(mContent.substr(mReadPosition, 2));
        mReadPosition += 2;

        return i;
    }

    uint32 ReadWiredUint()
    {
        uint32 i = DecodeWired(mContent.substr(mReadPosition));
        mReadPosition += EncodeWired(i).length();

        return i;
    }

    bool ReadWiredBool()
    {
        int64 i = DecodeWired(mContent.substr(mReadPosition));
        mReadPosition += EncodeWired(i).length();

        if (i == 1)
            return true;
        else
            return false;
    }

    bool ReadBase64Bool()
    {
        int64 i = DecodeBase64(mContent.substr(mReadPosition++, 1));

        if (i == 1)
            return true;
        else
            return false;
    }

    void ReadSkip(uint8 value)
    {
        for (uint8 i = 0; i < value; i++)
        {
            DecodeBase64(mContent.substr(mReadPosition, 2));
            mReadPosition += 2;
        }
    }

    std::size_t GetSize() const
    {
        return mReadPosition;
    }

    uint32 GetHeader() const
    {
        return mHeader;
    }

private:
    std::size_t mReadPosition;
    std::string mContent;
    uint32 mHeader;
};