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

#include "CataloguePackets.h"
#include "CataloguePage.h"
#include "ItemDefinition.h"

namespace SteerStone
{
    namespace HabboPacket
    {
        namespace Catalogue
        {
            //////////////////////////////////////////////////////////////////////////
            //////////////////////////////////////////////////////////////////////////

            StringBuffer const * CataloguePages::Write()
            {
                for (auto const& l_Itr : CataloguePagesStorage)
                {
                    m_Buffer.AppendString(l_Itr.NameIndex, false);
                    m_Buffer.AppendTab();
                    m_Buffer.AppendString(l_Itr.Name, false);
                    m_Buffer.AppendCarriage();
                }

                m_Buffer.AppendSOH();

                return &m_Buffer;
            }

            //////////////////////////////////////////////////////////////////////////
            //////////////////////////////////////////////////////////////////////////

            StringBuffer const * CatalogueItems::Write()
            {
                m_Buffer.AppendString("i:", false);
                m_Buffer.AppendString(CataloguePageInfo->GetNameIndex(), false);
                m_Buffer.AppendString("\r", false);

                m_Buffer.AppendString("n:", false);
                m_Buffer.AppendString(CataloguePageInfo->GetName(), false);
                m_Buffer.AppendString("\r", false);

                m_Buffer.AppendString("l:", false);
                m_Buffer.AppendString(CataloguePageInfo->GetLayout(), false);
                m_Buffer.AppendString("\r", false);

                m_Buffer.AppendString("g:", false);
                m_Buffer.AppendString(CataloguePageInfo->GetImageHeadLine(), false);
                m_Buffer.AppendString("\r", false);

                m_Buffer.AppendString("e:", false);
                m_Buffer.AppendString(CataloguePageInfo->GetImageTeasers(), false);
                m_Buffer.AppendString("\r", false);

                m_Buffer.AppendString("h:", false);
                m_Buffer.AppendString(CataloguePageInfo->GetBody(), false);
                m_Buffer.AppendString("\r", false);

                if (!CataloguePageInfo->GetLinkList().empty())
                {
                    m_Buffer.AppendString("u:", false);
                    m_Buffer.AppendString(CataloguePageInfo->GetLinkList(), false);
                    m_Buffer.AppendString("\r", false);
                }

                if (!CataloguePageInfo->GetLabelPick().empty())
                {
                    m_Buffer.AppendString("w:", false);
                    m_Buffer.AppendString(CataloguePageInfo->GetLabelPick(), false);
                    m_Buffer.AppendString("\r", false);
                }

                if (!CataloguePageInfo->GetLabelExtraS().empty())
                {
                    m_Buffer.AppendString("s:", false);
                    m_Buffer.AppendString(CataloguePageInfo->GetLabelExtraS(), false);
                    m_Buffer.AppendString("\r", false);
                }

                if (!CataloguePageInfo->GetLabelExrtraT().empty())
                {
                    m_Buffer.AppendString("t:", false);
                    m_Buffer.AppendString(CataloguePageInfo->GetLabelExrtraT(), false);
                    m_Buffer.AppendString("\r", false);
                }

                for (auto const& l_Itr : CataloguePageInfo->GetCatalogueItems())
                {
                    m_Buffer.AppendString("p:", false);
                    m_Buffer.AppendString(l_Itr->GetName(), false);
                    m_Buffer.AppendTab();

                    m_Buffer.AppendString(l_Itr->GetDescription(), false);
                    m_Buffer.AppendTab();

                    m_Buffer.AppendString(std::to_string(l_Itr->GetPrice()), false);
                    m_Buffer.AppendTab();

                    m_Buffer.AppendString("", false);
                    m_Buffer.AppendTab();

                    m_Buffer.AppendString(l_Itr->GetType(), false);
                    m_Buffer.AppendTab();

                    m_Buffer.AppendString(l_Itr->GetIcon(), false);
                    m_Buffer.AppendTab();

                    m_Buffer.AppendString(l_Itr->GetSize(), false);
                    m_Buffer.AppendTab();

                    m_Buffer.AppendString(l_Itr->GetDimensions(), false);
                    m_Buffer.AppendTab();

                    m_Buffer.AppendString(l_Itr->GetSaleCode(), false);
                    m_Buffer.AppendTab();

                    if (l_Itr->IsPackage() || l_Itr->GetItemDefinition()->GetSprite() == "poster")
                    {
                        m_Buffer.AppendString("", false);
                        m_Buffer.AppendTab();
                    }

                    if (l_Itr->IsPackage())
                    {
                        m_Buffer.AppendString(std::to_string(l_Itr->GetCataloguePackage()->size()), false);
                        m_Buffer.AppendTab();

                        for (auto const& l_SecondItr : *l_Itr->GetCataloguePackage())
                        {
                            m_Buffer.AppendString(l_SecondItr.ItemDefinitionInfo->GetIcon(l_SecondItr.SpecialSpriteId), false);
                            m_Buffer.AppendTab();

                            m_Buffer.AppendString(std::to_string(l_SecondItr.Amount), false);
                            m_Buffer.AppendTab();

                            m_Buffer.AppendString(l_SecondItr.ItemDefinitionInfo->GetColour(), false);
                            m_Buffer.AppendTab();
                        }
                    }
                    else if (!l_Itr->GetItemDefinition()->HasBehaviour(ItemBehaviour::WallItem))
                    {
                        m_Buffer.AppendString(l_Itr->GetItemDefinition()->GetColour(), false);
                        m_Buffer.AppendTab();
                    }

                    m_Buffer.AppendCarriage();
                }

                m_Buffer.AppendSOH();

                return &m_Buffer;
            }

            //////////////////////////////////////////////////////////////////////////
            //////////////////////////////////////////////////////////////////////////

            StringBuffer const* AliasToggle::Write()
            {
                m_Buffer.AppendWiredBool(false);

                m_Buffer.AppendSOH();

                return &m_Buffer;
            }

            //////////////////////////////////////////////////////////////////////////
            //////////////////////////////////////////////////////////////////////////

            StringBuffer const * SpriteList::Write()
            {
                m_Buffer.AppendWired(0);

                m_Buffer.AppendSOH();

                return &m_Buffer;
            }

            //////////////////////////////////////////////////////////////////////////
            //////////////////////////////////////////////////////////////////////////

        } ///< NAMESPACE CATALOGUE
    } ///< NAMESPACE HABBOPACKET
} ///< NAMESPACE STEERSTONE