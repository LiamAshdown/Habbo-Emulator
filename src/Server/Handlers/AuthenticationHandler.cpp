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
#include "../../World/World.h"
//-----------------------------------------------//
void WorldSession::HandleVersionCheck(std::string& packetBuffer, std::vector<std::string>& packetStorage)
{
    if (packetStorage[1] == "client002")
    {
        WorldPacket data("# ENCRYPTION_OFF\r##");
        WorldPacket buffer("# SECRET_KEY\r31vw2swky25q9ko940i8x068ftxrmt0wa3vgj27qtrr3m35rn067o549fl\r##");
        SendPacket(data.Write());
        SendPacket(buffer.Write());
    }
}
//-----------------------------------------------//
void WorldSession::HandleKeyEncrypted(std::string& packetBuffer, std::vector<std::string>& packetStorage)
{
    ;// Opcode not handled yet
}
//-----------------------------------------------//
void WorldSession::HandleClientIp(std::string& packetBuffer, std::vector<std::string>& packetStorage)
{
    ;// Opcode not handled yet
}
//-----------------------------------------------//
void WorldSession::HandleApproveName(std::string& packetBuffer, std::vector<std::string>& packetStorage)
{
    ;// Opcode not handled yet
}
//-----------------------------------------------//
inline std::string SplitString(std::string string, std::string key)
{
    std::size_t startPosition = string.find(key);

    if (string[startPosition + key.length()] == '=')
    {
        std::string keyString;
        for (std::size_t i = startPosition + (key.length() + 1); i < string.length(); i++)
        {
            if (string[i] == '\r')
                break;

            keyString += string[i];
        }

        return keyString;
    }

    return "0";
}
//-----------------------------------------------//
void WorldSession::HandleRegister(std::string& packetBuffer, std::vector<std::string>& packetStorage)
{
    std::shared_ptr<MySQLConnection> connection = sDBManager->getConnectionPool()->borrow();
    try {
        sql::PreparedStatement* prepareStatement = connection->sql_connection->prepareStatement
        ("INSERT INTO users(name, password, credits, email, figure, birthday, phonenumber, customData, had_read_agreement, sex, country, has_special_rights, badge_type, inroom) VALUES(? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? , ? )");
        prepareStatement->setString(1, SplitString(packetStorage[1], "name"));
        prepareStatement->setString(2, SplitString(packetStorage[1], "password"));
        prepareStatement->setInt(3, 0);                                    // Credits
        prepareStatement->setString(4, SplitString(packetStorage[1], "email"));
        prepareStatement->setString(5, SplitString(packetStorage[1], "figure"));
        prepareStatement->setString(6, SplitString(packetStorage[1], "birthday"));
        prepareStatement->setString(7, SplitString(packetStorage[1], "phonenumber"));
        prepareStatement->setString(8, SplitString(packetStorage[1], "customData"));
        prepareStatement->setInt(9, 1);                                    // Unknown
        prepareStatement->setString(10, SplitString(packetStorage[1], "sex"));
        prepareStatement->setString(11, SplitString(packetStorage[1], "country"));
        prepareStatement->setString(12, "0");                              // TODO: special rights? probably something to do with being admin or mod
        prepareStatement->setString(13, "0");                              // Badge type... admin badge etc?
        prepareStatement->setInt(14, 0);                                   // Room type
        prepareStatement->executeQuery();
    }
    catch (sql::SQLException &e) 
    {
        sDBManager->printException(e, const_cast<char*>(__FILE__), const_cast<char*>(__FUNCTION__), __LINE__);
    }
}
//-----------------------------------------------//
void WorldSession::HandleLogin(std::string& packetBuffer, std::vector<std::string>& packetStorage)
{
    if (InitializePlayerData(packetStorage[1]))
    {
        // If our size is more than 4, that means we are entering a room
        if (packetStorage[4] == "0")
        {
            if (Room* room = GetPlayer()->GetRoom())
            {
                room->SendRoomFurniture(GetPlayer());
                room->SendRoomHeight(GetPlayer());
                room->SendUserStatusToAll(GetPlayer());
                room->SendUpdateStatusToAll(GetPlayer());
            }
            else
                CloseSocket();
        }
    }
    else
        SendLoginFailure();
}
//-----------------------------------------------//