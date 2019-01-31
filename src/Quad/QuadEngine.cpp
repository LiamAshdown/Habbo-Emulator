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
#include <boost/thread.hpp>
#include "QuadEngine.h"
#include "../Network/DatabaseManager.h"
#include "../Manager/RoomManager.h"
#include "../Room/Room.h"
#include "../World/World.h"
#include "../Manager/ScheduleWalker.h"
#include "../Server/Packet/Opcodes.h"
#include <thread>
//-----------------------------------------------//
#define WORLD_SLEEP_FOR 500
//-----------------------------------------------//
QuadEngine::QuadEngine()
{
}
//-----------------------------------------------//
QuadEngine::~QuadEngine()
{
}
//-----------------------------------------------//
void QuadEngine::Boot()
{
    std::cout << "[QUADRAL]: Loading Server..." << std::endl;
    std::cout << std::endl;
    std::cout << "[QUADRAL]: Booting up MYSQL" << std::endl;

    if (sDBManager->testConnection())
        std::cout << "[QUADRAL]: MYSQL connection sucessful" << std::endl;
    sDBManager->getConnectionFactory()->create();

    std::cout << "[QUADRAL]: Booting up World Update" << std::endl;
    boost::thread* thread = new boost::thread(&QuadEngine::UpdateWorld, this);
    thread->detach();
    std::cout << "[QUADRAL]: Loading Furniture" << std::endl;
    sWorld->LoadPublicFurniture();
    std::cout << "[QUADRAL]: Loading Room Height" << std::endl;
    sWorld->LoadHeightMap();
    std::cout << "[QUADRAL]: Loading Packet Handlers" << std::endl;
    sPacketHandler->InitializePackets();

    std::cout << "[QUADRAL]: Loading Public Rooms" << std::endl;
    sWorld->LoadRooms();
    sWorld->LoadPublicRoomsPort();

}
//-----------------------------------------------//
void QuadEngine::UpdateWorld()
{
    Timer timer;

    while (true)
    {
        sScheduleWalker->WalkUpdate();

        // Update the world every 50 ms
        if (timer.Elasped() < WORLD_SLEEP_FOR)
        {
            uint32 diff = WORLD_SLEEP_FOR - timer.Elasped();
            std::this_thread::sleep_for(std::chrono::milliseconds(diff));
            timer.Reset();
        }
    }
}
//-----------------------------------------------//