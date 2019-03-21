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

namespace Quad
{
    typedef std::vector<PublicItem> PublicItemVec;
    typedef std::map<std::string, PublicItemVec> PublicItemMap;

    class ItemManager
    {
    public:
        static ItemManager* instance();

    public:
        ItemManager();
        ~ItemManager();
        
    public:
        void LoadPublicRoomItems();

        PublicItemVec GetPublicRoomItems(const std::string& model);

    private:
        PublicItemMap mPublicItems;
        std::mutex mMutex;
    };
}

#define sItemMgr Quad::ItemManager::instance()

#endif /* _Quad_ItemManager_h_ */