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

#include <Precompiled.hpp>

#include "DiaStopWatch.hpp"
#include "DiaServerWatch.hpp"

namespace SteerStone { namespace Core { namespace Diagnostic {

    SINGLETON_P_I(ServerTime);

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    ServerTime::ServerTime()
    {
    }
    /// Deconstructor
    ServerTime::~ServerTime()
    {
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Get Start time when engine booted up
    uint32 ServerTime::GetServerTime()
    {
        static const std::chrono::system_clock::time_point l_StartTime = std::chrono::system_clock::now();
        return static_cast<uint32>((std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()) - std::chrono::duration_cast<std::chrono::milliseconds>(l_StartTime.time_since_epoch())).count());
    }
    /// Time Difference
    /// @p_OldMSTime : Old Time
    /// @p_NewMSTime : New Time    
    uint32 ServerTime::GetTimeDifference(uint32 const& p_OldMSTime, uint32 const& p_NewMSTime)
    {
        if (p_OldMSTime > p_NewMSTime)
        {
            const uint32 l_Diff1 = (uint32(0xFFFFFFFF) - p_OldMSTime) + p_NewMSTime;
            const uint32 l_Diff2 = p_OldMSTime - p_NewMSTime;

            return std::min(l_Diff1, l_Diff2);
        }

        return p_NewMSTime - p_OldMSTime;
    }
    /// Tick Time
    uint32 ServerTime::TickTime()
    {
        return m_Time;
    }
    /// Tick Previous Time
    uint32 ServerTime::TickPrevTime()
    {
        return m_PrevTime;
    }
    /// Tick
    uint32 ServerTime::Tick()
    {
        /// Save previous world tick time
        m_PrevTime = m_Time;

        /// Get the new one and don't forget to persist current system time in m_SystemTickTime
        m_Time = GetServerTime();

        /// Return tick diff
        return GetTimeDifference(m_PrevTime, m_Time);
    }
}   ///< namespace Diagnostic
}   ///< namespace Core
}   ///< namespace SteerStone
