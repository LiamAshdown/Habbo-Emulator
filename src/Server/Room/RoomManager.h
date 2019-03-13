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

#ifndef _Quad_RoomManager_h_
#define _Quad_RoomManager_h_
#include <mutex>
#include "Common/SharedDefines.h"
#include "Room.h"

namespace Quad
{
    typedef std::map<uint32, std::shared_ptr<RoomCategory>> RoomCategoriesMap;
    typedef std::map<uint32, std::shared_ptr<Room>> RoomsMap;
    typedef std::map<uint32, std::vector<std::shared_ptr<FavouriteRooms>>> FavouriteRoomsMap;

    class RoomManager
    {
    public:
        static RoomManager* instance();

    public:
        RoomManager();
        ~RoomManager();

    public:
        void LoadRoomCategories();
        void LoadRooms();
        void LoadFavouriteRooms();

        std::shared_ptr<RoomCategory> GetRoomCategory(const uint32 id);
        std::shared_ptr<Room> GetRoom(const uint32 id);
        std::vector<std::shared_ptr<FavouriteRooms>> GetFavouriteRooms(const uint32& Id);
            
        RoomCategoriesMap* GetRoomCategories();
        RoomsMap* GetRooms();

        void AddFavouriteRoom(const uint32& accountId, const bool& isPublic, const uint32& roomId);
        void DeleteFavouriteRoom(const uint32& accountId, const uint32& roomId);

    private:
        RoomCategoriesMap mRoomCategories;
        RoomsMap mRooms;
        FavouriteRoomsMap mFavouriteRooms;

    private:
        std::mutex mMutex;
    };
}

#define sRoomMgr Quad::RoomManager::instance()

#endif /* _Quad_RoomManager_h_ */