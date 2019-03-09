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
#include "Room.h"
#include "Player.h"
//-----------------------------------------------//
namespace Quad
{
    //-----------------------------------------------//
    Room::Room()
    {
    }
    //-----------------------------------------------//
    Room::~Room()
    {
    }
    //-----------------------------------------------//
    uint32 Room::GetId() const
    {
        return mId;
    }
    //-----------------------------------------------//
    std::string Room::GetName() const
    {
        return mName;
    }
    //-----------------------------------------------//
    std::string Room::GetPassword() const
    {
        return mPassword;
    }
    //-----------------------------------------------//
    std::string Room::GetOwnerName() const
    {
        return mOwnerName;
    }
    //-----------------------------------------------//
    std::string Room::GetFloorLevel() const
    {
        return mFloorLevel;
    }
    //-----------------------------------------------//
    std::string Room::GetModel() const
    {
        return mModel;
    }
    //-----------------------------------------------//
    bool Room::GetType() const
    {
        return mType;
    }
    //-----------------------------------------------//
    bool Room::IsEnabled() const
    {
        return mEnabled;
    }
    //-----------------------------------------------//
    bool Room::IsShowOwnerName() const
    {
        return mShowOwnerName;
    }
    //-----------------------------------------------//
    bool Room::IsSuperUser() const
    {
        return mSuperUser;;
    }
    //-----------------------------------------------//
    uint32 Room::GetCurrentIn() const
    {
        return mCurrentIn;
    }
    //-----------------------------------------------//
    uint32 Room::GetMaxIn() const
    {
        return mMaxIn;
    }
    //-----------------------------------------------//
    void Room::AddPlayer(std::shared_ptr<Player> player)
    {
        std::vector<std::shared_ptr<Player>>::iterator itr = std::find(mPlayers.begin(), mPlayers.end(), player);

        if (itr != mPlayers.end())
        {
            LOG_ERROR << "Tried to add player: " << player->GetName() << " but already exists in room!";
            return;
        }

        mPlayers.push_back(player);
    }
    //-----------------------------------------------//
    void Room::RemovePlayer(std::shared_ptr<Player> player)
    {
        std::vector<std::shared_ptr<Player>>::iterator itr = std::find(mPlayers.begin(), mPlayers.end(), player);

        if (itr != mPlayers.end())
        {
            mPlayers.erase(itr);
            return;
        }
        
        LOG_ERROR << "Tried to remove player: " << player->GetName() << " but doesn't exist in room!";
    }
    //-----------------------------------------------//
}
//-----------------------------------------------//