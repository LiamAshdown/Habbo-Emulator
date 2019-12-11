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

#include <thread>
#include <chrono>

namespace SteerStone { namespace Core { namespace Threading {

    /// This thread helper
    class ThisThread
    {
        DISALLOW_COPY_AND_ASSIGN(ThisThread);
        DEFAULT_CONSTRUCTOR(ThisThread);

        //////////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////////

        public:
            /// Get this thread native handle
            static std::thread::native_handle_type GetNativeHandle();
            /// Get thread name
            static std::string GetThreadName();
            /// Get current thread's CPU index
            static uint32_t GetCurrentThreadCPU();

            /// Set thread name
            /// @p_Name : Thread name
            static void SetThreadName(const std::string & p_Name);
            /// Set thread CPU affinity
            /// @p_CPU : CPU ID from 0
            static void SetThreadCPUAffinity(uint32 p_CPU);

            /// For this thread for specific amount of time
            /// @p_Milliseconds : Total time
            static void SleepFor(uint64 p_Milliseconds);
            /// For this thread for specific amount of time
            /// @p_Duration : Duration
            template<class T, class P> static void SleepFor(const std::chrono::duration<T, P> & p_Duration)
            {
                SleepFor(std::chrono::duration_cast<std::chrono::milliseconds>(p_Duration).count());
            }
            /// Yield thread
            static void YieldThread();
    };

}   ///< namespace Threading
}   ///< namespace Core
}   ///< namespace SteerStone
