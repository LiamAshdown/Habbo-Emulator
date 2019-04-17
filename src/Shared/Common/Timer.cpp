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

#include "Timer.h"

namespace SteerStone
{
    namespace Time
    {
        /// Singleton
        WorldTimer* WorldTimer::instance()
        {
            static WorldTimer instance;
            return &instance;
        }

        /// GetServerTime - Get the time of the server
        uint32 WorldTimer::GetServerTime()
        {
            static auto const start_time = std::chrono::system_clock::now();
            return static_cast<uint32>((std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()) - std::chrono::duration_cast<std::chrono::milliseconds>(start_time.time_since_epoch())).count());
        }

        /// GetTimeDifference
        /// @p_OldMSTime : Old Time
        /// @p_NewMSTime : New Time    
        uint32 WorldTimer::GetTimeDifference(uint32 const& p_OldMSTime, uint32 const& p_NewMSTime)
        {
            if (p_OldMSTime > p_NewMSTime)
            {
                const uint32 diff_1 = (uint32(0xFFFFFFFF) - p_OldMSTime) + p_NewMSTime;
                const uint32 diff_2 = p_OldMSTime - p_NewMSTime;

                return std::min(diff_1, diff_2);
            }

            return p_NewMSTime - p_OldMSTime;
        }

        /// TickTime - Get our Tick Time
        uint32 WorldTimer::TickTime()
        {
            return m_Time;
        }

        /// TickPrevTIme - Get our previous Tick Time
        uint32 WorldTimer::TickPrevTime()
        {
            return m_PrevTime;
        }

        /// Tick - Get our Tick
        uint32 WorldTimer::Tick()
        {
            /// Save previous world tick time
            m_PrevTime = m_Time;

            /// Get the new one and don't forget to persist current system time in m_SystemTickTime
            m_Time = WorldTimer::GetServerTime();

            /// Return tick diff
            return GetTimeDifference(m_PrevTime, m_Time);
        } 
    } ///< NAMESPACE TIME
} ///< NAMESPACE STEERSTONE

#define sWorldTimer SteerStone::Time::WorldTimer::instance;
