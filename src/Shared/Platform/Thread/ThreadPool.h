/*
* Liam Ashdown
* Copyright (C) 2018
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

#ifndef _THREADPOOL_THREADPOOL_h
#define _THREADPOOL_THREADPOOL_h
#include "Common/SharedDefines.h"
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <stdexcept>
#endif /* _THREADPOOL_THREADPOOL_h */

namespace SteerStone
{
    /// Thread Pooling class for handling functions
    class ThreadPool
    {
    public:
        friend class Worker;

    public:
        /// Constructor
        /// @p_PoolSize : Number of workers to launch
        ThreadPool(const std::size_t p_PoolSize);
        /// Deconstructor
        ~ThreadPool();

    public:
        /// Constructor
        /// @p_Task : Function to pass into queue
        void Enqueue(std::function<void()> p_Task);

    private:
        std::vector<std::thread> m_Workers;                                    ///< Worker Threads
        std::deque<std::function<void()>> m_Tasks;                             ///< Task execute
        std::condition_variable m_Condition;                                   ///< Condition
        std::mutex m_Mutex;                                                     ///< Mutex
        bool m_Stop;                                                            ///< Stop operation of Thread Pool
    };
} ///< NAMESPACE STEERSTONE