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
#include <ctime>
#include <chrono>

class Timer
{
public:
    Timer()
    {
        Reset();
    }

    double Elasped()
    {
        auto now = std::chrono::high_resolution_clock::now();
        return std::chrono::duration<double, std::milli>(now - mStartTime).count();
    }

    void Reset()
    {
        mStartTime = std::chrono::high_resolution_clock::now();
    }

private:
    std::chrono::time_point<std::chrono::steady_clock> mStartTime;
};

#endif /* _Quad_Timer_h_ */