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
#include "World.h"
#include "Database/QueryDatabase.h"
//-----------------------------------------------//
namespace Quad
{
    //-----------------------------------------------//
    Player::Player(PlayerSocket* playerSocket)
        : mSocket(playerSocket ? playerSocket->Shared<PlayerSocket>() : nullptr)
    {
        mPingInterval = sConfig->GetIntDefault("PongInterval", 30000);
        mUpdateAccount = sConfig->GetIntDefault("PlayerAccountUpdate", 600000);
        mMaxFriendsLimit = sConfig->GetIntDefault("MaxFriendsLimit", 50);

        mPonged = true;
    }
    //-----------------------------------------------//
    Player::~Player()
    {
        mMessenger.reset();
        mFavouriteRooms.reset();

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
    int32 Player::GetPositionX() const
    {
        return mPositionX;
    }
    //-----------------------------------------------//
    int32 Player::GetPositionY() const
    {
        return mPositionY;
    }
    //-----------------------------------------------//
    int32 Player::GetPositionZ() const
    {
        return mPositionX;
    }
    //-----------------------------------------------//
    int32 Player::GetOrientation() const
    {
        return mOrientation;
    }
    //-----------------------------------------------//
    std::string Player::GetName() const
    {
        return mName;
    }
    //-----------------------------------------------//
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
        ToSocket()->SendPacket(buffer);
    }
    //-----------------------------------------------//
    StringBuffer& Player::GetUserRoomObject()
    {
        StringBuffer buffer;
        buffer.AppendBase64(PacketServerHeader::SERVER_ROOM_USER_OBJECTS);
        buffer.AppendString("i:", false);
        buffer.AppendString("66470", false);
        buffer.AppendString("\r", false);

        buffer.AppendString("a:", false);
        buffer.AppendString(std::to_string(GetId()), false);
        buffer.AppendString("\r", false);

        buffer.AppendString("n:", false);
        buffer.AppendString(GetName(), false);
        buffer.AppendString("\r", false);

        buffer.AppendString("f:", false);
        buffer.AppendString(GetFigure(), false);
        buffer.AppendString("\r", false);

        buffer.AppendString("s:", false);
        buffer.AppendString(GetGender() == "Male" ? "M" : "F", false);
        buffer.AppendString("\r", false);

        buffer.AppendString("l:", false);
        buffer.AppendString(std::to_string(GetPositionX()), false);
        buffer.AppendString(" ", false);
        buffer.AppendString(std::to_string(GetPositionY()), false);
        buffer.AppendString(" ", false);
        buffer.AppendString(std::to_string(GetPositionZ()), false);
        buffer.AppendString("\r", false);

        if (GetMotto().length() > 0)
        {
            buffer.AppendString("c:", false);
            buffer.AppendString(GetMotto(), false);
            buffer.AppendString("\r", false);
        }

        buffer.AppendString("b:", false);
        buffer.AppendSOH();

        uint8* ptr = &buffer.mStorage[0];
        std::cout << "test" << std::endl;
        return buffer;
    }
    //-----------------------------------------------//
    void Player::SendAccountPreferences()
    {
        StringBuffer buffer;
        buffer.AppendBase64(PacketServerHeader::SERVER_ACCOUNT_PREFERENCES);
        buffer.AppendWiredBool(IsSoundEnabled());
        buffer.AppendWiredBool(true); // TODO; Tutorial Finished
        buffer.AppendSOH();
        ToSocket()->SendPacket(buffer);
    }
    //-----------------------------------------------//
    void Player::SendAccountBadges()
    {
        StringBuffer buffer;
        buffer.AppendBase64(PacketServerHeader::SERVER_AVAILABLE_BADGES);
        buffer.AppendWired(mBadges.size());

        uint8 badgesActive = 0;
        for (std::vector<PlayerBadgesData>::const_iterator& itr = mBadges.begin(); itr != mBadges.end(); itr++)
        {
            PlayerBadgesStruct playerBadge = (*itr);

            buffer.AppendString(playerBadge.GetBadge());
            if (playerBadge.IsActive())
                ++badgesActive;
        }

        buffer.AppendWired(badgesActive);
        buffer.AppendWiredBool(mBadges.empty() ? false : true);
        buffer.AppendSOH();
        ToSocket()->SendPacket(buffer);
    }
    //-----------------------------------------------//
    void Player::SendMessengerUpdate()
    {
        StringBuffer buffer;
        buffer.AppendBase64(PacketServerHeader::SERVER_MESSENGER_UPDATE);
        mMessenger->ParseMessengerUpdate(buffer);
        buffer.AppendSOH();
        ToSocket()->SendPacket(buffer);
    }
    //-----------------------------------------------//
    void Player::SendFuseRights()
    {
        StringBuffer buffer;
        buffer.AppendBase64(PacketServerHeader::SERVER_FUSE_RIGHTS);
        for (std::vector<PlayerFuseRightsData>::const_iterator& itr = mFuseRights.begin(); itr != mFuseRights.end(); itr++)
            buffer.AppendString(itr->GetFuseRight());

        buffer.AppendSOH();
        ToSocket()->SendPacket(buffer);
    }
    //-----------------------------------------------//
    void Player::SendFavouriteRooms()
    {
        StringBuffer buffer;
        buffer.AppendBase64(PacketServerHeader::SERVER_FAVOURITE_ROOMS_RESULT);
        mFavouriteRooms->ParseSendFavouriteRooms(buffer);
        buffer.AppendSOH();
        ToSocket()->SendPacket(buffer);
    }
    //-----------------------------------------------//
    void Player::AddFavouriteRoom(const bool& isPublic, const uint32& roomId)
    {
        mFavouriteRooms->AddFavouriteRoom(isPublic, roomId);
    }
    //-----------------------------------------------//
    void Player::RemoveFavouriteRoom(const uint32& roomId)
    {
        mFavouriteRooms->RemoveFavouriteRoom(roomId);
    }
    //-----------------------------------------------//
    void Player::UpdatePosition(const int32& x, const int32& y, const int32& z, const int32& orientation)
    {
        mPositionX = x;
        mPositionY = y;
        mPositionZ = z;
        mOrientation = orientation;
    }
    //-----------------------------------------------//
    void Player::LoadPlayerData()
    {
        mMessenger = std::make_unique<Messenger>(mId);
        mMessenger->LoadMessenger();

        mFavouriteRooms = std::make_unique<FavouriteRoom>(mId);
        mFavouriteRooms->LoadFavouriteRooms();
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
        ToSocket()->SendPacket(buffer);

        buffer.Clear();

        buffer.AppendBase64(PacketServerHeader::SERVER_FRIEND_REQUEST);
        mMessenger->ParseMessengerFriendRequests(buffer);
        buffer.AppendSOH();
        ToSocket()->SendPacket(buffer);
    }
    //-----------------------------------------------//
    void Player::MessengerAcceptRequest(const uint32& senderId)
    {
        StringBuffer buffer;

        mMessenger->AcceptFriendRequest(buffer, senderId);
    }
    //-----------------------------------------------//
    void Player::SendClubStatus()
    {
    }
    //-----------------------------------------------//
    bool Player::IsPonged() const
    {
        return mPonged;
    }
    //-----------------------------------------------//
    bool Player::Update(const uint32& diff)
    {
        if (mSocket && !mSocket->IsClosed())
        {
            if (mPingInterval < diff)
            {
                if (IsPonged())
                {
                    SendPing();
                    mPingInterval = sConfig->GetIntDefault("PongInterval", 30000);
                }
                else
                {
                    LOG_INFO << "Disconnecting Player: " << GetId() << " have not recieved a pong back";
                    return false;
                }
            }
            else
                mPingInterval -= diff;

            if (mUpdateAccount < diff)
            {
                QueryDatabase database("users");
                database.PrepareQuery("UPDATE account SET email = ?, figure = ?, pool_figure = ?, motto = ?, console_motto = ?, birthday = ?, gender = ?, credits = ?, tickets = ?, films = ?, sound_enabled = ?");
                database.GetStatement()->setString(1, GetEmail());
                database.GetStatement()->setString(2, GetFigure());
                database.GetStatement()->setString(3, GetPoolFigure());
                database.GetStatement()->setString(4, GetMotto());
                database.GetStatement()->setString(5, GetConsoleMotto());
                database.GetStatement()->setString(6, GetBirthday());
                database.GetStatement()->setString(7, GetGender());
                database.GetStatement()->setUInt(8, GetCredits());
                database.GetStatement()->setUInt(9, GetTickets());
                database.GetStatement()->setUInt(10, GetFilms());
                database.GetStatement()->setBoolean(11, IsSoundEnabled());
                database.ExecuteQuery();

                mUpdateAccount = sConfig->GetIntDefault("PlayerAccountUpdate", 600000);
            }
            else 
                mUpdateAccount -= diff;

            return true;
        }
        else
            return false;
    }
    //-----------------------------------------------//
    void Player::UpdateFavouriteRooms()
    {
        mFavouriteRooms->UpdateFavouriteRooms();
    }
    //-----------------------------------------------//
    void Player::Logout()
    {
        UpdateFavouriteRooms();
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
        ToSocket()->SendPacket(buffer);
    }
    //-----------------------------------------------//
}
//-----------------------------------------------//