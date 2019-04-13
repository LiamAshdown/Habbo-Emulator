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
#include "HabboSocket.h"
#include "RoomManager.h"
#include "Database/QueryDatabase.h"

#include "Opcode/Packets/Server/LoginPackets.h"
#include "Opcode/Packets/Server/RoomPackets.h"

namespace SteerStone
{
    void HabboSocket::HandleCreateFlat(std::unique_ptr<ClientPacket> p_Packet)
    {
        std::vector<std::string> l_Split;
        boost::split(l_Split, p_Packet->GetContent(), boost::is_any_of("/"));

        std::string l_FloorType = l_Split[1];
        std::string l_RoomName = l_Split[2];
        std::string l_RoomModel = l_Split[3];
        std::string l_RoomAccess = l_Split[4];
        bool l_ShowOwnerName = l_Split[5] == "1" ? true : false;

        bool l_CreateFlat = true;

        if (l_FloorType != "first floor")
            l_CreateFlat = false;

        if (l_RoomName.empty())
            l_CreateFlat = false;

        if (l_RoomModel.empty())
            l_CreateFlat = false;

        /// Credits Quackster for this part
        std::string l_RoomModelContains = l_RoomModel.substr(6);
        if (l_RoomModelContains != "a" &&
            l_RoomModelContains != "c" &&
            l_RoomModelContains != "d" &&
            l_RoomModelContains != "e" &&
            l_RoomModelContains != "f" &&
            l_RoomModelContains != "i" &&
            l_RoomModelContains != "j" &&
            l_RoomModelContains != "k" &&
            l_RoomModelContains != "l" &&
            l_RoomModelContains != "m" &&
            l_RoomModelContains != "n")
            l_CreateFlat = false;

        /// ErrorMessenger creating flat? let player know
        if (!l_CreateFlat)
        {
            HabboPacket::Login::LocalisedError l_Packet;
            l_Packet.Error = "ErrorMessenger creating a private room";
            m_Habbo->ToSocket()->SendPacket(l_Packet.Write());
            return;
        }

        uint16 l_AccessType = RoomAccessType::ROOM_ACCESS_TYPE_OPEN;

        if (l_RoomAccess == "closed")
            l_AccessType = RoomAccessType::ROOM_ACCESS_TYPE_OPEN;

        if (l_RoomAccess == "password")
            l_AccessType = RoomAccessType::ROOM_ACCESS_TYPE_PASSWORD;

        /// Okay all good! Lets insert our new room into database
        QueryDatabase l_Database("rooms");
        l_Database.PrepareQuery("INSERT INTO rooms (owner_id, owner_name, name, model, show_name, description, access_type) VALUES (?, ?, ?, ?, ?, ?, ?)");
        l_Database.GetStatement()->setUInt(1, m_Habbo->GetId());
        l_Database.GetStatement()->setString(2, m_Habbo->GetName());
        l_Database.GetStatement()->setString(3, l_RoomName);
        l_Database.GetStatement()->setString(4, l_RoomModel);
        l_Database.GetStatement()->setBoolean(5, l_ShowOwnerName);
        l_Database.GetStatement()->setString(6, "");
        l_Database.GetStatement()->setUInt(7, l_AccessType);
        l_Database.ExecuteQuery();

        /// Get the Room Id of room we just created
        l_Database.ClearParameters();

        l_Database.PrepareQuery("SELECT LAST_INSERT_ID() as id");
        l_Database.ExecuteQuery();

        if (l_Database.GetResult())
            m_Habbo->m_LastCreatedRoomId = l_Database.Fetch()->GetInt32(1);

        /// Let client know created room successfully!
        HabboPacket::Room::GoToFlat l_Packet;
        l_Packet.Id   = std::to_string(m_Habbo->m_LastCreatedRoomId);
        l_Packet.Name = l_RoomName;
        m_Habbo->ToSocket()->SendPacket(l_Packet.Write());
    }

    void HabboSocket::HandleSetFlatCategory(std::unique_ptr<ClientPacket> p_Packet)
    {
        uint32 l_RoomId = p_Packet->ReadWiredUint();
        uint32 l_CategoryId = p_Packet->ReadWiredUint();
    }

} ///< NAMESPACE STEERSTONE
