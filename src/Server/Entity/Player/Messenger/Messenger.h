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

namespace Quad
{
    typedef struct MessengerFriendsStruct
    {
    public:
        friend class Messenger;

    public:
        MessengerFriendsStruct() {}
        ~MessengerFriendsStruct() {}

    public:
        uint32 GetId() const { return mId; }
        std::string GetFigure() const { return mFigure; }
        std::string GetName() const { return mName; }
        std::string GetGender() const { return mGender; }
        std::string GetMotto() const { return mMotto; }
        std::string GetLastOnline() const { return mLastOnline; }

    private:
        uint32 mId;
        std::string mFigure;
        std::string mName;
        std::string mGender;
        std::string mMotto;
        std::string mLastOnline;
    }MessengerFriends;

    typedef std::vector<std::unique_ptr<MessengerFriends>> MessengerFriendsMap;

    class Messenger
    {
    public:

    public:
        Messenger();
        ~Messenger();

    public:
        void LoadMessenger(const uint32& id);

        void ParseFriendData(StringBuffer& buffer);

    private:
        MessengerFriendsMap mMessengerFriends;
    };
}

#endif /* _Quad_Messenger_h_ */