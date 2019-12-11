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

#ifndef _SINGLETON_HPP_
#define _SINGLETON_HPP_
#include <PCH/Precompiled.hpp>

#include "Core/Core.hpp"

/// Static singleton declaration
#define SINGLETON_S_D(Type)     DISALLOW_COPY_AND_ASSIGN(Type);    \
                                private:                           \
                                    static Type m_Instance;        \
                                    Type();                        \
                                    ~Type();                       \
                                public:                            \
                                    static Type * GetSingleton();  

/// Static singleton implementation
#define SINGLETON_S_I(Type)     Type Type::m_Instance = Type();    \
                                Type * Type::GetSingleton()        \
                                {                                  \
                                    return &m_Instance;            \
                                }

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

/// Pointer singleton declaration
#define SINGLETON_P_D(Type)     DISALLOW_COPY_AND_ASSIGN(Type);    \
                                private:                           \
                                    static Type * m_Instance;      \
                                    Type();                        \
                                    ~Type();                       \
                                public:                            \
                                    static Type * GetSingleton();  \
                                    static void DestroySingleton();

/// Static singleton implementation
#define SINGLETON_P_I(Type)     Type * Type::m_Instance = 0;                        \
                                Type * Type::GetSingleton()                         \
                                {                                                   \
                                    if (!m_Instance)                                \
                                    {                                               \
                                        m_Instance = new Type;                      \
                                        ::SteerStone::Core::SingletonGarbageCollector::GetSingleton()->Add<Type>(m_Instance); \
                                    }                                               \
                                    return m_Instance;                              \
                                }                                                   \
                                void Type::DestroySingleton()                       \
                                {                                                   \
                                    if (m_Instance)                                 \
                                    {                                               \
                                        ::SteerStone::Core::SingletonGarbageCollector::GetSingleton()->Remove(m_Instance);    \
                                        delete m_Instance;                          \
                                        m_Instance = 0;                             \
                                    }                                               \
                                }                                                   \

namespace SteerStone { namespace Core { 

    /// Singleton
    class SingletonGarbageCollector
    {
        SINGLETON_P_D(SingletonGarbageCollector);

        typedef void(*DestroyFunction)();

        /// Element
        struct Element
        {
            void* Instance;
            DestroyFunction Destroy;
        };

    public:
        /// Add singleton
        template<class T> void Add(T* p_Singleton)
        {
            Add((void*)p_Singleton, &T::DestroySingleton);
        }

        /// Add singleton
        /// @p_Singleton : Singleton instance
        /// @p_Function  : Clean up function
        void Add(void* p_Singleton, DestroyFunction p_Function);

        /// Remove singleton
        /// @p_Singleton : Singleton instance
        void Remove(void* p_Singleton);

        /// Destroy all
        static void Clear();

    private:
        std::mutex  m_Mutex;                ///< Mutex
        bool        m_InDelete;             ///< Is in destruction phase

        std::vector<Element> m_Singletons;  ///< Singletons
    };

} ///< SteerStone
} ///< Core

#endif /* _SINGLETON_HPP_ */