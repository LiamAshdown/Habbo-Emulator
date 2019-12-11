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
#include <Windows.h>

#include "Core/Core.hpp"

namespace SteerStone { namespace Core { namespace Utils {

    /// Returns Thread Id
    inline const uint64 GetThreadId()
    {
    #ifdef _WIN32
        return GetCurrentThreadId();
    #elif  defined(__linux__)
        long tid;
        syscall(SYS_thr_self, &tid);
        return static_cast<unsigned int>(tid);
    #endif
    }
}
}
}