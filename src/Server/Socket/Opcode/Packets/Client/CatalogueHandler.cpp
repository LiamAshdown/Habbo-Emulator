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

#include "Opcode/Packets/Server/CataloguePackets.h"

#include "HabboSocket.h"
#include "Habbo.h"
#include "CatalogueManager.h"


namespace SteerStone
{
    void HabboSocket::HandleCataloguePages(ClientPacket* p_Packet)
    {
        HabboPacket::Catalogue::CataloguePages l_Packet;

        for (auto const& l_Itr : sCatalogueMgr->GetCataloguePages())
        {
            CataloguePageHolder l_CataloguePageHolder;
            l_CataloguePageHolder.Name = l_Itr.second.GetName();
            l_CataloguePageHolder.NameIndex = l_Itr.second.GetNameIndex();

            l_Packet.CataloguePagesStorage.push_back(l_CataloguePageHolder);
        }

        SendPacket(l_Packet.Write());
    }

    void HabboSocket::HandleCatalogueItems(ClientPacket* p_Packet)
    {
        std::string l_Page = p_Packet->GetContent();

        std::vector<std::string> l_Split;
        boost::split(l_Split, l_Page, boost::is_any_of("/"));

        CataloguePage* l_CataloguePage = sCatalogueMgr->GetCaloguePageByName(l_Split[1]);

            if (!l_CataloguePage)
            return;

        HabboPacket::Catalogue::CatalogueItems l_Packet;
        l_Packet.CataloguePageInfo = l_CataloguePage;
        SendPacket(l_Packet.Write());
    }

    void HabboSocket::HandleGetAliasList(ClientPacket* p_Packet)
    {
        HabboPacket::Catalogue::SpriteList l_SpritePacket;
        SendPacket(l_SpritePacket.Write());

        HabboPacket::Catalogue::AliasToggle l_AliasPacket;
        SendPacket(l_AliasPacket.Write());
    }

} ///< NAMESPACE STEERSTONE
