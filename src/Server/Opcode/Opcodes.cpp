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
        StorePacket(OpcodesList::CMSG_INITIALIZE_CRYPTO,             "CMSG_INITIALIZE_CRYPTO",                &PlayerSocket::HandleInitializeCrypto           );
        StorePacket(OpcodesList::CMSG_GENERATE_KEY,                  "CMSG_GENERATE_KEY",                     &PlayerSocket::HandleGenerateKey                );
        StorePacket(OpcodesList::MSG_GDATE,                          "MSG_GDATE",                             &PlayerSocket::HandleGDate                      );
        StorePacket(OpcodesList::CMSG_APPROVE_NAME,                  "CMSG_APPROVE_NAME",                     &PlayerSocket::HandleApproveUsername            );
        StorePacket(OpcodesList::CMSG_APPROVE_PASSWORD,              "CMSG_APPROVE_PASSWORD",                 &PlayerSocket::HandleApprovePassword            );
        StorePacket(OpcodesList::CMSG_APPROVE_EMAIL,                 "CMSG_APPROVE_EMAIL",                    &PlayerSocket::HandleApproveEmail               );
        StorePacket(OpcodesList::CMSG_REGISTER,                      "CMSG_REGISTER",                         &PlayerSocket::HandleRegisteration              );

        ///< SMSG
        StorePacket(OpcodesList::SMSG_CRYPTO_PARAMETERS,             "SMSG_CRYPTO_PARAMETERS",                &PlayerSocket::HandleServerMessage              );
        StorePacket(OpcodesList::SMSG_SESSION_PARAMETERS,            "SMSG_SESSION_PARAMETERS",               &PlayerSocket::HandleServerMessage              );
        StorePacket(OpcodesList::SMSG_AVAILABLE_SETS,                "SMSG_AVAILABLE_SETS",                   &PlayerSocket::HandleServerMessage              );
        StorePacket(OpcodesList::SMSG_APPROVE_NAME_REPLY,            "SMSG_APPROVE_NAME_REPLY",               &PlayerSocket::HandleServerMessage              );
        StorePacket(OpcodesList::SMSG_APPROVE_PASSWORD_REPLY,        "SMSG_APPROVE_PASSWORD_REPLY",           &PlayerSocket::HandleServerMessage              );
        StorePacket(OpcodesList::SMSG_APPROVE_EMAIL_REPLY,           "SMSG_APPROVE_EMAIL_REPLY",              &PlayerSocket::HandleServerMessage              );
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