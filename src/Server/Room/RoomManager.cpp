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
#include <boost/multi_array.hpp>
#include "RoomManager.h"
#include "Database/QueryDatabase.h"
#include "World.h"
#include "ItemManager.h"
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
    void RoomManager::LoadRoomCategories()
    {
        QueryDatabase database("rooms");
        database.PrepareQuery("SELECT id, parent_id, name, public_spaces, allow_trading, minrole_access, minrole_setflatcat FROM rooms_categories");
        database.ExecuteQuery();

        if (!database.GetResult())
        {
            LOG_ERROR << "room_categories is empty!";
            return;
        }

        Result* result = database.Fetch();

        do
        {
            RoomCategoryData& roomCategories = mRoomCategories[result->GetInt32(1)];

            roomCategories.mCategoryId     = result->GetUint32(1);
            roomCategories.mParentId       = result->GetUint32(2);
            roomCategories.mName           = result->GetString(3);
            roomCategories.mPublicSpace    = result->GetBool(4);
            roomCategories.mAllowTrading   = result->GetUint16(5);
            roomCategories.mMinRoleAccess  = result->GetBool(6);
            roomCategories.mMinRoleSetFlat = result->GetBool(7);

        } while (result->GetNextResult());

        LOG_INFO << "Loaded " << mRoomCategories.size() << " Room Categories";
    }
    //-----------------------------------------------//
    void RoomManager::LoadRooms()
    {
        QueryDatabase database("rooms");
        database.PrepareQuery("SELECT id, owner_id, owner_name, category, name, description, model, ccts, wall_paper, floor, show_name, super_users, access_type, password, visitors_now, visitors_max, rating FROM rooms");
        database.ExecuteQuery();

        if (!database.GetResult())
        {
            LOG_ERROR << "hotel_rooms is empty!";
            return;
        }

        Result* result = database.Fetch();

        do
        {
            std::unique_ptr<Room> room = std::make_unique<Room>();
            room->mId                      = result->GetUint32(1);
            room->mOwnerId                 = result->GetUint32(2);
            room->mOwnerName               = result->GetString(3);
            room->mCategory                = result->GetUint32(4);
            room->mName                    = result->GetString(5);
            room->mDescription             = result->GetString(6);
            room->mModel                   = result->GetString(7);
            room->mCcts                    = result->GetString(8);
            room->mWallPaper               = result->GetUint32(9);
            room->mFloor                   = result->GetUint32(10);
            room->mShowName                = result->GetBool(11);
            room->mSuperUsers              = result->GetBool(12);
            room->mAccessType              = result->GetString(13);
            room->mPassword                = result->GetString(14);
            room->mVisitorsNow             = result->GetUint32(15);
            room->mVisitorsMax             = result->GetUint32(16);
            room->mRoomModel               = *GetRoomModel(room->GetModel());

            mRooms[room->GetId()] = std::move(room);

        } while (result->GetNextResult());

        LOG_INFO << "Loaded " << mRooms.size() << " Hotel Rooms";
    }
    //-----------------------------------------------//
    void RoomManager::LoadRoomModels()
    {
        QueryDatabase database("rooms");
        database.PrepareQuery("SELECT id, model_id, model_name, door_x, door_y, door_z, door_dir, height_map from room_models");
        database.ExecuteQuery();

        if (!database.GetResult())
        {
            LOG_ERROR << "height_map is empty!";
            return;
        }

        Result* result = database.Fetch();

        do
        {
            RoomModelsData& roomModel = mRoomModels[result->GetString(2)];
            roomModel.mId = result->GetUint32(1);
            roomModel.mModelId = result->GetString(2);
            roomModel.mModel = result->GetString(3);
            roomModel.mDoorX = result->GetInt32(4);
            roomModel.mDoorY = result->GetInt32(5);
            roomModel.mDoorZ = result->GetFloat(6);
            roomModel.mDoorOrientation = result->GetInt32(7);
            roomModel.mHeightMap = result->GetString(8);
            
            boost::replace_all(roomModel.mHeightMap, "|", "\r");

        } while (result->GetNextResult());


        LOG_INFO << "Loaded " << mRoomModels.size() << " Room Models";
    }
    //-----------------------------------------------//
    RoomCategoryData* RoomManager::GetRoomCategory(const uint32& id)
    {
        RoomCategoriesMap::iterator dataItr = mRoomCategories.find(id);
        if (dataItr != mRoomCategories.end())
            return &dataItr->second;
        else
            return nullptr;
    }
    //-----------------------------------------------//
    std::shared_ptr<Room> RoomManager::GetRoom(const uint32& id)
    {
        RoomsMap::const_iterator itr = mRooms.find(id);
        if (itr != mRooms.end())
            return itr->second;
        else
            return nullptr;
    }
    //-----------------------------------------------//
    RoomModelsData* RoomManager::GetRoomModel(const std::string& model)
    {
        RoomModelsMap::iterator itr = mRoomModels.find(model);
        if (itr != mRoomModels.end())
            return &itr->second;
        else
            return nullptr;
    }
    //-----------------------------------------------//
    RoomCategoriesMap* RoomManager::GetRoomCategories()
    {
        return &mRoomCategories;
    }
    //-----------------------------------------------//
    RoomsMap* RoomManager::GetRooms()
    {
        return &mRooms;
    }
    //-----------------------------------------------//
}
//-----------------------------------------------//