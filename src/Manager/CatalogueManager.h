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

#ifndef _QuadEmu_CatalogueManager_h_
#define _QuadEmu_CatalogueManager_h_
#include "SharedDefines.h"
#include "Item.h"
#include "PublicItem.h"
#endif /* _QuadEmu_CatalogueManager_ */

typedef struct CatalogueData
{
    uint32 ItemId;
    std::string CallId;
    uint32 Credits;
}CatalogueDataStruct;

typedef std::map<std::string, CatalogueDataStruct*> CatalogueMap;
typedef std::map<uint32, Item*> ItemDefinitionsMap;

class CatalogueManager
{
public:
    friend class World;
    static CatalogueManager* instance();

    CatalogueManager();
    ~CatalogueManager();

public:
    Item* GetCatalogueItem(const std::string& catalogue);
    CatalogueDataStruct* GetCatalogueByCallId(const std::string& catalogue);
    Item* GetItemDefintion(const uint32& itemId);

private:
    CatalogueMap mCatalogue;
    ItemDefinitionsMap mItemDefinitions;
   
};
#define sCatalogueManager CatalogueManager::instance()
