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