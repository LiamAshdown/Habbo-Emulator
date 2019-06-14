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

#include "ItemDefinition.h"

namespace SteerStone
{
    /// Constructor
    ItemDefinition::ItemDefinition()
    {
    }

    /// Deconstructor
    ItemDefinition::~ItemDefinition()
    {
    }

    /// HasBehaviour
    /// Returns whether this item has a specific behaviour
    /// @p_Behaviour : Behaviour we are searching for
    bool ItemDefinition::HasBehaviour(std::string const p_Behaviour)
    {
        if (m_Behaviour.empty())
            return false;

        std::vector<std::string> l_Split;
        boost::split(l_Split, m_Behaviour, boost::is_any_of(","));

        for (auto const& l_Itr : l_Split)
        {
            if (l_Itr == p_Behaviour)
                return true;
        }

        return false;
    }

    /// GetIcon
    /// @p_SpriteId : Id we are searching for
    std::string ItemDefinition::GetIcon(uint32 const p_SpriteId)
    {
        std::string l_Icon = m_Sprite;

        if (p_SpriteId > 0)
            l_Icon += " " + p_SpriteId;

        return l_Icon;
    }
} ///< NAMESPACE STEERSTONE