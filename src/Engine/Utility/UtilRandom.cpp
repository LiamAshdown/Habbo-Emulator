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

#include "UtilRandom.hpp"
#include "SFMT.h"
#include <boost/thread/tss.hpp>
#include <random>
#include <assert.h>

namespace SteerStone { namespace Core { namespace Utils {
    
    SINGLETON_P_I(SFMTEngine)

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    SFMTEngine::SFMTEngine()
    {

    }
    /// Deconstructor
    SFMTEngine::~SFMTEngine()
    {
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    static SFMTRand* GetRng()
    {
        static boost::thread_specific_ptr<SFMTRand> l_SfmtRand;
        SFMTRand* l_Random = l_SfmtRand.get();

        if (!l_Random)
        {
            l_Random = new SFMTRand();
            l_SfmtRand.reset(l_Random);
        }

        return l_Random;
    }

    /// Return a random number in the range min..max.
    int32 Int32Random(int32 min, int32 max)
    {
        assert(max >= min);
        return int32(GetRng()->IRandom(min, max));
    }
    /// Return a random number in the range min..max (inclusive)
    uint32 UInt32Random(uint32 p_Min, uint32 p_Max)
    {
        if (p_Max <= p_Min)
            return p_Min;

        return GetRng()->URandom(p_Min, p_Max);
    }
    /// Return a random number in the range 0 .. UINT32_MAX.
    uint32 Rand32()
    {
        return GetRng()->BRandom();
    }
    /// Return a random number in the range min..max
    float FloatRandom(float p_Min, float p_Max)
    {
        assert(p_Max >= p_Min);
        return float(GetRng()->Random() * (p_Max - p_Min) + p_Min);
    }
    /// Return a random double from 0.0 to 1.0 (exclusive)
    double DoubleRandom()
    {
        return GetRng()->Random();
    }
    /// Return a random double from 0.0 to 100.0 (exclusive)
    double RandomChance()
    {
        return GetRng()->Random() * 100.0;
    }
    /// Return true if a random roll fits in the specified chance (range 0-100)
    bool RoleChanceFloat(float p_Chance)
    {
        return p_Chance > RandomChance();
    }
    /// Return true if a random roll fits in the specified chance (range 0-100)
    bool RollChanceInterger32(int32 p_Chance)
    {
        return p_Chance > Int32Random(0, 99);
    }

}   ///< namespace Utils
}   ///< namespace Core
}   ///< namespace SteerStone