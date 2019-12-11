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

#include "Core/Core.hpp"

namespace SteerStone { namespace Core { namespace Utils {

    /// Flags
    enum ObjectGuardFlags
    {
        ObjectGuardFlags_None              = 0,
        ObjectGuardFlags_AlreadyLocked     = 1 << 0,
        ObjectGuardFlags_Ignore            = 1 << 1,
    };

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Object guard
    template<typename T> class ObjectGuard
    {
        public:
            /// Constructor
            /// @p_Object : Object instance to guard
            /// @p_Flags  : Guard flags
            ObjectGuard(T * p_Object, ObjectGuardFlags p_Flags = ObjectGuardFlags_None);
            /// Destructor
            ~ObjectGuard();

            /// Overload indirection operator
            T * operator-> ();
            /// Overload deference operator
            T * operator* ();

        private:
            T *                 m_Object;    ///< Object
            ObjectGuardFlags    m_Flags;     ///< Flags

    };

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Read object guard
    template<typename T> class ObjectReadGuard
    {
        public:
            /// Constructor
            /// @p_Object : Object instance to guard
            /// @p_Flags  : Guard flags
            ObjectReadGuard(T * p_Object, ObjectGuardFlags p_Flags = ObjectGuardFlags_None);
            /// Destructor
            ~ObjectReadGuard();

            /// Overload indirection operator
            T * operator-> ();
            /// Overload deference operator
            T * operator* ();

        private:
            T *                 m_Object;    ///< Object
            ObjectGuardFlags    m_Flags;     ///< Flags

    };

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Write object guard
    template<typename T> class ObjectWriteGuard
    {
        public:
            /// Constructor
            /// @p_Object : Object instance to guard
            /// @p_Flags  : Guard flags
            ObjectWriteGuard(T * p_Object, ObjectGuardFlags p_Flags = ObjectGuardFlags_None);
            /// Destructor
            ~ObjectWriteGuard();

            /// Overload indirection operator
            T * operator-> ();
            /// Overload deference operator
            T * operator* ();

        private:
            T *                 m_Object;    ///< Object
            ObjectGuardFlags    m_Flags;     ///< Flags

    };

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    /// @p_Object : Object instance to guard
    /// @p_Flags  : Guard flags
    template<typename T> ObjectGuard<T>::ObjectGuard(T * p_Object, ObjectGuardFlags p_Flags)
        : m_Object(p_Object), m_Flags(p_Flags)
    {
        if (m_Flags & ObjectGuardFlags_Ignore)
            return;

        if ((m_Flags & ObjectGuardFlags_AlreadyLocked) == 0)
            m_Object->Lock();
    }
    /// Destructor
    template<typename T> ObjectGuard<T>::~ObjectGuard()
    {
        if (m_Flags & ObjectGuardFlags_Ignore)
            return;

        m_Object->Unlock();
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Overload indirection operator
    template<typename T> T * ObjectGuard<T>::operator-> ()
    {
        return m_Object;
    }
    /// Overload deference operator
    template<typename T> T * ObjectGuard<T>::operator* ()
    {
        return m_Object;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    /// @p_Object : Object instance to guard
    /// @p_Flags  : Guard flags
    template<typename T> ObjectReadGuard<T>::ObjectReadGuard(T * p_Object, ObjectGuardFlags p_Flags)
        : m_Object(p_Object), m_Flags(p_Flags)
    {
        if (m_Flags & ObjectGuardFlags_Ignore)
            return;

        if ((m_Flags & ObjectGuardFlags_AlreadyLocked) == 0)
            m_Object->LockRead();
    }
    /// Destructor
    template<typename T> ObjectReadGuard<T>::~ObjectReadGuard()
    {
        if (m_Flags & ObjectGuardFlags_Ignore)
            return;

        m_Object->Unlock();
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Overload indirection operator
    template<typename T> T * ObjectReadGuard<T>::operator-> ()
    {
        return m_Object;
    }
    /// Overload deference operator
    template<typename T> T * ObjectReadGuard<T>::operator* ()
    {
        return m_Object;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    /// @p_Object : Object instance to guard
    /// @p_Flags  : Guard flags
    template<typename T> ObjectWriteGuard<T>::ObjectWriteGuard(T * p_Object, ObjectGuardFlags p_Flags)
        : m_Object(p_Object), m_Flags(p_Flags)
    {
        if (m_Flags & ObjectGuardFlags_Ignore)
            return;

        if ((m_Flags & ObjectGuardFlags_AlreadyLocked) == 0)
            m_Object->LockWrite();
    }
    /// Destructor
    template<typename T> ObjectWriteGuard<T>::~ObjectWriteGuard()
    {
        if (m_Flags & ObjectGuardFlags_Ignore)
            return;

        m_Object->Unlock();
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Overload indirection operator
    template<typename T> T * ObjectWriteGuard<T>::operator-> ()
    {
        return m_Object;
    }
    /// Overload deference operator
    template<typename T> T * ObjectWriteGuard<T>::operator* ()
    {
        return m_Object;
    }

}   ///< namespace Utils
}   ///< namespace Core
}   ///< namespace SteerStone
