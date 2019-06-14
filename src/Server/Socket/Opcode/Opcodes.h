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

#ifndef _OPCODES_OPCODES_h
#define _OPCODES_OPCODES_h
#include "Common/SharedDefines.h"
#include "Network/ClientPacket.h"
#endif /* _OPCODES_OPCODES_h */

enum PacketClientHeader : uint16
{
    ///////////////////////////////////////////
    //             LOGIN HANDLER
    ///////////////////////////////////////////
    CLIENT_INFO_RETRIEVE                  = 7,
    CLIENT_TRY_LOGIN                      = 4,
    CLIENT_VERSION_CHECK                  = 5,
    CLIENT_UNIQUE_ID                      = 6,
    CLIENT_GET_PASSWORD                   = 47,
    CLIENT_LANG_CHECK                     = 58,
    CLIENT_BTCKS                          = 105,
    CLIENT_GET_AVAILABLE_BADGES           = 157,
    CLIENT_GET_SESSION_PARAMETERS         = 181,
    CLIENT_PONG                           = 196,
    CLIENT_GENERATE_KEY                   = 202,
    CLIENT_SSO                            = 204,
    CLIENT_INITIALIZE_CRYPTO              = 206,
    CLIENT_SECRET_KEY                     = 207,
    CLIENT_GET_SOUND_SETTINGS             = 228,
    CLIENT_SET_SOUND_SETTINGS             = 229,

    ///////////////////////////////////////////
    //             ROOM HANDLER
    ///////////////////////////////////////////
    CLIENT_ROOM_DIRECTORY                 = 2,
    CLIENT_GET_DOOR_FLAT                  = 28,
    CLIENT_ROOM_CHAT                      = 52,
    CLIENT_ROOM_SHOUT                     = 55,
    CLIENT_ROOM_WHISPER                   = 56,
    CLIENT_QUIT                           = 53,
    CLIENT_GO_VIA_DOOR                    = 54,
    CLIENT_TRY_FLAT                       = 57,
    CLIENT_GO_TO_FLAT                     = 59,
    CLIENT_G_HEIGHT_MAP                   = 60,
    CLIENT_G_USERS                        = 61,
    CLIENT_G_OBJECTS                      = 62,
    CLIENT_G_ITEMS                        = 63,
    CLIENT_G_STAT                         = 64,
    CLIENT_GET_STRIP                      = 65,
    CLIENT_FLAT_PROP_BY_ITEM              = 66,
    CLIENT_ADD_STRIP_ITEM                 = 67,
    CLIENT_TRADE_UNACCEPT                 = 68,
    CLIENT_TRADE_ACCEPT                   = 69,
    CLIENT_TRADE_CLOSE                    = 70,
    CLIENT_TRADE_OPEN                     = 71,
    CLIENT_TRADE_ADD_ITEM                 = 72,
    CLIENT_MOVE_STUFF                     = 73,
    CLIENT_SET_STUFF_DATA                 = 74,
    CLIENT_MOVE                           = 75,
    CLIENT_THROW_DICE                     = 76,
    CLIENT_DICE_OFF                       = 77,
    CLIENT_PRESENT_OPEN                   = 78,
    CLIENT_LOOK_TO                        = 79, 
    CLIENT_CARRY_DRINK                    = 80,
    CLIENT_IN_TO_DOOR                     = 81,
    CLIENT_DOOR_GO_IN                     = 82,
    CLIENT_G_IDATA                        = 83,
    CLIENT_SET_ITEM_DATA                  = 84,
    CLIENT_REMOVE_ITEM                    = 85,
    CLIENT_CARRY_ITEM                     = 87,
    CLIENT_STOP                           = 88,
    CLIENT_USE_ITEM                       = 89,
    CLIENT_PLACE_STUFF                    = 90,
    CLIENT_DANCE                          = 93,  
    CLIENT_WAVE                           = 94,
    CLIENT_KICK_USER                      = 95,
    CLIENT_ASSIGN_RIGHTS                  = 96,
    CLIENT_REMOVE_RIGHTS                  = 97,
    CLIENT_LET_USER_IN                    = 98,
    CLIENT_REMOVE_STUFF                   = 99,
    CLIENT_GO_AWAY                        = 115,
    CLIENT_GET_ROOM_ADD                   = 126,
    CLIENT_GET_PET_STAT                   = 128,
    CLIENT_SET_BADGE                      = 158,
    CLIENT_GET_INTEREST                   = 182,
    CLIENT_CONVERT_FURNI_TO_CREDITS       = 183,
    CLIENT_ROOM_QUEUE                     = 211,
    CLIENT_SET_ITEM_STATE                 = 214,
    CLIENT_SET_SPECTATOR_AMOUNT           = 216,
    CLIENT_GET_GROUP_BADGES               = 230,
    CLIENT_GROUP_DETAILS                  = 231,
    CLIENT_ROOM_VOTE                      = 261,

