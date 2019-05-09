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
* along with this program.  If not, see <http://www.gnu.org/licenses/>.f
*/

#include "HabboSocket.h"

namespace SteerStone
{
    /// Singleton
    Opcodes* Opcodes::instance()
    {
        static Opcodes instance;
        return &instance;
    }
    
    OpcodeHandler const Opcodes::m_EmptyHandler =
    {
        "NULL",
        PacketStatus::STATUS_UNHANDLED,
        PacketProcess::PROCESS_NOW,
        &HabboSocket::HandleNULL
    };
    
    /// InitializePackets
    /// Load our packets into storages to be accessed later
    void Opcodes::InitializePackets()
    {

        ///////////////////////////////////////////
        //             LOGIN HANDLER
        ///////////////////////////////////////////
        StoreClientPacket(PacketClientHeader::CLIENT_INFO_RETRIEVE,                 "CLIENT_INFO_RETRIEVE",              PacketStatus::STATUS_HOTEL_VIEW,         PacketProcess::PROCESS_NOW,            &HabboSocket::HandleGetInfoRetrieve            );
        StoreClientPacket(PacketClientHeader::CLIENT_TRY_LOGIN,                     "CLIENT_TRY_LOGIN",                  PacketStatus::STATUS_AUTH,               PacketProcess::PROCESS_NOW,            &HabboSocket::HandleTryLogin                   );
        StoreClientPacket(PacketClientHeader::CLIENT_VERSION_CHECK,                 "CLIENT_VERSION_CHECK",              PacketStatus::STATUS_AUTH,               PacketProcess::PROCESS_NOW,            &HabboSocket::HandleNULL                       );
        StoreClientPacket(PacketClientHeader::CLIENT_UNIQUE_ID,                     "CLIENT_UNIQUE_ID",                  PacketStatus::STATUS_AUTH,               PacketProcess::PROCESS_NOW,            &HabboSocket::HandleNULL                       );
        StoreClientPacket(PacketClientHeader::CLIENT_GET_PASSWORD,                  "CLIENT_GET_PASSWORD",               PacketStatus::STATUS_HOTEL_VIEW,         PacketProcess::PROCESS_NOW,            &HabboSocket::HandleNULL                       );
        StoreClientPacket(PacketClientHeader::CLIENT_LANG_CHECK,                    "CLIENT_LANG_CHECK",                 PacketStatus::STATUS_HOTEL_VIEW,         PacketProcess::PROCESS_NOW,            &HabboSocket::HandleNULL                       );
        StoreClientPacket(PacketClientHeader::CLIENT_BTCKS,                         "CLIENT_BTCKS",                      PacketStatus::STATUS_HOTEL_VIEW,         PacketProcess::PROCESS_NOW,            &HabboSocket::HandleNULL                       );
        StoreClientPacket(PacketClientHeader::CLIENT_GET_AVAILABLE_BADGES,          "CLIENT_GET_AVAILABLE_BADGES",       PacketStatus::STATUS_ALL,                PacketProcess::PROCESS_NOW,            &HabboSocket::HandleGetAvailableBadges         );
        StoreClientPacket(PacketClientHeader::CLIENT_GET_SESSION_PARAMETERS,        "CLIENT_GET_SESSION_PARAMETERS",     PacketStatus::STATUS_AUTH,               PacketProcess::PROCESS_NOW,            &HabboSocket::HandleNULL                       );
        StoreClientPacket(PacketClientHeader::CLIENT_PONG,                          "CLIENT_PONG",                       PacketStatus::STATUS_ALL,                PacketProcess::PROCESS_NOW,            &HabboSocket::HandlePong                       );
        StoreClientPacket(PacketClientHeader::CLIENT_GENERATE_KEY,                  "CLIENT_GENERATE_KEY",               PacketStatus::STATUS_AUTH,               PacketProcess::PROCESS_NOW,            &HabboSocket::HandleGenerateKey                );
        StoreClientPacket(PacketClientHeader::CLIENT_SSO,                           "CLIENT_SSO",                        PacketStatus::STATUS_AUTH,               PacketProcess::PROCESS_NOW,            &HabboSocket::HandleNULL                       );
        StoreClientPacket(PacketClientHeader::CLIENT_INITIALIZE_CRYPTO,             "CLIENT_INITIALIZE_CRYPTO",          PacketStatus::STATUS_AUTH,               PacketProcess::PROCESS_NOW,            &HabboSocket::HandleInitializeCrypto           );
        StoreClientPacket(PacketClientHeader::CLIENT_SECRET_KEY,                    "CLIENT_SECRET_KEY",                 PacketStatus::STATUS_AUTH,               PacketProcess::PROCESS_NOW,            &HabboSocket::HandleNULL                       );
        StoreClientPacket(PacketClientHeader::CLIENT_GET_SOUND_SETTINGS,            "CLIENT_GET_SOUND_SETTINGS",         PacketStatus::STATUS_ALL,                PacketProcess::PROCESS_NOW,            &HabboSocket::HandleGetAccountPreferences      );
        StoreClientPacket(PacketClientHeader::CLIENT_SET_SOUND_SETTINGS,            "CLIENT_SET_SOUND_SETTINGS",         PacketStatus::STATUS_ALL,                PacketProcess::PROCESS_NOW,            &HabboSocket::HandleNULL                       );
        StoreClientPacket(PacketClientHeader::CLIENT_GET_AVAILABLE_SETS,            "CLIENT_GET_AVAILABLE_SETS",         PacketStatus::STATUS_AUTH,               PacketProcess::PROCESS_NOW,            &HabboSocket::HandleNULL                       );

        ///////////////////////////////////////////
        //             ROOM HANDLER
        ///////////////////////////////////////////
        StoreClientPacket(PacketClientHeader::CLIENT_ROOM_DIRECTORY,                "CLIENT_ROOM_DIRECTORY",             PacketStatus::STATUS_ALL,                PacketProcess::PROCESS_NOW,            &HabboSocket::HandleRoomDirectory              );
        StoreClientPacket(PacketClientHeader::CLIENT_ROOM_CHAT,                     "CLIENT_ROOM_CHAT",                  PacketStatus::STATUS_IN_ROOM,            PacketProcess::PROCESS_NOW,            &HabboSocket::HandleRoomChat                   );
        StoreClientPacket(PacketClientHeader::CLIENT_ROOM_SHOUT,                    "CLIENT_ROOM_SHOUT",                 PacketStatus::STATUS_IN_ROOM,            PacketProcess::PROCESS_NOW,            &HabboSocket::HandleRoomShout                  );
        StoreClientPacket(PacketClientHeader::CLIENT_ROOM_WHISPER,                  "CLIENT_ROOM_WHISPER",               PacketStatus::STATUS_IN_ROOM,            PacketProcess::PROCESS_NOW,            &HabboSocket::HandleRoomWhisper                );
        StoreClientPacket(PacketClientHeader::CLIENT_QUIT,                          "CLIENT_QUIT",                       PacketStatus::STATUS_IN_ROOM,            PacketProcess::PROCESS_NOW,            &HabboSocket::HandleLeaveRoom                  );
        StoreClientPacket(PacketClientHeader::CLIENT_GO_VIA_DOOR,                   "CLIENT_GO_VIA_DOOR",                PacketStatus::STATUS_IN_ROOM,            PacketProcess::PROCESS_NOW,            &HabboSocket::HandleNULL                       );
        StoreClientPacket(PacketClientHeader::CLIENT_TRY_FLAT,                      "CLIENT_TRY_FLAT",                   PacketStatus::STATUS_ALL,                PacketProcess::PROCESS_PLAYER_UPDATE,  &HabboSocket::HandleTryFlat                    );
        StoreClientPacket(PacketClientHeader::CLIENT_GO_TO_FLAT,                    "CLIENT_GO_TO_FLAT",                 PacketStatus::STATUS_ALL,                PacketProcess::PROCESS_PLAYER_UPDATE,  &HabboSocket::HandleGoToFlat                   );
        StoreClientPacket(PacketClientHeader::CLIENT_G_HEIGHT_MAP,                  "CLIENT_G_HEIGHT_MAP",               PacketStatus::STATUS_IN_ROOM,            PacketProcess::PROCESS_NOW,            &HabboSocket::HandleGHeightMap                 );
        StoreClientPacket(PacketClientHeader::CLIENT_G_USERS,                       "CLIENT_G_USERS",                    PacketStatus::STATUS_IN_ROOM,            PacketProcess::PROCESS_NOW,            &HabboSocket::HandleGUsers                     );
        StoreClientPacket(PacketClientHeader::CLIENT_G_OBJECTS,                     "CLIENT_G_OBJECTS",                  PacketStatus::STATUS_IN_ROOM,            PacketProcess::PROCESS_NOW,            &HabboSocket::HandleGObjects                   );
        StoreClientPacket(PacketClientHeader::CLIENT_G_STAT,                        "CLIENT_G_STAT",                     PacketStatus::STATUS_IN_ROOM,            PacketProcess::PROCESS_NOW,            &HabboSocket::HandleGStat                      );
        StoreClientPacket(PacketClientHeader::CLIENT_G_ITEMS,                       "CLIENT_G_ITEMS",                    PacketStatus::STATUS_IN_ROOM,            PacketProcess::PROCESS_NOW,            &HabboSocket::HandleGItems                     );
        StoreClientPacket(PacketClientHeader::CLIENT_GET_STRIP,                     "CLIENT_GET_STRIP",                  PacketStatus::STATUS_IN_ROOM,            PacketProcess::PROCESS_NOW,            &HabboSocket::HandleNULL                       );
        StoreClientPacket(PacketClientHeader::CLIENT_FLAT_PROP_BY_ITEM,             "CLIENT_FLAT_PROP_BY_ITEM",          PacketStatus::STATUS_IN_ROOM,            PacketProcess::PROCESS_NOW,            &HabboSocket::HandleNULL                       );
        StoreClientPacket(PacketClientHeader::CLIENT_ADD_STRIP_ITEM,                "CLIENT_ADD_STRIP_ITEM",             PacketStatus::STATUS_IN_ROOM,            PacketProcess::PROCESS_NOW,            &HabboSocket::HandleNULL                       );
        StoreClientPacket(PacketClientHeader::CLIENT_TRADE_UNACCEPT,                "CLIENT_TRADE_UNACCEPT",             PacketStatus::STATUS_IN_ROOM,            PacketProcess::PROCESS_NOW,            &HabboSocket::HandleNULL                       );
        StoreClientPacket(PacketClientHeader::CLIENT_TRADE_ACCEPT,                  "CLIENT_TRADE_ACCEPT",               PacketStatus::STATUS_IN_ROOM,            PacketProcess::PROCESS_NOW,            &HabboSocket::HandleNULL                       );
        StoreClientPacket(PacketClientHeader::CLIENT_TRADE_CLOSE,                   "CLIENT_TRADE_CLOSE",                PacketStatus::STATUS_IN_ROOM,            PacketProcess::PROCESS_NOW,            &HabboSocket::HandleNULL                       );
        StoreClientPacket(PacketClientHeader::CLIENT_TRADE_OPEN,                    "CLIENT_TRADE_OPEN",                 PacketStatus::STATUS_IN_ROOM,            PacketProcess::PROCESS_NOW,            &HabboSocket::HandleNULL                       );
        StoreClientPacket(PacketClientHeader::CLIENT_TRADE_ADD_ITEM,                "CLIENT_TRADE_ADD_ITEM",             PacketStatus::STATUS_IN_ROOM,            PacketProcess::PROCESS_NOW,            &HabboSocket::HandleNULL                       );
        StoreClientPacket(PacketClientHeader::CLIENT_MOVE_STUFF,                    "CLIENT_MOVE_STUFF",                 PacketStatus::STATUS_IN_ROOM,            PacketProcess::PROCESS_NOW,            &HabboSocket::HandleNULL                       );
        StoreClientPacket(PacketClientHeader::CLIENT_SET_STUFF_DATA,                "CLIENT_SET_STUFF_DATA",             PacketStatus::STATUS_IN_ROOM,            PacketProcess::PROCESS_NOW,            &HabboSocket::HandleNULL                       );
        StoreClientPacket(PacketClientHeader::CLIENT_MOVE,                          "CLIENT_MOVE",                       PacketStatus::STATUS_IN_ROOM,            PacketProcess::PROCESS_NOW,            &HabboSocket::HandleMove                       );
        StoreClientPacket(PacketClientHeader::CLIENT_SET_STUFF_DATA,                "CLIENT_THROW_DICE",                 PacketStatus::STATUS_IN_ROOM,            PacketProcess::PROCESS_NOW,            &HabboSocket::HandleNULL                       );
        StoreClientPacket(PacketClientHeader::CLIENT_DICE_OFF,                      "CLIENT_DICE_OFF",                   PacketStatus::STATUS_IN_ROOM,            PacketProcess::PROCESS_NOW,            &HabboSocket::HandleNULL                       );
        StoreClientPacket(PacketClientHeader::CLIENT_PRESENT_OPEN,                  "CLIENT_PRESENT_OPEN",               PacketStatus::STATUS_IN_ROOM,            PacketProcess::PROCESS_NOW,            &HabboSocket::HandleNULL                       );
        StoreClientPacket(PacketClientHeader::CLIENT_LOOK_TO,                       "CLIENT_LOOK_TO",                    PacketStatus::STATUS_IN_ROOM,            PacketProcess::PROCESS_NOW,            &HabboSocket::HandleLookTo                     );
        StoreClientPacket(PacketClientHeader::CLIENT_CARRY_DRINK,                   "CLIENT_CARRY_DRINK",                PacketStatus::STATUS_IN_ROOM,            PacketProcess::PROCESS_NOW,            &HabboSocket::HandleNULL                       );
        StoreClientPacket(PacketClientHeader::CLIENT_IN_TO_DOOR,                    "CLIENT_IN_TO_DOOR",                 PacketStatus::STATUS_IN_ROOM,            PacketProcess::PROCESS_NOW,            &HabboSocket::HandleNULL                       );
        StoreClientPacket(PacketClientHeader::CLIENT_DOOR_GO_IN,                    "CLIENT_DOOR_GO_IN",                 PacketStatus::STATUS_IN_ROOM,            PacketProcess::PROCESS_NOW,            &HabboSocket::HandleNULL                       );
        StoreClientPacket(PacketClientHeader::CLIENT_G_IDATA,                       "CLIENT_G_IDATA",                    PacketStatus::STATUS_IN_ROOM,            PacketProcess::PROCESS_NOW,            &HabboSocket::HandleNULL                       );
        StoreClientPacket(PacketClientHeader::CLIENT_SET_ITEM_DATA,                 "CLIENT_SET_ITEM_DATA",              PacketStatus::STATUS_IN_ROOM,            PacketProcess::PROCESS_NOW,            &HabboSocket::HandleNULL                       );
        StoreClientPacket(PacketClientHeader::CLIENT_REMOVE_ITEM,                   "CLIENT_REMOVE_ITEM",                PacketStatus::STATUS_IN_ROOM,            PacketProcess::PROCESS_NOW,            &HabboSocket::HandleNULL                       );
        StoreClientPacket(PacketClientHeader::CLIENT_CARRY_ITEM,                    "CLIENT_CARRY_ITEM",                 PacketStatus::STATUS_IN_ROOM,            PacketProcess::PROCESS_NOW,            &HabboSocket::HandleNULL                       );
        StoreClientPacket(PacketClientHeader::CLIENT_STOP,                          "CLIENT_STOP",                       PacketStatus::STATUS_IN_ROOM,            PacketProcess::PROCESS_NOW,            &HabboSocket::HandleStop                       );
        StoreClientPacket(PacketClientHeader::CLIENT_USE_ITEM,                      "CLIENT_USE_ITEM",                   PacketStatus::STATUS_IN_ROOM,            PacketProcess::PROCESS_NOW,            &HabboSocket::HandleNULL                       );
        StoreClientPacket(PacketClientHeader::CLIENT_PLACE_STUFF,                   "CLIENT_PLACE_STUFF",                PacketStatus::STATUS_IN_ROOM,            PacketProcess::PROCESS_NOW,            &HabboSocket::HandleNULL                       );
        StoreClientPacket(PacketClientHeader::CLIENT_DANCE,                         "CLIENT_DANCE",                      PacketStatus::STATUS_IN_ROOM,            PacketProcess::PROCESS_NOW,            &HabboSocket::HandleDance                      );
        StoreClientPacket(PacketClientHeader::CLIENT_WAVE,                          "CLIENT_WAVE",                       PacketStatus::STATUS_IN_ROOM,            PacketProcess::PROCESS_NOW,            &HabboSocket::HandleWave                       );
        StoreClientPacket(PacketClientHeader::CLIENT_KICK_USER,                     "CLIENT_KICK_USER",                  PacketStatus::STATUS_IN_ROOM,            PacketProcess::PROCESS_ROOM_UPDATE,    &HabboSocket::HandleKickUser                   );
        StoreClientPacket(PacketClientHeader::CLIENT_ASSIGN_RIGHTS,                 "CLIENT_ASSIGN_RIGHTS",              PacketStatus::STATUS_IN_ROOM,            PacketProcess::PROCESS_ROOM_UPDATE,    &HabboSocket::HandleAssignRights               );
        StoreClientPacket(PacketClientHeader::CLIENT_REMOVE_RIGHTS,                 "CLIENT_REMOVE_RIGHTS",              PacketStatus::STATUS_IN_ROOM,            PacketProcess::PROCESS_ROOM_UPDATE,    &HabboSocket::HandleRemoveRights               );
        StoreClientPacket(PacketClientHeader::CLIENT_LET_USER_IN,                   "CLIENT_LET_USER_IN",                PacketStatus::STATUS_IN_ROOM,            PacketProcess::PROCESS_NOW,            &HabboSocket::HandleLetInUser                  );
        StoreClientPacket(PacketClientHeader::CLIENT_REMOVE_STUFF,                  "CLIENT_REMOVE_STUFF",               PacketStatus::STATUS_IN_ROOM,            PacketProcess::PROCESS_NOW,            &HabboSocket::HandleNULL                       );
        StoreClientPacket(PacketClientHeader::CLIENT_GO_AWAY,                       "CLIENT_GO_AWAY",                    PacketStatus::STATUS_IN_ROOM,            PacketProcess::PROCESS_NOW,            &HabboSocket::HandleNULL                       );
        StoreClientPacket(PacketClientHeader::CLIENT_GET_ROOM_ADD,                  "CLIENT_GET_ROOM_ADD",               PacketStatus::STATUS_IN_ROOM,            PacketProcess::PROCESS_NOW,            &HabboSocket::HandleGetRoomAdd                 );
        StoreClientPacket(PacketClientHeader::CLIENT_GET_PET_STAT,                  "CLIENT_GET_PET_STAT",               PacketStatus::STATUS_IN_ROOM,            PacketProcess::PROCESS_NOW,            &HabboSocket::HandleNULL                       );
        StoreClientPacket(PacketClientHeader::CLIENT_SET_BADGE,                     "CLIENT_SET_BADGE",                  PacketStatus::STATUS_IN_ROOM,            PacketProcess::PROCESS_NOW,            &HabboSocket::HandleSetBadge                   );
        StoreClientPacket(PacketClientHeader::CLIENT_GET_INTEREST,                  "CLIENT_GET_INTEREST",               PacketStatus::STATUS_ALL,                PacketProcess::PROCESS_NOW,            &HabboSocket::HandleGetInterest                );
        StoreClientPacket(PacketClientHeader::CLIENT_CONVERT_FURNI_TO_CREDITS,      "CLIENT_CONVERT_FURNI_TO_CREDITS",   PacketStatus::STATUS_IN_ROOM,            PacketProcess::PROCESS_NOW,            &HabboSocket::HandleNULL                       );
        StoreClientPacket(PacketClientHeader::CLIENT_ROOM_QUEUE,                    "CLIENT_ROOM_QUEUE",                 PacketStatus::STATUS_IN_ROOM,            PacketProcess::PROCESS_NOW,            &HabboSocket::HandleNULL                       );
        StoreClientPacket(PacketClientHeader::CLIENT_SET_ITEM_STATE,                "CLIENT_SET_ITEM_STATE",             PacketStatus::STATUS_IN_ROOM,            PacketProcess::PROCESS_NOW,            &HabboSocket::HandleNULL                       );
        StoreClientPacket(PacketClientHeader::CLIENT_SET_SPECTATOR_AMOUNT,          "CLIENT_SET_SPECTATOR_AMOUNT",       PacketStatus::STATUS_IN_ROOM,            PacketProcess::PROCESS_NOW,            &HabboSocket::HandleNULL                       );
        StoreClientPacket(PacketClientHeader::CLIENT_GET_GROUP_BADGES,              "CLIENT_GET_GROUP_BADGES",           PacketStatus::STATUS_IN_ROOM,            PacketProcess::PROCESS_NOW,            &HabboSocket::HandleNULL                       );
        StoreClientPacket(PacketClientHeader::CLIENT_GROUP_DETAILS,                 "CLIENT_GROUP_DETAILS",              PacketStatus::STATUS_IN_ROOM,            PacketProcess::PROCESS_NOW,            &HabboSocket::HandleNULL                       );
        StoreClientPacket(PacketClientHeader::CLIENT_ROOM_VOTE,                     "CLIENT_ROOM_VOTE",                  PacketStatus::STATUS_IN_ROOM,            PacketProcess::PROCESS_ROOM_UPDATE,    &HabboSocket::HandleRoomVote                   );

        ///////////////////////////////////////////
        //             NAVIGATOR HANDLER
        ///////////////////////////////////////////
        StoreClientPacket(PacketClientHeader::CLIENT_S_BUSY_F,                      "CLIENT_S_BUSY_F",                   PacketStatus::STATUS_ALL,                PacketProcess::PROCESS_NOW,            &HabboSocket::HandleNULL                       );
        StoreClientPacket(PacketClientHeader::CLIENT_SUSERF,                        "CLIENT_SUSERF",                     PacketStatus::STATUS_ALL,                PacketProcess::PROCESS_NOW,            &HabboSocket::HandleSearchUserFlats            );
        StoreClientPacket(PacketClientHeader::CLIENT_SEARCH_FLATS,                  "CLIENT_SEARCH_FLATS",               PacketStatus::STATUS_ALL,                PacketProcess::PROCESS_NOW,            &HabboSocket::HandleSearchFlats                );
        StoreClientPacket(PacketClientHeader::CLIENT_GET_FAVOURITE_ROOMS,           "CLIENT_GET_FAVOURITE_ROOMS",        PacketStatus::STATUS_ALL,                PacketProcess::PROCESS_NOW,            &HabboSocket::HandleGetFavouriteRooms          );
        StoreClientPacket(PacketClientHeader::CLIENT_ADD_FAVOURITE_ROOM,            "CLIENT_ADD_FAVOURITE_ROOM",         PacketStatus::STATUS_ALL,                PacketProcess::PROCESS_NOW,            &HabboSocket::HandleAddFavouriteRoom           );
        StoreClientPacket(PacketClientHeader::CLIENT_DELETE_FAVOURITE_ROOM,         "CLIENT_DELETE_FAVOURITE_ROOM",      PacketStatus::STATUS_ALL,                PacketProcess::PROCESS_NOW,            &HabboSocket::HandleRemoveFavouriteRoom        );
        StoreClientPacket(PacketClientHeader::CLIENT_GET_FLAT_INFO,                 "CLIENT_GET_FLAT_INFO",              PacketStatus::STATUS_ALL,                PacketProcess::PROCESS_NOW,            &HabboSocket::HandleGetFlatInfo                );
        StoreClientPacket(PacketClientHeader::CLIENT_DELETE_FLAT,                   "CLIENT_DELETE_FLAT",                PacketStatus::STATUS_ALL,                PacketProcess::PROCESS_NOW,            &HabboSocket::HandleDeleteFlat                 );
        StoreClientPacket(PacketClientHeader::CLIENT_UPDATE_FLAT,                   "CLIENT_UPDATE_FLAT",                PacketStatus::STATUS_ALL,                PacketProcess::PROCESS_NOW,            &HabboSocket::HandleUpdateFlat                 );
        StoreClientPacket(PacketClientHeader::CLIENT_SET_FLAT_INFO,                 "CLIENT_SET_FLAT_INFO",              PacketStatus::STATUS_ALL,                PacketProcess::PROCESS_NOW,            &HabboSocket::HandleSetFlatInfo                );
        StoreClientPacket(PacketClientHeader::CLIENT_CREATE_FLAT,                   "CLIENT_CREATE_FLAT",                PacketStatus::STATUS_ALL,                PacketProcess::PROCESS_NOW,            &HabboSocket::HandleCreateFlat                 );
        StoreClientPacket(PacketClientHeader::CLIENT_NAVIGATE,                      "CLIENT_NAVIGATE",                   PacketStatus::STATUS_ALL,                PacketProcess::PROCESS_NOW,            &HabboSocket::HandleNavigate                   );
        StoreClientPacket(PacketClientHeader::CLIENT_GET_USER_FLAT_CATEGORIES,      "CLIENT_GET_USER_FLAT_CATEGORIES",   PacketStatus::STATUS_ALL,                PacketProcess::PROCESS_NOW,            &HabboSocket::HandleGetUserFlatsCategories     );
        StoreClientPacket(PacketClientHeader::CLIENT_GET_FLAT_CATEGORY,             "CLIENT_GET_FLAT_CATEGORY",          PacketStatus::STATUS_ALL,                PacketProcess::PROCESS_NOW,            &HabboSocket::HandleGetFlatCategory            );
        StoreClientPacket(PacketClientHeader::CLIENT_SET_FLAT_CATEGORY,             "CLIENT_SET_FLAT_CATEGORY",          PacketStatus::STATUS_ALL,                PacketProcess::PROCESS_NOW,            &HabboSocket::HandleSetFlatCategory            );
        StoreClientPacket(PacketClientHeader::CLIENT_GET_SPACE_NODE_USERS,          "CLIENT_GET_SPACE_NODE_USERS",       PacketStatus::STATUS_ALL,                PacketProcess::PROCESS_NOW,            &HabboSocket::HandleNodeSpaceUsers             );
        StoreClientPacket(PacketClientHeader::CLIENT_REMOVE_ALL_RIGHTS,             "CLIENT_REMOVE_ALL_RIGHTS",          PacketStatus::STATUS_ALL,                PacketProcess::PROCESS_ROOM_UPDATE,    &HabboSocket::HandleRemoveAllRights            );
        StoreClientPacket(PacketClientHeader::CLIENT_GET_PARENT_CHAIN,              "CLIENT_GET_PARENT_CHAIN",           PacketStatus::STATUS_ALL,                PacketProcess::PROCESS_NOW,            &HabboSocket::HandleNULL                       );

        ///////////////////////////////////////////
        //             HOBBA HANDLER
        ///////////////////////////////////////////
        StoreClientPacket(PacketClientHeader::CLIENT_PICK_CRY_FOR_HELP,              "CLIENT_PICK_CRY_FOR_HELP",         PacketStatus::STATUS_ALL,                PacketProcess::PROCESS_NOW,             &HabboSocket::HandleNULL                       );
        StoreClientPacket(PacketClientHeader::CLIENT_CRY_FOR_HELP,                   "CLIENT_CRY_FOR_HELP",              PacketStatus::STATUS_ALL,                PacketProcess::PROCESS_NOW,             &HabboSocket::HandleNULL                       );
        StoreClientPacket(PacketClientHeader::CLIENT_CHANGE_CALL_BACK_CATEGORY,      "CLIENT_CHANGE_CALL_BACK_CATEGORY", PacketStatus::STATUS_ALL,                PacketProcess::PROCESS_NOW,             &HabboSocket::HandleNULL                       );
        StoreClientPacket(PacketClientHeader::CLIENT_MESSAGE_TO_CALLER,              "CLIENT_MESSAGE_TO_CALLER",         PacketStatus::STATUS_ALL,                PacketProcess::PROCESS_NOW,             &HabboSocket::HandleNULL                       );
        StoreClientPacket(PacketClientHeader::CLIENT_MODERATION_ACTION,              "CLIENT_MODERATION_ACTION",         PacketStatus::STATUS_ALL,                PacketProcess::PROCESS_NOW,             &HabboSocket::HandleNULL                       );

        ///////////////////////////////////////////
        //         SOUND MACHINE HANDLER
        ///////////////////////////////////////////
        StoreClientPacket(PacketClientHeader::CLIENT_GET_SOUND_MACHINE_CONFIG,       "CLIENT_GET_SOUND_MACHINE_CONFIG", PacketStatus::STATUS_IN_ROOM,             PacketProcess::PROCESS_NOW,              &HabboSocket::HandleNULL                       );
        StoreClientPacket(PacketClientHeader::CLIENT_SAVE_SOUND_MACHINE_CONFIG,      "CLIENT_SAVE_SOUND_MACHINE_CONFIG",PacketStatus::STATUS_IN_ROOM,             PacketProcess::PROCESS_NOW,              &HabboSocket::HandleNULL                       );
        StoreClientPacket(PacketClientHeader::CLIENT_INSERT_SOUND_PACKAGE,           "CLIENT_INSERT_SOUND_PACKAGE",    PacketStatus::STATUS_IN_ROOM,              PacketProcess::PROCESS_NOW,              &HabboSocket::HandleNULL                       );
        StoreClientPacket(PacketClientHeader::CLIENT_EJECT_SOUND_PACKAGE,            "CLIENT_EJECT_SOUND_PACKAGE",     PacketStatus::STATUS_IN_ROOM,              PacketProcess::PROCESS_NOW,              &HabboSocket::HandleNULL                       );
        StoreClientPacket(PacketClientHeader::CLIENT_GET_SOUND_DATA,                 "CLIENT_GET_SOUND_DATA",          PacketStatus::STATUS_IN_ROOM,              PacketProcess::PROCESS_NOW,              &HabboSocket::HandleNULL                       );

        ///////////////////////////////////////////
        //         MOUNTAIN HANDLER
        ///////////////////////////////////////////
        StoreClientPacket(PacketClientHeader::CLIENT_CLOSE_UK_MAKE_OPPI,             "CLIENT_CLOSE_UK_MAKE_OPPI",      PacketStatus::STATUS_IN_ROOM,              PacketProcess::PROCESS_NOW,              &HabboSocket::HandleNULL                       );
        StoreClientPacket(PacketClientHeader::CLIENT_CHANGE_SHIRT,                   "CLIENT_CHANGE_SHIRT",            PacketStatus::STATUS_IN_ROOM,              PacketProcess::PROCESS_NOW,              &HabboSocket::HandleNULL                       );
        StoreClientPacket(PacketClientHeader::CLIENT_REFRESH_FIGURE,                 "CLIENT_REFRESH_FIGURE",          PacketStatus::STATUS_IN_ROOM,              PacketProcess::PROCESS_NOW,              &HabboSocket::HandleNULL                       );
        StoreClientPacket(PacketClientHeader::CLIENT_SWIM_SUIT,                      "CLIENT_SWIM_SUIT",               PacketStatus::STATUS_IN_ROOM,              PacketProcess::PROCESS_NOW,              &HabboSocket::HandleSwimSuit                   );

        ///////////////////////////////////////////
        //         PELLEHYPPY HANDLER
        ///////////////////////////////////////////
        StoreClientPacket(PacketClientHeader::CLIENT_JUMP_START,                      "CLIENT_JUMP_START",             PacketStatus::STATUS_IN_ROOM,              PacketProcess::PROCESS_NOW,              &HabboSocket::HandleNULL                       );
        StoreClientPacket(PacketClientHeader::CLIENT_SIGN,                            "CLIENT_SIGN",                   PacketStatus::STATUS_IN_ROOM,              PacketProcess::PROCESS_NOW,              &HabboSocket::HandleNULL                       );
        StoreClientPacket(PacketClientHeader::CLIENT_JUMP_PERF,                       "CLIENT_JUMP_PERF",              PacketStatus::STATUS_IN_ROOM,              PacketProcess::PROCESS_NOW,              &HabboSocket::HandleNULL                       );
        StoreClientPacket(PacketClientHeader::CLIENT_SPLASH_POSITION,                 "CLIENT_SPLASH_POSITION",        PacketStatus::STATUS_IN_ROOM,              PacketProcess::PROCESS_NOW,              &HabboSocket::HandleNULL                       );

        ///////////////////////////////////////////
        //           HUBU HANDLER
        ///////////////////////////////////////////
        StoreClientPacket(PacketClientHeader::CLIENT_VOTE,                          "CLIENT_VOTE",                      PacketStatus::STATUS_IN_ROOM,             PacketProcess::PROCESS_NOW,             &HabboSocket::HandleNULL                       );
        StoreClientPacket(PacketClientHeader::CLIENT_TRY_INFO_BUS,                  "CLIENT_TRY_INFO_BUS",              PacketStatus::STATUS_IN_ROOM,             PacketProcess::PROCESS_NOW,             &HabboSocket::HandleInfoBus                    );
        StoreClientPacket(PacketClientHeader::CLIENT_CHANGE_WORLD,                  "CLIENT_CHANGE_WORLD",              PacketStatus::STATUS_IN_ROOM,             PacketProcess::PROCESS_NOW,             &HabboSocket::HandleChangeWorld                );

        ///////////////////////////////////////////
        //        REGISTRATION HANDLER
        ///////////////////////////////////////////
        StoreClientPacket(PacketClientHeader::CLIENT_GDATE,                          "CLIENT_GDATE",                    PacketStatus::STATUS_HOTEL_VIEW,          PacketProcess::PROCESS_NOW,             &HabboSocket::HandleGDate                      );
        StoreClientPacket(PacketClientHeader::CLIENT_APPROVE_NAME,                   "CLIENT_APPROVE_NAME",             PacketStatus::STATUS_AUTH,                PacketProcess::PROCESS_NOW,             &HabboSocket::HandleApproveUsername            );
        StoreClientPacket(PacketClientHeader::CLIENT_REGISTER,                       "CLIENT_REGISTER",                 PacketStatus::STATUS_AUTH,                PacketProcess::PROCESS_NOW,             &HabboSocket::HandleRegisteration              );
        StoreClientPacket(PacketClientHeader::CLIENT_UPDATE,                         "CLIENT_UPDATE",                   PacketStatus::STATUS_AUTH,                PacketProcess::PROCESS_NOW,             &HabboSocket::HandleNULL                       );
        StoreClientPacket(PacketClientHeader::CLIENT_AC,                             "CLIENT_AC",                       PacketStatus::STATUS_AUTH,                PacketProcess::PROCESS_NOW,             &HabboSocket::HandleNULL                       );
        StoreClientPacket(PacketClientHeader::CLIENT_COPPA_REG_CHECK_TIME,           "CLIENT_COPPA_REG_CHECK_TIME",     PacketStatus::STATUS_AUTH,                PacketProcess::PROCESS_NOW,             &HabboSocket::HandleNULL                       );
        StoreClientPacket(PacketClientHeader::CLIENT_COPPA_REG_GET_REAL_TIME,        "CLIENT_COPPA_REG_GET_REAL_TIME",  PacketStatus::STATUS_AUTH,                PacketProcess::PROCESS_NOW,             &HabboSocket::HandleNULL                       );
        StoreClientPacket(PacketClientHeader::CLIENT_PARENT_EMAIL_REQUIRED,          "CLIENT_PARENT_EMAIL_REQUIRED",    PacketStatus::STATUS_AUTH,                PacketProcess::PROCESS_NOW,             &HabboSocket::HandleParentEmailRequired        );
        StoreClientPacket(PacketClientHeader::CLIENT_VALIDATE_PARENT_EMAIL,          "CLIENT_VALIDATE_PARENT_EMAIL",    PacketStatus::STATUS_AUTH,                PacketProcess::PROCESS_NOW,             &HabboSocket::HandleValidateParentEmail        );
        StoreClientPacket(PacketClientHeader::CLIENT_SEND_PARENT_EMAIL,              "CLIENT_SEND_PARENT_EMAIL",        PacketStatus::STATUS_AUTH,                PacketProcess::PROCESS_NOW,             &HabboSocket::HandleNULL                       );
        StoreClientPacket(PacketClientHeader::CLIENT_UPDATE_ACCOUNT,                 "CLIENT_UPDATE_ACCOUNT",           PacketStatus::STATUS_HOTEL_VIEW,          PacketProcess::PROCESS_NOW,             &HabboSocket::HandleNULL                       );
        StoreClientPacket(PacketClientHeader::CLIENT_APPROVE_PASSWORD,               "CLIENT_APPROVE_PASSWORD",         PacketStatus::STATUS_AUTH,                PacketProcess::PROCESS_NOW,             &HabboSocket::HandleApprovePassword            );
        StoreClientPacket(PacketClientHeader::CLIENT_APPROVE_EMAIL,                  "CLIENT_APPROVE_EMAIL",            PacketStatus::STATUS_AUTH,                PacketProcess::PROCESS_NOW,             &HabboSocket::HandleApproveEmail               );

        ///////////////////////////////////////////
        //           RECYCLER HANDLER
        ///////////////////////////////////////////
        StoreClientPacket(PacketClientHeader::CLIENT_GET_FURNI_RECYCLER_CONFIG,      "CLIENT_GET_FURNI_RECYCLER_CONFIG",PacketStatus::STATUS_HOTEL_VIEW,          PacketProcess::PROCESS_NOW,             &HabboSocket::HandleNULL                       );
        StoreClientPacket(PacketClientHeader::CLIENT_GET_FURNI_RECYCLER_CONFIG,      "CLIENT_GET_FURNI_RECYCLER_CONFIG",PacketStatus::STATUS_ALL,                 PacketProcess::PROCESS_NOW,             &HabboSocket::HandleNULL                       );
        StoreClientPacket(PacketClientHeader::CLIENT_APPROVE_RECYCLED_FURNI,         "CLIENT_APPROVE_RECYCLED_FURNI",   PacketStatus::STATUS_IN_ROOM,             PacketProcess::PROCESS_NOW,             &HabboSocket::HandleNULL                       );
        StoreClientPacket(PacketClientHeader::CLIENT_START_FURNI_RECYCLING,          "CLIENT_START_FURNI_RECYCLING",    PacketStatus::STATUS_IN_ROOM,             PacketProcess::PROCESS_NOW,             &HabboSocket::HandleNULL                       );
        StoreClientPacket(PacketClientHeader::CLIENT_CONFIRM_FURNI_RECYCLING,        "CLIENT_CONFIRM_FURNI_RECYCLING",  PacketStatus::STATUS_IN_ROOM,             PacketProcess::PROCESS_NOW,             &HabboSocket::HandleNULL                       );

         ///////////////////////////////////////////
        //           PURSE HANDLER
        ///////////////////////////////////////////
        StoreClientPacket(PacketClientHeader::CLIENT_GET_CREDITS,                    "CLIENT_GET_CREDITS",              PacketStatus::STATUS_ALL,                PacketProcess::PROCESS_NOW,              &HabboSocket::HandleCreditBalance              );
        StoreClientPacket(PacketClientHeader::CLIENT_GET_USER_CREDIT_LOG,            "CLIENT_GET_USER_CREDIT_LOG",      PacketStatus::STATUS_HOTEL_VIEW,          PacketProcess::PROCESS_NOW,             &HabboSocket::HandleNULL                       );
        StoreClientPacket(PacketClientHeader::CLIENT_REDEEM_VOUCHER,                 "CLIENT_REDEEM_VOUCHER",           PacketStatus::STATUS_HOTEL_VIEW,          PacketProcess::PROCESS_NOW,             &HabboSocket::HandleNULL                       );

        ///////////////////////////////////////////
        //           POLL HANDLER
        ///////////////////////////////////////////
        StoreClientPacket(PacketClientHeader::CLIENT_POLL_START,                     "CLIENT_POLL_START",               PacketStatus::STATUS_IN_ROOM,             PacketProcess::PROCESS_NOW,              &HabboSocket::HandleNULL                       );
        StoreClientPacket(PacketClientHeader::CLIENT_POLL_REJECT,                    "CLIENT_POLL_REJECT",              PacketStatus::STATUS_IN_ROOM,             PacketProcess::PROCESS_NOW,              &HabboSocket::HandleNULL                       );
        StoreClientPacket(PacketClientHeader::CLIENT_POLL_ANSWER,                    "CLIENT_POLL_ANSWER",              PacketStatus::STATUS_IN_ROOM,             PacketProcess::PROCESS_NOW,              &HabboSocket::HandleNULL                       );

        ///////////////////////////////////////////
        //          MESSENGER HANDLER
        ///////////////////////////////////////////
        StoreClientPacket(PacketClientHeader::CLIENT_FIND_USER,                      "CLIENT_FIND_USER",                PacketStatus::STATUS_ALL,                 PacketProcess::PROCESS_NOW,              &HabboSocket::HandleMessengerFindUser          );
        StoreClientPacket(PacketClientHeader::CLIENT_MESSENGER_INITIALIZE,           "CLIENT_MESSENGER_INITIALIZE",     PacketStatus::STATUS_ALL,                 PacketProcess::PROCESS_NOW,              &HabboSocket::HandleMessengerInitialize        );
        StoreClientPacket(PacketClientHeader::CLIENT_MESSENGER_UPDATE,               "CLIENT_MESSENGER_UPDATE",         PacketStatus::STATUS_ALL,                 PacketProcess::PROCESS_NOW,              &HabboSocket::HandleMessengerUpdate            );
        StoreClientPacket(PacketClientHeader::CLIENT_MESSENGER_C_CLICK,              "CLIENT_MESSENGER_C_CLICK",        PacketStatus::STATUS_ALL,                 PacketProcess::PROCESS_NOW,              &HabboSocket::HandleNULL                       );
        StoreClientPacket(PacketClientHeader::CLIENT_MESSENGER_C_READ,               "CLIENT_MESSENGER_C_READ",         PacketStatus::STATUS_ALL,                 PacketProcess::PROCESS_NOW,              &HabboSocket::HandleNULL                       );
        StoreClientPacket(PacketClientHeader::CLIENT_MESSENGER_MARK_READ,            "CLIENT_MESSENGER_MARK_READ",      PacketStatus::STATUS_ALL,                 PacketProcess::PROCESS_NOW,              &HabboSocket::HandleMessengerMarkRead          );
        StoreClientPacket(PacketClientHeader::CLIENT_MESSENGER_SEND_MESSAGE,         "CLIENT_MESSENGER_SEND_MESSAGE",   PacketStatus::STATUS_ALL,                 PacketProcess::PROCESS_NOW,              &HabboSocket::HandleMessengerSendMessage       );
        StoreClientPacket(PacketClientHeader::CLIENT_MESSENGER_CONSOLE_MOTTO,        "CLIENT_MESSENGER_CONSOLE_MOTTO",  PacketStatus::STATUS_ALL,                 PacketProcess::PROCESS_NOW,              &HabboSocket::HandleConsoleMotto               );
        StoreClientPacket(PacketClientHeader::CLIENT_MESSENGER_ACCEPT_BUDDY,         "CLIENT_MESSENGER_ACCEPT_BUDDY",   PacketStatus::STATUS_ALL,                 PacketProcess::PROCESS_NOW,              &HabboSocket::HandleMessengerAcceptBuddy       );
        StoreClientPacket(PacketClientHeader::CLIENT_MESSENGER_DECLINE_BUDDY,        "CLIENT_MESSENGER_DECLINE_BUDDY",  PacketStatus::STATUS_ALL,                 PacketProcess::PROCESS_NOW,              &HabboSocket::HandleMessengerRejectBuddy       );
        StoreClientPacket(PacketClientHeader::CLIENT_MESSENGER_REQUEST_BUDDY,        "CLIENT_MESSENGER_REQUEST_BUDDY",  PacketStatus::STATUS_ALL,                 PacketProcess::PROCESS_NOW,              &HabboSocket::HandleMessengerSendRequest       );
        StoreClientPacket(PacketClientHeader::CLIENT_MESSENGER_REMOVE_BUDDY,         "CLIENT_MESSENGER_REMOVE_BUDDY",   PacketStatus::STATUS_ALL,                 PacketProcess::PROCESS_NOW,              &HabboSocket::HandleMessengerRemoveBuddy       );
        StoreClientPacket(PacketClientHeader::CLIENT_MESSENGER_GET_MESSAGE,          "CLIENT_MESSENGER_GET_MESSAGE",    PacketStatus::STATUS_ALL,                 PacketProcess::PROCESS_NOW,              &HabboSocket::HandleNULL                       );
        StoreClientPacket(PacketClientHeader::CLIENT_MESSENGER_REPORT_MESSAGE,       "CLIENT_MESSENGER_REPORT_MESSAGE", PacketStatus::STATUS_ALL,                 PacketProcess::PROCESS_NOW,              &HabboSocket::HandleNULL                       );
        StoreClientPacket(PacketClientHeader::CLIENT_MESSENGER_BUDDY_REQUESTS,       "CLIENT_MESSENGER_BUDDY_REQUESTS", PacketStatus::STATUS_ALL,                 PacketProcess::PROCESS_NOW,              &HabboSocket::HandleNULL                       );

        ///////////////////////////////////////////
        //           CLUB HANDLER
        ///////////////////////////////////////////
        StoreClientPacket(PacketClientHeader::CLIENT_CLUB_BUY,                      "CLIENT_BUY_HABBO_CLUB",            PacketStatus::STATUS_HOTEL_VIEW,          PacketProcess::PROCESS_NOW,              &HabboSocket::HandleBuyHabboClub               );
        StoreClientPacket(PacketClientHeader::CLIENT_CLUB_GET_USER_INFO,            "CLIENT_CLUB_GET_USER_INFO",        PacketStatus::STATUS_HOTEL_VIEW,          PacketProcess::PROCESS_NOW,              &HabboSocket::HandleGetClub                    );
        StoreClientPacket(PacketClientHeader::CLIENT_GIFT_APPROVAL,                 "CLIENT_GIFT_APPROVAL",             PacketStatus::STATUS_HOTEL_VIEW,          PacketProcess::PROCESS_NOW,              &HabboSocket::HandleNULL                       );
        
        ///////////////////////////////////////////
        //           CATALOGUE HANDLER
        ///////////////////////////////////////////
        StoreClientPacket(PacketClientHeader::CLIENT_GPRC,                          "CLIENT_GPRC",                      PacketStatus::STATUS_IN_ROOM,             PacketProcess::PROCESS_NOW,              &HabboSocket::HandleNULL                       );
        StoreClientPacket(PacketClientHeader::CLIENT_GCIX,                          "CLIENT_GCIX",                      PacketStatus::STATUS_IN_ROOM,             PacketProcess::PROCESS_NOW,              &HabboSocket::HandleNULL                       );
        StoreClientPacket(PacketClientHeader::CLIENT_GCAP,                          "CLIENT_GCAP",                      PacketStatus::STATUS_IN_ROOM,             PacketProcess::PROCESS_NOW,              &HabboSocket::HandleNULL                       );
        

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        ///////////////////////////////////////////
        //             LOGIN HANDLER
        ///////////////////////////////////////////
        StoreServerPacket(PacketServerHeader::SERVER_CRYPTO_PARAMETERS,             "SERVER_CRYPTO_PARAMETERS",                &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_SESSION_PARAMETERS,            "SERVER_SESSION_PARAMETERS",               &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_LOGIN_OK,                      "SERVER_LOGIN_OK",                         &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_SYSTEM_BROADCAST,              "SERVER_SYSTEM_BROADCAST",                 &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_USER_OBJECT,                   "SERVER_USER_OBJECT",                      &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_FUSE_RIGHTS,                   "SERVER_FUSE_RIGHTS",                      &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_AVAILABLE_BADGES,              "SERVER_AVAILABLE_BADGES",                 &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_SOUND_SETTINGS,                "SERVER_SOUND_SETTINGS",                   &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_PING,                          "SERVER_PING",                             &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_HOTEL_LOGOUT,                  "SERVER_HOTEL_LOGOUT",                     &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_MOD_ALERT,                     "SERVER_NOTICE",                           &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_CHECK_SUM,                     "SERVER_CHECK_SUM",                        &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_ESP_NOTIFY,                    "SERVER_ESP_NOTIFY",                       &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_USER_BANNED,                   "SERVER_USER_BANNED",                      &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_ERROR,                         "SERVER_ERROR",                            &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_GDATE,                         "SERVER_GDATE",                            &HabboSocket::HandleServerMessage              );

        ///////////////////////////////////////////
        //             ROOM HANDLER
        ///////////////////////////////////////////
        StoreServerPacket(PacketServerHeader::SERVER_CLC,                           "SERVER_CLC",                              &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_CLC_OK,                        "SERVER_CLC_OK",                           &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_CHAT,                          "SERVER_CHAT",                             &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_WHISPER,                       "SERVER_WHISPER",                          &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_SHOUT,                         "SERVER_SHOUT",                            &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_USERS,                         "SERVER_USERS",                            &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_LOGOUT,                        "SERVER_LOGOUT",                           &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_OBJECTS,                       "SERVER_OBJECTS",                          &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_ROOM_HEIGHT_MAP,               "SERVER_ROOM_HEIGHT_MAP",                  &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_ACTIVE_OBJECTS,                "SERVER_ACTIVE_OBJECTS",                   &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_STATUS ,                       "SERVER_STATUS ",                          &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_FLAT_LET_IN,                   "SERVER_FLAT_LET_IN",                      &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_ITEMS,                         "SERVER_ITEMS",                            &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_YOU_ARE_CONTROLLER,            "SERVER_YOU_ARE_CONTROLLER",               &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_YOU_ARE_NOT_CONTROLLER,        "SERVER_YOU_ARE_NOT_CONTROLLER",           &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_FLAT_PROPERTY,                 "SERVER_FLAT_PROPERTY",                    &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_YOU_ARE_OWNER,                 "SERVER_YOU_ARE_OWNER",                    &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_IDATA,                         "SERVER_IDATA",                            &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_GO_TO_FLAT,                    "SERVER_GO_TO_FLAT",                       &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_TELEPORT_INITIALIZED,          "SERVER_TELEPORT_INITIALIZED",             &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_DOOR_DELETED,                  "SERVER_DOOR_DELETED",                     &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_DOOR_DELETED_1,                "SERVER_DOOR_DELETED_1",                   &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_ROOM_READY,                    "SERVER_ROOM_READY",                       &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_YOU_ARE_MOD,                   "SERVER_YOU_ARE_MOD",                      &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_SHOW_PROGRAM,                  "SERVER_SHOW_PROGRAM",                     &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_NO_USER_FOR_GIFT,              "SERVER_NO_USER_FOR_GIFT",                 &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_HANDLE_ITEMS,                  "SERVER_HANDLE_ITEMS",                     &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_REMOVE_ITEM,                   "SERVER_REMOVE_ITEM",                      &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_UPDATE_ITEM,                   "SERVER_UPDATE_ITEM",                      &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_STUFF_DATA_UPDATE,             "SERVER_STUFF_DATA_UPDATE",                &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_DOOR_OUT,                      "SERVER_DOOR_OUT",                         &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_DICE_VALUE,                    "SERVER_DICE_VALUE",                       &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_DOOR_BELL_RINGING,             "SERVER_DOOR_BELL_RINGING",                &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_DOOR_IN,                       "SERVER_DOOR_IN",                          &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_ACTIVE_OBJECT_ADD,             "SERVER_ACTIVE_OBJECT_ADD",                &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_ACTIVE_OBJECT_REMOVE,          "SERVER_ACTIVE_OBJECT_REMOVE",             &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_ACTIVE_OBJECT_UPDATE,          "SERVER_ACTIVE_OBJECT_UPDATE",             &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_STRIP_INFO,                    "SERVER_STRIP_INFO",                       &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_REMOVE_STRIP_ITEM,             "SERVER_REMOVE_STRIP_ITEM",                &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_STRIP_UPDATED,                 "SERVER_STRIP_UPDATED",                    &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_YOU_ARE_NOT_ALLOWED,           "SERVER_YOU_ARE_NOT_ALLOWED",              &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_TRADE_COMPLETED,               "SERVER_TRADE_COMPLETED",                  &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_TRADE_ITEMS,                   "SERVER_TRADE_ITEMS",                      &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_TRADE_ACCEPT,                  "SERVER_TRADE_ACCEPT",                     &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_TRADE_CLOSE,                   "SERVER_TRADE_CLOSE",                      &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_TRADE_COMPLETED_1,             "SERVER_TRADE_COMPLETED_1",                &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_PRESENT_OPEN,                  "SERVER_PRESENT_OPEN",                     &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_FLAT_NOT_ALLOWED_TO_ENTER,     "SERVER_FLAT_NOT_ALLOWED_TO_ENTER",        &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_INVENTORY,                     "SERVER_INVENTORY",                        &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_ROOM_URL,                      "SERVER_ROOM_URL",                         &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_ROOM_ADD,                      "SERVER_ROOM_ADD",                         &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_PET_STAT,                      "SERVER_PET_STAT",                         &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_HEIGHT_MAP_UPDATE,             "SERVER_HEIGHT_MAP_UPDATE",                &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_USE_BADGE,                     "SERVER_USE_BADGE",                        &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_SLIDE_OBJECT_BUNDLE,           "SERVER_SLIDE_OBJECT_BUNDLE",              &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_ROOM_INTEREST,                 "SERVER_ROOM_INTEREST",                    &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_ROOM_QUEUE_DATA,               "SERVER_ROOM_QUEUE_DATA",                  &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_YOU_ARE_SPECTATOR,             "SERVER_YOU_ARE_SPECTATOR",                &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_REMOVE_SPECTATOR,              "SERVER_REMOVE_SPECTATOR",                 &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_FIGURE_CHANGE,                 "SERVER_FIGURE_CHANGE",                    &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_SPECTATOR_AMOUNT,              "SERVER_SPECTATOR_AMOUNT",                 &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_GROUP_BADGES,                  "SERVER_GROUP_BADGES",                     &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_GROUP_MEMBER_SHIP_UPDATE,      "SERVER_GROUP_MEMBER_SHIP_UPDATE",         &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_GROUP_DETAILS,                 "SERVER_GROUP_DETAILS",                    &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_UPDATE_VOTES,                  "SERVER_UPDATE_VOTES",                     &HabboSocket::HandleServerMessage              );

        ///////////////////////////////////////////
        //             NAVIGATOR HANDLER
        ///////////////////////////////////////////
        StoreServerPacket(PacketServerHeader::SERVER_AVAILABLE_SETS,                "SERVER_AVAILABLE_SETS",                   &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_FLAT_INFO,                     "SERVER_FLAT_INFO",                        &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_FLAT_RESULTS_1,                "SERVER_FLAT_RESULTS_1",                   &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_NO_FLATS_FOR_USER,             "SERVER_NO_FLATS_FOR_USER",                &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_NO_FLATS,                      "SERVER_NO_FLATS",                         &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_FAVOURITE_ROOMS_RESULT,        "SERVER_FAVOURITE_ROOMS_RESULT",           &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_FLAT_PASSWORD_OK,              "SERVER_FLAT_PASSWORD_OK",                 &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_NAVIGATE_NODE_INFO,            "SERVER_NAVIGATE_NODE_INFO",               &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_USER_FLAT_CATEGORIES,          "SERVER_USER_FLAT_CATEGORIES",             &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_USER_FLAT_CATEGORY,            "SERVER_USER_FLAT_CATEGORY",               &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_SPACE_NODE_USERS,              "SERVER_SPACE_NODE_USERS",                 &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_CANT_CONNECT,                  "SERVER_CANT_CONNECT",                     &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_SUCCESS,                       "SERVER_SUCCESS",                          &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_PARENT_FAILURE,                "SERVER_PARENT_FAILURE",                   &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_ROOM_FORWARD,                  "SERVER_ROOM_FORWARD",                     &HabboSocket::HandleServerMessage              );

        ///////////////////////////////////////////
        //             HOBBA HANDLER
        ///////////////////////////////////////////
        StoreServerPacket(PacketServerHeader::SERVER_CRY_FOR_HELP,                  "SERVER_CRY_FOR_HELP",                     &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_PICKED_CRY,                    "SERVER_PICKED_CRY",                       &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_DELETE_CRY,                    "SERVER_DELETE_CRY",                       &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_CRY_REPLY,                     "SERVER_CRY_REPLY",                        &HabboSocket::HandleServerMessage              );

        ///////////////////////////////////////////
        //         SOUND MACHINE HANDLER
        ///////////////////////////////////////////
        StoreServerPacket(PacketServerHeader::SERVER_HANDLE_SOUND_DATA,             "SERVER_HANDLE_SOUND_DATA",                &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_MACHINE_SOUND_PACKAGES,        "SERVER_MACHINE_SOUND_PACKAGES",           &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_USER_SOUND_PACKAGES,           "SERVER_USER_SOUND_PACKAGES",              &HabboSocket::HandleServerMessage              );

        ///////////////////////////////////////////
        //         ERROR REPORT HANDLER
        ///////////////////////////////////////////
        StoreServerPacket(PacketServerHeader::SERVER_ERROR_REPORT,                  "SERVER_ERROR_REPORT",                     &HabboSocket::HandleServerMessage              );

        ///////////////////////////////////////////
        //         MOUNTAIN HANDLER
        ///////////////////////////////////////////
        StoreServerPacket(PacketServerHeader::SERVER_OPEN_UI_MAKE_OPPI,              "SERVER_OPEN_UI_MAKE_OPPI",               &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_CLOSE_UI_MAKE_OPPI,             "SERVER_CLOSE_UI_MAKE_OPPI",              &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_MD_EXIT,                        "SERVER_MD_EXIT",                         &HabboSocket::HandleServerMessage              );

        ///////////////////////////////////////////
        //         PELLEHYPPY HANDLER
        ///////////////////////////////////////////
        StoreServerPacket(PacketServerHeader::SERVER_JUMP_DATA,                      "SERVER_JUMP_DATA",                       &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_JUMP_LIFT_DOOR_OPEN,            "SERVER_JUMP_LIFT_DOOR_OPEN",             &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_JUMP_LIFT_DOOR_CLOSE,           "SERVER_JUMP_LIFT_DOOR_CLOSE",            &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_JUMPING_PLACE_OK,               "SERVER_JUMPING_PLACE_OK",                &HabboSocket::HandleServerMessage              );

        ///////////////////////////////////////////
        //            HUBU HANDLER
        ///////////////////////////////////////////
        StoreServerPacket(PacketServerHeader::SERVER_VOTE_QUESTION,                  "SERVER_VOTE_QUESTION",                   &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_VOTE_RESULTS,                   "SERVER_VOTE_RESULTS",                    &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_CANNOT_ENTER_BUS,               "SERVER_CANNOT_ENTER_BUS",                &HabboSocket::HandleServerMessage              );

        ///////////////////////////////////////////
        //        REGISTRATION HANDLER
        ///////////////////////////////////////////
        StoreServerPacket(PacketServerHeader::SERVER_OK,                              "SERVER_OK",                             &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_LOGIN_OK,                        "SERVER_LOGIN_OK",                       &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_AVAILABLE_SETS,                  "SERVER_AVAILABLE_SETS",                 &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_APPROVE_NAME,                    "SERVER_APPROVE_NAME",                   &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_NAME_UNACCEPTABLE,               "SERVER_NAME_UNACCEPTABLE",              &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_REG_OK,                          "SERVER_REG_OK",                         &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_ACR,                             "SERVER_REG_OK",                         &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_UPDATE_OK,                       "SERVER_UPDATE_OK",                      &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_RE_REG_REQUIRED,                 "SERVER_RE_REG_REQUIRED",                &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_COPPA_CHECK_TIME,                "SERVER_COPPA_CHECK_TIME",               &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_COPPA_GET_REAL_TIME,             "SERVER_COPPA_GET_REAL_TIME",            &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_PARENT_EMAIL_REQUIRED,           "SERVER_PARENT_EMAIL_REQUIRED",          &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_PARENT_EMAIL_VALIDATED,          "SERVER_PARENT_EMAIL_VALIDATED",         &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_UPDATE_ACCOUNT,                  "SERVER_UPDATE_ACCOUNT",                 &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_EMAIL_APPROVED,                  "SERVER_EMAIL_APPROVED",                 &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_EMAIL_REJECTED,                  "SERVER_EMAIL_REJECTED",                 &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_UPDATE_REQUEST,                  "SERVER_UPDATE_REQUEST",                 &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_PASSWORD_APPROVED,               "SERVER_PASSWORD_APPROVED",              &HabboSocket::HandleServerMessage              );

        ///////////////////////////////////////////
        //           RECYCLER HANDLER
        ///////////////////////////////////////////
        StoreServerPacket(PacketServerHeader::SERVER_RECYCLER_CONFIG,                  "SERVER_RECYCLER_CONFIG",               &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_STATUS ,                          "SERVER_STATUS ",                       &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_APPROVE_RECYCLING_RESULT,         "SERVER_APPROVE_RECYCLING_RESULT",      &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_START_RECYCLING_RESULT,           "SERVER_START_RECYCLING_RESULT",        &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_CONFIRM_RECYCLING_RESULT,         "SERVER_CONFIRM_RECYCLING_RESULT",      &HabboSocket::HandleServerMessage              );

        ///////////////////////////////////////////
        //           PURSE HANDLER
        ///////////////////////////////////////////
        StoreServerPacket(PacketServerHeader::SERVER_CREDITS,                           "SERVER_CREDITS",                      &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_PURSE_1,                           "SERVER_PURSE_1",                      &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_VOUCHER_REDEEM_OK,                 "SERVER_VOUCHER_REDEEM_OK",            &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_VOUCHER_REDEEM_ERROR,              "SERVER_VOUCHER_REDEEM_ERROR",         &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_TICKETS,                           "SERVER_TICKETS",                      &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_NO_TICKETS,                        "SERVER_NO_TICKETS",                   &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_TICKETS_BUY,                       "SERVER_TICKETS_BUY",                  &HabboSocket::HandleServerMessage              );

        ///////////////////////////////////////////
        //           POLL HANDLER
        ///////////////////////////////////////////
        StoreServerPacket(PacketServerHeader::SERVER_POLL_OFFER,                        "SERVER_POLL_OFFER",                   &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_POLL_CONTENTS,                     "SERVER_POLL_CONTENTS",                &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_POLL_ERROR,                        "SERVER_POLL_ERROR",                   &HabboSocket::HandleServerMessage              );

        ///////////////////////////////////////////
        //          MESSENGER HANDLER
        ///////////////////////////////////////////
        StoreServerPacket(PacketServerHeader::SERVER_MESSENGER_INITIALIZE,               "SERVER_MESSENGER_INITIALIZE",        &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_MESSENGER_CONSOLE_UPDATE,           "SERVER_MESSENGER_CONSOLE_UPDATE",    &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_MESSENGER_MEMBER_INFO,              "SERVER_MESSENGER_MEMBER_INFO",       &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_MESSENGER_BUDDY_REQUEST,            "SERVER_MESSENGER_BUDDY_REQUEST",     &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_MESSENGER_CAMPAIGN_MESSAGE,         "SERVER_MESSENGER_CAMPAIGN_MESSAGE",  &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_MESSENGER_MESSAGE,                  "SERVER_MESSENGER_MESSAGE",           &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_MESSENGER_ADD_BUDDY,                "SERVER_MESSENGER_ADD_BUDDY",         &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_MESSENGER_REMOVE_BUDDY,             "SERVER_MESSENGER_REMOVE_BUDDY",      &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_MY_PERSISTENT_MESSAGE,              "SERVER_MY_PERSISTENT_MESSAGE",       &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_MESSENGER_ERROR,                    "SERVER_MESSENGER_ERROR",             &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_MESSENGER_BUDDY_LIST,               "SERVER_MESSENGER_BUDDY_LIST",        &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_MESSENGER_MESSAGES,                 "SERVER_MESSENGER_MESSAGES",          &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_MESSENGER_BUDDY_REQUEST_LIST,       "SERVER_MESSENGER_BUDDY_REQUEST_LIST",&HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_MESSENGER_BUDDY_REQUEST_RESULT,     "SERVER_MESSENGER_BUDDY_REQUEST_RESULT",&HabboSocket::HandleServerMessage            );

        ///////////////////////////////////////////
        //           PAALU HANDLER
        ///////////////////////////////////////////
        StoreServerPacket(PacketServerHeader::SERVER_PT_START,                          "SERVER_PT_START",                     &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_PT_PREPARE,                        "SERVER_PT_PREPARE",                   &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_PT_END,                            "SERVER_PT_END",                       &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_PT_TIME_OUT,                       "SERVER_PT_TIME_OUT",                  &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_STATUS ,                           "SERVER_STATUS ",                      &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_PT_WIN,                            "SERVER_PT_WIN",                       &HabboSocket::HandleServerMessage              );
        StoreServerPacket(PacketServerHeader::SERVER_PT_BOTH_LOSE,                      "SERVER_PT_BOTH_LOSE",                 &HabboSocket::HandleServerMessage              );


        LOG_INFO << "Loaded " << m_ClientOpcode.size() << " CMSG OPCodes";
        LOG_INFO << "Loaded " << m_ServerOpcode.size() << " SMSG OPCodes";
    } 
    
