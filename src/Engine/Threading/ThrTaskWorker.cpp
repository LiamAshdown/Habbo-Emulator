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
#include "Threading/ThrThread.hpp"
#include "Threading/ThrThisThread.hpp"

#include "Logger/Base.hpp"

#include <chrono>

namespace SteerStone { namespace Core { namespace Threading {

    /// Constructor
    /// @p_WorkerType : Type of Worker
    TaskWorker::TaskWorker(WorkerType p_WorkerType)
        : m_Name("ThrTaskWorker"), m_CPUAffinity(0), m_IsRunning(true), m_TotalRunTime(0), m_TotalRunCount(0), m_AverageRunTime(0), m_WorkerType(p_WorkerType)
    {
        m_Thread = new std::thread([this]() { UpdateThread(); });
    }
    /// Destructor
    TaskWorker::~TaskWorker()
    {
        m_Mutex.lock();
        m_IsRunning = false;
        m_Mutex.unlock();

        PopAll();

        if (m_Thread)
        {
            if (m_Thread->joinable())
                m_Thread->join();

            delete m_Thread;
            m_Thread = nullptr;
        }
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Push task
    void TaskWorker::PushTask(const Task::Ptr & p_Task)
    {
        std::lock_guard<std::recursive_mutex> l_Lock(m_Mutex);

        m_Tasks.push_back(p_Task);
    }
    /// Pop task
    void TaskWorker::PopTask(const Task::Ptr & p_Task)
    {
        std::lock_guard<std::recursive_mutex> l_Lock(m_Mutex);

        auto l_It = std::find(m_Tasks.begin(), m_Tasks.end(), p_Task);

        if (l_It != m_Tasks.end())
            m_Tasks.erase(l_It);
    }
    /// Pop all
    void TaskWorker::PopAll()
    {
        std::lock_guard<std::recursive_mutex> l_Lock(m_Mutex);
        m_Tasks.clear();
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Have task
    bool TaskWorker::HaveTask(const Task::Ptr & p_Task)
    {
        std::lock_guard<std::recursive_mutex> l_Lock(m_Mutex);

        return std::find(m_Tasks.begin(), m_Tasks.end(), p_Task) != m_Tasks.end();
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Get flags
    uint64 TaskWorker::GetTotalRunTime() const
    {
        return m_TotalRunTime;
    }
    /// Get flags
    uint64 TaskWorker::GetTotalRunCount() const
    {
        return m_TotalRunCount;
    }
    /// Get average execution time
    uint64 TaskWorker::GetAverageUpdateTime() const
    {
        return m_AverageRunTime;
    }
    /// Get Task Size
    std::size_t TaskWorker::GetTaskSize() const
    {
        return m_Tasks.size();
    }
    /// Reset avg update time
    void TaskWorker::ResetAverageUpdateTime()
    {
        std::lock_guard<std::recursive_mutex> l_Lock(m_Mutex);

        m_TotalRunTime      = 0;
        m_TotalRunCount     = 0;
        m_AverageRunTime    = 0;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// PreSuspend the worker
    void TaskWorker::PreSuspend()
    {
        if (!m_Thread)
            return;

        m_IsRunning = false;
    }
    /// Suspend the worker
    void TaskWorker::Suspend()
    {
        if (!m_Thread)
            return;

        m_Mutex.lock();
        m_IsRunning         = false;
        m_AverageRunTime    = 0;
        m_TotalRunTime      = 0;
        m_TotalRunCount     = 0;
        m_Mutex.unlock();

        if (m_Thread->joinable())
            m_Thread->join();

        delete m_Thread;

        m_Thread = nullptr;
    }
    /// Resume the worker
    void TaskWorker::Resume()
    {
        if (m_Thread)
            return;

        m_Mutex.lock();
        m_IsRunning = true;
        m_Thread    = new std::thread([this]() { UpdateThread(); });
        m_Mutex.unlock();

        SetCPUAffinty(m_CPUAffinity);
        SetName(m_Name);
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Set thread CPU affinity
    /// @p_Affinity : Affinity
    void TaskWorker::SetCPUAffinty(int32 p_Affinity)
    {
        m_CPUAffinity = p_Affinity;

        if (!m_Thread)
            return;

        Thread::SetThreadCPUAffinity(m_Thread->native_handle(), p_Affinity);
    }
    /// Set task worker name
    /// @p_Name : New name
    void TaskWorker::SetName(const std::string & p_Name)
    {
        m_Name = p_Name;

        if (!m_Thread)
            return;

        Thread::SetThreadName(m_Thread->native_handle(), m_Name);
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Update thread
    void TaskWorker::UpdateThread()
    {
        Diagnostic::StopWatch l_TasksMonitor;
        Diagnostic::StopWatch l_DiffTimer;

        l_DiffTimer.Start();
        while (m_IsRunning)
        {
            l_TasksMonitor.Start();

            m_Mutex.lock();
            std::vector<Task::Ptr> l_Copy(m_Tasks);
            m_Mutex.unlock();

            uint64 l_DiffTime = l_DiffTimer.GetElapsed();
            for (auto & l_Task : l_Copy)
            {
                if (!m_IsRunning)
                    break;

                if (l_Task->UpdateTaskTime(l_DiffTime))
                    if (!l_Task->UpdateTask())
                        TaskManager::GetSingleton()->PopTask(l_Task);
            }

            l_DiffTimer.Reset();

            /// Fast exit
            if (!m_IsRunning)
                return;

            l_TasksMonitor.Stop();

            m_TotalRunTime += l_TasksMonitor.GetElapsed();
            m_TotalRunCount++;

            m_AverageRunTime = static_cast<uint32>(m_TotalRunTime / m_TotalRunCount);

            ThisThread::SleepFor(std::chrono::milliseconds(1));
        }
    }

//     /// Update thread
//     void TaskWorker::UpdateThread()
//     {
//         Diagnostic::StopWatch l_TasksMonitor;
//         Diagnostic::StopWatch l_DiffTimer;
//
//         std::vector<Task::Ptr> l_TasksToUpdate;
//         std::vector<Task::Ptr> l_TasksToPop;
//
//         l_DiffTimer.Start();
//         while (m_IsRunning)
//         {
//             l_TasksMonitor.Start();
//
//             m_Mutex.lock();
//
//             l_TasksToUpdate.clear();
//             l_TasksToPop.clear();
//
//             uint64 l_DiffTime = l_DiffTimer.GetElapsed();
//             for (auto & l_Task : m_Tasks)
//             {
//                 if (l_Task->UpdateTaskTime(l_DiffTime))
//                     l_TasksToUpdate.push_back(l_Task);
//             }
//
//             m_Mutex.unlock();
//
//             l_DiffTimer.Reset();
//
//             for (auto & l_Task : l_TasksToUpdate)
//             {
//                 if (!m_IsRunning)
//                     break;
//
//                 if (!l_Task->UpdateTask())
//                     l_TasksToPop.push_back(l_Task);
//             }
//
//             for (auto & l_Task : l_TasksToPop)
//                 TaskManager::GetSingleton()->PopTask(l_Task);
//
//             /// Fast exit
//             if (!m_IsRunning)
//                 return;
//
//             l_TasksMonitor.Stop();
//
//             m_TotalRunTime += l_TasksMonitor.GetElapsed();
//             m_TotalRunCount++;
//
//             m_AverageRunTime = static_cast<uint32>(m_TotalRunTime / m_TotalRunCount);
//
//             ThisThread::SleepFor(std::chrono::milliseconds(1));
//         }
//     }

}   ///< namespace Threading
}   ///< namespace Core
}   ///< namespace SteerStone