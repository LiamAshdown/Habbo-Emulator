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

#ifndef _THREAD_POOL_WORKER_h
#define _THREAD_POOL_WORKER_h   
#include "Common/SharedDefines.h"
#include "ThreadPool.h"
#endif /* _THREAD_POOL_WORKER_h */

namespace SteerStone
{
    /// Class which holds our function queue from Thread Pool
    class Worker
    {
    public:
        /// Constructor
        /// @p_ThreadPool : Pass by reference ThreadPool
        Worker(ThreadPool& p_ThreadPool) : m_Pool(p_ThreadPool)
        {
            LaunchWorkerThread();
        }

        /// Launch our Worker thread to be ready to process incoming functions
        void LaunchWorkerThread();

    private:
        ThreadPool& m_Pool;
    };
} /// NAMESPACE STEERSTONE