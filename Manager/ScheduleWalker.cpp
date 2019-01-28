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
#include "../Pathfinding/PathFinding.h"
//-----------------------------------------------//
ScheduleWalker* ScheduleWalker::instance()
{
    assert(!mIsDestructed);
    (void)mIsDestructed;

    boost::mutex::scoped_lock lock(GetMutex());
    static ScheduleWalker instance;
    return &instance;
}
//-----------------------------------------------//
ScheduleWalker::ScheduleWalker()
{
    mIsDestructed = true;
}
//-----------------------------------------------//
ScheduleWalker::~ScheduleWalker()
{
    
}
//-----------------------------------------------//
void ScheduleWalker::ScheduleWalk(Player* player, uint8& x, uint8& y)
{
    boost::mutex::scoped_lock lock(GetMutex());
    PathFinder path(player, player->GetCache());
    path.FindPath(x, y);

    WalkerWorker worker(player, path);

    mWalkers.push_back(&worker);
}
//-----------------------------------------------//
void ScheduleWalker::WalkUpdate()
{
    while (true)
    {
        if (mIsDestructed)
            break;

        for (auto& itr : mWalkers)
        {
            for (auto& coordinate : itr->mPath)
            {
                itr->mPath->
                int roomHeight = itr->mPath->Map[coordinate.y][coordinate.x];
                roomHeight = roomHeight - 48;

                WorldPacket buffer("# STATUS \r");
                buffer << (std::string)itr->mPlayer->GetName();
                buffer.AppendSpace();
                buffer << (uint8)itr->mPlayer->GetPlayerPositionX();
                buffer.AppendComma();
                buffer << (uint8)itr->mPlayer->GetPlayerPositionY();
                buffer.AppendComma();
                buffer << (uint8)roomHeight;
                buffer.AppendComma();
                buffer << (uint8)itr->mPlayer->CalculateRotation(itr->mPlayer->GetPlayerPositionX(), player->GetPlayerPositionY(), coordinate.x, coordinate.y);
                buffer.AppendComma();
                buffer << (uint8)itr->mPlayer->CalculateRotation(itr->mPlayer->GetPlayerPositionX(), player->GetPlayerPositionY(), coordinate.x, coordinate.y);
                buffer << "/mv ";
                buffer << (uint8)coordinate.x;
                buffer.AppendComma();
                buffer << (uint8)coordinate.y;
                buffer.AppendComma();
                buffer << (uint8)roomHeight;
                buffer << "/";
                buffer.AppendEndCarriage();
                itr->mPlayer->GetRoom()->SendPacketToAll(buffer.Write());
                itr->mPlayer->SetPlayerPosition(coordinate.x, coordinate.y, player->CalculateRotation(player->GetPlayerPositionX(), player->GetPlayerPositionY(), coordinate.x, coordinate.y));
                itr->mPlayer->SetCurrentRoomHeight(roomHeight);
            }
        }
        
    }
}
//-----------------------------------------------//
boost::mutex& ScheduleWalker::GetMutex()
{
    static boost::mutex mutex;
    return mutex;
}
//-----------------------------------------------//