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
    void PlayerSocket::HandleSearchBusyFlats(std::unique_ptr<Packet> packet)
    {
        StringBuffer buffer;
        buffer << "# BUSY_FLAT_RESULTS 1";

        for (const auto& itr : sRoomMgr->GetRoomStorage())
        {
            buffer.AppendCarriage();
            buffer << (uint32)(itr.second->GetId());
            buffer.AppendForwardSlash();
            buffer << (std::string)itr.second->GetName();
            buffer << (std::string)" Room";
            buffer.AppendForwardSlash();
            buffer << (std::string)itr.second->GetOwnerName();
            buffer.AppendForwardSlash();
            buffer << (uint8)0;
            buffer.AppendForwardSlash();
            buffer.AppendForwardSlash();
            buffer << (std::string)itr.second->GetFloorLevel();
            buffer.AppendForwardSlash();
            buffer << (std::string)GetRemoteAddress();
            buffer.AppendForwardSlash();
            buffer << (std::string)GetRemoteAddress();
            buffer.AppendForwardSlash();
            buffer << (uint32)(itr.second->GetServerPort());
            buffer.AppendForwardSlash();
            buffer << (uint8)itr.second->GetNowIn();
            buffer.AppendForwardSlash();
            buffer << (uint8)itr.second->GetMaxIn();
            buffer.AppendForwardSlash();
        }

        buffer.AppendEndCarriage();
        SendPacket((char*)buffer.GetContents(), buffer.GetSize());
    }
    //-----------------------------------------------//
    void PlayerSocket::HandleInitUnitListener(std::unique_ptr<Packet> packet)
    {
        StringBuffer buffer;
        buffer << "# ALLUNITS";
        buffer.AppendCarriage();

        for (const auto& itr : sRoomMgr->GetRoomStorage())
        {
            if (itr.second->GetType() != RoomFlag::ROOM_TYPE_PUBLIC || !itr.second->IsEnabled())
                continue;

            buffer << (std::string)itr.second->GetName();
            buffer.AppendComma();
            buffer << (uint8)itr.second->GetNowIn();
            buffer.AppendComma();
            buffer << (uint8)itr.second->GetMaxIn();
            buffer.AppendComma();
            buffer << (std::string)GetRemoteAddress();
            buffer.AppendForwardSlash();
            buffer << (std::string)GetRemoteAddress();
            buffer.AppendComma();
            buffer << (uint32)itr.second->GetServerPort();
            buffer.AppendComma();
            buffer << itr.second->GetName();
            buffer.AppendTab();
            buffer << (std::string)itr.second->GetFloorLevel();
            buffer.AppendComma();
            buffer << (uint8)itr.second->GetNowIn();
            buffer.AppendComma();
            buffer << (uint8)itr.second->GetMaxIn();
            buffer.AppendComma();
            buffer << (std::string)itr.second->GetModel();
            buffer.AppendCarriage();
        }

        buffer.AppendEndCarriage();
        SendPacket((char*)buffer.GetContents(), buffer.GetSize());
    }
}