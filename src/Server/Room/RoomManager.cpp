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
#include "RoomManager.h"
#include "Database/QueryDatabase.h"
#include "World.h"
//-----------------------------------------------//
namespace Quad
{
    //-----------------------------------------------//
    RoomManager* RoomManager::instance()
    {
        static RoomManager instance;
        return &instance;
    }
    //-----------------------------------------------//
    RoomManager::RoomManager()
    {
    }
    //-----------------------------------------------//
    RoomManager::~RoomManager()
    {
    }
    //-----------------------------------------------//
    void RoomManager::LoadRooms()
    {
        QueryDatabase database("rooms");
        database.PrepareQuery("SELECT id, name, description, password, owner_name, floor_level, model,  type, enabled, show_owner, super_user, max_in FROM hotel_rooms");
        database.ExecuteResultPrepareQuery();

        if (!database.GetExecuteQueryResult())
        {
            LOG_ERROR << "hotel_rooms database is empty!";
            return;
        }

        Field* fields = database.Fetch();

        do
        {
            std::shared_ptr<Room> room = std::make_shared<Room>();
            room->mId = fields->GetUint32(1);
            room->mName = fields->GetString(2);
            room->mDescription = fields->GetString(3);
            room->mPassword = fields->GetString(4);
            room->mOwnerName = fields->GetString(5);
            room->mFloorLevel = fields->GetString(6);
            room->mModel = fields->GetString(7);
            room->mType = fields->GetBool(8);
            room->mEnabled = fields->GetBool(9);
            room->mShowOwnerName = fields->GetBool(10);
            room->mSuperUser = fields->GetBool(11);
            room->mMaxIn = fields->GetUint32(12);

            ///< IMPORTANT!
            // Each room has its own port. Due to the way Sulake coded it's habbo client, the habbo client
            // connects to each room on its own port. So in order to replicate this, each time we create
            // a new room, we add the default server port (37120) + the room Id. For example:
            // 37120 + 1 = room port
            mRooms[DEFAULT_SERVER_PORT + room->mId] = room;

        } while (fields->GetNextResult());

        LOG_INFO << "Sucessfully loaded Hotel Rooms!";
    }
    //-----------------------------------------------//
    std::shared_ptr<Room> RoomManager::GetRoom(uint32 Id)
    {
        std::lock_guard<std::mutex> guard(mMutex);

        RoomMap::const_iterator itr = mRooms.find(DEFAULT_SERVER_PORT + Id);
        if (itr != mRooms.end())
            return itr->second;
        else
        {
            LOG_ERROR << "Room Id: " << Id << " does not exist!";
            return nullptr;
        }
    }
    //-----------------------------------------------//
    void RoomManager::AddRoom(std::shared_ptr<Room> room)
    {
        std::lock_guard<std::mutex> guard(mMutex);

        if (mRooms.count(DEFAULT_SERVER_PORT + room->GetId()))
        {
            LOG_ERROR << "Room Id: " << room->GetId() << " already exists!";
            return;
        }

        mRooms[DEFAULT_SERVER_PORT + room->GetId()] = room;

        sWorld->AddListener(DEFAULT_SERVER_PORT + room->GetId());
    }
    //-----------------------------------------------//
    void RoomManager::RemoveRoom(std::shared_ptr<Room> room)
    {
        std::lock_guard<std::mutex> guard(mMutex);

        RoomMap::const_iterator itr = mRooms.find(DEFAULT_SERVER_PORT + room->GetId());
        if (itr != mRooms.end())
            mRooms.erase(itr);
        else
        {
            LOG_ERROR << "Room Id: " << room->GetId() << " does not exist!";
            return;
        }

    }
    //-----------------------------------------------//
}
//-----------------------------------------------//