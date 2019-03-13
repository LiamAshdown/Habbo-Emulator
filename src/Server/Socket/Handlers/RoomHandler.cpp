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
    void PlayerSocket::HandleNavigate(std::unique_ptr<Packet> packet)
    {
        bool hideFullRooms = packet->ReadBool();
        int32 categoryId = packet->ReadWiredInt();

        StringBuffer buffer;
        StringBuffer secondBuffer;
        StringBuffer thirdBuffer;

        std::shared_ptr<RoomCategory> roomCategory = sRoomMgr->GetRoomCategory(categoryId);

        RoomsMap* rooms = sRoomMgr->GetRooms();
        uint32 nowVisitors = 0;
        uint32 maxVisitors = 0;
        uint32 guestRooms = 0;
        for (auto& itr = rooms->begin(); itr != rooms->end(); itr++)
        {
            std::shared_ptr<RoomCategory> category = sRoomMgr->GetRoomCategory(itr->second->GetCategory());

            if (hideFullRooms)
                if (itr->second->GetVisitorsNow() == itr->second->GetVisitorsMax())
                    continue;

            if (itr->second->GetCategory() == categoryId)
            {
                nowVisitors += itr->second->GetVisitorsNow();
                maxVisitors += itr->second->GetVisitorsMax();

                if (category->IsPublicSpace())
                {
                    thirdBuffer.AppendWired(itr->second->GetId());
                    thirdBuffer.AppendWired(1);
                    thirdBuffer.AppendString(itr->second->GetName());
                    thirdBuffer.AppendWired(itr->second->GetVisitorsNow());
                    thirdBuffer.AppendWired(itr->second->GetVisitorsMax());
                    thirdBuffer.AppendWired(itr->second->GetCategory());
                    thirdBuffer.AppendString(itr->second->GetDescription());
                    thirdBuffer.AppendWired(itr->second->GetId());
                    thirdBuffer.AppendWired(0);
                    thirdBuffer.AppendString(itr->second->GetCcts());
                    thirdBuffer.AppendWired(0);
                    thirdBuffer.AppendWired(1);
                }
                else
                {
                    thirdBuffer.AppendWired(itr->second->GetId());
                    thirdBuffer.AppendString(itr->second->GetName());
                    thirdBuffer.AppendString(itr->second->GetOwnerName());
                    thirdBuffer.AppendString(itr->second->GetAccessType());
                    thirdBuffer.AppendWired(itr->second->GetVisitorsMax());
                    thirdBuffer.AppendString(itr->second->GetDescription());
                    guestRooms++;
                }
            }
        }

        if (!roomCategory->IsPublicSpace())
            secondBuffer.AppendWired(guestRooms);

        buffer.AppendBase64(OpcodesServer::SMSG_NAVIGATE_NODE_INFO);
        buffer.AppendWiredBool(hideFullRooms);
        buffer.AppendWired(categoryId);
        buffer.AppendWired(roomCategory->IsPublicSpace() ? 0 : 2);
        buffer.AppendString(roomCategory->GetName());
        buffer.AppendWired(nowVisitors);
        buffer.AppendWired(maxVisitors);
        buffer.AppendWired(roomCategory->GetParentCategory());

        buffer.Append(secondBuffer);
        buffer.Append(thirdBuffer);

        RoomCategoriesMap* categories = sRoomMgr->GetRoomCategories();
        for (auto& itr = categories->begin(); itr != categories->end(); itr++)
        {
            if (itr->second->GetParentCategory() != categoryId)
                continue;

            buffer.AppendWired(itr->second->GetCategory());
            buffer.AppendWired(0);
            buffer.AppendString(itr->second->GetName());
            buffer.AppendWired(nowVisitors);
            buffer.AppendWired(maxVisitors);
            buffer.AppendWired(categoryId);
        }

        buffer.AppendSOH();
        Write((const char*)buffer.GetContents(), buffer.GetSize());
    }
    //-----------------------------------------------//
    void PlayerSocket::HandleGetUserFlatsCatergors(std::unique_ptr<Packet> packet)
    {
        RoomCategoriesMap* categories = sRoomMgr->GetRoomCategories();

        StringBuffer buffer;
        buffer.Append(categories->size());
        for (auto itr = categories->begin(); itr != categories->end(); itr++)
        {
            buffer.AppendWired(itr->second->GetCategory());
            buffer.AppendString(itr->second->GetName());
        }
        buffer.AppendSOH();
        Write((const char*)buffer.GetContents(), buffer.GetSize());
    }
    //-----------------------------------------------//
    void PlayerSocket::HandleGetFavouriteRooms(std::unique_ptr<Packet> packet)
    {
        uint32 roomId = packet->ReadBase64Uint();
        std::vector<std::shared_ptr<FavouriteRooms>> favouriteRooms = sRoomMgr->GetFavouriteRooms(mPlayer->GetId());

        StringBuffer buffer;
        StringBuffer secondBuffer;

        uint32 privateFlatSize = 0;
        buffer.AppendBase64(OpcodesServer::SMSG_FAVOURITE_ROOMS_RESULT);
        buffer.AppendWired(0);
        buffer.AppendWired(0);
        buffer.AppendWired(2);
        buffer.AppendString("");
        buffer.AppendWired(0);
        buffer.AppendWired(0);
        buffer.AppendWired(0);

        if (!favouriteRooms.empty())
        {
            for (auto& itr = favouriteRooms.begin(); itr != favouriteRooms.end(); itr++)
            {
                std::shared_ptr<Room> room = sRoomMgr->GetRoom((*itr)->GetRoomId());

                if (!(*itr)->IsPublicSpace())
                {
                    secondBuffer.AppendWired(room->GetId());
                    secondBuffer.AppendString(room->GetName());
                    secondBuffer.AppendString(room->GetOwnerName());
                    secondBuffer.AppendString(room->GetAccessType());
                    secondBuffer.AppendWired(room->GetVisitorsNow());
                    secondBuffer.AppendWired(room->GetVisitorsMax());
                    secondBuffer.AppendString(room->GetDescription());
                    privateFlatSize++;
                }
                else
                {
                    secondBuffer.AppendWired(room->GetId());
                    secondBuffer.AppendWired(1);
                    secondBuffer.AppendString(room->GetName());
                    secondBuffer.AppendWired(room->GetVisitorsNow());
                    secondBuffer.AppendWired(room->GetVisitorsMax());
                    secondBuffer.AppendWired(room->GetCategory());
                    secondBuffer.AppendString(room->GetDescription());
                    secondBuffer.AppendWired(room->GetId());
                    secondBuffer.AppendWired(0);
                    secondBuffer.AppendString(room->GetCcts());
                    secondBuffer.AppendWired(0);
                    secondBuffer.AppendWired(1);
                }
            }
        }

        buffer.AppendWired(privateFlatSize);
        buffer.Append(secondBuffer);

        buffer.AppendSOH();
        SendPacket((const char*)buffer.GetContents(), buffer.GetSize());

    }
    //-----------------------------------------------//
    void PlayerSocket::HandleAddFavouriteRoom(std::unique_ptr<Packet> packet)
    {
        bool roomType = packet->ReadBool();
        uint32 roomId = packet->ReadWiredUint();

        sRoomMgr->AddFavouriteRoom(mPlayer->GetId(), roomType, roomId);
    }
    //-----------------------------------------------//
    void PlayerSocket::HandleDeleteFavouriteRoom(std::unique_ptr<Packet> packet)
    {
        bool roomType = packet->ReadBool();
        uint32 roomId = packet->ReadWiredUint();

        sRoomMgr->DeleteFavouriteRoom(mPlayer->GetId(), roomId);
    }
    //-----------------------------------------------//
    void PlayerSocket::HandleSearchRooms(std::unique_ptr<Packet> packet)
    {
        std::string search = packet->ReadString();

        RoomsMap* rooms = sRoomMgr->GetRooms();

        StringBuffer buffer;
        buffer.AppendBase64(OpcodesServer::SMSG_SEARCH_RESULTS);
        for (auto& itr = rooms->begin(); itr != rooms->end(); itr++)
        {
            std::shared_ptr<Room> room = itr->second;

            // TODO fix this structure
            if (room->GetName().find(search) != std::string::npos)
            {
                buffer.AppendWired(room->GetId());
                buffer.AppendString(room->GetName());
                buffer.AppendString(room->GetOwnerName());
                buffer.AppendString(room->GetAccessType());
                buffer.AppendString("x");
                buffer.AppendWired(room->GetVisitorsNow());
                buffer.AppendWired(room->GetVisitorsMax());
                buffer.AppendString("null");
                buffer.AppendString(room->GetDescription());
                buffer.AppendString(room->GetDescription());
            }
        }

        buffer.AppendSOH();
        SendPacket((const char*)buffer.GetContents(), buffer.GetSize());
    }
    //-----------------------------------------------//
    void PlayerSocket::HandleGetInterest(std::unique_ptr<Packet> packet)
    {
        StringBuffer buffer;
        buffer.AppendBase64(OpcodesServer::SMSG_ROOM_INTEREST);
        buffer.AppendString("0");
        buffer.AppendSOH();
        SendPacket((const char*)buffer.GetContents(), buffer.GetSize());
    }
    //-----------------------------------------------//
}