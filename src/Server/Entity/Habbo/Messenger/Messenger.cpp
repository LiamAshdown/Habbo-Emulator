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
#include "Room.h"
#include "Opcode/Packets/Server/MessengerPackets.h"

namespace SteerStone
{
    /// Constructor
    /// @p_Id : Account Id, we don't use Habbo class here
    Messenger::Messenger(const uint32& id) : m_Id(id)
    {
    }
    
    /// Deconstructor
    Messenger::~Messenger()
    {
        m_MessengerFriends.clear();
        m_MessengerFriendRequests.clear();
    }
    
    /// LoadMessenger - Load our messenger
    void Messenger::LoadMessenger()
    {
        LoadMessengerFriends();
        LoadMessengerFriendRequests();
    }

    /// LoadMessengerFriends - Load our Messenger Friends from l_Database
    void Messenger::LoadMessengerFriends()
    {
        m_MessengerFriends.clear();

        QueryDatabase l_Database("users");
        l_Database.PrepareQuery("SELECT id, user_name, figure, console_motto, gender, last_online FROM messenger_friends INNER JOIN account ON messenger_friends.to_id = account.id WHERE(messenger_friends.from_id = ?)");
        l_Database.GetStatement()->setUInt(1, m_Id);
        l_Database.ExecuteQuery();

        if (!l_Database.GetResult())
            return;

        Result* l_Result = l_Database.Fetch();

        do
        {
            MessengerFriendsData messenger;
            messenger.m_Id = l_Result->GetUint32(1);
            messenger.m_Name = l_Result->GetString(2);
            messenger.m_Figure = l_Result->GetString(3);
            messenger.m_ConsoleMotto = l_Result->GetString(4);
            messenger.m_Gender = l_Result->GetString(5);
            messenger.mLastOnline = l_Result->GetString(6);
            messenger.mFriendsSize++;
            m_MessengerFriends.push_back(messenger);

        } while (l_Result->GetNextResult());
    }
    
    /// LoadMessengerFriendRequests - Load our Messenger Friend requests from l_Database
    void Messenger::LoadMessengerFriendRequests()
    {
        m_MessengerFriendRequests.clear();

        QueryDatabase l_Database("users");
        l_Database.PrepareQuery("SELECT id, user_name, figure, console_motto, gender, last_online FROM messenger_requests INNER JOIN account ON messenger_requests.from_id = account.id WHERE(messenger_requests.to_id = ?)");
        l_Database.GetStatement()->setUInt(1, m_Id);
        l_Database.ExecuteQuery();

        if (!l_Database.GetResult())
            return;

        Result* l_Result = l_Database.Fetch();

        MessengerFriendsData l_Messenger;
        l_Messenger.m_Id = l_Result->GetUint32(1);
        l_Messenger.m_Name = l_Result->GetString(2);
        l_Messenger.m_Figure = l_Result->GetString(3);
        l_Messenger.m_ConsoleMotto = l_Result->GetString(4);
        l_Messenger.m_Gender = l_Result->GetString(5);
        l_Messenger.mLastOnline = l_Result->GetString(6);

        m_MessengerFriendRequests.push_back(l_Messenger);
    }
    
    /// UpdateConsole - Reload LoadMessengerFriendRequests and LoadMessengerFriends
    void Messenger::UpdateConsole()
    {
        LoadMessengerFriends();
        LoadMessengerFriendRequests();
    }
    
    /// HasFriendRequest - Check if we have any friend requests
    bool Messenger::HasFriendRequest() const
    {
        return m_MessengerFriendRequests.size() > 0 ? true : false;
    }

    /// CanSendFriendRequest - Check if our friend list is full
    bool Messenger::CanSendFriendRequest() const
    {
        /// TODO; Habbo Club
        return m_MessengerFriends.size() >= sConfig->GetIntDefault("MessgengerMaxFriendsLimit", 50) ? false : true;
    }
    
