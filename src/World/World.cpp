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
#include "../Entity/Player/Player.h"
#include "CatalogueManager.h"
#include "RoomManager.h"
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

    for (auto itr : mPublicRoomListener)
        delete itr;

    for (auto itr : mPublicItem)
        for (auto itrItem : itr.second)
            delete itrItem;

    mSessions.clear();
    mPublicRoomListener.clear();
    mPublicItem.clear();
}
//-----------------------------------------------//
WorldSession * World::FindSession(const uint32& mId) const
{
    SessionMap::const_iterator itr = mSessions.find(mId);

    if (itr != mSessions.end() && itr->second)
        return itr->second;
    
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
        QUAD_LOG_ERROR("Player session already exists");
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
    std::vector<PublicItem*> itemVec;
    try
    {
        std::shared_ptr<sql::Connection> sql_connection = connection->sql_connection;
        std::shared_ptr<sql::Statement> statement = std::shared_ptr<sql::Statement>(sql_connection->createStatement());
        std::shared_ptr<sql::ResultSet> result_set = std::shared_ptr<sql::ResultSet>(statement->executeQuery("SELECT * FROM room_public_items"));
        result_set->next();
        do
        {
            PublicItem* publicRoomItems         = new PublicItem;
            publicRoomItems->mId            = result_set->getInt(1);
            publicRoomItems->mModelName = result_set->getString(3);
            publicRoomItems->mName    = result_set->getString(4);
            publicRoomItems->mPosition->x        = result_set->getInt(5);
            publicRoomItems->mPosition->y        = result_set->getInt(6);
            publicRoomItems->mPosition->z        = result_set->getInt(7);
            publicRoomItems->mRotation      = result_set->getInt(8);
            publicRoomItems->mData          = result_set->getString(9);
            itemVec.push_back(publicRoomItems);

        } while (result_set->next());
    }
    catch (sql::SQLException &e)
    {
        sDBManager->printException(e, const_cast<char*>(__FILE__), const_cast<char*>(__FUNCTION__), __LINE__);
    }

    std::vector<PublicItem*> poolAVec;
    std::vector<PublicItem*> poolBVec;
    std::vector<PublicItem*> lobbyAVec;
    std::vector<PublicItem*> lobbyTheaterVec;
    std::vector<PublicItem*> habBurgerVec;
    std::vector<PublicItem*> floorLobbyAVec;
    std::vector<PublicItem*> floorLobbyBVec;
    std::vector<PublicItem*> floorLobbyCVec;
    std::vector<PublicItem*> barAVec;
    std::vector<PublicItem*> pubAVec;
    std::vector<PublicItem*> barBVec;
    std::vector<PublicItem*> malhaBarAVec;
    std::vector<PublicItem*> malhaBarBVec;
    std::vector<PublicItem*> tavisCafeVec;
    std::vector<PublicItem*> hallCVec;
    std::vector<PublicItem*> entryHallVec;
    std::vector<PublicItem*> HallAVec;
    std::vector<PublicItem*> HallBVec;
    std::vector<PublicItem*> HallDVec;
    for (auto& itr : itemVec)
    {
        if (itr->mModelName == "pool_a")
            poolAVec.push_back(itr);
        else if (itr->mModelName == "pool_b")
            poolBVec.push_back(itr);
        else if (itr->mModelName == "lobby_a")
            lobbyAVec.push_back(itr);
        else if (itr->mModelName == "theater")
            lobbyTheaterVec.push_back(itr);
        else if (itr->mModelName == "habburger")
            habBurgerVec.push_back(itr);
        else if (itr->mModelName == "floorlobby_a")
            floorLobbyAVec.push_back(itr);
        else if (itr->mModelName == "floorlobby_b")
            floorLobbyBVec.push_back(itr);
        else if (itr->mModelName == "floorlobby_c")
            floorLobbyCVec.push_back(itr);
        else if (itr->mModelName == "bar_a")
            barAVec.push_back(itr);
        else if (itr->mModelName == "pub_a")
            pubAVec.push_back(itr);
        else if (itr->mModelName == "bar_b")
            barBVec.push_back(itr);
        else if (itr->mModelName == "malja_bar_a")
            malhaBarAVec.push_back(itr);
        else if (itr->mModelName == "malja_bar_b")
            malhaBarBVec.push_back(itr);
        else if (itr->mModelName == "taivas_cafe")
            tavisCafeVec.push_back(itr);
        else if (itr->mModelName == "hallC")
            hallCVec.push_back(itr);
        else if (itr->mModelName == "entryhall")
            hallCVec.push_back(itr);
        else if (itr->mModelName == "hallA")
            HallAVec.push_back(itr);
        else if (itr->mModelName == "hallB")
            HallBVec.push_back(itr);
        else if (itr->mModelName == "hallD")
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
void World::LoadCatalogue()
{
    std::shared_ptr<MySQLConnection> connection = sDBManager->getConnectionPool()->borrow();
    std::vector<Item*> itemVec;
    try
    {
        std::shared_ptr<sql::Connection> sql_connection = connection->sql_connection;
        std::shared_ptr<sql::Statement> statement = std::shared_ptr<sql::Statement>(sql_connection->createStatement());
        std::shared_ptr<sql::ResultSet> result_set = std::shared_ptr<sql::ResultSet>(statement->executeQuery("SELECT * FROM catalogue"));
        result_set->next();
        do
        {
            CatalogueDataStruct* catalogueData = new CatalogueDataStruct;
            catalogueData->ItemId    = result_set->getInt(2);
            catalogueData->CallId    = result_set->getString(3);
            catalogueData->Credits   = result_set->getInt(4);

            sCatalogueManager->mCatalogue[catalogueData->CallId] = catalogueData;
        } while (result_set->next());
    }
    catch (sql::SQLException &e)
    {
        sDBManager->printException(e, const_cast<char*>(__FILE__), const_cast<char*>(__FUNCTION__), __LINE__);
    }
}
//-----------------------------------------------//
void World::LoadItemDefinitions()
{
    std::shared_ptr<MySQLConnection> connection = sDBManager->getConnectionPool()->borrow();
    try
    {
        std::shared_ptr<sql::Connection> sql_connection = connection->sql_connection;
        std::shared_ptr<sql::Statement> statement = std::shared_ptr<sql::Statement>(sql_connection->createStatement());
        std::shared_ptr<sql::ResultSet> result_set = std::shared_ptr<sql::ResultSet>(statement->executeQuery("SELECT * FROM item_definitions"));
        result_set->next();
        do
        {
            Item* itemDefinitions                = new Item;
            itemDefinitions->mId                 = result_set->getInt(1);
            itemDefinitions->mSprite             = result_set->getString(2);
            itemDefinitions->mColour             = result_set->getString(3);
            itemDefinitions->mPosition->length   = result_set->getInt(4);
            itemDefinitions->mPosition->width    = result_set->getInt(5);
            itemDefinitions->mPosition->height   = result_set->getDouble(6);
            itemDefinitions->mDataClass          = result_set->getInt(7);
            itemDefinitions->mBehaviour          = result_set->getString(8);
            itemDefinitions->mName               = result_set->getInt(9);
            itemDefinitions->mDescription        = result_set->getString(10);
            sCatalogueManager->mItemDefinitions[itemDefinitions->mId] = itemDefinitions;

        } while (result_set->next());
    }
    catch (sql::SQLException &e)
    {
        sDBManager->printException(e, const_cast<char*>(__FILE__), const_cast<char*>(__FUNCTION__), __LINE__);
    }
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
//-----------------------------------------------//
void World::LoadPublicRoomsPort()
{
    for (auto& itr : sRoomManager->GetRoomStorage())
    {
        std::cout << "[QUADRAL]: Added " << itr.second->GetName() << " on port " << itr.second->GetRoomId() << std::endl;
        mPublicRoomListener.push_back(new Listener(mIoService, itr.second->GetRoomId()));
    }

    std::cout << "[QUADRAL]: Finished loading... listening on port 37120" << std::endl;

    Listener server(mIoService, 37120);
    mIoService.run();

}
//-----------------------------------------------//