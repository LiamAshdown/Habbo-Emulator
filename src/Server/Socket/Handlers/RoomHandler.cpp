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
        bool hideFullRooms = packet->ReadWiredBool();
        int32 categoryId = packet->ReadWiredInt();

        StringBuffer buffer;
        StringBuffer secondBuffer;
        StringBuffer thirdBuffer;

        std::shared_ptr<RoomCategory> roomCategory = sRoomMgr->GetRoomCategory(categoryId);

        RoomsMap* rooms = sRoomMgr->GetRooms();
        uint32 nowVisitors = 0;
        uint32 maxVisitors = 0;
        uint32 guestRooms = 0;

        for (RoomsMap::const_iterator& itr = rooms->begin(); itr != rooms->end(); itr++)
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
                    thirdBuffer.AppendWired(itr->second->GetId() + ROOM_ID_OFFSET);
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
    void PlayerSocket::HandleGetUserFlatsCategories(std::unique_ptr<Packet> packet)
    {
        RoomCategoriesMap* categories = sRoomMgr->GetRoomCategories();

        StringBuffer buffer;
        StringBuffer secondBuffer;
        uint32 flatCategorySize = 0;

        for (RoomCategoriesMap::const_iterator& itr = categories->begin(); itr != categories->end(); itr++)
        {
            if (!itr->second->IsPublicSpace())
            {
                secondBuffer.AppendWired(itr->second->GetCategory());
                secondBuffer.AppendString(itr->second->GetName());
                flatCategorySize++;
            }
        }
        buffer.AppendBase64(PacketServerHeader::SERVER_USER_FLAT_CATEGORIES);
        buffer.Append(flatCategorySize);
        buffer.Append(secondBuffer);
        buffer.AppendSOH();
        Write((const char*)buffer.GetContents(), buffer.GetSize());
    }
    //-----------------------------------------------//
    void PlayerSocket::HandleGetFavouriteRooms(std::unique_ptr<Packet> packet)
    {
        uint32 roomId = packet->ReadBase64Uint();
        FavouriteRoomsVector favouriteRooms = sRoomMgr->GetFavouriteRooms(mPlayer->GetId());

        StringBuffer buffer;
        StringBuffer secondBuffer;

        uint32 privateFlatSize = 0;
        buffer.AppendBase64(PacketServerHeader::SERVER_FAVOURITE_ROOMS_RESULT);
        buffer.AppendWired(0);
        buffer.AppendWired(0);
        buffer.AppendWired(2);
        buffer.AppendString("");
        buffer.AppendWired(0);
        buffer.AppendWired(0);
        buffer.AppendWired(0);

        if (!favouriteRooms.empty())
        {
            for (FavouriteRoomsVector::const_iterator itr = favouriteRooms.begin(); itr != favouriteRooms.end(); itr++)
            {
                std::shared_ptr<Room> room = sRoomMgr->GetRoom((*itr)->GetRoomId());

                if (!room)
                    continue;

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
                    secondBuffer.AppendWired(room->GetId() + ROOM_ID_OFFSET);
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
        bool roomType = packet->ReadWiredBool();
        uint32 roomId = packet->ReadWiredUint() - ROOM_ID_OFFSET;

        sRoomMgr->AddFavouriteRoom(mPlayer->GetId(), roomType, roomId);
    }
    //-----------------------------------------------//
    void PlayerSocket::HandleDeleteFavouriteRoom(std::unique_ptr<Packet> packet)
    {
        bool roomType = packet->ReadWiredBool();
        uint32 roomId = packet->ReadWiredUint() - ROOM_ID_OFFSET;

        sRoomMgr->DeleteFavouriteRoom(mPlayer->GetId(), roomId);
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
        SendPacket((const char*)buffer.GetContents(), buffer.GetSize());
    }
    //-----------------------------------------------//
    void PlayerSocket::HandleGetInterest(std::unique_ptr<Packet> packet)
    {
        StringBuffer buffer;
        buffer.AppendBase64(PacketServerHeader::SERVER_ROOM_INTEREST);
        buffer.AppendStringDelimiter("", "\t");
        buffer.AppendString("", false);
        buffer.AppendSOH();
        SendPacket((const char*)buffer.GetContents(), buffer.GetSize());
    }
    //-----------------------------------------------//
    void PlayerSocket::HandleRoomDirectory(std::unique_ptr<Packet> packet)
    {
        bool isPublic = packet->ReadBase64Bool();
        uint32 roomId = packet->ReadWiredUint();

        StringBuffer buffer;

        buffer.AppendBase64(PacketServerHeader::SERVER_OPEN_CONNECTION);
        buffer.AppendSOH();
        SendPacket((const char*)buffer.GetContents(), buffer.GetSize());

        buffer.Clear();

        buffer.AppendBase64(PacketServerHeader::SERVER_ROOM_URL);
        buffer.AppendString("/client/");
        buffer.AppendSOH();
        SendPacket((const char*)buffer.GetContents(), buffer.GetSize());

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
        SendPacket((const char*)buffer.GetContents(), buffer.GetSize());
    }
    //-----------------------------------------------//
    void PlayerSocket::HandleGetRoomAdd(std::unique_ptr<Packet> packet)
    {
        StringBuffer buffer;
        buffer.AppendBase64(PacketServerHeader::SERVER_ROOM_ADD);
        buffer.AppendString("0", false);
        buffer.AppendSOH();
        SendPacket((const char*)buffer.GetContents(), buffer.GetSize());
    }
    //-----------------------------------------------//
    void PlayerSocket::HandleGetRoomHeight(std::unique_ptr<Packet> packet)
    {
        StringBuffer buffer;
        buffer.AppendBase64(PacketServerHeader::SERVER_ROOM_HEIGHT_MAP);
        std::string heightMap = "xxxxxxxxxxxxxxxx000000\r\nxxxxx0xxxxxxxxxx000000\r\nxxxxx00000000xxx000000\r\nxxxxx000000000xx000000\r\n0000000000000000000000\r\n0000000000000000000000\r\n0000000000000000000000\r\n0000000000000000000000\r\n0000000000000000000000\r\nxxxxx000000000000000xx\r\nxxxxx000000000000000xx\r\nx0000000000000000000xx\r\nx0000000000000000xxxxx\r\nxxxxxx00000000000xxxxx\r\nxxxxxxx0000000000xxxxx\r\nxxxxxxxx000000000xxxxx\r\nxxxxxxxx000000000xxxxx\r\nxxxxxxxx000000000xxxxx\r\nxxxxxxxx000000000xxxxx\r\nxxxxxxxx000000000xxxxx\r\nxxxxxxxx000000000xxxxx\r\nxxxxxx00000000000xxxxx\r\nxxxxxx00000000000xxxxx\r\nxxxxxx00000000000xxxxx\r\nxxxxxx00000000000xxxxx\r\nxxxxxx00000000000xxxxx\r\nxxxxx000000000000xxxxx\r\nxxxxx000000000000xxxxx\r\n";
        buffer.AppendString(heightMap, false);
        buffer.AppendSOH();
        SendPacket((const char*)buffer.GetContents(), buffer.GetSize());
    }
    //-----------------------------------------------//
    void PlayerSocket::HandleRoomUsers(std::unique_ptr<Packet> packet)
    {
        // TODO
        StringBuffer buffer;
        buffer.AppendBase64(PacketServerHeader::SERVER_ROOM_USER_OBJECTS);
        buffer.AppendSOH();
        SendPacket((const char*)buffer.GetContents(), buffer.GetSize());
    }
    //-----------------------------------------------//
}