    ///////////////////////////////////////////
    //             NAVIGATOR HANDLER
    ///////////////////////////////////////////
    CLIENT_S_BUSY_F                       = 13,
    CLIENT_SUSERF                         = 16,
    CLIENT_SEARCH_FLATS                   = 17,
    CLIENT_GET_FAVOURITE_ROOMS            = 18,
    CLIENT_ADD_FAVOURITE_ROOM             = 19,
    CLIENT_DELETE_FAVOURITE_ROOM          = 20,
    CLIENT_GET_FLAT_INFO                  = 21,
    CLIENT_DELETE_FLAT                    = 23,
    CLIENT_UPDATE_FLAT                    = 24,
    CLIENT_SET_FLAT_INFO                  = 25,
    CLIENT_CREATE_FLAT                    = 29,
    CLIENT_NAVIGATE                       = 150,
    CLIENT_GET_USER_FLAT_CATEGORIES       = 151,
    CLIENT_GET_FLAT_CATEGORY              = 152,
    CLIENT_SET_FLAT_CATEGORY              = 153,
    CLIENT_GET_SPACE_NODE_USERS           = 154,
    CLIENT_REMOVE_ALL_RIGHTS              = 155,
    CLIENT_GET_PARENT_CHAIN               = 156,

    ///////////////////////////////////////////
    //             HOBBA HANDLER
    ///////////////////////////////////////////
    CLIENT_PICK_CRY_FOR_HELP              = 48,
    CLIENT_CRY_FOR_HELP                   = 86,
    CLIENT_CHANGE_CALL_BACK_CATEGORY      = 198,
    CLIENT_MESSAGE_TO_CALLER              = 199,
    CLIENT_MODERATION_ACTION              = 200,

    ///////////////////////////////////////////
    //         SOUND MACHINE HANDLER
    ///////////////////////////////////////////
    CLIENT_GET_SOUND_MACHINE_CONFIG        = 217,
    CLIENT_SAVE_SOUND_MACHINE_CONFIG       = 218,
    CLIENT_INSERT_SOUND_PACKAGE            = 219,
    CLIENT_EJECT_SOUND_PACKAGE             = 220,
    CLIENT_GET_SOUND_DATA                  = 221,

    ///////////////////////////////////////////
    //         MOUNTAIN HANDLER
    ///////////////////////////////////////////
    CLIENT_CLOSE_UK_MAKE_OPPI              = 108,
    CLIENT_CHANGE_SHIRT                    = 109,
    CLIENT_REFRESH_FIGURE                  = 110,
    CLIENT_SWIM_SUIT                       = 116,

    ///////////////////////////////////////////
    //         PELLEHYPPY HANDLER
    ///////////////////////////////////////////
    CLIENT_JUMP_START                     = 103,
    CLIENT_SIGN                           = 122,
    CLIENT_JUMP_PERF                      = 106,
    CLIENT_SPLASH_POSITION                = 107,

    ///////////////////////////////////////////
    //           HUBU HANDLER
    ///////////////////////////////////////////
    CLIENT_CHANGE_WORLD                   = 111,
    CLIENT_VOTE                           = 112,
    CLIENT_TRY_INFO_BUS                   = 113,

