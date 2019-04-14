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

#pragma once
#include "Common/SharedDefines.h"
#include "Messenger/Messenger.h"
#include "FavouriteRooms/FavouriteRoom.h"
#include "Fuse/FuseRights.h"
#include "Club/Club.h"
#include "Badge/Badge.h"
#include <mutex>

namespace SteerStone
{
    enum SessionParameters
    {
        PARAMETER_COPPA                     = 0, ///< TODO; Find out what this does
        PARAMETER_VOUCHER                   = 1, ///< TODO; Determines if voucher is enabled in client
        PARAMETER_REQUIRE_PARENT_EMAIL      = 2, ///< TODO; Find out what this does
        PARAMETER_SEND_PARENT_EMAIL         = 3, ///< TODO; Find out what this does
        PARAMETER_DIRECT_MAIL               = 4, ///< Allowed to send email directly to user
        PARAMETER_DATE_FORMAT               = 5, ///< Date format
        PARAMETER_INTEGRATION_ENALBED       = 6, ///< TODO; Find out what this does
        PARAMETER_PROFILE_EDITING           = 7, ///< Allow user to edit profile ingame
        PARAMETER_TRACKING_HEADER           = 8, ///< Find out what this does
        PARAMETER_TUTORIAL                  = 9, ///< Show tutorial on first time login
    };

    enum AccountRank
    {
        HABBO_NORMAL                        = 0, ///< Default habbo rank
        HABBO_CLUB                          = 1, ///< Habbo Club Subscription
        HABBO_HOBBA                         = 2, ///< Hobba
        HABBO_SUPER_HOBBA                   = 3, ///< Super Hobba
        HABBO_MODERATOR                     = 4, ///< Moderator
        HABBO_ADMIN                         = 5, ///< Adminstrator
    };

    enum LogoutReason
    {
        LOGOUT_DISCONNECTED                 = -1,
        LOGGED_OUT                          = 1,
        LOGOUT_CONCURRENT                   = 2,
        LOGOUT_TIMEOUT                      = 3,
    };

    /// Used to send Status update to client(s)
    enum Status : uint32
    {
        STATUS_NONE                         = 0,             
        STATUS_WALKING                      = 1,  ///< Update user is walking
        STATUS_SITTING                      = 2,  ///< Update user is sitting
        STATUS_WAVING                       = 4,  ///< Update user is waving
        STATUS_DANCING                      = 8,  ///< Update user is dancing
        STATUS_SWIMMING                     = 16, ///< Update user is swimming
        STATUS_ROTATION                     = 32, ///< Update user rotation
        STATUS_TELEPORT                     = 64, ///< Teleport user to given location in room
    };

    class Room;
    class ClientPacket;
    class WalkWay;

    class Habbo
    {
    public:
        friend class HabboSocket;

    public:
        /// Constructor
        /// @p_HabboSocket -Socket is inheriting from
        Habbo(HabboSocket* p_HabboSocket);

        /// Deconstructor
        ~Habbo();

    public:

        ///////////////////////////////////////////
        //                 ROOMS
        ///////////////////////////////////////////

        /// SetRoom
        /// @p_Room - Room Id which player is inside room
        /// @p_WalkWay - Walk Way tile user will spawn ontop on
        bool SetRoom(std::shared_ptr<Room> p_Room, WalkWay* p_WalkWay = nullptr);

        /// GetRoom
        /// Get room
        std::shared_ptr<Room> GetRoom() const;

        /// DestroyRoom
        /// Set Room to nullptr - player is no longer inside room
        void DestroyRoom();

        ///////////////////////////////////////////
        //            FAVOURITE ROOMS
        ///////////////////////////////////////////

        /// SendFavouriteRooms
        /// Send Favourite Rooms list to user
        void SendFavouriteRooms();

        /// AddFavouriteRoom
        /// @p_IsPublic - Is room public
        /// @p_RoomId - Id of room
        void AddFavouriteRoom(bool const& p_IsPublic, uint32 const& p_RoomId);

        /// RemoveFavouriteRoom
        /// @p_RoomId - Id of room
        void RemoveFavouriteRoom(uint32 const& p_RoomId);

        /// SaveFavouriteRoomToDB
        /// Update Favourite Rooms to database
        void SaveFavouriteRoomToDB();

        ///////////////////////////////////////////
        //           MESSENGER CONSOLE
        ///////////////////////////////////////////

