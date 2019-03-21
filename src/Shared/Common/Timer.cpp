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
//-----------------------------------------------//
#include "Timer.h"
//-----------------------------------------------//
namespace Quad
{
    //-----------------------------------------------//
    namespace Time
    {
        //-----------------------------------------------//
        WorldTimer* WorldTimer::instance()
        {
            static WorldTimer instance;
            return &instance;
        }
        //-----------------------------------------------//
        WorldTimer::WorldTimer()
        {
        }
        //-----------------------------------------------//
        WorldTimer::~WorldTimer()
        {
        }
        //-----------------------------------------------//
        uint32 WorldTimer::Tick()
        {
            // Save previous world tick time
            mPrevTime = mTime;

            // Get the new one and don't forget to persist current system time in m_SystemTickTime
            mTime = WorldTimer::GetServerTime();

            // Return tick diff
            return GetTimeDifference(mPrevTime, mTime);
        }
        //-----------------------------------------------//
        uint32 WorldTimer::GetServerTime()
        {
            static auto const start_time = std::chrono::system_clock::now();
            return static_cast<uint32>((std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()) - std::chrono::duration_cast<std::chrono::milliseconds>(start_time.time_since_epoch())).count());
        }
        //-----------------------------------------------//
        inline uint32 WorldTimer::GetTimeDifference(const uint32 & oldMSTime, const uint32 & newMSTime)
        {
            if (oldMSTime > newMSTime)
            {
                const uint32 diff_1 = (uint32(0xFFFFFFFF) - oldMSTime) + newMSTime;
                const uint32 diff_2 = oldMSTime - newMSTime;

                return std::min(diff_1, diff_2);
            }

            return newMSTime - oldMSTime;
        }
        //-----------------------------------------------//
        uint32 WorldTimer::TickTime() 
        { 
            return mTime; 
        }
        //-----------------------------------------------//
        uint32 WorldTimer::TickPrevTime() 
        {
            return mPrevTime; 
        }
        //-----------------------------------------------//
    }
    //-----------------------------------------------//
}
//-----------------------------------------------//