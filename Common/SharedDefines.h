/*
* Liam Ashdown
* Copyright (C) 2018
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

#ifndef _QuadEmu_Common_h_
#define _QuadEmu_Common_h_
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
#include "Timer.h"
#include "Logging/Logger.h"
#endif /* _QuadEmu_Common_ */

#define ListenerPort 37120
#define ListenerIp 127.0.0.1

typedef std::int64_t int64;
typedef std::int32_t int32;
typedef std::int16_t int16;
typedef std::int8_t int8;
typedef std::uint64_t uint64;
typedef std::uint32_t uint32;
typedef std::uint16_t uint16;
typedef std::uint8_t uint8;
