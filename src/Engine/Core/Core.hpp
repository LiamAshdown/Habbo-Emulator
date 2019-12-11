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
#include <string>
#include <cstdlib>

/// Integral types
#ifdef _MSC_VER
    typedef __int8 int8;
    typedef unsigned __int8 uint8;
    typedef __int16 int16;
    typedef unsigned __int16 uint16;
    typedef __int32 int32;
    typedef unsigned __int32 uint32;
    typedef __int64 int64;
    typedef unsigned __int64 uint64;
#else
    #include <stdint.h>
    #include <inttypes.h>
    typedef int8_t int8;
    typedef uint8_t uint8;
    typedef int16_t int16;
    typedef uint16_t uint16;
    typedef int32_t int32;
    typedef uint32_t uint32;
    typedef int64_t int64;
    typedef uint64_t uint64;
#endif

/// Disable warnings
#ifdef _MSC_VER
    #pragma warning(disable : 4172)
    #pragma warning(disable : 4251)
    #pragma warning(disable : 4049)
    #pragma warning(disable : 4217)
    #pragma warning(disable : 4348)
    #define NOMINMAX 1
#endif

/// Useful Macros
#define DISALLOW_COPY(TypeName) \
    TypeName(const TypeName &); \
    TypeName(const TypeName &&)

#define DISALLOW_ASSIGN(TypeName) \
    void operator=(const TypeName &)

#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
    DISALLOW_COPY(TypeName);               \
    DISALLOW_ASSIGN(TypeName)

#define DEFAULT_CONSTRUCTOR(TypeName) \
    TypeName() {} \
    virtual ~TypeName() {}

#define UNUSED_ARGUMENT(Param) \
    (void)Param;
#define UNUSED(x) ((void)x)

#define STR(x) #x
#define XSTR(x) STR(x)