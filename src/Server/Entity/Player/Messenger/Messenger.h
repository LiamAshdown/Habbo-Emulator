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

#ifndef _Quad_Messenger_h_
#define _Quad_Messenger_h_
#include "Common/SharedDefines.h"
#include "Network/StringBuffer.h"

enum MessengerError
{
    ACCEPT_SUCCESS                  = 0,
    TARGET_FRIEND_LIST_FULL         = 2,
    TARGET_DOES_NOT_ACCEPT          = 3,
    FRIEND_REQUEST_NOT_FOUND        = 4,
    BUDDY_REMOVE_ERROR              = 37,
    FRIEND_LIST_FULL                = 39,
    CONCURRENCY_ERROR               = 42
};

namespace Quad
{
    typedef struct MessengerFriendsStruct
    {
    public:
        friend class Messenger;

    public:
        MessengerFriendsStruct() : mId(0), mFigure(""), mGender(""), mConsoleMotto(""), mLastOnline("") {}
        ~MessengerFriendsStruct() {}

    public:
        uint32 GetId()                const { return mId; }
        std::string GetFigure()       const { return mFigure; }
        std::string GetName()         const { return mName; }
        std::string GetGender()       const { return mGender; }
        std::string GetConsoleMotto() const { return mConsoleMotto; }
        std::string GetLastOnline()   const { return mLastOnline; }

    private:
        uint32 mId;
        std::string mFigure;
        std::string mName;
        std::string mGender;
        std::string mConsoleMotto;
        std::string mLastOnline;
    }MessengerFriends;

    typedef std::vector<std::unique_ptr<MessengerFriends>> MessengerFriendsVector;
    typedef std::vector<std::unique_ptr<MessengerFriends>> MessengerFriendRequestsVector;

    class Messenger
    {
    public:

    public:
        explicit Messenger(const uint32& id);
        ~Messenger();

    public:
        void LoadMessenger();
        void LoadMessengerFriends();
        void LoadMessengerFriendRequests();

        void UpdateConsole();
        void AcceptFriendRequest(StringBuffer& buffer, const uint32& senderId);

        void ParseMessengerFriends(StringBuffer& buffer);
        void ParseMessengerFriendRequests(StringBuffer& buffer);
        void ParseMessengerUpdate(StringBuffer& buffer);
        void ParseMessengerAcceptRequest(StringBuffer& buffer, const uint32& senderId, bool success);

    private:
        MessengerFriendsVector mMessengerFriends;
        MessengerFriendRequestsVector mMessengerFriendRequests;

        uint32 mId;
    };
}

#endif /* _Quad_Messenger_h_ */