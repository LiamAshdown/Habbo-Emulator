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

#ifndef _Quad_World_h_
#define _Quad_World_h
#include "Common/SharedDefines.h"
#include "PlayerSocket.h"
#include "Network/Listener.h"
#include "Config/Config.h"
#include "Database/Database.h"
#include "Platform/Thread/ThreadPool.h"

namespace Quad
{
    class World
    {
    public:
        static World* instance();

    public:
        World();
        ~World();

    public:
        void AddListener(const uint16 port);
        void UpdateWorld();

    private:
        std::unique_ptr<ThreadPool> mPool;
        std::vector<std::unique_ptr<Listener<PlayerSocket>>> mListener;
    };
}

#define sWorld Quad::World::instance()

#endif /* _Quad_World_h_ */