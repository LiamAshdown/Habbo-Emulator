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
#include "Player.h"
//-----------------------------------------------//
namespace Quad
{
    //-----------------------------------------------//
    Player::Player()
    {
    }
    Player::~Player()
    {
        mRoom.reset();
    }
    //-----------------------------------------------//
    uint32 Player::GetId() const
    {
        return mId;
    }
    //-----------------------------------------------//
    std::string Player::GetName() const
    {
        return mName;
    }
    uint32 Player::GetCredits() const
    {
        return mCredits;
    }
    //-----------------------------------------------//
    std::string Player::GetEmail() const
    {
        return mEmail;
    }
    //-----------------------------------------------//
    std::string Player::GetFigure() const
    {
        return mFigure;
    }
    //-----------------------------------------------//
    std::string Player::GetBirthday() const
    {
        return mBirthday;
    }
    //-----------------------------------------------//
    std::string Player::GetPhoneNumber() const
    {
        return mPhoneNumber;
    }
    //-----------------------------------------------//
    std::string Player::GetMission() const
    {
        return mMission;
    }
    //-----------------------------------------------//
    bool Player::GetReadAgreement() const
    {
        return mReadAgreement;
    }
    //-----------------------------------------------//
    std::string Player::GetSex() const
    {
        return mSex;
    }
    //-----------------------------------------------//
    std::string Player::GetCountry() const
    {
        return mCountry;
    }
    //-----------------------------------------------//
    bool Player::GetSpecialRights() const
    {
        return mSpecialRights;
    }
    //-----------------------------------------------//
    uint32 Player::GetBadgeType() const
    {
        return mBadgeType;
    }
    //-----------------------------------------------//
    bool Player::CanSendMail() const
    {
        return mDirectMail;
    }
    //-----------------------------------------------//
    bool Player::IsInitialized() const
    {
        return mInitialized;
    }
    //-----------------------------------------------//
    void Player::SetRoom(std::shared_ptr<Room> room)
    {
        mRoom.reset(new Room);

        mRoom = room;
    }
    //-----------------------------------------------//
    std::shared_ptr<Room> Player::GetRoom() const
    {
        return mRoom;
    }
    //-----------------------------------------------//
}
//-----------------------------------------------//