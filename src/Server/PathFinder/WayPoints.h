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
        friend class Room;

    public:
        /// Constructor
        /// @p_TileGrid - Tile Grid array which stores instance of tile
        /// @p_MaxGridX : Max X Tile Grid
        /// @p_MaxGridY : Max Y Tile Grid
        WayPoints(RoomModel* p_RoomModel);

        /// Deconstructor
        ~WayPoints();

    public:
        /// ToHabbo
        /// Returns Habbo who has an active path
        Habbo* ToHabbo() { return m_Habbo; }

        /// GetEndPositionX
        /// Returns end waypoint
        int16 GetEndPositionX() const { return m_EndX; }

        /// GetEndPositionY
        /// Returns end waypoint
        int16 GetEndPositionY() const { return m_EndY; }

        /// CheckForInteractiveObjects
        /// Check if user is sitting or standing ontop any objects which user can interact with (automatically)
        void CheckForInteractiveObjects();

    private:
        int16 m_EndX;               ///< Keep a reference of end position in case we need to recaculate path
        int16 m_EndY;
        RoomModel* m_RoomModel;
        Habbo* m_Habbo;             ///< Habbo user who has an active path
    };

} ///< NAMESPACE STEERSTONE