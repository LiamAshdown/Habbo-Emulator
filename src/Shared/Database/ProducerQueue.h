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

#ifndef _DATABASE_PRODUCER_QUEUE_h
#define _DATABASE_PRODUCER_QUEUE_h
#include "Common/SharedDefines.h"
#include <atomic>
#include <mutex>
#endif /* !_DATABASE_PRODUCER_QUEUE_h */

namespace SteerStone
{
    template <typename T>
    class ProducerQueue
    {
    public:
        /// Constructor
        ProducerQueue<T>() : m_ShutDown(false) {}

        /// Deconstructor
        ~ProducerQueue<T>() {}

    public:
        /// Push
        /// @p_Value : Object we are pushing to queue
        void Push(const T& p_Object)
        {
            std::unique_lock<std::mutex> l_Guard(m_Mutex);

            /// Don't push any new objects if we are in the process of shutting down
            if (m_ShutDown)
                return;

            m_Queue.push(p_Object);
        }

        /// Pop
        /// @p_Object : Object we are getting from queue
        bool Pop(T& p_Object)
        {
            std::unique_lock<std::mutex> l_Guard(m_Mutex);

            if (m_Queue.empty() || m_ShutDown)
                return false;

            p_Object = m_Queue.front();

            m_Queue.pop();

            return true;
        }

        /// Shutdown
        /// Clear storage and delete objects
        void ShutDown()
        {
            std::unique_lock<std::mutex> l_Guard(m_Mutex);

            m_ShutDown = true;

            while (!m_Queue.empty())
            {
                T& p_Object = m_Queue.front();

                DeleteQueuedObject(p_Object);

                m_Queue.pop();
            }
        }

    private:
        /// Only accept new pointers
        template<typename T>
        typename std::enable_if<std::is_pointer<T>::value>::type DeleteQueuedObject(T& p_Object) { delete p_Object; p_Object = nullptr; }

    private:
        std::queue<T> m_Queue;
        std::mutex m_Mutex;
        std::atomic<bool> m_ShutDown;
    };
} ///< NAMESPACE STEERSTONE