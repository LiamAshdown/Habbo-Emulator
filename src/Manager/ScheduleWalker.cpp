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
//-----------------------------------------------//
#include <thread>
#include <chrono>
#include "ScheduleWalker.h"
#include "../Server/Thread/ThreadPool.h"
#include "../Entity/Player/Player.h"
#include "../Server/WorldSession.h"
#include "../Server/Packet/WorldPacket.h"
#include "../Room/Room.h"
#include <boost/range/adaptor/reversed.hpp>
//-----------------------------------------------//
ScheduleWalker* ScheduleWalker::instance()
{
    boost::mutex::scoped_lock lock(GetMutex());
    static ScheduleWalker instance;
    return &instance;
}
//-----------------------------------------------//
ScheduleWalker::ScheduleWalker()
{
    mIsDestructed = false;
}
//-----------------------------------------------//
ScheduleWalker::~ScheduleWalker()
{
    
}
//-----------------------------------------------//
WalkerWorker::WalkerWorker(PathFinder* pathfinder) : mPathFinder(pathfinder)
{

}
//-----------------------------------------------//
void ScheduleWalker::ScheduleWalk(Player* player, uint8& x, uint8& y)
{
    boost::mutex::scoped_lock lock(GetMutex());
    PathFinder* path = new PathFinder(player, player->GetCache());
    path->FindPath(x, y);
    WalkerWorker* worker = new WalkerWorker(path);
    worker->mPosition = 0;

    mWalkers.push_back(worker);
}
//-----------------------------------------------//
void ScheduleWalker::WalkUpdate()
{
    for (WalkerWorker* itr : mWalkers)
    {
        if (itr->mPosition >= itr->mPathFinder->GetPath().size())
        {
            delete itr->mPathFinder;
            delete itr;
            mWalkers.pop_front();
            continue;
        }

        auto coordinate = itr->mPathFinder->GetPath().at(itr->mPosition);

        int roomHeight = itr->mPathFinder->Map[coordinate.y][coordinate.x];
        roomHeight = roomHeight - 48;

        WorldPacket buffer("# STATUS \r");
        buffer << (std::string)itr->mPathFinder->GetPlayer()->GetName();
        buffer.AppendSpace();
        buffer << (uint8)itr->mPathFinder->GetPlayer()->GetPlayerPositionX();
        buffer.AppendComma();
        buffer << (uint8)itr->mPathFinder->GetPlayer()->GetPlayerPositionY();
        buffer.AppendComma();
        buffer << (uint8)roomHeight;
        buffer.AppendComma();
        buffer << (uint8)itr->mPathFinder->GetPlayer()->CalculateRotation(itr->mPathFinder->GetPlayer()->GetPlayerPositionX(), itr->mPathFinder->GetPlayer()->GetPlayerPositionY(), coordinate.x, coordinate.y);
        buffer.AppendComma();
        buffer << (uint8)itr->mPathFinder->GetPlayer()->CalculateRotation(itr->mPathFinder->GetPlayer()->GetPlayerPositionX(), itr->mPathFinder->GetPlayer()->GetPlayerPositionY(), coordinate.x, coordinate.y);
        buffer.AppendMove();
        buffer << (uint8)coordinate.x;
        buffer.AppendComma();
        buffer << (uint8)coordinate.y;
        buffer.AppendComma();
        buffer << (uint8)roomHeight;
        buffer.AppendForwardSlash();
        buffer.AppendEndCarriage();
        itr->mPathFinder->GetPlayer()->GetRoom()->SendPacketToAll(buffer.Write());
        itr->mPathFinder->GetPlayer()->SetPlayerPosition(coordinate.x, coordinate.y, itr->mPathFinder->GetPlayer()->CalculateRotation(itr->mPathFinder->GetPlayer()->GetPlayerPositionX(), itr->mPathFinder->GetPlayer()->GetPlayerPositionY(), coordinate.x, coordinate.y));
        itr->mPosition++;
        continue;
    }
}
//-----------------------------------------------//
boost::mutex& ScheduleWalker::GetMutex()
{
    static boost::mutex mutex;
    return mutex;
}
//-----------------------------------------------//