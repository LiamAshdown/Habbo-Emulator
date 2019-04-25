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

#include "Habbo.h"
#include "RoomManager.h"
#include "TriggerEventManager.h"
#include "Opcode/Packets/Server/RoomPackets.h"
#include "Opcode/Packets/Server/NavigatorPackets.h"
#include "Common/Maths.h"
#include "Database/QueryDatabase.h"
#include <functional>

namespace SteerStone
{
    /// Constructor
    Room::Room(){}
    
    /// Deconstructor
    Room::~Room(){}
    
    /// LoadGridData
    void Room::LoadGridData()
    {
        std::vector<std::string> l_Split;
        boost::split(l_Split, GetRoomModel().GetHeightMap(), boost::is_any_of("\r"));

        GetRoomModel().m_MapSizeY = l_Split.size();
        GetRoomModel().m_MapSizeX = l_Split[0].length();
        GetRoomModel().TileGrid.resize(boost::extents[GetRoomModel().m_MapSizeX][GetRoomModel().m_MapSizeY]);

        /// Load Static Grid Data
        for (int32 l_Y = 0; l_Y < GetRoomModel().m_MapSizeY; l_Y++)
        {
            std::string l_Line = l_Split[l_Y];

            for (int32 l_X = 0; l_X < GetRoomModel().m_MapSizeX; l_X++)
            {     
                /// I don't like this, is there a better way of doing this?
                GetRoomModel().TileGrid[l_X][l_Y] = new TileInstance(l_X, l_Y);

                uint8 l_Tile = l_Line[l_X];

                if (std::isdigit(l_Tile)) ///< If the tile is a number, it means we can use it
                {
                    GetRoomModel().TileGrid[l_X][l_Y]->m_TileState = TileState::TILE_STATE_OPEN;
                    GetRoomModel().TileGrid[l_X][l_Y]->m_TileHeight = l_Tile - 48; ///< Ascii format
                }
                else ///< Else the tile is closed
                {
                    GetRoomModel().TileGrid[l_X][l_Y]->m_TileState = TileState::TILE_STATE_CLOSED;
                    GetRoomModel().TileGrid[l_X][l_Y]->m_TileHeight = 0;
                }

                if (GetRoomModel().GetDoorX() == l_X && GetRoomModel().GetDoorY() == l_Y)
                {
                    GetRoomModel().TileGrid[l_X][l_Y]->m_TileState = TileState::TILE_STATE_OPEN;
                    GetRoomModel().TileGrid[l_X][l_Y]->m_TileHeight = GetRoomModel().GetDoorZ();
                }

                /// Add static Public Item to tile
                if (GetRoomCategory()->GetRoomType() == RoomType::ROOM_TYPE_PUBLIC)
                {
                    GetRoomModel().TileGrid[l_X][l_Y]->AddItem(sItemMgr->GetPublicItemByPosition(GetModel(), l_X, l_Y));

                    if (Item* l_Item = GetRoomModel().TileGrid[l_X][l_Y]->GetItem())
                    {
                        /// Load our item triggers
                        for (auto const& l_Itr : l_Item->GetTrigger())
                        {
                            GetRoomModel().TileGrid[l_X][l_Y]->AddTrigger(sTriggerMgr->GetTrigger(l_Itr));
                        }
                    }
                    else ///< If Item doesn't exist, then just add a default trigger (which does nothing)
                        GetRoomModel().TileGrid[l_X][l_Y]->AddTrigger(sTriggerMgr->GetTrigger("default"));

                    /// Add WalkWay tiles
                    GetRoomModel().TileGrid[l_X][l_Y]->AddWalkWay(sRoomMgr->GetWalkWay(GetId(), l_X, l_Y));
                }
                else
                {
                    GetRoomModel().TileGrid[l_X][l_Y]->AddTrigger(sTriggerMgr->GetTrigger("default")); ///< Temporary
                }
            }
        }    
    }

