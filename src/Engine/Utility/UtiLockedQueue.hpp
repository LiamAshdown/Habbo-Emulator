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

#pragma once
#include <PCH/Precompiled.hpp>
#include <deque>
#include <mutex>
#include <atomic>
#include "Core/Core.hpp"

namespace SteerStone { namespace Core { namespace Utils {

    template <typename T> class LockedQueue
    {
        DISALLOW_COPY_AND_ASSIGN(LockedQueue);

        //////////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////////

    public:
        /// Constructor
        LockedQueue<T>() : m_ShutDown(false)
        {}

        /// Deconstructor
        ~LockedQueue<T>()
        {}

        //////////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////////

    public:
        /// Push object to storage
        /// @p_Value : Object we are pushing to queue
        void Add(const T& p_Object)
        {
            std::lock_guard<std::mutex> l_Guard(m_Lock);

            m_Queue.push_back(std::move(p_Object));
        }
        /// Get next result in queue
        /// @p_Object : Object being passed to
        bool Next(T& p_Object)
        {
            std::lock_guard<std::mutex> l_Guard(m_Lock);

            if (m_Queue.empty())
                return false;

            p_Object = m_Queue.front();
            m_Queue.pop_front();

            return true;
        }
        /// Get next result in queue
        /// @p_Object : Object being passed to
        /// @p_Filter : Check whether we can pass the object
        template<typename Filter> bool Next(T& p_Object, Filter& p_Filter)
        {
            std::lock_guard<std::mutex> l_Guard(m_Lock);

            if (m_Queue.empty())
                return false;

            p_Object = m_Queue.front();
            if (!p_Filter.Process(p_Object))
                return false;

            m_Queue.pop_front();

            return true;
        }

        /// Pop Front of queue
        void PopFront()
        {
            std::lock_guard<std::mutex> l_Guard(m_Lock);
            m_Queue.pop_front();
        }
        /// Pop Back of queue
        void PopBack()
        {
            std::lock_guard<std::mutex> l_Guard(m_Lock);
            m_Queue.pop_back();
        }

        /// Check if queue is empty
        bool Empty()
        {
            std::lock_guard<std::mutex> l_Guard(m_Lock);
            return m_Queue.empty();
        }

        /// Clear storage and delete objects
        void ShutDown()
        {
            std::unique_lock<std::mutex> l_Guard(m_Lock);

            while (!m_Queue.empty())
            {
                T& p_Object = m_Queue.front();

                DeleteQueuedObject(p_Object);

                m_Queue.pop();
            }

            m_ShutDown = true;
        }

    private:
        /// Delete object
        template<typename T> typename std::enable_if<std::is_pointer<T>::value>::type DeleteQueuedObject(T& p_Object) { delete p_Object; p_Object = nullptr; }

    private:
        std::deque<T> m_Queue;                ///< Storage for objects
        std::atomic_bool m_ShutDown;          ///< Shutdown
        std::mutex m_Lock;                    ///< Mutex
    };

}   ///< namespace Utility
}   ///< namespace Core
}   ///< namespace SteerStone