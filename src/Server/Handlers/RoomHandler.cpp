/*
* Liam Ashdown
* Copyright (C) 2018
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
//-----------------------------------------------//
#include "../../Network/DatabaseManager.h"
#include "../../Entity/Player/Player.h"
#include "../../Manager/RoomManager.h"
#include "../../Room/Room.h"
#include "../WorldSession.h"
#include "../Packet/Opcodes.h"
#include "../../World/World.h"
//-----------------------------------------------//
void WorldSession::HandleSearchBusyFlats(std::string& packetBuffer, std::vector<std::string>& packetStorage)
{
    if (packetStorage.size() > 1)
    {
        WorldPacket data("# BUSY_FLAT_RESULTS 1");
        for (const auto& itr : sRoomManager->GetRoomStorage())
        {
            if (itr.second->GetType() != RoomFlags::PLAYER_ROOM || itr.second->IsHidden())
                continue;

            data.AppendCarriage();
            data << (uint32)(itr.second->GetRowId());
            data.AppendForwardSlash();
            data << (std::string)itr.second->GetName();
            data << (std::string)" Room";
            data.AppendForwardSlash();
            data << (std::string)itr.second->GetOwnerName();
            data.AppendForwardSlash();
            data << (std::string)itr.second->GetState();
            data.AppendForwardSlash();
            data.AppendForwardSlash();
            data << (std::string)itr.second->GetFloorLevel();
            data.AppendForwardSlash();
            data << GetSocket()->GetRealmIP();
            data.AppendForwardSlash();
            data << GetSocket()->GetRealmIP();
            data.AppendForwardSlash();
            data << (uint32)itr.second->GetRoomId();
            data.AppendForwardSlash();
            data << (uint8)itr.second->GetNowIn();
            data.AppendForwardSlash();
            data << (uint8)itr.second->GetMaxIn();
            data.AppendForwardSlash();
        }
        data.AppendEndCarriage();
        SendPacket(data.Write());
    }
    else
    {
        WorldPacket data("# BUSY_FLAT_RESULTS 1");
        data.AppendEndCarriage();
        SendPacket(data.Write());
    }
}
//-----------------------------------------------//
void WorldSession::HandleInitUnitListener(std::string& packetBuffer, std::vector<std::string>& packetStorage)
{
    WorldPacket data("# ALLUNITS");
    data.AppendCarriage();

    for (auto& itr : sRoomManager->GetRoomStorage())
    {
        if (itr.second->GetType() != RoomFlags::PUBLIC_ROOM || itr.second->IsHidden())
            continue;

        data << (std::string)itr.second->GetName();
        data.AppendComma();
        data << (uint8)itr.second->GetNowIn();
        data.AppendComma();
        data << (uint8)itr.second->GetMaxIn();
        data.AppendComma();
        data << GetSocket()->GetRealmIP();
        data.AppendForwardSlash();
        data << GetSocket()->GetRealmIP();
        data.AppendComma();
        data << (uint32)itr.second->GetRoomId();
        data.AppendComma();
        data << itr.second->GetName();
        data.AppendTab();
        data << (std::string)itr.second->GetFloorLevel();
        data.AppendComma();
        data << (uint8)itr.second->GetNowIn();
        data.AppendComma();
        data << (uint8)itr.second->GetMaxIn();
        data.AppendComma();
        data << (std::string)itr.second->GetModel();
        data.AppendCarriage();
    }
    data.AppendEndCarriage();
    SendPacket(data.Write());
}
//-----------------------------------------------//
void WorldSession::HandleMove(std::string& packetBuffer, std::vector<std::string>& packetStorage)
{
    if (!GetPlayer()->IsLeavingRoom())
    {
        uint8 x = std::stoi(packetStorage[1]);
        uint8 y = std::stoi(packetStorage[2]);
        GetPlayer()->Move(x, y);
    }
}
//-----------------------------------------------//
void WorldSession::HandleChat(std::string& packetBuffer, std::vector<std::string>& packetStorage)
{
    GetPlayer()->GetRoom()->SendChatToAll(GetPlayer(), OpcodesList::CHAT, packetBuffer);
}
//-----------------------------------------------//
void WorldSession::HandleShout(std::string& packetBuffer, std::vector<std::string>& packetStorage)
{
    GetPlayer()->GetRoom()->SendChatToAll(GetPlayer(), OpcodesList::SHOUT, packetBuffer);
}
//-----------------------------------------------//
void WorldSession::HandleWhisper(std::string& packetBuffer, std::vector<std::string>& packetStorage)
{
    GetPlayer()->GetRoom()->SendChatToAll(GetPlayer(), OpcodesList::WHISPER, packetBuffer);
}
//-----------------------------------------------//
void WorldSession::HandleCreateFlat(std::string& packetBuffer, std::vector<std::string>& packetStorage)
{
    GetPlayer()->CreateRoom(packetStorage);
}
//-----------------------------------------------//
void WorldSession::HandleSetFlatInfo(std::string& packetBuffer, std::vector<std::string>& packetStorage)
{
    GetPlayer()->SetRoomInfo(packetBuffer);
}
//-----------------------------------------------//
void WorldSession::HandleTryFlat(std::string& packetBuffer, std::vector<std::string>& packetStorage)
{ 
    std::vector<std::string> roomDetails;
    boost::split(roomDetails, packetBuffer, boost::is_any_of("/"));

    if (Room* room = sRoomManager->GetRoom((std::stoi(roomDetails[1]))))
    {
        if (room->IsEnabled())
        {
            if (room->GetState() == "open")
            {
                WorldPacket data("# FLAT_LETIN\r##");
                SendPacket(data.Write());
            }
            else if (room->GetState() == "closed")
            {
                ;// Ring Bell
            }
            else // Password
            {
                if (room->GetPassword() == roomDetails[2])
                {
                    WorldPacket data("# FLAT_LETIN\r##");
                    SendPacket(data.Write());
                }
                else
                {
                    WorldPacket data("# ERROR: Incorrect flat password\r##");
                    SendPacket(data.Write());
                }
            }
        }
    }
}
//-----------------------------------------------//
void WorldSession::HandleGoToFlat(std::string& packetBuffer, std::vector<std::string>& packetStorage)
{
    std::vector<std::string> roomDetails;
    boost::split(roomDetails, packetBuffer, boost::is_any_of("/"));

    if (Room* room = sRoomManager->GetRoom((std::stoi(roomDetails[1]))))
    {
        WorldPacket data;
        data << (std::string)"# ROOM_READY\r##";
        data << (std::string)"# FLATPROPERTY\rwallpaper/201\r##";
        data << (std::string)"# FLATPROPERTY\rfloor/0\r##";
        data << (std::string)"# YOUAREOWNER\r##";
        SendPacket(data.Write());

        room->SendRoomHeight(GetPlayer());

        data.Clear();

        data << (std::string)"# OBJECTS WORLD 0 ";
        data << (std::string)room->GetModel();

        data << (std::string)"# ACTIVE_OBJECTS\r##";
        data << (std::string)"# ITEMS\r##";
        SendPacket(data.Write());
        
        GetPlayer()->SetRoom(room);
        GetPlayer()->GetRoom()->SendUserStatusToAll(GetPlayer());
        GetPlayer()->GetRoom()->SendUpdateStatusToAll(GetPlayer());

    }
}
//-----------------------------------------------//
void WorldSession::HandleGetFlatInfo(std::string& packetBuffer, std::vector<std::string>& packetStorage)
{
    boost::split(packetStorage, packetBuffer, boost::is_any_of("/"));
    if (Room* room = sRoomManager->GetRoom((std::stoi(packetStorage[1]))))
    {
        WorldPacket data("# SETFLATINFO");
        data.AppendCarriage();
        data.AppendForwardSlash();
        data << (uint32)room->GetRowId();
        data.AppendForwardSlash();
        data.AppendEndCarriage();
        SendPacket(data.Write());
    }
}
//-----------------------------------------------//
void WorldSession::HandleUpdateFlat(std::string& packetBuffer, std::vector<std::string>& packetStorage)
{
    boost::split(packetStorage, packetBuffer, boost::is_any_of("/"));

    if (Room* room = sRoomManager->GetRoom((std::stoi(packetStorage[1]))))
    {
        std::shared_ptr<MySQLConnection> connection = sDBManager->getConnectionPool()->borrow();
        try
        {
            sql::PreparedStatement* prepareStatement = connection->sql_connection->prepareStatement
            ("UPDATE rooms SET name = ?, state = ?, allsuperuser = ? WHERE id = ?");

            prepareStatement->setString(1, packetStorage[2]);
            prepareStatement->setString(2, packetStorage[3]);
            prepareStatement->setBoolean(3, packetStorage[4] == "1" ? 1 : 0);
            prepareStatement->setInt(4, std::stoi(packetStorage[1]));
            prepareStatement->executeQuery();
        }
        catch (sql::SQLException &e)
        {
            sDBManager->printException(e, const_cast<char*>(__FILE__), const_cast<char*>(__FUNCTION__), __LINE__);
        }
    }
}
//-----------------------------------------------//
void WorldSession::HandleDeleteFlat(std::string& packetBuffer, std::vector<std::string>& packetStorage)
{
    boost::split(packetStorage, packetBuffer, boost::is_any_of("/"));

    if (Room* room = sRoomManager->GetRoom((std::stoi(packetStorage[1]))))
    {
        std::shared_ptr<MySQLConnection> connection = sDBManager->getConnectionPool()->borrow();
        try
        {
            sql::PreparedStatement* prepareStatement = connection->sql_connection->prepareStatement
            ("DELETE FROM rooms WHERE Id = ?");
            prepareStatement->setInt(1, std::stoi(packetStorage[1]));
            prepareStatement->executeQuery();

            sRoomManager->RemoveRoom(std::stoi(packetStorage[1]));

            WorldPacket data("# BUSY_FLAT_RESULTS 1");
            for (const auto& itr : sRoomManager->GetRoomStorage())
            {
                if (itr.second->GetType() != RoomFlags::PLAYER_ROOM || itr.second->IsHidden())
                    continue;

                data.AppendCarriage();
                data << (uint32)(itr.second->GetRowId());
                data.AppendForwardSlash();
                data << (std::string)itr.second->GetName();
                data << (std::string)" Room";
                data.AppendForwardSlash();
                data << (std::string)itr.second->GetOwnerName();
                data.AppendForwardSlash();
                data << (std::string)itr.second->GetState();
                data.AppendForwardSlash();
                data.AppendForwardSlash();
                data << (std::string)itr.second->GetFloorLevel();
                data.AppendForwardSlash();
                data << GetSocket()->GetRealmIP();
                data.AppendForwardSlash();
                data << GetSocket()->GetRealmIP();
                data.AppendForwardSlash();
                data << (uint32)itr.second->GetRoomId();
                data.AppendForwardSlash();
                data << (uint8)itr.second->GetNowIn();
                data.AppendForwardSlash();
                data << (uint8)itr.second->GetMaxIn();
                data.AppendForwardSlash();
            }
            data.AppendEndCarriage();
            SendPacket(data.Write());
        }
        catch (sql::SQLException &e)
        {
            sDBManager->printException(e, const_cast<char*>(__FILE__), const_cast<char*>(__FUNCTION__), __LINE__);
        }
    }
}
//-----------------------------------------------//
void WorldSession::HandleGoAway(std::string& packetBuffer, std::vector<std::string>& packetStorage)
{
    // TODO; find out the proper way of leaving the room, setting the z axis to 99 only temporary makes the user leave
    // until some one else joins
    GetPlayer()->SetLeavingRoom(true);
    GetPlayer()->Move(GetPlayer()->GetRoom()->GetRoomModel().x, GetPlayer()->GetRoom()->GetRoomModel().y);
}
//-----------------------------------------------//
void WorldSession::HandleLookTo(std::string& packetBuffer, std::vector<std::string>& packetStorage)
{
    uint8 x = std::stoi(packetStorage[1]);
    uint8 y = std::stoi(packetStorage[2]);

    GetPlayer()->Move(x, y);
}
//-----------------------------------------------//