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
#include "Hotel.h"
#include "Room.h"
#include "Messenger.h"
#include "FavouriteRoom.h"
#endif /* _HABBO_HABBO_h */

enum LogoutReason
{
    LOGOUT_DISCONNECTED         = -1,
    LOGGED_OUT                  = 1,
    LOGOUT_CONCURRENT           = 2,
    LOGOUT_TIMEOUT              = 3,
};

namespace SteerStone
{
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

    class Habbo
    {
    public:
        friend class HabboSocket;

    public:
        Habbo(HabboSocket* p_HabboSocket);
        ~Habbo();

    public:
        /// Rooms
        bool SetRoom(std::shared_ptr<Room> room);
        std::shared_ptr<Room> GetRoom() const;

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
        bool IsPonged() const;

        /// Socket
        std::shared_ptr<HabboSocket> ToSocket();

    public:
        /// Habbo Info
        std::string GetName() const;
        std::string GetEmail() const;
        std::string GetFigure() const;
        std::string GetBirthday() const;
        std::string GetPhoneNumber() const;
        std::string GetMotto() const;
        std::string GetConsoleMotto() const;
        std::string GetGender() const;
        std::string GetCountry() const;
        std::string GetPoolFigure() const;

        uint32 GetFilms() const;
        uint32 GetCredits() const;
        uint32 GetId() const;
        uint32 GetTickets() const;

        int32 GetPositionX() const;
        int32 GetPositionY() const;
        int32 GetPositionZ() const;
        int32 GetOrientation() const;

        uint8 GetRank() const;

        bool CanSendMail() const;
        bool IsInitialized() const;
        bool GetReadAgreement() const;
        bool GetSpecialRights() const;
        bool IsSoundEnabled() const;
        bool AcceptsFriendRequests() const;

    private:
        void SendPing();

    private:
        /// Habbo Info
        std::string m_Name;
        std::string m_Password;
        std::string m_Email;
        std::string m_Figure;
        std::string m_Birthday;
        std::string mPhoneNumber;
        std::string m_Motto;
        std::string m_ConsoleMotto;
        std::string m_Gender;
        std::string mCountry;
        std::string m_PoolFigure;

        bool mReadAgreement;
        bool mSpecialRights;
        bool m_DirectMail;
        bool m_Initialized;
        bool m_SoundEnabled;
        bool m_AcceptFriendRequests;
        bool mPonged;

        uint32 m_Id;
        uint32 m_Credits;
        uint32 m_Tickets;
        uint32 m_Films;
        uint32 mMaxFriendsLimit;
        uint32 mPingInterval;
        uint32 mUpdateAccount;

        int32 mPositionX;
        int32 mPositionY;
        int32 mPositionZ;
        int32 mOrientation;

        uint8 m_Rank;

        /// Variables
        std::shared_ptr<Room> mRoom;
        std::vector<HabboFuseRightsData> mFuseRights;
        std::vector<HabboBadgesData> mBadges;
        std::unique_ptr<Messenger> m_Messenger;
        std::unique_ptr<FavouriteRoom> m_FavouriteRooms;
        std::shared_ptr<HabboSocket> m_Socket;
        std::mutex mMutex;
    };
} ///< NAMESPACE STEERSTONE