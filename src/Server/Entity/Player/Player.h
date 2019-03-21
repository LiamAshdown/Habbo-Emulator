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

#ifndef _Quad_Player_h_
#define _Quad_Player_h_
#include "Common/SharedDefines.h"
#include "Room.h"
#include "Messenger.h"
#include "FavouriteRoom.h"
#include "Common/Timer.h"
#include <mutex>

namespace Quad
{
    typedef struct PlayerBadgesStruct
    {
    public:
        friend class PlayerSocket;

    public:
        PlayerBadgesStruct() : mBadge(""), mIsActive(false) {}
        ~PlayerBadgesStruct() {}

    public:
        std::string GetBadge() const { return mBadge; }
        bool IsActive() const { return mIsActive; }

    private:
        std::string mBadge;
        bool mIsActive;
    }PlayerBadgesData;

    typedef struct PlayerClubSubscriptionStruct
    {
    public:
        friend class PlayerSocket;

    public:
        PlayerClubSubscriptionStruct() : mDaysRemaining(0), mRemainingPeriods(0), mDaysPassed(0),
        mLastChecked(0) {}
        ~PlayerClubSubscriptionStruct() {}

    private:
        std::string mLastChecked;
        uint32 mDaysRemaining;
        uint32 mRemainingPeriods;
        uint32 mDaysPassed;

    }PlayerClubSubscriptionData;

    typedef struct PlayerFuseRightsStruct
    {
    public:
        friend class PlayerSocket;

    public:
        PlayerFuseRightsStruct() : mRank(0), mFuseRight("") {}
        ~PlayerFuseRightsStruct() {}

    public:
        uint8 GetRank() const { return mRank; }
        std::string GetFuseRight() const { return mFuseRight; }

    private:
        uint8 mRank;
        std::string mFuseRight;
    }PlayerFuseRightsData;

    class Player
    {
    public:
        friend class PlayerSocket;

    public:
        Player(PlayerSocket* playerSocket);
        ~Player();

    public:
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

        bool CanSendMail() const;
        bool IsInitialized() const;
        bool GetReadAgreement() const;
        bool GetSpecialRights() const;
        bool IsSoundEnabled() const;

        bool SetRoom(std::shared_ptr<Room> room);
        std::shared_ptr<Room> GetRoom() const;

        void SendUserObject();
        StringBuffer& GetUserRoomObject();
        void SendAccountPreferences();
        void SendAccountBadges();
        void SendMessengerUpdate();
        void SendFuseRights();

        // Favourite Rooms
        void SendFavouriteRooms();
        void AddFavouriteRoom(const bool& isPublic, const uint32& roomId);
        void RemoveFavouriteRoom(const uint32& roomId);

        void UpdatePosition(const int32& x, const int32& y, const int32& z, const int32& orientation);

        void LoadPlayerData();

        void SendInitializeMessenger();
        void MessengerAcceptRequest(const uint32& senderId);

        void SendClubStatus();

        bool IsPonged() const;

        bool Update(const uint32& diff);
        void UpdateFavouriteRooms();

        void Logout();

        std::shared_ptr<PlayerSocket> ToSocket();

    private:
        void SendPing();

    private:
        std::string mName;
        std::string mPassword;
        std::string mEmail;
        std::string mFigure;
        std::string mBirthday;
        std::string mPhoneNumber;
        std::string mMotto;
        std::string mConsoleMotto;
        std::string mGender;
        std::string mCountry;
        std::string mPoolFigure;

        bool mReadAgreement;
        bool mSpecialRights;
        bool mDirectMail;
        bool mInitialized;
        bool mSoundEnabled;

        uint32 mId;
        uint32 mCredits;
        uint32 mTickets;
        uint32 mFilms;
        uint32 mMaxFriendsLimit;

        int32 mPositionX;
        int32 mPositionY;
        int32 mPositionZ;
        int32 mOrientation;

        std::shared_ptr<PlayerSocket> mSocket;

        std::shared_ptr<Room> mRoom;
        std::vector<PlayerFuseRightsData> mFuseRights;
        std::vector<PlayerBadgesData> mBadges;
        std::unique_ptr<Messenger> mMessenger;
        std::unique_ptr<FavouriteRoom> mFavouriteRooms;

        uint32 mPingInterval;
        uint32 mUpdateAccount;
        bool mPonged;
        std::mutex mMutex;
    };
}

#endif /* _Quad_Player_h_ */