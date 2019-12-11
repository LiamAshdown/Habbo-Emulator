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
#include <chrono>

#include "Core/Core.hpp"
#include "Singleton/Singleton.hpp"

namespace SteerStone { namespace Core { namespace Diagnostic {

    /// Timer
    class ServerTime
    {
        SINGLETON_P_D(ServerTime);

        //////////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////////

        public:
            /// Get Start time when engine booted up
            uint32 GetServerTime();
            /// Time Difference
            /// @p_OldMSTime : Old Time
            /// @p_NewMSTime : New Time
            uint32 GetTimeDifference(uint32 const& p_OldMSTime, uint32 const& p_NewMSTime);
            /// Tick Time
            uint32 TickTime();
            /// Tick Previous Time
            uint32 TickPrevTime();
            /// Tick
            uint32 Tick();

        //////////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////////

        private:
            uint32 m_Time;                               ///< Current Time
            uint32 m_PrevTime;                           ///< Previous Time
    };

}   ///< namespace Diagnostic
}   ///< namespace Core
}   ///< namespace SteerStone

#define sServerTimeManager SteerStone::Core::Diagnostic::ServerTime::GetSingleton()
