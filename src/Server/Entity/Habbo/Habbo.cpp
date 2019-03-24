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

#include "Habbo.h"
#include "Opcode/Packets/Server/HabboInfoPackets.h"
#include "Opcode/Packets/Server/MessengerPackets.h"
#include "Opcode/Packets/Server/FavouriteRoomPackets.h"
#include "Opcode/Packets/Server/HabboInfoPackets.h"
#include "Opcode/Packets/Server/MiscPackets.h"

namespace SteerStone
{
    Habbo::Habbo(HabboSocket* p_HabboSocket)
        : m_Socket(p_HabboSocket ? p_HabboSocket->Shared<HabboSocket>() : nullptr)
    {
        mPingInterval = sConfig->GetIntDefault("PongInterval", 30000);
        mUpdateAccount = sConfig->GetIntDefault("PlayerAccountUpdate", 600000);
        mMaxFriendsLimit = sConfig->GetIntDefault("MaxFriendsLimit", 50);

        SendPing();
    }

    Habbo::~Habbo()
    {
        m_Messenger.reset();
        m_FavouriteRooms.reset();

        if (m_Socket)
        {
            if (!m_Socket->IsClosed())
                m_Socket->CloseSocket();

            m_Socket->DestroyHabbo();
        }
    }

    //////////////////////////////////////////////
    //                ROOMS
    /////////////////////////////////////////////

    bool Habbo::SetRoom(std::shared_ptr<Room> room)
    {
        if (mRoom = room)
            return true;
        else
            return false;
    }

    std::shared_ptr<Room> Habbo::GetRoom() const
    {
        return mRoom;
    }

    //////////////////////////////////////////////
    //            FAVOURITE ROOM
    /////////////////////////////////////////////

    void Habbo::SendFavouriteRooms()
    {
        HabboPacket::FavouriteRoom::FavouriteRoomResult l_Packet;
        m_FavouriteRooms->ParseSendFavouriteRooms(l_Packet.GetBuffer());
        ToSocket()->SendPacket(l_Packet.Write());
    }

    void Habbo::AddFavouriteRoom(bool const& p_IsPublic, uint32 const& p_RoomId)
    {
        m_FavouriteRooms->AddFavouriteRoom(p_IsPublic, p_RoomId);
    }

    void Habbo::RemoveFavouriteRoom(uint32 const& p_RoomId)
    {
        m_FavouriteRooms->RemoveFavouriteRoom(p_RoomId);
    }

    void Habbo::SaveFavouriteRoomToDB()
    {
        m_FavouriteRooms->SaveToDB();
    }

    //////////////////////////////////////////////
    //              MESSENGER
    /////////////////////////////////////////////

    void Habbo::SendInitializeMessenger()
    {
        HabboPacket::Messenger::MessengerInitialize l_Packet;
        l_Packet.ConsoleMotto = GetConsoleMotto();
        l_Packet.FriendsLimit = sConfig->GetIntDefault("MessgengerMaxFriendsLimit", 50);
        l_Packet.ClubFriendsLimit = sConfig->GetIntDefault("MessgengerMaxFriendsClubLimit", 100);
        m_Messenger->ParseMessengerFriends(l_Packet.GetBuffer());
        ToSocket()->SendPacket(l_Packet.Write());

        if (m_Messenger->HasFriendRequest())
        {
            HabboPacket::Messenger::MessengerFriendRequest l_PacketFriendRequest;
            m_Messenger->ParseMessengerFriendRequests(l_PacketFriendRequest.GetBuffer());
            ToSocket()->SendPacket(l_PacketFriendRequest.Write());
        }
    }

    void Habbo::MessengerAcceptRequest(uint32 const& p_SenderId)
    {
        //StringBuffer buffer;
        //buffer.AppendBase64(PacketServerHeader::SERVER_MESSENGER_INIT);
        //m_Messenger->ParseMessengerAcceptRequest(buffer, senderId);
    }

    void Habbo::SendMessengerUpdate()
    {
        HabboPacket::Messenger::MessengerUpdate l_Packet;
        m_Messenger->ParseMessengerUpdate(l_Packet.GetBuffer());
        ToSocket()->SendPacket(l_Packet.Write());

        if (m_Messenger->HasFriendRequest())
        {
            HabboPacket::Messenger::MessengerFriendRequest l_PacketFriendRequest;
            m_Messenger->ParseMessengerFriendRequests(l_PacketFriendRequest.GetBuffer());
            ToSocket()->SendPacket(l_PacketFriendRequest.Write());
        }
    }

    void Habbo::SendSearchUserResults(std::string const& p_Name)
    {
        HabboPacket::Messenger::MessengerFindUserResult l_Packet;
        l_Packet.Messenger = "MESSENGER";
        m_Messenger->ParseMessengerSearchUser(l_Packet.GetBuffer(), p_Name);
        ToSocket()->SendPacket(l_Packet.Write());
    }

    //////////////////////////////////////////////
    //              HABBO INFO
    /////////////////////////////////////////////

    void Habbo::SendHabboObject()
    {
        HabboPacket::HabboInfo::HabboObject l_Packet;
        l_Packet.Id = std::to_string(m_Id);
        l_Packet.Name = m_Name;
        l_Packet.Figure = m_Figure;
        l_Packet.Gender = m_Gender;
        l_Packet.Motto = m_Motto;
        l_Packet.Tickets = m_Tickets;
        l_Packet.PoolFigure = m_PoolFigure;
        l_Packet.Films = m_Films;
        ToSocket()->SendPacket(l_Packet.Write());
    }

