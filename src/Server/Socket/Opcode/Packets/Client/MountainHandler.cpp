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

#include "HabboSocket.h"
#include "Habbo.h"
#include "RoomManager.h"

namespace SteerStone
{
    void HabboSocket::HandleSwimSuit(ClientPacket* p_Packet)
    {
        if (TileInstance* l_TileInstance = m_Habbo->GetRoom()->GetRoomModel().GetTileInstance(m_Habbo->GetPositionX(), m_Habbo->GetPositionY()))
        {
            /// Set our Pool Figure
            m_Habbo->m_PoolFigure = p_Packet->GetContent();

            /// Set the file occupied to false which will call TriggerEventLeave
            l_TileInstance->SetTileOccupied(false, m_Habbo);
        }

    }
} ///< NAMESPACE STEERSTONE
