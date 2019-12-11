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
#include <condition_variable>
#include <atomic>

#include "Core/Core.hpp"

namespace SteerStone { namespace Core { namespace Database {

    template <typename T> class ProducerQueue
    {
        DISALLOW_COPY_AND_ASSIGN(ProducerQueue);

        //////////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////////

    public:
        /// Constructor
        ProducerQueue<T>() : m_ShutDown(false) 
        {}

        /// Deconstructor
        ~ProducerQueue<T>() 
        {}

        //////////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////////

    public:
        /// Push object to storage
        /// @p_Value : Object we are pushing to queue
        void Push(const T& p_Object)
        {
            std::lock_guard<std::mutex> l_Guard(m_Lock);

            m_Queue.push(std::move(p_Object));

            m_Condition.notify_one();
        }

        /// Pass access to object before removing from storage
        /// @p_Object : Object we are accessing from storage
        void WaitAndPop(T& p_Object)
        {
            std::unique_lock<std::mutex> l_Guard(m_Lock);

            if (m_Queue.empty() || !m_ShutDown)
                m_Condition.wait(l_Guard);

            if (m_Queue.empty() || m_ShutDown)
                return;

            p_Object = m_Queue.front();

            m_Queue.pop();
        }
        
        /// Get Size
        const std::size_t GetSize()
        {
            std::lock_guard<std::mutex> l_Guard(m_Lock);

            return m_Queue.size();
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

            m_Condition.notify_all();
        }

    private:
        /// Delete object
        template<typename T> typename std::enable_if<std::is_pointer<T>::value>::type DeleteQueuedObject(T& p_Object) { delete p_Object; p_Object = nullptr; }

    private:
        std::queue<T> m_Queue;                ///< Storage for objects
        std::atomic_bool m_ShutDown;          ///< Shutdown
        std::condition_variable m_Condition;  ///< Condition
        std::mutex m_Lock;                    ///< Mutex
    };

}   ///< namespace Database
}   ///< namespace Core
}   ///< namespace SteerStone