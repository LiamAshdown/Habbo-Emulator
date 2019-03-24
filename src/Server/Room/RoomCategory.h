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

#ifndef _Quad_RoomCategory_h_
#define _Quad_RoomCategory_h_
#include "Common/SharedDefines.h"

namespace SteerStone
{
    class RoomCategory
    {
    public:
        friend class RoomManager;

    public:
        RoomCategory();
        ~RoomCategory();

    public:
        uint32 GetCategory() const;
        std::string GetName() const;
        bool GetRoomType() const;
        uint8 GetMinRoleAccess() const;
        uint8 GetMinRoleSetFlat() const;
        uint8 CanTrade() const;
        uint32 GetParentCategory() const;

    private:
        uint32 mCategoryId;
        uint32 mParentId;
        std::string m_Name;
        bool mRoomType;
        bool mAllowTrading;
        uint16 mMinRoleAccess;
        uint16 mMinRoleSetFlat;
    };
}

#endif /* _Quad_RoomCategory_h_ */