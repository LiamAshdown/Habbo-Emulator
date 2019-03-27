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

#ifndef _HABBO_MESSENGER_h
#define _HABBO_MESSENGER_h
#include "Common/SharedDefines.h"
#endif /* _HABBO_MESSENGER_h */

namespace SteerStone
{
    class StringBuffer;
    class Habbo;
    class ClientPacket;

    /// Structure which holds information about Habbo
    typedef struct MessengerFriendsStruct
    {
    public:
        friend class Messenger;

    public:
        /// Constructor
        MessengerFriendsStruct() : m_Id(0), m_Figure(""), m_Gender(""), m_ConsoleMotto(""), m_LastOnline(""), m_FriendsSize(0) {}
        /// Deconstructor
        ~MessengerFriendsStruct() {}

    public:
        /// Get Info about Habbo for console
        uint32 GetId()                const { return m_Id;          }
        std::string GetFigure()       const { return m_Figure;      }
        std::string GetName()         const { return m_Name;        }
        std::string GetGender()       const { return m_Gender;      }
        std::string GetConsoleMotto() const { return m_ConsoleMotto;}
        std::string GetLastOnline()   const { return m_LastOnline;  }
        uint32 GetFriendsSize()       const { return m_FriendsSize; }

    private:
        /// Variables
        uint32 m_Id;
        std::string m_Figure;
        std::string m_Name;
        std::string m_Gender;
        std::string m_ConsoleMotto;
        std::string m_LastOnline;
        uint32 m_FriendsSize;
    }MessengerFriendData;

    typedef std::unordered_map<uint32, MessengerFriendData> MessengerFriendsMap;

    /// This class is responsible for handling Habbo Messenger Console
    class Messenger
    {
    public:
        /// Constructor
        /// @p_Habbo : Habbo class
        explicit Messenger(Habbo* p_Habbo);

        /// Deconstructor
        ~Messenger();

    public:

        void LoadMessengerFriends();

        void LoadMessengerFriendRequests();

        void LoadMessengerMessages();

        /// UpdateConsole
        /// Reload LoadMessengerFriendRequests and LoadMessengerFriends
        void UpdateConsole();

        bool HasFriendRequestPending() const;

        /// IsFriendListFull
        bool IsFriendListFull() const;

        /// GetMessengerSize
        /// Returns size of Messenger Friends Map
        uint32 GetMessengerSize() const;

        /// GetFriend
        /// @p_Id : Friend Id which returns information about friend
        MessengerFriendData& GetFriend(uint32 const& p_Id);

        /// SaveToDB
        /// This function is used to query the database on removing friends etc..
        /// Currently not used
        void SaveToDB();

        /// ReadMessage
        /// Message which user read and no longer needs to be notified there's a new message
        /// @p_MessageId : Id of message
        void ReadMessage(uint32 const& p_MessageId);

        /// RemoveFriendRequestFromStorage
        /// @p_Id : Friend Request Id we are removing from our m_MessengerFriendRequests storage
        void RemoveFriendRequestFromStorage(uint32 const& p_Id);

        /// ParseMessengerInitialize 
        /// Initialize our console, this is called when habbo logs in
        /// @p_Buffer : Buffer which is being parsed
        void ParseMessengerInitialize(StringBuffer& p_Buffer);

        /// ParseMessengerFriendRequest 
        /// Update our console if we have any friend requests in our m_MessengerFriendRequests storage
        void ParseMessengerFriendRequest();

        /// ParseMessengerUpdate
        /// Update what our friends are up to
        /// @p_Buffer : Buffer which is being parsed
        void ParseMessengerUpdate(StringBuffer& p_Buffer);

        /// ParseMessengerAcceptFriendRequest
        /// @p_SenderId : Account Id who sent friend request
        void ParseMessengerAcceptFriendRequest(uint32 const& p_SenderId);

        /// ParseMessengerSearchUser
        /// @p_Buffer : Buffer which is being parsed
        /// @p_Name : Name of Habbo we are searching for
        void ParseMessengerSearchUser(StringBuffer& p_Buffer, std::string const& p_Name);

        /// ParseMessengerSendFriendRequest
        /// @p_Name : Name of Habbo we are sending friend request too
        void ParseMessengerSendFriendRequest(std::string const& p_Name);

        /// ParseMessengerRemoveFriend
        /// @p_Packet : Incoming client packet which we will decode
        void ParseMessengerRemoveFriend(std::unique_ptr<ClientPacket> p_Packet);

        /// ParseMessengerRejectRequest
        /// @p_Packet : Incoming client packet which we will decode
        void ParseMessengerRejectRequest(std::unique_ptr<ClientPacket> p_Packet);

        /// ParseMessengerSendMessage
        /// @p_Packet : Incoming client packet which we will decode
        void ParseMessengerSendMessage(std::unique_ptr<ClientPacket> p_Packet);

    private:
        /// Variables
        MessengerFriendsMap m_MessengerFriends;                             ///< Storage which holds our messenger friends
        MessengerFriendsMap m_MessengerFriendRequests;                      ///< Storage which holds our messenger friends requests
        MessengerFriendsMap M_MessengerRemovedFriends;                      ///< Storage which contains removed friends, which gets processed when habbo logs out
        Habbo* m_Habbo;                                                     ///< Habbo Class which is created when player logs in
    };
} ///< NAMESPACE STEERSTONE