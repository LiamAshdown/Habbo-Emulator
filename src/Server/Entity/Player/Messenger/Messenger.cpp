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
    Messenger::Messenger()
    {
    }
    //-----------------------------------------------//
    Messenger::~Messenger()
    {
        IF_LOG(plog::debug)
            LOG_DEBUG << "Destructor Messenger called!";
    }
    //-----------------------------------------------//
    void Messenger::LoadMessenger(const uint32& id)
    {
        QueryDatabase database("users");
        database.PrepareQuery("SELECT id, user_name, figure, console_motto, gender, last_online FROM messenger_friends INNER JOIN accounts ON messenger_friends.to_id = accounts.id WHERE(messenger_friends.from_id = ?)");
        database.GetStatement()->setUInt(1, id);
        database.ExecuteQuery();

        if (!database.GetResult())
            return;

        Field* fields = database.Fetch();

        std::unique_ptr<MessengerFriends> messenger = std::make_unique<MessengerFriends>();
        messenger->mId = fields->GetUint32(1);
        messenger->mName = fields->GetString(2);
        messenger->mFigure = fields->GetString(3);
        messenger->mMotto = fields->GetString(4);
        messenger->mGender = fields->GetString(5);
        messenger->mLastOnline = fields->GetString(6);

        mMessengerFriends.push_back(std::move(messenger));
    }
    //-----------------------------------------------//
    void Messenger::ParseFriendData(StringBuffer& buffer)
    {
        buffer.AppendWired(mMessengerFriends.size());

        for (MessengerFriendsMap::const_iterator itr = mMessengerFriends.begin(); itr != mMessengerFriends.end(); itr++)
        {
            Player* player = sWorld->FindPlayer((*itr)->GetId());

            buffer.AppendWired((*itr)->GetId());
            buffer.AppendString((*itr)->GetName());
            buffer.AppendWiredBool((*itr)->GetGender() == "Male" ? "m" : "f");
            buffer.AppendString((*itr)->GetMotto());

            if (player)
            {
                buffer.AppendWiredBool(1);
                buffer.AppendString("On hotel view");
            }
            else
            {
                buffer.AppendWired(0);
                buffer.AppendString((*itr)->GetLastOnline());
            }

            buffer.AppendString((*itr)->GetLastOnline());
            buffer.AppendString((*itr)->GetFigure());
        }
    }
    //-----------------------------------------------//
}
//-----------------------------------------------//

/*if (isOnline) 
{
    if (player.getRoomUser().getRoom() != null) 
    {
        Room room = player.getRoomUser().getRoom();

        if (room.getData().getOwnerId() > 0) 
            response.writeString("Floor1a");
        else 
            response.writeString(room.getData().getPublicName());
    }
    else 
        response.writeString("On hotel view");
}
else
    response.writeString(DateUtil.getDateAsString(this.lastOnline));*/