    /// EnterRoom 
    /// @p_Habbo : Habbo user entering in room
    /// @p_WalkWay : Walkway tile position
    bool Room::EnterRoom(Habbo* p_Habbo, WalkWay* p_WalkWay /*= nullptr*/)
    {
        /// Check if habbo already exists inside room
        auto const& l_Itr = m_Habbos.find(p_Habbo->GetRoomGUID());
        if (l_Itr != m_Habbos.end())
        {
            LOG_ERROR << "Habbo " << p_Habbo->GetName() << " tried to enter Room Id " << GetId() << " but is already inside room!";
            return false;
        }

        /// Room is full
        if (m_VisitorsNow >= m_VisitorsMax)
        {
            HabboPacket::Navigator::CantConnect l_Packet;
            l_Packet.ErrorCode = ConnectionError::ROOM_IS_FULL;
            p_Habbo->ToSocket()->SendPacket(l_Packet.Write());
            return false;
        }

        /// If we were on a walk way tile, use walk way position
        if (p_WalkWay)
            p_Habbo->UpdatePosition(p_WalkWay->GetToPositionX(), p_WalkWay->GetToPositionY(), p_WalkWay->GetToPositionZ(), p_WalkWay->GetToRotation());
        else ///< else player will appear at the door
            p_Habbo->UpdatePosition(GetRoomModel().GetDoorX(), GetRoomModel().GetDoorY(), GetRoomModel().GetDoorZ(),
                GetRoomModel().GetDoorOrientation());

        /// User can walk
        p_Habbo->SetCanWalk(true);

        p_Habbo->SetRoomGUID(Maths::GetRandomUint32(1, 9999));

        /// Send update that we joined the room
        p_Habbo->SendMessengerUpdate();

        m_FunctionCallBack.AddCallBack(std::bind(&Room::EnterRoomCallBack, this, p_Habbo));

        return true;
    }

    /// EnterRoomCallBack 
    /// Gets processed on Room::Update
    /// @p_Habbo : Habbo user entering in room
    void Room::EnterRoomCallBack(Habbo* p_Habbo)
    {
        m_Habbos[p_Habbo->GetRoomGUID()] = std::make_unique<RoomHabboInfo>(p_Habbo);

        AddFigure(p_Habbo);
        RefreshRights(p_Habbo);
        SendRoomStatuses(p_Habbo);

        m_VisitorsNow++;
        GetRoomCategory()->GetVisitorsNow()++;
    }
    
    /// @p_Habbo : Habbo user leaving room
    /// @p_HotelView : Kick user to hotel view
    void Room::LeaveRoom(Habbo* p_Habbo, bool p_HotelView /*= false*/)
    {
        auto const& l_Itr = m_Habbos.find(p_Habbo->GetRoomGUID());
        if (l_Itr != m_Habbos.end())
        {
            /// Set current tile occupied to false since we are leaving room
            if (TileInstance* l_TileInstance = GetRoomModel().GetTileInstance(l_Itr->second->m_Habbo->GetPositionX(), l_Itr->second->m_Habbo->GetPositionY()))
                l_TileInstance->SetTileOccupied(false, l_Itr->second->ToHabbo());

            /// Send update that we left the room
            l_Itr->second->m_Habbo->SendMessengerUpdate();

            /// User cannot walk
            p_Habbo->SetCanWalk(false);

            /// If true, kick user to hotel view
            if (p_HotelView)
            {
                HabboPacket::Room::CLC l_Packet;
                p_Habbo->SendPacket(l_Packet.Write());
            }

            /// Set our room to null
            l_Itr->second->m_Habbo->DestroyRoom();

            /// Notify other users that we left the room
            SendUserLeftRoom(l_Itr->first);

            m_FunctionCallBack.AddCallBack(std::bind(&Room::LeaveRoomCallBack, this, l_Itr));
        }
        else
            LOG_INFO << "Player " << p_Habbo->GetName() << " tried to leave Room Id " << GetId() << " but player does not exist in room!";
    }

