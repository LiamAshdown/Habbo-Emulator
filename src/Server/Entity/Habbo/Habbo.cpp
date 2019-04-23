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
#include "Common/Maths.h"
#include "Hotel.h"
#include "RoomManager.h"
#include "Opcode/Packets/Server/MessengerPackets.h"
#include "Opcode/Packets/Server/NavigatorPackets.h"
#include "Opcode/Packets/Server/MiscPackets.h"
#include "Opcode/Packets/Server/RoomPackets.h"
#include "Opcode/Packets/Server/LoginPackets.h"

namespace SteerStone
{
    /// Constructor
    /// @p_HabboSocket -Socket is inheriting from
    Habbo::Habbo(HabboSocket* p_HabboSocket)
        : m_Socket(p_HabboSocket ? p_HabboSocket->Shared<HabboSocket>() : nullptr)
    {
        m_PingInterval = sHotel->GetIntConfig(IntConfigs::CONFIG_PONG_INTERVAL);
        m_MaxFriendsLimit = sConfig->GetIntDefault("MaxFriendsLimit", 50);
        m_LastCreatedRoomId = 0;
        m_RoomGUID = 0;
        m_DanceId = 0;
        m_ScheduledForDelete = false;

        SendPing();
    }

    /// Deconstructor
    Habbo::~Habbo()
    {
        Logout();
    }

    ///////////////////////////////////////////
    //                 ROOMS
    ///////////////////////////////////////////

    /// SetRoom
    /// @p_Room - Room Id which player is inside room
    /// @p_WalkWay - Walk Way tile user will spawn ontop on
    bool Habbo::SetRoom(std::shared_ptr<Room> p_Room, WalkWay* p_WalkWay /*= nullptr*/)
    {
        /// Are we already inside a room? Leave room if so
        if (GetRoom())
            GetRoom()->LeaveRoom(this);

        m_Room = p_Room;
        if (auto l_Room = m_Room.lock())
            if (l_Room->EnterRoom(this, p_WalkWay))
                return true;

        return false;
    }

    /// GetRoom
    /// Get room
    std::shared_ptr<Room> Habbo::GetRoom() const
    {
        if (m_Room.expired() || !GetRoomGUID())
            return nullptr;

        return m_Room.lock();
    }

    /// DestroyRoom
    /// Set Room to nullptr - player is no longer inside room
    void Habbo::DestroyRoom()
    {
        m_RoomGUID = 0;
    }

    //////////////////////////////////////////////
    //            FAVOURITE ROOMS
    /////////////////////////////////////////////

    /// SendFavouriteRooms
    /// Send Favourite Rooms list to user
    void Habbo::SendFavouriteRooms()
    {
        HabboPacket::Navigator::FavouriteRoomResult l_Packet;
        m_FavouriteRooms->ParseSendFavouriteRooms(l_Packet.GetBuffer());
        SendPacket(l_Packet.Write());
    }

    /// AddFavouriteRoom
    /// @p_IsPublic - Is room public
    /// @p_RoomId - Id of room
    void Habbo::AddFavouriteRoom(bool const& p_IsPublic, uint32 const& p_RoomId)
    {
        m_FavouriteRooms->AddFavouriteRoom(p_IsPublic, p_RoomId);
    }

    /// RemoveFavouriteRoom
    /// @p_RoomId - Id of room
    void Habbo::RemoveFavouriteRoom(uint32 const& p_RoomId)
    {
        m_FavouriteRooms->RemoveFavouriteRoom(p_RoomId);
    }

    /// SaveFavouriteRoomToDB
    /// Update Favourite Rooms to database
    void Habbo::SaveFavouriteRoomToDB()
    {
        m_FavouriteRooms->SaveToDB();
    }

    ///////////////////////////////////////////
    //           MESSENGER CONSOLE
    ///////////////////////////////////////////