    ///////////////////////////////////////////
    //        REGISTRATION HANDLER
    ///////////////////////////////////////////
    CLIENT_GDATE                          = 49,
    CLIENT_GET_AVAILABLE_SETS             = 9,
    CLIENT_FIND_USER                      = 41,
    CLIENT_APPROVE_NAME                   = 42,
    CLIENT_REGISTER                       = 43,
    CLIENT_UPDATE                         = 44,
    CLIENT_AC                             = 46,
    CLIENT_COPPA_REG_CHECK_TIME           = 130,
    CLIENT_COPPA_REG_GET_REAL_TIME        = 131,
    CLIENT_PARENT_EMAIL_REQUIRED          = 146,
    CLIENT_VALIDATE_PARENT_EMAIL          = 147,
    CLIENT_SEND_PARENT_EMAIL              = 148,
    CLIENT_UPDATE_ACCOUNT                 = 149,
    CLIENT_APPROVE_EMAIL                  = 197,
    CLIENT_APPROVE_PASSWORD               = 203,

    ///////////////////////////////////////////
    //           RECYCLER HANDLER
    ///////////////////////////////////////////
    CLIENT_GET_FURNI_RECYCLER_CONFIG      = 222,
    CLIENT_GET_FURNI_RECYCLER_STATUS      = 223,
    CLIENT_APPROVE_RECYCLED_FURNI         = 224,
    CLIENT_START_FURNI_RECYCLING          = 225,
    CLIENT_CONFIRM_FURNI_RECYCLING        = 226,

    ///////////////////////////////////////////
    //           PURSE HANDLER
    ///////////////////////////////////////////
    CLIENT_GET_CREDITS                    = 8,
    CLIENT_GET_USER_CREDIT_LOG            = 127,
    CLIENT_REDEEM_VOUCHER                 = 129,

    ///////////////////////////////////////////
    //           POLL HANDLER
    ///////////////////////////////////////////
    CLIENT_POLL_START                     = 234,
    CLIENT_POLL_REJECT                    = 235,
    CLIENT_POLL_ANSWER                    = 236,

    ///////////////////////////////////////////
    //          MESSENGER HANDLER
    ///////////////////////////////////////////
    CLIENT_MESSENGER_INITIALIZE           = 12,
    CLIENT_MESSENGER_UPDATE               = 15,
    CLIENT_MESSENGER_C_CLICK              = 30,
    CLIENT_MESSENGER_C_READ               = 31,
    CLIENT_MESSENGER_MARK_READ            = 32,
    CLIENT_MESSENGER_SEND_MESSAGE         = 33,
    CLIENT_MESSENGER_CONSOLE_MOTTO       = 36,
    CLIENT_MESSENGER_ACCEPT_BUDDY         = 37,
    CLIENT_MESSENGER_DECLINE_BUDDY        = 38,
    CLIENT_MESSENGER_REQUEST_BUDDY        = 39,
    CLIENT_MESSENGER_REMOVE_BUDDY         = 40,
    CLIENT_MESSENGER_GET_MESSAGE          = 191,
    CLIENT_MESSENGER_REPORT_MESSAGE       = 201,
    CLIENT_MESSENGER_BUDDY_REQUESTS       = 233,

    ///////////////////////////////////////////
    //           CLUB HANDLER
    ///////////////////////////////////////////
    CLIENT_CLUB_GET_USER_INFO             = 26,
    CLIENT_CLUB_BUY                       = 190,
    CLIENT_GIFT_APPROVAL                  = 210,

    ///////////////////////////////////////////
    //           CATALOGUE HANDLER
    ///////////////////////////////////////////
    CLIENT_GPRC                           = 100,
    CLIENT_GCIX                           = 101,
    CLIENT_GCAP                           = 102,
    CLIENT_GET_ALIAS_LIST                 = 215,
     
};

