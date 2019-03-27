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

#ifndef MANAGERS_PATH_FINDER_MANAGER_h
#define MANAGERS_PATH_FINDER_MANAGER_h
#include "Common/SharedDefines.h"
#include "PathFinder.h"
#endif /* MANAGERS_PATH_FINDER_MANAGER_h */

namespace SteerStone
{
    /// Class which holds information about current pathfinder objects
    /// Class gets updated by Hotel tick
    /// Singleton
    class PathFinderManager
    {
    public:
        static PathFinderManager* instance();

    public:
        /// Constructor
        PathFinderManager();

        /// Deconstructor
        ~PathFinderManager();
       
    };
} ///< NAMESPACE STEERSTONE

#define sPathFinderMgr SteerStone::PathFinderManager::instance()