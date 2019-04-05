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

#include "Habbo.h"
#include "WayPoints.h"
#include "Opcode/Packets/Server/RoomPackets.h"

namespace SteerStone
{
    /// Constructor
    /// @p_TileGrid - Tile Grid array which stores instance of tile
    /// @p_MaxGridX : Max X Tile Grid
    /// @p_MaxGridY : Max Y Tile Grid
    WayPoints::WayPoints(RoomModel* p_RoomModel) : PathFinder(p_RoomModel), m_Habbo(nullptr), m_RoomModel(p_RoomModel)
    {
    }

    /// Deconstructor
    WayPoints::~WayPoints()
    {
    }

    /// CheckForInteractiveObjects
    /// Check if user is sitting or standing ontop any objects which user can interact with (automatically)
    void WayPoints::CheckForInteractiveObjects()
    {
        /// Get the current tile instance from Habbo position
        if (TileInstance* l_TileInstance = m_RoomModel->GetTileInstance(m_Habbo->GetPositionX(), m_Habbo->GetPositionY()))
        {
            if (Item* l_Item = l_TileInstance->GetItem())
            {
                /// If Habbo is ontop of an item which we can sit on, execute the sit function
                if (l_Item->GetBehaviour() == "can_sit_on_top")
                    m_Habbo->SendUpdateStatusSit(l_Item->GetPositionX(), l_Item->GetPositionY(), l_Item->GetPositionZ(), l_Item->GetRotation());
            }
            else
                m_Habbo->SendUpdateStatusStop(); ///< Nothing else? Then send stop status
        }
        else
            m_Habbo->SendUpdateStatusStop();
    }

} ///< NAMESPACE STEERSTONE
