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
//-----------------------------------------------//
#include "ThreadPool.h"
#include "ThreadWorker.h"
//-----------------------------------------------//
void Worker::operator()()
{
    std::function<void()> task;
    while (true)
    {
        {
            std::unique_lock<std::mutex> guard(mPool.mMutex);

            while (!mPool.mStop && mPool.mTasks.empty())
                mPool.mCondition.wait(guard);

            if (mPool.mStop)
                break;

            task = mPool.mTasks.front();
            mPool.mTasks.pop_front();

        }

        task();
    }
}
//-----------------------------------------------//
ThreadPool::ThreadPool(std::size_t poolSize) : mStop(false)
{
    for (size_t i = 0; i < poolSize; ++i)
        mWorkers.push_back(std::thread(Worker(*this)));
}
//-----------------------------------------------//
ThreadPool::~ThreadPool()
{
    mStop = true;
    mCondition.notify_all();

    for (size_t i = 0; i < mWorkers.size(); ++i)
        mWorkers[i].join();
}
//-----------------------------------------------//
void ThreadPool::Enqueue(std::function<void()> task)
{
    {
        std::unique_lock<std::mutex> lock(mMutex);
        mTasks.push_back(std::function<void()>(task));
    }

    mCondition.notify_one();
}
//-----------------------------------------------//