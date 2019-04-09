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
#include "RoomManager.h"
#include "Opcode/Packets/Server/RoomPackets.h"

namespace SteerStone
{
    /// Constructor
    /// @p_Habbo :
    /// @p_RoomModel :
    WayPoints::WayPoints(Habbo* p_Habbo, RoomModel* p_RoomModel) : m_Habbo(p_Habbo), PathFinder(p_RoomModel), m_RoomModel(p_RoomModel)
    {
        m_ActivePath = false;
    }

    /// Deconstructor
    WayPoints::~WayPoints()
    {
    }

    /// SetEndPosition
    /// @p_X : End Position X
    /// @p_Y : End Position Y
    void WayPoints::SetEndPosition(int16 const p_X, int16 const p_Y)
    {
        m_EndX = p_X;
        m_EndY = p_Y;
    }

    /// CheckForInteractiveObjects
    /// Check if user is sitting or standing ontop any objects which user can interact with (automatically)
    void WayPoints::CheckForInteractiveObjects()
    {
        /// Get the current tile instance from Habbo position
        if (TileInstance* l_TileInstance = m_RoomModel->GetTileInstance(m_Habbo->GetPositionX(), m_Habbo->GetPositionY()))
        {
            /// Execute trigger event if there's any
            l_TileInstance->ExecuteTrigger(m_Habbo, m_Habbo->GetRoom(), l_TileInstance->GetItem());

            /// If we are on WalkWay tile - move to new room
            if (WalkWay* l_WalkWay = l_TileInstance->GetWalkWay())
            {
                /// Initialize room connection with client
                HabboPacket::Room::OpenConnection l_Packet;
                m_Habbo->ToSocket()->SendPacket(l_Packet.Write());

                /// Send Room Advertisement url
                HabboPacket::Room::RoomUrl l_PacketUrl;
                m_Habbo->ToSocket()->SendPacket(l_PacketUrl.Write());

                /// Enter the room
                m_Habbo->SetRoom(sRoomMgr->GetRoom(l_WalkWay->GetToId()), l_WalkWay);
            }
        }
    }
} ///< NAMESPACE STEERSTONE
