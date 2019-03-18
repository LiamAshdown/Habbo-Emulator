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
    void World::AddListener(const uint16 port)
    {
        std::unique_ptr<Quad::Listener<Quad::PlayerSocket>> listener = std::make_unique<Quad::Listener<Quad::PlayerSocket>>(sConfig->GetStringDefault("BindIP", "127.0.0.1"), port,
            sConfig->GetIntDefault("NetworkThreadProcessors", 1));

        mListener.push_back(std::move(listener));
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
    void World::AddPlayer(const uint32& mId, Player* currentSession)
    {
        std::lock_guard<std::mutex> guard(mMutex);

        if (!mPlayers.count(mId))
            mPlayers[mId] = currentSession;
    }
    //-----------------------------------------------//
    void World::RemovePlayer(const uint32& mId)
    {
        std::lock_guard<std::mutex> guard(mMutex);

        PlayerMap::const_iterator itr = mPlayers.find(mId);
        if (itr->second && itr != mPlayers.end())
            itr->second->ToSocket()->CloseSocket();
    }
    //-----------------------------------------------//
    void World::CleanUp()
    {
        for (PlayerMap::iterator itr = mPlayers.begin(); itr != mPlayers.end();)
        {
            Player* player = itr->second;

            itr = mPlayers.erase(itr);
            delete player;
        }

        mPlayers.clear();
        mListener.clear();
    }
    //-----------------------------------------------//
    bool World::StopWorld()
    {
        return mStopWorld;
    }
    //-----------------------------------------------//
    void World::UpdateWorld()
    {
        for (PlayerMap::iterator itr = mPlayers.begin(); itr != mPlayers.end();)
        {
            Player* player = itr->second;

            if (!player->Update())
            {
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