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

#ifndef _ITEM_ITEM_MANAGER_h
#define _ITEM_ITEM_MANAGER_h
#include "Common/SharedDefines.h"
#include "Item.h"
#include <mutex>
#endif /* _ITEM_ITEM_MANAGER_h */

namespace SteerStone
{
    typedef std::vector<Item> PublicItemVec;
    typedef std::map<std::string, PublicItemVec> PublicItemMap;

    /// Class which holds item information
    /// Singleton
    class ItemManager
    {
    public:
        static ItemManager* instance();

    public:
        /// Constructor
        ItemManager();
        /// Constructor
        ~ItemManager();
        
    public:
        /// LoadPublicRoomItems
        /// Load Public items from database
        void LoadPublicRoomItems();

        /// GetPublicRoomItems
        /// @p_Model : Room Model to retrieve furniture data
        PublicItemVec GetPublicRoomItems(std::string const& p_Model);

        /// GetPublicItemByPosition
        /// @p_X : X axis of public item position
        /// @p_X : Y axis of public item position
        Item* GetPublicItemByPosition(std::string const p_Model, const int16 p_X, int16 const p_Y);

    private:
        PublicItemMap m_PublicItems;                     ///< Map which holds Room Model as key and info of the item
        std::mutex m_Mutex;                             ///< Mutex
    };
} ///< NAMESPACE STEERSTONE

#define sItemMgr SteerStone::ItemManager::instance()