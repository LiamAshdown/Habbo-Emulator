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
    VERSIONCHECK                = 900,
    KEYENCRYPTED                = 919,
    CLIENTIP                    = 600,
    APPROVENAME                 = 830,
    REGISTER                    = 613,
    STATUSOK                    = 638,
    LOGIN                       = 377,
    GETCREDITS                  = 750,
    SEARCHBUSYFLATS             = 1139,
    INFORETRIEVE                = 914,
    INITUNITLISTENER            = 1242,
    MOVE                        = 407,
    CHAT                        = 288,
    SHOUT                       = 403,
    WHISPER                     = 546,
    CREATEFLAT                  = 731,
    SETFLATINFO                 = 831,
    TRYFLAT                     = 550,
    GOTOFLAT                    = 608,
    GETFLATINFO                 = 819,
    UPDATEFLAT                  = 746,
    DELETEFLAT                  = 730,
    GOAWAY                      = 456,
    DANCE                       = 475,
    STOP                        = 326,
    LOOKTO                      = 472,
    GETORDERINFO                = 904,
    PURCHASE                    = 603,
    GETSTRIP                    = 626,
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