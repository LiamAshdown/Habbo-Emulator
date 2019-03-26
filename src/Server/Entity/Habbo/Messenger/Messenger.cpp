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
            MessengerFriendsData l_Messenger;
            l_Messenger.m_Id            = l_Result->GetUint32(1);
            l_Messenger.m_Name          = l_Result->GetString(2);
            l_Messenger.m_Figure        = l_Result->GetString(3);
            l_Messenger.m_ConsoleMotto  = l_Result->GetString(4);
            l_Messenger.m_Gender        = l_Result->GetString(5);
            l_Messenger.mLastOnline     = l_Result->GetString(6);

            m_MessengerFriends.push_back(l_Messenger);

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

    /// IsFriendListFull - Check if our friend list is full
    bool Messenger::IsFriendListFull() const
    {
        /// TODO; Habbo Club
        return m_MessengerFriends.size() >= sConfig->GetIntDefault("MessgengerMaxFriendsLimit", 50) ? true : false;
    }

    /// GetMessengerSize - Get size of Messenger
    uint32 Messenger::GetMessengerSize() const
    {
        return m_MessengerFriends.size();
    }
    
    /// ParseMessengerFriends
    /// @p_Buffer : Buffer which is being parsed
    void Messenger::ParseMessengerFriends(StringBuffer& p_Buffer)
    {
        for (auto const& l_Itr : m_MessengerFriends)
        {
            MessengerFriendsData const& l_MessengerFriend = l_Itr;
            Habbo const* l_Habbo = sHotel->FindHabbo(l_MessengerFriend.GetId());

            p_Buffer.AppendWired(l_MessengerFriend.GetId());
            p_Buffer.AppendString(l_MessengerFriend.GetName());
            p_Buffer.AppendWiredBool(l_MessengerFriend.GetGender() == "Male" ? true : false);
            p_Buffer.AppendString(l_MessengerFriend.GetConsoleMotto());

            p_Buffer.AppendWiredBool(l_Habbo ? true : false);  ///< Is user online?

            /// User is online!
            if (l_Habbo)
            {
                /// Is user in room>
                if (l_Habbo->GetRoom())
                {
                    /// Is user in a public room or flat?
                    if (l_Habbo->GetRoom()->GetOwnerId() > 0)
                        p_Buffer.AppendString("Floor1a");
                    else
                        p_Buffer.AppendString(l_Habbo->GetRoom()->GetName());
                }
                else
                    p_Buffer.AppendString("On Hotel View");
            }
            else
                p_Buffer.AppendString(l_MessengerFriend.GetLastOnline()); ///< Last time user was online

            p_Buffer.AppendString(l_MessengerFriend.GetLastOnline());
            p_Buffer.AppendString(l_MessengerFriend.GetFigure());
        }
    }
    
    /// ParseMessengerFriendRequests
    /// @p_Buffer : Buffer which is being parsed
    void Messenger::ParseMessengerFriendRequests(StringBuffer& p_Buffer)
    {
        for (auto const& l_Itr : m_MessengerFriendRequests)
        {
            MessengerFriendsData const& l_MessengerFriend = l_Itr;

            p_Buffer.AppendWired(l_MessengerFriend.GetId());
            p_Buffer.AppendString(l_MessengerFriend.GetName());
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
            MessengerFriendsData const& l_MessengerFriend = l_Itr;
            Habbo const* l_Habbo = sHotel->FindHabbo(l_MessengerFriend.GetId());

            p_Buffer.AppendWired(l_MessengerFriend.GetId());
            p_Buffer.AppendString(l_MessengerFriend.GetConsoleMotto());
            p_Buffer.AppendWiredBool(l_Habbo ? true : false); ///< Is user online?

            /// User is online!
            if (l_Habbo)
            {
                /// Is user in room>
                if (l_Habbo->GetRoom())
                {
                    /// Is user in a public room or flat?
                    if (l_Habbo->GetRoom()->GetOwnerId() > 0)
                        p_Buffer.AppendString("Floor1a");
                    else
                        p_Buffer.AppendString(l_Habbo->GetRoom()->GetName());
                }
                else
                    p_Buffer.AppendString("On Hotel View");
            }
            else
                p_Buffer.AppendString(l_MessengerFriend.GetLastOnline()); ///< Last time user was online
        }
    }
    
    /// ParseMessengerAcceptFriendRequest
    /// @p_Habbo : Habbo Class incase we need to send error message to client who sending is accepting friend request
    /// @p_SenderId : Account Id who sent friend request
    void Messenger::ParseMessengerAcceptFriendRequest(Habbo* p_Habbo, uint32 const& p_SenderId)
    {
        QueryDatabase l_Database("users");
        l_Database.PrepareQuery("SELECT id, user_name, figure, console_motto, gender, last_online, allow_friend_requests, messenger_requests.from_id, messenger_friends.to_id FROM account LEFT JOIN messenger_requests ON messenger_requests.to_id = account.id LEFT JOIN messenger_friends ON messenger_friends.from_id = account.id WHERE(account.id = ?)");
        l_Database.GetStatement()->setUInt(1, p_SenderId);
        l_Database.ExecuteQuery();

        HabboPacket::Messenger::MessengerRequestBuddyError l_Packet;
        l_Packet.Error = MessengerErrorCode::ACCEPT_SUCCESS;

        if (!l_Database.GetResult())
        {
            p_Habbo->ToSocket()->SendPacket(l_Packet.Write());
            return;
        }

        Result* l_Result = l_Database.Fetch();

        /// Check if target user friends is full
       /// TODO; Habbo Club
        if (l_Result->GetRowCount() >= sConfig->GetIntDefault("MessgengerMaxFriendsLimit", 50))
            l_Packet.Error = MessengerErrorCode::TARGET_FRIEND_LIST_FULL;

        /// Does target user already exist on friend list?
        if (l_Result->GetUint32(9) == m_Id)
            l_Packet.Error = MessengerErrorCode::TARGET_DOES_NOT_ACCEPT;

        /// Does target user accept friend requests?
        if (!l_Result->GetBool(7))
            l_Packet.Error = MessengerErrorCode::TARGET_DOES_NOT_ACCEPT;

        if (l_Packet.Error != MessengerErrorCode::ACCEPT_SUCCESS)
        {
            p_Habbo->ToSocket()->SendPacket(l_Packet.Write());
            return;
        }

        /// Succesfull! Insert into our messenger friends for both users
        /// TODO; Is there anyway to clean this?
        /// I don't feel comfortable on querying the database every time we accept a new friend request
        l_Database.PrepareQuery("INSERT INTO messenger_friends (from_id, to_id) VALUES (?, ?) ON DUPLICATE KEY UPDATE to_id = ?");
        l_Database.GetStatement()->setUInt(1, m_Id);
        l_Database.GetStatement()->setUInt(2, l_Result->GetUint32(1));
        l_Database.GetStatement()->setUInt(3, l_Result->GetUint32(1));
        l_Database.ExecuteQuery();

        l_Database.ClearParameters();

        l_Database.GetStatement()->setUInt(1, l_Result->GetUint32(1));
        l_Database.GetStatement()->setUInt(2, m_Id);
        l_Database.GetStatement()->setUInt(3, m_Id);
        l_Database.ExecuteQuery();

        l_Database.ClearParameters();

        /// Remove friend request
        l_Database.PrepareQuery("DELETE FROM messenger_requests WHERE from_id = ? AND to_id = ?");
        l_Database.GetStatement()->setUInt(1, l_Result->GetUint32(1));
        l_Database.GetStatement()->setUInt(2, m_Id);

        /// Send Packet to update console with new friend!
        /// TODO; Do we need to do the same to the other client? or let the console update itself later on?
        UpdateConsole(); /// < Update console to pull in latest friends from database
        HabboPacket::Messenger::MessengerAddFriend l_PacketAddFriend;
        ParseMessengerFriends(l_PacketAddFriend.GetSecondaryBuffer());
        p_Habbo->ToSocket()->SendPacket(l_PacketAddFriend.Write());
    }
    
    /// ParseMessengerAcceptFriendRequest
    /// @p_Buffer : Buffer which is being parsed
    /// @p_Name : Name of Habbo we are searching for
    void Messenger::ParseMessengerSearchUser(StringBuffer& p_Buffer, std::string const& p_Name)
    {
        /// TODO; This code is quite bad in terms of performance.
        /// We could possibly create another column in account table which shows if user
        /// is online or not... but I don't like querying Database just for online status every
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

        p_Buffer.AppendWired(l_Result->GetUint32(1));
        p_Buffer.AppendString(l_Result->GetString(2));
        p_Buffer.AppendWiredBool(l_Result->GetString(5) == "Male" ? true : false);
        p_Buffer.AppendString(l_Result->GetString(4));

        Habbo* l_Habbo = sHotel->FindHabbo(l_Result->GetUint32(1));

        p_Buffer.AppendWiredBool(l_Habbo ? true : false); ///< Is user online?

           /// User is online!
        if (l_Habbo)
        {
            /// Is user in room>
            if (l_Habbo->GetRoom())
            {
                /// Is user in a public room or flat?
                if (l_Habbo->GetRoom()->GetOwnerId() > 0)
                    p_Buffer.AppendString("Floor1a");
                else
                    p_Buffer.AppendString(l_Habbo->GetRoom()->GetName());
            }
            else
                p_Buffer.AppendString("On Hotel View");
        }
        else
            p_Buffer.AppendString(l_Result->GetString(4)); ///< Last time user was online

        p_Buffer.AppendString(l_Result->GetString(6)); ///< Last time user was online
        p_Buffer.AppendString(l_Result->GetString(3)); ///< User Figure
    }

    /// ParseMessengerSendFriendRequest
    /// @p_Habbo : Habbo Class incase we need to send error message to client who sending friend request
    /// @p_Name : Name of Habbo we are sending friend request too
    void Messenger::ParseMessengerSendFriendRequest(Habbo* p_Habbo, std::string const& p_Name)
    {
        QueryDatabase l_Database("users");
        l_Database.PrepareQuery("SELECT id, user_name, figure, console_motto, gender, last_online, allow_friend_requests, messenger_requests.from_id, messenger_friends.to_id FROM account LEFT JOIN messenger_requests ON messenger_requests.to_id = account.id LEFT JOIN messenger_friends ON messenger_friends.from_id = account.id WHERE(account.user_name = ?)");
        l_Database.GetStatement()->setString(1, p_Name);
        l_Database.ExecuteQuery();

        HabboPacket::Messenger::MessengerError l_Packet;
        l_Packet.Error = MessengerErrorCode::ACCEPT_SUCCESS;

        /// Target user does not exist
        if (!l_Database.GetResult())
        {
            l_Packet.Error = MessengerErrorCode::FRIEND_REQUEST_NOT_FOUND;
            p_Habbo->ToSocket()->SendPacket(l_Packet.Write());
            return;
        }

        Result* l_Result = l_Database.Fetch();

        /// Check if target user friends is full
        /// TODO; Habbo Club
        if (l_Result->GetRowCount() >= sConfig->GetIntDefault("MessgengerMaxFriendsLimit", 50))
            l_Packet.Error = MessengerErrorCode::TARGET_FRIEND_LIST_FULL;

        /// Does target user already exist on friend request list?
        if (l_Result->GetUint32(8) == m_Id)
            l_Packet.Error = MessengerErrorCode::TARGET_DOES_NOT_ACCEPT;

        /// Does target user already exist on friend list?
        if (l_Result->GetUint32(9) == m_Id)
            l_Packet.Error = MessengerErrorCode::TARGET_DOES_NOT_ACCEPT;

        /// Does target user accept friend requests?
        if (!l_Result->GetBool(7))
            l_Packet.Error = MessengerErrorCode::TARGET_DOES_NOT_ACCEPT;

        if (l_Packet.Error != MessengerErrorCode::ACCEPT_SUCCESS)
        {
            p_Habbo->ToSocket()->SendPacket(l_Packet.Write());
            return;
        }

        ///< All Good send friend request notification to player if online
        if (Habbo* l_Habbo = sHotel->FindHabbo(l_Result->GetUint32(1)))
        {
            HabboPacket::Messenger::MessengerSendFriendRequest l_Packet;
            l_Packet.Id = p_Habbo->GetId();
            l_Packet.Name = p_Habbo->GetName();
            l_Habbo->ToSocket()->SendPacket(l_Packet.Write());
        }
        else
        {
            /// Insert friend request into database if user is not online
            /// So user will pick up friend request the next time he logs in
            /// TODO; What happens if user is online and goes offline and not accept the friend request?
            /// the friend request will go limbo - not good!
            l_Database.PrepareQuery("INSERT INTO messenger_requests (from_id, to_id) VALUES (?, ?) ON DUPLICATE KEY UPDATE to_id = ?");
            l_Database.GetStatement()->setUInt(1, m_Id);
            l_Database.GetStatement()->setUInt(2, l_Result->GetUint32(1));
            l_Database.GetStatement()->setUInt(3, l_Result->GetUint32(1));
            l_Database.ExecuteQuery();
        }
    }
    
} ///< NAMESPACE MESSENGER