    /// LeaveRoomCallBack
    /// Gets processed on Room::Update
    /// @p_Itr : Iteration of habbo which going to be removed
    void Room::LeaveRoomCallBack(GUIDUserMap::iterator& p_Itr)
    {
        if (p_Itr->second->ToHabbo()->IsScheduledForDelete())
            p_Itr->second->ToHabbo()->SetIsScheduledForDelete(false);

        m_Habbos.erase(p_Itr);

        m_VisitorsNow--;
        GetRoomCategory()->GetVisitorsNow()--;
    }
    
    /// Send Habbo Figure to clients in room
    /// @p_Habbo : Habbo which is joining room
    void Room::AddFigure(Habbo* p_Habbo)
    {
        for (auto const& l_Itr : m_Habbos)
        {
            Habbo* l_Habbo = l_Itr.second->ToHabbo();

            if (l_Habbo->GetId() != p_Habbo->GetId())
            {
                HabboPacket::Room::HabboRoomObject l_Packet;
                l_Packet.UniqueId   = boost::lexical_cast<std::string>(p_Habbo->GetRoomGUID());
                l_Packet.Id         = boost::lexical_cast<std::string>(p_Habbo->GetId());
                l_Packet.Name       = p_Habbo->GetName();
                l_Packet.Figure     = p_Habbo->GetFigure();
                l_Packet.PoolFigure = GetCcts("hh_people_pool").empty() ? std::string() : p_Habbo->GetPoolFigure();
                l_Packet.Gender     = p_Habbo->GetGender() == "Male" ? "M" : "F";
                l_Packet.Badge      = p_Habbo->GetCurrentBadge();
                l_Packet.X          = boost::lexical_cast<std::string>(p_Habbo->GetPositionX());
                l_Packet.Y          = boost::lexical_cast<std::string>(p_Habbo->GetPositionY());
                l_Packet.Z          = boost::lexical_cast<std::string>(p_Habbo->GetPositionZ());
                l_Packet.Motto      = p_Habbo->GetMotto();
                l_Packet.Badge      = p_Habbo->GetCurrentBadge();
                l_Habbo->SendPacket(l_Packet.Write());
            }

            HabboPacket::Room::HabboRoomObject l_Packet;
            l_Packet.UniqueId       = boost::lexical_cast<std::string>(l_Habbo->GetRoomGUID());
            l_Packet.Id             = boost::lexical_cast<std::string>(l_Habbo->GetId());
            l_Packet.Name           = l_Habbo->GetName();
            l_Packet.Figure         = l_Habbo->GetFigure();
            l_Packet.PoolFigure     = GetCcts("hh_people_pool").empty() ? std::string() : l_Habbo->GetPoolFigure();
            l_Packet.Gender         = l_Habbo->GetGender() == "Male" ? "M" : "F";
            l_Packet.Badge          = l_Habbo->GetCurrentBadge();
            l_Packet.X              = boost::lexical_cast<std::string>(l_Habbo->GetPositionX());
            l_Packet.Y              = boost::lexical_cast<std::string>(l_Habbo->GetPositionY());
            l_Packet.Z              = boost::lexical_cast<std::string>(l_Habbo->GetPositionZ());
            l_Packet.Motto          = l_Habbo->GetMotto();
            p_Habbo->SendPacket(l_Packet.Write());
        }
    }

