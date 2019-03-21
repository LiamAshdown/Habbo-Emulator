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
    //-----------------------------------------------//
    void PlayerSocket::HandleNavigate(std::unique_ptr<Packet> packet)
    {
        bool hideFullRooms = packet->ReadWiredBool();
        int32 categoryId = packet->ReadWiredInt();

        StringBuffer buffer;
        StringBuffer secondBuffer;
        StringBuffer thirdBuffer;

        RoomCategoryData* roomCategory = sRoomMgr->GetRoomCategory(categoryId);

        RoomsMap* rooms = sRoomMgr->GetRooms();
        uint32 nowVisitors = 0;
        uint32 maxVisitors = 0;
        uint32 guestRooms = 0;

        for (RoomsMap::iterator& itr = rooms->begin(); itr != rooms->end(); itr++)
        {
            RoomCategoryData* category = sRoomMgr->GetRoomCategory(itr->second->GetCategory());
            std::shared_ptr<Room> room = itr->second;

            if (hideFullRooms)
                if (room->GetVisitorsNow() == room->GetVisitorsMax())
                    continue;

            if (room->GetCategory() == categoryId)
            {
                nowVisitors += room->GetVisitorsNow();
                maxVisitors += room->GetVisitorsMax();

                if (category->IsPublicSpace())
                {
                    thirdBuffer.AppendWired(room->GetId() + ROOM_ID_OFFSET);
                    thirdBuffer.AppendWired(1);
                    thirdBuffer.AppendString(room->GetName());
                    thirdBuffer.AppendWired(room->GetVisitorsNow());
                    thirdBuffer.AppendWired(room->GetVisitorsMax());
                    thirdBuffer.AppendWired(room->GetCategory());
                    thirdBuffer.AppendString(room->GetDescription());
                    thirdBuffer.AppendWired(room->GetId());
                    thirdBuffer.AppendWired(0);
                    thirdBuffer.AppendString(room->GetCcts());
                    thirdBuffer.AppendWired(0);
                    thirdBuffer.AppendWired(1);
                }
                else
                {
                    thirdBuffer.AppendWired(room->GetId());
                    thirdBuffer.AppendString(room->GetName());
                    thirdBuffer.AppendString(room->GetOwnerName());
                    thirdBuffer.AppendString(room->GetAccessType());
                    thirdBuffer.AppendWired(room->GetVisitorsNow());
                    thirdBuffer.AppendWired(room->GetVisitorsMax());
                    thirdBuffer.AppendString(room->GetDescription());
                    guestRooms++;
                }
            }
        }

        if (!roomCategory->IsPublicSpace())
            secondBuffer.AppendWired(guestRooms);

        buffer.AppendBase64(PacketServerHeader::SERVER_NAVIGATE_NODE_INFO);
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
        for (RoomCategoriesMap::const_iterator& itr = categories->begin(); itr != categories->end(); itr++)
        {
            RoomCategoryData roomCategory = itr->second;

            if (roomCategory.GetParentCategory() != categoryId)
                continue;

            buffer.AppendWired(roomCategory.GetCategory());
            buffer.AppendWired(0);
            buffer.AppendString(roomCategory.GetName());
            buffer.AppendWired(nowVisitors);
            buffer.AppendWired(maxVisitors);
            buffer.AppendWired(categoryId);
        }

        buffer.AppendSOH();
        SendPacket(buffer);
    }
    //-----------------------------------------------//
    void PlayerSocket::HandleGetUserFlatsCategories(std::unique_ptr<Packet> packet)
    {
        RoomCategoriesMap* categories = sRoomMgr->GetRoomCategories();

        StringBuffer buffer;
        StringBuffer secondBuffer;
        uint32 flatCategorySize = 0;

        for (RoomCategoriesMap::const_iterator& itr = categories->begin(); itr != categories->end(); itr++)
        {
            RoomCategoryData roomCategory = itr->second;

            if (!roomCategory.IsPublicSpace())
            {
                secondBuffer.AppendWired(roomCategory.GetCategory());
                secondBuffer.AppendString(roomCategory.GetName());
                flatCategorySize++;
            }
        }
        buffer.AppendBase64(PacketServerHeader::SERVER_USER_FLAT_CATEGORIES);
        buffer.Append(flatCategorySize);
        buffer.Append(secondBuffer);
        buffer.AppendSOH();
        SendPacket(buffer);
    }
    //-----------------------------------------------//
    void PlayerSocket::HandleSearchRooms(std::unique_ptr<Packet> packet)
    {
        std::string search = packet->ReadString();

        RoomsMap* rooms = sRoomMgr->GetRooms();

        StringBuffer buffer;
        buffer.AppendBase64(PacketServerHeader::SERVER_SEARCH_RESULTS);
        for (RoomsMap::const_iterator& itr = rooms->begin(); itr != rooms->end(); itr++)
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
        SendPacket(buffer);
    }
    //-----------------------------------------------//
}
//-----------------------------------------------//