    /// StoreClientPacket
    /// Store Client packet into our storage
    /// @p_Opcode : Opcode Id
    /// @p_Name : Name of Opcode
    /// @p_Status : Status of Opcode
    /// @p_Process : When to process the packet
    /// @p_Handler : Handler Function which we will be accessing too
    void Opcodes::StoreClientPacket(uint64 const p_OpCode, char const* p_Name, PacketStatus const p_Status, PacketProcess p_Process, void(HabboSocket::* p_Handler)(ClientPacket* p_Packet))
    {
        OpcodeHandler& l_Ref = m_ClientOpcode[p_OpCode];
        l_Ref.Name = p_Name;
        l_Ref.Status = p_Status;
        l_Ref.Handler = p_Handler;
    }

    
    /// StoreServerPacket
    /// Store Server packet into our storage
    /// @p_Opcode : Opcode Id
    /// @p_Name : Name of Opcode
    /// @p_Handler : Handler Function which we will be accessing too
    void Opcodes::StoreServerPacket(uint64 const p_OpCode, char const* p_Name, void(HabboSocket::* p_Handler)(ClientPacket* p_Packet))
    {
        OpcodeHandler& l_Ref = m_ServerOpcode[p_OpCode];
        l_Ref.Name = p_Name;
        l_Ref.Handler = p_Handler;
    }
    
    /// GetClientPacket
    /// @p_Id : Id of client packet we are searching for
    OpcodeHandler const& Opcodes::GetClientPacket(const uint64& Id)
    {
        OpcodeMap::const_iterator l_Itr = m_ClientOpcode.find(Id);
        if (l_Itr != m_ClientOpcode.end())
            return l_Itr->second;
        return m_EmptyHandler;
    }
      
    /// GetServerPacket
    /// @p_Id : Id of server packet we are searching for
    OpcodeHandler const& Opcodes::GetServerPacket(const uint64& Id)
    {
        OpcodeMap::const_iterator l_Itr = m_ServerOpcode.find(Id);
        if (l_Itr != m_ServerOpcode.end())
            return l_Itr->second;
        return m_EmptyHandler;
    }
}
