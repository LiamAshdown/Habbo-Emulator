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
#include "Database/DatabaseTypes.h"

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
        PreparedStatement* l_PreparedStatement = RoomDatabase.GetPrepareStatement();
        l_PreparedStatement->PrepareStatement("SELECT id, parent_id, name, public_spaces, allow_trading, minrole_access, minrole_setflatcat FROM rooms_categories");
        std::unique_ptr<PreparedResultSet> l_PreparedResultSet = l_PreparedStatement->ExecuteStatement();

        if (!l_PreparedStatement)
        {
            LOG_ERROR << "room_categories is empty!";
            RoomDatabase.FreePrepareStatement(l_PreparedStatement);
            return;
        }

        do
        {
            ResultSet* l_Result                = l_PreparedResultSet->FetchResult();

            RoomCategory& l_RoomCategories     = m_RoomCategories[l_Result[0].GetUInt32()];
            l_RoomCategories.m_CategoryId      = l_Result[0].GetUInt32();
            l_RoomCategories.m_ParentId        = l_Result[1].GetUInt32();
            l_RoomCategories.m_Name            = l_Result[2].GetString();
            l_RoomCategories.m_RoomType        = l_Result[3].GetBool();
            l_RoomCategories.m_AllowTrading    = l_Result[4].GetBool();
            l_RoomCategories.m_MinRoleAccess   = l_Result[5].GetUInt16();
            l_RoomCategories.m_MinRoleSetFlat  = l_Result[6].GetUInt16();

        } while (l_PreparedResultSet->GetNextRow());

        RoomDatabase.FreePrepareStatement(l_PreparedStatement);

        LOG_INFO << "Loaded " << m_RoomCategories.size() << " Room Categories";
    } 

    /// LoadRooms
    /// Load rooms from database
    void RoomManager::LoadRoomModels()
    {
        PreparedStatement* l_PreparedStatement = RoomDatabase.GetPrepareStatement();
        l_PreparedStatement->PrepareStatement("SELECT id, model_id, model_name, door_x, door_y, door_z, door_dir, height_map from room_models");
        std::unique_ptr<PreparedResultSet> l_PreparedResultSet = l_PreparedStatement->ExecuteStatement();

        if (!l_PreparedStatement)
        {
            LOG_ERROR << "height_map is empty!";
            RoomDatabase.FreePrepareStatement(l_PreparedStatement);
            return;
        }

        do
        {
            ResultSet* l_Result             = l_PreparedResultSet->FetchResult();

            RoomModel& l_RoomModel          = m_RoomModels[l_Result[1].GetString()];
            l_RoomModel.m_Id                = l_Result[0].GetUInt32();
            l_RoomModel.m_ModelId           = l_Result[1].GetString();
            l_RoomModel.m_Model             = l_Result[2].GetString();
            l_RoomModel.m_DoorX             = l_Result[3].GetInt32();
            l_RoomModel.m_DoorY             = l_Result[4].GetInt32();
            l_RoomModel.m_DoorZ             = l_Result[5].GetDouble();
            l_RoomModel.m_DoorOrientation   = l_Result[6].GetUInt32();
            l_RoomModel.m_HeightMap         = l_Result[7].GetString();
            boost::replace_all(l_RoomModel.m_HeightMap, "|", "\r");

        } while (l_PreparedResultSet->GetNextRow());

        RoomDatabase.FreePrepareStatement(l_PreparedStatement);

        LOG_INFO << "Loaded " << m_RoomModels.size() << " Room Models";
    }

    /// LoadRoomUrls
    /// Load room urls from database
    void RoomManager::LoadRoomUrls()
    {
        PreparedStatement* l_PreparedStatement = RoomDatabase.GetPrepareStatement();
        l_PreparedStatement->PrepareStatement("SELECT id, image_url, link_url FROM room_url WHERE is_active = 1");
        std::unique_ptr<PreparedResultSet> l_PreparedResultSet = l_PreparedStatement->ExecuteStatement();

        if (!l_PreparedStatement)
        {
            LOG_ERROR << "room_url is empty!";
            RoomDatabase.FreePrepareStatement(l_PreparedStatement);
            return;
        }

        do
        {
            ResultSet* l_Result    = l_PreparedResultSet->FetchResult();

            RoomUrlData& l_RoomUrl = m_RoomUrl[l_Result[0].GetUInt32()];
            l_RoomUrl.Id           = l_Result[0].GetUInt32();
            l_RoomUrl.ImageUrl     = l_Result[1].GetString();
            l_RoomUrl.LinkUrl      = l_Result[2].GetString();

        } while (l_PreparedResultSet->GetNextRow());

        RoomDatabase.FreePrepareStatement(l_PreparedStatement);

        LOG_INFO << "Loaded " << m_RoomUrl.size() << " Room Urls";
    }

    /// LoadRoomWalkWays
    /// Load Walk way positions (used to walk into another room)
    void RoomManager::LoadRoomWalkWays()
    {
        PreparedStatement* l_PreparedStatement = RoomDatabase.GetPrepareStatement();
        l_PreparedStatement->PrepareStatement("SELECT from_id, to_id, from_model, to_model, from_position, to_position FROM room_walk_way WHERE is_active = 1");
        std::unique_ptr<PreparedResultSet> l_PreparedResultSet = l_PreparedStatement->ExecuteStatement();

        if (!l_PreparedStatement)
        {
            LOG_ERROR << "room_walk_way is empty!";
            RoomDatabase.FreePrepareStatement(l_PreparedStatement);
            return;
        }
        
        do
        {
            ResultSet* l_Result = l_PreparedResultSet->FetchResult();

            WalkWay l_RoomWalkWay;
            l_RoomWalkWay.m_FromId         = l_Result[0].GetUInt32();
            l_RoomWalkWay.m_ToId           = l_Result[1].GetUInt32();
            l_RoomWalkWay.m_WalkWayFromMod = l_Result[2].GetString();
            l_RoomWalkWay.m_WalkWayToMod   = l_Result[3].GetString();

            std::vector<std::string> l_SpaceSplit;
            boost::split(l_SpaceSplit, l_Result[4].GetString(), boost::is_any_of(" "));

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
            boost::split(l_CommaSplit, l_Result[5].GetString(), boost::is_any_of(","));

            l_RoomWalkWay.m_WalkWayToPos.X        = std::stoi(l_CommaSplit[0]);
            l_RoomWalkWay.m_WalkWayToPos.Y        = std::stoi(l_CommaSplit[1]);
            l_RoomWalkWay.m_WalkWayToPos.Z        = std::stoi(l_CommaSplit[2]);
            l_RoomWalkWay.m_WalkWayToPos.Rotation = std::stoi(l_CommaSplit[3]);

            m_RoomWalkWays.push_back(l_RoomWalkWay);

        } while (l_PreparedResultSet->GetNextRow());

        RoomDatabase.FreePrepareStatement(l_PreparedStatement);

        LOG_INFO << "Loaded " << m_RoomWalkWays.size() << " Room Walk Ways";
    }

    /// LoadRooms
    /// Load rooms from database
    void RoomManager::LoadRooms()
    {
        PreparedStatement* l_PreparedStatement = RoomDatabase.GetPrepareStatement();
        l_PreparedStatement->PrepareStatement("SELECT id, owner_id, owner_name, category, name, description, model, ccts, wall_paper, " 
            "floor, show_name, super_users, access_type, password, visitors_max, room_visible, rating FROM rooms");
        std::unique_ptr<PreparedResultSet> l_PreparedResultSet = l_PreparedStatement->ExecuteStatement();

        if (!l_PreparedStatement)
        {
            LOG_ERROR << "hotel_rooms is empty!";
            RoomDatabase.FreePrepareStatement(l_PreparedStatement);
            return;
        }

        do
        {
            ResultSet* l_Result               = l_PreparedResultSet->FetchResult();

            std::unique_ptr<Room> l_Room      = std::make_unique<Room>();
            l_Room->m_Id                      = l_Result[0].GetUInt32();
            l_Room->m_OwnerId                 = l_Result[1].GetUInt32();
            l_Room->m_OwnerName               = l_Result[2].GetString();
            l_Room->m_CategoryId              = l_Result[3].GetUInt32();
            l_Room->m_Name                    = l_Result[4].GetString();
            l_Room->m_Description             = l_Result[5].GetString();
            l_Room->m_Model                   = l_Result[6].GetString();

            std::vector<std::string> l_Split;
            boost::split(l_Split, l_Result[7].GetString(), boost::is_any_of(","));
            for (auto &l_Itr : l_Split) 
            {
                l_Room->m_Ccts.push_back(l_Itr);
            }

            l_Room->m_WallPaper               = l_Result[8].GetUInt32();
            l_Room->m_Floor                   = l_Result[9].GetUInt32();
            l_Room->m_ShowName                = l_Result[10].GetBool();
            l_Room->m_AllowSuperRights        = l_Result[11].GetBool();
            l_Room->m_AccessType              = l_Result[12].GetUInt16();
            l_Room->m_Password                = l_Result[13].GetString();
            l_Room->m_VisitorsNow             = 0;
            l_Room->m_VisitorsMax             = l_Result[14].GetUInt32();
            l_Room->m_RoomVisible             = l_Result[15].GetBool();
            l_Room->m_Rating                  = l_Result[16].GetUInt32();
            l_Room->m_RoomModel               = *GetRoomModel(l_Room->GetModel());
            l_Room->m_RoomCategory            = GetRoomCategory(l_Room->GetCategoryId());
            l_Room->GetRoomCategory()->m_VisitorsMax += l_Room->GetVisitorsMax();
            l_Room->LoadGridData();

            m_Rooms[l_Room->GetId()] = std::move(l_Room);

        } while (l_PreparedResultSet->GetNextRow());

        RoomDatabase.FreePrepareStatement(l_PreparedStatement);

        LOG_INFO << "Loaded " << m_Rooms.size() << " Hotel Rooms";
    }

    /// LoadRoomRights
    /// Load room rights from database
    void RoomManager::LoadRoomRights()
    {
        PreparedStatement* l_PreparedStatement = RoomDatabase.GetPrepareStatement();

        for (auto const& l_Itr : m_Rooms)
        {
            l_PreparedStatement->PrepareStatement("SELECT account_id FROM room_rights WHERE room_id = ?");
            l_PreparedStatement->SetUint32(0, l_Itr.second->GetId());
            std::unique_ptr<PreparedResultSet> l_PreparedResultSet = l_PreparedStatement->ExecuteStatement();

            if (l_PreparedResultSet)
            {
                while (l_PreparedResultSet->GetNextRow())
                {
                    ResultSet* l_Result = l_PreparedResultSet->FetchResult();

                    l_Itr.second->m_SuperRights.insert(l_Result[0].GetUInt32());
                }
            }

        }

        RoomDatabase.FreePrepareStatement(l_PreparedStatement);

        LOG_INFO << "Loaded Rooms Rights!";
    }

    /// LoadVotedUsers
    /// Load room rating from database
    void RoomManager::LoadVotedUsers()
    {
        PreparedStatement* l_PreparedStatement = RoomDatabase.GetPrepareStatement();

        for (auto const& l_Itr : m_Rooms)
        {
            l_PreparedStatement->PrepareStatement("SELECT account_id FROM room_rating WHERE room_id = ? ");
            l_PreparedStatement->SetUint32(0, l_Itr.second->GetId());
            std::unique_ptr<PreparedResultSet> l_PreparedResultSet = l_PreparedStatement->ExecuteStatement();

            if (l_PreparedResultSet)
            {
                while (l_PreparedResultSet->GetNextRow())
                {
                    ResultSet* l_Result = l_PreparedResultSet->FetchResult();

                    l_Itr.second->m_VotedUsers.insert(l_Result[0].GetUInt32());
                }
            }
        }

        RoomDatabase.FreePrepareStatement(l_PreparedStatement);

        LOG_INFO << "Loaded Rooms Ratings!";
    }

    /// AddRoom
    /// Add room to storage from database
    /// @p_RoomId : Id of room we are querying database to get room info
    void RoomManager::AddRoom(uint32 const p_RoomId)
    {
        boost::unique_lock<boost::shared_mutex> l_Guard(m_Mutex);

        PreparedStatement* l_PreparedStatement = RoomDatabase.GetPrepareStatement();
        l_PreparedStatement->PrepareStatement("SELECT id, owner_id, owner_name, category, name, description, model, ccts, wall_paper, "
            "floor, show_name, super_users, access_type, password, visitors_max, room_visible, rating WHERE id = ?");
        l_PreparedStatement->SetUint32(1, p_RoomId);
        std::unique_ptr<PreparedResultSet> l_PreparedResultSet = l_PreparedStatement->ExecuteStatement();

        if (!l_PreparedResultSet)
            return;

        ResultSet* l_Result = l_PreparedResultSet->FetchResult();

        std::unique_ptr<Room> l_Room        = std::make_unique<Room>();
        l_Room->m_Id                        = l_Result[0].GetUInt32();
        l_Room->m_OwnerId                   = l_Result[1].GetUInt32();
        l_Room->m_OwnerName                 = l_Result[2].GetString();
        l_Room->m_CategoryId                = l_Result[3].GetUInt32();
        l_Room->m_Name                      = l_Result[4].GetString();
        l_Room->m_Description               = l_Result[5].GetString();
        l_Room->m_Model                     = l_Result[6].GetString();

        std::vector<std::string> l_Split;
        boost::split(l_Split, l_Result[7].GetString(), boost::is_any_of(","));
        for (auto &l_Itr : l_Split)
        {
            l_Room->m_Ccts.push_back(l_Itr);
        }

        l_Room->m_WallPaper                 = l_Result[8].GetUInt32();
        l_Room->m_Floor                     = l_Result[9].GetUInt32();
        l_Room->m_ShowName                  = l_Result[10].GetBool();
        l_Room->m_AllowSuperRights          = l_Result[11].GetBool();
        l_Room->m_AccessType                = l_Result[12].GetUInt16();
        l_Room->m_Password                  = l_Result[13].GetString();
        l_Room->m_VisitorsNow               = 0;
        l_Room->m_VisitorsMax               = l_Result[14].GetUInt32();
        l_Room->m_RoomVisible               = l_Result[15].GetBool();
        l_Room->m_Rating                    = l_Result[16].GetUInt32();
        l_Room->m_RoomModel                 = *GetRoomModel(l_Room->GetModel());
        l_Room->m_RoomCategory              = GetRoomCategory(l_Room->GetCategoryId());
        l_Room->GetRoomCategory()->m_VisitorsMax += l_Room->GetVisitorsMax();
        l_Room->LoadGridData();

        m_Rooms[l_Room->GetId()] = std::move(l_Room);

        RoomDatabase.FreePrepareStatement(l_PreparedStatement);
    }

    /// UpdateRooms
    /// Update all rooms
    /// @p_Diff - Update tick
    void RoomManager::UpdateRooms(uint32 const& p_Diff)
    {
        boost::shared_lock<boost::shared_mutex> l_Guard(m_Mutex);

        /// Check any rooms are pending for deletion
        IsScheduledToDeleteRoom();

        /// TODO; Don't update rooms which are not active
        for (auto const& l_Itr : m_Rooms)
        {
            l_Itr.second->Update(p_Diff);
        }
    }

    /// IsScheduledToDeleteRoom
    /// Check if we can delete the room
    void RoomManager::IsScheduledToDeleteRoom()
    {
        for (auto l_Itr = m_RoomDeletion.begin(); l_Itr != m_RoomDeletion.end();)
        {
            /// If there is users inside the room, don't delete
            if ((*l_Itr)->GetVisitorsNow() > 0)
                ++l_Itr;
            else
            {
                /// Okay we can delete room
                auto const& l_RemoveItr = m_Rooms.find((*l_Itr)->GetId());
                if (l_RemoveItr != m_Rooms.end())
                    m_Rooms.erase(l_RemoveItr);

                PreparedStatement* l_PreparedStatement = RoomDatabase.GetPrepareStatement();
                l_PreparedStatement->PrepareStatement("DELETE FROM rooms WHERE id = ?");
                l_PreparedStatement->SetUint32(0, (*l_Itr)->GetId());
                l_PreparedStatement->ExecuteStatement();
                RoomDatabase.FreePrepareStatement(l_PreparedStatement);

                l_Itr = m_RoomDeletion.erase(l_Itr);
            }
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
        boost::shared_lock<boost::shared_mutex> l_Guard(m_Mutex);

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
        PreparedStatement* l_PreparedStatement = RoomDatabase.GetPrepareStatement();
        l_PreparedStatement->PrepareStatement("SELECT category, name, description, wall_paper, show_name, super_users, access_type, password, visitors_max, room_visible FROM rooms WHERE id = ?");
        l_PreparedStatement->SetUint32(0, p_Id);
        std::unique_ptr<PreparedResultSet> l_PreparedResultSet = l_PreparedStatement->ExecuteStatement();

        if (!l_PreparedResultSet)
            return;

        ResultSet* l_Result = l_PreparedResultSet->FetchResult();

        std::shared_ptr<Room> l_Room        = GetRoom(p_Id);
        l_Room->m_CategoryId                = l_Result[0].GetUInt32();
        l_Room->m_Name                      = l_Result[1].GetString();
        l_Room->m_Description               = l_Result[2].GetString();
        l_Room->m_WallPaper                 = l_Result[3].GetUInt32();
        l_Room->m_ShowName                  = l_Result[4].GetBool();
        l_Room->m_AllowSuperRights          = l_Result[5].GetBool();
        l_Room->m_AccessType                = l_Result[6].GetUInt16();
        l_Room->m_Password                  = l_Result[7].GetString();
        l_Room->m_VisitorsMax               = l_Result[8].GetUInt32();
        l_Room->m_RoomVisible               = l_Result[9].GetBool();

        ;
        RoomDatabase.FreePrepareStatement(l_PreparedStatement);
    }

    /// ScheduleDeleteRoom
    /// @p_RoomId : Room Id we are preparing to delete
    void RoomManager::ScheduleDeleteRoom(const uint32 p_RoomId)
    {
        auto const& l_Itr = m_Rooms.find(p_RoomId);
        if (l_Itr != m_Rooms.end())
            m_RoomDeletion.push_back(l_Itr->second);
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
