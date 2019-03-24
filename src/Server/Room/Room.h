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
#include "RoomCategory.h"
#include "RoomModel.h"
#include "ItemManager.h"

enum RoomType
{
    ROOM_TYPE_FLAT                      = 0,
    ROOM_TYPE_PUBLIC                    = 1
};

enum RoomConnectionError
{
    ROOM_IS_FULL                        = 1,
    ROOM_IS_CLOSED                      = 2,
    ROOM_IS_IN_QUEUE                    = 3,
};

namespace SteerStone
{
    class Habbo;

    class Room
    {
    public:
        friend class RoomManager;

    public:
        Room();
        ~Room();

    public:
        void EnterRoom(Habbo* player);
        void LeaveRoom(Habbo* player);
        RoomModel* GetRoomModel();
        void SendObjectsWorld(Habbo* player);

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
        void SendUserObjects(Habbo* player);

    private:
        uint32 m_Id;
        uint32 mOwnerId;
        std::string mOwnerName;
        uint32 mCategory;
        std::string m_Name;
        std::string mDescription;
        std::string mModel;
        std::string mCcts;
        uint32 mWallPaper;
        uint32 mFloor;
        bool mShowName;
        bool mSuperUsers;
        std::string mAccessType;
        std::string m_Password;
        uint32 mVisitorsNow;
        uint32 mVisitorsMax;

        RoomModel mRoomModel;
        PublicItemVec mPublicItems;

        std::vector<Habbo*> mPlayers;
    };
}

#endif /* _Quad_Room_h_ */