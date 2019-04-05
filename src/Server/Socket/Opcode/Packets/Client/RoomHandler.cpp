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
    void HabboSocket::HandleGetInterest(std::unique_ptr<ClientPacket> p_Packet)
    {
        HabboPacket::Room::RoomInterest l_Packet;
        SendPacket(l_Packet.Write());
    }

    void HabboSocket::HandleRoomDirectory(std::unique_ptr<ClientPacket> p_Packet)
    {
        bool l_IsPublic = p_Packet->ReadBase64Bool();
        uint32 l_RoomId = p_Packet->ReadWiredUint();

        /// Initialize room connection with client
        HabboPacket::Room::OpenConnection l_Packet;
        SendPacket(l_Packet.Write());

        /// Send Room Advertisement url
        HabboPacket::Room::RoomUrl l_PacketUrl;
        SendPacket(l_PacketUrl.Write());

        /// Room::EnterRoom handles the error if room is full for example
        if (m_Habbo->SetRoom(sRoomMgr->GetRoom(l_RoomId)))
        {
            HabboPacket::Room::RoomReady l_PacketRoomReady;
            l_PacketRoomReady.Model = m_Habbo->GetRoom()->GetRoomModel().GetModel();
            l_PacketRoomReady.Id = l_RoomId;
            SendPacket(l_PacketRoomReady.Write());
        }
        else
        {
            /// If we get to here this means the room does not exist
            HabboPacket::Room::RoomCantConnect l_PacketCantConnect;
            l_PacketCantConnect.ErrorCode = RoomConnectionError::ROOM_IS_CLOSED;
            SendPacket(l_PacketCantConnect.Write());
        }
    }

    void HabboSocket::HandleGetRoomAdd(std::unique_ptr<ClientPacket> p_Packet)
    {
        HabboPacket::Room::RoomAdd l_Packet;
        SendPacket(l_Packet.Write());
    }

    void HabboSocket::HandleGetRoomHeight(std::unique_ptr<ClientPacket> p_Packet)
    {
        HabboPacket::Room::RoomHeight l_Packet;
        l_Packet.HeightMap = m_Habbo->GetRoom()->GetRoomModel().GetHeightMap();
        SendPacket(l_Packet.Write());
    }

    void HabboSocket::HandleRoomUsers(std::unique_ptr<ClientPacket> p_Packet)
    {
        /// Sending Habbo object so client and other clients can see a new habbo has joined the room
        m_Habbo->GetRoom()->SendNewHabboEntering(m_Habbo);
    }

    void HabboSocket::HandleGameObjects(std::unique_ptr<ClientPacket> p_Packet)
    {
        /// Send Furniture and any active furniture to client
        m_Habbo->GetRoom()->SendWorldObjects(m_Habbo);
        m_Habbo->GetRoom()->SendActiveObjects(m_Habbo);
    }

    void HabboSocket::HandleFurnitureRevisions(std::unique_ptr<ClientPacket> p_Packet)
    {
    }

    void HabboSocket::HandleLeaveRoom(std::unique_ptr<ClientPacket> p_Packet)
    {
        if (m_Habbo->GetRoom())
            m_Habbo->GetRoom()->LeaveRoom(m_Habbo);
    }

    void HabboSocket::HandleHabboMove(std::unique_ptr<ClientPacket> p_Packet)
    {
        int16 l_X = p_Packet->ReadBase64Int();
        int16 l_Y = p_Packet->ReadBase64Int();

        LOG_INFO << l_X << " " << l_Y;

        if (m_Habbo->GetRoom())
            m_Habbo->GetRoom()->Walk(m_Habbo, l_X, l_Y);
    }

    void HabboSocket::HandleUserDance(std::unique_ptr<ClientPacket> p_Packet)
    {
        if (m_Habbo->IsWaving())
            m_Habbo->SetIsWaving(false);

        m_Habbo->SetIsDancing(true);

        m_Habbo->SendUpdateStatusDance();
    }

    void HabboSocket::HandleHabboStopDance(std::unique_ptr<ClientPacket> p_Packet)
    {
        m_Habbo->SetIsDancing(false);

        m_Habbo->SendUpdateStatusDance();
    }

    void HabboSocket::HandleRoomHabboStatuses(std::unique_ptr<ClientPacket> p_Packet)
    {
        m_Habbo->GetRoom()->SendHabboRoomStatuses(m_Habbo);
    }

    void HabboSocket::HandleHabboWave(std::unique_ptr<ClientPacket> p_Packet)
    {
        m_Habbo->SendUpdateStatusWave();
    }
}