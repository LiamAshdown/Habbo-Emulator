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

#include "RoomManager.h"
#include "Database/QueryDatabase.h"

namespace SteerStone
{
    /// Singleton
    RoomManager* RoomManager::instance()
    {
        static RoomManager instance;
        return &instance;
    }
    
    /// Constructor
    RoomManager::RoomManager()
    {
    }
    
    /// Deconstructor
    RoomManager::~RoomManager()
    {
    }
    
    /// Loadl_RoomCategories - Load l_Room categories from Database
    void RoomManager::LoadRoomCategories()
    {
        QueryDatabase l_Database("rooms");
        l_Database.PrepareQuery("SELECT id, parent_id, name, public_spaces, allow_trading, minrole_access, minrole_setflatcat FROM rooms_categories");
        l_Database.ExecuteQuery();

        if (!l_Database.GetResult())
        {
            LOG_ERROR << "room_categories is empty!";
            return;
        }

        Result* l_Result = l_Database.Fetch();

        do
        {
            RoomCategory& l_RoomCategories     = m_RoomCategories[l_Result->GetInt32(1)];
            l_RoomCategories.m_CategoryId      = l_Result->GetUint32(1);
            l_RoomCategories.m_ParentId        = l_Result->GetUint32(2);
            l_RoomCategories.m_Name            = l_Result->GetString(3);
            l_RoomCategories.m_RoomType        = l_Result->GetBool(4);
            l_RoomCategories.m_AllowTrading    = l_Result->GetUint16(5);
            l_RoomCategories.m_MinRoleAccess   = l_Result->GetBool(6);
            l_RoomCategories.m_MinRoleSetFlat  = l_Result->GetBool(7);

        } while (l_Result->GetNextResult());

        LOG_INFO << "Loaded " << m_RoomCategories.size() << " Room Categories";
    } 

    /// LoadRooms - Load rooms from database
    void RoomManager::LoadRoomModels()
    {
        QueryDatabase l_Database("rooms");
        l_Database.PrepareQuery("SELECT id, model_id, model_name, door_x, door_y, door_z, door_dir, height_map from room_models");
        l_Database.ExecuteQuery();

        if (!l_Database.GetResult())
        {
            LOG_ERROR << "height_map is empty!";
            return;
        }

        Result* l_Result = l_Database.Fetch();

        do
        {
            RoomModel& l_RoomModel          = m_RoomModels[l_Result->GetString(2)];
            l_RoomModel.m_Id                = l_Result->GetUint32(1);
            l_RoomModel.m_ModelId           = l_Result->GetString(2);
            l_RoomModel.m_Model             = l_Result->GetString(3);
            l_RoomModel.m_DoorX             = l_Result->GetInt32(4);
            l_RoomModel.m_DoorY             = l_Result->GetInt32(5);
            l_RoomModel.m_DoorZ             = l_Result->GetFloat(6);
            l_RoomModel.m_DoorOrientation   = l_Result->GetInt32(7);
            l_RoomModel.m_HeightMap         = l_Result->GetString(8);

            boost::replace_all(l_RoomModel.m_HeightMap, "|", "\r");

        } while (l_Result->GetNextResult());


        LOG_INFO << "Loaded " << m_RoomModels.size() << " Room Models";
    }

