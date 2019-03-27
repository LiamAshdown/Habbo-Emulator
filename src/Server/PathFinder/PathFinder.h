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

#ifndef _PATHFINDER_PATHFINDER_h
#define _PATHFINDER_PATHFINDER_h
#include "Common/SharedDefines.h"
#endif /* _PATHFINDER_PATHFINDER_h */

namespace SteerStone
{
    /// Class which calculates user path to a specific grid in the room
    /// using A* algorithm
    class PathFinder
    {
    public:
        /// Constructor
        PathFinder();

        /// Deconstructor
        ~PathFinder();
    };
} ///< NAMESPACE STEERSTONE