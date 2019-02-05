/*
* Liam Ashdown
* Copyright (C) 2018
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
#include "PublicItem.h"
//-----------------------------------------------//
PublicItem::PublicItem()
{
    mId = 0;
    mModelName = "";
    mName = "";
    mData = "";
    mRotation = 0;
    mPosition = new ItemPositionStruct;
}
//-----------------------------------------------//
PublicItem::~PublicItem()
{
}
//-----------------------------------------------//
uint32 PublicItem::GetId() const
{
    return mId; 
}
//-----------------------------------------------//
std::string PublicItem::GetModelName() const
{
    return mModelName;
}
//-----------------------------------------------//
std::string PublicItem::GetName() const
{
    return mName;
}
//-----------------------------------------------//
ItemPositionStruct * PublicItem::GetPosition() const
{
    return mPosition;
}
//-----------------------------------------------//
uint8 PublicItem::GetRotation() const
{
    return mRotation;
}//-----------------------------------------------//
std::string PublicItem::GetData() const
{
    return mData;
}
//-----------------------------------------------//
