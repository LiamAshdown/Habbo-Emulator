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

#pragma once

#include "Core.hpp"

#include <istream>
#include <ostream>

/// Avoid byte reading as char in stream
/// @p_Stream : Source stream
/// @p_Value  : Target variable
std::istream & operator>> (std::istream & p_Stream, uint8 & p_Value);
/// Avoid byte reading as char in stream
/// @p_Stream : Source stream
/// @p_Value  : Target variable
std::istream & operator>> (std::istream & p_Stream, int8  & p_Value);

/// Avoid byte write as char in stream
/// @p_Stream : Target stream
/// @p_Value  : Source variable
std::ostream & operator<< (std::ostream & p_Stream, uint8 & p_Value);
/// Avoid byte write as char in stream
/// @p_Stream : Target stream
/// @p_Value  : Source variable
std::ostream & operator<< (std::ostream & p_Stream, int8  & p_Value);
