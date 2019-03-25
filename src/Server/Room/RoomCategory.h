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

#ifndef _ROOM_ROOM_CATEGORY_h
#define _ROOM_ROOM_CATEGORY_h
#include "Common/SharedDefines.h"
#endif /* _ROOM_ROOM_CATEGORY_h */

namespace SteerStone
{
    /// Holds information about Room Categories
    class RoomCategory
    {
    public:
        friend class RoomManager;

    public:
        /// Constructor
        /// m_VisitorsNow and m_VisitorsMax is not loaded from database, so set default value
        RoomCategory() : m_VisitorsNow(0), m_VisitorsMax(0) {}
        /// Deconstructor
        ~RoomCategory() {}

    public:
        /// Room Info
        uint32 GetCategoryId()     const { return m_CategoryId;     }
        std::string GetName()      const { return m_Name;           }
        bool GetRoomType()         const { return m_RoomType;       }
        uint8 GetMinRoleAccess()   const { return m_MinRoleAccess;  }
        uint8 GetMinRoleSetFlat()  const { return m_MinRoleSetFlat; }
        uint8 CanTrade()           const { return m_AllowTrading;   }
        uint32 GetParentCategoryId() const { return m_ParentId;       }
        uint32& GetVisitorsNow()         { return m_VisitorsNow;    }
        uint32& GetVisitorsMax()         { return m_VisitorsMax;    }

    private:
        /// Variables
        uint32 m_CategoryId;
        std::string m_Name;
        bool m_RoomType;
        uint16 m_MinRoleAccess;
        uint16 m_MinRoleSetFlat;
        bool m_AllowTrading;
        uint32 m_ParentId;
        uint32 m_VisitorsNow;
        uint32 m_VisitorsMax;
    };
} ///< NAMESPACE STEERSTONE
