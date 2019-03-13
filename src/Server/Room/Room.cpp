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
#include "Room.h"
#include "Player.h"
#include "PlayerSocket.h"
#include "Network/StringBuffer.h"
#include "RoomManager.h"
#include "ItemManager.h"
//-----------------------------------------------//
namespace Quad
{
    //-----------------------------------------------//
    Room::Room()
    {
    }
    //-----------------------------------------------//
    Room::~Room()
    {
    }
    //-----------------------------------------------//
    uint32 Room::GetId() const
    {
        return mId;
    }
    //-----------------------------------------------//
    uint32 Room::GetOwnerId() const
    {
        return mOwnerId;
    }
    //-----------------------------------------------//
    std::string Room::GetOwnerName() const
    {
        return mOwnerName;
    }
    //-----------------------------------------------//
    uint32 Room::GetCategory() const
    {
        return mCategory;
    }
    //-----------------------------------------------//
    std::string Room::GetName() const
    {
        return mName;
    }
    //-----------------------------------------------//
    std::string Room::GetDescription() const
    {
        return mDescription;
    }
    //-----------------------------------------------//
    std::string Room::GetModel() const
    {
        return mModel;
    }
    //-----------------------------------------------//
    std::string Room::GetCcts() const
    {
        return mCcts;
    }
    //-----------------------------------------------//
    uint32 Room::GetWallPaper() const
    {
        return mWallPaper;
    }
    //-----------------------------------------------//
    uint32 Room::GetFloor() const
    {
        return mFloor;
    }
    //-----------------------------------------------//
    bool Room::ShowName() const
    {
        return mShowName;
    }
    //-----------------------------------------------//
    bool Room::GetSuperUsers() const
    {
        return mSuperUsers;
    }
    //-----------------------------------------------//
    std::string Room::GetAccessType() const
    {
        return mAccessType;
    }
    //-----------------------------------------------//
    std::string Room::GetPassword() const
    {
        return mPassword;
    }
    //-----------------------------------------------//
    uint32 Room::GetVisitorsNow() const
    {
        return mVisitorsNow;
    }
    //-----------------------------------------------//
    uint32 Room::GetVisitorsMax() const
    {
        return mVisitorsMax;
    }
    //-----------------------------------------------//
}
//-----------------------------------------------//