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

#include "Item.h"

namespace SteerStone
{
    /// Constructor
    Item::Item()
    {
        m_Id = 0;
        m_SpriteId = 0;
        m_X = 0;
        m_Y = 0;
        m_Z = 0;
        m_Rotation = 0;
        m_TopHeight = 0;
        m_Length = 0;
        m_Width = 0;
        m_DrinkId = 0;
        m_Tradable = false;
        m_Recycleable = false;
    }

    /// Deconstructor
    Item::~Item()
    {
    }

    /// CanBeWalkedOn
    /// Check if tile can be walked on
    /// @p_Extra : If true do extra checks
    bool Item::CanBeWalkedOn(bool p_Extra /*= false*/)
    {
        if (GetTrigger() == "solid")
            return false;

        if (p_Extra)
            if (GetTrigger() == "can_sit_on_top")
                return false;

        return true;
    }
} /// NAMESPACE STEERSTONE
