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

#ifndef _Quad_Room_h_
#define _Quad_Room_h_
#include "Common/SharedDefines.h"
#include "Item.h"

enum RoomFlag
{
    ROOM_TYPE_PUBLIC                    = 0,
    ROOM_TYPE_FLATS                     = 2
};

namespace Quad
{
    class Player;

    typedef struct RoomCategoryStruct
    {
    public:
        friend class RoomManager;

    public:
        RoomCategoryStruct() {}
        ~RoomCategoryStruct() {}

    public:
        uint32 GetCategory() const { return mCategoryId; }
        std::string GetName() const { return mName; }
        bool IsPublicSpace() const { return mPublicSpace; }
        uint8 GetMinRoleAccess() const { return mMinRoleAccess; }
        uint8 GetMinRoleSetFlat() const { return mMinRoleSetFlat; }
        uint8 CanTrade() const { return mAllowTrading; }
        uint32 GetParentCategory() const { return mParentId; }

    private:
        uint32 mCategoryId;
        uint32 mParentId;
        std::string mName;
        bool mPublicSpace;
        bool mAllowTrading;
        uint16 mMinRoleAccess;
        uint16 mMinRoleSetFlat;

    }RoomCategory;

    typedef struct FavouriteRoomsStruct
    {
    public:
        friend class RoomManager;

    public:
        FavouriteRoomsStruct() : mId(0), mRoomId(0), mPublicSpace(false) {}
        FavouriteRoomsStruct(const FavouriteRoomsStruct& object)
        {
            mId = object.mId;
            mRoomId = object.mRoomId;
            mPublicSpace = object.mPublicSpace;

            LOG_INFO << "COPIED!";
        }
        ~FavouriteRoomsStruct() {}

    public:
        uint32 GetId() const { return mId; }
        uint32 GetRoomId() const { return mRoomId; }
        bool IsPublicSpace() const { return mPublicSpace; }

        void SetId(const uint32& id) { mId = id; }
        void SetRoomId(const uint32& roomId) { mRoomId = roomId; }
        void SetPublicSpace(const bool& isPublicSpace) { mPublicSpace = isPublicSpace; }

    private:
        uint32 mId;
        uint32 mRoomId;
        bool mPublicSpace;

    }FavouriteRooms;

    class Room
    {
    public:
        friend class RoomManager;

    public:
        Room();
        ~Room();

    public:
        uint32 GetId() const;
        uint32 GetOwnerId() const;
        std::string GetOwnerName() const;
        uint32 GetCategory() const;
        std::string GetName() const;
        std::string GetDescription() const;
        std::string GetModel() const;
        std::string GetCcts() const;
        uint32 GetWallPaper() const;
        uint32 GetFloor() const;
        bool ShowName() const;
        bool GetSuperUsers() const;
        std::string GetAccessType() const;
        std::string GetPassword() const;
        uint32 GetVisitorsNow() const;
        uint32 GetVisitorsMax() const;

    private:
        uint32 mId;
        uint32 mOwnerId;
        std::string mOwnerName;
        uint32 mCategory;
        std::string mName;
        std::string mDescription;
        std::string mModel;
        std::string mCcts;
        uint32 mWallPaper;
        uint32 mFloor;
        bool mShowName;
        bool mSuperUsers;
        std::string mAccessType;
        std::string mPassword;
        uint32 mVisitorsNow;
        uint32 mVisitorsMax;
    };
}

#endif /* _Quad_Room_h_ */