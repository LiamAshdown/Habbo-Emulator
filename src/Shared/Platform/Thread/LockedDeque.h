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

#ifndef _THREAD_LOCKED_DEQUE_h
#define _THREAD_LOCKED_DEQUE_h   
#include "Common/SharedDefines.h"
#include <mutex>
#endif /* _THREAD_LOCKED_DEQUE_h */

namespace SteerStone
{
    /// Provides Thread safe deque
    template <class T>
    class LockedDeque
    {
    public:
        /// Constructor
        LockedDeque() {}

        /// Deconstructor
        ~LockedDeque() {}

    public:
        /// Add
        /// Add the object to our storage
        /// @p_Object : Object we are adding to our storage
        void Add(T const& p_Object)
        {
            std::unique_lock<std::mutex> l_Guard(m_Mutex);

            m_Deque.push_back(p_Object);
        }

        /// Next
        /// Retrieve next object at the front of the storage
        bool Next(T& p_Object)
        {
            std::unique_lock<std::mutex> l_Guard(m_Mutex);

            if (!m_Deque.empty())
                return false;

            p_Object = m_Deque.front();

            m_Deque.pop_front();

            return true;
        }

        /// Next
        /// Retrieve next at the back of the storage
        bool Previous(T& p_Object)
        {
            std::unique_lock<std::mutex> l_Guard(m_Mutex);

            if (m_Deque.empty())
                return false;

            p_Object = m_Deque.back();

            m_Deque.pop_back();

            return true;
        }

        /// End
        /// Return our object at end of storage without popping
        /// @p_Increment : Retrieve a specific position if specified
        void End(T& p_Object, int32 p_Increment = 0)
        {
            std::unique_lock<std::mutex> l_Guard(m_Mutex);

            p_Object = m_Deque.at(p_Increment);
        }

        /// End
        /// Return our object at front of storage without popping6
        /// @p_Increment : Retrieve a specific position if specified
        void Front(T& p_Object, int32 p_Increment = 0)
        {
            std::unique_lock<std::mutex> l_Guard(m_Mutex);

            p_Object = m_Deque.at(p_Increment);
        }

        /// PopFront
        /// Remove object from beginning of storage
        void PopFront()
        {
            std::unique_lock<std::mutex> l_Guard(m_Mutex);

            m_Deque.pop_front();
        }

        /// PopBack
        /// Remove object from end of storage
        void PopBack()
        {
            std::unique_lock<std::mutex> l_Guard(m_Mutex);

            m_Deque.pop_front();
        }

        /// Size
        /// Returns size of our storage
        std::size_t Size()
        {
            return m_Deque.size();
        }

        /// Clear
        /// Clear storage
        void Clear()
        {
            std::unique_lock<std::mutex> l_Guard(m_Mutex);
            m_Deque.clear();

        }
        
        /// Empty
        /// Returns if our storage is empty or not
        bool Empty()
        {
            return m_Deque.empty();
        }

    private:
        std::mutex m_Mutex;         ///< Ensure thread safety
        std::deque<T> m_Deque;      ///< Deque Storage to hold our objects
    };

} /// NAMESPACE STEERSTONE