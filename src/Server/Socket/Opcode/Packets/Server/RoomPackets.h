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

#ifndef _SERVER_ROOM_PACKETS_h
#define _SERVER_ROOM_PACKETS_h
#include "Common/SharedDefines.h"
#include "Network/ServerPacket.h"
#include "Opcodes.h"
#endif /* _SERVER_ROOM_PACKETS_h */

namespace SteerStone
{
    namespace HabboPacket
    {
        namespace Room
        {
            /// SERVER_ROOM_USER_OBJECTS packet builder
            class HabboRoomObject final : public ServerPacket
            {
            public:
                /// Constructor 
                HabboRoomObject() : ServerPacket(SERVER_ROOM_USER_OBJECTS) {}

            public:
                /// Write the packet
                StringBuffer const* Write();

                std::string UniqueId;
                std::string Id;
                std::string Name;
                std::string Figure;
                std::string Gender;
                std::string X;
                std::string Y;
                std::string Z;
                boost::optional<std::string> Motto;
            };


        } ///< NAMESPACE ROOM
    } ///< NAMESPACE HABBOPACKET
} ///< NAMESPACE STEERSTONE