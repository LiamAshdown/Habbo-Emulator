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

#ifndef _ROOM_ROOM_WALK_WAY_h
#define _ROOM_ROOM_WALK_WAY_h
#include "Common/SharedDefines.h"
#endif /* _ROOM_ROOM_WALK_WAY_h */

namespace SteerStone
{
    /// Holds XYZ Coordinates of WalkWay Position
    typedef struct WalkWayPositionStruct
    {
    public:
        /// Constructor
        WalkWayPositionStruct() : X(0), Y(0), Z(0) {}

        /// Deconstructor
        ~WalkWayPositionStruct() {}

    public:
        int16 X;
        int16 Y;
        int16 Z;
        int16 Rotation;
    }WalkWayPosition;

    /// Holds info about Room Model and position to spawn user on entering new room
    class WalkWay
    {
    public:
        friend class RoomManager;

    public:
        /// Constructor
        WalkWay() {}

        /// Deconstructor
        ~WalkWay() {}

    public:
        /// WalkWay Info
        uint32 GetFromId()   const { return m_FromId;                }
        uint32 GetToId()     const { return m_ToId;                  }
        int16 GetPositionX() const { return m_WalkWayToPos.X;        }
        int16 GetPositionY() const { return m_WalkWayToPos.Y;        }
        int16 GetPositionZ() const { return m_WalkWayToPos.Z;        }
        int16 GetRotation()  const { return m_WalkWayToPos.Rotation; }

    private:
        uint32 m_FromId;                                    ///< Room Id user is coming from
        uint32 m_ToId;                                      ///< Room Id user is going to
        std::vector<WalkWayPosition> m_WalkWayFromPos;      ///< List of coordinates which is a walk way tile
        WalkWayPosition m_WalkWayToPos;                     ///< Coordinates which user will spawn at
        std::string m_WalkWayFromMod;                       ///< Room Model of user previous room
        std::string m_WalkWayToMod;                         ///< Room Model user is going to
    };

} ///< NAMESPACE STEERSTONE
