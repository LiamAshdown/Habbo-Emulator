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

#pragma once
#include "Common/SharedDefines.h"
#include "HabboSocket.h"

namespace SteerStone
{
    typedef struct CataloguePageStruct
    {
        std::string NameIndex;
        std::string Name;
    }CataloguePageHolder;

    class CataloguePage;

    namespace HabboPacket
    {
        namespace Catalogue
        {
            /// SERVER_CATALOGUE_PAGES packet builder
            class CataloguePages final : public ServerPacket
            {
            public:
                /// Constructor 
                CataloguePages() : ServerPacket(SERVER_CATALOGUE_PAGES) {}

            public:
                /// Write the packet
                StringBuffer const* Write();

                std::vector<CataloguePageHolder> CataloguePagesStorage;
            };

            /// SERVER_CATALOGUE_ITEMS packet builder
            class CatalogueItems final : public ServerPacket
            {
            public:
                /// Constructor 
                CatalogueItems() : ServerPacket(SERVER_CATALOGUE_ITEMS) {}

            public:
                /// Write the packet
                StringBuffer const* Write();

                CataloguePage* CataloguePageInfo;
            };

            /// SERVER_ALIAS_TOGGLE packet builder
            class AliasToggle final : public ServerPacket
            {
            public:
                /// Constructor 
                AliasToggle() : ServerPacket(SERVER_ALIAS_TOGGLE) {}

            public:
                /// Write the packet
                StringBuffer const* Write();
            };

            /// SERVER_SPRITE_LIST packet builder
            class SpriteList final : public ServerPacket
            {
            public:
                /// Constructor 
                SpriteList() : ServerPacket(SERVER_SPRITE_LIST) {}

            public:
                /// Write the packet
                StringBuffer const* Write();
            };
        } ///< NAMESPACE CATALOGUE
    } ///< NAMESPACE HABBOPACKET
} ///< NAMESPACE STEERSTONE