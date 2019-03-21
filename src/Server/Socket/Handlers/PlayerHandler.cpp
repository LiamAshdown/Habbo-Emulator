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
#include "../PlayerSocket.h"
#include "Network/StringBuffer.h"
#include "Database/QueryDatabase.h"
#include "../Entity/Player/Player.h"
//-----------------------------------------------//
namespace Quad
{
    //-----------------------------------------------//
    void PlayerSocket::HandlePong(std::unique_ptr<Packet> packet)
    {
        mPlayer->mPonged = true;
    }
    void PlayerSocket::HandleGetInfo(std::unique_ptr<Packet> packet)
    {
        if (mPlayer && mPlayer->IsInitialized())
            mPlayer->SendUserObject();
    }
    //-----------------------------------------------//
    void PlayerSocket::HandleCreditBalance(std::unique_ptr<Packet> packet)
    {
        StringBuffer buffer;
        buffer.AppendBase64(PacketServerHeader::SERVER_CREDIT_BALANCE);
        buffer.AppendString(std::to_string(mPlayer->GetCredits()) + ".0");
        buffer.AppendSOH();
        SendPacket(buffer);
    }
    //-----------------------------------------------//
    void PlayerSocket::HandleNoDespaceUsers(std::unique_ptr<Packet> packet)
    {
        uint32 roomId = packet->ReadWiredUint();

        StringBuffer buffer;
        buffer.AppendBase64(PacketClientHeader::CLIENT_NO_DESPACE_USERS);
        buffer.AppendString(mPlayer->GetName());
        buffer.AppendSOH();
        SendPacket(buffer);
    }
    //-----------------------------------------------//
    void PlayerSocket::HandleGetClub(std::unique_ptr<Packet> packet)
    {

    }
    //-----------------------------------------------//
    void PlayerSocket::HandleGetAvailableBadges(std::unique_ptr<Packet> packet)
    {
        mPlayer->SendAccountBadges();
    }
    //-----------------------------------------------//
    void PlayerSocket::HandleGetAccountPreferences(std::unique_ptr<Packet> packet)
    {
        mPlayer->SendAccountPreferences();
    }
    //-----------------------------------------------//
    void PlayerSocket::HandleGetFavouriteRooms(std::unique_ptr<Packet> packet)
    {
        mPlayer->SendFavouriteRooms();
    }
    //-----------------------------------------------//
    void PlayerSocket::HandleAddFavouriteRoom(std::unique_ptr<Packet> packet)
    {
        bool roomType = packet->ReadWiredBool();
        uint32 roomId = packet->ReadWiredUint() - ROOM_ID_OFFSET;

        mPlayer->AddFavouriteRoom(roomType, roomId);
    }
    //-----------------------------------------------//
    void PlayerSocket::HandleRemoveFavouriteRoom(std::unique_ptr<Packet> packet)
    {
        bool roomType = packet->ReadWiredBool();
        uint32 roomId = packet->ReadWiredUint();

        mPlayer->RemoveFavouriteRoom(roomId);
    }
}
//-----------------------------------------------//