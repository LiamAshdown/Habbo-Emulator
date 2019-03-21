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
        uint32 GetId() const { return mId; }
        std::string GetRoomModel() const { return mRoomModel; }
        std::string GetSprite() const { return mSprite; }
        int32 GetPositionX() const { return mX; }
        int32 GetPositionY() const { return mY; }
        double GetPositionZ() const { return mZ; }
        int32 GetRotation() const { return mRotation; }
        float GetTopHeight() const { return mTopHeight; }
        int32 GetLength() const { return mLength; }
        int32 GetWidth() const { return mWidth; }
        std::string GetBehaviour() const { return mBehaviour; }

    private:
        uint32 mId;
        std::string mRoomModel;
        std::string mSprite;
        int32 mX;
        int32 mY;
        double mZ;
        int32 mRotation;
        float mTopHeight;
        int32 mLength;
        int32 mWidth;
        std::string mBehaviour;
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