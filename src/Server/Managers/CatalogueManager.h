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
#endif /* MANAGERS_CATALOGUE_MANAGER_h */

namespace SteerStone
{
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
        /// LoadPages
        /// Load Catalogue pages from database
        void LoadPages();
       
    };
} ///< NAMESPACE STEERSTONE

#define sCatalogueMgr SteerStone::PoolManager::instance()