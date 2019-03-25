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

#include "ThreadWorker.h"

namespace SteerStone
{
    /// Launch our Worker thread to be ready to process incoming functions
    void Worker::LaunchWorkerThread()
    {
        std::function<void()> l_Task;

        while (true)
        {
            {
                std::unique_lock<std::mutex> l_Guard(m_Pool.m_Mutex);

                while (!m_Pool.m_Stop && m_Pool.m_Tasks.empty())
                    m_Pool.m_Condition.wait(l_Guard);

                if (m_Pool.m_Stop)
                    break;

                l_Task = m_Pool.m_Tasks.front();
                m_Pool.m_Tasks.pop_front();

            }

            l_Task();
        }
    }

    /// Constructor
    /// @p_PoolSize : Number of workers to launch
    ThreadPool::ThreadPool(std::size_t poolSize) : m_Stop(false)
    {
        for (size_t i = 0; i < poolSize; ++i)
            m_Workers.push_back(std::thread(&Worker::LaunchWorkerThread, Worker(*this)));
    }

    /// Deconstructor
    ThreadPool::~ThreadPool()
    {
        m_Stop = true;
        m_Condition.notify_all();

        for (size_t i = 0; i < m_Workers.size(); ++i)
            m_Workers[i].join();
    }

    /// Constructor
    /// @p_Task : Function to pass into queue
    void ThreadPool::Enqueue(std::function<void()> task)
    {
        {
            std::unique_lock<std::mutex> l_Guard(m_Mutex);
            m_Tasks.push_back(std::function<void()>(task));
        }

        m_Condition.notify_one();
    }
} ///< NAMESPACE STEERSTONE