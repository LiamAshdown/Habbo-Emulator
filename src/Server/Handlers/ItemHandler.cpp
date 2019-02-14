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
#include "CatalogueManager.h"
#include "../WorldSession.h"
#include "../Packet/Opcodes.h"
#include "../../World/World.h"
//-----------------------------------------------//
void WorldSession::HandleOrderInfo(std::string& packetBuffer, std::vector<std::string>& packetStorage)
{
    Item* item = sCatalogueManager->GetCatalogueItem(packetStorage[2]);

    if (!item)
        return;

    uint32 credits = sCatalogueManager->GetCatalogueByCallId(packetStorage[2])->Credits;

    WorldPacket data("# GETORDERINFO");
    data.AppendCarriage();
    data << (std::string)item->GetSprite();
    data.AppendForwardSlash();
    data << (uint32)credits;
    data.AppendForwardSlash();
    data.AppendCarriage();
    data << (uint32)credits;
    data.AppendEndCarriage();
    SendPacket(data.Write());

    // Store the item Id for later since we will use it when player purchases
    GetPlayer()->SetPendingPurchase(item->GetId(), credits);
}
//-----------------------------------------------//
void WorldSession::HandlePurchase(std::string& packetBuffer, std::vector<std::string>& packetStorage)
{
    Item* item = sCatalogueManager->GetItemDefintion(GetPlayer()->GetPendingPurchase().itemId);    

    if (!item)
        return;

    std::shared_ptr<MySQLConnection> connection = sDBManager->getConnectionPool()->borrow();
    try
    {
        sql::PreparedStatement* prepareStatement = connection->sql_connection->prepareStatement
        ("INSERT INTO user_hand_furniture(user_id, item_id, room_id, x, y, z, rotation, extra_data) VALUES(?, ?, ?, ?, ?, ?, ?, ?)");

        prepareStatement->setInt(1, GetPlayer()->GetAccountId());
        prepareStatement->setInt(2, item->GetId());
        prepareStatement->setInt(3, GetPlayer()->GetRoom()->GetRoomId());
        prepareStatement->setInt(4, 0);
        prepareStatement->setInt(5, 0);
        prepareStatement->setInt(6, 0);
        prepareStatement->setDouble(7, 0);
        prepareStatement->setString(8, "");

        prepareStatement->executeQuery();

        GetPlayer()->SetCredits(GetPlayer()->GetCredits() - GetPlayer()->GetPendingPurchase().itemCredits);

        // Update client user credits
        WorldPacket data("# WALLETBALANCE");
        ByteBuffer buffer("# ADDSTRIPITEM\r##");
        data.AppendCarriage();
        data << (uint32)GetPlayer()->GetCredits();
        data.AppendEndCarriage();
        data.Append(buffer);
        SendPacket(data.Write());
    }
    catch (sql::SQLException &e)
    {
        sDBManager->printException(e, const_cast<char*>(__FILE__), const_cast<char*>(__FUNCTION__), __LINE__);
    }

    sDBManager->getConnectionPool()->unborrow(connection);
}