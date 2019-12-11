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

#include "Core/Core.hpp"

#include <chrono>

namespace SteerStone { namespace Core { namespace Diagnostic {

    /// Time measuring tools
    class StopWatch
    {
        using TimeStamp = std::chrono::time_point<std::chrono::high_resolution_clock>;

        public:
            /// Constructor
            StopWatch();

            /// Reset
            void Reset();
            /// Start
            void Start();
            /// Stop
            void Stop();

            /// Is running
            bool IsRunning() const;

            /// Get elapsed time between Start & Stop
            int64 GetElapsed();

        protected:
            TimeStamp   m_Start;    ///< Reference time
            bool        m_Running;  ///< Is running
            int64       m_Elapsed;  ///< Elapsed time

    };

}   ///< namespace Diagnostic
}   ///< namespace Core
}   ///< namespace SteerStone