    /// ParseMessengerFriends
    /// @p_Buffer : Buffer which is being parsed
    void Messenger::ParseMessengerFriends(StringBuffer& p_Buffer)
    {
        p_Buffer.AppendWired(m_MessengerFriends.size());

        for (auto const& l_Itr : m_MessengerFriends)
        {
            MessengerFriendsData const& messengerFriend = l_Itr;
            Habbo const* l_Habbo = sHotel->FindHabbo(messengerFriend.GetId());

            p_Buffer.AppendWired(messengerFriend.GetId());
            p_Buffer.AppendString(messengerFriend.GetName());
            p_Buffer.AppendWiredBool(messengerFriend.GetGender() == "Male" ? true : false);
            p_Buffer.AppendString(messengerFriend.GetConsoleMotto());

            p_Buffer.AppendWiredBool(l_Habbo ? true : false);

            if (l_Habbo)
            {
                if (l_Habbo->GetRoom())
                {
                    if (l_Habbo->GetRoom()->GetOwnerId() > 0)
                        p_Buffer.AppendString("Floor1a");
                    else
                        p_Buffer.AppendString(l_Habbo->GetRoom()->GetName());
                }
                else
                    p_Buffer.AppendString("On Hotel View");
            }
            else
                p_Buffer.AppendString(messengerFriend.GetLastOnline());

            p_Buffer.AppendString(messengerFriend.GetLastOnline());
            p_Buffer.AppendString(messengerFriend.GetFigure());
        }
    }
    
    /// ParseMessengerFriendRequests
    /// @p_Buffer : Buffer which is being parsed
    void Messenger::ParseMessengerFriendRequests(StringBuffer& p_Buffer)
    {
        for (auto const& l_Itr : m_MessengerFriendRequests)
        {
            MessengerFriendsData const& messengerFriend = l_Itr;

            p_Buffer.AppendWired(messengerFriend.GetId());
            p_Buffer.AppendString(messengerFriend.GetName());
        }
    }

    /// ParseMessengerUpdate
    /// @p_Buffer : Buffer which is being parsed
    void Messenger::ParseMessengerUpdate(StringBuffer& p_Buffer)
    {
        /// Query the Database again, when we update console
        UpdateConsole();

        p_Buffer.AppendWired(m_MessengerFriends.size());

        for (auto const& l_Itr : m_MessengerFriends)
        {
            MessengerFriendsData const& messengerFriend = l_Itr;
            Habbo const* l_Habbo = sHotel->FindHabbo(messengerFriend.GetId());

            p_Buffer.AppendWired(messengerFriend.GetId());
            p_Buffer.AppendString(messengerFriend.GetConsoleMotto());
            p_Buffer.AppendWiredBool(l_Habbo ? true : false);

            if (l_Habbo)
            {
                if (l_Habbo->GetRoom())
                {
                    if (l_Habbo->GetRoom()->GetOwnerId() > 0)
                        p_Buffer.AppendString("Floor1a");
                    else
                        p_Buffer.AppendString(l_Habbo->GetRoom()->GetName());
                }
                else
                    p_Buffer.AppendString("On Hotel View");
            }
            else
                p_Buffer.AppendString(messengerFriend.GetLastOnline());
        }
    }
    
    /// ParseMessengerAcceptRequest
    /// @p_Buffer : Buffer which is being parsed
    /// @p_SenderId : Account Id who sent friend request
    void Messenger::ParseMessengerAcceptRequest(StringBuffer& p_Buffer, uint32 const& p_SenderId)
    {

    }
    
    /// ParseMessengerAcceptRequest
    /// @p_Buffer : Buffer which is being parsed
    /// @p_Name : Name of Habbo we are searching for
    void Messenger::ParseMessengerSearchUser(StringBuffer& p_Buffer, std::string const& p_Name)
    {
        /// TODO; This code is quite bad in terms of performance.
        /// We could possibly create another column in account table which shows if user
        /// is online or not... but I don't like querying l_Database just for online status every
        /// login or logout
        QueryDatabase l_Database("users");
        l_Database.PrepareQuery("SELECT id, user_name, figure, console_motto, gender, last_online FROM account WHERE user_name = ?");
        l_Database.GetStatement()->setString(1, p_Name);
        l_Database.ExecuteQuery();

        /// User doesn't exist
        if (!l_Database.GetResult())
        {
            p_Buffer.AppendWired(0);
            return;
        }

        Result* l_Result = l_Database.Fetch();

        p_Buffer.AppendString("MESSENGER");
        p_Buffer.AppendWired(l_Result->GetUint32(1));
        p_Buffer.AppendString(l_Result->GetString(2));
        p_Buffer.AppendWiredBool(l_Result->GetString(5) == "Male" ? true : false);
        p_Buffer.AppendString(l_Result->GetString(4));

        Habbo* l_Habbo = sHotel->FindHabbo(l_Result->GetUint32(1));

        p_Buffer.AppendWiredBool(l_Habbo ? true : false);

        if (l_Habbo)
        {
            if (l_Habbo->GetRoom())
            {
                if (l_Habbo->GetRoom()->GetOwnerId() > 0)
                    p_Buffer.AppendString("Floor1a");
                else
                    p_Buffer.AppendString(l_Habbo->GetRoom()->GetName());
            }
            else
                p_Buffer.AppendString("On Hotel View");
        }
        else
            p_Buffer.AppendString(l_Result->GetString(4));

        p_Buffer.AppendString(l_Result->GetString(6));
        p_Buffer.AppendString(l_Result->GetString(3));
    }

