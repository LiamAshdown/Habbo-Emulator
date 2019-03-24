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
#include "RoomCategory.h"
//-----------------------------------------------//
namespace SteerStone
{
    //-----------------------------------------------//
    RoomCategory::RoomCategory()
    {
    }
    //-----------------------------------------------//
    RoomCategory::~RoomCategory()
    {
    }
    //-----------------------------------------------//
    uint32 RoomCategory::GetCategory() const
    {
        return mCategoryId;
    }
    //-----------------------------------------------//
    std::string RoomCategory::GetName() const
    {
        return m_Name;
    }
    //-----------------------------------------------//
    bool RoomCategory::GetRoomType() const
    {
        return mRoomType;
    }
    //-----------------------------------------------//
    uint8 RoomCategory::GetMinRoleAccess() const
    {
        return mMinRoleAccess;
    }
    //-----------------------------------------------//
    uint8 RoomCategory::GetMinRoleSetFlat() const
    {
        return mMinRoleSetFlat;
    }
    //-----------------------------------------------//
    uint8 RoomCategory::CanTrade() const
    {
        return mAllowTrading;
    }
    //-----------------------------------------------//
    uint32 RoomCategory::GetParentCategory() const
    {
        return mParentId;
    }
    //-----------------------------------------------//
}
//-----------------------------------------------//