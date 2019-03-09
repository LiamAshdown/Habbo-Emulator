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
#include "Network/StringBuffer.h"
#include "PlayerSocket.h"
//-----------------------------------------------//
namespace Quad
{
    //-----------------------------------------------//
    Player::Player(PlayerSocket* playerSocket)
        : mSocket(playerSocket ? playerSocket->Shared<PlayerSocket>() : nullptr)
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
    void Player::SendObjectData()
    {
        StringBuffer buffer("# USEROBJECT");
        buffer.AppendCarriage();
        buffer << "name=";
        buffer << (std::string)GetName();
        buffer.AppendCarriage();
        buffer << "email=";
        buffer << (std::string)GetEmail();
        buffer.AppendCarriage();
        buffer << "figure=";
        buffer << (std::string)GetFigure();
        buffer.AppendCarriage();
        buffer << "birthday=";
        buffer << (std::string)GetBirthday();
        buffer.AppendCarriage();
        buffer << "phonenumber=";
        buffer << (std::string)GetPhoneNumber();
        buffer.AppendCarriage();
        buffer << "custombuffer=";
        buffer << (std::string)GetMission();
        buffer.AppendCarriage();
        buffer << "had_read_agreement=";
        buffer << (uint8)GetReadAgreement();
        buffer.AppendCarriage();
        buffer << "sex=";
        buffer << (std::string)GetSex();
        buffer.AppendCarriage();
        buffer << "country=";
        buffer << (std::string)GetCountry();
        buffer.AppendCarriage();
        buffer << "has_special_rights=";
        buffer << (uint8)0;
        buffer.AppendCarriage();
        buffer << "badge_type=";
        buffer << (uint32)GetBadgeType();
        buffer.AppendCarriage();
        buffer.AppendEndCarriage();
        mSocket->SendPacket((char*)buffer.GetContents(), buffer.GetSize());
    }
    //-----------------------------------------------//
    std::shared_ptr<PlayerSocket> Player::ToSocket()
    {
        return mSocket;
    }
    //-----------------------------------------------//
}
//-----------------------------------------------//