enum PacketServerHeader : uint16
{
    ///////////////////////////////////////////
    //             LOGIN HANDLER
    ///////////////////////////////////////////
    SERVER_CRYPTO_PARAMETERS              = 277, ///< #handleCryptoParameters
    SERVER_SESSION_PARAMETERS             = 257, ///< #handleSessionParameters
    SERVER_LOGIN_OK                       = 3,   ///< #handleLoginOK
    SERVER_SYSTEM_BROADCAST               = 139, ///< #handleSystemBroadcast
    SERVER_USER_OBJECT                    = 5,   ///< #handleUserObj
    SERVER_FUSE_RIGHTS                    = 2,   ///< #handleRights
    SERVER_AVAILABLE_BADGES               = 229, ///< #handleAvailableBadges
    SERVER_SOUND_SETTINGS                 = 308, ///< #handleSoundSetting
    SERVER_PING                           = 50,  ///< #handlePing
    SERVER_HOTEL_LOGOUT                   = 287, ///< #handleHotelLogout
    SERVER_MOD_ALERT                      = 161, ///< #handleModAlert
    SERVER_CHECK_SUM                      = 141, ///< #handleCheckSum
    SERVER_ESP_NOTIFY                     = 52,  ///< #handleEPSnotify
    SERVER_USER_BANNED                    = 35,  ///< #handleUserBanned
    SERVER_ERROR                          = 33,  ///< #handleErr
    SERVER_GDATE                          = 49,  ///< #handle_GDate

