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

#ifndef _HABBO_HABBO_CLUB_h
#define _HABBO_HABBO_CLUB_h
#include "Common/SharedDefines.h"
#endif /* _HABBO_HABBO_CLUB_h */

namespace SteerStone
{
    class Habbo;

    /// Holds info about user habbo club
    class HabboClub
    {
    public:
        /// Constructor
        /// @p_Habbo : Habbo class being added
        HabboClub(Habbo* p_Habbo);

        /// Deconstructor
        ~HabboClub();

    public:
        /// AddSubscription
        /// Add Club Subscription to Habbo
        /// @p_Days : Amount of days to subscribed for
        /// @p_Price : Price of club subscription
        void AddSubscription(uint32 p_Days, uint32 p_Price);

        /// LoadSubscription
        /// Load Habbo Subscription
        void LoadSubscription();

        /// IsSubscribed
        /// Returns if user is subscribed to habbo club
        bool IsSubscribed() const;

    private:
        int32 m_DaysMonthRemaining;             ///< Number of days this month is remaining
        int32 m_RemainingDays;                  ///< Days remaining till habbo club expires
        int32 m_PrePaidMonths;                  ///< Amount of prepaid months user has left
        int32 m_DaysPassed;                     ///< Days passed since purchase of habbo club
        int32 m_MonthsPassed;                   ///< Months passed since purchase of habbo club
        bool m_Subscribed;                      ///< Whether user is subscribed to habbo club

        Habbo* m_Habbo;                         ///< Habbo class which holds info about habbo club
    };
    
} ///< NAMESPACE STEERSTONE