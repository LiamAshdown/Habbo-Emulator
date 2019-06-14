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

#include "CatalogueItem.h"
#include "ItemDefinition.h"

namespace SteerStone
{
    /// Constructor
    CatalogueItem::CatalogueItem()
    {
    }
    
    /// Deconstructor
    CatalogueItem::~CatalogueItem()
    {
    }

    /// GetType
    /// Return Item Type;
    std::string CatalogueItem::GetType() const
    {
        if (IsPackage())
            return "d";
        else if (m_ItemDefinition->HasBehaviour(ItemBehaviour::WallItem))
            return "s";
        else
            return "s";
    }

    /// GetIcon
    /// Return Item Icon
    std::string CatalogueItem::GetIcon() const
    {
        if (IsPackage())
            return "";

        return m_ItemDefinition->GetIcon(m_ItemSpecialSpriteId);
    }

    /// GetSize
    /// Return item size
    std::string CatalogueItem::GetSize() const
    {
        if (IsPackage() || m_ItemDefinition->HasBehaviour(ItemBehaviour::WallItem))
            return "";
        else
            return "0";
    }

    /// GetDimensions
    /// Return item dimensions
    std::string CatalogueItem::GetDimensions() const
    {
        if (IsPackage() || m_ItemDefinition->HasBehaviour(ItemBehaviour::WallItem))
            return "";
        else
            return m_ItemDefinition->GetLength() + "," + m_ItemDefinition->GetWidth();
    }

} ///< NAMESPACE STEERSTONE