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
#include "Item.h"
//-----------------------------------------------//
Item::Item()
{
    mId = 0;
    mSprite = "";
    mColour = "";
    mPosition = new ItemDataStruct;
    mDataClass = "";
    mBehaviour = "";
    mName = "";
    mDescription = "";
}
//-----------------------------------------------//
Item::~Item()
{
    delete mPosition;
}
//-----------------------------------------------//
uint32 Item::GetId() const
{
    return mId;
}
//-----------------------------------------------//
std::string Item::GetSprite() const
{
    return mSprite;
}
//-----------------------------------------------//
std::string Item::GetColour() const
{
    return mColour;
}
//-----------------------------------------------//
ItemDataStruct * Item::GetPosition()
{
    return mPosition;
}
//-----------------------------------------------//
std::string Item::GetData() const
{
    return mDataClass;
}
//-----------------------------------------------//
std::string Item::GetBehaviour() const
{
    return mBehaviour;
}
//-----------------------------------------------//
std::string Item::GetName() const
{
    return mName;
}
//-----------------------------------------------//
std::string Item::GetDescription() const
{
    return mDescription;
}
//-----------------------------------------------//
