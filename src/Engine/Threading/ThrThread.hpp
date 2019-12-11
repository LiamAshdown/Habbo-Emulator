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
#include <mutex>
#include <map>

namespace SteerStone { namespace Core { namespace Threading {

    /// Thread helper
    class Thread
    {
        DISALLOW_COPY_AND_ASSIGN(Thread);
        DEFAULT_CONSTRUCTOR(Thread);

        public:
            /// Get thread name
            /// @p_Handle : Thread handle
            static std::string GetThreadName(std::thread::native_handle_type p_Handle);

            /// Set thread name
            /// @p_Handle   : Thread handle
            /// @p_Name     : Thread name
            static void SetThreadName(std::thread::native_handle_type p_Handle, const std::string & p_Name);
            /// Set thread CPU affinity
            /// @p_Handle   : Thread CPU
            /// @p_CPU      : CPU ID from 0
            static void SetThreadCPUAffinity(std::thread::native_handle_type p_Handle, uint32 p_CPU);

        private:
            static std::mutex                       m_Mutex;          ///< Global mutex
            static std::map<uint64, std::string>    m_ThreadsName;    ///< Name of the threads

    };

}   ///< namespace Threading
}   ///< namespace Core
}   ///< namespace SteerStone
