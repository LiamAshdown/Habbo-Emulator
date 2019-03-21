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

#ifndef _Quad_SharedDefines_h_
#define _Quad_SharedDefines_h_
#include "boost/date_time/posix_time/posix_time.hpp"
#include <boost/optional.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <unordered_map>
#include <functional>
#include <algorithm>
#include <iostream>
#include <ctype.h>
#include <memory>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <numeric>
#include <unordered_set>
#include <unordered_map>
#include <stdlib.h>
#include "../Platform/CompilerDefs.h"   
#include <plog/Log.h>
#include <plog/Appenders/ColorConsoleAppender.h>
#endif /* _Quad_SharedDefines_h_ */

#if PLATFORM == PLATFORM_WINDOWS
#  define WIN32_LEAN_AND_MEAN
#  include <Windows.h>
#  ifndef _WIN32_WINNT
#    define _WIN32_WINNT 0x0603
#  endif
#  endif

typedef std::int64_t int64;
typedef std::int32_t int32;
typedef std::int16_t int16;
typedef std::int8_t int8;
typedef std::uint64_t uint64;
typedef std::uint32_t uint32;
typedef std::uint16_t uint16;
typedef std::uint8_t uint8;

typedef std::vector<std::string> Tokens;

#define DEFAULT_SERVER_PORT 37120
#define UPDATE_WORLD_TIMER 50
#define ROOM_ID_OFFSET 1000

std::string SplitString(const std::string& stringData, const std::string& tokenizer);
uint16 ConvertEndian(uint16 value);
int DecodeBase64(const std::string buffer);
int DecodeWired(std::string buffer);
std::string EncodeBase64(const uint32 value);
std::string EncodeWired(int64 value);
std::string GetDate();