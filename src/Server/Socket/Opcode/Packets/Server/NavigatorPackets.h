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
    enum ConnectionError
    {
        ROOM_IS_FULL                = 1,
        ROOM_IS_CLOSED              = 2,
        ROOM_IS_IN_QUEUE            = 3
    };

    struct FlatResultData
    {
        std::string RoomId;
        std::string RoomName;
        std::string OwnerName;
        std::string AccessType;
        std::string VisitorsNow;
        std::string VisitorsMax;
        std::string Description;
        
    };

    namespace HabboPacket
    {
        namespace Navigator
        {
            /// SERVER_CANT_CONNECT packet builder
            class CantConnect final : public ServerPacket
            {
            public:
                /// Constructor 
                CantConnect() : ServerPacket(SERVER_CANT_CONNECT) {}

            public:
                /// Write the packet
                StringBuffer const* Write();

                ConnectionError ErrorCode;
            };

            /// SERVER_FAVOURITE_ROOMS_RESULT packet builder
            class FavouriteRoomResult final : public ServerPacket
            {
            public:
                /// Constructor 
                FavouriteRoomResult() : ServerPacket(SERVER_FAVOURITE_ROOMS_RESULT) {}

            public:
                /// Write the packet
                StringBuffer const* Write();
            };
            
            /// SERVER_NO_FLATS_FOR_USER packet builder
            class NoFlatsForUser final : public ServerPacket
            {
            public:
                /// Constructor 
                NoFlatsForUser() : ServerPacket(SERVER_NO_FLATS_FOR_USER) {}

            public:
                /// Write the packet
                StringBuffer const* Write();

                std::string Name;
            };
            
            /// SERVER_FLAT_RESULTS packet builder
            class FlatResults final : public ServerPacket
            {
            public:
                /// Constructor 
                FlatResults() : ServerPacket(SERVER_FLAT_RESULTS) {}

            public:
                /// Write the packet
                StringBuffer const* Write();

                std::vector<FlatResultData> Flats;
            };

            /// SERVER_NO_FLATS packet builder
            class NoFlats final : public ServerPacket
            {
            public:
                /// Constructor 
                NoFlats() : ServerPacket(SERVER_NO_FLATS) {}

            public:
                /// Write the packet
                StringBuffer const* Write();
            };

            /// SERVER_FLAT_RESULTS_1 packet builder
            class FlatResultsSearch final : public ServerPacket
            {
            public:
                /// Constructor 
                FlatResultsSearch() : ServerPacket(SERVER_FLAT_RESULTS_1) {}

            public:
                /// Write the packet
                StringBuffer const* Write();

                std::vector<FlatResultData> Flats;
            };

            /// SERVER_FLAT_INFO packet builder
            class FlatInfo final : public ServerPacket
            {
            public:
                /// Constructor 
                FlatInfo() : ServerPacket(SERVER_FLAT_INFO) {}

            public:
                /// Write the packet
                StringBuffer const* Write();

                bool AllowSuperUsers;
                uint16 AccessType;
                uint32 RoomId;
                std::string OwnerName;
                bool ShowOwnerName;
                std::string Model;
                std::string Name;
                std::string Description;
                bool AllowTrading;
                bool HasCategory;
                uint32 NowVisitors;
                uint32 MaxVisitors;
            };

            /// SERVER_USER_FLAT_CATEGORY packet builder
            class FlatCategory final : public ServerPacket
            {
            public:
                /// Constructor 
                FlatCategory() : ServerPacket(SERVER_USER_FLAT_CATEGORY) {}

            public:
                /// Write the packet
                StringBuffer const* Write();

                uint32 RoomId;
                uint32 CategoryId;
            };

            /// SERVER_SPACE_NODE_USERS packet builder
            class NodeSpaceUsers final : public ServerPacket
            {
            public:
                /// Constructor 
                NodeSpaceUsers() : ServerPacket(SERVER_SPACE_NODE_USERS) {}

            public:
                /// Write the packet
                StringBuffer const* Write();

                std::vector<std::string> Names;
            };

        } ///< NAMESPACE NAVIGATOR
    } ///< NAMESPACE HABBOPACKET
} ///< NAMESPACE STEERSTONE