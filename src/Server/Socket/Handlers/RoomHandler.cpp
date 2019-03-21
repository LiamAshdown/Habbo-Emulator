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
#include "RoomManager.h"
//-----------------------------------------------//
namespace Quad
{
    //-----------------------------------------------//
    void PlayerSocket::HandleGetInterest(std::unique_ptr<Packet> packet)
    {
        StringBuffer buffer;
        buffer.AppendBase64(PacketServerHeader::SERVER_ROOM_INTEREST);
        buffer.AppendString("0");
        buffer.AppendSOH();
        SendPacket(buffer);
    }
    //-----------------------------------------------//
    void PlayerSocket::HandleRoomDirectory(std::unique_ptr<Packet> packet)
    {
        bool isPublic = packet->ReadBase64Bool();
        uint32 roomId = packet->ReadWiredUint();

        StringBuffer buffer;

        buffer.AppendBase64(PacketServerHeader::SERVER_OPEN_CONNECTION);
        buffer.AppendSOH();
        SendPacket(buffer);

        buffer.Clear();

        buffer.AppendBase64(PacketServerHeader::SERVER_ROOM_URL);
        buffer.AppendString("/client/");
        buffer.AppendSOH();
        SendPacket(buffer);

        buffer.Clear();

        if (mPlayer->SetRoom(sRoomMgr->GetRoom(roomId)))
        {
            buffer.AppendBase64(PacketServerHeader::SERVER_ROOM_READY);
            buffer.AppendString(mPlayer->GetRoom()->GetModel());
            buffer.AppendString(" ");
            buffer.AppendWired(roomId);
        }
        else
        {
            buffer.AppendBase64(PacketServerHeader::SERVER_CANT_CONNECT);
            buffer.AppendWired(RoomConnectionError::ROOM_IS_CLOSED);
        }

        buffer.AppendSOH();
        SendPacket(buffer);
    }
    //-----------------------------------------------//
    void PlayerSocket::HandleGetRoomAdd(std::unique_ptr<Packet> packet)
    {
        StringBuffer buffer;
        buffer.AppendBase64(PacketServerHeader::SERVER_ROOM_ADD);
        buffer.AppendWired(0);
        buffer.AppendSOH();
        SendPacket(buffer);
    }
    //-----------------------------------------------//
    void PlayerSocket::HandleGetRoomHeight(std::unique_ptr<Packet> packet)
    {
        StringBuffer buffer;
        buffer.AppendBase64(PacketServerHeader::SERVER_ROOM_HEIGHT_MAP);
        buffer.AppendString(mPlayer->GetRoom()->GetRoomModel()->GetHeightMap());
        buffer.AppendSOH();
        SendPacket(buffer);
    }
    //-----------------------------------------------//
    void PlayerSocket::HandleRoomUsers(std::unique_ptr<Packet> packet)
    {
        mPlayer->GetRoom()->EnterRoom(mPlayer);
        //StringBuffer buffer;
        //buffer.AppendBase64(PacketServerHeader::SERVER_ROOM_USER_OBJECTS);
        //buffer.AppendSOH();
        //SendPacket(buffer);
    }
    //-----------------------------------------------//
    void PlayerSocket::HandleGameObjects(std::unique_ptr<Packet> packet)
    {
        mPlayer->GetRoom()->SendObjectsWorld(mPlayer);

        StringBuffer buffer;
        buffer.AppendBase64(PacketServerHeader::SERVER_ACTIVE_OBJECTS);
        buffer.AppendString("H", false);
        buffer.AppendSOH();
        SendPacket(buffer);
    }

    void PlayerSocket::HandleFurnitureRevisions(std::unique_ptr<Packet> packet)
    {
    }
}