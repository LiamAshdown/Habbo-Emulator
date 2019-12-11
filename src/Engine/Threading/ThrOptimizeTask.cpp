/*
* Liam Ashdown
* HardCPP (Merydwin)
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

#include <PCH/Precompiled.hpp>

#include "Threading/ThrOptimizeTask.hpp"
#include "Threading/ThrTaskManager.hpp"

namespace SteerStone { namespace Core { namespace Threading {

    /// Constructor
    /// @p_Period : Task period
    OptimizeTask::OptimizeTask(uint64 p_Period)
        : Task("OptimizeTask", TaskType::Critical), m_Period(p_Period)
    {

    }
    /// Destructor
    OptimizeTask::~OptimizeTask()
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Get period
    uint64 OptimizeTask::GetTaskPeriod() const
    {
        return m_Period;
    }
    /// Get period
    /// @p_Period : New period
    void OptimizeTask::SetTaskPeriod(uint64 p_Period)
    {
        m_Period = p_Period;
    }
    /// Execute
    /// If return false, the task will stop
    bool OptimizeTask::TaskExecute()
    {
        TaskManager::GetSingleton()->Optimize();

        return true;
    }

}   ///< namespace Threading
}   ///< namespace Core
}   ///< namespace SteerStone
