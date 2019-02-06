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
#include "CatalogueManager.h"
#include "World.h"
//-----------------------------------------------//
CatalogueManager* CatalogueManager::instance()
{
    static CatalogueManager instance;
    return &instance;
}
//-----------------------------------------------//
CatalogueManager::CatalogueManager()
{
}
//-----------------------------------------------//
CatalogueManager::~CatalogueManager()
{
    for (auto itr : mItemDefinitions)
        delete itr.second;

    mItemDefinitions.clear();
}
//-----------------------------------------------//
Item* CatalogueManager::GetCatalogueItem(const std::string& catalogue)
{
    CatalogueMap::const_iterator itr = mCatalogue.find(catalogue);

    if (itr != mCatalogue.end())
    {
        ItemDefinitionsMap::const_iterator itemItr = mItemDefinitions.find(itr->second->ItemId);

        if (itemItr != mItemDefinitions.end())
            return itemItr->second;
    }

    return nullptr;
}
//-----------------------------------------------//
CatalogueDataStruct* CatalogueManager::GetCatalogueByCallId(const std::string & catalogue)
{
    CatalogueMap::const_iterator itr = mCatalogue.find(catalogue);

    if (itr != mCatalogue.end())
        return itr->second;

    return nullptr;
}
//-----------------------------------------------//
Item* CatalogueManager::GetItemDefintion(const uint32& itemId)
{
    ItemDefinitionsMap::const_iterator itr = mItemDefinitions.find(itemId);

    if (itr != mItemDefinitions.end())
        return itr->second;
    
    return nullptr;
}
