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

#include "Singleton/Singleton.hpp"
#include "Threading/ThrTaskWorker.hpp"
#include "Threading/ThrOptimizeTask.hpp"

#include <vector>
#include <functional>
#include <string>

#define EXLUSIVE_CURRENCY_COUNT 0.30f

namespace SteerStone { namespace Core { namespace Threading {

    /// TaskWorker
    class TaskManager
    {
        SINGLETON_P_D(TaskManager);

        //////////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////////

        public:
            /// Push task
            /// @p_Task : Task to push
            void PushTask(const Task::Ptr & p_Task);

            /// Push a lambda task
            /// @p_Name     : Task name
            /// @p_TaskType : Task Type
            /// @p_Period   : Task interval
            /// @p_Function : Task
            Task::Ptr PushTask(const std::string & p_Name, const TaskType p_TaskType, const uint64 p_Period, const std::function<bool()> & p_Function);
            /// Push a lambda task
            /// @p_TaskType : Task Type
            /// @p_Period   : Task interval
            /// @p_Function : Task
            Task::Ptr PushTask(const TaskType p_TaskType, const uint64 p_Period, const std::function<bool()> & p_Function);
            /// Push a lambda task
            /// @p_Name     : Task name
            /// @p_TaskType : Task Type
            /// @p_Function : Task
            Task::Ptr PushRunOnceTask(const std::string & p_Name, const TaskType p_TaskType, const std::function<void()> & p_Function);
            /// Push a lambda task
            /// @p_TaskType : Task Type
            /// @p_Function : Task
            Task::Ptr PushRunOnceTask(const TaskType p_TaskType, const std::function<void()> & p_Function);

            /// Pop task
            /// @p_Task : Task to pop
            void PopTask(const Task::Ptr & p_Task);

            /// Get all tasks
            std::vector<Task::Ptr> GetTasks();

            /// Set worker count
            /// @p_Count : Worker count
            void SetWorkerCount(uint32 p_Count);
            /// Set optimize task period
            /// @p_Period : New period
            void SetOptimizePeriod(uint64 p_Period);

            /// Optimize
            /// Only for Inclusive Workers
            void Optimize();

        //////////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////////

        private:
            std::recursive_mutex    m_Mutex;        ///< Global mutex
            bool                    m_LogTasks;     ///< Should log tasks
            OptimizeTaskPtr         m_OptimizeTask; ///< Optimize task instance

            std::vector<Task::Ptr>      m_Tasks;                ///< Tasks
            std::vector<TaskWorker*>    m_InclusiveTaskWorkers; ///< Workers
            std::vector<TaskWorker*>    m_ExclusiveTaskWorkers; ///< Workers
            std::vector<TaskWorker*>    m_CriticalTaskWorkers;  ///< Workers

    };

}   ///< namespace Threading
}   ///< namespace Core
}   ///< namespace SteerStone

#define sThreadManager SteerStone::Core::Threading::TaskManager::GetSingleton()