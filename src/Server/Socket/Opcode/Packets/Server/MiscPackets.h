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

#ifndef _SERVER_MISC_PACKETS_h
#define _SERVER_MISC_PACKETS_h
#include "Common/SharedDefines.h"
#include "Network/ServerPacket.h"
#include "Opcodes.h"
#endif /* _SERVER_MISC_PACKETS_h */

namespace SteerStone
{
    namespace HabboPacket
    {
        namespace Misc
        {
            /// SERVER_ALERT packet builder
            class ServerAlert final : public ServerPacket
            {
            public:
                /// Constructor 
                ServerAlert() : ServerPacket(SERVER_ALERT) {}

            public:
                /// Write the packet
                StringBuffer const* Write();

                std::string Message;                    ///< Message to be sent to client
            };

            /// SERVER_HOTEL_LOGOUT packet builder
            class HotelLogout final : public ServerPacket
            {
            public:
                /// Constructor 
                HotelLogout() : ServerPacket(SERVER_HOTEL_LOGOUT) {}

            public:
                /// Write the packet
                StringBuffer const* Write();

                uint8 Reason;                          ///< Logout Reason
            };

            /// SERVER_PING packet builder
            class Ping final : public ServerPacket
            {
            public:
                /// Constructor 
                Ping() : ServerPacket(SERVER_PING) {}

            public:
                /// Write the packet
                StringBuffer const* Write();
            };

        } ///< NAMESPACE MISC
    } ///< NAMESPACE HABBOPACKET
} ///< NAMESPACE STEERSTONE