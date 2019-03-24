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
#include "RoomManager.h"
//-----------------------------------------------//
namespace SteerStone
{
    //-----------------------------------------------//
    void HabboSocket::HandleGetInterest(std::unique_ptr<ClientPacket> p_Packet)
    {
        StringBuffer buffer;
        buffer.AppendBase64(PacketServerHeader::SERVER_ROOM_INTEREST);
        buffer.AppendString("0");
        buffer.AppendSOH();
        SendPacket(buffer);
    }
    //-----------------------------------------------//
    void HabboSocket::HandleRoomDirectory(std::unique_ptr<ClientPacket> p_Packet)
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
    void HabboSocket::HandleGetRoomAdd(std::unique_ptr<ClientPacket> p_Packet)
    {
        StringBuffer buffer;
        buffer.AppendBase64(PacketServerHeader::SERVER_ROOM_ADD);
        buffer.AppendWired(0);
        buffer.AppendSOH();
        SendPacket(buffer);
    }
    //-----------------------------------------------//
    void HabboSocket::HandleGetRoomHeight(std::unique_ptr<ClientPacket> p_Packet)
    {
        StringBuffer buffer;
        buffer.AppendBase64(PacketServerHeader::SERVER_ROOM_HEIGHT_MAP);
        buffer.AppendString(mPlayer->GetRoom()->GetRoomModel()->GetHeightMap());
        buffer.AppendSOH();
        SendPacket(buffer);
    }
    //-----------------------------------------------//
    void HabboSocket::HandleRoomUsers(std::unique_ptr<ClientPacket> p_Packet)
    {
        mPlayer->GetRoom()->EnterRoom(mPlayer);
    }
    //-----------------------------------------------//
    void HabboSocket::HandleGameObjects(std::unique_ptr<ClientPacket> p_Packet)
    {
        mPlayer->GetRoom()->SendObjectsWorld(mPlayer);

        StringBuffer buffer;
        buffer.AppendBase64(PacketServerHeader::SERVER_ACTIVE_OBJECTS);
        buffer.AppendString("H", false);
        buffer.AppendSOH();
        SendPacket(buffer);
    }

    void HabboSocket::HandleFurnitureRevisions(std::unique_ptr<ClientPacket> p_Packet)
    {
    }
}