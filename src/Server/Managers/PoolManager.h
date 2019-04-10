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
    class Habbo;

    typedef std::unordered_map<std::string, Habbo*> PoolQueueMap;

    /// Handles the queue to enter pool booth to change to swim wear
    class PoolManager
    {
    public:
        static PoolManager* instance();

    public:
        /// Constructor
        PoolManager();

        /// Deconstructor
        ~PoolManager();

    public:
        /// AddHabbo
        /// Add Habbo to queue
        /// @p_Name : Name of queue we are adding habbo too
        /// @p_Habbo : Habbo we are adding to queue
        void AddHabbo(std::string p_Name, Habbo* p_Habbo);

    private:
        PoolQueueMap m_PoolQueue;           ///< Key is Item name and structure is Habbo class
       
    };
} ///< NAMESPACE STEERSTONE

#define sPoolMgr SteerStone::PoolManager::instance()