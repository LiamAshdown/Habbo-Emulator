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

#ifndef COMMON_TIMER_h
#define COMMON_TIMER_h
#include "SharedDefines.h"
#undef min
#include <algorithm>
#include <chrono>
#endif /* COMMON_TIMER_h */

namespace SteerStone
{
    namespace Time
    {
        /// Class which holds the update timer in our main thread loop, which updates the world
        /// Singleton class
        class WorldTimer
        {
        public:
            static WorldTimer* instance();

        public:
            /// Constructor
            WorldTimer() {}
            /// Deconstructor
            ~WorldTimer() {}

        public:
            /// GetServerTime - Get the time of the server
            uint32 GetServerTime();

            /// GetTimeDifference
            /// @p_OldMSTime : Old Time
            /// @p_NewMSTime : New Time
            uint32 GetTimeDifference(uint32 const& p_OldMSTime, uint32 const& p_NewMSTime);

            /// TickTime - Get our Tick Time
            uint32 TickTime();

            /// TickPrevTIme - Get our previous Tick Time
            uint32 TickPrevTime();

            /// Tick - Get our Tick
            uint32 Tick();

        private:
            uint32 m_Time;                               ///< Current Time
            uint32 m_PrevTime;                           ///< Previous Time
        };
    } ///< NAMESPACE TIME
} ///< NAMESPACE STEERSTONE
#define sHotelTimer SteerStone::Time::WorldTimer::instance() 