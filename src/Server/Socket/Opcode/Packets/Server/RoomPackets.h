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
    enum RoomConnectionError
    {
        ROOM_IS_FULL                = 1,
        ROOM_IS_CLOSED              = 2,
        ROOM_IS_IN_QUEUE            = 3
    };

    enum ActiveObjectType
    {
        OBJECT_TYPE_PUBLIC          = 0,
        OBJECT_TYPE_WALL            = 1,
        OBJECT_TYPE_POST_IT         = 2,
        OBJECT_TYPE_PRESENT         = 3,
        OBJECT_TYPE_ROLLER          = 4,
        OBJECT_TYPE_EXTRA_PARAMETER = 5,
    };

    struct WorldObject
    {
        std::string Id;
        std::string Sprite;
        std::string X;
        std::string Y;
        std::string Z;
        std::string Rotation;
        std::string Length;
    };

    struct ActiveObject
    {
        uint8 Type;
        std::string Id;
        std::string Sprite;
        std::string SpriteId;
        std::string Name;
        std::string Description;
        std::string Colour;
        uint32 Length;
        uint32 Width;
        uint32 TopHeight;
        uint32 MaxStatus;
        uint32 Behaviour;
        uint32 Interactor;
        bool Tradeable;
        bool Recyable;
        uint32 DrinkId;
        ActiveObjectType ObjectType;
    };

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

            /// SERVER_ROOM_INTEREST packet builder
            class RoomInterest final : public ServerPacket
            {
            public:
                /// Constructor 
                RoomInterest() : ServerPacket(SERVER_ROOM_INTEREST) {}

            public:
                /// Write the packet
                StringBuffer const* Write();
            };

            /// SERVER_OPEN_CONNECTION packet builder
            class OpenConnection final : public ServerPacket
            {
            public:
                /// Constructor 
                OpenConnection() : ServerPacket(SERVER_OPEN_CONNECTION) {}

            public:
                /// Write the packet
                StringBuffer const* Write();
            };

            /// SERVER_ROOM_URL packet builder
            class RoomUrl final : public ServerPacket
            {
            public:
                /// Constructor 
                RoomUrl() : ServerPacket(SERVER_ROOM_URL) {}

            public:
                /// Write the packet
                StringBuffer const* Write();
            };

            /// SERVER_ROOM_CANT_CONNECT packet builder
            class RoomCantConnect final : public ServerPacket
            {
            public:
                /// Constructor 
                RoomCantConnect() : ServerPacket(SERVER_ROOM_CANT_CONNECT) {}

            public:
                /// Write the packet
                StringBuffer const* Write();

                RoomConnectionError ErrorCode;              ///< Room Error code, failed to go inside room
            };

            /// SERVER_ROOM_READY packet builder
            class RoomReady final : public ServerPacket
            {
            public:
                /// Constructor 
                RoomReady() : ServerPacket(SERVER_ROOM_READY) {}

            public:
                /// Write the packet
                StringBuffer const* Write();

                std::string Model;
                uint32 Id;
            };

            /// SERVER_ROOM_ADD packet builder
            class RoomAdd final : public ServerPacket
            {
            public:
                /// Constructor 
                RoomAdd() : ServerPacket(SERVER_ROOM_ADD) {}

            public:
                /// Write the packet
                StringBuffer const* Write();
            };

            /// SERVER_ROOM_HEIGHT_MAP packet builder
            class RoomHeight final : public ServerPacket
            {
            public:
                /// Constructor 
                RoomHeight() : ServerPacket(SERVER_ROOM_HEIGHT_MAP) {}

            public:
                /// Write the packet
                StringBuffer const* Write();

                std::string HeightMap;
            };

            /// SERVER_OBJECTS_WORLD packet builder
            class ObjectsWorld final : public ServerPacket
            {
            public:
                /// Constructor 
                ObjectsWorld() : ServerPacket(SERVER_OBJECTS_WORLD) {}

            public:
                /// Write the packet
                StringBuffer const* Write();

                std::vector<WorldObject> WorldObjects;
            };

            /// SERVER_ACTIVE_OBJECTS packet builder
            class ActiveObjects final : public ServerPacket
            {
            public:
                /// Constructor 
                ActiveObjects() : ServerPacket(SERVER_ACTIVE_OBJECTS) {}

            public:
                /// Write the packet
                StringBuffer const* Write();

                std::vector<ActiveObject> ObjectsActive;
            };

            /// SERVER_USER_UPDATE_STATUS packet builder
            class UserUpdateStatus final : public ServerPacket
            {
            public:
                /// Constructor 
                UserUpdateStatus() : ServerPacket(SERVER_USER_UPDATE_STATUS) {}

            public:
                /// Write the packet
                StringBuffer const* Write();

                std::string GUID;
                std::string CurrentX;
                std::string CurrentY;
                std::string CurrentZ;
                std::string HeadRotation;
                std::string BodyRotation;
                bool Dancing;
                bool Sitting;
                bool Walking;
                bool Waving;
                std::string Status = "";
                std::string NewX;
                std::string NewY;
                std::string NewZ;
            };

            /// SERVER_LEAVE_ROOM packet builder
            class LeaveRoom final : public ServerPacket
            {
            public:
                /// Constructor 
                LeaveRoom() : ServerPacket(SERVER_LEAVE_ROOM) {}

            public:
                /// Write the packet
                StringBuffer const* Write();

                uint32 GUID;
            };

        } ///< NAMESPACE ROOM
    } ///< NAMESPACE HABBOPACKET
} ///< NAMESPACE STEERSTONE