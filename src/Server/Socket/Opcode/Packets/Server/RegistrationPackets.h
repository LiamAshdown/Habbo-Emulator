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
        namespace Registration
        {
            /// SERVER_APPROVE_NAME packet builder
            class ApproveNameReply final : public ServerPacket
            {
            public:
                /// Constructor 
                ApproveNameReply() : ServerPacket(SERVER_APPROVE_NAME) {}

            public:
                /// Write the packet
                StringBuffer const* Write();

                std::string Name;
                ApproveNameError ErrorCode;
            };

            /// SERVER_APPROVE_PASSWORD_REPLY packet builder
            class ApprovePasswordReply final : public ServerPacket
            {
            public:
                /// Constructor 
                ApprovePasswordReply() : ServerPacket(SERVER_PASSWORD_APPROVED) {}

            public:
                /// Write the packet
                StringBuffer const* Write();

                std::string Name;
                std::string Password;
                ApprovePasswordError ErrorCode;
            };

            /// SERVER_APPROVE_EMAIL_REPLY packet builder
            class ApproveEmailReply final : public ServerPacket
            {
            public:
                /// Constructor 
                ApproveEmailReply() : ServerPacket(SERVER_EMAIL_APPROVED) {}

            public:
                /// Write the packet
                StringBuffer const* Write();
            };


        } ///< NAMESPACE REGISTRATION
    } ///< NAMESPACE HABBOPACKET
} ///< NAMESPACE STEERSTONE