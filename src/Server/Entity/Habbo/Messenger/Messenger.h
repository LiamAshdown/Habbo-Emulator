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

    /// Structure which holds information about Habbo
    typedef struct MessengerFriendsStruct
    {
    public:
        friend class Messenger;

    public:
        /// Constructor
        MessengerFriendsStruct() : m_Id(0), m_Figure(""), m_Gender(""), m_ConsoleMotto(""), mLastOnline(""), mFriendsSize(0) {}
        /// Deconstructor
        ~MessengerFriendsStruct() {}

    public:
        /// Get Info about Habbo
        uint32 GetId()                const { return m_Id;          }
        std::string GetFigure()       const { return m_Figure;      }
        std::string GetName()         const { return m_Name;        }
        std::string GetGender()       const { return m_Gender;      }
        std::string GetConsoleMotto() const { return m_ConsoleMotto;}
        std::string GetLastOnline()   const { return mLastOnline;   }
        uint32 GetFriendsSize()       const { return mFriendsSize;  }

    private:
        /// Variables
        uint32 m_Id;
        std::string m_Figure;
        std::string m_Name;
        std::string m_Gender;
        std::string m_ConsoleMotto;
        std::string mLastOnline;
        uint32 mFriendsSize;
    }MessengerFriendsData;

    typedef std::vector<MessengerFriendsData> MessengerFriendsVector;
    typedef std::vector<MessengerFriendsData> MessengerFriendRequestsVector;

    /// This class is responsible for handling Habbo Messenger Console
    class Messenger
    {
    public:
        /// Constructor
        /// @p_Id : Account Id, we don't use Habbo class here
        explicit Messenger(uint32 const& p_Id);

        /// Deconstructor
        ~Messenger();

    public:
        /// LoadMessenger - Load our messenger
        void LoadMessenger();

        /// LoadMessengerFriends - Load our Messenger Friends from database
        void LoadMessengerFriends();

        /// LoadMessengerFriendRequests - Load our Messenger Friend requests from database
        void LoadMessengerFriendRequests();

        /// UpdateConsole - Reload LoadMessengerFriendRequests and LoadMessengerFriends
        void UpdateConsole();

        /// HasFriendRequests - Check if we have any friend requests
        bool HasFriendRequest() const;

        /// CanSendFriendRequest - Check if our friend list is full
        bool CanSendFriendRequest() const;

        /// ParseMessengerFriends
        /// @p_Buffer : Buffer which is being parsed
        void ParseMessengerFriends(StringBuffer& p_Buffer);

        /// ParseMessengerFriendRequests
        /// @p_Buffer : Buffer which is being parsed
        void ParseMessengerFriendRequests(StringBuffer& p_Buffer);

        /// ParseMessengerUpdate
        /// @p_Buffer : Buffer which is being parsed
        void ParseMessengerUpdate(StringBuffer& p_Buffer);

        /// ParseMessengerAcceptRequest
        /// @p_Buffer : Buffer which is being parsed
        /// @p_SenderId : Account Id who sent friend request
        void ParseMessengerAcceptRequest(StringBuffer& p_Buffer, uint32 const& p_SenderId);

        /// ParseMessengerAcceptRequest
        /// @p_Buffer : Buffer which is being parsed
        /// @p_Name : Name of Habbo we are searching for
        void ParseMessengerSearchUser(StringBuffer& p_Buffer, const std::string& p_Name);

        /// ParseMessengerSendFriendRequest
        /// @p_Habbo : Habbo Class incase we need to send error message to client who sending friend request
        /// @p_Name : Name of Habbo we are sending friend request too
        void ParseMessengerSendFriendRequest(Habbo* p_Habbo, const std::string& p_Name);

    private:
        /// Variables
        MessengerFriendsVector m_MessengerFriends;                          ///< Vector storage which holds our messenger friends
        MessengerFriendRequestsVector m_MessengerFriendRequests;            ///< Vector storage which holds our messenger friends requests
        uint32 m_Id;                                                        ///< Account Id, we don't use Habbo class here, not needed
    };
} ///< NAMESPACE STEERSTONE