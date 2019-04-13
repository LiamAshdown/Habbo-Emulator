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

#ifndef _HABBO_BADGE_h
#define _HABBO_BADGE_h
#include "Common/SharedDefines.h"
#endif /* _HABBO_BADGE_h */

namespace SteerStone
{
    class Habbo;

    /// Holds badges for user
    class Badge
    {
    public:
        /// Constructor
        /// @p_Habbo : Badges belonging to habbo
        Badge(Habbo* p_Habbo);

        /// Deconstructor
        ~Badge();

    public:
        /// LoadBadges
        /// Load user badges
        void LoadBadges();

        /// SendBadges
        /// Send user badges to client
        void SendBadges();

        /// IsBadgeVisible
        /// Is badge visible to other users
        bool IsCurrentBadgeVisible() const;

        /// GetCurrentBadge
        /// Returns current badge user is wearing
        std::string GetCurrentBadgeName() const;

    public:
        std::map<std::string, bool> m_Badges;              ///< Holds user badges
        std::tuple<std::string, bool> m_CurrentBadge;      ///< Current badge user is wearing
        Habbo* m_Habbo;                                    ///< Habbo class which holds info about badges

    };
} ///< NAMESPACE STEERSTONE