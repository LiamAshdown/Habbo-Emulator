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

#include "Threading/ThrTask.hpp"

#include "Logger/Base.hpp"

namespace SteerStone { namespace Core { namespace Threading {

    /// Constructor
    /// @p_Name     : Task name
    /// @p_TaskType : Task type
    Task::Task(const std::string & p_Name, TaskType p_TaskType)
        : m_TaskName(p_Name), m_TaskType(p_TaskType), m_TaskTimer(-1), m_TaskTotalRunTime(0), m_TaskTotalRunCount(0), m_TaskAverageRunTime(0), m_TaskLastDiffTime(0)
    {
        m_TaskStopWatch.Start();
    }
    /// Destructor
    Task::~Task()
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Set Task
    /// @p_TaskType : Task type
    void Task::SetTaskType(TaskType p_TaskType)
    {
        m_TaskType = p_TaskType;
    }

    /// Get name
    const std::string& Task::GetTaskName()
    {
        return m_TaskName;
    }
    /// Get flags
    TaskType Task::GetTaskType() const
    {
        return m_TaskType;
    }
    /// Get flags
    uint64 Task::GetTaskTotalRunTime() const
    {
        return m_TaskTotalRunTime;
    }
    /// Get flags
    uint64 Task::GetTaskTotalRunCount() const
    {
        return m_TaskTotalRunCount;
    }
    /// Get average execution time
    uint64 Task::GetTaskAverageUpdateTime() const
    {
        return m_TaskAverageRunTime;
    }
    /// Get last diff time
    uint64 Task::GetTaskLastDiffTime() const
    {
        return m_TaskLastDiffTime;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Update
    /// @p_Diff : Delta time between to run
    bool Task::UpdateTaskTime(uint64 p_Diff)
    {
        int64 l_Elapsed = m_TaskStopWatch.GetElapsed();

        //LOG_INFO("task", "timer %0, internval %1, diff %2", m_TaskTimer, GetTaskPeriod(), l_Elapsed);

        m_TaskTimer       -= l_Elapsed;
        m_TaskLastDiffTime = l_Elapsed;

        m_TaskStopWatch.Reset();

        return m_TaskTimer <= 0;
    }
    /// Update
    bool Task::UpdateTask()
    {
        m_TaskStopWatch.Reset();
        bool l_Result = true;

      //  try
        //{
            l_Result = TaskExecute();
       // }
        //catch (std::exception p_Exception)
       // {
       //     LOG_ERROR("TheTask", R"LOG(Exception in task "%0", details : \n%1)LOG", m_TaskName, p_Exception.what());
       // }
       // catch (...)
       // {
       //     LOG_ERROR("TheTask", R"LOG(UNKWNOWN Exception in task "%0")LOG", m_TaskName);
      //  }

        m_TaskTotalRunTime += m_TaskStopWatch.GetElapsed();
        m_TaskTotalRunCount++;

        m_TaskAverageRunTime = static_cast<uint32>(m_TaskTotalRunTime / m_TaskTotalRunCount);

        m_TaskTimer = static_cast<float>(GetTaskPeriod()) * 0.80f;

        return l_Result;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Get period time between execution
    uint64 Task::GetTaskPeriod() const
    {
        return 0;
    }

}   ///< namespace Threading
}   ///< namespace Core
}   ///< namespace SteerStone
