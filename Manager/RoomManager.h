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


#ifndef _QuadEmu_RoomManager_h_
#define _QuadEmu_RoomManager_h_
#include "../Common/SharedDefines.h"
#endif /* _QuadEmu_RoomManager_ */

class Room;
class Player;

typedef std::map<uint32, Room*> RoomMap;

class RoomManager
{
public:
    static RoomManager* instance();

    RoomManager() {}
    ~RoomManager();

    void AddRoom(uint32 Id, Room* room);
    Room* GetRoom(uint32 Id, bool addPort = true);
    void RemoveRoom(const uint32& Id);

    void CreateRoom(Player* player, uint32 mId, std::string floor, std::string name, std::string model, std::string state, bool showName);
    uint32 GenerateRoomPort(uint32 mId);

    RoomMap GetRoomStorage();

protected:
    RoomMap mRoomStorage;
    static uint32 RealmPort;
};

#define sRoomManager RoomManager::instance()
