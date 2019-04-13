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
#include "Common/SharedDefines.h"
#include "HabboSocket.h"

namespace SteerStone
{
    namespace HabboPacket
    {
        namespace Club
        {
            /// SERVER_CLUB_INFO packet builder
            class ClubInfo final : public ServerPacket
            {
            public:
                /// Constructor 
                ClubInfo() : ServerPacket(SERVER_CLUB_INFO) {}

            public:
                /// Write the packet
                StringBuffer const* Write();

                std::string ProductName;
                int64 DaysLeft;
                int64 ElaspedMonths;
                int64 PrepaidMonths;
                int64 ResponseFlag;
            };

        } ///< NAMESPACE CLUB
    } ///< NAMESPACE HABBOPACKET
} ///< NAMESPACE STEERSTONE