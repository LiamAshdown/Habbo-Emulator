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

namespace SteerStone { namespace Core { namespace Diagnostic {

    /// Constructor
    StopWatch::StopWatch()
        : m_Start(std::chrono::steady_clock::now())
    {
        m_Elapsed = 0;
        m_Running = true;
        Reset();
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Reset
    void StopWatch::Reset()
    {
        m_Start = std::chrono::steady_clock::now();
    }
    /// Start
    void StopWatch::Start()
    {
        m_Running   = true;
        m_Start     = std::chrono::steady_clock::now();
    }
    /// Stop
    void StopWatch::Stop()
    {
        TimeStamp l_Stop = std::chrono::steady_clock::now();

        m_Running = false;
        m_Elapsed = static_cast<int64>(std::chrono::duration_cast<std::chrono::milliseconds>(l_Stop - m_Start).count());
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Get elapsed time between Start & Stop
    int64 StopWatch::GetElapsed()
    {
        if (m_Running)
        {
            auto l_CurrentClock = std::chrono::steady_clock::now();
            return static_cast<int64>(std::chrono::duration_cast<std::chrono::milliseconds>(l_CurrentClock - m_Start).count());
        }

        return m_Elapsed;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Is running ?
    bool StopWatch::IsRunning() const
    {
        return m_Running;
    }

}   ///< namespace Diagnostic
}   ///< namespace Core
}   ///< namespace SteerStone
