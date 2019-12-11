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

#include "Threading/ThrThisThread.hpp"
#include "Threading/ThrThread.hpp"

#if defined(_WIN32)
#   include <windows.h>
#elif defined(__APPLE__)
#   include <cpuid.h>
#elif defined(linux)
#   include <utmpx.h>
#else
#   error "Unsuported platform in ThrThisThread.cpp"
#endif

#include <thread>
#include <chrono>

namespace SteerStone { namespace Core { namespace Threading {

    /// Get this thread native handle
    std::thread::native_handle_type ThisThread::GetNativeHandle()
    {
#if defined(_WIN32)
        return GetCurrentThread();
#else
        return pthread_self();
#endif
    }
    /// Get thread name
    std::string ThisThread::GetThreadName()
    {
        return Thread::GetThreadName(GetNativeHandle());
    }
    /// Get current thread's CPU index
    uint32_t ThisThread::GetCurrentThreadCPU()
    {
#if defined(_WIN32)
        return static_cast<uint32>(GetCurrentProcessorNumber());
#elif defined(__APPLE__)
        int32_t l_CPU;
        uint32_t l_CPUInfo[4];
        __cpuid_count(1, 0, l_CPUInfo[0], l_CPUInfo[1], l_CPUInfo[2], l_CPUInfo[3]);

        ///< CPUInfo[1] is EBX, bits 24-31 are APIC ID
        if ((l_CPUInfo[3] & (1 << 9)) == 0)
            l_CPU = -1;                       ///< no APIC on chip
        else
            l_CPU = (unsigned)l_CPUInfo[1] >> 24;

        if (l_CPU < 0)
            l_CPU = 0;

        return static_cast<uint32>(l_CPU);
#else
        return static_cast<uint32>(sched_getcpu());
#endif
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Set thread name
    /// @p_Name : Thread name
    void ThisThread::SetThreadName(const std::string & p_Name)
    {
        Thread::SetThreadName(GetNativeHandle(), p_Name);
    }
    /// Set thread CPU affinity
    /// @p_CPU : CPU ID from 0
    void ThisThread::SetThreadCPUAffinity(uint32 p_CPU)
    {
        Thread::SetThreadCPUAffinity(GetNativeHandle(), p_CPU);
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// For this thread for specific amount of time
    /// @p_Milliseconds : Total time
    void ThisThread::SleepFor(uint64 p_Milliseconds)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(p_Milliseconds));
    }
    /// Yield thread
    void ThisThread::YieldThread()
    {
        std::this_thread::yield();
    }

}   ///< namespace Threading
}   ///< namespace Core
}   ///< namespace SteerStone
