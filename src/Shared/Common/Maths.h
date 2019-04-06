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

#ifndef _COMMON_MATHS_h
#define _COMMON_MATHS_h
#include "SharedDefines.h"
#include <random>
#include <ctime>
#endif /* _COMMON_MATHS_h */

namespace SteerStone
{
    namespace Maths
    {
        /// GetRandomNumber
        /// Return a random number in specified range
        uint32 GetRandomUint32(uint32 const p_Min, uint32 const p_Max);

        /// CalculateWalkDirection
        /// @p_CurrentX - Current X axis
        /// @p_CurrentY - Current Y axis
        /// @p_NextX - X axis on new position
        /// @p_NextY - Y axis on new position
        int16 CalculateWalkDirection(int16 const p_CurrentX, int16 const p_CurrentY, int16 const p_NextX, int16 const p_NextY);

        /// CalculateHumanDirection
        /// @p_CurrentX - Current X axis
        /// @p_CurrentY - Current Y axis
        /// @p_LookToX - Look To X axis     
        /// @p_LookToY - Look To Y axis
        int16 CalculateHumanDirection(int16 const m_CurrentX, int16 const m_CurrentY, int16 const p_LookToX, int16 const p_LookToY);

        /// CalculateHeadRotation
        /// @p_Rotation - Current Rotation
        /// @p_CurrentX - Current X axis
        /// @p_CurrentY - Current Y axis
        /// @p_LookToX - Look To X axis
        /// @p_LookToY - Look To Y axis
        int16 CalculateHeadRotation(int16 const p_Rotation, int16 const m_CurrentX, int16 const m_CurrentY, int16 const p_LookToX, int16 const p_LookToY);

    } ///< NAMESPACE MATHS
} ///< NAMESPACE STEERSTONE