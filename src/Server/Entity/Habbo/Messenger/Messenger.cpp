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
    /// @p_Habbo : Habbo class
    Messenger::Messenger(Habbo* p_Habbo) : m_Habbo(p_Habbo)
    {
    }
    
    /// Deconstructor
    Messenger::~Messenger()
    {
        m_MessengerFriends.clear();
        m_MessengerFriendRequests.clear();
    }

    void Messenger::LoadMessengerFriends()
    {
        m_MessengerFriends.clear();

        QueryDatabase l_Database("users");
        l_Database.PrepareQuery("SELECT id, user_name, figure, console_motto, gender, last_online FROM messenger_friends INNER JOIN account ON messenger_friends.to_id = account.id WHERE(messenger_friends.from_id = ?)");
        l_Database.GetStatement()->setUInt(1, m_Habbo->GetId());
        l_Database.ExecuteQuery();

        if (!l_Database.GetResult())
            return;

        Result* l_Result = l_Database.Fetch();

        do
        {
            MessengerFriendData& l_Friend  = m_MessengerFriends[l_Result->GetUint32(1)];
            l_Friend.m_Id                  = l_Result->GetUint32(1);
            l_Friend.m_Name                = l_Result->GetString(2);
            l_Friend.m_Figure              = l_Result->GetString(3);
            l_Friend.m_ConsoleMotto        = l_Result->GetString(4);
            l_Friend.m_Gender              = l_Result->GetString(5);
            l_Friend.m_LastOnline          = l_Result->GetString(6);

        } while (l_Result->GetNextResult());
    }
    
    void Messenger::LoadMessengerFriendRequests()
    {
        m_MessengerFriendRequests.clear();

        QueryDatabase l_Database("users");
        l_Database.PrepareQuery("SELECT id, user_name, figure, console_motto, gender, last_online FROM messenger_requests INNER JOIN account ON messenger_requests.from_id = account.id WHERE(messenger_requests.to_id = ?)");
        l_Database.GetStatement()->setUInt(1, m_Habbo->GetId());
        l_Database.ExecuteQuery();

        if (!l_Database.GetResult())
            return;

        Result* l_Result = l_Database.Fetch();

        do
        {
            MessengerFriendData& l_Friend = m_MessengerFriendRequests[l_Result->GetUint32(1)];
            l_Friend.m_Id                  = l_Result->GetUint32(1);
            l_Friend.m_Name                = l_Result->GetString(2);
            l_Friend.m_Figure              = l_Result->GetString(3);
            l_Friend.m_ConsoleMotto        = l_Result->GetString(4);
            l_Friend.m_Gender              = l_Result->GetString(5);
            l_Friend.m_LastOnline          = l_Result->GetString(6);

        } while (l_Result->GetNextResult());
    }

    void Messenger::LoadMessengerMessages()
    {
        QueryDatabase l_Database("users");
        l_Database.PrepareQuery("SELECT sender_id, body, date FROM messenger_messages WHERE receiver_id = ? AND has_read = 0");
        l_Database.GetStatement()->setUInt(1, m_Habbo->GetId());
        l_Database.ExecuteQuery();

        if (!l_Database.GetResult())
            return;

        Result* l_Result = l_Database.Fetch();

        do
        {
            HabboPacket::Messenger::SendMessage l_Packet;
            l_Packet.Id         = l_Result->GetUint32(1);
            l_Packet.ToId       = m_Habbo->GetId();
            l_Packet.Date       = l_Result->GetString(3);
            l_Packet.Message    = l_Result->GetString(2);

            m_Habbo->ToSocket()->SendPacket(l_Packet.Write());

        } while (l_Result->GetNextResult());
    }
    
    /// UpdateConsole
    /// Reload LoadMessengerFriendRequests and LoadMessengerFriends
    void Messenger::UpdateConsole()
    {
        LoadMessengerFriends();
        LoadMessengerFriendRequests();
    }
    
    bool Messenger::HasFriendRequestPending() const
    {
        return m_MessengerFriendRequests.size() > 0 ? true : false;
    }

    /// IsFriendListFull
    bool Messenger::IsFriendListFull() const
    {
        uint32 l_FriendsLimit = 0;
        if (m_Habbo->IsSubscribed())
            l_FriendsLimit = sHotel->GetIntConfig(CONFIG_MESSENGER_MAX_CLUB_FRIENDS);
        else
            l_FriendsLimit = sHotel->GetIntConfig(CONFIG_MESSENGER_MAX_FRIENDS);

        return m_MessengerFriends.size() >= l_FriendsLimit;
    }

    /// GetMessengerSize
    /// Returns size of Messenger Friends Map
    uint32 Messenger::GetMessengerSize() const
    {
        return m_MessengerFriends.size();
    }

    /// GetFriend
    /// @p_Id : Friend Id which returns information about friend
    MessengerFriendData* Messenger::GetFriend(uint32 const p_Id)
    {
        auto const& l_Itr = m_MessengerFriends.find(p_Id);
        if (l_Itr != m_MessengerFriends.end())
            return &l_Itr->second;
        else
            return nullptr;
    }

    /// SaveToDB
    /// This function is used to query the database on removing friends etc..
    /// Currently not used
    void Messenger::SaveToDB()
    {
    }

    /// ReadMessage
    /// Message which user read and no longer needs to be notified there's a new message
    /// @p_MessageId : Id of message
    void Messenger::ReadMessage(uint32 const p_MessageId)
    {
        QueryDatabase l_Database("users");
        l_Database.PrepareQuery("UPDATE messenger_messages SET has_read = 1 WHERE receiver_id = ? ORDER BY id DESC LIMIT 1");
        l_Database.GetStatement()->setUInt(1, m_Habbo->GetId());
        l_Database.ExecuteQuery();
    }

    /// RemoveFriendRequestFromStorage
    /// @p_Id : Friend Request Id we are removing from our m_MessengerFriendRequests storage
    void Messenger::RemoveFriendRequestFromStorage(uint32 const p_Id)
    {
        auto const& l_Itr = m_MessengerFriendRequests.find(p_Id);
        if (l_Itr != m_MessengerFriendRequests.end())
            m_MessengerFriendRequests.erase(l_Itr);
    }
    
    /// ParseMessengerInitialize 
    /// Initialize our console, this is called when habbo logs in
    /// @p_Buffer : Buffer which is being parsed
    void Messenger::ParseMessengerInitialize(StringBuffer& p_Buffer)
    {
        for (auto const& l_Itr : m_MessengerFriends)
        {
            MessengerFriendData const& l_Friend = l_Itr.second;
            Habbo const* l_Habbo = sHotel->FindHabbo(l_Friend.GetId());

            p_Buffer.AppendWired(l_Friend.GetId());
            p_Buffer.AppendString(l_Friend.GetName());
            p_Buffer.AppendWired(l_Friend.GetGender() == "Male" ? true : false);
            p_Buffer.AppendString(l_Friend.GetConsoleMotto());

            p_Buffer.AppendWired(l_Habbo ? true : false);  ///< Is user online?

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
                p_Buffer.AppendString(l_Friend.GetLastOnline()); ///< Last time user was online

            p_Buffer.AppendString(l_Friend.GetLastOnline());
            p_Buffer.AppendString(l_Friend.GetFigure());
        }
    }
    
    /// ParseMessengerFriendRequest
    /// Update our console if we have any friend requests in our m_MessengerFriendRequests storage
    void Messenger::ParseMessengerFriendRequest()
    {
        for (auto const& l_Itr : m_MessengerFriendRequests)
        {
            MessengerFriendData const& l_Friend = l_Itr.second;

            HabboPacket::Messenger::MessengerBuddyRequest l_Packet;
            l_Packet.Id = l_Friend.GetId();
            l_Packet.Name = l_Friend.GetName();
            m_Habbo->ToSocket()->SendPacket(l_Packet.Write());
        }
    }

    /// ParseMessengerUpdate
    /// Update what our friends are up to
    /// @p_Buffer : Buffer which is being parsed
    void Messenger::ParseMessengerUpdate(StringBuffer& p_Buffer)
    {
        /// Query the Database again, when we update console
        UpdateConsole();

        p_Buffer.AppendWired(m_MessengerFriends.size());

        for (auto const& l_Itr : m_MessengerFriends)
        {
            MessengerFriendData const& l_Friend = l_Itr.second;
            Habbo const* l_Habbo = sHotel->FindHabbo(l_Friend.GetId());

            p_Buffer.AppendWired(l_Friend.GetId());
            p_Buffer.AppendString(l_Friend.GetConsoleMotto());
            p_Buffer.AppendWired(l_Habbo ? true : false); ///< Is user online?

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
                p_Buffer.AppendString(l_Friend.GetLastOnline()); ///< Last time user was online
        }
    }
    
    /// ParseMessengerAcceptFriendRequest
    /// @p_SenderId : Account Id who sent friend request
    void Messenger::ParseMessengerAcceptFriendRequest(uint32 const p_SenderId)
    {
        QueryDatabase l_Database("users");
        l_Database.PrepareQuery("SELECT id, user_name, figure, console_motto, gender, last_online, allow_friend_requests, subscribed, messenger_requests.from_id, messenger_friends.to_id FROM account LEFT JOIN messenger_requests ON messenger_requests.to_id = account.id LEFT JOIN messenger_friends ON messenger_friends.from_id = account.id WHERE(account.id = ?)");
        l_Database.GetStatement()->setUInt(1, p_SenderId);
        l_Database.ExecuteQuery();

        HabboPacket::Messenger::BuddyRequestResult l_Packet;
        l_Packet.Error = MessengerErrorCode::ACCEPT_SUCCESS;

        /// Habbo doesn't exist? Internal error
        if (!l_Database.GetResult())
        {
            LOG_FATAL << "Habbo " << m_Habbo->GetName() << " tried to accept friend request from Habbo Id " << p_SenderId << " but Habbo does not exist!";
            l_Packet.Error = MessengerErrorCode::CONCURRENCY_ERROR;
            m_Habbo->ToSocket()->SendPacket(l_Packet.Write());
            return;
        }

        Result* l_Result = l_Database.Fetch();

        /// Check if target user friends is full
        uint32 l_FriendsLimit = 0;
        if (m_Habbo->IsSubscribed())
            l_FriendsLimit = sHotel->GetIntConfig(CONFIG_MESSENGER_MAX_CLUB_FRIENDS);
        else
            l_FriendsLimit = sHotel->GetIntConfig(CONFIG_MESSENGER_MAX_FRIENDS);

        if (l_Result->GetRowCount() >=  l_FriendsLimit)
            l_Packet.Error = MessengerErrorCode::TARGET_FRIEND_LIST_FULL;

        /// Does target user already exist on friend list?
        if (l_Result->GetUint32(10) == m_Habbo->GetId())
            l_Packet.Error = MessengerErrorCode::TARGET_DOES_NOT_ACCEPT;

        /// Does target user accept friend requests?
        if (!l_Result->GetBool(7))
            l_Packet.Error = MessengerErrorCode::TARGET_DOES_NOT_ACCEPT;

        if (l_Packet.Error != MessengerErrorCode::ACCEPT_SUCCESS)
        {
            m_Habbo->ToSocket()->SendPacket(l_Packet.Write());
            return;
        }

        /// Succesfull! Insert into our messenger friends for both users
        /// TODO; Is there anyway to clean this?
        /// I don't feel comfortable on querying the database every time we accept a new friend request
        l_Database.PrepareQuery("INSERT INTO messenger_friends (from_id, to_id) VALUES (?, ?) ON DUPLICATE KEY UPDATE to_id = ?");
        l_Database.GetStatement()->setUInt(1, m_Habbo->GetId());
        l_Database.GetStatement()->setUInt(2, l_Result->GetUint32(1));
        l_Database.GetStatement()->setUInt(3, l_Result->GetUint32(1));
        l_Database.ExecuteQuery();

        l_Database.ClearParameters();

        l_Database.GetStatement()->setUInt(1, l_Result->GetUint32(1));
        l_Database.GetStatement()->setUInt(2, m_Habbo->GetId());
        l_Database.GetStatement()->setUInt(3, m_Habbo->GetId());
        l_Database.ExecuteQuery();

        l_Database.ClearParameters();

        /// Remove friend request
        l_Database.PrepareQuery("DELETE FROM messenger_requests WHERE from_id = ? AND to_id = ?");
        l_Database.GetStatement()->setUInt(1, l_Result->GetUint32(1));
        l_Database.GetStatement()->setUInt(2, m_Habbo->GetId());
        l_Database.ExecuteQuery();

        /// Send Packet to update console with new friend!
        /// Is User online?
        Habbo* l_Habbo = sHotel->FindHabbo(l_Result->GetUint32(1));

        HabboPacket::Messenger::AddBuddy l_PacketAddFriend;
        l_PacketAddFriend.Id            = l_Result->GetUint32(1);
        l_PacketAddFriend.Name          = l_Result->GetString(2);
        l_PacketAddFriend.Gender        = l_Result->GetString(5) == "Male" ? true : false;
        l_PacketAddFriend.ConsoleMotto  = l_Result->GetString(4);
        l_PacketAddFriend.IsOnline      = l_Habbo ? true : false;
        
        if (l_PacketAddFriend.IsOnline)
        {
            /// Is user in room>
            if (l_Habbo->GetRoom())
            {
                /// Is user in a public room or flat?
                if (l_Habbo->GetRoom()->GetOwnerId() > 0)
                    l_PacketAddFriend.Status = "Floor1a";
                else
                    l_PacketAddFriend.Status = l_Habbo->GetRoom()->GetName();
            }
            else
                l_PacketAddFriend.Status = "On Hotel View";
        }
        else
            l_PacketAddFriend.Status = l_Result->GetString(6); ///< Last time user was online

        l_PacketAddFriend.LastOnline = l_Result->GetString(6); ///< Last time user was online
        l_PacketAddFriend.Figure = l_Result->GetString(3); ///< User Figure
        
        m_Habbo->ToSocket()->SendPacket(l_PacketAddFriend.Write());

        /// Do the same for other user
        if (l_PacketAddFriend.IsOnline)
        {
            HabboPacket::Messenger::AddBuddy l_PacketAddFriendOther;
            l_PacketAddFriendOther.Id            = m_Habbo->GetId();
            l_PacketAddFriendOther.Name          = m_Habbo->GetName();
            l_PacketAddFriendOther.Gender        = m_Habbo->GetGender() == "Male" ? true : false;
            l_PacketAddFriendOther.ConsoleMotto  = m_Habbo->GetConsoleMotto();

            /// Is user in room>
            if (m_Habbo->GetRoom())
            {
                /// Is user in a public room or flat?
                if (m_Habbo->GetRoom()->GetOwnerId() > 0)
                    l_PacketAddFriendOther.Status = "Floor1a";
                else
                    l_PacketAddFriendOther.Status = m_Habbo->GetRoom()->GetName();
            }
            else
                l_PacketAddFriendOther.Status = "On Hotel View";

            l_PacketAddFriendOther.LastOnline    = GetDate(); ///< Last time user was online
            l_PacketAddFriendOther.Figure        = m_Habbo->GetFigure(); ///< User Figure

            l_Habbo->ToSocket()->SendPacket(l_PacketAddFriendOther.Write());
            l_Habbo->SendMessengerUpdate();
        }
    }
    
    /// ParseMessengerSearchUser
    /// @p_Buffer : Buffer which is being parsed
    /// @p_Name : Name of Habbo we are searching for
    void Messenger::ParseMessengerSearchUser(StringBuffer& p_Buffer, std::string const p_Name)
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
        p_Buffer.AppendWired(l_Result->GetString(5) == "Male" ? true : false);
        p_Buffer.AppendString(l_Result->GetString(4));

        Habbo* l_Habbo = sHotel->FindHabbo(l_Result->GetUint32(1));

        p_Buffer.AppendWired(l_Habbo ? true : false); ///< Is user online?

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

    /// ParseMessengerSendBuddyRequest
    /// @p_Name : Name of Habbo we are sending friend request too
    void Messenger::ParseMessengerSendBuddyRequest(std::string const p_Name)
    {
        QueryDatabase l_Database("users");
        l_Database.PrepareQuery("SELECT id, user_name, figure, console_motto, gender, last_online, allow_friend_requests, subscribed, messenger_requests.from_id, messenger_friends.to_id FROM account LEFT JOIN messenger_requests ON messenger_requests.to_id = account.id LEFT JOIN messenger_friends ON messenger_friends.from_id = account.id WHERE(account.user_name = ?)");
        l_Database.GetStatement()->setString(1, p_Name);
        l_Database.ExecuteQuery();

        HabboPacket::Messenger::ErrorMessenger l_Packet;
        l_Packet.Error = MessengerErrorCode::ACCEPT_SUCCESS;

        /// Target user does not exist
        if (!l_Database.GetResult())
        {
            l_Packet.Error = MessengerErrorCode::FRIEND_REQUEST_NOT_FOUND;
            m_Habbo->ToSocket()->SendPacket(l_Packet.Write());
            return;
        }

        Result* l_Result = l_Database.Fetch();

        /// Check if target user friends is full
        uint32 l_FriendsLimit = 0;
        if (m_Habbo->IsSubscribed())
            l_FriendsLimit = sHotel->GetIntConfig(CONFIG_MESSENGER_MAX_CLUB_FRIENDS);
        else
            l_FriendsLimit = sHotel->GetIntConfig(CONFIG_MESSENGER_MAX_FRIENDS);

        if (l_Result->GetRowCount() >= l_FriendsLimit)
            l_Packet.Error = MessengerErrorCode::TARGET_FRIEND_LIST_FULL;

        /// Does target user already exist on friend request list?
        if (l_Result->GetUint32(7) == m_Habbo->GetId())
            l_Packet.Error = MessengerErrorCode::TARGET_DOES_NOT_ACCEPT;

        /// Does target user already exist on friend list?
        if (l_Result->GetUint32(10) == m_Habbo->GetId())
            l_Packet.Error = MessengerErrorCode::TARGET_DOES_NOT_ACCEPT;

        /// Does target user accept friend requests?
        if (!l_Result->GetBool(7))
            l_Packet.Error = MessengerErrorCode::TARGET_DOES_NOT_ACCEPT;

        if (l_Packet.Error != MessengerErrorCode::ACCEPT_SUCCESS)
        {
            m_Habbo->ToSocket()->SendPacket(l_Packet.Write());
            return;
        }

        ///< All Good send friend request notification to player if online
        if (Habbo* l_Habbo = sHotel->FindHabbo(l_Result->GetUint32(1)))
        {
            HabboPacket::Messenger::MessengerBuddyRequest l_Packet;
            l_Packet.Id = m_Habbo->GetId();
            l_Packet.Name = m_Habbo->GetName();
            l_Habbo->ToSocket()->SendPacket(l_Packet.Write());
        }
        else
        {
            /// Insert friend request into database if user is not online
            /// So user will pick up friend request the next time he logs in
            /// TODO; What happens if user is online and goes offline and not accept the friend request?
            /// the friend request will go limbo - not good!
            l_Database.PrepareQuery("INSERT INTO messenger_requests (from_id, to_id) VALUES (?, ?) ON DUPLICATE KEY UPDATE to_id = ?");
            l_Database.GetStatement()->setUInt(1, m_Habbo->GetId());
            l_Database.GetStatement()->setUInt(2, l_Result->GetUint32(1));
            l_Database.GetStatement()->setUInt(3, l_Result->GetUint32(1));
            l_Database.ExecuteQuery();
        }
    }

    /// ParseMessengerRemoveBuddy
    /// @p_Packet : Incoming client packet which we will decode
    void Messenger::ParseMessengerRemoveBuddy(std::unique_ptr<ClientPacket> p_Packet)
    {
        HabboPacket::Messenger::MessengerRemoveBuddy l_Packet;

        uint32 l_Size = p_Packet->ReadWiredUint();

        /// Read the packet and obtain our friend Ids
        for (uint8 l_I = 0; l_I < l_Size; l_I++)
        {
            uint32 l_Id = p_Packet->ReadWiredUint();

            auto const& l_Itr = m_MessengerFriends.find(l_Id);
            /// Does friend exist in console?
            if (l_Itr == m_MessengerFriends.end())
            {
                LOG_ERROR << "Tried to remove friend Id: " << l_Id << " from " << m_Habbo->GetName() << " but friend doesn't exist!";
                HabboPacket::Messenger::ErrorMessenger l_PacketError;
                l_PacketError.Error = MessengerErrorCode::CONCURRENCY_ERROR;
                m_Habbo->ToSocket()->SendPacket(l_PacketError.Write());
                continue;
            }

            /// TODO; We should move this to SaveToDB function
            /// We store removed friends into a storage which we can loop through
            /// when habbo logs out, we need to create a async thread to do this operation,
            /// so when it comes to updating the database on misc querys we can do it on another thread
            /// this way we don't cause any lag if a habbo is mass removing friends as we don't do 
            /// one thread per player
            QueryDatabase l_Database("users");
            l_Database.PrepareQuery("DELETE from messenger_friends WHERE from_id = ? AND to_id = ?");
            l_Database.GetStatement()->setUInt(1, m_Habbo->GetId());
            l_Database.GetStatement()->setUInt(2, l_Id);
            l_Database.ExecuteQuery();

            l_Database.ClearParameters();

            l_Database.GetStatement()->setUInt(1, l_Id);
            l_Database.GetStatement()->setUInt(2, m_Habbo->GetId());
            l_Database.ExecuteQuery();

            m_MessengerFriends.erase(l_Itr);
            l_Packet.FriendsId.push_back(l_Id);

            /// If our ex-friend is online send remove packet to user aswell
            if (Habbo* l_Habbo = sHotel->FindHabbo(l_Id))
            {
                HabboPacket::Messenger::MessengerRemoveBuddy l_ExFriendPacket;
                l_ExFriendPacket.FriendsId.push_back(m_Habbo->GetId());
                l_Habbo->ToSocket()->SendPacket(l_ExFriendPacket.Write());

                /// And also update users console
                l_Habbo->SendMessengerUpdate();
            }
        }
        m_Habbo->ToSocket()->SendPacket(l_Packet.Write());
    }

    /// ParseMessengerRejectBuddy
    /// @p_Packet : Incoming client packet which we will decode
    void Messenger::ParseMessengerRejectBuddy(std::unique_ptr<ClientPacket> p_Packet)
    {
        bool l_DeclineAll = p_Packet->ReadWiredBool();

        /// Remove all friend requests relating to habbo Id
        if (l_DeclineAll)
        {
            QueryDatabase l_Database("users");
            l_Database.PrepareQuery("DELETE FROM messenger_requests WHERE to_id = ?");
            l_Database.GetStatement()->setUInt(1, m_Habbo->GetId());
            l_Database.ExecuteQuery();
            m_MessengerFriendRequests.clear(); ///< Clear our storage
            return;
        }

        uint32 l_Size = p_Packet->ReadWiredUint();

        /// Read the packet and obtain our friend Ids
        for (uint8 l_I = 0; l_I < l_Size; l_I++)
        {
            uint32 l_Id = p_Packet->ReadWiredUint();

            QueryDatabase l_Database("users");
            l_Database.PrepareQuery("DELETE from messenger_friends WHERE from_id = ? AND to_id = ?");
            l_Database.GetStatement()->setUInt(1, l_Id);
            l_Database.GetStatement()->setUInt(2, m_Habbo->GetId());
            l_Database.ExecuteQuery();
            RemoveFriendRequestFromStorage(l_Id);
        }
    }

    /// ParseMessengerSendMessage
    /// @p_Packet : Incoming client packet which we will decode
    void Messenger::ParseMessengerSendMessage(std::unique_ptr<ClientPacket> p_Packet)
    {
        /// TODO; Add a config check on max friends habbo can send to
        uint32 l_Size = p_Packet->ReadWiredUint();

        std::set<uint32> l_Friends;
        for (uint8 l_I = 0; l_I < l_Size; l_I++)
        {
            uint32 const& l_Id = p_Packet->ReadWiredUint();
            l_Friends.insert(l_Id);
        }
        std::string l_Message = p_Packet->ReadString();

        for (auto const& l_Itr : l_Friends)
        {
            MessengerFriendData const* l_Friend = GetFriend(l_Itr);

            if (!l_Friend)
                continue;

            /// Is Habbo user online? If so send packet to let user know he recieved a new message
            if (Habbo* l_Habbo = sHotel->FindHabbo(l_Friend->GetId()))
            {
                HabboPacket::Messenger::SendMessage l_Packet;
                l_Packet.Id      = m_Habbo->GetId();
                l_Packet.ToId    = l_Friend->GetId();
                l_Packet.Date    = GetDate();
                l_Packet.Message = l_Message;
                
                l_Habbo->ToSocket()->SendPacket(l_Packet.Write());
            }

            /// Save message to the database
            QueryDatabase l_Database("users");
            l_Database.PrepareQuery("INSERT INTO messenger_messages (receiver_id, sender_id, body) VALUES (?, ?, ?)");
            l_Database.GetStatement()->setUInt(1, l_Friend->GetId());
            l_Database.GetStatement()->setUInt(2, m_Habbo->GetId());
            l_Database.GetStatement()->setString(3, l_Message);
            l_Database.ExecuteQuery();
        }
    }
} ///< NAMESPACE MESSENGER
