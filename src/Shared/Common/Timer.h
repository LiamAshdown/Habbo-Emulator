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

#ifndef _Quad_Timer_h_
#define _Quad_Timer_h_
#include "SharedDefines.h"
#undef min
#include <algorithm>
#include <chrono>

namespace Quad
{
    namespace Time
    {
        class WorldTimer
        {
        public:
            static WorldTimer* instance();

        public:
            WorldTimer();
            ~WorldTimer();

        public:
            uint32 GetServerTime();
            inline uint32 GetTimeDifference(const uint32& oldMSTime, const uint32& newMSTime);

            uint32 TickTime();
            uint32 TickPrevTime();
            uint32 Tick();

        private:
            uint32 mTime;
            uint32 mPrevTime;
        };
    }
}
#define sWorldTimer Quad::Time::WorldTimer::instance() 

#endif /* _Quad_Timer_h_ */