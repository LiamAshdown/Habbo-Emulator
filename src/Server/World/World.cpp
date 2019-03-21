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
#include "World.h"
#include "Platform/Thread/ThreadPool.h"
#include "Common/Timer.h"
#include "Player.h"
#include "Config/Config.h"
#include "Database/QueryDatabase.h"
//-----------------------------------------------//
namespace Quad
{
    //-----------------------------------------------//
    volatile bool World::mStopWorld = false;
    //-----------------------------------------------//
    World* World::instance()
    {
        static World instance;
        return &instance;
    }
    //-----------------------------------------------//
    World::World()
    {
      
    }
    //-----------------------------------------------//
    World::~World()
    {
    }
    //-----------------------------------------------//
    Player* World::FindPlayer(const uint32& mId) const
    {
        PlayerMap::const_iterator itr = mPlayers.find(mId);
        if (itr != mPlayers.end() && itr->second)
            return itr->second;

        return nullptr;
    }
    //-----------------------------------------------//
    void World::AddPlayer(Player* player)
    {
        std::lock_guard<std::mutex> guard(mMutex);

        PlayerMap::const_iterator itr = mPlayers.find(player->GetId());
        if (itr != mPlayers.end())
            delete itr->second;

        mPlayers[player->GetId()] = player;
    }
    //-----------------------------------------------//
    void World::RemovePlayer(const uint32& mId)
    {
        std::lock_guard<std::mutex> guard(mMutex);

        PlayerMap::const_iterator itr = mPlayers.find(mId);
        if (itr != mPlayers.end())
            itr->second->Logout();
    }
    //-----------------------------------------------//
    void World::CleanUp()
    {
        for (PlayerMap::iterator itr = mPlayers.begin(); itr != mPlayers.end();)
        {
            Player* player = itr->second;
            player->Logout();

            itr = mPlayers.erase(itr);
            delete player;
        }

        mPlayers.clear();
    }
    //-----------------------------------------------//
    bool World::StopWorld()
    {
        return mStopWorld;
    }
    //-----------------------------------------------//
    void World::UpdateWorld(const uint32& diff)
    {
        for (PlayerMap::iterator itr = mPlayers.begin(); itr != mPlayers.end();)
        {
            Player* player = itr->second;

            if (!player->Update(diff))
            {
                player->Logout();

                itr = mPlayers.erase(itr);
                delete player;
            }
            else
                ++itr;
        }
    }
    //-----------------------------------------------//
}
//-----------------------------------------------//