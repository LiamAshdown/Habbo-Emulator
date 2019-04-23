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
    
    /// LoadRoomCategories
    /// Load Room categories from Database
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
            l_RoomCategories.m_AllowTrading    = l_Result->GetBool(5);
            l_RoomCategories.m_MinRoleAccess   = l_Result->GetUint16(6);
            l_RoomCategories.m_MinRoleSetFlat  = l_Result->GetUint16(7);

        } while (l_Result->GetNextResult());

        LOG_INFO << "Loaded " << m_RoomCategories.size() << " Room Categories";
    } 

    /// LoadRooms
    /// Load rooms from database
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

    /// LoadRoomUrls
    /// Load room urls from database
    void RoomManager::LoadRoomUrls()
    {
        QueryDatabase l_Database("rooms");
        l_Database.PrepareQuery("SELECT id, image_url, link_url FROM room_url WHERE is_active = 1");
        l_Database.ExecuteQuery();

        if (!l_Database.GetResult())
        {
            LOG_ERROR << "room_url is empty!";
            return;
        }

        Result* l_Result = l_Database.Fetch();

        do
        {
            RoomUrlData& l_RoomUrl = m_RoomUrl[l_Result->GetUint32(1)];
            l_RoomUrl.Id = l_Result->GetUint32(1);
            l_RoomUrl.ImageUrl = l_Result->GetString(2);
            l_RoomUrl.LinkUrl = l_Result->GetString(3);

        } while (l_Result->GetNextResult());

        LOG_INFO << "Loaded " << m_RoomUrl.size() << " Room Urls";
    }

    /// LoadRoomWalkWays
    /// Load Walk way positions (used to walk into another room)
    void RoomManager::LoadRoomWalkWays()
    {
        QueryDatabase l_Database("rooms");
        l_Database.PrepareQuery("SELECT from_id, to_id, from_model, to_model, from_position, to_position FROM room_walk_way WHERE is_active = 1");
        l_Database.ExecuteQuery();

        if (!l_Database.GetResult())
        {
            LOG_ERROR << "room_walk_way is empty!";
            return;
        }

        Result* l_Result = l_Database.Fetch();
        
        do
        {
            WalkWay l_RoomWalkWay;
            l_RoomWalkWay.m_FromId         = l_Result->GetUint32(1);
            l_RoomWalkWay.m_ToId           = l_Result->GetUint32(2);
            l_RoomWalkWay.m_WalkWayFromMod = l_Result->GetString(3);
            l_RoomWalkWay.m_WalkWayToMod   = l_Result->GetString(4);

            std::vector<std::string> l_SpaceSplit;
            boost::split(l_SpaceSplit, l_Result->GetString(5), boost::is_any_of(" "));

            for (auto& l_Itr : l_SpaceSplit)
            {
                std::vector<std::string> l_CommaSplit;
                boost::split(l_CommaSplit, l_Itr, boost::is_any_of(","));

                WalkWayPosition l_Position;
                l_Position.X = std::stoi(l_CommaSplit[0]);
                l_Position.Y = std::stoi(l_CommaSplit[1]);

                l_RoomWalkWay.m_WalkWayFromPos.push_back(l_Position);
            }

            std::vector<std::string> l_CommaSplit;
            boost::split(l_CommaSplit, l_Result->GetString(6), boost::is_any_of(","));

            l_RoomWalkWay.m_WalkWayToPos.X        = std::stoi(l_CommaSplit[0]);
            l_RoomWalkWay.m_WalkWayToPos.Y        = std::stoi(l_CommaSplit[1]);
            l_RoomWalkWay.m_WalkWayToPos.Z        = std::stoi(l_CommaSplit[2]);
            l_RoomWalkWay.m_WalkWayToPos.Rotation = std::stoi(l_CommaSplit[3]);

            m_RoomWalkWays.push_back(l_RoomWalkWay);

        } while (l_Result->GetNextResult());

        LOG_INFO << "Loaded " << m_RoomWalkWays.size() << " Room Walk Ways";
    }

    /// LoadRooms
    /// Load rooms from database
    void RoomManager::LoadRooms()
    {
        QueryDatabase l_Database("rooms");
        l_Database.PrepareQuery("SELECT id, owner_id, owner_name, category, name, description, model, ccts, wall_paper, floor, show_name, super_users, access_type, password, visitors_now, visitors_max, room_visible FROM rooms");
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

            std::vector<std::string> l_Split;
            boost::split(l_Split, l_Result->GetString(8), boost::is_any_of(","));
            for (auto &l_Itr : l_Split) 
            {
                l_Room->m_Ccts.push_back(l_Itr);
            }

            l_Room->m_WallPaper               = l_Result->GetUint32(9);
            l_Room->m_Floor                   = l_Result->GetUint32(10);
            l_Room->m_ShowName                = l_Result->GetBool(11);
            l_Room->m_SuperUsers              = l_Result->GetBool(12);
            l_Room->m_AccessType              = l_Result->GetUint16(13);
            l_Room->m_Password                = l_Result->GetString(14);
            l_Room->m_VisitorsNow             = l_Result->GetUint32(15);
            l_Room->m_VisitorsMax             = l_Result->GetUint32(16);
            l_Room->m_RoomVisible             = l_Result->GetBool(17);
            l_Room->m_RoomModel               = *GetRoomModel(l_Room->GetModel());
            l_Room->m_RoomCategory            = GetRoomCategory(l_Room->GetCategoryId());
            l_Room->GetRoomCategory()->m_VisitorsMax += l_Room->GetVisitorsMax();
            l_Room->LoadGridData();

            m_Rooms[l_Room->GetId()] = std::move(l_Room);

        } while (l_Result->GetNextResult());

        LOG_INFO << "Loaded " << m_Rooms.size() << " Hotel Rooms";
    }

    /// LoadRoomRights
    /// Load room rights from database
    void RoomManager::LoadRoomRights()
    {
        for (auto const& l_Itr : m_Rooms)
        {
            QueryDatabase l_Database("rooms");
            l_Database.PrepareQuery("SELECT id FROM room_rights WHERE room_id = ?");
            l_Database.GetStatement()->setUInt(1, l_Itr.second->GetId());
            l_Database.ExecuteQuery();

            if (!l_Database.GetResult())
                continue;

            Result* l_Result = l_Database.Fetch();

            do
            {
                l_Itr.second->m_SuperRights.insert(l_Result->GetUint32(1));
            } while (l_Result->GetNextResult());
        }
    }

    /// AddRoom
    /// Add room to storage from database
    /// @p_RoomId : Id of room we are querying database to get room info
    void RoomManager::AddRoom(uint32 const p_RoomId)
    {
        std::lock_guard<std::mutex> l_Guard(m_Mutex);

        QueryDatabase l_Database("rooms");
        l_Database.PrepareQuery("SELECT id, owner_id, owner_name, category, name, description, model, ccts, wall_paper, floor, show_name, super_users, access_type, password, visitors_now, visitors_max, room_visible FROM rooms WHERE id = ?");
        l_Database.GetStatement()->setUInt(1, p_RoomId);
        l_Database.ExecuteQuery();

        if (!l_Database.GetResult())
            return;

        Result* l_Result = l_Database.Fetch();

        std::unique_ptr<Room> l_Room        = std::make_unique<Room>();
        l_Room->m_Id                        = l_Result->GetUint32(1);
        l_Room->m_OwnerId                   = l_Result->GetUint32(2);
        l_Room->m_OwnerName                 = l_Result->GetString(3);
        l_Room->m_CategoryId                = l_Result->GetUint32(4);
        l_Room->m_Name                      = l_Result->GetString(5);
        l_Room->m_Description               = l_Result->GetString(6);
        l_Room->m_Model                     = l_Result->GetString(7);

        std::vector<std::string> l_Split;
        boost::split(l_Split, l_Result->GetString(8), boost::is_any_of(","));
        for (auto &l_Itr : l_Split)
        {
            l_Room->m_Ccts.push_back(l_Itr);
        }

        l_Room->m_WallPaper                 = l_Result->GetUint32(9);
        l_Room->m_Floor                     = l_Result->GetUint32(10);
        l_Room->m_ShowName                  = l_Result->GetBool(11);
        l_Room->m_SuperUsers                = l_Result->GetBool(12);
        l_Room->m_AccessType                = l_Result->GetUint16(13);
        l_Room->m_Password                  = l_Result->GetString(14);
        l_Room->m_VisitorsNow               = l_Result->GetUint32(15);
        l_Room->m_VisitorsMax               = l_Result->GetUint32(16);
        l_Room->m_RoomVisible               = l_Result->GetBool(17);
        l_Room->m_RoomModel                 = *GetRoomModel(l_Room->GetModel());
        l_Room->m_RoomCategory              = GetRoomCategory(l_Room->GetCategoryId());
        l_Room->GetRoomCategory()->m_VisitorsMax += l_Room->GetVisitorsMax();
        l_Room->LoadGridData();

        m_Rooms[l_Room->GetId()] = std::move(l_Room);
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
    RoomCategory* RoomManager::GetRoomCategory(uint32 const p_Id)
    {
        auto const& l_Itr = m_RoomCategories.find(p_Id);
        if (l_Itr != m_RoomCategories.end())
            return &l_Itr->second;
        else
            return nullptr;
    }
    
    /// GetRoomCategory
    /// @p_Id : Model Id
    RoomModel* RoomManager::GetRoomModel(std::string const p_Model)
    {
        auto const& l_Itr = m_RoomModels.find(p_Model);
        if (l_Itr != m_RoomModels.end())
            return &l_Itr->second;
        else
            return nullptr;
    }

    /// GetRoomUrl
    /// @p_Id : Room Id
    RoomUrlData * RoomManager::GetRoomUrl(uint32 const p_Id)
    {
        auto const& l_Itr = m_RoomUrl.find(p_Id);
        if (l_Itr != m_RoomUrl.end())
            return &l_Itr->second;
        else
            return nullptr;
    }

    /// GetRoomCategory
    /// @p_Id : Room Id
    std::shared_ptr<Room> RoomManager::GetRoom(uint32 const p_Id)
    {
        auto const& l_Itr = m_Rooms.find(p_Id);
        if (l_Itr != m_Rooms.end())
            return l_Itr->second;
        else
            return nullptr;
    }
    
    /// GetWalkWay
    /// @p_Id : Model Id
    /// @p_X : Tile Position X
    /// @p_Y : Tile Position Y
    WalkWay* RoomManager::GetWalkWay(uint32 const p_Id, int16 const p_X, int16 const p_Y)
    {
        for (auto& l_Itr : m_RoomWalkWays)
        {
            for (auto const& l_PosItr : l_Itr.m_WalkWayFromPos)
            {
                if (l_Itr.GetFromId() == p_Id && l_PosItr.X == p_X && l_PosItr.Y == p_Y)
                    return &l_Itr;
            }
        }
        return nullptr;
    }

    /// ReloadRoom
    /// Reload room from database
    /// @p_Id : Room Id
    void RoomManager::ReloadRoom(uint32 const p_Id)
    {
        std::lock_guard<std::mutex> l_Guard(m_Mutex);

        QueryDatabase l_Database("rooms");
        l_Database.PrepareQuery("SELECT category, name, description, wall_paper, show_name, super_users, access_type, password, visitors_max, room_visible FROM rooms WHERE id = ?");
        l_Database.GetStatement()->setUInt(1, p_Id);
        l_Database.ExecuteQuery();

        if (!l_Database.GetResult())
            return;

        Result* l_Result = l_Database.Fetch();

        std::shared_ptr<Room> l_Room        = GetRoom(p_Id);
        l_Room->m_CategoryId                = l_Result->GetUint32(1);
        l_Room->m_Name                      = l_Result->GetString(2);
        l_Room->m_Description               = l_Result->GetString(3);
        l_Room->m_WallPaper                 = l_Result->GetUint32(4);
        l_Room->m_ShowName                  = l_Result->GetBool(5);
        l_Room->m_SuperUsers                = l_Result->GetBool(6);
        l_Room->m_AccessType                = l_Result->GetUint16(7);
        l_Room->m_Password                  = l_Result->GetString(8);
        l_Room->m_VisitorsMax               = l_Result->GetUint32(9);
        l_Room->m_RoomVisible               = l_Result->GetBool(10);
    }

    /// GetRoomCategories
    /// Get Room Category Map
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
