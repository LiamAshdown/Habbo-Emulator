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
    enum ApproveNameError
    {
        NAME_VALID                      = 0,
        NAME_TOO_LONG                   = 1,
        NAME_UNACCEPTABLE_TO_STAFF      = 2,
        NAME_UNACCEPTABLE_TO_STAFF_2    = 3,
        NAME_TAKEN                      = 4
    };

    enum ApprovePasswordError
    {
        PASSWORD_VALID                  = 0,
        PASSWORD_TOO_SHORT              = 1,
        PASSWORD_TOO_LONG               = 2,
        PASSWORD_INVALID_CHARS          = 3,
        PASSWORD_REQUIRES_NUMBERS       = 4,
        PASSWORD_USER_NAME_SIMILIAR     = 5
    };

    namespace HabboPacket
    {
        namespace Authentication
        {
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

            /// SERVER_APPROVE_NAME_REPLY packet builder
            class ApproveNameReply final : public ServerPacket
            {
            public:
                /// Constructor 
                ApproveNameReply() : ServerPacket(SERVER_APPROVE_NAME_REPLY) {}

            public:
                /// Write the packet
                StringBuffer const* Write();

                std::string Name;                           ///< Name of Habbo
                ApproveNameError ErrorCode;                 ///< Error Code if Name is incorrect
            };

            /// SERVER_APPROVE_PASSWORD_REPLY packet builder
            class ApprovePasswordReply final : public ServerPacket
            {
            public:
                /// Constructor 
                ApprovePasswordReply() : ServerPacket(SERVER_APPROVE_PASSWORD_REPLY) {}

            public:
                /// Write the packet
                StringBuffer const* Write();

                std::string Name;                           ///< Name of Habbo
                std::string Password;                       ///< Password of Habbo
                ApprovePasswordError ErrorCode;             ///< Error Code if Password is incorrect
            };

            /// SERVER_APPROVE_EMAIL_REPLY packet builder
            class ApproveEmailReply final : public ServerPacket
            {
            public:
                /// Constructor 
                ApproveEmailReply() : ServerPacket(SERVER_APPROVE_EMAIL_REPLY) {}

            public:
                /// Write the packet
                StringBuffer const* Write();
            };

            /// SERVER_LOCALISED_ERROR packet builder
            class LocalisedError final : public ServerPacket
            {
            public:
                /// Constructor 
                LocalisedError() : ServerPacket(SERVER_LOCALISED_ERROR) {}

            public:
                /// Write the packet
                StringBuffer const* Write();
                    
                std::string Error;                  ///< Error Message which will pop up on user client
            };

            /// SERVER_LOGIN packet builder
            class Login final : public ServerPacket
            {
            public:
                /// Constructor 
                Login() : ServerPacket(SERVER_LOGIN) {}

            public:
                /// Write the packet
                StringBuffer const* Write();
            };


        } ///< NAMESPACE AUTHENTICATION
    } ///< NAMESPACE HABBOPACKET
} ///< NAMESPACE STEERSTONE