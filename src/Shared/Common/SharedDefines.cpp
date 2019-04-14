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

#include "SharedDefines.h"
#include "Timer.h"

std::string SplitString(std::string const& p_StringData, std::string const& p_Tokenizer)
{
    std::size_t l_StartPosition = p_StringData.find(p_Tokenizer);

    if (p_StringData[l_StartPosition + p_Tokenizer.length()] == '=')
    {
        std::string l_KeyString;
        for (std::size_t i = l_StartPosition + (p_Tokenizer.length() + 1); i < p_StringData.length(); i++)
        {
            if (p_StringData[i] == '\r')
                break;

            l_KeyString += p_StringData[i];
        }

        return l_KeyString;
    }

    return std::string();
}

Tokens StrSplit(std::string const& p_Src, std::string const& p_Seperator)
{
    Tokens l_R;
    std::string l_S;
    for (char l_I : p_Src)
    {
        if (p_Seperator.find(l_I) != std::string::npos)
        {
            if (l_S.length()) 
                l_R.push_back(l_S);

            l_S.clear();
        }
        else
            l_S += l_I;
    }

    if (l_S.length())
        l_R.push_back(l_S);

    return l_R;
}

uint16 ConvertEndian(uint16 value)
{
    return ((value & 0xff) << 8) | ((value & 0xff00) >> 8);
}

std::string EncodeBase64(const uint32 value)
{
    int length = 2;
    std::string stack = "";
    for (int x = 1; x <= length; x++)
    {
        int offset = 6 * (length - x);
        uint8 val = (uint8)(64 + (value >> offset & 0x3f));
        stack += (char)val;
    }
    return stack;
}

int DecodeBase64(const std::string buffer)
{
    int intTot = 0;
    int y = 0;
    for (int x = (buffer.length() - 1); x >= 0; x--)
    {
        int intTmp = (int)(char)((buffer[x] - 64));
        if (y > 0)
        {
            intTmp = intTmp * (int)(pow(64, y));
        }
        intTot += intTmp;
        y++;
    }
    return intTot;
}

std::string EncodeWired(int64 value)
{
    char wf[6];
    int position = 0;
    int bytes = 1;
    int negativeMask = value >= 0 ? 0 : 4;
    value = abs(value);
    wf[position++] = (char)(64 + (value & 3));
    for (value >>= 2; value != 0; value >>= 6)
    {
        bytes++;
        wf[position++] = (char)(64 + (value & 0x3f));
    }

    wf[0] = (char)(wf[0] | bytes << 3 | negativeMask);

    std::string tmp(wf);
    tmp.resize(bytes);

    return tmp;
}

int DecodeWired(std::string buffer)
{
    try
    {
        int pos = 0;
        int v = 0;
        bool negative = (buffer[pos] & 4) == 4;
        int totalBytes = buffer[pos] >> 3 & 7;
        v = buffer[pos] & 3;
        pos++;
        int shiftAmount = 2;
        for (int b = 1; b < totalBytes; b++)
        {
            v |= (buffer[pos] & 0x3f) << shiftAmount;
            shiftAmount = 2 + 6 * b;
            pos++;
        }

        if (negative)
            v *= -1;

        return v;
    }
    catch (const std::exception& e)
    {
        return 0;
    }
}

std::string GetDate()
{
    time_t rawtime;
    struct tm * timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    int day = timeinfo->tm_mday;
    int month = timeinfo->tm_mon + 1;
    int year = timeinfo->tm_year + 1900;

    std::stringstream iss;
    iss << day;
    iss << "-";
    iss << month;
    iss << "-";
    iss << year;

    return iss.str();
}