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
#include "QuadEngine.h"
#include "../Network/DatabaseManager.h"
#include "../Manager/RoomManager.h"
#include "../Room/Room.h"
#include "../World/World.h"
#include "../Manager/ScheduleWalker.h"
#include "../Server/Packet/Opcodes.h"
#include <thread>

//-----------------------------------------------//
void QuadEngine::Boot()
{
    std::cout << "[QUADRAL]: Loading Server..." << std::endl;
    std::cout << std::endl;
    std::cout << "[QUADRAL]: Booting up MYSQL" << std::endl;


    if (sDBManager->testConnection())
        std::cout << "[QUADRAL]: MYSQL connection sucessful" << std::endl;
    sDBManager->getConnectionFactory()->create();

    std::cout << "[QUADRAL]: Loading Public Rooms" << std::endl;
    sWorld->LoadRooms();
    LoadPublicRoomsPort();
    sWorld->LoadPublicFurniture();
    sWorld->LoadHeightMap();
    sPacketHandler->InitializePackets();

    std::cout << "[QUADRAL]: Finished loading... listening on port 37120" << std::endl;
    Listener server(io_service, 37120);
    io_service.run();
}
//-----------------------------------------------//
void QuadEngine::LoadPublicRoomsPort()
{
    for (auto& itr : sRoomManager->GetRoomStorage())
    {
        std::cout << "[QUADRAL]: Added " << itr.second->GetName() << " on port " << itr.second->GetRoomId() << std::endl;
        publicRoomListener.push_back(std::shared_ptr<Listener>(new Listener(io_service, itr.second->GetRoomId())));
    }
}
//-----------------------------------------------//