    /// ParseMessengerSendFriendRequest
    /// @p_Habbo : Habbo Class incase we need to send error message to client who sending friend request
    /// @p_Name : Name of Habbo we are sending friend request too
    void Messenger::ParseMessengerSendFriendRequest(Habbo* p_Habbo, const std::string& p_Name)
    {
        QueryDatabase l_Database("users");
        l_Database.PrepareQuery("SELECT id, user_name, figure, console_motto, gender, last_online, allow_friend_requests, messenger_requests.from_id, messenger_friends.to_id FROM account LEFT JOIN messenger_requests ON messenger_requests.to_id = account.id LEFT JOIN messenger_friends ON messenger_friends.from_id = account.id WHERE(account.user_name = ?)");
        l_Database.GetStatement()->setString(1, p_Name);
        l_Database.ExecuteQuery();

        /// Target user does not exist
        if (!l_Database.GetResult())
        {
            HabboPacket::Messenger::MessengerError l_Packet;
            l_Packet.Error = MessengerErrorCode::FRIEND_REQUEST_NOT_FOUND;
            l_Packet.MessageId = 0; ///< Client logs error and needs an Id... not supported by us
            p_Habbo->ToSocket()->SendPacket(l_Packet.Write());
            return;
        }

        Result* l_Result = l_Database.Fetch();

        /// Check if target user friends is full
        /// TODO; Habbo Club
        if (l_Result->GetRowCount() >= sConfig->GetIntDefault("MessgengerMaxFriendsLimit", 50))
        {
            HabboPacket::Messenger::MessengerError l_Packet;
            l_Packet.Error = MessengerErrorCode::FRIEND_LIST_FULL;
            l_Packet.MessageId = 0; ///< Client logs error and needs an Id... not supported by us
            p_Habbo->ToSocket()->SendPacket(l_Packet.Write());
            return;
        }

        /// Does target user already exist on friend request list?
        if (l_Result->GetUint32(8) == m_Id)
        {
            HabboPacket::Messenger::MessengerError l_Packet;
            l_Packet.Error = MessengerErrorCode::TARGET_DOES_NOT_ACCEPT;
            l_Packet.MessageId = 0; ///< Client logs error and needs an Id... not supported by us
            p_Habbo->ToSocket()->SendPacket(l_Packet.Write());
            return;
        }

        /// Does target user already exist on friend  list?
        if (l_Result->GetUint32(9) == m_Id)
        {
            HabboPacket::Messenger::MessengerError l_Packet;
            l_Packet.Error = MessengerErrorCode::TARGET_DOES_NOT_ACCEPT;
            l_Packet.MessageId = 0; ///< Client logs error and needs an Id... not supported by us
            p_Habbo->ToSocket()->SendPacket(l_Packet.Write());
            return;
        }

        /// Does target user accept friend requests?
        if (!l_Result->GetBool(7))
        {
            HabboPacket::Messenger::MessengerError l_Packet;
            l_Packet.Error = MessengerErrorCode::TARGET_DOES_NOT_ACCEPT;
            l_Packet.MessageId = 0; ///< Client logs error and needs an Id... not supported by us
            p_Habbo->ToSocket()->SendPacket(l_Packet.Write());
            return;
        }

        ///< All Good send friend request notification to player if online
        Habbo* l_Habbo = sHotel->FindHabbo(l_Result->GetUint32(1));

        if (l_Habbo)
        {
            HabboPacket::Messenger::MessengerSendFriendRequest l_Packet;
            l_Packet.Id = p_Habbo->GetId();
            l_Packet.Name = p_Habbo->GetName();
            l_Habbo->ToSocket()->SendPacket(l_Packet.Write());
        }

        /// Insert friend request into database
        l_Database.PrepareQuery("INSERT INTO messenger_requests (from_id, to_id) VALUES (?, ?) ON DUPLICATE KEY UPDATE to_id = ?");
        l_Database.GetStatement()->setUInt(1, m_Id);
        l_Database.GetStatement()->setUInt(2, l_Result->GetUint32(1));
        l_Database.GetStatement()->setUInt(3, l_Result->GetUint32(1));
        l_Database.ExecuteQuery();
    }
    
} ///< NAMESPACE MESSENGER
