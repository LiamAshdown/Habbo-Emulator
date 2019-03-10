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
//-----------------------------------------------//
namespace Quad
{
    //-----------------------------------------------//
    World* World::instance()
    {
        static World instance;
        return &instance;
    }
    //-----------------------------------------------//
    World::World()
    {
        mPool.reset(new ThreadPool(1));

        mPool->Enqueue(std::bind(&World::UpdateWorld, this));
    }
    //-----------------------------------------------//
    World::~World()
    {
        mPool.reset();
    }
    //-----------------------------------------------//
    void World::AddListener(const uint16 port)
    {
        std::unique_ptr<Quad::Listener<Quad::PlayerSocket>> listener = std::make_unique<Quad::Listener<Quad::PlayerSocket>>(sConfig->GetStringDefault("BindIP", "127.0.0.1"), port,
            sConfig->GetIntDefault("NetworkThreadProcessors", 1));

        mListener.push_back(std::move(listener));

        LOG_INFO << "Added listener port: " << port;
    }
    //-----------------------------------------------//
    void World::UpdateWorld()
    {
        Timer timer;

        while (true)
        {
            // Update the world every 50 ms
            if (timer.Elasped() < UPDATE_WORLD_TIMER)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds((uint32)(UPDATE_WORLD_TIMER - timer.Elasped())));
                timer.Reset();
            }
        }
    }
    //-----------------------------------------------//
}
//-----------------------------------------------//