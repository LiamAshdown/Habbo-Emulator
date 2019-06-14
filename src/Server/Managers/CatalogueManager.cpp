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

#include "CatalogueManager.h"
#include "ItemManager.h"
#include "Database/DatabaseTypes.h"

namespace SteerStone
{
    /// Singleton
    CatalogueManager * CatalogueManager::instance()
    {
        static CatalogueManager instance;
        return &instance;
    }

    /// Constructor
    CatalogueManager::CatalogueManager()
    {

    }

    /// Deconstructor
    CatalogueManager::~CatalogueManager()
    {

    }

    /// LoadCataloguePages
    /// Load Catalogue pages from database
    void CatalogueManager::LoadCataloguePages()
    {
        PreparedStatement* l_PreparedStatement = RoomDatabase.GetPrepareStatement();
        l_PreparedStatement->PrepareStatement("SELECT id, order_id, min_role, index_visible, is_club_only, name_index, link_list, name, layout, "
            "image_headline, image_teasers, body, label_pick, label_extra_s, label_extra_t FROM catalogue_pages ORDER BY order_id ASC");
        std::unique_ptr<PreparedResultSet> l_PreparedResultSet = l_PreparedStatement->ExecuteStatement();

        if (!l_PreparedResultSet)
        {
            LOG_INFO << "catalogue_pages is empty!";
            RoomDatabase.FreePrepareStatement(l_PreparedStatement);
            return;
        }

        do
        {
            ResultSet* l_Result                 = l_PreparedResultSet->FetchResult();

            CataloguePage l_CataloguePage;
            l_CataloguePage.m_Id                = l_Result[0].GetUInt32();
            l_CataloguePage.m_OrderId           = l_Result[1].GetUInt32();
            l_CataloguePage.m_MinRole           = l_Result[2].GetUInt16();
            l_CataloguePage.m_IndexVisible      = l_Result[3].GetBool();
            l_CataloguePage.m_ClubOnly          = l_Result[4].GetBool();
            l_CataloguePage.m_NameIndex         = l_Result[5].GetString();
            l_CataloguePage.m_LinkList          = l_Result[6].GetString();
            l_CataloguePage.m_Name              = l_Result[7].GetString();
            l_CataloguePage.m_Layout            = l_Result[8].GetString();
            l_CataloguePage.m_ImageHeadLine     = l_Result[9].GetString();
            l_CataloguePage.m_ImageTeasers      = l_Result[10].GetString();
            l_CataloguePage.m_Body              = l_Result[11].GetString();
            l_CataloguePage.m_LabelPick         = l_Result[12].GetString();
            l_CataloguePage.m_LabelExtraS       = l_Result[13].GetString();
            l_CataloguePage.m_LabelExtraT       = l_Result[14].GetString();

            m_CataloguePages.push_back(std::make_pair(l_Result[7].GetString(), l_CataloguePage));

        } while (l_PreparedResultSet->GetNextRow());

        RoomDatabase.FreePrepareStatement(l_PreparedStatement);

        LOG_INFO << "Loaded " << m_CataloguePages.size() << " Catalogue Pages!";
    }

    /// LoadCatalogueItems
    /// Load Catalogue items from database
    void CatalogueManager::LoadCatalogueItems()
    {
        PreparedStatement* l_PreparedStatement = RoomDatabase.GetPrepareStatement();
        l_PreparedStatement->PrepareStatement("SELECT id, sale_code, page_id, order_id, price, is_hidden, amount, definition_id, "
            "item_specialspriteid, name, description, is_package, package_name, package_description FROM catalogue_items ORDER BY order_id ASC");
        std::unique_ptr<PreparedResultSet> l_PreparedResultSet = l_PreparedStatement->ExecuteStatement();

        if (!l_PreparedResultSet)
        {
            LOG_INFO << "catalogue_items is empty!";
            RoomDatabase.FreePrepareStatement(l_PreparedStatement);
            return;
        }

        do
        {
            ResultSet* l_Result = l_PreparedResultSet->FetchResult();

            CatalogueItem& l_CatalogueItem                  = m_CatalogueItems[l_Result[0].GetUInt32()];
            l_CatalogueItem.m_Id                            = l_Result[0].GetUInt32();
            l_CatalogueItem.m_SaleCode                      = l_Result[1].GetString();
            l_CatalogueItem.m_PageId                        = l_Result[2].GetString();
            l_CatalogueItem.m_OrderId                       = l_Result[3].GetUInt32();
            l_CatalogueItem.m_Price                         = l_Result[4].GetUInt32();
            l_CatalogueItem.m_Hidden                        = l_Result[5].GetBool();
            l_CatalogueItem.m_Amount                        = l_Result[6].GetUInt32();
            l_CatalogueItem.m_DefinitionId                  = l_Result[7].GetUInt32();
            l_CatalogueItem.m_ItemSpecialSpriteId           = l_Result[8].GetUInt32();
            l_CatalogueItem.m_Name                          = l_Result[9].GetString();
            l_CatalogueItem.m_Description                   = l_Result[10].GetString();
            l_CatalogueItem.m_IsPackage                     = l_Result[11].GetBool();
            l_CatalogueItem.m_PackageName                   = l_Result[12].GetString();
            l_CatalogueItem.m_PackageDescription            = l_Result[13].GetString();
            l_CatalogueItem.m_ItemDefinition                = sItemMgr->GetItemDefinition(l_CatalogueItem.GetDefinitionId());
            
            CataloguePackageMap::iterator l_CataloguePackage = m_CataloguePackages.find(l_CatalogueItem.GetDefinitionId());
            if (l_CataloguePackage != m_CataloguePackages.end())
                l_CatalogueItem.m_CataloguePackages = &l_CataloguePackage->second;
            
        } while (l_PreparedResultSet->GetNextRow());

        RoomDatabase.FreePrepareStatement(l_PreparedStatement);

        LOG_INFO << "Loaded " << m_CatalogueItems.size() << " Catalogue Items!";

        /// Now sort the catalogue items into their respective pagess
        SortCatalogueItems();
    }

