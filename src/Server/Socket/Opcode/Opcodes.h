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

#ifndef _Quad_Opcodes_h_
#define _Quad_Opcodes_h_
#include "Common/SharedDefines.h"
#include "Network/ClientPacket.h"

enum PacketClientHeader : uint16
{
    ///< Authentication
    CLIENT_INITIALIZE_CRYPTO              = 206,
    CLIENT_GENERATE_KEY                   = 202,
    CLIENT_GDATE                          = 49,
    CLIENT_APPROVE_NAME                   = 42,
    CLIENT_APPROVE_PASSWORD               = 203,
    CLIENT_APPROVE_EMAIL                  = 197,
    CLIENT_REGISTER                       = 43,
    CLIENT_TRY_LOGIN                      = 4,
    CLIENT_GET_INFO                       = 7,
    CLIENT_GET_CREDITS                    = 8,
    CLIENT_FLAT_CATALOGUE                 = 222,
    CLIENT_NO_DESPACE_USERS               = 154,
    CLIENT_GET_CLUB                       = 26,
    CLIENT_NAVIGATE                       = 150,
    CLIENT_GET_USER_FLAT_CATEGORYS        = 151,
    CLIENT_GET_FAVOURITE_ROOMS            = 18,
    CLIENT_ADD_FAVOURITE_ROOM             = 19,
    CLIENT_DELETE_FAVOURITE_ROOM          = 20,
    CLIENT_SEARCH_ROOMS                   = 17,
    CLIENT_GET_INTEREST                   = 182,
    CLIENT_MESSENGER_INIT                 = 12,
    CLIENT_ROOM_DIRECTORY                 = 2,
    CLIENT_PONG                           = 196,
    CLIENT_GET_ROOM_ADD                   = 126,
    CLIENT_ROOM_HEIGHT_MAP                = 60,
    CLIENT_GET_AVAILABLE_BADGES           = 157,
    CLIENT_GET_ACCOUNT_PREFERENCES        = 228,
    CLIENT_ROOM_USERS                     = 61,
    CLIENT_MESSENGER_UPDATE               = 15,
    CLIENT_MESSENGER_ACCEPT_REQUEST       = 37,
    CLIENT_GAME_OBJECTS                   = 62,
    CLIENT_FURNITURE_REVISIONS            = 213,
    CLIENT_MESSENGER_FIND_USER            = 41,
    CLIENT_MESSENGER_SEND_REQUEST         = 39,
    CLIENT_LEAVE_ROOM                     = 53,
    CLIENT_MESSENGER_REMOVE_FRIEND        = 40,
    CLIENT_MESSENGER_REJECT_REQUEST       = 38,
    CLIENT_MESSENGER_SEND_MESSAGE         = 33,
    CLIENT_MESSENGER_REPLY                = 32,
    CLIENT_HABBO_MOVE                     = 75,
    CLIENT_HABBO_DANCE                    = 93,  
    CLIENT_HABBO_STOP_DANCE               = 88,
    CLIENT_ROOM_HABBO_STATUSES            = 64,
    CLIENT_HABBO_WAVE                     = 94,  
    CLIENT_LOOK_TO                        = 79,
};

enum PacketServerHeader : uint16
{
    SERVER_CRYPTO_PARAMETERS              = 277,
    SERVER_SESSION_PARAMETERS             = 257,
    SERVER_AVAILABLE_SETS                 = 8,
    SERVER_APPROVE_NAME_REPLY             = 36,
    SERVER_APPROVE_PASSWORD_REPLY         = 282,
    SERVER_APPROVE_EMAIL_REPLY            = 271,
    SERVER_LOCALISED_ERROR                = 33,
    SERVER_LOGIN                          = 3,
    SERVER_ALERT                          = 139,
    SERVER_NOTICE                         = 139,
    SERVER_HABBO_OBJECT                   = 5,
    SERVER_CREDIT_BALANCE                 = 6,
    SERVER_NAVIGATE_NODE_INFO             = 220,
    SERVER_USER_FLAT_CATEGORIES           = 221,
    SERVER_FAVOURITE_ROOMS_RESULT         = 61,
    SERVER_SEARCH_RESULTS                 = 55,
    SERVER_ROOM_INTEREST                  = 258,
    SERVER_MESSENGER_INIT                 = 12,
    SERVER_OPEN_CONNECTION                = 19,
    SERVER_ROOM_URL                       = 166,
    SERVER_ROOM_CANT_CONNECT              = 224,
    SERVER_PING                           = 50,
    SERVER_ROOM_READY                     = 69,
    SERVER_ROOM_ADD                       = 208,
    SERVER_ROOM_HEIGHT_MAP                = 31,
    SERVER_FUSE_RIGHTS                    = 2,
    SERVER_AVAILABLE_BADGES               = 229,
    SERVER_ACCOUNT_PREFERENCES            = 308,
    SERVER_MESSENGER_SEND_FRIEND_REQUEST  = 132,
    SERVER_ROOM_USER_OBJECTS              = 28,
    SERVER_MESSENGER_UPDATE               = 13,
    SERVER_OBJECTS_WORLD                  = 30,
    SERVER_ACTIVE_OBJECTS                 = 32,
    SERVER_ALIAS_TOGGLE                   = 297,
    SERVER_USER_RIGHTS                    = 2,
    SERVER_HOTEL_LOGOUT                   = 287,
    SERVER_MESSENGER_FIND_USER_RESULT     = 128,
    SERVER_GDATE                          = 49,
    SERVER_MESSENGER_ERROR                = 260,
    SERVER_REQUEST_BUDDY_ERROR            = 315,
    SERVER_MESSENGER_ADD_FRIEND           = 137,
    SERVER_MESSENGER_REMOVE_FRIEND        = 138,
    SERVER_MESSENGER_SEND_MESSAGE         = 134,
    SERVER_USER_UPDATE_STATUS             = 34,
    SERVER_LEAVE_ROOM                     = 29,
};

namespace SteerStone
{
    class HabboSocket;

    struct OpcodeHandler
    {
        char const* name;
        void (HabboSocket::*handler)(std::unique_ptr<ClientPacket> p_Packet);
    };

    typedef std::map<uint64, OpcodeHandler> OpcodeMap;

    class Opcodes
    {
    public:
        static Opcodes* instance();

    public:
        Opcodes() {}
        ~Opcodes() {}

    public:
        void InitializePackets();
        OpcodeHandler const& GetClientPacket(const uint64& Id);
        OpcodeHandler const& GetServerPacket(const uint64& Id);

        const char* GetClientPacketName(const uint64& Id);
        const char* GetServerPacketName(const uint64& Id);

    private:
        void StoreClientPacket(const uint64& opcode, char const* name, void (HabboSocket::*handler)(std::unique_ptr<ClientPacket> p_Packet));
        void StoreServerPacket(const uint64& opcode, char const* name, void (HabboSocket::*handler)(std::unique_ptr<ClientPacket> p_Packet));

    private:
        static OpcodeHandler const emptyHandler;
        OpcodeMap mClientOpcode;
        OpcodeMap mServerOpcode;
    };
}

#define sOpcode SteerStone::Opcodes::instance()

#endif /* _Quad_Opcodes_h_ */