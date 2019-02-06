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

typedef struct ItemData
{
    ItemData() : length(0), width(0), height(0) {}
    uint8 length;
    uint8 width;
    float height;
}ItemDataStruct;

class Item
{
public:
    friend class World;

    Item();
    ~Item();

    uint32 GetId() const;
    std::string GetSprite() const;
    std::string GetColour() const;
    ItemDataStruct* GetPosition();
    std::string GetData() const;
    std::string GetBehaviour() const;
    std::string GetName() const;
    std::string GetDescription() const;

private:
    uint32 mId;
    std::string mSprite;
    std::string mColour;
    ItemDataStruct* mPosition;
    std::string  mDataClass;
    std::string mBehaviour;
    std::string mName;
    std::string mDescription;
};