        /// SendInitializeMessenger
        /// Send Initialize console on user login
        void SendInitializeMessenger();

        /// MessengerAcceptRequest
        /// @p_SenderId - Id of room
        void MessengerAcceptRequest(uint32 const p_SenderId);

        /// SendMessengerUpdate
        /// Update messenger status
        void SendMessengerUpdate();

        /// SendSearchUserResults
        /// @p_Name - Room name user is searching for
        void SendSearchUserResults(std::string const p_Name);

        /// MessengerBuddyRequest
        /// @p_Name - Name of friend user is sending request for
        void MessengerBuddyRequest(std::string const p_Name);

        /// MessengerRemoveBuddy
        /// @p_Packet - Client packet which is being decoded
        void MessengerRemoveBuddy(std::unique_ptr<ClientPacket> p_Packet);

        /// MessengerRejectRequest
        /// @p_Packet - Client packet which is being decoded
        void MessengerRejectRequest(std::unique_ptr<ClientPacket> p_Packet);

        /// MessengerSendMessage
        /// @p_Packet - Client packet which is being decoded
        void MessengerSendMessage(std::unique_ptr<ClientPacket> p_Packet);

        /// MessengerReply
        /// @p_MessageId - Id of message (account id)
        void MessengerReply(uint32 const p_MessageId);

        ///////////////////////////////////////////
        //             USER OBJECTS
        ///////////////////////////////////////////

        /// SendHabboObject
        /// Send user object on login
        void SendHabboObject();

        /// SendUpdateStatusWalk
        /// @p_X - X axis on new position
        /// @p_Y - Y axis on new position
        /// @p_Z - Z axis on new position
        /// @p_Rotation - New Rotation
        void UpdatePosition(int16 const p_X, int16 const p_Y, int16 const p_Z, int16 const p_Rotation);

        /// TeleportTo
        /// Teleport player to given location
        /// @p_X - X axis on new position
        /// @p_Y - Y axis on new position
        /// @p_Z - Z axis on new position
        /// @p_Rotation - New Rotation
        void TeleportTo(int16 const p_X, int16 const p_Y, int16 const p_Z, int16 const p_Rotation);

        /// LookTo
        /// @p_X : X axis to face targeted user
        /// @p_Y : Y axis to face targeted user
        void LookTo(int16 const p_X, int16 const p_Y);

        ///////////////////////////////////////////
        //             ACCOUNT INFO
        ///////////////////////////////////////////

        /// SendAccountPreferences
        /// Send user account preferences (set from users.account database)
        void SendAccountPreferences();

        /// SendAccountBadges
        /// Send user account badges (set from users.account_badges database)
        void SendAccountBadges();

        /// SendSetBadge
        /// Update new current badge
        /// @p_Badge : New badge we are setting to
        /// @p_Visible : Is new badge visible
        void SendSetBadge(std::string const p_Badge, bool const p_Visible);

        /// SendFuseRights
        /// Send user account rights (set from users.rank_fuserights database)
        void SendFuseRights();

        /// SendClubStatus
        /// Send user account club status (set from users.club_subscriptions database)
        void SendClubStatus();

        /// SetRank
        /// Update users rank
        /// @p_Rank : Rank user will recieve
        void SetRank(uint8 const p_Rank);

        ///////////////////////////////////////////
        //             HABBO INFO
        ///////////////////////////////////////////

        /// InitializeHabboData
        /// Initialize Habbo Info data when user logs in
        void InitializeHabboData();

        /// Update
        /// Update Habbo user
        /// @p_Diff - Tick Timer
        bool Update(uint32 const& p_Diff);

        /// Logout
        /// @p_Reason - Logout reason (enum LogoutReason)
        void Logout(LogoutReason const p_Reason = LOGGED_OUT);

        /// SaveToDB
        /// Save Habbo data to database on logout
        void SaveToDB();

    public:
        ///////////////////////////////////////////
        //                 ROOMS
        ///////////////////////////////////////////
        uint32 GetRoomGUID()             const { return m_RoomGUID;            }
        void SetRoomGUID(uint32 const& p_GUID) { m_RoomGUID = p_GUID;          }

        ///////////////////////////////////////////
        //           MESSENGER CONSOLE
        ///////////////////////////////////////////

        bool CanSendMail()              const  { return m_DirectMail;           }
        bool AcceptsFriendRequests()    const  { return m_AcceptFriendRequests; }

