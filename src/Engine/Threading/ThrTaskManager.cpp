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

#include "Threading/ThrTaskManager.hpp"
#include "Threading/ThrLambdaTask.hpp"

#include "Logger/Base.hpp"

#include <algorithm>
#include <math.h>

namespace SteerStone { namespace Core { namespace Threading {

    SINGLETON_P_I(TaskManager);

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    TaskManager::TaskManager()
        : m_LogTasks(true)
    {
        #ifdef STEERSTONE_CORE_DEBUG
            LOG_INFO("ThrTaskManager", "Initialized");
        #endif

        /// Default interval is 2 minutes
        m_OptimizeTask = std::make_shared<OptimizeTask>(10 * 1000);

        /// By default we are using all CPU cores but not the first one
        SetWorkerCount(std::thread::hardware_concurrency() - 1);
        PushTask(m_OptimizeTask);
    }
    /// Destructor
    TaskManager::~TaskManager()
    {
        LOG_INFO("ThrTaskManager", "Destroyed");
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Push task
    /// @p_Task : Task to push
    void TaskManager::PushTask(const Task::Ptr & p_Task)
    {
        if (m_LogTasks)
            LOG_INFO("ThrTaskManager", "Task %0 started", p_Task->GetTaskName());

        std::lock_guard<std::recursive_mutex> l_Lock(m_Mutex);

        if (p_Task->GetTaskType() == TaskType::Critical)
        {
            TaskWorker * l_CriticalWorker = new TaskWorker(WorkerType::Exclusive);

            /// Skip CPU 0 at first iteration, to save some CPU for the kernel
            l_CriticalWorker->SetCPUAffinty((m_CriticalTaskWorkers.size() + 1) % std::thread::hardware_concurrency());
            l_CriticalWorker->SetName(Utils::StringBuilder("CriticalTaskWorker_%0", m_CriticalTaskWorkers.size()));
            l_CriticalWorker->PushTask(p_Task);

            m_CriticalTaskWorkers.push_back(l_CriticalWorker);
        }
        else if (p_Task->GetTaskType() == TaskType::Moderate)
        {
            for (std::size_t l_I = 0; l_I < m_ExclusiveTaskWorkers.size(); l_I++)
            {
                TaskWorker* l_CurrentWorker = m_ExclusiveTaskWorkers[l_I];

                if (l_CurrentWorker->GetTaskSize() == 0)
                {
                    l_CurrentWorker->PushTask(p_Task);
                    return;
                }
            }

            p_Task->SetTaskType(TaskType::Critical);
            PushTask(p_Task);

            LOG_WARNING("ThrTaskManager", "Could not add exclusive task %0. Re-adding task as Critical", p_Task->GetTaskName());
        }
        else
        {
            if (!m_InclusiveTaskWorkers.empty())
            {
                TaskWorker *    l_CurrentWorker         = m_InclusiveTaskWorkers.at(0);
                uint64          l_MinAverageUpdateTime  = l_CurrentWorker->GetAverageUpdateTime();

                for (std::size_t l_I = 1; l_I < m_InclusiveTaskWorkers.size(); ++l_I)
                {
                    if (m_InclusiveTaskWorkers[l_I]->GetAverageUpdateTime() < l_MinAverageUpdateTime)
                        l_CurrentWorker = m_InclusiveTaskWorkers[l_I];
                }

                l_CurrentWorker->PushTask(p_Task);
            }

            m_Tasks.push_back(p_Task);

            if (m_InclusiveTaskWorkers.empty())
                LOG_ERROR("ThrTaskManager", "Task %0 added with no inclusive worker", p_Task->GetTaskName());
        }
    }
    /// Push a lambda task
    /// @p_Name     : Task name
    /// @p_TaskType : Task Type
    /// @p_Period   : Task interval
    /// @p_Function : Task
    Task::Ptr TaskManager::PushTask(const std::string & p_Name, const TaskType p_TaskType, uint64 p_Period, const std::function<bool()> & p_Function)
    {
        const Task::Ptr l_Task = std::make_shared<LambdaTask>(p_Name, p_TaskType, p_Period, p_Function);

        PushTask(l_Task);

        return l_Task;
    }
    /// Push a lambda task
    /// @p_TaskType : Task Type
    /// @p_Period   : Task interval
    /// @p_Function : Task
    Task::Ptr TaskManager::PushTask(const TaskType p_TaskType, const uint64 p_Period, const std::function<bool()> & p_Function)
    {
        const std::string l_TaskName = Utils::StringBuilder("ANONYMOUS_LAMBDA_%0", clock());
        return PushTask(l_TaskName, p_TaskType, p_Period, p_Function);
    }
    /// Push a lambda task
    /// @p_Name     : Task name
    /// @p_TaskType : Task Type
    /// @p_Function : Task
    Task::Ptr TaskManager::PushRunOnceTask(const std::string & p_Name, const TaskType p_TaskType, const std::function<void()> & p_Function)
    {
        const Task::Ptr l_Task = std::make_shared<LambdaTask>(p_Name, p_TaskType, 0, [p_Function]() -> bool {
            p_Function();
            return false;
        });

        PushTask(l_Task);

        return l_Task;
    }
    /// Push a lambda task
    /// @p_TaskType : Task Type
    /// @p_Function : Task
    Task::Ptr TaskManager::PushRunOnceTask(const TaskType p_TaskType, const std::function<void()> & p_Function)
    {
        const std::string l_TaskName = Utils::StringBuilder("ANONYMOUS_RUN_ONCE_LAMBDA_%0", clock());
        return PushRunOnceTask(l_TaskName, p_TaskType, p_Function);
    }
    /// Pop task
    /// @p_Task : Task to pop
    void TaskManager::PopTask(const Task::Ptr & p_Task)
    {
        if (m_LogTasks)
            LOG_WARNING("ThrTaskManager", "Task %0 ended", p_Task->GetTaskName());

        std::lock_guard<std::recursive_mutex> l_Lock(m_Mutex);

        if (p_Task->GetTaskType() == TaskType::Critical)
        {
            auto l_It = std::find_if(m_CriticalTaskWorkers.begin(), m_CriticalTaskWorkers.end(), [&p_Task](TaskWorker * p_Worker) -> bool {
                return p_Worker->HaveTask(p_Task);
            });

            if (l_It == m_CriticalTaskWorkers.end())
                return;

            TaskWorker * l_Worker = *l_It;
            l_Worker->Suspend();
            l_Worker->PopAll();

            delete l_Worker;

            m_CriticalTaskWorkers.erase(l_It);
        }
        else if (p_Task->GetTaskType() == TaskType::Moderate)
        {
            for (auto l_Worker : m_ExclusiveTaskWorkers)
                l_Worker->PopTask(p_Task);
        }
        else
        {
            auto l_It = std::find(m_Tasks.begin(), m_Tasks.end(), p_Task);

            if (l_It == m_Tasks.end())
                return;

            for (auto l_Worker : m_InclusiveTaskWorkers)
                l_Worker->PopTask(p_Task);

            m_Tasks.erase(l_It);
        }
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Get all tasks
    std::vector<Task::Ptr> TaskManager::GetTasks()
    {
        std::lock_guard<std::recursive_mutex> l_Lock(m_Mutex);

        return m_Tasks;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Set worker count
    /// @p_Count : Worker count
    void TaskManager::SetWorkerCount(uint32 p_Count)
    {
        std::lock_guard<std::recursive_mutex> l_Lock(m_Mutex);

        const uint32 l_TaskWorkerCount = m_InclusiveTaskWorkers.size() + m_ExclusiveTaskWorkers.size();

        if (p_Count == l_TaskWorkerCount)
            return;

        if (p_Count >= std::thread::hardware_concurrency())
        {
            LOG_ERROR("ThrTaskManager", "To many workers for actual CPU unit", p_Count);
            return;
        }

        if (p_Count < l_TaskWorkerCount)
        {
            const std::size_t l_ExclusiveCount = std::floor(p_Count * EXLUSIVE_CURRENCY_COUNT);
            const std::size_t l_InclusiveCount = p_Count - l_ExclusiveCount;

            auto l_WorkersCopy = m_InclusiveTaskWorkers;
            m_InclusiveTaskWorkers.resize(l_InclusiveCount);

            for (size_t l_I = l_InclusiveCount; l_I < l_WorkersCopy.size(); ++l_I)
            {
                TaskWorker* l_Worker = l_WorkersCopy[l_I];
                l_Worker->Suspend();

                delete l_Worker;
            }

            l_WorkersCopy = m_ExclusiveTaskWorkers;
            m_InclusiveTaskWorkers.resize(l_InclusiveCount);

            for (size_t l_I = l_ExclusiveCount; l_I < l_WorkersCopy.size(); ++l_I)
            {
                TaskWorker* l_Worker = l_WorkersCopy[l_I];
                l_Worker->Suspend();

                delete l_Worker;
            }

            LOG_INFO("ThrTaskManager", "Set to %0 Exclusive Workers", l_ExclusiveCount);
            LOG_INFO("ThrTaskManager", "Set to %0 Inclusive Workers", l_InclusiveCount);
        }
        else if (p_Count > l_TaskWorkerCount)
        {
            const uint32 l_HardwareConcurency = std::thread::hardware_concurrency() - 1;
            const uint32 l_ExclusiveCount     = std::floor(p_Count * EXLUSIVE_CURRENCY_COUNT);

            LOG_ASSERT(l_ExclusiveCount, "ThrTaskManager", "ERROR: MONGOOSE. Please refer to SteerStone Documentation.");

            for (size_t l_I = m_ExclusiveTaskWorkers.size(); l_I < l_ExclusiveCount; ++l_I)
            {
                TaskWorker* l_Worker = new TaskWorker(WorkerType::Exclusive);

                /// Skip CPU 0 at first iteration, to save some CPU for the kernel
                l_Worker->SetCPUAffinty(static_cast<int32>((l_I + 1) % l_HardwareConcurency));
                l_Worker->SetName(Utils::StringBuilder("TaskWorker_%0", l_I));

                m_ExclusiveTaskWorkers.push_back(l_Worker);
            }

            const std::size_t l_InclusiveCount = (p_Count - l_ExclusiveCount) + m_ExclusiveTaskWorkers.size();

            for (size_t l_I = m_InclusiveTaskWorkers.size() + m_ExclusiveTaskWorkers.size(); l_I < l_InclusiveCount; ++l_I)
            {
                TaskWorker * l_Worker = new TaskWorker(WorkerType::Inclusive);

                l_Worker->SetCPUAffinty(static_cast<int32>((l_I + 1) % l_HardwareConcurency));
                l_Worker->SetName(Utils::StringBuilder("TaskWorker_%0", l_I));

                m_InclusiveTaskWorkers.push_back(l_Worker);
            }

            LOG_INFO("ThrTaskManager", "Set to %0 Exclusive Workers", l_ExclusiveCount);
            LOG_INFO("ThrTaskManager", "Set to %0 Inclusive Workers", (p_Count - l_ExclusiveCount));
        }
    }
    /// Set optimize task period
    /// @p_Period : New period
    void TaskManager::SetOptimizePeriod(uint64 p_Period)
    {
        m_OptimizeTask->SetTaskPeriod(p_Period);
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Optimize
    /// Only for Inclusive workers
    void TaskManager::Optimize()
    {
        m_Mutex.lock();

        if (m_InclusiveTaskWorkers.empty() || m_Tasks.empty())
        {
            m_Mutex.unlock();
            return;
        }

        std::vector<TaskWorker*> l_TaskWorkers = m_InclusiveTaskWorkers;
        m_Mutex.unlock();

        for (auto l_Worker : l_TaskWorkers)
            l_Worker->PreSuspend();

        for (auto l_Worker : l_TaskWorkers)
        {
            l_Worker->Suspend();
            l_Worker->PopAll();
        }

        m_Mutex.lock();

        std::sort(m_Tasks.begin(), m_Tasks.end(), [](const Task::Ptr & p_A, const Task::Ptr & p_B)
        {
            return (p_A->GetTaskAverageUpdateTime() < p_B->GetTaskAverageUpdateTime());
        });

        std::size_t l_WorkerCount       = m_InclusiveTaskWorkers.size();
        std::size_t l_CurrentWorker     = 0;

        for (auto & l_Task : m_Tasks)
        {
            m_InclusiveTaskWorkers[l_CurrentWorker % l_WorkerCount]->PushTask(l_Task);
            l_CurrentWorker++;
        }

        for (std::size_t l_I = 0; l_I < m_InclusiveTaskWorkers.size(); ++l_I)
            m_InclusiveTaskWorkers[l_I]->Resume();

        LOG_INFO("ThrTaskManager", "Optimized : %0 tasks on %1 inclusive workers", m_Tasks.size(), m_InclusiveTaskWorkers.size());

        m_Mutex.unlock();
    }

}   ///< namespace Threading
}   ///< namespace Core
}   ///< namespace SteerStone