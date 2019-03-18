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
#include "Config/Config.h"
#include "Common/Timer.h"
#include "World.h"
//-----------------------------------------------//
namespace Quad
{
    //-----------------------------------------------//
    Player::Player(PlayerSocket* playerSocket)
        : mSocket(playerSocket ? playerSocket->Shared<PlayerSocket>() : nullptr)
    {
        mPingInterval = sConfig->GetIntDefault("PongInterval", 10000);
        mPingTimer.Reset();
        mPonged = true;
    }
    Player::~Player()
    {
        mMessenger.reset();

        if (mSocket)
        {
            if (!mSocket->IsClosed())
                mSocket->CloseSocket();

            mSocket->DestroyPlayer();
        }
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
    bool Player::IsSoundEnabled() const
    {
        return mSoundEnabled;
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
    bool Player::SetRoom(std::shared_ptr<Room> room)
    {
        if (mRoom = room)
            return true;
        else
            return false;
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
        buffer.AppendBase64(PacketServerHeader::SERVER_USER_OBJECT);
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
    void Player::SendAccountPreferences()
    {
        StringBuffer buffer;
        buffer.AppendBase64(PacketServerHeader::SERVER_ACCOUNT_PREFERENCES);
        buffer.AppendWiredBool(IsSoundEnabled());
        buffer.AppendWiredBool(true); // TODO; Tutorial Finished
        buffer.AppendSOH();
        ToSocket()->SendPacket((const char*)buffer.GetContents(), buffer.GetSize());
    }
    //-----------------------------------------------//
    void Player::SendAccountBadges()
    {
        StringBuffer buffer;
        buffer.AppendBase64(PacketServerHeader::SERVER_AVAILABLE_BADGES);
        buffer.AppendWired(mBadges.size());

        uint8 badgesActive = 0;
        for (std::vector<PlayerBadges>::const_iterator& itr = mBadges.begin(); itr != mBadges.end(); itr++)
        {
            PlayerBadgesStruct playerBadge = (*itr);

            buffer.AppendString(playerBadge.GetBadge());
            if (playerBadge.IsActive())
                ++badgesActive;
        }

        buffer.AppendWired(badgesActive);
        buffer.AppendWiredBool(mBadges.empty() ? false : true);
        buffer.AppendSOH();
        ToSocket()->SendPacket((const char*)buffer.GetContents(), buffer.GetSize());
    }
    //-----------------------------------------------//
    void Player::SendMessengerUpdate()
    {
        StringBuffer buffer;
        buffer.AppendBase64(PacketServerHeader::SERVER_MESSENGER_UPDATE);
        mMessenger->ParseMessengerUpdate(buffer);
        buffer.AppendSOH();
        ToSocket()->SendPacket((const char*)buffer.GetContents(), buffer.GetSize());
    }
    //-----------------------------------------------//
    void Player::UpdatePosition(const uint16& x, const uint16& y, const uint16& z, const uint16& orientation)
    {
        mPositionX = x;
        mPositionY = y;
        mPositionZ = z;
        mOrientation = orientation;
    }
    //-----------------------------------------------//
    void Player::LoadMessenger()
    {
        mMessenger = std::make_unique<Messenger>(mId);
        mMessenger->LoadMessenger();
    }
    //-----------------------------------------------//
    void Player::SendInitializeMessenger()
    {
        StringBuffer buffer;
        buffer.AppendBase64(PacketServerHeader::SERVER_MESSENGER_INIT);
        buffer.AppendString(GetConsoleMotto());

        buffer.AppendWired(100);
        buffer.AppendWired(200);
        buffer.AppendWired(600);

        mMessenger->ParseMessengerFriends(buffer);

        buffer.AppendSOH();
        ToSocket()->SendPacket((const char*)buffer.GetContents(), buffer.GetSize());

        buffer.Clear();

        buffer.AppendBase64(PacketServerHeader::SERVER_FRIEND_REQUEST);
        mMessenger->ParseMessengerFriendRequests(buffer);
        buffer.AppendSOH();
        ToSocket()->SendPacket((const char*)buffer.GetContents(), buffer.GetSize());
    }
    //-----------------------------------------------//
    bool Player::IsPonged() const
    {
        return mPonged;
    }
    //-----------------------------------------------//
    bool Player::Update()
    {
        if (mSocket && !mSocket->IsClosed())
        {
            if (mPingTimer.Elasped() > mPingInterval)
            {
                if (IsPonged())
                {
                    SendPing();
                    mPingTimer.Reset();
                }
                else
                {
                    LOG_INFO << "Disconnecting Player: " << GetId() << " have not recieved a pong back";
                    return false;
                }
            }

            return true;
        }
        else
            return false;
    }
    //-----------------------------------------------//
    void Player::Logout()
    {
    }
    //-----------------------------------------------//
    std::shared_ptr<PlayerSocket> Player::ToSocket()
    {
        return mSocket;
    }
    //-----------------------------------------------//
    void Player::SendPing()
    {
        mPonged = false;

        StringBuffer buffer;
        buffer.AppendBase64(PacketServerHeader::SERVER_PING);
        buffer.AppendSOH();
        ToSocket()->SendPacket((const char*)buffer.GetContents(), buffer.GetSize());
    }
    //-----------------------------------------------//
}
//-----------------------------------------------//