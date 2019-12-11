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

#include "Singleton.hpp"
#include "Base.hpp"

namespace SteerStone { namespace Core {

    SINGLETON_P_I(SingletonGarbageCollector)

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    SingletonGarbageCollector::SingletonGarbageCollector()
    : m_InDelete(false)
    {

    }
    /// Destructor
    SingletonGarbageCollector::~SingletonGarbageCollector()
    {
        m_InDelete = true;

        std::lock_guard<std::mutex> l_Lock(m_Mutex);

        for (size_t l_I = 0; l_I < m_Singletons.size(); ++l_I)
            m_Singletons[l_I].Destroy();

        m_Singletons.clear();

        Logger::Base::DestroySingleton();
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Add singleton
    /// @p_Singleton : Singleton instance
    /// @p_Function  : Clean up function
    void SingletonGarbageCollector::Add(void* p_Singleton, DestroyFunction p_Function)
    {
        if (p_Singleton == Logger::Base::GetSingleton())
            return;

        std::lock_guard<std::mutex> l_Lock(m_Mutex);

        Element l_Block;
        l_Block.Instance = p_Singleton;
        l_Block.Destroy = p_Function;

        m_Singletons.push_back(l_Block);
    }
    /// Remove singleton
    /// @p_Singleton : Singleton instance
    void SingletonGarbageCollector::Remove(void* p_Singleton)
    {
        if (m_InDelete)
            return;

        std::lock_guard<std::mutex> l_Lock(m_Mutex);

        for (std::vector<Element>::iterator l_Itr = m_Singletons.begin(); l_Itr != m_Singletons.end(); ++l_Itr)
        {
            if (l_Itr->Instance == p_Singleton)
            {
                m_Singletons.erase(l_Itr);
                return;
            }
        }
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Destroy all
    void SingletonGarbageCollector::Clear()
    {
        SingletonGarbageCollector::DestroySingleton();
    }

}   ///< namespace Core
}   ///< namespace SteerStone