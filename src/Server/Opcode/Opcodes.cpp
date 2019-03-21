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
        StoreClientPacket(PacketClientHeader::CLIENT_INITIALIZE_CRYPTO,             "CLIENT_INITIALIZE_CRYPTO",                &PlayerSocket::HandleInitializeCrypto           );
        StoreClientPacket(PacketClientHeader::CLIENT_GENERATE_KEY,                  "CLIENT_GENERATE_KEY",                     &PlayerSocket::HandleGenerateKey                );
        StoreClientPacket(PacketClientHeader::CLIENT_GDATE,                         "CLIENT_GDATE",                            &PlayerSocket::HandleGDate                      );
        StoreClientPacket(PacketClientHeader::CLIENT_APPROVE_NAME,                  "CLIENT_APPROVE_NAME",                     &PlayerSocket::HandleApproveUsername            );
        StoreClientPacket(PacketClientHeader::CLIENT_APPROVE_PASSWORD,              "CLIENT_APPROVE_PASSWORD",                 &PlayerSocket::HandleApprovePassword            );
        StoreClientPacket(PacketClientHeader::CLIENT_APPROVE_EMAIL,                 "CLIENT_APPROVE_EMAIL",                    &PlayerSocket::HandleApproveEmail               );
        StoreClientPacket(PacketClientHeader::CLIENT_REGISTER,                      "CLIENT_REGISTER",                         &PlayerSocket::HandleRegisteration              );
        StoreClientPacket(PacketClientHeader::CLIENT_TRY_LOGIN,                     "CLIENT_TRY_LOGIN",                        &PlayerSocket::HandleTryLogin                   );
        StoreClientPacket(PacketClientHeader::CLIENT_GET_INFO,                      "CLIENT_GET_INFO",                         &PlayerSocket::HandleGetInfo                    );
        StoreClientPacket(PacketClientHeader::CLIENT_GET_CREDITS,                   "CLIENT_GET_CREDITS",                      &PlayerSocket::HandleCreditBalance              );
        StoreClientPacket(PacketClientHeader::CLIENT_FLAT_CATALOGUE,                "CLIENT_FLAT_CATALOGUE",                   &PlayerSocket::HandleNULL                       );
        StoreClientPacket(PacketClientHeader::CLIENT_NO_DESPACE_USERS,              "CLIENT_NO_DESPACE_USERS",                 &PlayerSocket::HandleNULL);
        StoreClientPacket(PacketClientHeader::CLIENT_GET_CLUB,                      "CLIENT_GET_CLUB",                         &PlayerSocket::HandleGetClub                    );
        StoreClientPacket(PacketClientHeader::CLIENT_NAVIGATE,                      "CLIENT_NAVIGATE",                         &PlayerSocket::HandleNavigate                   );
        StoreClientPacket(PacketClientHeader::CLIENT_GET_USER_FLAT_CATEGORYS,       "CLIENT_GET_USER_FLAT_CATEGORYS",          &PlayerSocket::HandleGetUserFlatsCategories     );
        StoreClientPacket(PacketClientHeader::CLIENT_GET_FAVOURITE_ROOMS,           "CLIENT_GET_FAVOURITE_ROOMS",              &PlayerSocket::HandleGetFavouriteRooms          );
        StoreClientPacket(PacketClientHeader::CLIENT_ADD_FAVOURITE_ROOM,            "CLIENT_ADD_FAVOURITE_ROOM",               &PlayerSocket::HandleAddFavouriteRoom           );
        StoreClientPacket(PacketClientHeader::CLIENT_DELETE_FAVOURITE_ROOM,         "CLIENT_DELETE_FAVOURITE_ROOM",            &PlayerSocket::HandleRemoveFavouriteRoom        );
        StoreClientPacket(PacketClientHeader::CLIENT_SEARCH_ROOMS,                  "CLIENT_SEARCH_ROOMS",                     &PlayerSocket::HandleSearchRooms                );
        StoreClientPacket(PacketClientHeader::CLIENT_GET_INTEREST,                  "CLIENT_GET_INTEREST",                     &PlayerSocket::HandleGetInterest                );
        StoreClientPacket(PacketClientHeader::CLIENT_MESSENGER_INIT,                "CLIENT_MESSENGER_INIT",                   &PlayerSocket::HandleMessengerInitialize        );
        StoreClientPacket(PacketClientHeader::CLIENT_ROOM_DIRECTORY,                "CLIENT_ROOM_DIRECTORY",                   &PlayerSocket::HandleRoomDirectory              );
        StoreClientPacket(PacketClientHeader::CLIENT_PONG,                          "CLIENT_PONG",                             &PlayerSocket::HandlePong                       );
        StoreClientPacket(PacketClientHeader::CLIENT_STOP,                          "CLIENT_STOP",                             &PlayerSocket::HandleNULL                       );
        StoreClientPacket(PacketClientHeader::CLIENT_GET_ROOM_ADD,                  "CLIENT_GET_ROOM_ADD",                     &PlayerSocket::HandleGetRoomAdd                 );
        StoreClientPacket(PacketClientHeader::CLIENT_ROOM_HEIGHT_MAP,               "CLIENT_ROOM_HEIGHT_MAP",                  &PlayerSocket::HandleGetRoomHeight              );
        StoreClientPacket(PacketClientHeader::CLIENT_GET_AVAILABLE_BADGES,          "CLIENT_GET_AVAILABLE_BADGES",             &PlayerSocket::HandleGetAvailableBadges         );
        StoreClientPacket(PacketClientHeader::CLIENT_GET_ACCOUNT_PREFERENCES,       "CLIENT_GET_ACCOUNT_PREFERENCES",          &PlayerSocket::HandleGetAccountPreferences      );
        StoreClientPacket(PacketClientHeader::CLIENT_ROOM_USERS,                    "CLIENT_ROOM_USERS",                       &PlayerSocket::HandleRoomUsers                  );
        StoreClientPacket(PacketClientHeader::CLIENT_MESSENGER_UPDATE,              "CLIENT_MESSENGER_UPDATE",                 &PlayerSocket::HandleMessengerUpdate            );
        StoreClientPacket(PacketClientHeader::CLIENT_MESSENGER_ACCEPT_REQUEST,      "CLIENT_MESSENGER_ACCEPT_REQUEST",         &PlayerSocket::HandleMessengerAcceptRequest     );
        StoreClientPacket(PacketClientHeader::CLIENT_GAME_OBJECTS,                  "CLIENT_GAME_OBJECTS",                     &PlayerSocket::HandleGameObjects                );
        StoreClientPacket(PacketClientHeader::CLIENT_FURNITURE_REVISIONS,           "CLIENT_FURNITURE_REVISIONS",              &PlayerSocket::HandleFurnitureRevisions         );

        ///< SMSG
        StoreServerPacket(PacketServerHeader::SERVER_CRYPTO_PARAMETERS,             "SERVER_CRYPTO_PARAMETERS",                &PlayerSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_SESSION_PARAMETERS,            "SERVER_SESSION_PARAMETERS",               &PlayerSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_AVAILABLE_SETS,                "SERVER_AVAILABLE_SETS",                   &PlayerSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_APPROVE_NAME_REPLY,            "SERVER_APPROVE_NAME_REPLY",               &PlayerSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_APPROVE_PASSWORD_REPLY,        "SERVER_APPROVE_PASSWORD_REPLY",           &PlayerSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_APPROVE_EMAIL_REPLY,           "SERVER_APPROVE_EMAIL_REPLY",              &PlayerSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_LOCALISED_ERROR,               "SERVER_LOCALISED_ERROR",                  &PlayerSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_LOGIN,                         "SERVER_LOGIN",                            &PlayerSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_ALERT,                         "SERVER_ALERT",                            &PlayerSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_NOTICE,                        "SERVER_NOTICE",                           &PlayerSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_USER_OBJECT,                   "SERVER_USER_OBJECT",                      &PlayerSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_CREDIT_BALANCE,                "SERVER_CREDIT_BALANCE",                   &PlayerSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_USER_FLAT_CATEGORIES,          "SERVER_USER_FLAT_CATEGORIES",             &PlayerSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_SEARCH_RESULTS,                "SERVER_SEARCH_RESULTS",                   &PlayerSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_ROOM_INTEREST,                 "SERVER_ROOM_INTEREST",                    &PlayerSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_OPEN_CONNECTION,               "SERVER_OPEN_CONNECTION",                  &PlayerSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_ROOM_URL,                      "SERVER_OPEN_CONNECTION",                  &PlayerSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_CANT_CONNECT,                  "SERVER_CANT_CONNECT",                     &PlayerSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_PING,                          "SERVER_PING",                             &PlayerSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_ROOM_READY,                    "SERVER_ROOM_READY",                       &PlayerSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_ROOM_ADD,                      "SERVER_ROOM_ADD",                         &PlayerSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_ROOM_HEIGHT_MAP,               "SERVER_ROOM_HEIGHT_MAP",                  &PlayerSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_FUSE_RIGHTS,                   "SERVER_FUSE_RIGHTS",                      &PlayerSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_AVAILABLE_BADGES,              "SERVER_AVAILABLE_BADGES",                 &PlayerSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_ACCOUNT_PREFERENCES,           "SERVER_ACCOUNT_PREFERENCES",              &PlayerSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_FRIEND_REQUEST,                "SERVER_FRIEND_REQUEST",                   &PlayerSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_ROOM_USER_OBJECTS,             "SERVER_ROOM_USER_OBJECTS",                &PlayerSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_MESSENGER_UPDATE,              "SERVER_MESSENGER_UPDATE",                 &PlayerSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_OBJECTS_WORLD,                 "SERVER_OBJECTS_WORLD",                    &PlayerSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_ACTIVE_OBJECTS,                "SERVER_ACTIVE_OBJECTS",                   &PlayerSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_ALIAS_TOGGLE,                  "SERVER_ALIAS_TOGGLE",                     &PlayerSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_USER_RIGHTS,                   "SERVER_USER_RIGHTS",                      &PlayerSocket::HandleServerMessage              );

        LOG_INFO << "Loaded " << mClientOpcode.size() << " CMSG OPCodes";
        LOG_INFO << "Loaded " << mServerOpcode.size() << " SMSG OPCodes";
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