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
#include "FavouriteRoom.h"
#include "Database/QueryDatabase.h"
#include "RoomManager.h"
#include "Config/Config.h"
//-----------------------------------------------//
namespace Quad
{
    //-----------------------------------------------//
    FavouriteRoom::FavouriteRoom(const uint32& id) : mId(id)
    {
    }
    //-----------------------------------------------//
    FavouriteRoom::~FavouriteRoom()
    {
        mFavouriteRooms.clear();
        mDeletedFavouriteRooms.clear();
    }
    //-----------------------------------------------//
    void FavouriteRoom::LoadFavouriteRooms()
    {
        mFavouriteRooms.clear();

        QueryDatabase database("rooms");
        database.PrepareQuery("SELECT id, room_id, public_space FROM favourite_rooms WHERE id = ?");
        database.GetStatement()->setUInt(1, mId);
        database.ExecuteQuery();

        if (!database.GetResult())
            return;

        Result* result = database.Fetch();

        do
        {
            FavouriteRoomsData favouriteRoom;
            favouriteRoom.mId = result->GetUint32(1);
            favouriteRoom.mRoomId = result->GetUint32(2);
            favouriteRoom.mPublicSpace = result->GetBool(3);

            mFavouriteRooms.push_back(favouriteRoom);

        } while (result->GetNextResult());
    }
    //-----------------------------------------------//
    void FavouriteRoom::ParseSendFavouriteRooms(StringBuffer& buffer)
    {
        StringBuffer secondBuffer;

        uint32 privateFlatSize = 0;
        buffer.AppendWired(0);
        buffer.AppendWired(0);
        buffer.AppendWired(2);
        buffer.AppendString("");
        buffer.AppendWired(0);
        buffer.AppendWired(0);
        buffer.AppendWired(0);

        if (!mFavouriteRooms.empty())
        {
            for (FavouriteRoomsVector::iterator& itr = mFavouriteRooms.begin(); itr != mFavouriteRooms.end(); itr++)
            {
                const FavouriteRoomsData& favouriteRoom = *itr;
                std::shared_ptr<Room> room = sRoomMgr->GetRoom(itr->GetRoomId());

                if (!room)
                    continue;

                if (favouriteRoom.IsPublicSpace())
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
    }
    //-----------------------------------------------//
    void FavouriteRoom::AddFavouriteRoom(const bool& isPublic, const uint32& roomId)
    {
        for (FavouriteRoomsVector::iterator& itr = mFavouriteRooms.begin(); itr != mFavouriteRooms.end(); itr++)
        {
            const FavouriteRoomsData& favouriteRoom = *itr;

            if (mFavouriteRooms.size() >= sConfig->GetIntDefault("MaxFavouriteRooms", 50))
                return;

            if (favouriteRoom.GetRoomId() == roomId)
                return;
        }

        FavouriteRoomsData favouriteRoom;
        favouriteRoom.mId = mId;
        favouriteRoom.mPublicSpace = isPublic;
        favouriteRoom.mRoomId = roomId;

        mFavouriteRooms.push_back(favouriteRoom);
    }
    //-----------------------------------------------//
    void FavouriteRoom::RemoveFavouriteRoom(const uint32& roomId)
    {
        for (FavouriteRoomsVector::iterator& itr = mFavouriteRooms.begin(); itr != mFavouriteRooms.end(); itr++)
        {
            const FavouriteRoomsData& favouriteRoom = *itr;

            if (favouriteRoom.GetRoomId() == roomId)
            {
                mDeletedFavouriteRooms.push_back(favouriteRoom);
                mFavouriteRooms.erase(itr);
                return;
            }
        }
    }
    //-----------------------------------------------//
    void FavouriteRoom::UpdateFavouriteRooms()
    {
        QueryDatabase database("rooms");
        for (FavouriteRoomsVector::iterator& itr = mFavouriteRooms.begin(); itr != mFavouriteRooms.end(); itr++)
        {
            const FavouriteRoomsData& favouriteRoom = *itr;

            database.PrepareQuery("INSERT INTO favourite_rooms (id, room_id, public_space) VALUES (?, ?, ?) ON DUPLICATE KEY UPDATE room_id = ?");
            database.GetStatement()->setUInt(1, favouriteRoom.GetId());
            database.GetStatement()->setUInt(2, favouriteRoom.GetRoomId());
            database.GetStatement()->setBoolean(3, favouriteRoom.IsPublicSpace());
            database.GetStatement()->setUInt(4, favouriteRoom.GetRoomId());
            database.ExecuteQuery();
        }

        for (FavouriteRoomsVector::iterator& itr = mDeletedFavouriteRooms.begin(); itr != mDeletedFavouriteRooms.end(); itr++)
        {
            FavouriteRoomsData favouriteRoom = *itr;

            database.PrepareQuery("DELETE FROM favourite_rooms WHERE room_id = ?");
            database.GetStatement()->setUInt(1, favouriteRoom.GetRoomId());
            database.ExecuteQuery();
        }
    }
    //-----------------------------------------------//
}
//-----------------------------------------------//