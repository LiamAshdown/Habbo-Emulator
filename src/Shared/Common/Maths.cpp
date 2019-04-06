/*
* Priston Tale EU
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

#include "Maths.h"

namespace SteerStone
{
    namespace Maths
    {
        static std::default_random_engine s_Random(time(NULL));

        /// GetRandomNumber
        /// Return a random number in specified range
        uint32 GetRandomUint32(uint32 const p_Min, uint32 const p_Max)
        {
            std::uniform_int_distribution<uint32> l_Random(p_Min, p_Max);
            return l_Random(s_Random);
        }

        /// CalculateWalkDirection
        /// @p_CurrentX - Current X axis
        /// @p_CurrentY - Current Y axis
        /// @p_NextX - X axis on new position
        /// @p_NextY - Y axis on new position
        /// Credit Quackster
        int16 CalculateWalkDirection(int16 const p_CurrentX, int16 const p_CurrentY, int16 const p_NextX, int16 const p_NextY)
        {
            if (p_CurrentX == p_NextX)
            {
                if (p_CurrentY < p_NextY)
                    return 4;
                else
                    return 0;
            }
            else if (p_CurrentX > p_NextX)
            {
                if (p_CurrentY == p_NextY)
                    return 6;
                else if (p_CurrentY < p_NextY)
                    return 5;
                else
                    return 7;
            }
            else 
            {
                if (p_CurrentY == p_NextY)
                    return 2;
                else if (p_CurrentY < p_NextY)
                    return 3;
                else
                    return 1;
            }
        }

        /// CalculateHumanDirection
        /// @p_CurrentX - Current X axis
        /// @p_CurrentY - Current Y axis
        /// @p_LookToX - Look To X axis
        /// @p_LookToY - Look To Y axis
        /// Credit Quackster
        int16 CalculateHumanDirection(int16 const m_CurrentX, int16 const m_CurrentY, int16 const p_LookToX, int16 const p_LookToY)
        {
            int16 l_Rotation = 0;

            if (m_CurrentX > p_LookToX && m_CurrentY > p_LookToY)
                l_Rotation = 7;
            else if (m_CurrentX < p_LookToX && m_CurrentY < p_LookToY)
                l_Rotation = 3;
            else if (m_CurrentX > p_LookToX && m_CurrentY < p_LookToY)
                l_Rotation = 5;
            else if (m_CurrentX < p_LookToX && m_CurrentY > p_LookToY)
                l_Rotation = 1;
            else if (m_CurrentX > p_LookToX)
                l_Rotation = 6;
            else if (m_CurrentX < p_LookToX)
                l_Rotation = 2;
            else if (m_CurrentY < p_LookToY)
                l_Rotation = 4;

            return l_Rotation;
        }

        /// CalculateHeadRotation
        /// @p_Rotation - Current Rotation
        /// @p_CurrentX - Current X axis
        /// @p_CurrentY - Current Y axis
        /// @p_LookToX - Look To X axis
        /// @p_LookToY - Look To Y axis
        /// Credit Quackster
        int16 CalculateHeadRotation(int16 const p_Rotation, int16 const m_CurrentX, int16 const m_CurrentY, int16 const p_LookToX, int16 const p_LookToY)
        {
            int16 l_HeadRotation = p_Rotation;
            int16 l_Diff = p_Rotation - CalculateHumanDirection(m_CurrentX, m_CurrentY, p_LookToX, p_LookToY);

            if ((p_Rotation % 2) == 0) {
                if (l_Diff > 0) {
                    l_HeadRotation = (p_Rotation - 1);
                }
                else if (l_Diff < 0) {
                    l_HeadRotation = (p_Rotation + 1);
                }
            }

            return l_HeadRotation;
        }
    } ///< NAMESPACE MATHS
} ///< NAMESPACE STEERSTONE
