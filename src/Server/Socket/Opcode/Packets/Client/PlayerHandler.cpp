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
#include "Opcode/Packets/Server/HabboInfoPackets.h"

namespace SteerStone
{
    void HabboSocket::HandlePong(std::unique_ptr<ClientPacket> p_Packet)
    {
        m_Habbo->m_Ponged = true;
    }

    void HabboSocket::HandleGetInfo(std::unique_ptr<ClientPacket> p_Packet)
    {
        if (m_Habbo && m_Habbo->IsInitialized())
            m_Habbo->SendHabboObject();
    }
    
    void HabboSocket::HandleCreditBalance(std::unique_ptr<ClientPacket> p_Packet)
    {
        HabboPacket::HabboInfo::CreditBalance l_Packet;
        l_Packet.Credits = std::to_string(m_Habbo->GetCredits());
        SendPacket(l_Packet.Write());
    }
    
    void HabboSocket::HandleNoDespaceUsers(std::unique_ptr<ClientPacket> p_Packet)
    {
        uint32 l_RoomId = p_Packet->ReadWiredUint() - PUBLIC_ROOM_OFFSET;

       /* StringBuffer buffer;
        buffer.AppendBase64(PacketClientHeader::CLIENT_NO_DESPACE_USERS);
        buffer.AppendString(mPlayer->GetName());
        buffer.AppendSOH();
        SendPacket(buffer);*/
    }
    
    void HabboSocket::HandleGetClub(std::unique_ptr<ClientPacket> p_Packet)
    {

    }
    
    void HabboSocket::HandleGetAvailableBadges(std::unique_ptr<ClientPacket> p_Packet)
    {
        m_Habbo->SendAccountBadges();
    }
    
    void HabboSocket::HandleGetAccountPreferences(std::unique_ptr<ClientPacket> p_Packet)
    {
        m_Habbo->SendAccountPreferences();
    }
    
    void HabboSocket::HandleGetFavouriteRooms(std::unique_ptr<ClientPacket> p_Packet)
    {
        m_Habbo->SendFavouriteRooms();
    }
    
    void HabboSocket::HandleAddFavouriteRoom(std::unique_ptr<ClientPacket> p_Packet)
    {
        bool l_RoomType = p_Packet->ReadWiredBool();
        uint32 l_RoomId = p_Packet->ReadWiredUint() - PUBLIC_ROOM_OFFSET;

        m_Habbo->AddFavouriteRoom(l_RoomType, l_RoomId);
    }
    
    void HabboSocket::HandleRemoveFavouriteRoom(std::unique_ptr<ClientPacket> p_Packet)
    {
        bool l_RoomType = p_Packet->ReadWiredBool();
        uint32 l_RoomId = p_Packet->ReadWiredUint() - PUBLIC_ROOM_OFFSET;

        m_Habbo->RemoveFavouriteRoom(l_RoomId);
    }
} ///< NAMESPACE STEERSTONE
