#pragma once
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
#include "Core/Core.hpp"
#include "Singleton/Singleton.hpp"

#include <limits>

namespace SteerStone { namespace Core { namespace Utils {

    /// Return a random number in the range min..max.
    int32 Int32Random(int32 min, int32 max);
    /// Return a random number in the range min..max (inclusive)
    uint32 UInt32Random(uint32 p_Min, uint32 p_Max);
    /// Return a random number in the range 0 .. UINT32_MAX.
    uint32 Rand32();
    /// Return a random number in the range min..max
    float FloatRandom(float p_Min, float p_Max);
    /// Return a random double from 0.0 to 1.0 (exclusive)
    double DoubleRandom();
    /// Return a random double from 0.0 to 100.0 (exclusive)
    double RandomChance();
    /// Return true if a random roll fits in the specified chance (range 0-100)
    bool RoleChanceFloat(float p_Chance);
    /// Return true if a random roll fits in the specified chance (range 0-100)
    bool RollChanceInterger32(int32 p_Chance);

    class SFMTEngine
    {
        SINGLETON_P_D(SFMTEngine);

        //////////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////////

    public:
        static constexpr uint32 Min() { return std::numeric_limits<uint32>::min(); }
        static constexpr uint32 Max() { return std::numeric_limits<uint32>::max(); }
        uint32 operator()() const { return Rand32(); }
    };

}   ///< namespace Utils
}   ///< namespace Core
}   ///< namespace SteerStone