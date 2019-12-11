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

#include "Threading/ThrTask.hpp"

#include <thread>
#include <shared_mutex>

namespace SteerStone { namespace Core { namespace Threading {

    enum class WorkerType
    {
        Inclusive,      ///< Can be popped
        Exclusive       ///< Cannot be popped          
    };

    /// TaskWorker
    class TaskWorker
    {
        DISALLOW_COPY_AND_ASSIGN(TaskWorker);

        //////////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////////

        public:
            /// Constructor
            /// @p_WorkerType : Type of Worker
            TaskWorker(WorkerType p_WorkerType);
            /// Destructor
            ~TaskWorker();

            /// Push task
            void PushTask(const Task::Ptr & p_Task);
            /// Pop task
            void PopTask(const Task::Ptr & p_Task);
            /// Pop all
            void PopAll();

            /// Have task
            bool HaveTask(const Task::Ptr & p_Task);

            /// Get flags
            uint64 GetTotalRunTime() const;
            /// Get flags
            uint64 GetTotalRunCount() const;
            /// Get average execution time
            uint64 GetAverageUpdateTime() const;
            /// Get Task Size
            std::size_t GetTaskSize() const;
            /// Reset avg update time
            void ResetAverageUpdateTime();

            /// PreSuspend the worker
            void PreSuspend();
            /// Suspend the worker
            void Suspend();
            /// Resume the worker
            void Resume();

            /// Set thread CPU affinity
            /// @p_Affinity : Affinity
            void SetCPUAffinty(int32 p_Affinity);
            /// Set task worker name
            /// @p_Name : New name
            void SetName(const std::string & p_Name);

        private:
            /// Update thread
            void UpdateThread();

        private:
            std::recursive_mutex    m_Mutex;        ///< Mutex
            std::string             m_Name;         ///< Name
            std::thread *           m_Thread;       ///< Thread
            int32                   m_CPUAffinity;  ///< CPU affinity
            bool                    m_IsRunning;    ///< Thread run condition
            WorkerType              m_WorkerType;   ///< Type

            std::vector<Task::Ptr> m_Tasks; ///< Tasks

            std::atomic<uint64> m_TotalRunTime;      ///< Total run time
            std::atomic<uint64> m_TotalRunCount;     ///< Total run count
            std::atomic<uint64> m_AverageRunTime;    ///< Avg execution time

    };

}   ///< namespace Threading
}   ///< namespace Core
}   ///< namespace SteerStone