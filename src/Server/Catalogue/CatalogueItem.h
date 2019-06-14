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

#ifndef _CATALOGUE_CATALOGUE_ITEMS_h
#define _CATALOGUE_CATALOGUE_ITEMS_h
#include "Common/SharedDefines.h"
#endif /* _CATALOGUE_CATALOGUE_ITEMS_h */

namespace SteerStone
{
    class ItemDefinition;

    typedef struct CataloguePackageStruct
    {
        uint32 Id;
        std::string SaleCode;
        uint32 DefinitionId;
        uint32 SpecialSpriteId;
        uint32 Amount;
        ItemDefinition* ItemDefinitionInfo;
    } CataloguePackage;

    class CatalogueItem
    {
    public:
        friend class CatalogueManager;

    public:
        /// Constructor
        CatalogueItem();

        /// Deconstructor
        ~CatalogueItem();

    public:
        /// GetType
        /// Return Item Type;
        std::string GetType() const;

        /// GetIcon
        /// Return Item Icon
        std::string GetIcon() const;

        /// GetSize
        /// Return item size
        std::string GetSize() const;

        /// GetDimensions
        /// Return item dimensions
        std::string GetDimensions() const;

        /// GetItemDefinition
        /// Returns item definition
        ItemDefinition* GetItemDefinition() { return m_ItemDefinition; }

        /// GetCataloguePackage
        /// Return catalogue packages
        std::vector<CataloguePackage>* GetCataloguePackage() { return m_CataloguePackages; }

    public:
        uint32 GetId()                      const { return m_Id;                        }
        std::string GetSaleCode()           const { return m_SaleCode;                  }
        std::string GetPageId()             const { return m_PageId;                    }
        uint32 GetOrderId()                 const { return m_OrderId;                   }
        uint32 GetPrice()                   const { return m_OrderId;                   }
        bool IsHidden()                     const { return m_Hidden;                    }
        uint32 GetAmount()                  const { return m_Amount;                    }
        uint32 GetDefinitionId()            const { return m_DefinitionId;              }
        uint32 GetItemSpecialSpriteId()     const { return m_ItemSpecialSpriteId;       }
        std::string GetName()               const { return m_Name;                      }
        std::string GetDescription()        const { return m_Description;               }
        bool IsPackage()                    const { return m_IsPackage;                 }
        std::string GetPackageName()        const { return m_PackageName;               }
        std::string GetPackageDescription() const { m_PackageDescription;               }

    private:
        uint32 m_Id;
        std::string m_SaleCode;
        std::string m_PageId;
        uint32 m_OrderId;
        uint32 m_Price;
        bool m_Hidden;
        uint32 m_Amount;
        uint32 m_DefinitionId;
        uint32 m_ItemSpecialSpriteId;
        std::string m_Name;
        std::string m_Description;
        bool m_IsPackage;
        std::string m_PackageName;
        std::string m_PackageDescription;
        std::vector<CataloguePackage>* m_CataloguePackages;
        ItemDefinition* m_ItemDefinition;
    };
} ///< NAMESPACE STEERSTONE