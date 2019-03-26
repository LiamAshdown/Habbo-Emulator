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
        HabboBadgesStruct() : m_Badge(""), m_IsActive(false) {}
        ~HabboBadgesStruct() {}

    public:
        std::string GetBadge() const { return m_Badge;    }
        bool IsActive()        const { return m_IsActive; }

    private:
        std::string m_Badge;
        bool m_IsActive;
    }HabboBadgesData;

    /// Holds Habbo Club Subscription
    typedef struct HabboClubSubscriptionStruct
    {
    public:
        friend class HabboSocket;

    public:
        HabboClubSubscriptionStruct() : m_DaysRemaining(0), m_RemainingPeriods(0), m_DaysPassed(0),
        m_LastChecked(0) {}
        ~HabboClubSubscriptionStruct() {}

    private:
        std::string m_LastChecked;
        uint32 m_DaysRemaining;
        uint32 m_RemainingPeriods;
        uint32 m_DaysPassed;
    }HabboClubSubscriptionData;

    /// Holds Habbo Fuse Rights
    typedef struct HabboFuseRightsStruct
    {
    public:
        friend class HabboSocket;

    public:
        HabboFuseRightsStruct() : m_Rank(0), m_FuseRight("") {}
        ~HabboFuseRightsStruct() {}

    public:
        uint8 GetRank()            const { return m_Rank;      }
        std::string GetFuseRight() const { return m_FuseRight; }

    private:
        uint8 m_Rank;
        std::string m_FuseRight;
    }HabboFuseRightsData;

    class Room;
    class ClientPacket;

    class Habbo
    {
    public:
        friend class HabboSocket;

    public:
        Habbo(HabboSocket* p_HabboSocket);
        ~Habbo();

    public:
        /// Rooms
        bool SetRoom(std::shared_ptr<Room> p_Room);
        std::shared_ptr<Room> GetRoom() const;
        void DestroyRoom();

        /// Favourite Rooms
        void SendFavouriteRooms();
        void AddFavouriteRoom(bool const& p_IsPublic, uint32 const& p_RoomId);
        void RemoveFavouriteRoom(uint32 const& p_RoomId);
        void SaveFavouriteRoomToDB();

        /// Messenger
        void SendInitializeMessenger();
        void MessengerAcceptRequest(uint32 const& p_SenderId);
        void SendMessengerUpdate();
        void SendSearchUserResults(std::string const& p_Name);
        void MessengerSendFriendRequest(std::string const& p_Name);
        void MessengerRemoveFriend(std::unique_ptr<ClientPacket> p_Packet, uint32 const& p_Size);

        /// Objects
        void SendHabboObject();

        /// Account Info
        void SendAccountPreferences();
        void SendAccountBadges();
        void SendFuseRights();
        void SendClubStatus();

        /// Habbo Info
        void LoadHabboInfo();
        void UpdatePosition(const int32& x, const int32& y, const int32& z, const int32& orientation);
        bool Update(const uint32& diff);
        void Logout(LogoutReason const p_Reason = LOGGED_OUT);

    public:
        /// Habbo Info
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

        int32 GetPositionX()            const { return m_PositionX;            }
        int32 GetPositionY()            const { return m_PositionY;            }
        int32 GetPositionZ()            const { return m_PositionZ;            }
        int32 GetOrientation()          const { return m_Orientation;          }

        uint8 GetRank()                 const { return m_Rank;                 }

        bool CanSendMail()              const { return m_DirectMail;           }
        bool IsInitialized()            const { return m_Initialized;          }
        bool GetReadAgreement()         const { return m_ReadAgreement;        }
        bool GetSpecialRights()         const { return m_SpecialRights;        }
        bool IsSoundEnabled()           const { return m_SoundEnabled;         }
        bool AcceptsFriendRequests()    const { return m_AcceptFriendRequests; }
        bool IsPonged()                 const { return m_Ponged;               }

        std::shared_ptr<HabboSocket> ToSocket() { return m_Socket; }

    private:
        /// SendPing - Send Ping response to client
        void SendPing();

    private:
        /// Habbo Info
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
        uint32 m_UpdateAccount;

        int32 m_PositionX;
        int32 m_PositionY;
        int32 m_PositionZ;
        int32 m_Orientation;

        uint8 m_Rank;

        /// Variables
        std::weak_ptr<Room> m_Room;
        std::vector<HabboFuseRightsData> m_FuseRights;
        std::vector<HabboBadgesData> m_Badges;
        std::unique_ptr<Messenger> m_Messenger;
        std::unique_ptr<FavouriteRoom> m_FavouriteRooms;
        std::shared_ptr<HabboSocket> m_Socket;
        std::mutex m_Mutex;
    };
} ///< NAMESPACE STEERSTONE