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
void WorldSession::HandleGetCredits(std::string& packetBuffer, std::vector<std::string>& packetStorage)
{
    WorldPacket data("# WALLETBALANCE\r");
    data << (uint32)GetPlayer()->GetCredits();
    data.AppendEndCarriage();
    data << (std::string) "# MESSENGERSMSACCOUNT\rnoaccount\r##";
    data << (std::string)"# MESSENGERREADY \r\r##";
    SendPacket(data.Write());
}
//-----------------------------------------------//
void WorldSession::HandleStatusOk(std::string& packetBuffer, std::vector<std::string>& packetStorage)
{
    WorldPacket data("# OK");
    data.AppendEndCarriage();
    SendPacket(data.Write());
}
//-----------------------------------------------//
void WorldSession::HandleInfoRetrieve(std::string& packetBuffer, std::vector<std::string>& packetStorage)
{
    if (IsPlayerInitialized())
        GetPlayer()->SendObjectData();
    else
        SendLoginFailure();
}
//-----------------------------------------------//
void WorldSession::HandleDance(std::string& packetBuffer, std::vector<std::string>& packetStorage)
{
    if (!GetPlayer()->IsDancing())
        GetPlayer()->SetIsDancing(true);
}
//-----------------------------------------------//
void WorldSession::HandleStop(std::string& packetBuffer, std::vector<std::string>& packetStorage)
{
    if (packetStorage[1] == "Dance")
    {
        if (GetPlayer()->IsDancing())
            GetPlayer()->SetIsDancing(false);
    }
}
//-----------------------------------------------//
void WorldSession::HandleGetStrip(std::string& packetBuffer, std::vector<std::string>& packetStorage)
{
    std::shared_ptr<MySQLConnection> connection = sDBManager->getConnectionPool()->borrow();
    try
    {
        std::shared_ptr<sql::Connection> sql_connection = connection->sql_connection;
        std::shared_ptr<sql::PreparedStatement> statement = std::shared_ptr<sql::PreparedStatement>(sql_connection->prepareStatement("SELECT * FROM users WHERE userId = ?"));
        statement->setInt(1, GetPlayer()->GetAccountId());

        std::shared_ptr<sql::ResultSet> result_set = std::shared_ptr<sql::ResultSet>(statement->executeQuery());
    
        WorldPacket data("# STRIPINFO");
        data.AppendCarriage();
        while (result_set->next())
        {
            data << (std::string)"BLSI;";
            data << (std::string)result_set->getString(1);
            data << (std::string)";0;S;";
            data << (std::string)result_set->getString(1);
        }

    }
    catch (sql::SQLException &e)
    {
        sDBManager->printException(e, const_cast<char*>(__FILE__), const_cast<char*>(__FUNCTION__), __LINE__);
    }
}
//-----------------------------------------------//