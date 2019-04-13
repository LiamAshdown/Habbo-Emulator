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
        namespace Login
        {
            /// SERVER_LOGIN_OK packet builder
            class LoginOK final : public ServerPacket
            {
            public:
                /// Constructor 
                LoginOK() : ServerPacket(SERVER_LOGIN_OK) {}

            public:
                /// Write the packet
                StringBuffer const* Write();
            };

            /// SERVER_AVAILABLE_SETS packet builder
            class AvailableSets final : public ServerPacket
            {
            public:
                /// Constructor 
                AvailableSets() : ServerPacket(SERVER_AVAILABLE_SETS) {}

            public:
                /// Write the packet
                StringBuffer const* Write();

            public:
                bool UseClubClothing;                       ///< Can use Habbo Clothing or not
            };

            /// SERVER_ERROR packet builder
            class LocalisedError final : public ServerPacket
            {
            public:
                /// Constructor 
                LocalisedError() : ServerPacket(SERVER_ERROR) {}

            public:
                /// Write the packet
                StringBuffer const* Write();

                std::string Error;
            };

            /// SERVER_CRYPTO_PARAMETERS packet builder
            class InitializeCrypto final : public ServerPacket
            {
            public:
                /// Constructor 
                InitializeCrypto() : ServerPacket(SERVER_CRYPTO_PARAMETERS) {}

            public:
                /// Write the packet
                StringBuffer const* Write();
            };

            /// SERVER_CRYPTO_PARAMETERS packet builder
            class CryptoParameters final : public ServerPacket
            {
            public:
                /// Constructor 
                CryptoParameters() : ServerPacket(SERVER_SESSION_PARAMETERS) {}

                std::unordered_map<uint16, std::string> Parameters;

            public:
                /// Write the packet
                StringBuffer const* Write();
            };

            /// SERVER_GDATE packet builder
            class GDate final : public ServerPacket
            {
            public:
                /// Constructor 
                GDate() : ServerPacket(SERVER_GDATE) {}

            public:
                /// Write the packet
                StringBuffer const* Write();

                std::string Date;                       ///< Current Date
            };

            /// SERVER_LOGIN_OK packet builder
            class Login final : public ServerPacket
            {
            public:
                /// Constructor 
                Login() : ServerPacket(SERVER_LOGIN_OK) {}

            public:
                /// Write the packet
                StringBuffer const* Write();
            };

            /// SERVER_USER_OBJECT packet builder
            class HabboObject final : public ServerPacket
            {
            public:
                /// Constructor 
                HabboObject() : ServerPacket(SERVER_USER_OBJECT) {}

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

            /// SERVER_SOUND_SETTINGS packet builder
            class AccountPreferences final : public ServerPacket
            {
            public:
                /// Constructor 
                AccountPreferences() : ServerPacket(SERVER_SOUND_SETTINGS) {}

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

                std::map<std::string, bool> Badges;
                std::string CurrentBadge;
                bool BadgeVisible;
            };


        } ///< NAMESPACE LOGIN
    } ///< NAMESPACE HABBOPACKET
} ///< NAMESPACE STEERSTONE