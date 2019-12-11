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

#pragma once

#include "Core.hpp"
#include <exception>

#if defined (__GLIBC__)
# include <endian.h>
# if (__BYTE_ORDER == __LITTLE_ENDIAN)
#  define LITTLE_ENDIAN
# elif (__BYTE_ORDER == __BIG_ENDIAN)
#  define BIG_ENDIAN
# else
#  error Unknown machine endianness detected.
# endif
# define BYTE_ORDER __BYTE_ORDER
#elif defined(_BIG_ENDIAN) && !defined(_LITTLE_ENDIAN) || \
    defined(__BIG_ENDIAN__) && !defined(__LITTLE_ENDIAN__) || \
    defined(_STLP_BIG_ENDIAN) && !defined(_STLP_LITTLE_ENDIAN)
# define BIG_ENDIAN
#elif defined(_LITTLE_ENDIAN) && !defined(_BIG_ENDIAN) || \
    defined(__LITTLE_ENDIAN__) && !defined(__BIG_ENDIAN__) || \
    defined(_STLP_LITTLE_ENDIAN) && !defined(_STLP_BIG_ENDIAN)
# define LITTLE_ENDIAN
#elif defined(__sparc) || defined(__sparc__) \
   || defined(_POWER) || defined(__powerpc__) \
   || defined(__ppc__) || defined(__hpux) || defined(__hppa) \
   || defined(_MIPSEB) || defined(_POWER) \
   || defined(__s390__)
# define BIG_ENDIAN
#elif defined(__i386__) || defined(__alpha__) \
   || defined(__ia64) || defined(__ia64__) \
   || defined(_M_IX86) || defined(_M_IA64) \
   || defined(_M_ALPHA) || defined(__amd64) \
   || defined(__amd64__) || defined(_M_AMD64) \
   || defined(__x86_64) || defined(__x86_64__) \
   || defined(_M_X64) || defined(__bfin__)
# define LITTLE_ENDIAN
#else
# error Unknown machine endianness detected.
#endif

namespace TheMMOProject { namespace Core { namespace Utils {

    /// Byte converter helper
    namespace ByteConverter
    {
        /// Apply
        /// @p_Value : Target value
        template<typename T> inline void Apply(T & p_Value)
        {
            throw std::exception(__FILE__  " " __LINE__ " Endian convert not defined for type " typeid(T).name);
        }
        /// Apply
        /// @p_Value : Target value
        template<> inline void Apply(uint16 & p_Value)
        {
            p_Value = (p_Value >> 8) |
                      (p_Value << 8);
        }
        /// Apply
        /// @p_Value : Target value
        template<> inline void Apply(uint32 & p_Value)
        {
            p_Value = ( p_Value >> 24)               |
                      ((p_Value <<  8) & 0x00FF0000) |
                      ((p_Value >>  8) & 0x0000FF00) |
                      ( p_Value << 24);
        }
        /// Apply
        /// @p_Value : Target value
        template<> inline void Apply(uint64 & p_Value)
        {
            p_Value = ( p_Value >> 56)                       |
                      ((p_Value << 40) & 0x00FF000000000000) |
                      ((p_Value << 24) & 0x0000FF0000000000) |
                      ((p_Value <<  8) & 0x000000FF00000000) |
                      ((p_Value >>  8) & 0x00000000FF000000) |
                      ((p_Value >> 24) & 0x0000000000FF0000) |
                      ((p_Value >> 40) & 0x000000000000FF00) |
                      ( p_Value << 56);
        }
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    #if defined(BIG_ENDIAN)
        /// Convert value to opposite endian
        /// @p_Value : Value to convert
        template<typename T> inline void EndianConvert(T & p_Value) { ::SteerStone::Core::Utils::ByteConverter::Apply<T>(p_Value); }
        /// Force convert value to opposite endian
        /// @p_Value : Value to convert
        template<typename T> inline void EndianConvertReverse(T & p_Value) { UNUSED(p_Value); }
    #else
        /// Convert value to opposite endian
        /// @p_Value : Value to convert
        template<typename T> inline void EndianConvert(T & p_Value) { UNUSED(p_Value); }
        /// Force convert value to opposite endian
        /// @p_Value : Value to convert
        template<typename T> inline void EndianConvertReverse(T & p_Value) { ::SteerStone::Core::Utils::ByteConverter::Apply<T>(p_Value); }
    #endif

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Convert value to opposite endian
    /// @p_Value : Value to convert
    template<typename T> void EndianConvert(T & p_Value);
    /// Force convert value to opposite endian
    /// @p_Value : Value to convert
    template<typename T> void EndianConvertReverse(T & p_Value);

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Convert value to opposite endian
    /// @p_Value : Value to convert
    inline void EndianConvert(uint8 & p_Value) { UNUSED(p_Value); }
    /// Convert value to opposite endian
    /// @p_Value : Value to convert
    inline void EndianConvert( int8 & p_Value) { UNUSED(p_Value); }
    /// Force convert value to opposite endian
    /// @p_Value : Value to convert
    inline void EndianConvertReverse(uint8 & p_Value) { UNUSED(p_Value); }
    /// Force convert value to opposite endian
    /// @p_Value : Value to convert
    inline void EndianConvertReverse( int8 & p_Value) { UNUSED(p_Value); }

}   ///< namespace Utils
}   ///< namespace Core
}   ///< namespace SteerStone
