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

#ifndef _SERVER_HABBOINFO_PACKETS_h
#define _SERVER_HABBOINFO_PACKETS_h
#include "Common/SharedDefines.h"
#include "Network/ServerPacket.h"
#include "Opcodes.h"
#endif /* _SERVER_HABBOINFO_PACKETS_h */

namespace SteerStone
{
    namespace HabboPacket
    {
        namespace HabboInfo
        {
            /// SERVER_FUSE_RIGHTS packet builder
            class FuseRights final : public ServerPacket
            {
            public:
                /// Constructor 
                FuseRights() : ServerPacket(SERVER_FUSE_RIGHTS) {}

            public:
                /// Write the packet
                StringBuffer const* Write();

                std::vector<std::string> HabboFuseRights;                   ///< Fuse rights
            };

            /// SERVER_HABBO_OBJECT packet builder
            class HabboObject final : public ServerPacket
            {
            public:
                /// Constructor 
                HabboObject() : ServerPacket(SERVER_HABBO_OBJECT) {}

            public:
                /// Write the packet
                StringBuffer const* Write();
                std::string Id;
                std::string Name;
                std::string Figure;
                std::string Gender;
                std::string Motto;
                uint32 Tickets;
                std::string PoolFigure;
                uint32 Films;
            };

            /// SERVER_ACCOUNT_PREFERENCES packet builder
            class AccountPreferences final : public ServerPacket
            {
            public:
                /// Constructor 
                AccountPreferences() : ServerPacket(SERVER_ACCOUNT_PREFERENCES) {}

            public:
                /// Write the packet
                StringBuffer const* Write();

                bool SoundEnabled;
                bool TutorialFinished;
            };


            /// SERVER_AVAILABLE_BADGES packet builder
            class AvailableBadges final : public ServerPacket
            {
            public:
                /// Constructor 
                AvailableBadges() : ServerPacket(SERVER_AVAILABLE_BADGES) {}

            public:
                /// Write the packet
                StringBuffer const* Write();

                std::vector<std::string> Badges;
                uint8 ActiveBadges;

            };

        } ///< NAMESPACE HABBOINFO
    } ///< NAMESPACE HABBOPACKET
} ///< NAMESPACE STEERSTONE