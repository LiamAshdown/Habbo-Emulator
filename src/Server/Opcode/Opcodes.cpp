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
        StoreClientPacket(OpcodesClient::CMSG_INITIALIZE_CRYPTO,             "CMSG_INITIALIZE_CRYPTO",                &PlayerSocket::HandleInitializeCrypto           );
        StoreClientPacket(OpcodesClient::CMSG_GENERATE_KEY,                  "CMSG_GENERATE_KEY",                     &PlayerSocket::HandleGenerateKey                );
        StoreClientPacket(OpcodesClient::MSG_GDATE,                          "MSG_GDATE",                             &PlayerSocket::HandleGDate                      );
        StoreClientPacket(OpcodesClient::CMSG_APPROVE_NAME,                  "CMSG_APPROVE_NAME",                     &PlayerSocket::HandleApproveUsername            );
        StoreClientPacket(OpcodesClient::CMSG_APPROVE_PASSWORD,              "CMSG_APPROVE_PASSWORD",                 &PlayerSocket::HandleApprovePassword            );
        StoreClientPacket(OpcodesClient::CMSG_APPROVE_EMAIL,                 "CMSG_APPROVE_EMAIL",                    &PlayerSocket::HandleApproveEmail               );
        StoreClientPacket(OpcodesClient::CMSG_REGISTER,                      "CMSG_REGISTER",                         &PlayerSocket::HandleRegisteration              );
        StoreClientPacket(OpcodesClient::CMSG_TRY_LOGIN,                     "CMSG_TRY_LOGIN",                        &PlayerSocket::HandleTryLogin                   );
        StoreClientPacket(OpcodesClient::CMSG_GET_INFO,                      "CMSG_GET_INFO",                         &PlayerSocket::HandleGetInfo                    );
        StoreClientPacket(OpcodesClient::CMSG_GET_CREDITS,                   "CMSG_GET_CREDITS",                      &PlayerSocket::HandleCreditBalance              );
        StoreClientPacket(OpcodesClient::CMSG_FLAT_CATALOGUE,                "CMSG_FLAT_CATALOGUE",                   &PlayerSocket::HandleNULL                       );
        StoreClientPacket(OpcodesClient::MSG_NO_DESPACE_USERS,               "MSG_NO_DESPACE_USERS",                  &PlayerSocket::HandleNoDespaceUsers             );
        StoreClientPacket(OpcodesClient::CMSG_GET_CLUB,                      "CMSG_GET_CLUB",                         &PlayerSocket::HandleGetClub                    );
        StoreClientPacket(OpcodesClient::CMSG_NAVIGATE,                      "CMSG_NAVIGATE",                         &PlayerSocket::HandleNavigate                   );
        StoreClientPacket(OpcodesClient::CMSG_GET_USER_FLAT_CATEGORYS,       "CMSG_GET_USER_FLAT_CATEGORYS",          &PlayerSocket::HandleGetUserFlatsCategories      );
        StoreClientPacket(OpcodesClient::CMSG_GET_FAVOURITE_ROOMS,           "CMSG_GET_FAVOURITE_ROOMS",              &PlayerSocket::HandleGetFavouriteRooms          );
        StoreClientPacket(OpcodesClient::CMSG_ADD_FAVOURITE_ROOM,            "CMSG_ADD_FAVOURITE_ROOM",               &PlayerSocket::HandleAddFavouriteRoom           );
        StoreClientPacket(OpcodesClient::CMSG_DELETE_FAVOURITE_ROOM,         "CMSG_DELETE_FAVOURITE_ROOM",            &PlayerSocket::HandleDeleteFavouriteRoom        );
        StoreClientPacket(OpcodesClient::CMSG_SEARCH_ROOMS,                  "CMSG_SEARCH_ROOMS",                     &PlayerSocket::HandleSearchRooms                );
        StoreClientPacket(OpcodesClient::CMSG_GET_INTEREST,                  "CMSG_GET_INTEREST",                     &PlayerSocket::HandleGetInterest                );
        StoreClientPacket(OpcodesClient::CMSG_MESSENGER_INIT,                "CMSG_MESSENGER_INIT",                   &PlayerSocket::HandleMessengerInitialize        );
        StoreClientPacket(OpcodesClient::CMSG_ROOM_DIRECTORY,                "CMSG_ROOM_DIRECTORY",                   &PlayerSocket::HandleRoomDirectory              );
        StoreClientPacket(OpcodesClient::CMSG_PONG,                          "CMSG_PONG",                             &PlayerSocket::HandlePong                       );

        ///< SMSG
        StoreServerPacket(OpcodesServer::SMSG_CRYPTO_PARAMETERS,             "SMSG_CRYPTO_PARAMETERS",                &PlayerSocket::HandleServerMessage              );
        StoreServerPacket(OpcodesServer::SMSG_SESSION_PARAMETERS,            "SMSG_SESSION_PARAMETERS",               &PlayerSocket::HandleServerMessage              );
        StoreServerPacket(OpcodesServer::SMSG_AVAILABLE_SETS,                "SMSG_AVAILABLE_SETS",                   &PlayerSocket::HandleServerMessage              );
        StoreServerPacket(OpcodesServer::SMSG_APPROVE_NAME_REPLY,            "SMSG_APPROVE_NAME_REPLY",               &PlayerSocket::HandleServerMessage              );
        StoreServerPacket(OpcodesServer::SMSG_APPROVE_PASSWORD_REPLY,        "SMSG_APPROVE_PASSWORD_REPLY",           &PlayerSocket::HandleServerMessage              );
        StoreServerPacket(OpcodesServer::SMSG_APPROVE_EMAIL_REPLY,           "SMSG_APPROVE_EMAIL_REPLY",              &PlayerSocket::HandleServerMessage              );
        StoreServerPacket(OpcodesServer::SMSG_MODERATOR_ALERT,               "SMSG_MODERATOR_ALERT",                  &PlayerSocket::HandleServerMessage              );
        StoreServerPacket(OpcodesServer::SMSG_LOGIN,                         "SMSG_LOGIN",                            &PlayerSocket::HandleServerMessage              );
        StoreServerPacket(OpcodesServer::SMSG_ALERT,                         "SMSG_ALERT",                            &PlayerSocket::HandleServerMessage              );
        StoreServerPacket(OpcodesServer::SMSG_NOTICE,                        "SMSG_NOTICE",                           &PlayerSocket::HandleServerMessage              );
        StoreServerPacket(OpcodesServer::SMSG_USER_OBJECT,                   "SMSG_USER_OBJECT",                      &PlayerSocket::HandleServerMessage              );
        StoreServerPacket(OpcodesServer::SMSG_CREDIT_BALANCE,                "SMSG_CREDIT_BALANCE",                   &PlayerSocket::HandleServerMessage              );
        StoreServerPacket(OpcodesServer::SMSG_USER_FLAT_CATEGORIES,           "SMSG_USER_FLAT_CATEGORIES",              &PlayerSocket::HandleServerMessage              );
        StoreServerPacket(OpcodesServer::SMSG_SEARCH_RESULTS,                "SMSG_SEARCH_RESULTS",                   &PlayerSocket::HandleServerMessage              );
        StoreServerPacket(OpcodesServer::SMSG_ROOM_INTEREST,                 "SMSG_ROOM_INTEREST",                    &PlayerSocket::HandleServerMessage              );
        StoreServerPacket(OpcodesServer::SMSG_OPEN_CONNECTION,               "SMSG_OPEN_CONNECTION",                  &PlayerSocket::HandleServerMessage              );
        StoreServerPacket(OpcodesServer::SMSG_ROOM_URL,                      "SMSG_OPEN_CONNECTION",                  &PlayerSocket::HandleServerMessage              );
        StoreServerPacket(OpcodesServer::SMSG_CANT_CONNECT,                  "SMSG_CANT_CONNECT",                     &PlayerSocket::HandleServerMessage              );
        StoreServerPacket(OpcodesServer::SMSG_PING,                          "SMSG_PING",                             &PlayerSocket::HandleServerMessage              );
        StoreServerPacket(OpcodesServer::SMSG_ROOM_READY,                    "SMSG_ROOM_READY",                       &PlayerSocket::HandleServerMessage              );

        LOG_INFO << "Loaded " << mClientOpcode.size() << " CMSG opcodes";
        LOG_INFO << "Loaded " << mServerOpcode.size() << " SMSG opcodes";
    } 
    //-----------------------------------------------//
    void Opcodes::StoreClientPacket(const uint64& opcode, char const * name, void(PlayerSocket::* handler)(std::unique_ptr<Packet> packet))
    {
        OpcodeHandler& ref = mClientOpcode[opcode];
        ref.name = name;
        ref.handler = handler;
    }
    //-----------------------------------------------//
    void Opcodes::StoreServerPacket(const uint64& opcode, char const * name, void(PlayerSocket::* handler)(std::unique_ptr<Packet> packet))
    {
        OpcodeHandler& ref = mServerOpcode[opcode];
        ref.name = name;
        ref.handler = handler;
    }
    //-----------------------------------------------//
    OpcodeHandler const& Opcodes::GetClientPacket(const uint64& Id)
    {
        OpcodeMap::const_iterator itr = mClientOpcode.find(Id);
        if (itr != mClientOpcode.end())
            return itr->second;
        return emptyHandler;
    }
    //-----------------------------------------------//
    const char * Opcodes::GetClientPacketName(const uint64& Id)
    {
        OpcodeMap::const_iterator itr = mClientOpcode.find(Id);
        if (itr != mClientOpcode.end())
            return itr->second.name;
        return "NULL";
    }
    //-----------------------------------------------//
    OpcodeHandler const& Opcodes::GetServerPacket(const uint64& Id)
    {
        OpcodeMap::const_iterator itr = mClientOpcode.find(Id);
        if (itr != mServerOpcode.end())
            return itr->second;
        return emptyHandler;
    }
    //-----------------------------------------------//
    const char * Opcodes::GetServerPacketName(const uint64& Id)
    {
        OpcodeMap::const_iterator itr = mClientOpcode.find(Id);
        if (itr != mServerOpcode.end())
            return itr->second.name;
        return "NULL";
    }
    //-----------------------------------------------//
}
//-----------------------------------------------//