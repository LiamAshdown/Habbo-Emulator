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
//-----------------------------------------------//
#include "ItemManager.h"
#include "Network/StringBuffer.h"
#include "Database/QueryDatabase.h"
//-----------------------------------------------//
namespace Quad
{
    //-----------------------------------------------//
    ItemManager* ItemManager::instance()
    {
        static ItemManager instance;
        return &instance;
    }
    //-----------------------------------------------//
    ItemManager::ItemManager()
    {
    }
    ItemManager::~ItemManager()
    {
        IF_LOG(plog::debug)
            LOG_DEBUG << "Destructor ItemManager called!";
    }
    //-----------------------------------------------//
    void ItemManager::LoadItemDefinitions()
    {
        QueryDatabase database("rooms");
        database.PrepareQuery("SELECT id, definition_id, sprite, model, x, y, z, rotation, object, data FROM room_public_items");
        database.ExecuteResultPrepareQuery();

        if (!database.GetExecuteQueryResult())
        {
            LOG_ERROR << "room_public_items database is empty!";
            return;
        }

        Field* fields = database.Fetch();

        std::vector<std::shared_ptr<PublicRoomItem>> publicRoomVect;

        do
        {
            std::unique_ptr<PublicRoomItem> item = std::make_unique<PublicRoomItem>();
            item->sId = fields->GetUint32(1);
            item->sDefinitionId = fields->GetUint32(2);
            item->sSprite = fields->GetString(3);
            item->sModel = fields->GetString(4);
            item->sX = fields->GetUint16(5);
            item->sY = fields->GetUint16(6);
            item->sZ = fields->GetUint16(7);
            item->sRotation = fields->GetUint8(8);
            item->sObject = fields->GetString(9);
            item->sData = fields->GetString(10);

            publicRoomVect.push_back(std::move(item));

        } while (fields->GetNextResult());
        
        std::vector<std::shared_ptr<PublicRoomItem>> poolAVec;
        std::vector<std::shared_ptr<PublicRoomItem>> poolBVec;
        std::vector<std::shared_ptr<PublicRoomItem>> lobbyAVec;
        std::vector<std::shared_ptr<PublicRoomItem>> lobbyTheaterVec;
        std::vector<std::shared_ptr<PublicRoomItem>> habBurgerVec;
        std::vector<std::shared_ptr<PublicRoomItem>> floorLobbyAVec;
        std::vector<std::shared_ptr<PublicRoomItem>> floorLobbyBVec;
        std::vector<std::shared_ptr<PublicRoomItem>> floorLobbyCVec;
        std::vector<std::shared_ptr<PublicRoomItem>> barAVec;
        std::vector<std::shared_ptr<PublicRoomItem>> pubAVec;
        std::vector<std::shared_ptr<PublicRoomItem>> barBVec;
        std::vector<std::shared_ptr<PublicRoomItem>> malhaBarAVec;
        std::vector<std::shared_ptr<PublicRoomItem>> malhaBarBVec;
        std::vector<std::shared_ptr<PublicRoomItem>> tavisCafeVec;
        std::vector<std::shared_ptr<PublicRoomItem>> hallCVec;
        std::vector<std::shared_ptr<PublicRoomItem>> entryHallVec;
        std::vector<std::shared_ptr<PublicRoomItem>> HallAVec;
        std::vector<std::shared_ptr<PublicRoomItem>> HallBVec;
        std::vector<std::shared_ptr<PublicRoomItem>> HallDVec;

        for (auto& itr : publicRoomVect)
        {
            if (itr->sModel == "pool_a")
                poolAVec.push_back(std::move(itr));
            else if (itr->sModel == "pool_b")
                poolBVec.push_back(std::move(itr));
            else if (itr->sModel == "lobby_a")
                lobbyAVec.push_back(std::move(itr));
            else if (itr->sModel == "theater")
                lobbyTheaterVec.push_back(std::move(itr));
            else if (itr->sModel == "habburger")
                habBurgerVec.push_back(std::move(itr));
            else if (itr->sModel == "floorlobby_a")
                floorLobbyAVec.push_back(std::move(itr));
            else if (itr->sModel == "floorlobby_b")
                floorLobbyBVec.push_back(std::move(itr));
            else if (itr->sModel == "floorlobby_c")
                floorLobbyCVec.push_back(std::move(itr));
            else if (itr->sModel == "bar_a")
                barAVec.push_back(std::move(itr));
            else if (itr->sModel == "pub_a")
                pubAVec.push_back(std::move(itr));
            else if (itr->sModel == "bar_b")
                barBVec.push_back(std::move(itr));
            else if (itr->sModel == "malja_bar_a")
                malhaBarAVec.push_back(std::move(itr));
            else if (itr->sModel == "malja_bar_b")
                malhaBarBVec.push_back(std::move(itr));
            else if (itr->sModel == "taivas_cafe")
                tavisCafeVec.push_back(std::move(itr));
            else if (itr->sModel == "hallC")
                hallCVec.push_back(std::move(itr));
            else if (itr->sModel == "entryhall")
                hallCVec.push_back(std::move(itr));
            else if (itr->sModel == "hallA")
                HallAVec.push_back(std::move(itr));
            else if (itr->sModel == "hallB")
                HallBVec.push_back(std::move(itr));
            else if (itr->sModel == "hallD")
                HallDVec.push_back(std::move(itr));
        }

        mPublicRoomItems[37121] = lobbyAVec;
        mPublicRoomItems[37122] = poolAVec;
        mPublicRoomItems[37123] = pubAVec;
        mPublicRoomItems[37125] = tavisCafeVec;
        mPublicRoomItems[37126] = habBurgerVec;
        mPublicRoomItems[37127] = poolBVec;
        mPublicRoomItems[37128] = barAVec;
        mPublicRoomItems[37129] = lobbyTheaterVec;
        mPublicRoomItems[37130] = floorLobbyAVec;
        mPublicRoomItems[37131] = floorLobbyBVec;
        mPublicRoomItems[37132] = floorLobbyCVec;
        mPublicRoomItems[37133] = barBVec;
        mPublicRoomItems[37134] = entryHallVec;
        mPublicRoomItems[37135] = hallCVec;
        mPublicRoomItems[37136] = HallBVec;
        mPublicRoomItems[37137] = HallAVec;
        mPublicRoomItems[37138] = HallDVec;
        mPublicRoomItems[37139] = malhaBarAVec;
        mPublicRoomItems[37140] = malhaBarBVec;
    }
    //-----------------------------------------------//
    void ItemManager::LoadPublicRoomItems()
    {
        QueryDatabase database("rooms");
        database.PrepareQuery("SELECT id, sprite, color, length, width, height, data_class, behaviour, name, description FROM item_definitions");
        database.ExecuteResultPrepareQuery();

        if (!database.GetExecuteQueryResult())
        {
            LOG_ERROR << "item_definitions database is empty!";
            return;
        }

        Field* fields = database.Fetch();

        do
        {
            std::unique_ptr<Item> item = std::make_unique<Item>();
            item->mId = fields->GetUint32(1);
            item->mSprite = fields->GetString(2);
            item->mColor = fields->GetString(3);
            item->mLength = fields->GetUint16(4);
            item->mWidth = fields->GetUint16(5);
            item->mHeight = fields->GetDouble(6);
            item->mDataClass = fields->GetString(7);
            item->mBehaviour = fields->GetString(8);
            item->mName = fields->GetString(9);
            item->mDescription = fields->GetString(10);

            mItemDefinitions[item->mId] = std::move(item);

        } while (fields->GetNextResult());
    }
    //-----------------------------------------------//
    std::vector<std::shared_ptr<PublicRoomItem>> ItemManager::GetRoomPublicItems(const uint32& id)
    {
        std::lock_guard<std::mutex> guard(mMutex);

        PublicRoomItemsMap::const_iterator itr = mPublicRoomItems.find(id);
        if (itr != mPublicRoomItems.end())
            return itr->second;
        else
        {
            LOG_ERROR << "Couldn't find public room storage for Id: " << id;
            return std::vector<std::shared_ptr<PublicRoomItem>>{};
        }
    }
    //-----------------------------------------------//
}
//-----------------------------------------------//