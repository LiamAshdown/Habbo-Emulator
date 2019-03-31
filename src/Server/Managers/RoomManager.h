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

#ifndef _MANAGERS_ROOM_MANAGER_h
#define _MANAGERS_ROOM_MANAGER_h
#include "Common/SharedDefines.h"
#include "Room.h"
#include <mutex>
#endif /* _MANAGERS_ROOM_MANAGER_h */

namespace SteerStone
{
    typedef std::unordered_map<uint32, RoomCategory> RoomCategoriesMap;
    typedef std::unordered_map<uint32, std::shared_ptr<Room>> RoomsMap;
    typedef std::unordered_map<std::string, RoomModel> RoomModelsMap;

    /// This class stores information about all rooms, models and heightmap
    /// Singleton class
    class RoomManager
    {
    public:
        static RoomManager* instance();

    public:
        /// Constructor
        RoomManager();

        /// Constructor
        ~RoomManager();

    public:
        /// LoadRoomCategories
        /// Load room categories from database
        void LoadRoomCategories();

        /// LoadRoomModels
        /// Load room models from database
        void LoadRoomModels();

        /// LoadRooms
        /// Load rooms from database
        void LoadRooms();

        /// UpdateRooms
        /// Update all active rooms
        /// @p_Diff - Update tick
        void UpdateRooms(uint32 const& p_Diff);

        /// GetRoomCategory
        /// @p_Id : Category Id
        RoomCategory* GetRoomCategory(uint32 const& p_Id);

        /// GetRoomCategory
        /// @p_Id : Model Id
        RoomModel* GetRoomModel(std::string const& p_Model);
       
        /// GetRoomCategory
        /// @p_Id : Room Id
        std::shared_ptr<Room> GetRoom(uint32 const& p_Id);

        /// GetRoomCategories
        /// Get Room Category Map
        RoomCategoriesMap* GetRoomCategories();

        /// GetRooms
        /// Get Room Map
        RoomsMap* GetRooms();

    private:
        RoomCategoriesMap m_RoomCategories;                          ///< Map Storage which holds key category Id and structure for category
        RoomsMap m_Rooms;                                            ///< Map Storage which holds key room Id and structure for room                               
        RoomModelsMap m_RoomModels;                                  ///< Map Storage which holds key Model Id and structure for model
        std::mutex m_Mutex;                                          ///< Mutex
    };
}

#define sRoomMgr SteerStone::RoomManager::instance()
