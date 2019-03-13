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
        uint32 GetBadgeType() const;
        uint32 GetId() const;
        uint32 GetTickets() const;

        bool CanSendMail() const;
        bool IsInitialized() const;
        bool GetReadAgreement() const;
        bool GetSpecialRights() const;

        void SetRoom(std::shared_ptr<Room> room);
        std::shared_ptr<Room> GetRoom() const;

        void SendUserObject();
        void UpdatePosition(const uint16& x, const uint16& y, const uint16& z, const uint16& orientation);

        std::shared_ptr<PlayerSocket> ToSocket();

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

        uint32 mId;
        uint32 mCredits;
        uint32 mBadgeType;
        uint32 mTickets;
        uint32 mFilms;

        uint16 mPositionX;
        uint16 mPositionY;
        uint16 mPositionZ;
        uint16 mOrientation;

        std::shared_ptr<Room> mRoom;
        std::shared_ptr<PlayerSocket> mSocket;
    };
}

#endif /* _Quad_Player_h_ */