    ///////////////////////////////////////////
    //             ROOM HANDLER
    ///////////////////////////////////////////
    SERVER_CLC                            = 18,  ///< #handle_clc
    SERVER_CLC_OK                         = 19,  ///< #handle_opc_ok
    SERVER_CHAT                           = 24,  ///< #handle_chat
    SERVER_WHISPER                        = 25,  ///< #handle_chat
    SERVER_SHOUT                          = 26,  ///< #handle_chat
    SERVER_USERS                          = 28,  ///< #handle_users
    SERVER_LOGOUT                         = 29,  ///< #handle_logout
    SERVER_OBJECTS                        = 30,  ///< #handle_OBJECTS
    SERVER_ROOM_HEIGHT_MAP                = 31,  ///< #handle_heightmap
    SERVER_ACTIVE_OBJECTS                 = 32,  ///< #handle_activeobjects
    SERVER_STATUS                         = 34,  ///< #handle_status
    SERVER_FLAT_LET_IN                    = 41,  ///< #handle_flat_letin
    SERVER_ITEMS                          = 45,  ///< #handle_items
    SERVER_YOU_ARE_CONTROLLER             = 42,  ///< #handle_room_rights
    SERVER_YOU_ARE_NOT_CONTROLLER         = 43,  ///< #handle_room_rights
    SERVER_FLAT_PROPERTY                  = 46,  ///< #handle_flatproperty
    SERVER_YOU_ARE_OWNER                  = 47,  ///< #handle_room_rights
    SERVER_IDATA                          = 48,  ///< #handle_idata
    SERVER_GO_TO_FLAT                     = 59,  ///< #handle_goto_flat
    SERVER_TELEPORT_INITIALIZED           = 62,  ///< #handle_doorflat
    SERVER_DOOR_DELETED                   = 63,  ///< #handle_doordeleted
    SERVER_DOOR_DELETED_1                 = 64,  ///< #handle_doordeleted
    SERVER_ROOM_READY                     = 69,  ///< #handle_room_ready
    SERVER_YOU_ARE_MOD                    = 70,  ///< #handle_youaremod
    SERVER_SHOW_PROGRAM                   = 71,  ///< #handle_showprogram
    SERVER_NO_USER_FOR_GIFT               = 76,  ///< #handle_no_user_for_gift
    SERVER_HANDLE_ITEMS                   = 83,  ///< #handle_no_user_for_gift
    SERVER_REMOVE_ITEM                    = 84,  ///< #handle_removeitem
    SERVER_UPDATE_ITEM                    = 85,  ///< #handle_updateitem
    SERVER_STUFF_DATA_UPDATE              = 88,  ///< #handle_stuffdataupdate
    SERVER_DOOR_OUT                       = 89,  ///< #handle_door_out
    SERVER_DICE_VALUE                     = 90,  ///< #handle_dice_value
    SERVER_DOOR_BELL_RINGING              = 91,  ///< #handle_doorbell_ringing
    SERVER_DOOR_IN                        = 92,  ///< #handle_door_in
    SERVER_ACTIVE_OBJECT_ADD              = 93,  ///< #handle_activeobject_add
    SERVER_ACTIVE_OBJECT_REMOVE           = 94,  ///< #handle_activeobject_remove
    SERVER_ACTIVE_OBJECT_UPDATE           = 95,  ///< #handle_activeobject_update
    SERVER_STRIP_INFO                     = 98,  ///< #handle_stripinfo
    SERVER_REMOVE_STRIP_ITEM              = 99,  ///< #handle_removestripitem
    SERVER_STRIP_UPDATED                  = 101, ///< #handle_stripupdated
    SERVER_YOU_ARE_NOT_ALLOWED            = 102, ///< #handle_othernotallowed
    SERVER_TRADE_COMPLETED                = 105, ///< #handle_trade_completed
    SERVER_TRADE_ITEMS                    = 108, ///< #handle_trade_items
    SERVER_TRADE_ACCEPT                   = 109, ///< #handle_trade_accept
    SERVER_TRADE_CLOSE                    = 110, ///< #handle_trade_close
    SERVER_TRADE_COMPLETED_1              = 112, ///< #handle_trade_completed
    SERVER_PRESENT_OPEN                   = 129, ///< #handle_presentopen
    SERVER_FLAT_NOT_ALLOWED_TO_ENTER      = 131, ///< #handle_flatnotallowedtoenter
    SERVER_INVENTORY                      = 140, ///< #handle_stripinfo
    SERVER_ROOM_URL                       = 166, ///< #handle_roomurl
    SERVER_ROOM_ADD                       = 208, ///< #handle_roomad
    SERVER_PET_STAT                       = 210, ///< #handle_petstat
    SERVER_HEIGHT_MAP_UPDATE              = 219, ///< #handle_heightmapupdate
    SERVER_USE_BADGE                      = 228, ///< #handle_userbadge
    SERVER_SLIDE_OBJECT_BUNDLE            = 230, ///< #handle_slideobjectbundle
    SERVER_ROOM_INTEREST                  = 258, ///< #handle_interstitialdata
    SERVER_ROOM_QUEUE_DATA                = 259, ///< #handle_roomqueuedata
    SERVER_YOU_ARE_SPECTATOR              = 254, ///< #handle_youarespectator
    SERVER_REMOVE_SPECTATOR               = 283, ///< #handle_removespecs
    SERVER_FIGURE_CHANGE                  = 266, ///< #handle_figure_change
    SERVER_SPECTATOR_AMOUNT               = 298, ///< #handle_spectator_amount
    SERVER_GROUP_BADGES                   = 309, ///< #handle_group_badges
    SERVER_GROUP_MEMBER_SHIP_UPDATE       = 310, ///< #handle_group_membership_update
    SERVER_GROUP_DETAILS                  = 311, ///< #handle_group_details
    SERVER_UPDATE_VOTES                   = 345, ///< #handle_room_vote

    ///////////////////////////////////////////
    //             NAVIGATOR HANDLER
    ///////////////////////////////////////////
    SERVER_FLAT_RESULTS                   = 16,  ///< #handle_flat_results
    SERVER_FLAT_INFO                      = 54,  ///< #handle_flatinfo
    SERVER_FLAT_RESULTS_1                 = 55,  ///< #handle_flat_results
    SERVER_NO_FLATS_FOR_USER              = 57,  ///< #handle_noflatsforuser
    SERVER_NO_FLATS                       = 58,  ///< #handle_noflats
    SERVER_FAVOURITE_ROOMS_RESULT         = 61,  ///< #handle_favouriteroomresults
    SERVER_FLAT_PASSWORD_OK               = 130, ///< #handle_flatpassword_ok
    SERVER_NAVIGATE_NODE_INFO             = 220, ///< #handle_navnodeinfo
    SERVER_USER_FLAT_CATEGORIES           = 221, ///< #handle_navnodeinfo
    SERVER_USER_FLAT_CATEGORY             = 222, ///< #handle_flatcat
    SERVER_SPACE_NODE_USERS               = 223, ///< #handle_spacenodeusers
    SERVER_CANT_CONNECT                   = 224, ///< #handle_cantconnect
    SERVER_SUCCESS                        = 225, ///< #handle_success
    SERVER_PARENT_FAILURE                 = 227, ///< #handle_parentchain
    SERVER_ROOM_FORWARD                   = 286, ///< #handle_roomforward

