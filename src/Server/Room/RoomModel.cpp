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
#include "RoomModel.h"
//-----------------------------------------------//
namespace SteerStone
{
    RoomModel::RoomModel()
    {
    }
    RoomModel::~RoomModel()
    {
    }
    //-----------------------------------------------//
    uint32 RoomModel::GetId() const
    {
        return m_Id;
    }
    //-----------------------------------------------//
    std::string RoomModel::GetModelId() const
    {
        return mModelId;
    }
    //-----------------------------------------------//
    std::string RoomModel::GetModel() const
    {
        return mModel;
    }
    //-----------------------------------------------//
    int32 RoomModel::GetDoorX() const
    {
        return mDoorX;
    }
    //-----------------------------------------------//
    int32 RoomModel::GetDoorY() const
    {
        return mDoorY;
    }
    //-----------------------------------------------//
    float RoomModel::GetDoorZ() const
    {
        return mDoorZ;
    }
    //-----------------------------------------------//
    int32 RoomModel::GetDoorOrientation() const
    {
        return mDoorOrientation;
    }
    //-----------------------------------------------//
    std::string RoomModel::GetHeightMap() const
    {
        return mHeightMap;
    }
    //-----------------------------------------------//
}
//-----------------------------------------------//