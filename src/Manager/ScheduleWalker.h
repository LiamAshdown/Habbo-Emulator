/*
* Liam Ashdown
* Copyright (C) 2018
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

#ifndef _QuadEmu_ScheduleWalker_h_
#define _QuadEmu_ScheduleWalker_h_
#include "../Common/SharedDefines.h"
#include "../Pathfinding/PathFinding.h"
#include <boost/interprocess/sync/scoped_lock.hpp>
#include <boost/thread/mutex.hpp>
#include <chrono>
#endif /* _QuadEmu_ScheduleWalker_ */

class Player;

class WalkerWorker
{
public:
    WalkerWorker(PathFinder* pathfinder);

public:
    PathFinder* mPathFinder;
    uint32 mPosition;
};

typedef std::map<uint32, WalkerWorker*> MapWalkers;

class ScheduleWalker
{
public:
    static ScheduleWalker* instance();

    ScheduleWalker();
    ~ScheduleWalker();

    void ScheduleWalk(Player* player, uint8& x, uint8& y);
    void WalkUpdate();

private:
    static boost::mutex& GetMutex();
    bool mIsDestructed;
    MapWalkers mWalkers;
};
#define sScheduleWalker ScheduleWalker::instance()