    /// SendRoomStatuses
    /// Send Habbo Statuses to user joining room
    /// @p_Habbo : Habbo which is joining room
    void Room::SendRoomStatuses(Habbo* p_Habbo)
    {
        for (auto const& l_Itr : m_Habbos)
        {
            Habbo* l_Habbo = l_Itr.second->ToHabbo();

            HabboPacket::Room::UserUpdateStatus l_Packet;
            l_Packet.GUID           = std::to_string(l_Habbo->GetRoomGUID());
            l_Packet.CurrentX       = std::to_string(l_Habbo->GetPositionX());
            l_Packet.CurrentY       = std::to_string(l_Habbo->GetPositionY());
            l_Packet.CurrentZ       = std::to_string(l_Habbo->GetPositionZ());
            l_Packet.BodyRotation   = std::to_string(l_Habbo->GetBodyRotation());
            l_Packet.HeadRotation   = std::to_string(l_Habbo->GetHeadRotation());
            l_Packet.Sitting        = l_Itr.second->HasStatus(Status::STATUS_SITTING);
            l_Packet.Walking        = l_Itr.second->HasStatus(Status::STATUS_WALKING);
            l_Packet.Dancing        = l_Itr.second->HasStatus(Status::STATUS_DANCING);
            l_Packet.DanceId        = std::to_string(l_Habbo->GetDanceId());
            l_Packet.Waving         = l_Itr.second->HasStatus(Status::STATUS_WAVING);
            l_Packet.Swimming       = l_Itr.second->HasStatus(Status::STATUS_SWIMMING);
            l_Packet.HasController  = l_Itr.second->HasStatus(Status::STATUS_CONTROLLER);
            l_Packet.IsOwner        = l_Itr.second->HasStatus(Status::STATUS_OWNER);
            p_Habbo->SendPacket(l_Packet.Write());
        }
    }

    /// SendUserLeftRoom
    void Room::SendUserLeftRoom(uint32 const p_GUID)
    {
        for (auto const& l_Itr : m_Habbos)
        {
            HabboPacket::Room::Logout l_Packet;
            l_Packet.GUID = std::to_string(p_GUID);
            l_Itr.second->ToHabbo()->SendPacket(l_Packet.Write());
        }
    }
    
    /// SendWorldObjects 
    /// @p_Habbo : Send Furniture Objects to Habbo client
    void Room::SendWorldObjects(Habbo* p_Habbo)
    {
        HabboPacket::Room::ObjectsWorld l_Packet;

        if (GetRoomCategory()->GetRoomType() == RoomType::ROOM_TYPE_PUBLIC)
        {
            for (auto const& l_Itr : sItemMgr->GetPublicRoomItems(GetModel()))
            {
                Item const* l_Item = &l_Itr;

                WorldObject l_WorldObject;
                l_WorldObject.Id = boost::lexical_cast<std::string>(l_Item->GetId());
                l_WorldObject.Sprite = boost::lexical_cast<std::string>(l_Item->GetSprite());
                l_WorldObject.X = boost::lexical_cast<std::string>(l_Item->GetPositionX());
                l_WorldObject.Y = boost::lexical_cast<std::string>(l_Item->GetPositionY());
                l_WorldObject.Z = boost::lexical_cast<std::string>(l_Item->GetPositionZ());
                l_WorldObject.Rotation = boost::lexical_cast<std::string>(l_Item->GetRotation());
                l_Packet.WorldObjects.push_back(l_WorldObject);
            }
        }
        else
        {
            /// TODO; Flat Furniture
        }
        p_Habbo->SendPacket(l_Packet.Write());
    }

    /// SendPacketToAll
    /// Send Packet to all users in room
    /// @p_Buffer : Data being sent to users in room
    void Room::SendPacketToAll(StringBuffer const* p_Buffer)
    {
        for (auto const& l_Itr : m_Habbos)
        {
            Habbo* l_Habbo = l_Itr.second->ToHabbo();

            l_Habbo->SendPacket(p_Buffer);
        }
    }

    /// SendObjects 
    /// @p_Habbo : Send Active Furniture Objects to Habbo user
    void Room::SendActiveObjects(Habbo* p_Habbo)
    {
        HabboPacket::Room::ActiveObjects l_Packet;
        p_Habbo->SendPacket(l_Packet.Write());
    }

    /// Walk 
    /// @p_Habbo : Habbo user who is walking
    /// @p_EndX : End Position habbo is going to
    /// @p_EndY : End Position habbo is going to
    bool Room::Walk(uint32 const p_RoomGUID, uint16 const p_EndX, uint16 const p_EndY)
    {
        auto const& l_Itr = m_Habbos.find(p_RoomGUID);
        if (l_Itr != m_Habbos.end())
            return l_Itr->second->CreatePath(p_EndX, p_EndY);

        return false;
    }

