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

#include <PCH/Precompiled.hpp>

#include "Threading/ThrLambdaTask.hpp"

namespace SteerStone { namespace Core { namespace Threading {

    /// Constructor
    /// @p_Name     : Task name
    /// @p_TaskType : Task Type
    /// @p_Period   : Task period
    /// @p_Function : Function
    LambdaTask::LambdaTask(const std::string & p_Name, TaskType p_TaskType, uint64 p_Period, std::function<bool()> p_Function)
        : Task(p_Name, p_TaskType), m_Period(p_Period), m_Function(p_Function)
    {

    }
    /// Destructor
    LambdaTask::~LambdaTask()
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Get period
    uint64 LambdaTask::GetTaskPeriod() const
    {
        return m_Period;
    }
    /// Execute
    /// If return false, the task will stop
    bool LambdaTask::TaskExecute()
    {
        return m_Function();
    }

}   ///< namespace Threading
}   ///< namespace Core
}   ///< namespace SteerStone
