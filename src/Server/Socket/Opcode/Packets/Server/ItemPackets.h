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
        namespace Item
        {
            /// SERVER_SHOW_PROGRAM packet builder
            class ShowProgram final : public ServerPacket
            {
            public:
                /// Constructor 
                ShowProgram() : ServerPacket(SERVER_SHOW_PROGRAM) {}

            public:
                /// Write the packet
                StringBuffer const* Write();

                std::string Program;
                std::string State;
            };

            /// SERVER_STUFF_DATA_UPDATE packet builder
            class StuffDataUpdate final : public ServerPacket
            {
            public:
                /// Constructor 
                StuffDataUpdate() : ServerPacket(SERVER_STUFF_DATA_UPDATE) {}

            public:
                /// Write the packet
                StringBuffer const* Write();

                std::string Program;
                std::string State;
            };
        } ///< NAMESPACE ITEM
    } ///< NAMESPACE HABBOPACKET
} ///< NAMESPACE STEERSTONE