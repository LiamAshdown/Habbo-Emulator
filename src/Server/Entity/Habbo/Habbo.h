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

#ifndef _HABBO_HABBO_h
#define _HABBO_HABBO_h
#include "Common/SharedDefines.h"
#include "Messenger.h"
#include "FavouriteRoom.h"
#include <mutex>
#endif /* _HABBO_HABBO_h */

namespace SteerStone
{
    enum LogoutReason
    {
        LOGOUT_DISCONNECTED         = -1,
        LOGGED_OUT                  = 1,
        LOGOUT_CONCURRENT           = 2,
        LOGOUT_TIMEOUT              = 3,
    };

    /// Holds Habbo Badges
    typedef struct HabboBadgesStruct
    {
    public:
        friend class HabboSocket;

    public:
        /// Constructor
        HabboBadgesStruct() : m_Badge(""), m_IsActive(false) {}

        /// Deconstructor
        ~HabboBadgesStruct() {}

    public:
        std::string GetBadge() const { return m_Badge;    }
        bool IsActive()        const { return m_IsActive; }

    private:
        std::string m_Badge;                ///< Badge Name
        bool m_IsActive;                    ///< Is badge active?
    }HabboBadgesData;

    /// Holds Habbo Club Subscription
    typedef struct HabboClubSubscriptionStruct
    {
    public:
        friend class HabboSocket;

    public:
        /// Constructor
        HabboClubSubscriptionStruct() : m_DaysRemaining(0), m_RemainingPeriods(0), m_DaysPassed(0),
        m_LastChecked(0) {}

        /// Deconstructor
        ~HabboClubSubscriptionStruct() {}

    private:
        std::string m_LastChecked;              ///< Last time server checked if user should have habbo club
        uint32 m_DaysRemaining;                 ///< Days remaining till habbo club expires
        uint32 m_RemainingPeriods;              ///< How many periods (months) till habbo club exprires
        uint32 m_DaysPassed;                    ///< Days passed since user has habbo club
    }HabboClubSubscriptionData;

    /// Holds Habbo Fuse Rights
    typedef struct HabboFuseRightsStruct
    {
    public:
        friend class HabboSocket;

    public:
        /// Constructor
        HabboFuseRightsStruct() : m_Rank(0), m_FuseRight("") {}

        /// Deconstructor
        ~HabboFuseRightsStruct() {}

    public:
        uint8 GetRank()            const { return m_Rank;      }
        std::string GetFuseRight() const { return m_FuseRight; }

    private:
        uint8 m_Rank;                       ///< Rank of fuse right
        std::string m_FuseRight;            ///< Fuse right string (accounts.rank_fuserights)
    }HabboFuseRightsData;

    class Room;
    class ClientPacket;

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
        bool SetRoom(std::shared_ptr<Room> p_Room);

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

        /// MessengerSendFriendRequest
        /// @p_Name - Name of friend user is sending request for
        void MessengerSendFriendRequest(std::string const p_Name);

        /// MessengerRemoveFriend
        /// @p_Packet - Client packet which is being decoded
        void MessengerRemoveFriend(std::unique_ptr<ClientPacket> p_Packet);

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
        void SendUpdateStatusWalk(int16 const p_X, int16 const p_Y, int16 const p_Z, bool p_SendToRoom = true);

        /// SendUpdateStatusStop
        /// Send Status stop when user finishes path
        void SendUpdateStatusStop(bool p_SendToRoom = true);

        /// SendUpdateStatusSit
        /// @p_X - X Axis current position
        /// @p_Y - Y Axis current position
        /// @p_Z - Z Axis current position
        void SendUpdateStatusSit(int16 const p_X, int16 const p_Y, int16 const p_Z, int16 const p_Rotation, bool p_SendToRoom = true);

        /// GetStatus
        /// Get current status on what user is doing
        std::string GetStatus();

        /// SendUpdateStatusWalk
        /// @p_X - X axis on new position
        /// @p_Y - Y axis on new position
        /// @p_Z - Z axis on new position
        /// @p_Rotation - New Rotation
        void UpdatePosition(int16 const p_X, int16 const p_Y, int16 const p_Z, int16 const p_Rotation);

        ///////////////////////////////////////////
        //             ACCOUNT INFO
        ///////////////////////////////////////////

        /// SendAccountPreferences
        /// Send user account preferences (set from users.account database)
        void SendAccountPreferences();

        /// SendAccountBadges
        /// Send user account badges (set from users.account_badges database)
        void SendAccountBadges();

        /// SendFuseRights
        /// Send user account rights (set from users.rank_fuserights database)
        void SendFuseRights();

        /// SendClubStatus
        /// Send user account club status (set from users.club_subscriptions database)
        void SendClubStatus();

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
        uint8 GetRank()                 const { return m_Rank;                 }

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

        uint32 GetFilms()               const { return m_Films;                }
        uint32 GetCredits()             const { return m_Credits;              }
        uint32 GetId()                  const { return m_Id;                   }
        uint32 GetTickets()             const { return m_Credits;              }

        bool IsInitialized()            const { return m_Initialized;          }
        bool GetReadAgreement()         const { return m_ReadAgreement;        }
        bool GetSpecialRights()         const { return m_SpecialRights;        }

        ///////////////////////////////////////////
        //             USER OBJECTS
        ///////////////////////////////////////////

        int16 GetPositionX()            const { return m_PositionX;            }
        int16 GetPositionY()            const { return m_PositionY;            }
        int16 GetPositionZ()            const { return m_PositionZ;            }
        uint8 GetBodyRotation()         const { return m_BodyRotation;         }
        uint8 GetHeadRotation()         const { return m_HeadRotation;         }

        void SetIsWalking(bool const p_Walking) { m_Walking = p_Walking;       }
        bool IsWalking()                const { return m_Walking;              }

        void SetIsSitting(bool const p_Sitting) { m_Sitting = p_Sitting;       }
        bool IsSitting()                const { return m_Sitting;              }

        std::shared_ptr<HabboSocket> ToSocket() { return m_Socket; }

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
        bool m_Walking;
        bool m_Sitting;

        uint32 m_Id;
        uint32 m_Credits;
        uint32 m_Tickets;
        uint32 m_Films;
        uint32 m_MaxFriendsLimit;
        uint32 m_PingInterval;
        uint32 m_RoomGUID;

        int16 m_PositionX;
        int16 m_PositionY;
        int16 m_PositionZ;
        uint8 m_BodyRotation;
        uint8 m_HeadRotation;

        uint8 m_Rank;

        /// Storages
        std::weak_ptr<Room> m_Room;
        std::vector<HabboFuseRightsData> m_FuseRights;
        std::vector<HabboBadgesData> m_Badges;
        std::unique_ptr<Messenger> m_Messenger;
        std::unique_ptr<FavouriteRoom> m_FavouriteRooms;
        std::shared_ptr<HabboSocket> m_Socket;
        std::mutex m_Mutex;
    };
} ///< NAMESPACE STEERSTONE