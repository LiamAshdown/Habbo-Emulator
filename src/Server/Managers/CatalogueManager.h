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

#ifndef MANAGERS_CATALOGUE_MANAGER_h
#define MANAGERS_CATALOGUE_MANAGER_h
#include "Common/SharedDefines.h"
#include "CataloguePage.h"
#endif /* MANAGERS_CATALOGUE_MANAGER_h */

namespace SteerStone
{
    typedef std::vector<std::pair<std::string, CataloguePage>> CataloguePageVector;
    typedef std::unordered_map<uint32, CatalogueItem> CatalogueItemMap;
    typedef std::map<uint32, std::vector<CataloguePackage>> CataloguePackageMap;

    /// Holds information about catalogue categories
    class CatalogueManager
    {
    public:
        static CatalogueManager* instance();

    public:
        /// Constructor
        CatalogueManager();

        /// Deconstructor
        ~CatalogueManager();

    public:
        /// LoadCataloguePages
        /// Load Catalogue pages from database
        void LoadCataloguePages();

        /// LoadCatalogueItems
        /// Load Catalogue items from database
        void LoadCatalogueItems();

        /// LoadCataloguePackages
        /// Load Catalogue Packages from database
        void LoadCataloguePackages();

        /// GetCataloguePages
        /// Returns Catalogue pages
        CataloguePageVector GetCataloguePages() const;

        /// GetCataloguePageByName
        /// @p_Name : Page name we are searching for
        CataloguePage* GetCaloguePageByName(std::string const p_Name);

    private:
        /// SortCatalogueItems
        /// Sort the items into their respective catalogue pages
        void SortCatalogueItems();

    private:
        CataloguePageVector m_CataloguePages;
        CatalogueItemMap m_CatalogueItems;
        CataloguePackageMap m_CataloguePackages;
    };
} ///< NAMESPACE STEERSTONE

#define sCatalogueMgr SteerStone::CatalogueManager::instance()