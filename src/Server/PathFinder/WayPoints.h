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

#ifndef _PATHFINDER_WAY_POINTS_h
#define _PATHFINDER_WAY_POINTS_h
#include "Common/SharedDefines.h"
#include "PathFinder.h"
#endif /* _PATHFINDER_WAY_POINTS_h */

namespace SteerStone
{
    class Habbo;

    /// Holds information about path points from PathFinder
    class WayPoints : public PathFinder
    {
    public:
        /// Constructor
        /// @p_Habbo :
        /// @p_RoomModel :
        WayPoints(Habbo* p_Habbo, RoomModel* p_RoomModel);

        /// Deconstructor
        ~WayPoints();

    public:
        /// ToHabbo
        /// Returns Habbo who has an active path
        Habbo* ToHabbo() { return m_Habbo; }

        /// GetEndPositionX
        /// Returns end position X
        int16 GetEndPositionX() const { return m_EndX; }

        /// GetEndPositionY
        /// Returns end position Y
        int16 GetEndPositionY() const { return m_EndY; }

        /// SetEndPosition
        /// @p_X : End Position X
        /// @p_Y : End Position Y
        void SetEndPosition(int16 const p_X, int16 const p_Y);

        /// HasActivePath
        /// Check if user has an active path
        bool HasActivePath() const { return m_ActivePath; }

        /// SetActivePath
        /// @p_ActivePath : True if active path, false if there's no path active
        void SetActivePath(bool const p_ActivePath) { m_ActivePath = p_ActivePath; }

        /// CheckForInteractiveObjects
        /// Check if user is sitting or standing ontop any objects which user can interact with (automatically)
        void CheckForInteractiveObjects();

    private:
        int16 m_EndX;               ///< Keep a reference of end position in case we need to recaculate path
        int16 m_EndY;
        bool m_ActivePath;          ///< Check if there's an active path
        RoomModel* m_RoomModel;     ///< Room Model we are currently in
        Habbo* m_Habbo;             ///< Habbo
    };
} ///< NAMESPACE STEERSTONE