    /// AddStatus
    /// @p_RoomGUID : Room GUID of user
    /// @p_Status : Habbo Status to be added
    void Room::AddStatus(uint32 const p_RoomGUID, uint32 const p_Status)
    {
        auto const& l_Itr = m_Habbos.find(p_RoomGUID);
        if (l_Itr != m_Habbos.end())
            l_Itr->second->AddStatus(p_Status);
    }

    /// RemoveStatus
    /// @p_RoomGUID : Room GUID of user
    /// @p_Status : Habbo Status to be removed
    void Room::RemoveStatus(uint32 const p_RoomGUID, uint32 const p_Status)
    {
        auto const& l_Itr = m_Habbos.find(p_RoomGUID);
        if (l_Itr != m_Habbos.end())
            l_Itr->second->RemoveStatus(p_Status);
    }

    /// HasStatus
    /// Check if user has an active status
    /// @p_RoomGUID : Room GUID of user
    /// @p_Status : Status to check
    bool Room::HasStatus(uint32 const p_RoomGUID, uint32 const p_Status) const
    {
        auto const& l_Itr = m_Habbos.find(p_RoomGUID);
        if (l_Itr != m_Habbos.end())
            return l_Itr->second->HasStatus(p_Status);
        else
            return false;
    }

    /// ProcessUserActions
    /// Process Habbo Actions; Status, pathfinding, etc..
    void Room::ProcessUserActions(const uint32 p_Diff)
    {
        for (auto const& l_Itr : m_Habbos)
        {
            if (l_Itr.second->ToHabbo()->IsScheduledForDelete())
                continue;

            l_Itr.second->ProcessActions(p_Diff);
        }
    }

    /// GetCcts
    /// Get Ccts
    /// @p_Specified : Return specified Ccts
    std::string Room::GetCcts(std::string const p_Specified /*= std::string()*/)
    {
        for (auto& l_Itr : m_Ccts)
        {
            if (p_Specified.empty())
                return l_Itr;
            else
                if (l_Itr == p_Specified)
                    return l_Itr;
        }

        return std::string();
    }

    /// SendNodeSpaceUsers
    /// @p_Habbo : Packet to send to
    void Room::SendNodeSpaceUsers(Habbo* p_Habbo)
    {
        HabboPacket::Navigator::NodeSpaceUsers l_Packet;
        for (auto const& l_Itr : m_Habbos)
            l_Packet.Names.push_back(l_Itr.second->ToHabbo()->GetName());
        
        p_Habbo->SendPacket(l_Packet.Write());
    }

    /// FindHabboByGuid
    /// @p_Id : Room GUID we are searching for
    Habbo* Room::FindHabboByGuid(uint32 const p_GUID)
    {
        auto const& l_Itr = m_Habbos.find(p_GUID);
        if (l_Itr != m_Habbos.end())
            return l_Itr->second->ToHabbo();
        else
            return nullptr;
    }

    /// FindHabboById
    /// @p_Id : Id of user we are searching for
    Habbo* Room::FindHabboById(uint32 const p_Id)
    {
        for (auto const& l_Itr : m_Habbos)
        {
            if (l_Itr.second->ToHabbo()->GetId() == p_Id)
                return l_Itr.second->ToHabbo();
        }
        return nullptr;
    }

    /// FindHabboByName
    /// @p_Name : Name of user to find
    Habbo* Room::FindHabboByName(std::string const p_Name)
    {
        for (auto const& l_Itr : m_Habbos)
        {
            if (l_Itr.second->ToHabbo()->GetName() == p_Name)
                return l_Itr.second->ToHabbo();
        }

        return nullptr;
    }

    ///////////////////////////////////////////
    //             SUPER RIGHTS
    ///////////////////////////////////////////