    ///////////////////////////////////////////
    //             HOBBA HANDLER
    ///////////////////////////////////////////
    SERVER_CRY_FOR_HELP                   = 148, ///< #handle_cryforhelp
    SERVER_PICKED_CRY                     = 149, ///< #handle_picked_cry
    SERVER_DELETE_CRY                     = 273, ///< #handle_delete_cry
    SERVER_CRY_REPLY                      = 274, ///< #handle_cry_reply

    ///////////////////////////////////////////
    //         SOUND MACHINE HANDLER
    ///////////////////////////////////////////
    SERVER_HANDLE_SOUND_DATA              = 300, ///< #handle_sound_data
    SERVER_MACHINE_SOUND_PACKAGES         = 301, ///< #handle_machine_sound_packages
    SERVER_USER_SOUND_PACKAGES            = 302, ///< #handle_user_sound_packages

    ///////////////////////////////////////////
    //         ERROR REPORT HANDLER
    ///////////////////////////////////////////
    SERVER_ERROR_REPORT                   = 299, ///< #handle_error_report

    ///////////////////////////////////////////
    //         MOUNTAIN HANDLER
    ///////////////////////////////////////////
    SERVER_OPEN_UI_MAKE_OPPI              = 96, ///< #handle_open_uimakoppi
    SERVER_CLOSE_UI_MAKE_OPPI             = 97, ///< #handle_close_uimakoppi
    SERVER_MD_EXIT                        = 121,///< #handle_md_exit

    ///////////////////////////////////////////
    //         PELLEHYPPY HANDLER
    ///////////////////////////////////////////
    SERVER_JUMP_DATA                      = 74,  ///< #handle_jumpdata
    SERVER_JUMP_LIFT_DOOR_OPEN            = 122, ///< #handle_jumpliftdoor_open
    SERVER_JUMP_LIFT_DOOR_CLOSE           = 123, ///< #handle_jumpliftdoor_close
    SERVER_JUMPING_PLACE_OK               = 125, ///< #handle_jumpingplace_ok

    ///////////////////////////////////////////
    //           HUBU HANDLER
    ///////////////////////////////////////////
    SERVER_VOTE_QUESTION                  = 79, ///< #handle_vote_question
    SERVER_VOTE_RESULTS                   = 80, ///< #handle_vote_results
    SERVER_CANNOT_ENTER_BUS               = 81, ///< #handle_cannot_enter_bus

    ///////////////////////////////////////////
    //        REGISTRATION HANDLER
    ///////////////////////////////////////////
    SERVER_OK                             = 1,  ///< #handle_ok
    SERVER_AVAILABLE_SETS                 = 8,  ///< #handle_availablesets
    SERVER_APPROVE_NAME                   = 36, ///< #handle_approvenamereply
    SERVER_NAME_UNACCEPTABLE              = 37, ///< #handle_nameunacceptable
    SERVER_REG_OK                         = 51, ///< #handle_regok
    SERVER_ACR                            = 164,///< #handle_acr
    SERVER_UPDATE_OK                      = 211,///< #handle_updateok
    SERVER_RE_REG_REQUIRED                = 167,///< #handle_reregistrationrequired
    SERVER_COPPA_CHECK_TIME               = 214,///< #handle_coppa_checktime
    SERVER_COPPA_GET_REAL_TIME            = 215,///< #handle_coppa_getrealtime
    SERVER_PARENT_EMAIL_REQUIRED          = 217,///< #handle_parent_email_required
    SERVER_PARENT_EMAIL_VALIDATED         = 218,///< #handle_parent_email_validated
    SERVER_UPDATE_ACCOUNT                 = 169,///< #handle_update_account
    SERVER_EMAIL_APPROVED                 = 271,///< #handle_email_approved
    SERVER_EMAIL_REJECTED                 = 272,///< #handle_email_rejected
    SERVER_UPDATE_REQUEST                 = 275,///< #handle_update_request
    SERVER_PASSWORD_APPROVED              = 282,///< #handle_password_approved

