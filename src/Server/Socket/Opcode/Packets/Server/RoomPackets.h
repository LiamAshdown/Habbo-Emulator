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
            /// SERVER_USERS packet builder
            class HabboRoomObject final : public ServerPacket
            {
            public:
                /// Constructor                 
                HabboRoomObject() : ServerPacket(SERVER_USERS) {}

            public:
                /// Write the packet
                StringBuffer const* Write();

                std::string UniqueId;
                std::string Id;
                std::string Name;
                std::string Figure;
                boost::optional<std::string> PoolFigure;
                std::string Gender;
                std::string X;
                std::string Y;
                std::string Z;
                boost::optional<std::string> Badge;
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

            /// SERVER_CLC_OK packet builder
            class OpenConnection final : public ServerPacket
            {
            public:
                /// Constructor 
                OpenConnection() : ServerPacket(SERVER_CLC_OK) {}

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

                boost::optional<std::string> ImageUrl;
                boost::optional<std::string> LinkUrl;
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

            /// SERVER_OBJECTS packet builder
            class ObjectsWorld final : public ServerPacket
            {
            public:
                /// Constructor 
                ObjectsWorld() : ServerPacket(SERVER_OBJECTS) {}

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
                UserUpdateStatus() : ServerPacket(SERVER_STATUS) {}

            public:
                /// Write the packet
                StringBuffer const* Write();

                std::string GUID;
                std::string CurrentX;
                std::string CurrentY;
                std::string CurrentZ;
                std::string HeadRotation;
                std::string BodyRotation;
                std::string DanceId;
                bool Dancing;
                bool Sitting;
                bool Walking;
                bool Waving;
                bool Swimming;
                bool HasController;
                bool IsOwner;
                std::string Status = "";
                std::string NewX;
                std::string NewY;
                std::string NewZ;
            };

            /// SERVER_LOGOUT packet builder
            class Logout final : public ServerPacket
            {
            public:
                /// Constructor 
                Logout() : ServerPacket(SERVER_LOGOUT) {}

            public:
                /// Write the packet
                StringBuffer const* Write();

                std::string GUID;
            };

            /// SERVER_ROOM_CHAT packet builder
            class Chat final : public ServerPacket
            {
            public:
                /// Constructor 
                Chat() : ServerPacket(SERVER_CHAT) {}

            public:
                /// Write the packet
                StringBuffer const* Write();

                uint32 GUID;
                std::string Message;
            };

            /// SERVER_ROOM_SHOUT packet builder
            class Shout final : public ServerPacket
            {
            public:
                /// Constructor 
                Shout() : ServerPacket(SERVER_SHOUT) {}

            public:
                /// Write the packet
                StringBuffer const* Write();

                uint32 GUID;
                std::string Message;
            };

            /// SERVER_ROOM_WHISPER packet builder
            class Whisper final : public ServerPacket
            {
            public:
                /// Constructor 
                Whisper() : ServerPacket(SERVER_WHISPER) {}

            public:
                /// Write the packet
                StringBuffer const* Write();

                uint32 GUID;
                std::string Message;
            };
            
            /// SERVER_ROOM_WHISPER packet builder
            class OpenUIMakeOPPI final : public ServerPacket
            {
            public:
                /// Constructor 
                OpenUIMakeOPPI() : ServerPacket(SERVER_OPEN_UI_MAKE_OPPI) {}

            public:
                /// Write the packet
                StringBuffer const* Write();
            };
           
            /// SERVER_USE_BADGE packet builder
            class UseBadge final : public ServerPacket
            {
            public:
                /// Constructor 
                UseBadge() : ServerPacket(SERVER_USE_BADGE) {}

            public:
                /// Write the packet
                StringBuffer const* Write();

                uint32 GUID;
                std::string Badge;
                bool BadgeVisible;
            };

            /// SERVER_GO_TO_FLAT packet builder
            class GoToFlat final : public ServerPacket
            {
            public:
                /// Constructor 
                GoToFlat() : ServerPacket(SERVER_GO_TO_FLAT) {}

            public:
                /// Write the packet
                StringBuffer const* Write();

                std::string Id;
                std::string Name;
            };

            /// SERVER_ITEMS packet builder
            class Items final : public ServerPacket
            {
            public:
                /// Constructor 
                Items() : ServerPacket(SERVER_ITEMS) {}

            public:
                /// Write the packet
                StringBuffer const* Write();
            };

            /// SERVER_FLAT_NOT_ALLOWED_TO_ENTER packet builder
            class FlatNotAllowedToEnter final : public ServerPacket
            {
            public:
                /// Constructor 
                FlatNotAllowedToEnter() : ServerPacket(SERVER_FLAT_NOT_ALLOWED_TO_ENTER) {}

            public:
                /// Write the packet
                StringBuffer const* Write();
            };

            /// SERVER_DOOR_BELL_RINGING packet builder
            class DoorBellRinging final : public ServerPacket
            {
            public:
                /// Constructor 
                DoorBellRinging() : ServerPacket(SERVER_DOOR_BELL_RINGING) {}

            public:
                /// Write the packet
                StringBuffer const* Write();

                boost::optional<std::string> Name;
            };

            /// SERVER_FLAT_LET_IN packet builder
            class FlatLetIn final : public ServerPacket
            {
            public:
                /// Constructor 
                FlatLetIn() : ServerPacket(SERVER_FLAT_LET_IN) {}

            public:
                /// Write the packet
                StringBuffer const* Write();
            };

            /// SERVER_YOU_ARE_CONTROLLER packet builder
            class YouAreController final : public ServerPacket
            {
            public:
                /// Constructor 
                YouAreController() : ServerPacket(SERVER_YOU_ARE_CONTROLLER) {}

            public:
                /// Write the packet
                StringBuffer const* Write();
            };

            /// SERVER_YOU_ARE_NOT_CONTROLLER packet builder
            class YouAreNotController final : public ServerPacket
            {
            public:
                /// Constructor 
                YouAreNotController() : ServerPacket(SERVER_YOU_ARE_NOT_CONTROLLER) {}

            public:
                /// Write the packet
                StringBuffer const* Write();
            };

            /// SERVER_YOU_ARE_OWNER packet builder
            class YouAreOwner final : public ServerPacket
            {
            public:
                /// Constructor 
                YouAreOwner() : ServerPacket(SERVER_YOU_ARE_OWNER) {}

            public:
                /// Write the packet
                StringBuffer const* Write();
            };

            /// SERVER_CLC packet builder
            class CLC final : public ServerPacket
            {
            public:
                /// Constructor 
                CLC() : ServerPacket(SERVER_CLC) {}

            public:
                /// Write the packet
                StringBuffer const* Write();
            };


        } ///< NAMESPACE ROOM
    } ///< NAMESPACE HABBOPACKET
} ///< NAMESPACE STEERSTONE