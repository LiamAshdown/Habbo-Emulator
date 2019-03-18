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
//-----------------------------------------------//
#include "Messenger.h"
#include "Database/QueryDatabase.h"
#include "Player.h"
#include "World.h"
//-----------------------------------------------//
namespace Quad
{
    //-----------------------------------------------//
    Messenger::Messenger(const uint32& id) : mId(id)
    {
    }
    //-----------------------------------------------//
    Messenger::~Messenger()
    {
    }
    //-----------------------------------------------//
    void Messenger::LoadMessenger()
    {
        LoadMessengerFriends();
        LoadMessengerFriendRequests();
    }
    //-----------------------------------------------//
    void Messenger::LoadMessengerFriends()
    {
        QueryDatabase database("users");
        database.PrepareQuery("SELECT id, user_name, figure, console_motto, gender, last_online FROM messenger_friends INNER JOIN account ON messenger_friends.to_id = account.id WHERE(messenger_friends.from_id = ?)");
        database.GetStatement()->setUInt(1, mId);
        database.ExecuteQuery();

        if (!database.GetResult())
            return;

        Result* result = database.Fetch();

        std::unique_ptr<MessengerFriends> messenger = std::make_unique<MessengerFriends>();
        messenger->mId = result->GetUint32(1);
        messenger->mName = result->GetString(2);
        messenger->mFigure = result->GetString(3);
        messenger->mConsoleMotto = result->GetString(4);
        messenger->mGender = result->GetString(5);
        messenger->mLastOnline = result->GetString(6);

        mMessengerFriends.push_back(std::move(messenger));
    }
    //-----------------------------------------------//
    void Messenger::LoadMessengerFriendRequests()
    {
        QueryDatabase database("users");
        database.PrepareQuery("SELECT id, user_name, figure, console_motto, gender, last_online FROM messenger_requests INNER JOIN account ON messenger_requests.from_id = account.id WHERE(messenger_requests.to_id = ?)");
        database.GetStatement()->setUInt(1, mId);
        database.ExecuteQuery();

        if (!database.GetResult())
            return;

        Result* result = database.Fetch();

        std::unique_ptr<MessengerFriends> messenger = std::make_unique<MessengerFriends>();
        messenger->mId = result->GetUint32(1);
        messenger->mName = result->GetString(2);
        messenger->mFigure = result->GetString(3);
        messenger->mConsoleMotto = result->GetString(4);
        messenger->mGender = result->GetString(5);
        messenger->mLastOnline = result->GetString(6);

        mMessengerFriendRequests.push_back(std::move(messenger));
    }
    //-----------------------------------------------//
    void Messenger::UpdateConsole()
    {
        mMessengerFriends.clear();

        // When we recieve console update, query the database
        QueryDatabase database("users");
        database.PrepareQuery("SELECT id, user_name, figure, console_motto, gender, last_online FROM messenger_friends INNER JOIN account ON messenger_friends.to_id = account.id WHERE(messenger_friends.from_id = ?)");
        database.GetStatement()->setUInt(1, mId);
        database.ExecuteQuery();

        if (!database.GetResult())
            return;

        Result* result = database.Fetch();

        std::unique_ptr<MessengerFriends> messenger = std::make_unique<MessengerFriends>();
        messenger->mId = result->GetUint32(1);
        messenger->mName = result->GetString(2);
        messenger->mFigure = result->GetString(3);
        messenger->mConsoleMotto = result->GetString(4);
        messenger->mGender = result->GetString(5);
        messenger->mLastOnline = result->GetString(6);

        mMessengerFriends.push_back(std::move(messenger));
    }
    //-----------------------------------------------//
    void Messenger::ParseMessengerFriends(StringBuffer& buffer)
    {
        buffer.AppendWired(mMessengerFriends.size());

        for (MessengerFriendsVector::const_iterator itr = mMessengerFriends.begin(); itr != mMessengerFriends.end(); itr++)
        {
            Player* player = sWorld->FindPlayer((*itr)->GetId());

            buffer.AppendWired((*itr)->GetId());
            buffer.AppendString((*itr)->GetName());
            buffer.AppendWiredBool((*itr)->GetGender() == "Male" ? true : false);
            buffer.AppendString((*itr)->GetConsoleMotto());

            buffer.AppendWiredBool(player ? true : false);

            // TODO; Do room update
            if (player)
                buffer.AppendString("On hotel view");
            else
                buffer.AppendString((*itr)->GetLastOnline());

            buffer.AppendString((*itr)->GetLastOnline());
            buffer.AppendString((*itr)->GetFigure());
        }
    }
    //-----------------------------------------------//
    void Messenger::ParseMessengerFriendRequests(StringBuffer& buffer)
    {
        for (MessengerFriendRequestsVector::const_iterator itr = mMessengerFriendRequests.begin(); itr != mMessengerFriendRequests.end(); itr++)
        {
            buffer.AppendWired((*itr)->GetId());
            buffer.AppendString((*itr)->GetName());
        }
    }
    //-----------------------------------------------//
    void Messenger::ParseMessengerUpdate(StringBuffer& buffer)
    {
        // Query the database again, when we update console
        UpdateConsole();

        buffer.AppendWired(mMessengerFriends.size());

        for (MessengerFriendsVector::const_iterator itr = mMessengerFriends.begin(); itr != mMessengerFriends.end(); itr++)
        {
            Player* player = sWorld->FindPlayer((*itr)->GetId());

            buffer.AppendWired((*itr)->GetId());
            buffer.AppendString((*itr)->GetConsoleMotto());
            buffer.AppendWiredBool(player ? true : false);

            // TODO; Do room update
            if (player)
                buffer.AppendString("On hotel view");
            else
                buffer.AppendString((*itr)->GetLastOnline());
        }
    }
    //-----------------------------------------------//
}
//-----------------------------------------------//