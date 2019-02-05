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

    uint32 credits = sCatalogueManager->GetCatalogue(packetStorage[2])->Credits;

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
}
//-----------------------------------------------//
void WorldSession::HandlePurchase(std::string& packetBuffer, std::vector<std::string>& packetStorage)
{

}