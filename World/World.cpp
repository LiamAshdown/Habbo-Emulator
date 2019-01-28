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
#include "../Network/DatabaseManager.h"
#include "../Server/Packet/WorldPacket.h"
#include "../Entity/Player/Player.h"
#include "../Manager/RoomManager.h"
#include "../Entity/Item/Item.h"
#include "../Server/WorldSession.h"
#include "../Room/Room.h"
#include "World.h"
//-----------------------------------------------//
World* World::instance()
{
    static World instance;
    return &instance;
}
//-----------------------------------------------//
World::World()
{
}
//-----------------------------------------------//
World::~World()
{
    for (auto itr : mSessions)
        delete itr.second;

    mSessions.clear();
}
//-----------------------------------------------//
WorldSession * World::FindSession(const uint32& mId) const
{
    SessionMap::const_iterator itr = mSessions.find(mId);

    if (itr != mSessions.end() && itr->second)
    {
        return itr->second;
    }
    
    return nullptr;
}
//-----------------------------------------------//
void World::AddSession(const uint32& mId, WorldSession* currentSession)
{
    if (!mSessions.count(mId))
    {
        mSessions[mId] = currentSession;
        QUAD_LOG_INFO("Add player session");
    }
    else
    {
        QUAD_LOG_ERROR("Player session already exists");
    }
}
//-----------------------------------------------//
void World::RemoveSession(const uint32& mId)
{
    SessionMap::const_iterator itr = mSessions.find(mId);
    if (itr->second && itr != mSessions.end())
    {
        delete itr->second;
        mSessions.erase(itr);
        QUAD_LOG_INFO("Deleted world session");
    }
}
//-----------------------------------------------//
void World::IncreasePlayerCount()
{
    ++mPlayerCount;
}
//-----------------------------------------------//
void World::DecreasePlayerCount()
{
    --mPlayerCount;
}
//-----------------------------------------------//
void World::LoadRooms()
{
    std::shared_ptr<MySQLConnection> connection = sDBManager->getConnectionPool()->borrow();
    try
    {
        std::shared_ptr<sql::Connection> sql_connection = connection->sql_connection;
        std::shared_ptr<sql::Statement> statement = std::shared_ptr<sql::Statement>(sql_connection->createStatement());
        std::shared_ptr<sql::ResultSet> result_set = std::shared_ptr<sql::ResultSet>(statement->executeQuery("SELECT * FROM rooms"));
        result_set->next();
        do
        {
            Room* room              = new Room;
            room->mId               = (ListenerPort + result_set->getInt(1));
            room->mRowId            = result_set->getInt(1);
            room->mName             = result_set->getString(2);
            room->mType             = result_set->getInt(3);
            room->mEnabled          = result_set->getBoolean(4);
            room->mHidden           = result_set->getBoolean(5);
            room->mOwnerId          = result_set->getInt(7);
            room->mDescription      = result_set->getString(8);
            room->mPassword         = result_set->getString(9);
            room->mState            = result_set->getString(10);
            room->mShowOwnerName    = result_set->getBoolean(11);
            room->mAllSuperUser     = result_set->getBoolean(12);
            room->mNowIn            = result_set->getInt(13);
            room->mMaxIn            = result_set->getInt(14);
            room->mFloorLevel       = result_set->getString(15);
            room->mModel            = result_set->getString(16);
            room->mOwnerName        = result_set->getString(19);
            sRoomManager->AddRoom(room->mId, room);
        } while (result_set->next());
    }
    catch (sql::SQLException &e)
    {
        sDBManager->printException(e, const_cast<char*>(__FILE__), const_cast<char*>(__FUNCTION__), __LINE__);
    }
}
//-----------------------------------------------//
void World::LoadPublicFurniture()
{
    std::shared_ptr<MySQLConnection> connection = sDBManager->getConnectionPool()->borrow();
    std::vector<Item*> itemVec;
    try
    {
        std::shared_ptr<sql::Connection> sql_connection = connection->sql_connection;
        std::shared_ptr<sql::Statement> statement = std::shared_ptr<sql::Statement>(sql_connection->createStatement());
        std::shared_ptr<sql::ResultSet> result_set = std::shared_ptr<sql::ResultSet>(statement->executeQuery("SELECT * FROM room_public_items"));
        result_set->next();
        do
        {
            Item* publicRoomItems               = new Item;
            publicRoomItems->mItemId            = result_set->getInt(1);
            publicRoomItems->mItemModelRoomName = result_set->getString(3);
            publicRoomItems->m_itemSpriteName   = result_set->getString(4);
            publicRoomItems->m_itemPos->x       = result_set->getInt(5);
            publicRoomItems->m_itemPos->y       = result_set->getInt(6);
            publicRoomItems->m_itemPos->z       = result_set->getInt(7);
            publicRoomItems->mItemRotation      = result_set->getInt(8);
            publicRoomItems->mitemData          = result_set->getString(9);
            itemVec.push_back(publicRoomItems);

        } while (result_set->next());
    }
    catch (sql::SQLException &e)
    {
        sDBManager->printException(e, const_cast<char*>(__FILE__), const_cast<char*>(__FUNCTION__), __LINE__);
    }

    std::vector<Item*> poolAVec;
    std::vector<Item*> poolBVec;
    std::vector<Item*> lobbyAVec;
    std::vector<Item*> lobbyTheaterVec;
    std::vector<Item*> habBurgerVec;
    std::vector<Item*> floorLobbyAVec;
    std::vector<Item*> floorLobbyBVec;
    std::vector<Item*> floorLobbyCVec;
    std::vector<Item*> barAVec;
    std::vector<Item*> pubAVec;
    std::vector<Item*> barBVec;
    std::vector<Item*> malhaBarAVec;
    std::vector<Item*> malhaBarBVec;
    std::vector<Item*> tavisCafeVec;
    std::vector<Item*> hallCVec;
    std::vector<Item*> entryHallVec;
    std::vector<Item*> HallAVec;
    std::vector<Item*> HallBVec;
    std::vector<Item*> HallDVec;
    for (auto& itr : itemVec)
    {
        if (itr->mItemModelRoomName == "pool_a")
            poolAVec.push_back(itr);
        else if (itr->mItemModelRoomName == "pool_b")
            poolBVec.push_back(itr);
        else if (itr->mItemModelRoomName == "lobby_a")
            lobbyAVec.push_back(itr);
        else if (itr->mItemModelRoomName == "theater")
            lobbyTheaterVec.push_back(itr);
        else if (itr->mItemModelRoomName == "habburger")
            habBurgerVec.push_back(itr);
        else if (itr->mItemModelRoomName == "floorlobby_a")
            floorLobbyAVec.push_back(itr);
        else if (itr->mItemModelRoomName == "floorlobby_b")
            floorLobbyBVec.push_back(itr);
        else if (itr->mItemModelRoomName == "floorlobby_c")
            floorLobbyCVec.push_back(itr);
        else if (itr->mItemModelRoomName == "bar_a")
            barAVec.push_back(itr);
        else if (itr->mItemModelRoomName == "pub_a")
            pubAVec.push_back(itr);
        else if (itr->mItemModelRoomName == "bar_b")
            barBVec.push_back(itr);
        else if (itr->mItemModelRoomName == "malja_bar_a")
            malhaBarAVec.push_back(itr);
        else if (itr->mItemModelRoomName == "malja_bar_b")
            malhaBarBVec.push_back(itr);
        else if (itr->mItemModelRoomName == "taivas_cafe")
            tavisCafeVec.push_back(itr);
        else if (itr->mItemModelRoomName == "hallC")
            hallCVec.push_back(itr);
        else if (itr->mItemModelRoomName == "entryhall")
            hallCVec.push_back(itr);
        else if (itr->mItemModelRoomName == "hallA")
            HallAVec.push_back(itr);
        else if (itr->mItemModelRoomName == "hallB")
            HallBVec.push_back(itr);
        else if (itr->mItemModelRoomName == "hallD")
            HallDVec.push_back(itr);
    }

    mPublicItem[37121] = lobbyAVec;
    mPublicItem[37122] = poolAVec;
    mPublicItem[37123] = pubAVec;
    mPublicItem[37125] = tavisCafeVec;
    mPublicItem[37126] = habBurgerVec;  
    mPublicItem[37127] = poolBVec;
    mPublicItem[37128] = barAVec;
    mPublicItem[37129] = lobbyTheaterVec;
    mPublicItem[37130] = floorLobbyAVec;
    mPublicItem[37131] = floorLobbyBVec;
    mPublicItem[37132] = floorLobbyCVec;
    mPublicItem[37133] = barBVec;
    mPublicItem[37134] = entryHallVec;
    mPublicItem[37135] = hallCVec;
    mPublicItem[37136] = HallBVec;
    mPublicItem[37137] = HallAVec;
    mPublicItem[37138] = HallDVec;
    mPublicItem[37139] = malhaBarAVec;
    mPublicItem[37140] = malhaBarBVec;
}
//-----------------------------------------------//
void World::LoadHeightMap()
{
    std::shared_ptr<MySQLConnection> connection = sDBManager->getConnectionPool()->borrow();
    std::vector<Item*> itemVec;
    try
    {
        std::shared_ptr<sql::Connection> sql_connection = connection->sql_connection;
        std::shared_ptr<sql::Statement> statement = std::shared_ptr<sql::Statement>(sql_connection->createStatement());
        std::shared_ptr<sql::ResultSet> result_set = std::shared_ptr<sql::ResultSet>(statement->executeQuery("SELECT * FROM room_models"));
        result_set->next();
        do
        {
            PublicHeightStruct publicHeightRoom;
            publicHeightRoom.model = result_set->getString(1);
            publicHeightRoom.heightmap = result_set->getString(6);
            boost::replace_all(publicHeightRoom.heightmap, " ", "\r");
            publicHeightRoom.x = result_set->getInt(2);
            publicHeightRoom.y = result_set->getInt(3);
            publicHeightRoom.z = result_set->getInt(4);
            publicHeightRoom.dir = result_set->getInt(5);
            mPublicHeight[publicHeightRoom.model] = publicHeightRoom;
        } while (result_set->next());
    }
    catch (sql::SQLException &e)
    {
        sDBManager->printException(e, const_cast<char*>(__FILE__), const_cast<char*>(__FUNCTION__), __LINE__);
    }
}