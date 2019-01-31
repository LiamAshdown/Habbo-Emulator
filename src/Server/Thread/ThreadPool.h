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

#ifndef _QuadEmu_ThreadPool_h_
#define _QuadEmu_ThreadPool_h_
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <stdexcept>
#include "../../Common/SharedDefines.h"
#endif /* _QuadEmu_ThreadPool_ */

class ThreadPool;

class Worker 
{
public:
    Worker(ThreadPool &s) : pool(s) { }
    void operator()();
private:
    ThreadPool &pool;
};

class ThreadPool
{
public:
    friend class Worker;

    ThreadPool(std::size_t);
    void enqueue(const std::function<void()>& task);
    ~ThreadPool();

private:

    // need to keep track of threads so we can join them
    std::vector< std::thread > mWorkers;

    // the task queue
    std::deque< std::function<void()> > mTasks;

    // synchronization
    std::mutex mMutex;
    std::condition_variable mCondition;
    bool mStop;
};