    /// RefreshRights
    /// Send Rights to user whether they are owner, super user or normal user
    /// @p_Habbo : Rights we are sending to
    void Room::RefreshRights(Habbo* p_Habbo)
    {
        auto const& l_Itr = m_SuperRights.find(p_Habbo->GetId());
        if (l_Itr != m_SuperRights.end())
        {
            p_Habbo->GetRoom()->AddStatus(p_Habbo->GetRoomGUID(), Status::STATUS_CONTROLLER);
            HabboPacket::Room::YouAreController l_Packet;
            p_Habbo->SendPacket(l_Packet.Write());
        }
        else
        {
            p_Habbo->GetRoom()->RemoveStatus(p_Habbo->GetRoomGUID(), Status::STATUS_CONTROLLER);
            HabboPacket::Room::YouAreNotController l_Packet;
            p_Habbo->SendPacket(l_Packet.Write());
        }

        /// Seperate check for owner and fuse right
        if (p_Habbo->GetId() == GetOwnerId() || p_Habbo->HasFuseRight(Fuse::AnyRoomController))
        {
            p_Habbo->GetRoom()->AddStatus(p_Habbo->GetRoomGUID(), Status::STATUS_OWNER);
            HabboPacket::Room::YouAreOwner l_Packet;
            p_Habbo->SendPacket(l_Packet.Write());
        }
    }

    /// GetRoomUsers
    /// Returns storage of users with room rights
    std::set<uint32>* Room::GetRoomRightsUsers()
    {
        boost::shared_lock<boost::shared_mutex> l_Guard(m_SuperRightsMutex);

        return &m_SuperRights;
    }

    /// AddFuserRights
    /// @p_Habbo : Habbo user we are adding user rights to
    void Room::AddUserRights(Habbo* p_Habbo)
    {
        boost::unique_lock<boost::shared_mutex> l_Guard(m_SuperRightsMutex);

        auto const& l_Itr = m_SuperRights.find(p_Habbo->GetId());
        if (l_Itr == m_SuperRights.end())
        {
            m_SuperRights.insert(p_Habbo->GetId());

            /// Refresh rights to update client
            RefreshRights(p_Habbo);

            /// And update the database
            QueryDatabase l_Database("rooms");
            l_Database.PrepareQuery("INSERT INTO room_rights (id, room_id) VALUES (?, ?)");
            l_Database.GetStatement()->setUInt(1, p_Habbo->GetId());
            l_Database.GetStatement()->setUInt(2, GetId());
            l_Database.ExecuteQuery();
        }
    }

    /// RemoveUserRights
    /// @p_Id : User we are removing user rights from
    void Room::RemoveUserRights(uint32 const p_Id)
    {
        boost::unique_lock<boost::shared_mutex> l_Guard(m_SuperRightsMutex);

        auto& l_Itr = m_SuperRights.find(p_Id);
        if (l_Itr != m_SuperRights.end())
        {
            m_SuperRights.erase(l_Itr);

            /// If user is online, refresh rights
            if (Habbo* l_Habbo = FindHabboById(p_Id))
                l_Habbo->GetRoom()->RefreshRights(l_Habbo);

            /// And update the database
            QueryDatabase l_Database("rooms");
            l_Database.PrepareQuery("DELETE FROM room_rights WHERE id = ? AND room_id = ?");
            l_Database.GetStatement()->setUInt(1, p_Id);
            l_Database.GetStatement()->setUInt(2, GetId());
            l_Database.ExecuteQuery();
        }
    }

    /// IsSuperUser
    /// @p_Id : Id we are checking if user has super rights
    bool Room::IsSuperUser(uint32 const p_Id)
    {
        boost::shared_lock<boost::shared_mutex> l_Guard(m_SuperRightsMutex);

        auto const& l_Itr = m_SuperRights.find(p_Id);
        if (l_Itr != m_SuperRights.end())
            return true;
        
        return false;
    }

    /// RemoveAllUserRights
    /// Remove all users who have super rights
    void Room::RemoveAllUserRights()
    {
        for (auto const& l_Itr : m_SuperRights)
            RemoveUserRights(l_Itr);
    }

    /// Update 
    /// Update all objects in room
    /// @p_Diff : Hotel last tick time
    void Room::Update(uint32 const p_Diff)
    {
        /// Process any pending functions
        m_FunctionCallBack.ProcessFunctions();

        ProcessUserActions(p_Diff);
    }
} ///< NAMESPACE STEERSTONE
