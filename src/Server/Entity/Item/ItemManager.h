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

#ifndef _Quad_ItemManager_h_
#define _Quad_ItemManager_h_
#include <mutex>
#include "Common/SharedDefines.h"
#include "Item.h"

typedef struct PublicRoomItemStruct
{
    uint32 sId;
    uint32 sDefinitionId;
    std::string sSprite;
    std::string sModel;
    uint16 sX;
    uint16 sY;
    uint16 sZ;
    uint8 sRotation;
    std::string sObject;
    std::string sData;

}PublicRoomItem;

namespace Quad
{
    typedef std::map<uint32, std::unique_ptr<Item>> ItemDefinitionsMap;
    typedef std::map<uint32, std::vector<std::shared_ptr<PublicRoomItem>>> PublicRoomItemsMap;

    class ItemManager
    {
    public:
        static ItemManager* instance();

    public:
        ItemManager();
        ~ItemManager();

    public:
        void LoadItemDefinitions();
        void LoadPublicRoomItems();

        std::vector<std::shared_ptr<PublicRoomItem>> GetRoomPublicItems(const uint32& id);

    private:
        ItemDefinitionsMap mItemDefinitions;
        PublicRoomItemsMap mPublicRoomItems;

        std::mutex mMutex;
    };
}

#define sItemMgr Quad::ItemManager::instance()

#endif /* _Quad_ItemManager_h_ */