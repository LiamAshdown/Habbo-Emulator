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

#pragma once

#include "Core/Core.hpp"
#include "Diagnostic/DiaStopWatch.hpp"

#include <memory>
#include <atomic>

namespace SteerStone { namespace Core { namespace Threading {

    /// Task types
    enum class TaskType : uint32_t
    {
        Normal,         ///< Support multiple tasks
        Moderate,       ///< Execute one and only task
        Critical        ///< Execute task regardless of hardware concurrency
    };

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Task model
    class Task
    {
        DISALLOW_COPY_AND_ASSIGN(Task);

        //////////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////////

        public:
            /// Shared ptr type for tasks
            using Ptr = std::shared_ptr<Task>;

        public:
            /// Constructor
            /// @p_Name     : Task name
            /// @p_TaskType : Task type
            Task(const std::string & p_Name, TaskType p_TaskType);
            /// Destructor
            virtual ~Task();

            /// Set Task
            /// @p_TaskType : Task type
            void SetTaskType(TaskType p_TaskType);

            /// Get name
            const std::string & GetTaskName();
            /// Get flags
            TaskType GetTaskType() const;
            /// Get flags
            uint64 GetTaskTotalRunTime() const;
            /// Get flags
            uint64 GetTaskTotalRunCount() const;
            /// Get average execution time
            uint64 GetTaskAverageUpdateTime() const;
            /// Get last diff time
            uint64 GetTaskLastDiffTime() const;

            /// Update
            /// @p_Diff : Delta time between to run
            bool UpdateTaskTime(uint64 p_Diff);
            /// Update
            bool UpdateTask();

            /// Get period
            virtual uint64 GetTaskPeriod() const;
            /// Execute
            /// If return false, the task will stop
            virtual bool TaskExecute() = 0;

        private:
            std::string m_TaskName;     ///< Name
            TaskType    m_TaskType;     ///< Type
            int64       m_TaskTimer;    ///< Time until next execution in MS

            std::atomic_uint64_t m_TaskTotalRunTime;    ///< Total run time
            std::atomic_uint64_t m_TaskTotalRunCount;   ///< Total run count
            std::atomic_uint64_t m_TaskAverageRunTime;  ///< Avg execution time
            std::atomic_uint64_t m_TaskLastDiffTime;    ///< Last diff time

            Diagnostic::StopWatch m_TaskStopWatch;      ///< Stop watch
    };

}   ///< namespace Threading
}   ///< namespace Core
}   ///< namespace SteerStone
