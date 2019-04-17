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
#include "Opcode/Packets/Server/ItemPackets.h"

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
        if (!m_Habbo->GetRoom())
            return;

        HabboPacket::Room::RoomAdd l_Packet;

        if (RoomUrlData* l_RoomUrlData = sRoomMgr->GetRoomUrl(m_Habbo->GetRoom()->GetId()))
        {
            l_Packet.ImageUrl = l_RoomUrlData->ImageUrl;
            l_Packet.LinkUrl = l_RoomUrlData->LinkUrl;
        }

        SendPacket(l_Packet.Write());
    }

    void HabboSocket::HandleGHeightMap(std::unique_ptr<ClientPacket> p_Packet)
    {
        if (!m_Habbo->GetRoom())
            return;

        HabboPacket::Room::RoomHeight l_Packet;
        l_Packet.HeightMap = m_Habbo->GetRoom()->GetRoomModel().GetHeightMap();
        SendPacket(l_Packet.Write());
    }

    void HabboSocket::HandleGUsers(std::unique_ptr<ClientPacket> p_Packet)
    {
        if (!m_Habbo->GetRoom())
            return;

        /// Sending Habbo object so client and other clients can see a new habbo has joined the room
        m_Habbo->GetRoom()->AddFigure(m_Habbo);
    }

    void HabboSocket::HandleGObjects(std::unique_ptr<ClientPacket> p_Packet)
    {
        if (!m_Habbo->GetRoom())
            return;

        /// Send Furniture and any active furniture to client
        m_Habbo->GetRoom()->SendWorldObjects(m_Habbo);
        m_Habbo->GetRoom()->SendActiveObjects(m_Habbo);
    }

    void HabboSocket::HandleFurnitureRevisions(std::unique_ptr<ClientPacket> p_Packet)
    {
    }

    void HabboSocket::HandleLeaveRoom(std::unique_ptr<ClientPacket> p_Packet)
    {
        if (!m_Habbo->GetRoom())
            return;
        
        m_Habbo->GetRoom()->LeaveRoom(m_Habbo);
    }

    void HabboSocket::HandleMove(std::unique_ptr<ClientPacket> p_Packet)
    {
        if (!m_Habbo->GetRoom())
            return;

        int16 l_X = p_Packet->ReadBase64Int();
        int16 l_Y = p_Packet->ReadBase64Int();
        
        m_Habbo->GetRoom()->Walk(m_Habbo->GetRoomGUID(), l_X, l_Y);
    }

    void HabboSocket::HandleDance(std::unique_ptr<ClientPacket> p_Packet)
    {
        if (!m_Habbo->GetRoom())
            return;

        if (!p_Packet->GetContent().empty() && m_Habbo->IsSubscribed())
        {
            int16 l_Id = p_Packet->ReadWiredInt();

            /// Is Id valid?
            if (l_Id < 0 || l_Id > 4)
                return;

            m_Habbo->SetDanceId(l_Id);
        }
        else
            m_Habbo->SetDanceId(0);

        m_Habbo->GetRoom()->RemoveStatus(m_Habbo->GetRoomGUID(), Status::STATUS_WAVING);
        m_Habbo->GetRoom()->AddStatus(m_Habbo->GetRoomGUID(), Status::STATUS_DANCING);
    }

    void HabboSocket::HandleStop(std::unique_ptr<ClientPacket> p_Packet)
    {
        if (!m_Habbo->GetRoom())
            return;

        m_Habbo->GetRoom()->RemoveStatus(m_Habbo->GetRoomGUID(), Status::STATUS_DANCING);
    }

    void HabboSocket::HandleGStat(std::unique_ptr<ClientPacket> p_Packet)
    {
        if (!m_Habbo->GetRoom())
            return;

        m_Habbo->GetRoom()->SendRoomStatuses(m_Habbo);
    }

    void HabboSocket::HandleWave(std::unique_ptr<ClientPacket> p_Packet)
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
                l_Habbo->SendPacket(l_Packet.Write());
                m_Habbo->SendPacket(l_Packet.Write());
            }
            else
            {
                l_Packet.Message = l_Contents; ///< If there's no username, send it to everyone in room
                m_Habbo->GetRoom()->SendPacketToAll(l_Packet.Write());
            }
        }
    }

    void HabboSocket::HandleSetBadge(std::unique_ptr<ClientPacket> p_Packet)
    {
        if (!m_Habbo->GetRoom())
            return;

        std::string l_Badge = p_Packet->ReadString();
        bool l_Visible = p_Packet->ReadWiredBool();
        m_Habbo->SendSetBadge(l_Badge, l_Visible);
    }
}