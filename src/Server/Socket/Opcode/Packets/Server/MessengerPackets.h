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
    enum MessengerErrorCode
    {
        ACCEPT_SUCCESS              = 0,
        TARGET_FRIEND_LIST_FULL     = 2,
        TARGET_DOES_NOT_ACCEPT      = 3,
        FRIEND_REQUEST_NOT_FOUND    = 4,
        BUDDY_REMOVE_ERROR          = 37,
        FRIEND_LIST_FULL            = 39,
        CONCURRENCY_ERROR           = 42
    };

    namespace HabboPacket
    {
        namespace Messenger
        {
            /// SERVER_MESSENGER_INIT packet builder
            class MessengerInitialize final : public ServerPacket
            {
            public:
                /// Constructor 
                MessengerInitialize() : ServerPacket(SERVER_MESSENGER_INIT) {}

            public:
                /// Write the packet
                StringBuffer const* Write();

                std::string ConsoleMotto;                               ///< Habbo Console Motto
                uint32 FriendsLimit;                                    ///< Max Normal Friends Limit
                uint32 ClubFriendsLimit;                                ///< Max Club Friends Limit
            };

            /// SERVER_MESSENGER_UPDATE packet builder
            class MessengerUpdate final : public ServerPacket
            {
            public:
                /// Constructor 
                MessengerUpdate() : ServerPacket(SERVER_MESSENGER_UPDATE) {}

            public:
                /// Write the packet
                StringBuffer const* Write();
            };

            /// SERVER_MESSENGER_SEND_FRIEND_REQUEST packet builder
            class MessengerSendFriendRequest final : public ServerPacket
            {
            public:
                /// Constructor 
                MessengerSendFriendRequest() : ServerPacket(SERVER_MESSENGER_SEND_FRIEND_REQUEST) {}

            public:
                /// Write the packet
                StringBuffer const* Write();

                uint32 Id;
                std::string Name;
            };

            /// SERVER_MESSENGER_FIND_USER_RESULT packet builder
            class MessengerFindUserResult final : public ServerPacket
            {
            public:
                /// Constructor 
                MessengerFindUserResult() : ServerPacket(SERVER_MESSENGER_FIND_USER_RESULT) {}

            public:
                /// Write the packet
                StringBuffer const* Write();

                std::string Messenger;
            };

            /// SERVER_MESSENGER_ERROR packet builder
            class MessengerError final : public ServerPacket
            {
            public:
                /// Constructor 
                MessengerError() : ServerPacket(SERVER_MESSENGER_ERROR) {}

            public:
                /// Write the packet
                StringBuffer const* Write();

                uint32 MessageId;
                MessengerErrorCode Error;
            };
        } ///< NAMESPACE Messenger
    } ///< NAMESPACE HABBOPACKET
} ///< NAMESPACE STEERSTONE