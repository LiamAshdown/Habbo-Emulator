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
#include "../HabboSocket.h"
//-----------------------------------------------//
namespace SteerStone
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
        &HabboSocket::HandleNULL
    };
    //-----------------------------------------------//
    void Opcodes::InitializePackets()
    {
        ///< CMSG
        StoreClientPacket(PacketClientHeader::CLIENT_INITIALIZE_CRYPTO,             "CLIENT_INITIALIZE_CRYPTO",                &HabboSocket::HandleInitializeCrypto           );
        StoreClientPacket(PacketClientHeader::CLIENT_GENERATE_KEY,                  "CLIENT_GENERATE_KEY",                     &HabboSocket::HandleGenerateKey                );
        StoreClientPacket(PacketClientHeader::CLIENT_GDATE,                         "CLIENT_GDATE",                            &HabboSocket::HandleGDate                      );
        StoreClientPacket(PacketClientHeader::CLIENT_APPROVE_NAME,                  "CLIENT_APPROVE_NAME",                     &HabboSocket::HandleApproveUsername            );
        StoreClientPacket(PacketClientHeader::CLIENT_APPROVE_PASSWORD,              "CLIENT_APPROVE_PASSWORD",                 &HabboSocket::HandleApprovePassword            );
        StoreClientPacket(PacketClientHeader::CLIENT_APPROVE_EMAIL,                 "CLIENT_APPROVE_EMAIL",                    &HabboSocket::HandleApproveEmail               );
        StoreClientPacket(PacketClientHeader::CLIENT_REGISTER,                      "CLIENT_REGISTER",                         &HabboSocket::HandleRegisteration              );
        StoreClientPacket(PacketClientHeader::CLIENT_TRY_LOGIN,                     "CLIENT_TRY_LOGIN",                        &HabboSocket::HandleTryLogin                   );
        StoreClientPacket(PacketClientHeader::CLIENT_GET_INFO,                      "CLIENT_GET_INFO",                         &HabboSocket::HandleGetInfo                    );
        StoreClientPacket(PacketClientHeader::CLIENT_GET_CREDITS,                   "CLIENT_GET_CREDITS",                      &HabboSocket::HandleCreditBalance              );
        StoreClientPacket(PacketClientHeader::CLIENT_FLAT_CATALOGUE,                "CLIENT_FLAT_CATALOGUE",                   &HabboSocket::HandleNULL                       );
        StoreClientPacket(PacketClientHeader::CLIENT_NO_DESPACE_USERS,              "CLIENT_NO_DESPACE_USERS",                 &HabboSocket::HandleNULL                       );
        StoreClientPacket(PacketClientHeader::CLIENT_GET_CLUB,                      "CLIENT_GET_CLUB",                         &HabboSocket::HandleGetClub                    );
        StoreClientPacket(PacketClientHeader::CLIENT_NAVIGATE,                      "CLIENT_NAVIGATE",                         &HabboSocket::HandleNavigate                   );
        StoreClientPacket(PacketClientHeader::CLIENT_GET_USER_FLAT_CATEGORYS,       "CLIENT_GET_USER_FLAT_CATEGORYS",          &HabboSocket::HandleGetUserFlatsCategories     );
        StoreClientPacket(PacketClientHeader::CLIENT_GET_FAVOURITE_ROOMS,           "CLIENT_GET_FAVOURITE_ROOMS",              &HabboSocket::HandleGetFavouriteRooms          );
        StoreClientPacket(PacketClientHeader::CLIENT_ADD_FAVOURITE_ROOM,            "CLIENT_ADD_FAVOURITE_ROOM",               &HabboSocket::HandleAddFavouriteRoom           );
        StoreClientPacket(PacketClientHeader::CLIENT_DELETE_FAVOURITE_ROOM,         "CLIENT_DELETE_FAVOURITE_ROOM",            &HabboSocket::HandleRemoveFavouriteRoom        );
        StoreClientPacket(PacketClientHeader::CLIENT_SEARCH_ROOMS,                  "CLIENT_SEARCH_ROOMS",                     &HabboSocket::HandleSearchRooms                );
        StoreClientPacket(PacketClientHeader::CLIENT_GET_INTEREST,                  "CLIENT_GET_INTEREST",                     &HabboSocket::HandleGetInterest                );
        StoreClientPacket(PacketClientHeader::CLIENT_MESSENGER_INIT,                "CLIENT_MESSENGER_INIT",                   &HabboSocket::HandleMessengerInitialize        );
        StoreClientPacket(PacketClientHeader::CLIENT_ROOM_DIRECTORY,                "CLIENT_ROOM_DIRECTORY",                   &HabboSocket::HandleRoomDirectory              );
        StoreClientPacket(PacketClientHeader::CLIENT_PONG,                          "CLIENT_PONG",                             &HabboSocket::HandlePong                       );
        StoreClientPacket(PacketClientHeader::CLIENT_STOP,                          "CLIENT_STOP",                             &HabboSocket::HandleNULL                       );
        StoreClientPacket(PacketClientHeader::CLIENT_GET_ROOM_ADD,                  "CLIENT_GET_ROOM_ADD",                     &HabboSocket::HandleGetRoomAdd                 );
        StoreClientPacket(PacketClientHeader::CLIENT_ROOM_HEIGHT_MAP,               "CLIENT_ROOM_HEIGHT_MAP",                  &HabboSocket::HandleGetRoomHeight              );
        StoreClientPacket(PacketClientHeader::CLIENT_GET_AVAILABLE_BADGES,          "CLIENT_GET_AVAILABLE_BADGES",             &HabboSocket::HandleGetAvailableBadges         );
        StoreClientPacket(PacketClientHeader::CLIENT_GET_ACCOUNT_PREFERENCES,       "CLIENT_GET_ACCOUNT_PREFERENCES",          &HabboSocket::HandleGetAccountPreferences      );
        StoreClientPacket(PacketClientHeader::CLIENT_ROOM_USERS,                    "CLIENT_ROOM_USERS",                       &HabboSocket::HandleRoomUsers                  );
        StoreClientPacket(PacketClientHeader::CLIENT_MESSENGER_UPDATE,              "CLIENT_MESSENGER_UPDATE",                 &HabboSocket::HandleMessengerUpdate            );
        StoreClientPacket(PacketClientHeader::CLIENT_MESSENGER_ACCEPT_REQUEST,      "CLIENT_MESSENGER_ACCEPT_REQUEST",         &HabboSocket::HandleMessengerAcceptRequest     );
        StoreClientPacket(PacketClientHeader::CLIENT_GAME_OBJECTS,                  "CLIENT_GAME_OBJECTS",                     &HabboSocket::HandleGameObjects                );
        StoreClientPacket(PacketClientHeader::CLIENT_FURNITURE_REVISIONS,           "CLIENT_FURNITURE_REVISIONS",              &HabboSocket::HandleFurnitureRevisions         );
        StoreClientPacket(PacketClientHeader::CLIENT_MESSENGER_FIND_USER,           "CLIENT_MESSENGER_FIND_USER",              &HabboSocket::HandleMessengerFindUser          );
        StoreClientPacket(PacketClientHeader::CLIENT_LEAVE_ROOM,                    "CLIENT_LEAVE_ROOM",                       &HabboSocket::HandleLeaveRoom                  );
        StoreClientPacket(PacketClientHeader::CLIENT_MESSENGER_SEND_REQUEST,        "CLIENT_MESSENGER_SEND_REQUEST",           &HabboSocket::HandleMessengerSendRequest       );
        StoreClientPacket(PacketClientHeader::CLIENT_MESSENGER_REMOVE_FRIEND,       "CLIENT_MESSENGER_REMOVE_FRIEND",          &HabboSocket::HandleMessengerRemoveFriend      );
        StoreClientPacket(PacketClientHeader::CLIENT_MESSENGER_REJECT_REQUEST,      "CLIENT_MESSENGER_REJECT_REQUEST",         &HabboSocket::HandleMessengerRejectRequest     );
        StoreClientPacket(PacketClientHeader::CLIENT_MESSENGER_SEND_MESSAGE,        "CLIENT_MESSENGER_SEND_MESSAGE",           &HabboSocket::HandleMessengerSendMessage       );
        StoreClientPacket(PacketClientHeader::CLIENT_MESSENGER_REPLY,               "CLIENT_MESSENGER_REPLY",                  &HabboSocket::HandleMessengerReply             );

        ///< SMSG
        StoreServerPacket(PacketServerHeader::SERVER_CRYPTO_PARAMETERS,             "SERVER_CRYPTO_PARAMETERS",                &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_SESSION_PARAMETERS,            "SERVER_SESSION_PARAMETERS",               &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_AVAILABLE_SETS,                "SERVER_AVAILABLE_SETS",                   &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_APPROVE_NAME_REPLY,            "SERVER_APPROVE_NAME_REPLY",               &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_APPROVE_PASSWORD_REPLY,        "SERVER_APPROVE_PASSWORD_REPLY",           &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_APPROVE_EMAIL_REPLY,           "SERVER_APPROVE_EMAIL_REPLY",              &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_LOCALISED_ERROR,               "SERVER_LOCALISED_ERROR",                  &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_LOGIN,                         "SERVER_LOGIN",                            &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_ALERT,                         "SERVER_ALERT",                            &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_NOTICE,                        "SERVER_NOTICE",                           &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_HABBO_OBJECT,                   "SERVER_HABBO_OBJECT",                    &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_CREDIT_BALANCE,                "SERVER_CREDIT_BALANCE",                   &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_USER_FLAT_CATEGORIES,          "SERVER_USER_FLAT_CATEGORIES",             &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_SEARCH_RESULTS,                "SERVER_SEARCH_RESULTS",                   &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_ROOM_INTEREST,                 "SERVER_ROOM_INTEREST",                    &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_OPEN_CONNECTION,               "SERVER_OPEN_CONNECTION",                  &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_ROOM_URL,                      "SERVER_OPEN_CONNECTION",                  &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_ROOM_CANT_CONNECT,                  "SERVER_ROOM_CANT_CONNECT",           &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_PING,                          "SERVER_PING",                             &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_ROOM_READY,                    "SERVER_ROOM_READY",                       &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_ROOM_ADD,                      "SERVER_ROOM_ADD",                         &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_ROOM_HEIGHT_MAP,               "SERVER_ROOM_HEIGHT_MAP",                  &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_FUSE_RIGHTS,                   "SERVER_FUSE_RIGHTS",                      &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_AVAILABLE_BADGES,              "SERVER_AVAILABLE_BADGES",                 &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_ACCOUNT_PREFERENCES,           "SERVER_ACCOUNT_PREFERENCES",              &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_MESSENGER_SEND_FRIEND_REQUEST, "SERVER_MESSENGER_SEND_FRIEND_REQUEST",    &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_ROOM_USER_OBJECTS,             "SERVER_ROOM_USER_OBJECTS",                &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_MESSENGER_UPDATE,              "SERVER_MESSENGER_UPDATE",                 &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_OBJECTS_WORLD,                 "SERVER_OBJECTS_WORLD",                    &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_ACTIVE_OBJECTS,                "SERVER_ACTIVE_OBJECTS",                   &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_ALIAS_TOGGLE,                  "SERVER_ALIAS_TOGGLE",                     &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_USER_RIGHTS,                   "SERVER_USER_RIGHTS",                      &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_HOTEL_LOGOUT,                  "SERVER_HOTEL_LOGOUT",                     &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_MESSENGER_FIND_USER_RESULT,    "SERVER_MESSENGER_FIND_USER_RESULT",       &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_GDATE,                         "SERVER_GDATE",                            &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_MESSENGER_ERROR,               "SERVER_MESSENGER_ERROR",                  &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_REQUEST_BUDDY_ERROR,           "SERVER_REQUEST_BUDDY_ERROR",              &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_MESSENGER_ADD_FRIEND,          "SERVER_MESSENGER_ADD_FRIEND",             &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_MESSENGER_REMOVE_FRIEND,       "SERVER_MESSENGER_REMOVE_FRIEND",          &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_MESSENGER_SEND_MESSAGE,        "SERVER_MESSENGER_SEND_MESSAGE",           &HabboSocket::HandleServerMessage              );

        LOG_INFO << "Loaded " << mClientOpcode.size() << " CMSG OPCodes";
        LOG_INFO << "Loaded " << mServerOpcode.size() << " SMSG OPCodes";
    } 
    //-----------------------------------------------//
    void Opcodes::StoreClientPacket(const uint64& opcode, char const * name, void(HabboSocket::* handler)(std::unique_ptr<ClientPacket> p_Packet))
    {
        OpcodeHandler& ref = mClientOpcode[opcode];
        ref.name = name;
        ref.handler = handler;
    }
    //-----------------------------------------------//
    void Opcodes::StoreServerPacket(const uint64& opcode, char const * name, void(HabboSocket::* handler)(std::unique_ptr<ClientPacket> p_Packet))
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