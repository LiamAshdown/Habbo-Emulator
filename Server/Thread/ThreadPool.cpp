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
//-----------------------------------------------//
#include "ThreadPool.h"
//-----------------------------------------------//
void Worker::operator()()
{
    std::function<void()> task;
    while (true)
    {
        {   // acquire lock
            std::unique_lock<std::mutex>
                lock(pool.mMutex);

            // look for a work item
            while (!pool.mStop && pool.mTasks.empty())
            { // if there are none wait for notification
                pool.mCondition.wait(lock);
            }

            if (pool.mStop) // exit if the pool is stopped
                return;

            // get the task from the queue
            task = pool.mTasks.front();
            pool.mTasks.pop_front();

        }   // release lock

        // execute the task
        task();
    }
}
//-----------------------------------------------//
// the constructor just launches some amount of workers
ThreadPool::ThreadPool(size_t threads)
    : mStop(false)
{
    for (size_t i = 0; i < threads; ++i)
        mWorkers.push_back(std::thread(Worker(*this)));
}
//-----------------------------------------------//
// the destructor joins all threads
ThreadPool::~ThreadPool()
{
    // stop all threads
    mStop = true;
    mCondition.notify_all();

    // join them
    for (size_t i = 0; i < mWorkers.size(); ++i)
        mWorkers[i].join();
}
//-----------------------------------------------//
void ThreadPool::enqueue(const std::function<void()>& task)
{
    { // acquire lock
        std::unique_lock<std::mutex> lock(mMutex);

        // add the task
        mTasks.push_back(std::function<void()>(task));
    } // release lock

    // wake up one thread
    mCondition.notify_one();
}
//-----------------------------------------------//