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

#include <PCH/Precompiled.hpp>

#include "Threading/ThrThread.hpp"

#include "Logger/Base.hpp"

#if defined(_WIN32)
#   include <windows.h>
#elif defined(__APPLE__)
    #include <sys/ucontext.h>
    #include <sys/types.h>
    #include <mach/thread_policy.h>
    #include <mach/task_info.h>
    #include <sys/types.h>
    #include <sys/sysctl.h>
    #include <mach/thread_act.h>
    #include <pthread.h>

    typedef struct cpu_set {
        uint32_t    count;
    } cpu_set_t;

    static inline void
    CPU_ZERO(cpu_set_t *cs) { cs->count = 0; }

    static inline void
    CPU_SET(int num, cpu_set_t *cs) { cs->count |= (1 << num); }

    static inline int
    CPU_ISSET(int num, cpu_set_t *cs) { return (cs->count & (1 << num)); }

    int pthread_setaffinity_np(pthread_t p_Thread, size_t p_CPUSetSize, cpu_set_t * p_CPUSet)
    {
        int l_Core = 0;

        for (l_Core = 0; l_Core < 8 * p_CPUSetSize; l_Core++)
        {
            if (CPU_ISSET(l_Core, p_CPUSet))
                break;
        }

        struct thread_affinity_policy l_Policy;
        l_Policy.affinity_tag = l_Core;

        auto l_MachThreadHandler = pthread_mach_thread_np(p_Thread);
        thread_policy_set(l_MachThreadHandler, THREAD_AFFINITY_POLICY, reinterpret_cast<thread_policy_t>(&l_Policy), THREAD_AFFINITY_POLICY_COUNT);

        return 0;
    }
#elif defined(linux)
    #include <pthread.h>
    #include <unistd.h>
    #include <sched.h>
    #include <errno.h>
    #include <sys/resource.h>
    #include <sys/syscall.h>
#else
#   error "Unsuported platform in ThrThread.cpp"
#endif

namespace SteerStone { namespace Core { namespace Threading {

    std::mutex                      Thread::m_Mutex;
    std::map<uint64, std::string>   Thread::m_ThreadsName;

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Get thread name
    /// @p_Handle : Thread handle
    std::string Thread::GetThreadName(std::thread::native_handle_type p_Handle)
    {
        uint64 l_ThreadID = 0;

#if defined(_WIN32)
        l_ThreadID = static_cast<uint64>(GetThreadId(p_Handle));
#else
        pthread_t l_Handle = reinterpret_cast<pthread_t>(p_Handle);

        union
        {
            pthread_t Ptr;
            uint64 ID;
        } l_Converter;

        l_Converter.Ptr = l_Handle;
        l_ThreadID      = l_Converter.ID;
#endif

        std::lock_guard<std::mutex> l_Lock(m_Mutex);

        auto l_It = m_ThreadsName.find(l_ThreadID);
        if (l_It == m_ThreadsName.end())
            return Utils::StringBuilder("Thread_0x%0", Utils::Converter<std::string>::ToString(p_Handle));

        return l_It->second;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Set thread name
    /// @p_Handle   : Thread handle
    /// @p_Name     : Thread name
    void Thread::SetThreadName(std::thread::native_handle_type p_Handle, const std::string & p_Name)
    {
        uint64 l_FakeThreadID = 0;

#if defined(_WIN32)
        const DWORD MS_VC_EXCEPTION = 0x406D1388;

#pragma pack(push,8)
        typedef struct tagTHREADNAME_INFO
        {
            DWORD dwType;       ///< Must be 0x1000.
            LPCSTR szName;      ///< Pointer to name (in user addr space).
            DWORD dwThreadID;   ///< Thread ID (-1=caller thread).
            DWORD dwFlags;      ///< Reserved for future use, must be zero.
        } THREADNAME_INFO;
#pragma pack(pop)

        THREADNAME_INFO l_Info;
        l_Info.dwType       = 0x1000;
        l_Info.szName       = p_Name.c_str();
        l_Info.dwThreadID   = GetThreadId(p_Handle);
        l_Info.dwFlags      = 0;

        l_FakeThreadID = GetThreadId(p_Handle);

#pragma warning(push)
#pragma warning(disable: 6320 6322)
        __try {
            RaiseException(MS_VC_EXCEPTION, 0, sizeof(l_Info) / sizeof(ULONG_PTR), (ULONG_PTR*)&l_Info);

            m_Mutex.lock();
            m_ThreadsName[l_FakeThreadID] = p_Name;
            m_Mutex.unlock();
        }
        __except (EXCEPTION_EXECUTE_HANDLER) {
        }
#pragma warning(pop)
#else

        pthread_t l_Handle = reinterpret_cast<pthread_t>(p_Handle);

#if defined(linux)
        pthread_setname_np(p_Handle, p_Name.c_str());
#endif

        union
        {
            pthread_t Ptr;
            uint64 ID;
        } l_Converter;

        l_Converter.Ptr = l_Handle;
        l_FakeThreadID  = l_Converter.ID;

        m_Mutex.lock();
        m_ThreadsName[l_FakeThreadID] = p_Name;
        m_Mutex.unlock();
#endif
    }
    /// Set thread CPU affinity
    /// @p_Handle   : Thread CPU
    /// @p_CPU      : CPU ID from 0
    void Thread::SetThreadCPUAffinity(std::thread::native_handle_type p_Handle, uint32 p_CPU)
    {
#ifdef WIN32
        if (!SetThreadAffinityMask(p_Handle, static_cast<int64>(1) << static_cast<int64>(p_CPU)))
        {
            LOG_ERROR("ThrThread", R"LOG(Failed to set thread "%0" CPU affinity to %1)LOG", GetThreadName(p_Handle), p_CPU);
            return;
        }
#else
        cpu_set_t l_CPUSet;
        CPU_ZERO(&l_CPUSet);
        CPU_SET(p_CPU, &l_CPUSet);

        /// On MacOS we can't force the kernel to stick a thread to a specific CPU core
        /// but the following call on MacOS will make a "recommendation" to the kernel and
        /// he will try to exec on the given core on special behavior. So it's not 100% but 40%
        if (pthread_setaffinity_np(p_Handle, sizeof(cpu_set_t), &l_CPUSet))
        {
            LOG_ERROR("ThrThread", R"LOG(Failed to set thread "%0" CPU affinity to %1)LOG", GetThreadName(p_Handle), p_CPU);
            return;
        }
#endif
    }

}   ///< namespace Threading
}   ///< namespace Core
}   ///< namespace SteerStone
