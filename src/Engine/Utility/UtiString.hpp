#pragma once
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

#include "Core/Core.hpp"

#include <string>
#include <vector>
#include <tuple>
#include <sstream>

namespace SteerStone { namespace Core { namespace Utils {

    /// String converter
    template<class T> class Converter
    {
        public:
            /// Convert anything to string
            /// @p_Value : Value to convert
            template<class D> static T ToString(const D & p_Value);
            /// Convert anything to string
            /// @p_Value : Value to convert
            static T ToString(const uint8 & p_Value);
            /// Convert anything to string
            /// @p_Value : Value to convert
            static T ToString(const int8 & p_Value);
            /// Convert anything to string
            /// @p_Value : Value to convert
            static T ToString(const char* p_Value);
            /// Convert anything to string
            /// @p_Value : Value to convert
            static T ToString(const wchar_t* p_Value);

            /// Convert string to anything else
            /// @p_Str : Source string
            static T FromString(const std::string & p_Str);
            /// Convert string to anything else
            /// @p_Str : Source string
            static T FromString(const std::wstring & p_Str);

    };

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// String utilities
    class String
    {
        public:
            /// Convert an UTF16 string to an UTF8 string
            /// @p_Input : String to convert
            static std::string Utf16ToUtf8(const std::wstring & p_Input);
            /// Convert an UTF8 string to an UTF16 string
            /// @p_Input : String to convert
            static std::wstring Utf8ToUtf16(const std::string & p_Input);

            /// Replace all occurrence of p_From by p_To into p_Str
            /// @p_Str  : Subject
            /// @p_From : Element to replace
            /// @p_To   : Replacement
            template<class T, class S, class R> static T ReplaceAll(T p_Str, const S & p_From, const R & p_To);
            /// Replace all occurrence of p_From[i] by p_To[i] into p_Str
            /// @p_Str  : Subject
            /// @p_From : Elements to replace
            /// @p_To   : Replacements
            template<class T> static T ReplaceAll(T p_Str, const std::vector<T> & p_From, const std::vector<T> & p_To);

            static std::string RawToHexString(const uint8 * p_Data, const std::size_t & p_Size);
            static std::vector<uint8> HexStringToRaw(const std::string & p_Input);

            static std::string StringToHexString(const std::string & p_Input);

    };

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Build string builder token
    /// @p_Index : Token index
    template<class T> inline T StringBuilderToken(int p_Index);
    /// Build string builder token
    /// @p_Index : Token index
    template<> inline std::wstring StringBuilderToken(int p_Index);