    /// SendInitializeMessenger
    /// Send Initialize console on user login
    void Habbo::SendInitializeMessenger()
    {
        HabboPacket::Messenger::Initialize l_Packet;
        l_Packet.ConsoleMotto = GetConsoleMotto();
        l_Packet.FriendsLimit = sConfig->GetIntDefault("MessgengerMaxFriendsLimit", 50);
        l_Packet.ClubFriendsLimit = sConfig->GetIntDefault("MessgengerMaxFriendsClubLimit", 100);
        l_Packet.MessengerSize = m_Messenger->GetMessengerSize();
        m_Messenger->ParseMessengerInitialize(l_Packet.GetSecondaryBuffer());
        SendPacket(l_Packet.Write());

        if (m_Messenger->HasFriendRequestPending())
            m_Messenger->ParseMessengerFriendRequest();

        m_Messenger->LoadMessengerMessages();
    }

    /// MessengerAcceptRequest
    /// @p_SenderId - Id of room
    void Habbo::MessengerAcceptRequest(uint32 const p_SenderId)
    {
        /// Is friend list full?
        if (m_Messenger->IsFriendListFull())
        {
            HabboPacket::Messenger::BuddyRequestResult l_Packet;
            l_Packet.Error = MessengerErrorCode::FRIEND_LIST_FULL;
            SendPacket(l_Packet.Write());
            return;
        }

        m_Messenger->ParseMessengerAcceptFriendRequest(p_SenderId);
    }

    /// SendMessengerUpdate
    /// Update messenger status
    void Habbo::SendMessengerUpdate()
    {
        HabboPacket::Messenger::Update l_Packet;
        m_Messenger->ParseMessengerUpdate(l_Packet.GetSecondaryBuffer());
        SendPacket(l_Packet.Write());
    }

    /// SendSearchUserResults
    /// @p_Name - Room name user is searching for
    void Habbo::SendSearchUserResults(std::string const p_Name)
    {
        HabboPacket::Messenger::FindUser l_Packet;
        l_Packet.Messenger = "MESSENGER";
        m_Messenger->ParseMessengerSearchUser(l_Packet.GetSecondaryBuffer(), p_Name);
        SendPacket(l_Packet.Write());
    }

    /// MessengerBuddyRequest
    /// @p_Name - Name of friend user is sending request for
    void Habbo::MessengerBuddyRequest(std::string const p_Name)
    {
        /// Is friend list full?
        if (m_Messenger->IsFriendListFull())
        {
            HabboPacket::Messenger::ErrorMessenger l_Packet;
            l_Packet.Error = MessengerErrorCode::FRIEND_LIST_FULL;
            SendPacket(l_Packet.Write());   
            return;
        }

        m_Messenger->ParseMessengerSendBuddyRequest(p_Name);
    }

    /// MessengerRemoveBuddy
   /// @p_Packet - Client packet which is being decoded
    void Habbo::MessengerRemoveBuddy(std::unique_ptr<ClientPacket> p_Packet)
    {
        m_Messenger->ParseMessengerRemoveBuddy(std::move(p_Packet));
    }

    /// MessengerRejectRequest
    /// @p_Packet - Client packet which is being decoded
    void Habbo::MessengerRejectRequest(std::unique_ptr<ClientPacket> p_Packet)
    {
        m_Messenger->ParseMessengerRejectBuddy(std::move(p_Packet));
    }

    /// MessengerSendMessage
    /// @p_Packet - Client packet which is being decoded
    void Habbo::MessengerSendMessage(std::unique_ptr<ClientPacket> p_Packet)
    {
        m_Messenger->ParseMessengerSendMessage(std::move(p_Packet));
    }

    /// MessengerReply
    /// @p_MessageId - Id of message (account id)
    void Habbo::MessengerReply(uint32 const p_MessageId)
    {
        m_Messenger->ReadMessage(p_MessageId);
    }

