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

#ifndef _Quad_ThreadPool_h_
#define _Quad_ThreadPool_h_
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <stdexcept>
#include "Common/SharedDefines.h"

class ThreadPool
{
public:
    friend class Worker;
public:
    ThreadPool(std::size_t poolSize);
    ~ThreadPool();

public:
    void Enqueue(std::function<void()> task);

private:
    std::vector<std::thread> mWorkers;

    // the task queue
    std::deque<std::function<void()>> mTasks;

    std::mutex mMutex;
    std::condition_variable mCondition;
    bool mStop;
};

#endif /* _Quad_ThreadPool_ */