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
#include "RoomManager.h"
#include "../Room/Room.h"
#include "../World/World.h"
#include "../Entity/Player/Player.h"
uint32 RoomManager::RealmPort = 37120;
//-----------------------------------------------//
RoomManager * RoomManager::instance()
{
    static RoomManager instance;
    return &instance;
}
//-----------------------------------------------//
RoomManager::~RoomManager()
{
    for (const auto& itr : mRoomStorage)
        delete itr.second;

    mRoomStorage.clear();
}
//-----------------------------------------------//
void RoomManager::AddRoom(uint32 Id, Room* room)
{
    mRoomStorage[Id] = room;
}
//-----------------------------------------------//
Room * RoomManager::GetRoom(uint32 Id, bool addPort)
{
    if (addPort)
        Id += ListenerPort;
    RoomMap::iterator itr = mRoomStorage.find(Id);
    if (itr != mRoomStorage.end())
        return itr->second;

    return nullptr;
}
//-----------------------------------------------//
void RoomManager::RemoveRoom(const uint32 & Id)
{
    RoomMap::iterator itr = mRoomStorage.find(ListenerPort + Id);
    if (itr != mRoomStorage.end())
        mRoomStorage.erase(itr);
}
//-----------------------------------------------//
void RoomManager::CreateRoom(Player * player, uint32 mId, std::string floor, std::string name, std::string model, std::string state, bool showName)
{
    Room* newRoom = new Room;
    newRoom->mOwnerId = player->GetAccountId();
    newRoom->mOwnerName = player->GetName();
    newRoom->mRowId = mId;
    newRoom->mId = ListenerPort + mId;
    newRoom->mFloorLevel = floor;
    newRoom->mName = name;
    newRoom->mModel = model;
    newRoom->mState = state;
    newRoom->mShowOwnerName = showName;
    sWorld->mPublicRoomListener.push_back(new Listener(sWorld->mIoService, newRoom->mId));
    AddRoom(newRoom->mId, newRoom);
}
//-----------------------------------------------//
uint32 RoomManager::GenerateRoomPort(uint32 mId)
{
    return (RealmPort - mId);
}
//-----------------------------------------------//
RoomMap RoomManager::GetRoomStorage()
{
    return mRoomStorage;
}
//-----------------------------------------------//