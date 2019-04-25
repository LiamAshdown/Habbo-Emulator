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

#ifndef _HABBO_FAVOURITE_ROOM_h
#define _HABBO_FAVOURITE_ROOM_h
#include "Common/SharedDefines.h"
#endif /* _HABBO_FAVOURITE_ROOM_h */

namespace SteerStone
{
    class StringBuffer;
    class Habbo;

    /// Structure which holds information about room
    typedef struct FavouriteRoomsStruct
    {
    public:
        friend class FavouriteRoom;

    public:
        /// Constructor
        FavouriteRoomsStruct() : m_Id(0), m_RoomId(0), m_RoomType(false) {}

        /// Deconstructor
        ~FavouriteRoomsStruct() {}

    public:
        /// Get info about Room
        uint32 GetId()          const { return m_Id; }
        uint32 GetRoomId()      const { return m_RoomId; }
        bool GetRoomType()      const { return m_RoomType; }

    private:
        /// Variables
        uint32 m_Id;
        uint32 m_RoomId;
        bool m_RoomType;

    }FavouriteRoomsData;

    typedef std::vector<FavouriteRoomsData> FavouriteRoomsVector;

    /// This class is responsible for handling Habbo Favourite Rooms
    class FavouriteRoom
    {
    public:
        /// Constructor
        /// @p_Habbo : Habbo class which owns the favourite rooms
        explicit FavouriteRoom(Habbo* p_Habbo);
        
        /// Deconstructor
        ~FavouriteRoom();

    public:
        /// LoadFavouriteRooms - Load our Favourite Rooms from database
        void LoadFavouriteRooms();

        /// AddFavouriteRoom
        /// @p_IsPublic : Is Room public or flat
        /// @p_RoomId : Room Id
        void AddFavouriteRoom(bool const& p_IsPublic, uint32 const& p_RoomId);

        /// RemoveFavouriteRoom
        /// @p_RoomId : Room Id
        void RemoveFavouriteRoom(uint32 const& p_RoomId);

        /// SaveToDB - Update our favourite rooms
        void SaveToDB();

        /// ParseSendFavouriteRooms
        /// @p_Buffer : Buffer which is being parsed
        void ParseSendFavouriteRooms(StringBuffer& buffer);

    private:
        /// Variables
        FavouriteRoomsVector m_FavouriteRooms;                              ///< Vector storage which holds our favourite rooms
        FavouriteRoomsVector m_DeletedFavouriteRooms;                       ///< Vector storage which holds our deleted favourite rooms
        Habbo* m_Habbo;
    };
} ///< NAMESPACE STEERSTONE