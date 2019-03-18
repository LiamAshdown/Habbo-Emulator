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
#include <mutex>

namespace Quad
{
    typedef std::unordered_map<uint32, Player*> PlayerMap;

    class World
    {
    public:
        static World* instance();

    public:
        World();
        ~World();

    public:
        void AddListener(const uint16 port);

        Player* FindPlayer(const uint32& mId) const;
        void AddPlayer(const uint32& mId, Player* currentSession);
        void RemovePlayer(const uint32& mId);

        static bool StopWorld();
        void UpdateWorld();

        void CleanUp();

    private:
        std::vector<std::unique_ptr<Listener<PlayerSocket>>> mListener;
        PlayerMap mPlayers;
        std::mutex mMutex;

        static volatile bool mStopWorld;
    };
}

#define sWorld Quad::World::instance()

#endif /* _Quad_World_h_ */