    ///////////////////////////////////////////
    //           RECYCLER HANDLER
    ///////////////////////////////////////////
    SERVER_RECYCLER_CONFIG                = 303,///< #handle_recycler_configuration
    SERVER_RECYCLER_STATUS                = 304,///< #handle_recycler_status
    SERVER_APPROVE_RECYCLING_RESULT       = 305,///< #handle_approve_recycling_result
    SERVER_START_RECYCLING_RESULT         = 306,///< #handle_start_recycling_result
    SERVER_CONFIRM_RECYCLING_RESULT       = 307,///< #handle_confirm_recycling_result

    ///////////////////////////////////////////
    //           PURSE HANDLER
    ///////////////////////////////////////////
    SERVER_CREDITS                        = 6,  ///< #handle_purse
    SERVER_PURSE_1                        = 209,///< #handle_purse
    SERVER_VOUCHER_REDEEM_OK              = 212,///< #handle_purse
    SERVER_VOUCHER_REDEEM_ERROR           = 213,///< #handle_purse
    SERVER_TICKETS                        = 72, ///< #handle_tickets
    SERVER_NO_TICKETS                     = 73, ///< #handle_notickets
    SERVER_TICKETS_BUY                    = 124,///< #handle_ticketsbuy

    ///////////////////////////////////////////
    //           POLL HANDLER
    ///////////////////////////////////////////
    SERVER_POLL_OFFER                      = 316,///< #handle_poll_offer
    SERVER_POLL_CONTENTS                   = 317,///< #handle_poll_contents
    SERVER_POLL_ERROR                      = 318,///< #handle_poll_error

    ///////////////////////////////////////////
    //          MESSENGER HANDLER
    ///////////////////////////////////////////
    SERVER_MESSENGER_INITIALIZE            = 12, ///< #handle_messenger_init
    SERVER_MESSENGER_CONSOLE_UPDATE        = 13, ///< #handle_console_update
    SERVER_MESSENGER_MEMBER_INFO           = 128,///< #handle_memberinfo
    SERVER_MESSENGER_BUDDY_REQUEST         = 132,///< #handle_buddy_request
    SERVER_MESSENGER_CAMPAIGN_MESSAGE      = 133,///< #handle_campaign_message
    SERVER_MESSENGER_MESSAGE               = 134,///< #handle_messenger_message
    SERVER_MESSENGER_ADD_BUDDY             = 137,///< #handle_add_buddy
    SERVER_MESSENGER_REMOVE_BUDDY          = 138,///< #handle_remove_buddy
    SERVER_MY_PERSISTENT_MESSAGE           = 147,///< #handle_mypersistentmessage
    SERVER_MESSENGER_ERROR                 = 260,///< #handle_messenger_error
    SERVER_MESSENGER_BUDDY_LIST            = 263,///< #handle_buddylist
    SERVER_MESSENGER_MESSAGES              = 313,///< #handle_messenger_messages
    SERVER_MESSENGER_BUDDY_REQUEST_LIST    = 314,///< #handle_buddy_request_list
    SERVER_MESSENGER_BUDDY_REQUEST_RESULT  = 315,///< #handle_buddy_request_result
    SERVER_MESSENGER_FIND_USER             = 128,///< #handle_find_user


    ///////////////////////////////////////////
    //           PAALU HANDLER
    ///////////////////////////////////////////
    SERVER_PT_START                        = 114,///< #handle_pt_start
    SERVER_PT_PREPARE                      = 115,///< #handle_pt_prepare
    SERVER_PT_END                          = 116,///< #handle_pt_end
    SERVER_PT_TIME_OUT                     = 117,///< #handle_pt_timeout
    SERVER_PT_STATUS                       = 118,///< #handle_pt_status
    SERVER_PT_WIN                          = 119,///< #handle_pt_win
    SERVER_PT_BOTH_LOSE                    = 120,///< #handle_pt_bothlose

