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

#ifndef _Quad_RoomManager_h_
#define _Quad_RoomManager_h_
#include <mutex>
#include "Common/SharedDefines.h"
#include "Room.h"

typedef struct RoomModelsStruct
{
    std::string sModel;
    uint16 sDoorX;
    uint16 sDoorY;
    uint16 sDoorZ;
    uint16 sOrientation;
    std::string sHeightMap;
    bool sHasPool;
    bool sHeightMapDisabled;
}RoomModels;

namespace Quad
{
    typedef std::map<uint32, std::shared_ptr<Room>> RoomMap;
    typedef std::map<std::string, std::shared_ptr<RoomModels>> RoomModelsMap;

    class RoomManager
    {
    public:
        static RoomManager* instance();

    public:
        RoomManager();
        ~RoomManager();

        RoomMap GetRoomStorage() const;

    public:
        std::shared_ptr<RoomModels> GetRoomModel(std::string model);
        void LoadRoomHeights();

        void LoadRooms();
        std::shared_ptr<Room> GetRoom(uint16 Id);
        void AddRoom(std::shared_ptr<Room> room);
        void RemoveRoom(std::shared_ptr<Room> room);
    private:
        RoomMap mRooms;
        RoomModelsMap mRoomModels;
        std::mutex mMutex;
    };
}

#define sRoomMgr Quad::RoomManager::instance()

#endif /* _Quad_RoomManager_h_ */