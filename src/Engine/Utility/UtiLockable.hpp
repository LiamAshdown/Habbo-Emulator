/*
* Liam Ashdown
* HardCPP (Merydwin)
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

#include "Core/Core.hpp"

#include <mutex>
#include <shared_mutex>

namespace SteerStone { namespace Core { namespace Utils {

    /// Base lock object
    class LockableBase
    {
        public:
            /// Lock
            virtual void Lock() = 0;
            /// Lock read
            virtual void LockRead();
            /// Lock write
            virtual void LockWrite();
            /// TryLock
            virtual bool TryLock() = 0;
            /// Try lock read
            virtual bool TryLockRead();
            /// Try lock write
            virtual bool TryLockWrite();

            /// Unlock
            virtual void Unlock() = 0;

    };

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Simple lock object
    class Lockable : public LockableBase
    {
        public:
            /// Lock
            void Lock() override final;
            /// TryLock
            bool TryLock() override final;

            /// Unlock
            void Unlock() override final;

        private:
            std::mutex m_Lock;      ///< Mutex

    };

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// RW lock object
    class LockableReadWrite : public LockableBase
    {
        public:
            /// Lock
            void Lock() override final;
            /// TryLock
            bool TryLock() override final;
            /// Lock read
            void LockRead() override final;
            /// Lock write
            void LockWrite() override final;
            /// Try lock read
            bool TryLockRead() override final;
            /// Try lock write
            bool TryLockWrite() override final;

            /// Unlock
            virtual void Unlock() override final;

        private:
            std::shared_mutex m_Lock;   ///< Mutex

    };

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    class NullLockable : public LockableBase
    {
        public:
            /// Lock
            void Lock() override final;
            /// TryLock
            bool TryLock() override final;

            /// Unlock
            void Unlock() override final;
    };

}   ///< namespace Utils
}   ///< namespace Core
}   ///< namespace SteerStone
