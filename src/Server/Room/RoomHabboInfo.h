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

#ifndef _ROOM_ROOM_HABBO_INFO_h
#define _ROOM_ROOM_HABBO_INFO_h
#include "Common/SharedDefines.h"
#include "WayPoints.h"
#include <atomic>
#endif /* _ROOM_ROOM_HABBO_INFO_h */

namespace SteerStone
{
    class Habbo;

    /// Deals with Pathfinding, Habbo statuses etc..
    class RoomHabboInfo
    {
    public:
        friend class Room;

    public:
        /// Constructor
        /// @p_Habbo - User which joined the room
        RoomHabboInfo(Habbo* p_Habbo);

        /// Deconstructor
        ~RoomHabboInfo();

    public:
        /// AddStatus
        /// @p_Status : Add Status to be processed on next room update
        void AddStatus(uint32 const p_Status);

        /// RemoveStatus
        /// @p_Status : Status to be removed
        void RemoveStatus(uint32 const p_Status);

        /// CreatePath
        /// @p_EndX : End position X
        /// @p_EndY : End position Y
        bool CreatePath(uint16 const p_EndX, uint16 const p_EndY);

        /// ProcessActions 
        /// Process all actions
        /// @p_Diff : Hotel last tick time
        void ProcessActions(uint32 const p_Diff);

        /// HasStatus
        /// Check if user has an active status
        /// @p_Status : Status to check
        bool HasStatus(uint32 p_Status) const;

    private:
        /// ProcessWayPoints
        /// Process next waypoint
        void ProcessNextWayPoint();

        /// ProcessStatusUpdates
        /// Process Habbo Status
        void ProcessStatusUpdates();

        /// CheckTimers
        /// Check durations of user; Waving, AFK etc..
        /// @p_Diff : Hotel last tick time
        void CheckTimers(uint32 const p_Diff);

        /// CanSendStatusUpdate
        /// Check if we can send status update to client(s)
        bool CanSendStatusUpdate() const { return m_UpdateClient; }

        /// ToHabbo
        /// Returns Habbo class
        Habbo* ToHabbo() const;

    private:
        Habbo* m_Habbo;
        std::atomic<uint32> m_Statuses;             ///< Holds user statuses
        std::atomic<bool> m_UpdateClient;           ///< Check if we can send status update to client(s)

        uint32 m_WaveTimer;                         ///< Time user can wave for
        uint32 m_AFKTimer;                          ///< Last time user sent a update status packet

        std::unique_ptr<WayPoints> m_Path;          ///< Holds Waypoints to walk a path
    };
} ///< NAMESPACE STEERSTONE