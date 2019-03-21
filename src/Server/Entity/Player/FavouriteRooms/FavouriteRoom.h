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

#ifndef _Quad_FavouriteRooms_h_
#define _Quad_FavouriteRooms_h_
#include "Common/SharedDefines.h"
#include "Network/StringBuffer.h"

namespace Quad
{
    typedef struct FavouriteRoomsStruct
    {
    public:
        friend class FavouriteRoom;

    public:
        FavouriteRoomsStruct() : mId(0), mRoomId(0), mPublicSpace(false) {}
        ~FavouriteRoomsStruct() {}

    public:
        uint32 GetId()          const { return mId; }
        uint32 GetRoomId()      const { return mRoomId; }
        bool IsPublicSpace()    const { return mPublicSpace; }

    private:
        uint32 mId;
        uint32 mRoomId;
        bool mPublicSpace;

    }FavouriteRoomsData;

    typedef std::vector<FavouriteRoomsData> FavouriteRoomsVector;

    class FavouriteRoom
    {
    public:
        explicit FavouriteRoom(const uint32& id);
        ~FavouriteRoom();

    public:
        void LoadFavouriteRooms();

        void ParseSendFavouriteRooms(StringBuffer& buffer);

        void AddFavouriteRoom(const bool& isPublic, const uint32& roomId);
        void RemoveFavouriteRoom(const uint32& roomId);

        void UpdateFavouriteRooms();

    private:
        FavouriteRoomsVector mFavouriteRooms;
        FavouriteRoomsVector mDeletedFavouriteRooms;
        uint32 mId;
    };
}

#endif /* _Quad_FavouriteRooms_h_ */