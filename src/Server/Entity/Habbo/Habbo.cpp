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
#include "Hotel.h"
#include "RoomManager.h"
#include "Opcode/Packets/Server/HabboInfoPackets.h"
#include "Opcode/Packets/Server/MessengerPackets.h"
#include "Opcode/Packets/Server/FavouriteRoomPackets.h"
#include "Opcode/Packets/Server/HabboInfoPackets.h"
#include "Opcode/Packets/Server/MiscPackets.h"

namespace SteerStone
{
    Habbo::Habbo(HabboSocket* p_HabboSocket)
        : m_Socket(p_HabboSocket ? p_HabboSocket->Shared<HabboSocket>() : nullptr), m_RoomGUID(0)
    {
        m_PingInterval = sConfig->GetIntDefault("PongInterval", 30000);
        m_UpdateAccount = sConfig->GetIntDefault("PlayerAccountUpdate", 600000);
        m_MaxFriendsLimit = sConfig->GetIntDefault("MaxFriendsLimit", 50);

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

    bool Habbo::SetRoom(std::shared_ptr<Room> p_Room)
    {
        if (GetRoom())
            GetRoom()->LeaveRoom(this);

        m_Room = p_Room;
        if (auto l_Room = m_Room.lock())
            if (l_Room->EnterRoom(this))
                return true;

        return false;
    }

    std::shared_ptr<Room> Habbo::GetRoom() const
    {
        return m_Room.lock();
    }

    void Habbo::DestroyRoom()
    {
        if (auto l_Room = m_Room.lock())
            l_Room.reset();
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
        l_Packet.MessengerSize = m_Messenger->GetMessengerSize();
        m_Messenger->ParseMessengerInitialize(l_Packet.GetSecondaryBuffer());
        ToSocket()->SendPacket(l_Packet.Write());

        if (m_Messenger->HasFriendRequestPending())
            m_Messenger->ParseMessengerFriendRequest();

        m_Messenger->LoadMessengerMessages();
    }

    void Habbo::MessengerAcceptRequest(uint32 const& p_SenderId)
    {
        /// Is friend list full?
        if (m_Messenger->IsFriendListFull())
        {
            HabboPacket::Messenger::MessengerRequestBuddyError l_Packet;
            l_Packet.Error = MessengerErrorCode::FRIEND_LIST_FULL;
            ToSocket()->SendPacket(l_Packet.Write());
            return;
        }

        m_Messenger->ParseMessengerAcceptFriendRequest(p_SenderId);
    }

    void Habbo::SendMessengerUpdate()
    {
        HabboPacket::Messenger::MessengerUpdate l_Packet;
        m_Messenger->ParseMessengerUpdate(l_Packet.GetSecondaryBuffer());
        ToSocket()->SendPacket(l_Packet.Write());
    }

    void Habbo::SendSearchUserResults(std::string const& p_Name)
    {
        HabboPacket::Messenger::MessengerFindUserResult l_Packet;
        l_Packet.Messenger = "MESSENGER";
        m_Messenger->ParseMessengerSearchUser(l_Packet.GetSecondaryBuffer(), p_Name);
        ToSocket()->SendPacket(l_Packet.Write());
    }

    void Habbo::MessengerSendFriendRequest(std::string const & p_Name)
    {
        /// Is friend list full?
        if (m_Messenger->IsFriendListFull())
        {
            HabboPacket::Messenger::MessengerError l_Packet;
            l_Packet.Error = MessengerErrorCode::FRIEND_LIST_FULL;
            ToSocket()->SendPacket(l_Packet.Write());   
            return;
        }

        m_Messenger->ParseMessengerSendFriendRequest(p_Name);
    }

    void Habbo::MessengerRemoveFriend(std::unique_ptr<ClientPacket> p_Packet)
    {
        m_Messenger->ParseMessengerRemoveFriend(std::move(p_Packet));
    }

    void Habbo::MessengerRejectRequest(std::unique_ptr<ClientPacket> p_Packet)
    {
        m_Messenger->ParseMessengerRejectRequest(std::move(p_Packet));
    }

    void Habbo::MessengerSendMessage(std::unique_ptr<ClientPacket> p_Packet)
    {
        m_Messenger->ParseMessengerSendMessage(std::move(p_Packet));
    }

    void Habbo::MessengerReply(uint32 const& p_MessageId)
    {
        m_Messenger->ReadMessage(p_MessageId);
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

        for (auto const& itr : m_Badges)
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

        for (auto const& itr : m_FuseRights)
            l_Packet.HabboFuseRights.push_back(itr.GetFuseRight());

        ToSocket()->SendPacket(l_Packet.Write());
    }

    void Habbo::SendClubStatus()
    {
        // TODO;
    }

    void Habbo::InitializeHabboData()
    {
        m_Messenger = std::make_unique<Messenger>(this);
        m_Messenger->UpdateConsole();

        m_FavouriteRooms = std::make_unique<FavouriteRoom>(m_Id);
        m_FavouriteRooms->LoadFavouriteRooms();
    }

    void Habbo::UpdatePosition(int16 const& p_X, int16 const& p_Y, int16 const& p_Z, int16 const& p_O)
    {
        m_PositionX = p_X;
        m_PositionY = p_Y;
        m_PositionZ = p_Z;
        m_Orientation = p_O;
    }

    bool Habbo::Update(uint32 const& p_Diff)
    {
        std::lock_guard<std::mutex> l_Guard(m_Mutex);

        if (m_Socket && !m_Socket->IsClosed())
        {
            if (m_UpdateAccount < p_Diff)
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

                m_UpdateAccount = sConfig->GetIntDefault("PlayerAccountUpdate", 600000);
            }
            else
                m_UpdateAccount -= p_Diff;

            if (m_PingInterval < p_Diff)
            {
                if (IsPonged())
                {
                    SendPing();
                    m_PingInterval = sConfig->GetIntDefault("PongInterval", 10000);
                }
                else
                {
                    Logout(LOGOUT_TIMEOUT);
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

    void Habbo::Logout(LogoutReason const p_Reason /*= LOGGED_OUT*/)
    {
        SaveFavouriteRoomToDB();

        if (GetRoom())
            GetRoom()->LeaveRoom(this);

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

    /// SendPing - Send Ping response to client
    void Habbo::SendPing()
    {
        m_Ponged = false;
        HabboPacket::Misc::Ping l_Packet;
        ToSocket()->SendPacket(l_Packet.Write());
    }

} ///< NAMESPACE STEERSTONE