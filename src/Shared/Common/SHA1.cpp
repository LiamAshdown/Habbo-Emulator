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
//-----------------------------------------------//
#include "SHA1.h"
//-----------------------------------------------//
std::string ByteArrayToHexStr(const uint8* bytes, uint32 arrayLen, bool reverse /* = false */)
{
    int32 init = 0;
    int32 end = arrayLen;
    int8 op = 1;

    if (reverse)
    {
        init = arrayLen - 1;
        end = -1;
        op = -1;
    }

    std::ostringstream ss;
    for (int32 i = init; i != end; i += op)
    {
        char buffer[4];
        sprintf(buffer, "%02X", bytes[i]);
        ss << buffer;
    }

    return ss.str();
}
//-----------------------------------------------//
std::string CalculateSHA1Hash(const std::string& content)
{
    unsigned char digest[SHA_DIGEST_LENGTH];
    SHA1((unsigned char*)content.c_str(), content.length(), (unsigned char*)&digest);

    return ByteArrayToHexStr(digest, SHA_DIGEST_LENGTH);
}
//-----------------------------------------------//