    ///////////////////////////////////////////
    //             USER OBJECTS
    ///////////////////////////////////////////

    /// SendHabboObject
    /// Send user object on login
    void Habbo::SendHabboObject()
    {
        HabboPacket::Login::HabboObject l_Packet;
        l_Packet.Id             = std::to_string(m_Id);
        l_Packet.Name           = m_Name;
        l_Packet.Figure         = m_Figure;
        l_Packet.Gender         = m_Gender;
        l_Packet.Motto          = m_Motto;
        l_Packet.Tickets        = m_Tickets;
        l_Packet.PoolFigure     = m_PoolFigure;
        l_Packet.Films          = m_Films;
        SendPacket(l_Packet.Write());
    }

    /// SendUpdateStatusWalk
    /// @p_X - X axis on new position
    /// @p_Y - Y axis on new position
    /// @p_Z - Z axis on new position
    /// @p_Rotation - New Rotation
    void Habbo::UpdatePosition(int16 const p_X, int16 const p_Y, int16 const p_Z, int16 const p_Rotation)
    {
        m_PositionX = p_X;
        m_PositionY = p_Y;
        m_PositionZ = p_Z;
        m_BodyRotation = p_Rotation;
        m_HeadRotation = p_Rotation;
    }

    /// TeleportTo
    /// Teleport player to given location
    /// @p_X - X axis on new position
    /// @p_Y - Y axis on new position
    /// @p_Z - Z axis on new position
    /// @p_Rotation - New Rotation
    void Habbo::TeleportTo(int16 const p_X, int16 const p_Y, int16 const p_Z, int16 const p_Rotation)
    {
        UpdatePosition(p_X, p_Y, p_Z, p_Rotation);
        GetRoom()->AddStatus(GetRoomGUID(), Status::STATUS_TELEPORT);
    }

    /// LookTo
    /// @p_X : X axis to face targeted user
    /// @p_Y : Y axis to face targeted user
    void Habbo::LookTo(int16 const p_X, int16 const p_Y)
    {
        if (GetRoom()->HasStatus(GetRoomGUID(), Status::STATUS_SITTING))
            SetHeadRotation(Maths::CalculateHeadRotation(GetHeadRotation(), GetPositionX(), GetPositionY(), p_X, p_Y));
        else
            UpdatePosition(GetPositionX(), GetPositionY(), GetPositionZ(), Maths::CalculateHumanDirection(GetPositionX(), GetPositionY(), p_X, p_Y));

        GetRoom()->AddStatus(GetRoomGUID(), Status::STATUS_ROTATION);
    }

    ///////////////////////////////////////////
    //             ACCOUNT INFO
    ///////////////////////////////////////////

    /// SendAccountPreferences
    /// Send user account preferences (set from users.account database)
    void Habbo::SendAccountPreferences()
    {
        HabboPacket::Login::AccountPreferences l_Packet;
        l_Packet.SoundEnabled = IsSoundEnabled();
        l_Packet.TutorialFinished = true; ///< TODO
        SendPacket(l_Packet.Write());
    }

    /// SendAccountBadges
    /// Send user account badges (set from users.account_badges database)
    void Habbo::SendAccountBadges()
    {
        m_Badge->SendBadges();
    }

    /// SendSetBadge
    /// Update current badge
    /// @p_Badge : New badge we are setting to
    /// @p_Visible : Is new badge visible
    void Habbo::SendSetBadge(std::string const p_Badge, bool const p_Visible)
    {
        m_Badge->SendSetBadge(p_Badge, p_Visible);
    }

    /// SendFuseRights
    /// Send user account rights (set from users.rank_fuserights database)
    void Habbo::SendFuseRights()
    {
        m_FuseRight->SendFuseRights();
    }

    /// SendClubStatus
    /// Send user account club status (set from users.club_subscriptions database)
    void Habbo::SendClubStatus()
    {
        m_HabboClub->LoadSubscription();
    }

