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
//-----------------------------------------------//
#include "../PlayerSocket.h"
//-----------------------------------------------//
namespace Quad
{
    //-----------------------------------------------//
    Opcodes* Opcodes::instance()
    {
        static Opcodes instance;
        return &instance;
    }
    //-----------------------------------------------//
    OpcodeHandler const Opcodes::emptyHandler =
    {
        "<none>",
        &PlayerSocket::HandleNULL
    };
    //-----------------------------------------------//
    void Opcodes::InitializePackets()
    {
        ///< CMSG
        StorePacket(OpcodesList::VERSIONCHECK,               "VERSIONCHECK",             &PlayerSocket::HandleVersionCheck       );
        StorePacket(OpcodesList::KEYENCRYPTED,               "KEYENCRYPTED",             &PlayerSocket::HandleNULL               );
        StorePacket(OpcodesList::CLIENTIP,                   "CLIENTIP",                 &PlayerSocket::HandleClientIP           );
        StorePacket(OpcodesList::APPROVENAME,                "APPROVENAME",              &PlayerSocket::HandleApproveUsername    );
        StorePacket(OpcodesList::REGISTER,                   "APPROVENAME",              &PlayerSocket::HandleRegisteration      );
        StorePacket(OpcodesList::LOGIN,                      "LOGIN",                    &PlayerSocket::HandleLogin              );
        StorePacket(OpcodesList::GETCREDITS,                 "GETCREDITS",               &PlayerSocket::HandleGetCredits         );
        StorePacket(OpcodesList::INFORETRIEVE,               "INFORETRIEVE",             &PlayerSocket::HandleInfoRetrieve       );
        StorePacket(OpcodesList::SEARCHBUSYFLATS,            "SEARCHBUSYFLATS",          &PlayerSocket::HandleSearchBusyFlats    );
        StorePacket(OpcodesList::INITUNITLISTENER,           "INITUNITLISTENER",         &PlayerSocket::HandleInitUnitListener   );
    } 
    //-----------------------------------------------//
    void Opcodes::StorePacket(const uint64& opcode, char const * name, void(PlayerSocket::* handler)(std::unique_ptr<Packet> packet))
    {
        OpcodeHandler& ref = mOpcode[opcode];
        ref.name = name;
        ref.handler = handler;
    }
    //-----------------------------------------------//
    OpcodeHandler const& Opcodes::GetPacket(const uint64& Id)
    {
        OpcodeMap::const_iterator itr = mOpcode.find(Id);
        if (itr != mOpcode.end())
            return itr->second;
        return emptyHandler;
    }
    //-----------------------------------------------//
    const char * Opcodes::GetOpCodeName(const uint64& Id)
    {
        OpcodeMap::const_iterator itr = mOpcode.find(Id);
        if (itr != mOpcode.end())
            return itr->second.name;
        return "NULL";
    }
    //-----------------------------------------------//
}
//-----------------------------------------------//