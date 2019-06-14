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

#ifndef _CATALOGUE_CATALOGUE_PAGE_h
#define _CATALOGUE_CATALOGUE_PAGE_h
#include "Common/SharedDefines.h"
#include "CatalogueItem.h"
#endif /* _CATALOGUE_CATALOGUE_PAGE_h */

namespace SteerStone
{
    class CataloguePage
    {
    public:
        friend class CatalogueManager;

    public:
        /// Constructor
        CataloguePage() {}

        /// Deconstructor
        ~CataloguePage() {}

    public:
        uint32 GetId()                  const { return m_Id;                    }
        uint32 GetOrderId()             const { return m_OrderId;               }
        uint16 GetMinRole()             const { return m_MinRole;               }
        bool IsIndexVisible()           const { return m_IndexVisible;          }
        bool IsClubOnly()               const { return m_ClubOnly;              }
        std::string GetNameIndex()      const { return m_NameIndex;             }
        std::string GetLinkList()       const { return m_LinkList;              }
        std::string GetName()           const { return m_Name;                  }
        std::string GetLayout()         const { return m_Layout;                }
        std::string GetImageHeadLine()  const { return m_ImageHeadLine;         }
        std::string GetImageTeasers()   const { return m_ImageTeasers;          }
        std::string GetBody()           const { return m_Body;                  }
        std::string GetLabelPick()      const { return m_LabelPick;             }
        std::string GetLabelExtraS()    const { return m_LabelExtraS;           }
        std::string GetLabelExrtraT()   const { return m_LabelExtraT;           }

        std::vector<CatalogueItem*>& GetCatalogueItems() { return m_CataloguePageItems; }

    private:
        uint32 m_Id;
        uint32 m_OrderId;
        uint16 m_MinRole;
        bool m_IndexVisible;
        bool m_ClubOnly;
        std::string m_NameIndex;
        std::string m_LinkList;
        std::string m_Name;
        std::string m_Layout;
        std::string m_ImageHeadLine;
        std::string m_ImageTeasers;
        std::string m_Body;
        std::string m_LabelPick;
        std::string m_LabelExtraS;
        std::string m_LabelExtraT;

        std::vector<CatalogueItem*> m_CataloguePageItems;
    };
} ///< NAMESPACE STEERSTONE