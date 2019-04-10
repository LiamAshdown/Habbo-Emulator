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

#include "PoolManager.h"

namespace SteerStone
{
    /// Singleton
    PoolManager * PoolManager::instance()
    {
        static PoolManager instance;
        return &instance;
    }

    /// Constructor
    PoolManager::PoolManager()
    {}

    /// Deconstructor
    PoolManager::~PoolManager()
    {}

    /// AddHabbo
    /// Add Habbo to queue
    /// @p_Name : Name of queue we are adding habbo too
    /// @p_Habbo : Habbo we are adding to queue
    void PoolManager::AddHabbo(std::string p_Name, Habbo * p_Habbo)
    {
        /// Check if Habbo already exists in the queue
        auto const& l_Itr = m_PoolQueue.find(p_Name);
    }
} ///< NAMESPACE STEERSTONE