    void Habbo::SendAccountPreferences()
    {
        HabboPacket::HabboInfo::AccountPreferences l_Packet;
        l_Packet.SoundEnabled = IsSoundEnabled();
        l_Packet.TutorialFinished = true; ///< TODO
        ToSocket()->SendPacket(l_Packet.Write());
    }

    void Habbo::SendAccountBadges()
    {
        HabboPacket::HabboInfo::AvailableBadges l_Packet;

        for (auto const& itr : mBadges)
        {
            if (itr.IsActive())
                l_Packet.ActiveBadges++;

            l_Packet.Badges.push_back(itr.GetBadge());
        }

        ToSocket()->SendPacket(l_Packet.Write());
    }

    void Habbo::SendFuseRights()
    {
        HabboPacket::HabboInfo::FuseRights l_Packet;
       
        for (auto const& itr : mFuseRights)
            l_Packet.HabboFuseRights.push_back(itr.GetFuseRight());

        ToSocket()->SendPacket(l_Packet.Write());
    }

    void Habbo::SendClubStatus()
    {
        // TODO;
    }

    void Habbo::LoadHabboInfo()
    {
        m_Messenger = std::make_unique<Messenger>(m_Id);
        m_Messenger->LoadMessenger();

        m_FavouriteRooms = std::make_unique<FavouriteRoom>(m_Id);
        m_FavouriteRooms->LoadFavouriteRooms();
    }

    void Habbo::UpdatePosition(const int32& x, const int32& y, const int32& z, const int32& orientation)
    {
        mPositionX = x;
        mPositionY = y;
        mPositionZ = z;
        mOrientation = orientation;
    }

    bool Habbo::Update(const uint32& diff)
    {
        if (m_Socket && !m_Socket->IsClosed())
        {
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

            if (mPingInterval < diff)
            {
                if (IsPonged())
                {
                    SendPing();
                    mPingInterval = sConfig->GetIntDefault("PongInterval", 10000);
                }
                else
                {
                    Logout(LOGOUT_TIMEOUT);
                    LOG_INFO << "Disconnecting Habbo: " << GetId() << " have not recieved a pong back";
                    return false;
                }
            }
            else
                mPingInterval -= diff;

            return true;
        }
        else
            return false;
    }

    void Habbo::Logout(LogoutReason const p_Reason /*= LOGGED_OUT*/)
    {
        SaveFavouriteRoomToDB();

        HabboPacket::Misc::HotelLogout l_Packet;
        l_Packet.Reason = p_Reason;
        ToSocket()->SendPacket(l_Packet.Write());

        if (m_Socket)
        {
            if (!m_Socket->IsClosed())
                m_Socket->CloseSocket();

            m_Socket->DestroyHabbo();
        }
    }

    bool Habbo::IsPonged() const
    {
        return mPonged;
    }

    std::shared_ptr<HabboSocket> Habbo::ToSocket()
    {
        return m_Socket;
    }

    void Habbo::SendPing()
    {
        mPonged = false;
        HabboPacket::Misc::Ping l_Packet;
        ToSocket()->SendPacket(l_Packet.Write());
    }

    uint32 Habbo::GetId() const
    {
        return m_Id;
    }

    uint32 Habbo::GetTickets() const
    {
        return m_Tickets;
    }

    int32 Habbo::GetPositionX() const
    {
        return mPositionX;
    }

    int32 Habbo::GetPositionY() const
    {
        return mPositionY;
    }

    int32 Habbo::GetPositionZ() const
    {
        return mPositionX;
    }

    int32 Habbo::GetOrientation() const
    {
        return mOrientation;
    }

    uint8 Habbo::GetRank() const
    {
        return m_Rank;
    }

    std::string Habbo::GetName() const
    {
        return m_Name;
    }

    uint32 Habbo::GetCredits() const
    {
        return m_Credits;
    }

    std::string Habbo::GetEmail() const
    {
        return m_Email;
    }

    std::string Habbo::GetFigure() const
    {
        return m_Figure;
    }

    std::string Habbo::GetBirthday() const
    {
        return m_Birthday;
    }

    std::string Habbo::GetPhoneNumber() const
    {
        return mPhoneNumber;
    }

    std::string Habbo::GetMotto() const
    {
        return m_Motto;
    }

    std::string Habbo::GetConsoleMotto() const
    {
        return m_ConsoleMotto;
    }

    bool Habbo::GetReadAgreement() const
    {
        return mReadAgreement;
    }

    std::string Habbo::GetGender() const
    {
        return m_Gender;
    }

    std::string Habbo::GetCountry() const
    {
        return mCountry;
    }

    std::string Habbo::GetPoolFigure() const
    {
        return m_PoolFigure;
    }

    uint32 Habbo::GetFilms() const
    {
        return m_Films;
    }

    bool Habbo::GetSpecialRights() const
    {
        return mSpecialRights;
    }

    bool Habbo::IsSoundEnabled() const
    {
        return m_SoundEnabled;
    }

    bool Habbo::AcceptsFriendRequests() const
    {
        return m_AcceptFriendRequests;
    }

    bool Habbo::CanSendMail() const
    {
        return m_DirectMail;
    }

    bool Habbo::IsInitialized() const
    {
        return m_Initialized;
    }

}
//-----------------------------------------------//