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

#ifndef _Quad_SHA1_h_
#define _Quad_SHA1_h_
#include "SharedDefines.h"
#include "openssl/sha.h"
#endif /* _Quad_SHA1_h_ */

std::string ByteArrayToHexStr(const uint8* bytes, uint32 arrayLen, bool reverse = false);
std::string CalculateSHA1Hash(const std::string& content);