    /// LoadItemPackages
    /// Load Item packages from database
    void CatalogueManager::LoadCataloguePackages()
    {
        PreparedStatement* l_PreparedStatement = RoomDatabase.GetPrepareStatement();
        l_PreparedStatement->PrepareStatement("SELECT id, salecode, definition_id, special_sprite_id, amount FROM catalogue_packages");
        std::unique_ptr<PreparedResultSet> l_PreparedResultSet = l_PreparedStatement->ExecuteStatement();

        if (!l_PreparedResultSet)
        {
            LOG_INFO << "catalogue_packages is empty!";
            RoomDatabase.FreePrepareStatement(l_PreparedStatement);
            return;
        }

        do
        {
            ResultSet* l_Result = l_PreparedResultSet->FetchResult();

            CataloguePackage l_CataloguePackage;
            l_CataloguePackage.Id                   = l_Result[0].GetUInt32();
            l_CataloguePackage.SaleCode             = l_Result[1].GetString();
            l_CataloguePackage.DefinitionId         = l_Result[2].GetUInt32();
            l_CataloguePackage.SpecialSpriteId      = l_Result[3].GetUInt32();
            l_CataloguePackage.Amount               = l_Result[4].GetUInt32();
            l_CataloguePackage.ItemDefinitionInfo   = sItemMgr->GetItemDefinition(l_CataloguePackage.DefinitionId);

            CataloguePackageMap::iterator& l_CataloguePackageItr = m_CataloguePackages.find(l_Result[2].GetUInt32());
            if (l_CataloguePackageItr != m_CataloguePackages.end())
                l_CataloguePackageItr->second.push_back(l_CataloguePackage);
            else
                m_CataloguePackages[l_Result[2].GetUInt32()].push_back(l_CataloguePackage);

        } while (l_PreparedResultSet->GetNextRow());

        RoomDatabase.FreePrepareStatement(l_PreparedStatement);

        LOG_INFO << "Loaded " << m_CataloguePackages.size() << " Catalogue Packages!";
    }

    /// GetCataloguePages
    /// Returns Catalogue pages
    CataloguePageVector CatalogueManager::GetCataloguePages() const
    {
        return m_CataloguePages;
    }

    /// GetCataloguePageByName
    /// @p_Name : Page name we are searching for
    CataloguePage* CatalogueManager::GetCaloguePageByName(std::string const p_Name)
    {
        for (auto& l_Itr : m_CataloguePages)
        {
            if (l_Itr.second.GetName() == p_Name)
                return &l_Itr.second;
        }
        
        return nullptr;
    }

    /// SortCatalogueItems
    /// Sort the items into their respective catalogue pages
    void CatalogueManager::SortCatalogueItems()
    {
        if (m_CataloguePages.empty())
        {
            LOG_ERROR << "m_CataloguePages is empty! Catalogue will fail to load and possibly crash!";
            return;
        }

        for (auto& l_Itr : m_CataloguePages)
        {
            for (auto& l_SecondItr : m_CatalogueItems)
            {
                CatalogueItem* l_CatalogueItem = &l_SecondItr.second;

                std::vector<std::string> l_Split;
                boost::split(l_Split, l_SecondItr.second.GetPageId(), boost::is_any_of(","));

                for (auto const& l_ThirdItr : l_Split)
                {
                    if ((std::stoi(l_ThirdItr) == l_Itr.second.GetId()))
                        l_Itr.second.m_CataloguePageItems.push_back(l_CatalogueItem);
                }
            }
        }
    }
} ///< NAMESPACE STEERSTONE
