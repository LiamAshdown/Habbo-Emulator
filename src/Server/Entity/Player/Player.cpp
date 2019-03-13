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
        IF_LOG(plog::debug)
            LOG_DEBUG << "Destructor Player called!";
    }
    //-----------------------------------------------//
    uint32 Player::GetId() const
    {
        return mId;
    }
    //-----------------------------------------------//
    uint32 Player::GetTickets() const
    {
        return mTickets;
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
    std::string Player::GetMotto() const
    {
        return mMotto;
    }
    //-----------------------------------------------//
    std::string Player::GetConsoleMotto() const
    {
        return mConsoleMotto;
    }
    //-----------------------------------------------//
    bool Player::GetReadAgreement() const
    {
        return mReadAgreement;
    }
    //-----------------------------------------------//
    std::string Player::GetGender() const
    {
        return mGender;
    }
    //-----------------------------------------------//
    std::string Player::GetCountry() const
    {
        return mCountry;
    }
    //-----------------------------------------------//
    std::string Player::GetPoolFigure() const
    {
        return mPoolFigure;
    }
    //-----------------------------------------------//
    uint32 Player::GetFilms() const
    {
        return mFilms;
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
    void Player::SendUserObject()
    {
        StringBuffer buffer;
        buffer.AppendBase64(OpcodesServer::SMSG_USER_OBJECT);
        buffer.AppendString(std::to_string(mId));
        buffer.AppendString(mName);
        buffer.AppendString(mFigure);
        buffer.AppendString(mGender);
        buffer.AppendString(mMotto);
        buffer.AppendWired(mTickets);
        buffer.AppendString(mPoolFigure);
        buffer.AppendWired(mFilms);
        buffer.AppendSOH();

        ToSocket()->SendPacket((const char*)buffer.GetContents(), buffer.GetSize());
    }
    //-----------------------------------------------//
    void Player::UpdatePosition(const uint16 & x, const uint16 & y, const uint16 & z, const uint16 & orientation)
    {
        mPositionX = x;
        mPositionY = y;
        mPositionZ = z;
        mOrientation = orientation;
    }
    //-----------------------------------------------//
    std::shared_ptr<PlayerSocket> Player::ToSocket()
    {
        return mSocket;
    }
    //-----------------------------------------------//
}
//-----------------------------------------------//