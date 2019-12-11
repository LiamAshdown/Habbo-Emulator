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

#include "UtiStreamHelpers.hpp"

/// Avoid byte reading as char in stream
/// @p_Stream : Source stream
/// @p_Value  : Target variable
std::istream & operator>> (std::istream & p_Stream, uint8 & p_Value)
{
    uint16 l_Value = 0;

    std::istream& l_ResultStream = p_Stream >> l_Value;
    p_Value = uint8(l_Value);

    return l_ResultStream;
}
/// Avoid byte reading as char in stream
/// @p_Stream : Source stream
/// @p_Value  : Target variable
std::istream & operator>> (std::istream & p_Stream, int8 & p_Value)
{
    int16 l_Value = 0;

    std::istream& l_ResultStream = p_Stream >> l_Value;
    p_Value = int8(l_Value);

    return l_ResultStream;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

/// Avoid byte write as char in stream
/// @p_Stream : Target stream
/// @p_Value  : Source variable
std::ostream & operator<< (std::ostream & p_Stream, uint8 & p_Value)
{
    return p_Stream << uint16(p_Value);
}
/// Avoid byte write as char in stream
/// @p_Stream : Target stream
/// @p_Value  : Source variable
std::ostream & operator<< (std::ostream & p_Stream, int8 & p_Value)
{
    return p_Stream << int16(p_Value);
}