    /// Unpack tuple for StringBuilder
    template<int N, int NN> struct _SBU
    {
        template<class T, class... TupleArgs> static void UnpackTuple(std::vector<T>& p_Vector, const std::tuple<TupleArgs...>& p_Tuple)
        {
            p_Vector.push_back(Converter<T>::ToString(std::get<NN - N>(p_Tuple)));
            _SBU<N - 1, NN>::UnpackTuple(p_Vector, p_Tuple);
        }
    };
    template<int NN> struct _SBU<0, NN>
    {
        template<class T, typename... TupleArgs> static void UnpackTuple(std::vector<T>& p_Vector, const std::tuple<TupleArgs...>& p_Tuple) { }
    };
    /// Build a string replacing all %x by corresponding argument
    /// @p_Str  : Subject
    /// @p_Args : Arguments
    template<class T, typename... Args> T StringBuilder(const T& p_Str, Args ...p_Args)
    {
        auto l_ArgsTuple = std::make_tuple(p_Args...);

        std::vector<T> l_Args;
        std::vector<T> l_Tokens;

        auto l_ArgsCount = std::tuple_size<decltype(l_ArgsTuple)>::value;

        l_Args.reserve(l_ArgsCount);
        l_Tokens.resize(l_ArgsCount);

        _SBU<sizeof...(Args), sizeof...(Args)>::template UnpackTuple<std::string, Args...>(l_Args, l_ArgsTuple);

        for (std::size_t l_I = 0; l_I < l_ArgsCount; ++l_I)
            l_Tokens[l_I] = StringBuilderToken<T>(l_I);

        return String::ReplaceAll(p_Str, l_Tokens, l_Args);
    }
    /// Build a string replacing all %x by corresponding argument
    /// @p_Str  : Subject
    /// @p_Args : Arguments
    template<typename... Args> std::string StringBuilder(const char* p_Str, Args ...p_Args)
    {
        return StringBuilder(std::string(p_Str), p_Args...);
    }
    /// Build a string replacing all %x by corresponding argument
    /// @p_Str  : Subject
    /// @p_Args : Arguments
    template<typename... Args> std::wstring StringBuilder(const wchar_t* p_Str, Args ...p_Args)
    {
        return StringBuilder(std::wstring(p_Str), p_Args...);
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Convert anything to string
    /// @p_Value : Value to convert
    template<class T> template<class D> T Converter<T>::ToString(const D & p_Value)
    {
        std::basic_ostringstream<typename T::value_type, typename T::traits_type> l_Stream;
        l_Stream << p_Value;

        return l_Stream.str();
    }
    /// Convert anything to string
    /// @p_Value : Value to convert
    template<> template<> inline std::string Converter<std::string>::ToString(const std::string & p_Value)
    {
        return p_Value;
    }
    /// Convert anything to string
    /// @p_Value : Value to convert
    template<> inline std::string Converter<std::string>::ToString(const char* p_Value)
    {
        return std::string(p_Value);
    }
    /// Convert anything to string
    /// @p_Value : Value to convert
    template<> inline std::string Converter<std::string>::ToString(const wchar_t* p_Value)
    {
        return String::Utf16ToUtf8(std::wstring(p_Value));
    }
    /// Convert anything to string
    /// @p_Value : Value to convert
    template<> template<> inline std::wstring Converter<std::wstring>::ToString(const std::wstring & p_Value)
    {
        return p_Value;
    }
    /// Convert anything to string
    /// @p_Value : Value to convert
    template<> inline std::wstring Converter<std::wstring>::ToString(const char* p_Value)
    {
        return String::Utf8ToUtf16(std::string(p_Value));
    }
    /// Convert anything to string
    /// @p_Value : Value to convert
    template<> inline std::wstring Converter<std::wstring>::ToString(const wchar_t* p_Value)
    {
        return std::wstring(p_Value);
    }
    /// Convert anything to string
    /// @p_Value : Value to convert
    template<class T> inline T Converter<T>::ToString(const uint8 & p_Value)
    {
        uint16 l_Value = p_Value;
        return Converter<T>::ToString(l_Value);
    }
    /// Convert anything to string
    /// @p_Value : Value to convert
    template<class T> inline T Converter<T>::ToString(const int8 & p_Value)
    {
        int16 l_Value = p_Value;
        return Converter<T>::ToString(l_Value);
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Convert string to anything else
    /// @p_Str : Source string
    template<class T> T Converter<T>::FromString(const std::string & p_Str)
    {
        std::istringstream l_Stream(p_Str);
        T l_Value = T();

        l_Stream >> l_Value;

        return l_Value;
    }
    /// Convert string to anything else
    /// @p_Str : Source string
    template<> inline uint8 Converter<uint8>::FromString(const std::string & p_Str)
    {
        return static_cast<uint8>(Converter<uint16>::FromString(p_Str));
    }
    /// Convert string to anything else
    /// @p_Str : Source string
    template<> inline int8 Converter<int8>::FromString(const std::string & p_Str)
    {
        return static_cast<int8>(Converter<int16>::FromString(p_Str));
    }
    /// Convert string to anything else
    /// @p_Str : Source string
    template<class T> T Converter<T>::FromString(const std::wstring & p_Str)
    {
        std::wistringstream l_Stream(p_Str);
        T l_Value = T();

        l_Stream >> l_Value;

        return l_Value;
    }
    /// Convert string to anything else
    /// @p_Str : Source string
    template<> inline uint8 Converter<uint8>::FromString(const std::wstring & p_Str)
    {
        return static_cast<uint8>(Converter<uint16>::FromString(p_Str));
    }
    /// Convert string to anything else
    /// @p_Str : Source string
    template<> inline int8 Converter<int8>::FromString(const std::wstring & p_Str)
    {
        return static_cast<int8>(Converter<int16>::FromString(p_Str));
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Split a sentence by a delimiter
    /// @p_Str              : Sentence
    /// @p_Delimiter        : Split artifact
    /// @p_KeepEmptyElement : Should keep empty element
    static std::vector<std::string> SplitAll(const std::string& p_Str, const std::string p_Delimiter, const bool p_KeepEmptyElement)
    {
        std::vector<std::string> l_Result;

        std::string::const_iterator l_SubStart = p_Str.begin(), l_SubEnd;

        while (true)
        {
            l_SubEnd = std::search(l_SubStart, p_Str.end(), p_Delimiter.begin(), p_Delimiter.end());
            std::string l_Temp(l_SubStart, l_SubEnd);

            if (p_KeepEmptyElement || !l_Temp.empty())
                l_Result.push_back(l_Temp);

            if (l_SubEnd == p_Str.end())
                break;

            l_SubStart = l_SubEnd + p_Delimiter.size();
        }

        return l_Result;
    }
    /// Replace all occurrence of p_From by p_To into p_Str
    /// @p_Str  : Subject
    /// @p_From : Element to replace
    /// @p_To   : Replacement
    template<class T, class S, class R> T String::ReplaceAll(T p_Str, const S & p_From, const R & p_To)
    {
        std::size_t l_StartPos = 0;

        T l_From    = Converter<T>::ToString(p_From);
        T l_To      = Converter<T>::ToString(p_To);

        while ((l_StartPos = p_Str.find(l_From, l_StartPos)) != T::npos)
        {
            p_Str.replace(l_StartPos, l_From.length(), l_To);
            l_StartPos += l_To.length();
        }

        return p_Str;
    }
    /// Replace all occurrence of p_From[i] by p_To[i] into p_Str
    /// @p_Str  : Subject
    /// @p_From : Elements to replace
    /// @p_To   : Replacements
    template<class T> T String::ReplaceAll(T p_Str, const std::vector<T> & p_From, const std::vector<T> & p_To)
    {
        for (std::size_t l_I = 0; l_I < p_From.size(); ++l_I)
        {
            std::size_t l_StartPos = 0;

            while ((l_StartPos = p_Str.find(p_From[l_I], l_StartPos)) != T::npos)
            {
                p_Str.replace(l_StartPos, p_From[l_I].length(), p_To[l_I]);
                l_StartPos += p_To[l_I].length();
            }
        }

        return p_Str;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Build string builder token
    /// @p_Index : Token index
    template<class T> T StringBuilderToken(int p_Index)
    {
        return std::string("%") + Converter<T>::ToString(p_Index);
    }
    /// Build string builder token
    /// @p_Index : Token index
    template<> inline std::wstring StringBuilderToken(int p_Index)
    {
        return std::wstring(L"%") + Converter<std::wstring>::ToString(p_Index);
    }

}   ///< namespace Utils
}   ///< namespace Core
}   ///< namespace SteerStone