    ///////////////////////////////////////////
    //           CLUB HANDLER
    ///////////////////////////////////////////
    SERVER_CLUB_BUY                        = 190,
    SERVER_GIFT_APPROVAL                   = 210,
    SERVER_CLUB_INFO                       = 7,

    ///////////////////////////////////////////
    //           CATALOGUE HANDLER
    ///////////////////////////////////////////
    SERVER_CATALOGUE_PAGES                 = 126,
    SERVER_CATALOGUE_ITEMS                 = 127,
    SERVER_SPRITE_LIST                     = 295,
    SERVER_ALIAS_TOGGLE                    = 297,
};

/// Used to prevent any one attempting to tamper with the client packets,
/// and send packets which are not yet registered by server yet
enum PacketStatus
{
    STATUS_AUTH,                                ///< Player is at authentication stage (habbo is not initialized yet)
    STATUS_HOTEL_VIEW,                          ///< Player is at hotel view (habbo is initialized)
    STATUS_IN_ROOM,                             ///< Player is inside room
    STATUS_ALL,                                 ///< Accept packet regardless what stage player is at
    STATUS_UNHANDLED                            ///< Packet is not handled yet
};

/// Used to determine whether we can process the packet now, or process
/// it on next hotel update to ensure thread safety
enum PacketProcess
{
    PROCESS_NOW,                                ///< Process packet now
    PROCESS_PLAYER_UPDATE,                      ///< Process packet on Player Update
    PROCESS_ROOM_UPDATE                         ///< Process packet on Room Update (player must be in room)
};

namespace SteerStone
{
    class HabboSocket;

    /// Holds the handler and name of opcode
    struct OpcodeHandler
    {
        char const* Name;
        PacketStatus Status;
        PacketProcess Process;
        void (HabboSocket::*Handler)(ClientPacket* p_Packet);
    };

    typedef std::map<uint64, OpcodeHandler> OpcodeMap;

    /// Stores Opcodes handler for server and client
    class Opcodes
    {
    public:
        static Opcodes* instance();

    public:
        /// Constructor
        Opcodes() {}

        /// Deconstructor
        ~Opcodes() {}

    public:
        /// InitializePackets
        /// Load our packets into storages to be accessed later
        void InitializePackets();

        /// GetClientPacket
        /// @p_Id : Id of client packet we are searching for
        OpcodeHandler const& GetClientPacket(const uint64& Id);

        /// GetServerPacket
        /// @p_Id : Id of server packet we are searching for
        OpcodeHandler const& GetServerPacket(const uint64& Id);

    private:
        /// StoreClientPacket
        /// Store Client packet into our storage
        /// @p_Opcode : Opcode Id
        /// @p_Name : Name of Opcode
        /// @p_Status : Status of Opcode
        /// @p_Process : When to process the packet
        /// @p_Handler : Handler Function which we will be accessing too
        void StoreClientPacket(uint64 const p_OpCode, char const* p_Name, PacketStatus const p_Status, PacketProcess p_Process, void (HabboSocket::*handler)(ClientPacket* p_Packet));

        /// StoreServerPacket
        /// Store Server packet into our storage
        /// @p_Opcode : Opcode Id
        /// @p_Name : Name of Opcode
        /// @p_Handler : Handler Function which we will be accessing too
        void StoreServerPacket(uint64 const p_OpCode, char const* p_Name, void (HabboSocket::*handler)(ClientPacket* p_Packet));

    private:
        static OpcodeHandler const m_EmptyHandler;      ///< Empty handler if our client packet has not been given a handler yet
        OpcodeMap m_ClientOpcode;                       ///< Holds our client packets
        OpcodeMap m_ServerOpcode;                       ///< Holds our server packets
    };
}

#define sOpcode SteerStone::Opcodes::instance()