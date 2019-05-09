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

#pragma once
#include "Common/SharedDefines.h"
#include "Room.h"
#include <mutex>

namespace SteerStone
{
    /// Holds room Urls info
    /// Each room can only hold 1 url, keep note
    typedef struct RoomUrlStruct
    {
    public:
        uint32 Id;              ///< Room Id
        std::string ImageUrl;   ///< Image to display on billboard
        std::string LinkUrl;    ///< Link to direct user to when clicking on billboard

    }RoomUrlData;

    typedef std::unordered_map<uint32, RoomCategory> RoomCategoriesMap;
    typedef std::unordered_map<uint32, std::shared_ptr<Room>> RoomsMap;
    typedef std::unordered_map<std::string, RoomModel> RoomModelsMap;
    typedef std::map<uint32, RoomUrlData> RoomUrlMap;

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
        
        /// LoadRoomUrls
        /// Load room urls from database
        void LoadRoomUrls();

        /// LoadRoomWalkWays
        /// Load Walk way positions (used to walk into another room)
        void LoadRoomWalkWays();

        /// LoadRooms
        /// Load rooms from database
        void LoadRooms();

        /// LoadRoomRights
        /// Load room rights from database
        void LoadRoomRights();

        /// LoadVotedUsers
        /// Load room rating from database
        void LoadVotedUsers();
        
        /// AddRoom
        /// Add room to storage from database
        /// @p_RoomId : Id of room we are querying database to get room info
        void AddRoom(uint32 const p_RoomId);

        /// UpdateRooms
        /// Update all active rooms
        /// @p_Diff - Update tick
        void UpdateRooms(uint32 const& p_Diff);

        /// IsScheduledToDeleteRoom
        /// Check if we can delete the room
        void IsScheduledToDeleteRoom();

        /// GetRoomCategory
        /// @p_Id : Category Id
        RoomCategory* GetRoomCategory(uint32 const p_Id);

        /// GetRoomCategory
        /// @p_Id : Model Id
        RoomModel* GetRoomModel(std::string const p_Model);

        /// GetRoomUrl
        /// @p_Id : Room Id
        RoomUrlData* GetRoomUrl(uint32 const p_Id);

        /// GetWalkWay
        /// @p_Id : Model Id
        /// @p_X : Tile Position X
        /// @p_Y : Tile Position Y
        WalkWay* GetWalkWay(uint32 const p_Id, int16 const p_X, int16 p_Y);
       
        /// GetRoomCategory
        /// @p_Id : Room Id
        std::shared_ptr<Room> GetRoom(uint32 const p_Id);

        /// ReloadRoom
        /// Reload room from database
        /// @p_Id : Room Id
        void ReloadRoom(uint32 const p_Id);

        /// ScheduleDeleteRoom
        /// @p_RoomId : Room Id we are preparing to delete
        void ScheduleDeleteRoom(uint32 const p_RoomId);

        /// GetRoomCategories
        /// Get Room Category Map
        RoomCategoriesMap* GetRoomCategories();

        /// GetRooms
        /// Get Room Map
        RoomsMap* GetRooms();

    private:
        RoomCategoriesMap m_RoomCategories;                          ///< Map Storage which holds key category Id and structure for Category
        RoomsMap m_Rooms;                                            ///< Map Storage which holds key room Id and structure for Room                               
        RoomModelsMap m_RoomModels;                                  ///< Map Storage which holds key Model Id and structure for Model
        RoomUrlMap m_RoomUrl;                                        ///< Map storage which holds key Room Id and structure for RoomUrl
        std::vector<WalkWay> m_RoomWalkWays;                         ///< Stores Walk Way tiles
        std::vector<std::shared_ptr<Room>> m_RoomDeletion;           ///< Room which will get deleted when it's ready to do so
        boost::shared_mutex m_Mutex;
    };
}

#define sRoomMgr SteerStone::RoomManager::instance()
