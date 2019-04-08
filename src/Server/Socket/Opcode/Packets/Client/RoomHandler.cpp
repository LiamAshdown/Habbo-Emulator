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

        /// Enter the room
        m_Habbo->SetRoom(sRoomMgr->GetRoom(l_RoomId));
    }

    void HabboSocket::HandleGetRoomAdd(std::unique_ptr<ClientPacket> p_Packet)
    {
        HabboPacket::Room::RoomAdd l_Packet;
        SendPacket(l_Packet.Write());
    }

    void HabboSocket::HandleGetRoomHeight(std::unique_ptr<ClientPacket> p_Packet)
    {
        if (!m_Habbo->GetRoom())
            return;

        HabboPacket::Room::RoomHeight l_Packet;
        l_Packet.HeightMap = m_Habbo->GetRoom()->GetRoomModel().GetHeightMap();
        SendPacket(l_Packet.Write());
    }

    void HabboSocket::HandleRoomUsers(std::unique_ptr<ClientPacket> p_Packet)
    {
        if (!m_Habbo->GetRoom())
            return;

        /// Sending Habbo object so client and other clients can see a new habbo has joined the room
        m_Habbo->GetRoom()->AddFigure(m_Habbo);
    }

    void HabboSocket::HandleGameObjects(std::unique_ptr<ClientPacket> p_Packet)
    {
        if (!m_Habbo->GetRoom())
            return;

        /// Send Furniture and any active furniture to client
        m_Habbo->GetRoom()->SendWorldObjects(m_Habbo);
        m_Habbo->GetRoom()->SendActiveObjects(m_Habbo);
    }

    void HabboSocket::HandleFurnitureRevisions(std::unique_ptr<ClientPacket> p_Packet)
    {
        if (!m_Habbo->GetRoom())
            return;
    }

    void HabboSocket::HandleLeaveRoom(std::unique_ptr<ClientPacket> p_Packet)
    {
        if (!m_Habbo->GetRoom())
            return;

        if (m_Habbo->GetRoom())
            m_Habbo->GetRoom()->LeaveRoom(m_Habbo);
    }

    void HabboSocket::HandleHabboMove(std::unique_ptr<ClientPacket> p_Packet)
    {
        if (!m_Habbo->GetRoom())
            return;

        int16 l_X = p_Packet->ReadBase64Int();
        int16 l_Y = p_Packet->ReadBase64Int();

        LOG_INFO << l_X << " " << l_Y;

        if (m_Habbo->GetRoom())
            m_Habbo->GetRoom()->Walk(m_Habbo->GetRoomGUID(), l_X, l_Y);
    }

    void HabboSocket::HandleUserDance(std::unique_ptr<ClientPacket> p_Packet)
    {
        if (!m_Habbo->GetRoom())
            return;

        m_Habbo->GetRoom()->RemoveStatus(m_Habbo->GetRoomGUID(), Status::STATUS_WAVING);
        m_Habbo->GetRoom()->AddStatus(m_Habbo->GetRoomGUID(), Status::STATUS_DANCING);
    }

    void HabboSocket::HandleHabboStopDance(std::unique_ptr<ClientPacket> p_Packet)
    {
        if (!m_Habbo->GetRoom())
            return;

        m_Habbo->GetRoom()->RemoveStatus(m_Habbo->GetRoomGUID(), Status::STATUS_DANCING);
    }

    void HabboSocket::HandleRoomHabboStatuses(std::unique_ptr<ClientPacket> p_Packet)
    {
        if (!m_Habbo->GetRoom())
            return;

        m_Habbo->GetRoom()->SendRoomStatuses(m_Habbo);
    }

    void HabboSocket::HandleHabboWave(std::unique_ptr<ClientPacket> p_Packet)
    {
        if (!m_Habbo->GetRoom())
            return;

        m_Habbo->GetRoom()->RemoveStatus(m_Habbo->GetRoomGUID(), Status::STATUS_DANCING);
        m_Habbo->GetRoom()->AddStatus(m_Habbo->GetRoomGUID(), Status::STATUS_WAVING);
    }

    void HabboSocket::HandleLookTo(std::unique_ptr<ClientPacket> p_Packet)
    {
        if (!m_Habbo->GetRoom())
            return;

        std::vector<std::string> l_Split;
        boost::split(l_Split, p_Packet->GetContent(), boost::is_any_of(" "));

        m_Habbo->LookTo(std::stoi(l_Split[0]), std::stoi(l_Split[1]));
    }

    void HabboSocket::HandleRoomChat(std::unique_ptr<ClientPacket> p_Packet)
    {
        if (!m_Habbo->GetRoom())
            return;

        HabboPacket::Room::Chat l_Packet;
        l_Packet.Message = p_Packet->ReadString();
        l_Packet.GUID = m_Habbo->GetRoomGUID();
        m_Habbo->GetRoom()->SendPacketToAll(l_Packet.Write());
    }

    void HabboSocket::HandleRoomShout(std::unique_ptr<ClientPacket> p_Packet)
    {
        if (!m_Habbo->GetRoom())

            return;
        HabboPacket::Room::Shout l_Packet;
        l_Packet.Message = p_Packet->ReadString();
        l_Packet.GUID = m_Habbo->GetRoomGUID();
        m_Habbo->GetRoom()->SendPacketToAll(l_Packet.Write());
    }

    void HabboSocket::HandleRoomWhisper(std::unique_ptr<ClientPacket> p_Packet)
    {
        if (!m_Habbo->GetRoom())
            return;

        std::string l_Contents = p_Packet->ReadString();

        HabboPacket::Room::Whisper l_Packet;
        l_Packet.GUID = m_Habbo->GetRoomGUID();

        auto const l_Position = l_Contents.find_first_of(' ');
        if (l_Position != std::string::npos)
        {
            std::string l_Name = l_Contents.substr(0, l_Position);
            
            if (Habbo* l_Habbo = m_Habbo->GetRoom()->FindHabboByName(l_Name))
            {
                l_Packet.Message = l_Contents.substr(l_Position + 1);
                l_Habbo->ToSocket()->SendPacket(l_Packet.Write());
                m_Habbo->ToSocket()->SendPacket(l_Packet.Write());
            }
            else
            {
                l_Packet.Message = l_Contents; ///< If there's no username, send it to everyone in room
                m_Habbo->GetRoom()->SendPacketToAll(l_Packet.Write());
            }
        }
    }
}