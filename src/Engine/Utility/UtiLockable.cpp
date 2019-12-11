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

#include <Precompiled.hpp>

#include "UtiLockable.hpp"

namespace SteerStone { namespace Core { namespace Utils {

    /// Lock read
    void LockableBase::LockRead()
    {
        Lock();
    }
    /// Lock write
    void LockableBase::LockWrite()
    {
        Lock();
    }
    /// Try lock read
    bool LockableBase::TryLockRead()
    {
        return TryLock();
    }
    /// Try lock write
    bool LockableBase::TryLockWrite()
    {
        return TryLock();
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Lock
    void Lockable::Lock()
    {
        m_Lock.lock();
    }
    /// Try lock
    bool Lockable::TryLock()
    {
        return m_Lock.try_lock();
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Unlock
    void Lockable::Unlock()
    {
        m_Lock.unlock();
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Lock
    void LockableReadWrite::Lock()
    {
        m_Lock.lock();
    }
    /// Try lock
    bool LockableReadWrite::TryLock()
    {
        return m_Lock.try_lock();
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Lock read
    void LockableReadWrite::LockRead()
    {
        m_Lock.lock_shared();
    }
    /// Lock write
    void LockableReadWrite::LockWrite()
    {
        m_Lock.lock();
    }
    /// Try lock read
    bool LockableReadWrite::TryLockRead()
    {
        return m_Lock.try_lock_shared();
    }
    /// Try lock write
    bool LockableReadWrite::TryLockWrite()
    {
        return m_Lock.try_lock();
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Unlock
    void LockableReadWrite::Unlock()
    {
        m_Lock.unlock();
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Lock
    void NullLockable::Lock()
    {

    }
    /// Try lock
    bool NullLockable::TryLock()
    {
        return true;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Unlock
    void NullLockable::Unlock()
    {

    }

}   ///< namespace Utils
}   ///< namespace Core
}   ///< namespace SteerStone
