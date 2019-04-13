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
    void HabboSocket::HandleInfoBus(std::unique_ptr<ClientPacket> p_Packet)
    {
        if (!m_Habbo->GetRoom())
            return;

        /// Walk on walkway tile to access info bus
        m_Habbo->GetRoom()->Walk(m_Habbo->GetRoomGUID(), 28, 4);
    }

    void HabboSocket::HandleChangeWorld(std::unique_ptr<ClientPacket> p_Packet)
    {
        if (!m_Habbo->GetRoom())
            return;

        /// Walk on walkway tile to access info bus
        m_Habbo->GetRoom()->Walk(m_Habbo->GetRoomGUID(), 11, 2);
    }
   
} ///< NAMESPACE STEERSTONE