    /// LoadRooms - Load rooms from database
    void RoomManager::LoadRooms()
    {
        QueryDatabase l_Database("rooms");
        l_Database.PrepareQuery("SELECT id, owner_id, owner_name, category, name, description, model, ccts, wall_paper, floor, show_name, super_users, access_type, password, visitors_now, visitors_max, rating FROM rooms");
        l_Database.ExecuteQuery();

        if (!l_Database.GetResult())
        {
            LOG_ERROR << "hotel_rooms is empty!";
            return;
        }

        Result* l_Result = l_Database.Fetch();

        do
        {
            std::unique_ptr<Room> l_Room      = std::make_unique<Room>();
            l_Room->m_Id                      = l_Result->GetUint32(1);
            l_Room->m_OwnerId                 = l_Result->GetUint32(2);
            l_Room->m_OwnerName               = l_Result->GetString(3);
            l_Room->m_CategoryId              = l_Result->GetUint32(4);
            l_Room->m_Name                    = l_Result->GetString(5);
            l_Room->m_Description             = l_Result->GetString(6);
            l_Room->m_Model                   = l_Result->GetString(7);
            l_Room->m_Ccts                    = l_Result->GetString(8);
            l_Room->m_WallPaper               = l_Result->GetUint32(9);
            l_Room->m_Floor                   = l_Result->GetUint32(10);
            l_Room->m_ShowName                = l_Result->GetBool(11);
            l_Room->m_SuperUsers              = l_Result->GetBool(12);
            l_Room->m_AccessType              = l_Result->GetString(13);
            l_Room->m_Password                = l_Result->GetString(14);
            l_Room->m_VisitorsNow             = l_Result->GetUint32(15);
            l_Room->m_VisitorsMax             = l_Result->GetUint32(16);
            l_Room->m_RoomModel               = *GetRoomModel(l_Room->GetModel());
            l_Room->m_RoomCategory            = GetRoomCategory(l_Room->GetCategoryId());
            l_Room->GetRoomCategory()->m_VisitorsMax += l_Room->GetVisitorsMax();

            std::vector<std::string> l_Split;
            boost::split(l_Split, l_Room->GetRoomModel().GetHeightMap(), boost::is_any_of("\t "));

            l_Room->GetRoomModel().m_MapSizeY = l_Split.size();
            l_Room->GetRoomModel().m_MapSizeX = l_Split[0].length();
            l_Room->GetRoomModel().m_Grid.resize(boost::extents[l_Room->GetRoomModel().m_MapSizeY][l_Room->GetRoomModel().m_MapSizeX]);
      
            uint8 l_YCounter = 0;
            for (auto const& l_I : l_Split)
            {
                uint8 l_XCounter = 0;
                for (auto const& l_J : l_I)
                {
                    l_Room->GetRoomModel().m_Grid[l_YCounter][l_XCounter] = l_J;
                    l_XCounter++;
                }
                l_YCounter++;
            }
            m_Rooms[l_Room->GetId()] = std::move(l_Room);

        } while (l_Result->GetNextResult());

        LOG_INFO << "Loaded " << m_Rooms.size() << " Hotel Rooms";
    }

    /// UpdateRooms
    /// Update all rooms
    /// @p_Diff - Update tick
    void RoomManager::UpdateRooms(uint32 const& p_Diff)
    {
        std::lock_guard<std::mutex> l_Guard(m_Mutex);

        /// TODO; Don't update rooms which are not active
        for (auto const& l_Itr : m_Rooms)
        {
            l_Itr.second->Update(p_Diff);
        }
    }
    
    /// GetRoomCategory
    /// @p_Id : Category Id
    RoomCategory* RoomManager::GetRoomCategory(uint32 const& p_Id)
    {
        auto const& l_Itr = m_RoomCategories.find(p_Id);
        if (l_Itr != m_RoomCategories.end())
            return &l_Itr->second;
        else
            return nullptr;
    }
    
    /// GetRoomCategory
    /// @p_Id : Model Id
    RoomModel* RoomManager::GetRoomModel(std::string const& p_Model)
    {
        auto const& l_Itr = m_RoomModels.find(p_Model);
        if (l_Itr != m_RoomModels.end())
            return &l_Itr->second;
        else
            return nullptr;
    }

    /// GetRoomCategory
    /// @p_Id : Room Id
    std::shared_ptr<Room> RoomManager::GetRoom(uint32 const& p_Id)
    {
        auto const& l_Itr = m_Rooms.find(p_Id);
        if (l_Itr != m_Rooms.end())
            return l_Itr->second;
        else
            return nullptr;
    }
    
    /// GetRoomCategories - Get Room Category Map
    RoomCategoriesMap* RoomManager::GetRoomCategories()
    {
        return &m_RoomCategories;
    }
    
    /// GetRooms - Get Room Map
    RoomsMap* RoomManager::GetRooms()
    {
        return &m_Rooms;
    }
} ///< NAMESPACE STEERSTONE
