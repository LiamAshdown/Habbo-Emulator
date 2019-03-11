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

#ifndef _Quad_Item_h_
#define _Quad_Item_h_
#include "Common/SharedDefines.h"

namespace Quad
{
    ///< We don't need to create a class for public room items, because they will never be changed
    ///< So we will leave it as a struct
    typedef struct PublicItemStruct
    {
    public:
        friend class ItemManager;

    public:
        PublicItemStruct() {}
        ~PublicItemStruct() {}

    public:
        uint32 GetId() const { return sId; }
        uint32 GetDefinitionId() const { return sDefinitionId; }
        std::string GetSprite() const { return sSprite; }
        std::string GetModel() const { return sModel; }
        uint16 GetPositionX() const { return sX; }
        uint16 GetPositionY() const { return sY; }
        uint16 GetPositionZ() const { return sZ; }
        uint8 GetRotation() const { return sRotation; }
        std::string GetObject() const { return sObject; }
        std::string GetData() const { return sData; }

    private:
        uint32 sId;
        uint32 sDefinitionId;
        std::string sSprite;
        std::string sModel;
        uint16 sX;
        uint16 sY;
        uint16 sZ;
        uint8 sRotation;
        std::string sObject;
        std::string sData;
    }PublicItem;

    class Item
    {
    public:
        friend class ItemManager;

    public:
        Item();
        ~Item();

    private:
        uint32 mId;
        std::string mSprite;
        std::string mColor;
        std::string mLength;
        uint16 mWidth;
        float mHeight;
        std::string mDataClass;
        std::string mBehaviour;
        std::string mName;
        std::string mDescription;
    };
}

#endif /* _Quad_Item_h_ */