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

#ifndef _QuadEmu_World_h_
#define _QuadEmu_World_h_
#include "../Common/SharedDefines.h"
#include "../Server/Protocol/Listener.h"
#endif /* _QuadEmu_World_ */

class WorldSession;
class Socket;
class WorldPacket;
class Item;

typedef struct mPublicHeight
{
    std::string model;
    std::string heightmap;
    int8 x;
    int8 y;
    int8 z;
    int8 dir;

}PublicHeightStruct;

typedef std::unordered_map<uint32, WorldSession*> SessionMap;
typedef std::map<uint32, std::vector<Item*>> PublicItemMap;
typedef std::map<std::string, PublicHeightStruct> PublicHeightMap;

class World
{
public:
    friend class RoomManager;
    static World* instance();

    World();
    ~World();

    // Session
    WorldSession* FindSession(const uint32& mId) const;
    void AddSession(const uint32& mId, WorldSession* currentSession);
    void RemoveSession(const uint32& mId);

    void IncreasePlayerCount();
    void DecreasePlayerCount();

    // Load Data on start up
    void LoadRooms();
    void LoadPublicFurniture();
    void LoadHeightMap();
    void LoadPublicRoomsPort();

public:
    PublicItemMap mPublicItem;
    PublicHeightMap mPublicHeight;

private:
    SessionMap mSessions;
    uint32 mPlayerCount;
    std::vector<Listener*> mPublicRoomListener;
    boost::asio::io_service mIoService;
};
#define sWorld World::instance()