    /// SetRank
    /// Update users rank
    /// @p_Rank : Rank user will recieve
    void Habbo::SetRank(uint8 const p_Rank)
    {
        m_Rank = p_Rank;

        /// Load our fuse rights again for user to recieve
        m_FuseRight->LoadFuseRights();
        SendFuseRights();
    }

    ///////////////////////////////////////////
    //             HABBO INFO
    ///////////////////////////////////////////

    /// InitializeHabboData
    /// Initialize Habbo Info data when user logs in
    void Habbo::InitializeHabboData()
    {
        m_Messenger = std::make_unique<Messenger>(this);
        m_Messenger->UpdateConsole();

        m_FavouriteRooms = std::make_unique<FavouriteRoom>(m_Id);
        m_FavouriteRooms->LoadFavouriteRooms();

        m_FuseRight = std::make_unique<FuseRights>(this);
        m_FuseRight->LoadFuseRights();

        m_Badge = std::make_unique<Badge>(this);
        m_Badge->LoadBadges();

        m_HabboClub = std::make_unique<HabboClub>(this);
    }

    /// Update
    /// Update Habbo user
    /// @p_Diff - Tick Timer
    bool Habbo::Update(uint32 const& p_Diff)
    {
        if (m_Socket && !m_Socket->IsClosed())
        {
            if (m_PingInterval < p_Diff)
            {
                if (IsPonged())
                {
                    SendPing();
                    m_PingInterval = sHotel->GetIntConfig(IntConfigs::CONFIG_PONG_INTERVAL);
                }
                else
                {
                    LOG_INFO << "Disconnecting Habbo: " << GetId() << " have not recieved a pong back";
                    return false;
                }
            }
            else
                m_PingInterval -= p_Diff;

            return true;
        }
        else
            return false;
    }

    /// Logout
    /// @p_Reason - Logout reason (enum LogoutReason)
    void Habbo::Logout(LogoutReason const p_Reason /*= LOGGED_OUT*/)
    {
        //SaveToDB();
        m_Messenger.reset();
        m_FavouriteRooms.reset();

        if (m_Socket)
        {
            if (!m_Socket->IsClosed())
            {
                /// Inform client player logged out
                HabboPacket::Misc::HotelLogout l_Packet;
                l_Packet.Reason = p_Reason;
                SendPacket(l_Packet.Write());

                m_Socket->CloseSocket();
            }

            m_Socket->DestroyHabbo();
        }
    }

    void Habbo::CleanUpBeforeDelete()
    { 
        if (GetRoom())
        {
            GetRoom()->LeaveRoom(this);
            m_ScheduledForDelete = true;
        }
    }

    /// SaveToDB
    /// Save Habbo data to database on logout
    void Habbo::SaveToDB()
    {
        QueryDatabase database("users");
        database.PrepareQuery("UPDATE account SET email = ?, figure = ?, pool_figure = ?, motto = ?, console_motto = ?, birthday = ?, gender = ?, credits = ?, tickets = ?, films = ?, sound_enabled = ?, subscribed = ?, rank = ? WHERE id = ?");
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
        database.GetStatement()->setBoolean(12, IsSubscribed());
        database.GetStatement()->setUInt(13, GetRank());
        database.GetStatement()->setUInt(14, GetId());
        database.ExecuteQuery();

        SaveFavouriteRoomToDB();
    }

    /// SendPacket 
    /// @p_Buffer : Buffer which holds our data to be send to the client
    void Habbo::SendPacket(StringBuffer const* p_Buffer)
    {
        if (!m_Socket)
            return;

        m_Socket->SendPacket(p_Buffer);
    }

    /// SendPing
    /// Send Ping response to client
    void Habbo::SendPing()
    {
        m_Ponged = false;
        HabboPacket::Misc::Ping l_Packet;
        SendPacket(l_Packet.Write());
    }

} ///< NAMESPACE STEERSTONE