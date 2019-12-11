/*
* Liam Ashdown
* HardCPP (Merydwin)
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

#include <Precompiled.hpp>

#include "UtiString.hpp"

namespace SteerStone { namespace Core { namespace Utils {

    /// Convert an UTF16 string to an UTF8 string
    /// @p_Input : String to convert
    std::string String::Utf16ToUtf8(const std::wstring & p_Input)
    {
        std::string l_Result;
        l_Result.reserve(p_Input.size() / 2);

        const wchar_t* l_Ptr = p_Input.c_str();
        unsigned int l_CodePoint = 0;

        for (; *l_Ptr != 0; ++l_Ptr)
        {
            if (*l_Ptr >= 0xD800 && *l_Ptr <= 0xDBFF)
                l_CodePoint = ((*l_Ptr - 0xD800) << 10) + 0x10000;
            else
            {
                if (*l_Ptr >= 0xDC00 && *l_Ptr <= 0xDFFF)
                    l_CodePoint |= *l_Ptr - 0xDC00;
                else
                    l_CodePoint = *l_Ptr;

                if (l_CodePoint <= 0x7F)
                    l_Result.append(1, static_cast<char>(l_CodePoint));
                else if (l_CodePoint <= 0x7FF)
                {
                    l_Result.append(1, static_cast<char>(0xC0 | ((l_CodePoint >> 6) & 0x1F)));
                    l_Result.append(1, static_cast<char>(0x80 | (l_CodePoint & 0x3F)));
                }
                else if (l_CodePoint <= 0xFFFF)
                {
                    l_Result.append(1, static_cast<char>(0xE0 | ((l_CodePoint >> 12) & 0x0F)));
                    l_Result.append(1, static_cast<char>(0x80 | ((l_CodePoint >> 6) & 0x3F)));
                    l_Result.append(1, static_cast<char>(0x80 | (l_CodePoint & 0x3F)));
                }
                else
                {
                    l_Result.append(1, static_cast<char>(0xF0 | ((l_CodePoint >> 18) & 0x07)));
                    l_Result.append(1, static_cast<char>(0x80 | ((l_CodePoint >> 12) & 0x3F)));
                    l_Result.append(1, static_cast<char>(0x80 | ((l_CodePoint >> 6) & 0x3F)));
                    l_Result.append(1, static_cast<char>(0x80 | (l_CodePoint & 0x3F)));
                }

                l_CodePoint = 0;
            }
        }

        return l_Result;
    }
    /// Convert an UTF8 string to an UTF16 string
    /// @p_Input : String to convert
    std::wstring String::Utf8ToUtf16(const std::string & p_Input)
    {
        std::wstring l_Result;
        l_Result.reserve(p_Input.size() * 3);

        const char* l_Ptr = p_Input.c_str();
        unsigned int l_CodePoint = 0;

        int l_Following = 0;

        for (; *l_Ptr != 0; ++l_Ptr)
        {
            unsigned char l_Character = *l_Ptr;
            if (l_Character <= 0x7F)
            {
                l_CodePoint = l_Character;
                l_Following = 0;
            }
            else if (l_Character <= 0xBF)
            {
                if (l_Following > 0)
                {
                    l_CodePoint = (l_CodePoint << 6) | (l_Character & 0x3F);
                    --l_Following;
                }
            }
            else if (l_Character <= 0xDF)
            {
                l_CodePoint = l_Character & 0x1F;
                l_Following = 1;
            }
            else if (l_Character <= 0xEF)
            {
                l_CodePoint = l_Character & 0x0F;
                l_Following = 2;
            }
            else
            {
                l_CodePoint = l_Character & 0x07;
                l_Following = 3;
            }
            if (l_Following == 0)
            {
                if (l_CodePoint > 0xFFFF)
                {
                    l_Result.append(1, static_cast<wchar_t>(0xD800 + (l_CodePoint >> 10)));
                    l_Result.append(1, static_cast<wchar_t>(0xDC00 + (l_CodePoint & 0x03FF)));
                }
                else
                    l_Result.append(1, static_cast<wchar_t>(l_CodePoint));

                l_CodePoint = 0;
            }
        }

        return l_Result;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    std::string String::RawToHexString(const uint8 * p_Data, const std::size_t & p_Size)
    {
        static const char sl_Symbols[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

        std::string l_Output;
        l_Output.reserve(2 * p_Size);

        for (std::size_t l_I = 0; l_I < p_Size; ++l_I)
        {
            const unsigned char l_Character = p_Data[l_I];
            l_Output.push_back(sl_Symbols[(l_Character & 0xF0) >> 4]);
            l_Output.push_back(sl_Symbols[(l_Character & 0x0F) >> 0]);
        }

        return l_Output;
    }

    std::vector<uint8> String::HexStringToRaw(const std::string & p_Input)
    {
        std::vector<uint8> l_Result;
        l_Result.reserve(p_Input.length() / 2);

        for (std::size_t l_I = 0; l_I < p_Input.length(); l_I += 2)
        {
            std::string l_Byte = p_Input.substr(l_I, 2);
            uint8 l_Value = (uint8)(int)strtol(l_Byte.c_str(), nullptr, 16);
            l_Result.push_back(l_Value);
        }

        return l_Result;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    std::string String::StringToHexString(const std::string & p_Input)
    {
        return RawToHexString(reinterpret_cast<const uint8*>(p_Input.c_str()), p_Input.size());
    }

}   ///< namespace Utils
}   ///< namespace Core
}   ///< namespace SteerStone
