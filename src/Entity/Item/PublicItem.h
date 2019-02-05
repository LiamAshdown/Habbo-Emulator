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

#ifndef _QuadEmu_Item_h_
#define _QuadEmu_Item_h_
#include "../../Common/SharedDefines.h"
#endif /* _QuadEmu_Item_ */

typedef struct ItemPosition
{
    ItemPosition() : x(0), y(0), z(0) {}
    uint8 x;
    uint8 y;
    uint8 z;
}ItemPositionStruct;

class PublicItem
{
public:
    friend class World;

    PublicItem();
    ~PublicItem();

    uint32 GetId() const;
    std::string GetModelName() const;
    std::string GetName() const;
    ItemPositionStruct* GetPosition() const;
    uint8 GetRotation() const;
    std::string GetData() const;

private:
    uint32 mId;
    std::string mModelName;
    std::string mName;
    ItemPositionStruct* mPosition;
    uint8 mRotation;
    std::string mData;
};

