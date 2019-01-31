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
#include "../Protocol/Socket.h"
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