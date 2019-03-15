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
#include "Network/Packet.h"

enum OpcodesClient : uint16
{
    CMSG_INITIALIZE_CRYPTO              = 206,
    CMSG_GENERATE_KEY                   = 202,
    MSG_GDATE                           = 49,
    CMSG_APPROVE_NAME                   = 42,
    CMSG_APPROVE_PASSWORD               = 203,
    CMSG_APPROVE_EMAIL                  = 197,
    CMSG_REGISTER                       = 43,
    CMSG_TRY_LOGIN                      = 4,
    CMSG_GET_INFO                       = 7,
    CMSG_GET_CREDITS                    = 8,
    CMSG_FLAT_CATALOGUE                 = 222,
    MSG_NO_DESPACE_USERS                = 223,
    CMSG_GET_CLUB                       = 26,
    CMSG_NAVIGATE                       = 150,
    CMSG_GET_USER_FLAT_CATEGORYS        = 151,
    CMSG_GET_FAVOURITE_ROOMS            = 18,
    CMSG_ADD_FAVOURITE_ROOM             = 19,
    CMSG_DELETE_FAVOURITE_ROOM          = 20,
    CMSG_SEARCH_ROOMS                   = 17,
    CMSG_GET_INTEREST                   = 182,
    CMSG_MESSENGER_INIT                 = 12,
    CMSG_ROOM_DIRECTORY                 = 2,
    CMSG_PONG                           = 196,
};

enum OpcodesServer : uint16
{
    SMSG_CRYPTO_PARAMETERS              = 277,
    SMSG_SESSION_PARAMETERS             = 257,
    SMSG_AVAILABLE_SETS                 = 8,
    SMSG_APPROVE_NAME_REPLY             = 36,
    SMSG_APPROVE_PASSWORD_REPLY         = 282,
    SMSG_APPROVE_EMAIL_REPLY            = 271,
    SMSG_MODERATOR_ALERT                = 33,
    SMSG_LOGIN                          = 3,
    SMSG_ALERT                          = 139,
    SMSG_NOTICE                         = 139,
    SMSG_USER_OBJECT                    = 5,
    SMSG_CREDIT_BALANCE                 = 6,
    SMSG_NAVIGATE_NODE_INFO             = 220,
    SMSG_USER_FLAT_CATEGORIES           = 221,
    SMSG_FAVOURITE_ROOMS_RESULT         = 61,
    SMSG_SEARCH_RESULTS                 = 55,
    SMSG_ROOM_INTEREST                  = 258,
    SMSG_MESSENGER_INIT                 = 12,
    SMSG_OPEN_CONNECTION                = 19,
    SMSG_ROOM_URL                       = 166,
    SMSG_CANT_CONNECT                   = 224,
    SMSG_PING                           = 50,
    SMSG_ROOM_READY                     = 69,
};

namespace Quad
{
    class PlayerSocket;

    struct OpcodeHandler
    {
        char const* name;
        void (PlayerSocket::*handler)(std::unique_ptr<Packet> packet);
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
        void StoreClientPacket(const uint64& opcode, char const* name, void (PlayerSocket::*handler)(std::unique_ptr<Packet> packet));
        void StoreServerPacket(const uint64& opcode, char const* name, void (PlayerSocket::*handler)(std::unique_ptr<Packet> packet));

    private:
        static OpcodeHandler const emptyHandler;
        OpcodeMap mClientOpcode;
        OpcodeMap mServerOpcode;
    };
}

#define sOpcode Quad::Opcodes::instance()

#endif /* _Quad_Opcodes_h_ */