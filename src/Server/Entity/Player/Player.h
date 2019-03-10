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

namespace Quad
{
    class Player
    {
    public:
        friend class PlayerSocket;

    public:
        Player(PlayerSocket* playerSocket);
        ~Player();

    public:
        uint32 GetId() const;
        std::string GetName() const;
        uint32 GetCredits() const;
        std::string GetEmail() const;
        std::string GetFigure() const;
        std::string GetBirthday() const;
        std::string GetPhoneNumber() const;
        std::string GetMission() const;
        bool GetReadAgreement() const;
        std::string GetSex() const;
        std::string GetCountry() const;
        bool GetSpecialRights() const;
        uint32 GetBadgeType() const;
        bool CanSendMail() const;
        bool IsInitialized() const;

        void SetRoom(std::shared_ptr<Room> room);
        std::shared_ptr<Room> GetRoom() const;

        void SendObjectData();
        void UpdatePosition(const uint16& x, const uint16& y, const uint16& z, const uint16& orientation);

        std::shared_ptr<PlayerSocket> ToSocket();

    private:
        uint32 mId;
        std::string mName;
        std::string mPassword;
        uint32 mCredits;
        std::string mEmail;
        std::string mFigure;
        std::string mBirthday;
        std::string mPhoneNumber;
        std::string mMission;
        bool mReadAgreement;
        std::string mSex;
        std::string mCountry;
        bool mSpecialRights;
        uint32 mBadgeType;
        bool mDirectMail;
        bool mInitialized;

        uint16 mPositionX;
        uint16 mPositionY;
        uint16 mPositionZ;
        uint16 mOrientation;

        std::shared_ptr<Room> mRoom;

        std::shared_ptr<PlayerSocket> mSocket;
    };
}

#endif /* _Quad_Player_h_ */