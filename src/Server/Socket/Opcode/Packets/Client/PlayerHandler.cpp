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
//-----------------------------------------------//
#include "../HabboSocket.h"
#include "Network/StringBuffer.h"
#include "Database/QueryDatabase.h"
#include "../Entity/Player/Player.h"
//-----------------------------------------------//
namespace SteerStone
{
    //-----------------------------------------------//
    void HabboSocket::HandlePong(std::unique_ptr<ClientPacket> p_Packet)
    {
        mPlayer->mPonged = true;
    }
    void HabboSocket::HandleGetInfo(std::unique_ptr<ClientPacket> p_Packet)
    {
        if (mPlayer && mPlayer->IsInitialized())
            mPlayer->SendUserObject();
    }
    //-----------------------------------------------//
    void HabboSocket::HandleCreditBalance(std::unique_ptr<ClientPacket> p_Packet)
    {
        StringBuffer buffer;
        buffer.AppendBase64(PacketServerHeader::SERVER_CREDIT_BALANCE);
        buffer.AppendString(std::to_string(mPlayer->GetCredits()) + ".0");
        buffer.AppendSOH();
        SendPacket(buffer);
    }
    //-----------------------------------------------//
    void HabboSocket::HandleNoDespaceUsers(std::unique_ptr<ClientPacket> p_Packet)
    {
        uint32 roomId = packet->ReadWiredUint() - PUBLIC_ROOM_OFFSET;

        StringBuffer buffer;
        buffer.AppendBase64(PacketClientHeader::CLIENT_NO_DESPACE_USERS);
        buffer.AppendString(mPlayer->GetName());
        buffer.AppendSOH();
        SendPacket(buffer);
    }
    //-----------------------------------------------//
    void HabboSocket::HandleGetClub(std::unique_ptr<ClientPacket> p_Packet)
    {

    }
    //-----------------------------------------------//
    void HabboSocket::HandleGetAvailableBadges(std::unique_ptr<ClientPacket> p_Packet)
    {
        mPlayer->SendAccountBadges();
    }
    //-----------------------------------------------//
    void HabboSocket::HandleGetAccountPreferences(std::unique_ptr<ClientPacket> p_Packet)
    {
        mPlayer->SendAccountPreferences();
    }
    //-----------------------------------------------//
    void HabboSocket::HandleGetFavouriteRooms(std::unique_ptr<ClientPacket> p_Packet)
    {
        mPlayer->SendFavouriteRooms();
    }
    //-----------------------------------------------//
    void HabboSocket::HandleAddFavouriteRoom(std::unique_ptr<ClientPacket> p_Packet)
    {
        bool roomType = packet->ReadWiredBool();
        uint32 roomId = packet->ReadWiredUint() - PUBLIC_ROOM_OFFSET;

        mPlayer->AddFavouriteRoom(roomType, roomId);
    }
    //-----------------------------------------------//
    void HabboSocket::HandleRemoveFavouriteRoom(std::unique_ptr<ClientPacket> p_Packet)
    {
        bool roomType = packet->ReadWiredBool();
        uint32 roomId = packet->ReadWiredUint() - PUBLIC_ROOM_OFFSET;

        mPlayer->RemoveFavouriteRoom(roomId);
    }
}
//-----------------------------------------------//