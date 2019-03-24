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

#include "SHA1.h"

std::string ByteArrayToHexStr(uint8 const* p_Bytes, uint32 p_ArrayLen, bool p_Reverse /* = false */)
{
    int32 init = 0;
    int32 end = p_ArrayLen;
    int8 op = 1;

    if (p_Reverse)
    {
        init = p_Reverse - 1;
        end = -1;
        op = -1;
    }

    std::ostringstream ss;
    for (int32 i = init; i != end; i += op)
    {
        char buffer[4];
        sprintf(buffer, "%02X", p_Bytes[i]);
        ss << buffer;
    }

    return ss.str();
}

std::string CalculateSHA1Hash(std::string const& p_Content)
{
    unsigned char digest[SHA_DIGEST_LENGTH];
    SHA1((unsigned char*)p_Content.c_str(), p_Content.length(), (unsigned char*)&digest);

    return ByteArrayToHexStr(digest, SHA_DIGEST_LENGTH);
}