        ///////////////////////////////////////////
        //             ACCOUNT INFO
        ///////////////////////////////////////////

        bool IsSoundEnabled()           const { return m_SoundEnabled;         }
        bool IsPonged()                 const { return m_Ponged;               }
        uint16 GetRank()                const { return m_Rank;                 }

        ///////////////////////////////////////////
        //             HABBO INFO
        ///////////////////////////////////////////

        std::string GetName()           const { return m_Name;                 }
        std::string GetEmail()          const { return m_Email;                }
        std::string GetFigure()         const { return m_Figure;               }
        std::string GetBirthday()       const { return m_Birthday;             }
        std::string GetPhoneNumber()    const { return m_PhoneNumber;          }
        std::string GetMotto()          const { return m_Motto;                }
        std::string GetConsoleMotto()   const { return m_ConsoleMotto;         }
        std::string GetGender()         const { return m_Gender;               }
        std::string GetCountry()        const { return m_Country;              }
        std::string GetPoolFigure()     const { return m_PoolFigure;           }
        std::string GetCurrentBadge()   const { return m_Badge->GetCurrentBadgeName(); }

        uint32 GetFilms()               const { return m_Films;                }
        uint32 GetCredits()             const { return m_Credits;              }
        uint32 GetId()                  const { return m_Id;                   }
        uint32 GetTickets()             const { return m_Credits;              }

        uint16 GetDanceId()             const { return m_DanceId;              }
        void SetDanceId(uint16 const p_Id)    { m_DanceId = p_Id;              }

        bool IsInitialized()            const { return m_Initialized;          }
        bool GetReadAgreement()         const { return m_ReadAgreement;        }
        bool GetSpecialRights()         const { return m_SpecialRights;        }
        bool IsSubscribed()             const { return m_HabboClub->IsSubscribed(); }

        ///////////////////////////////////////////
        //             USER OBJECTS
        ///////////////////////////////////////////

        int16 GetPositionX()            const { return m_PositionX;            }
        int16 GetPositionY()            const { return m_PositionY;            }
        int16 GetPositionZ()            const { return m_PositionZ;            }
        int16 GetBodyRotation()         const { return m_BodyRotation;         }
        int16 GetHeadRotation()         const { return m_HeadRotation;         }

        void SetBodyRotation(int16 const l_Rotation) { m_BodyRotation = l_Rotation; }
        void SetHeadRotation(int16 const l_Rotation) { m_HeadRotation = l_Rotation; }

        std::shared_ptr<HabboSocket> ToSocket() { return m_Socket;             }

    private:
        /// SendPing
        /// Send Ping response to client
        void SendPing();

    private:
        /// Variables
        /// This can be in any order
        std::string m_Name;
        std::string m_Password;
        std::string m_Email;
        std::string m_Figure;
        std::string m_Birthday;
        std::string m_PhoneNumber;
        std::string m_Motto;
        std::string m_ConsoleMotto;
        std::string m_Gender;
        std::string m_Country;
        std::string m_PoolFigure;

        bool m_ReadAgreement;
        bool m_SpecialRights;
        bool m_DirectMail;
        bool m_Initialized;
        bool m_SoundEnabled;
        bool m_AcceptFriendRequests;
        bool m_Ponged;

        uint32 m_Id;
        uint32 m_Credits;
        uint32 m_Tickets;
        uint32 m_Films;
        uint32 m_MaxFriendsLimit;
        uint32 m_PingInterval;
        uint32 m_RoomGUID;
        uint32 m_LastCreatedRoomId;     ///< Save Room Id Habbo just created

        int16 m_PositionX;
        int16 m_PositionY;
        int16 m_PositionZ;
        int16 m_BodyRotation;
        int16 m_HeadRotation;
        uint16 m_DanceId;               ///< Save Habbo dance Id to be used for status update

        uint16 m_Rank;

        /// Storages
        std::weak_ptr<Room>            m_Room;
        std::unique_ptr<Messenger>     m_Messenger;
        std::unique_ptr<FuseRights>    m_FuseRight;
        std::unique_ptr<HabboClub>     m_HabboClub;
        std::unique_ptr<FavouriteRoom> m_FavouriteRooms;
        std::unique_ptr<Badge>         m_Badge;
        std::shared_ptr<HabboSocket>   m_Socket;
    };
} ///< NAMESPACE STEERSTONE