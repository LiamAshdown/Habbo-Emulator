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
#include "PacketStructures.h"

enum OpcodesList : uint16
{
    ///< CMSG
    CMSG_INITIALIZE_CRYPTO              = 206,
    CMSG_GENERATE_KEY                   = 202,
    MSG_GDATE                           = 49,
    CMSG_APPROVE_NAME                   = 42,
    CMSG_APPROVE_PASSWORD               = 203,
    CMSG_APPROVE_EMAIL                  = 197,
    CMSG_REGISTER                       = 43,

    ///< SMSG
    SMSG_CRYPTO_PARAMETERS              = 277,
    SMSG_SESSION_PARAMETERS             = 257,
    SMSG_AVAILABLE_SETS                 = 8,
    SMSG_APPROVE_NAME_REPLY             = 36,
    SMSG_APPROVE_PASSWORD_REPLY         = 282,
    SMSG_APPROVE_EMAIL_REPLY            = 271,
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
        OpcodeHandler const& GetPacket(const uint64& Id);

        const char* GetOpCodeName(const uint64& Id);

    private:
        void StorePacket(const uint64& opcode, char const* name, void (PlayerSocket::*handler)(std::unique_ptr<Packet> packet));

    private:
        static OpcodeHandler const emptyHandler;
        OpcodeMap mOpcode;
    };
}

#define sOpcode Quad::Opcodes::instance()

#endif /* _Quad_Opcodes_h_ */