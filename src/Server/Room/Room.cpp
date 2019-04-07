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

#include "Room.h"
#include "Habbo.h"
#include "Opcode/Packets/Server/RoomPackets.h"
#include "Common/Maths.h"

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

                /// Add Item to Tile
                if (GetRoomCategory()->GetRoomType() == RoomType::ROOM_TYPE_PUBLIC)
                    GetRoomModel().TileGrid[l_X][l_Y]->AddItem(sItemMgr->GetPublicItemByPosition(GetModel(), l_X, l_Y));
            }
        }    
    }

    /// AddFigure
    /// Send Habbo Figure object to clients in room
    /// @p_Habbo : p_Habbo
    bool Room::EnterRoom(Habbo* p_Habbo)
    {
        /// Check if habbo already exists inside room
        auto const& l_Itr = m_Habbos.find(p_Habbo->GetRoomGUID());
        if (l_Itr != m_Habbos.end())
        {
            LOG_ERROR << "Habbo " << p_Habbo->GetName() << " tried to enter Room Id " << GetId() << " but is already inside room!";
            return true;
        }

        /// Room is full
        if (m_VisitorsNow >= m_VisitorsMax)
        {
            HabboPacket::Room::RoomCantConnect l_Packet;
            l_Packet.ErrorCode = RoomConnectionError::ROOM_IS_FULL;
            return false;
        }

        /// Update our Habbo Position to be at the door
        p_Habbo->UpdatePosition(GetRoomModel().GetDoorX(), GetRoomModel().GetDoorY(), GetRoomModel().GetDoorZ(),
            GetRoomModel().GetDoorOrientation());

        p_Habbo->SetRoomGUID(GenerateGUID());

        m_Mutex.lock();
        m_Habbos[p_Habbo->GetRoomGUID()] = std::make_unique<RoomHabboInfo>(p_Habbo);
        m_VisitorsNow++;
        GetRoomCategory()->GetVisitorsNow()++;
        m_Mutex.unlock();

        return true;
    }
    
    /// EnterRoom 
    /// @p_Habbo : Habbo user leaving room
    void Room::LeaveRoom(Habbo* p_Habbo)
    {
        auto const& l_Itr = m_Habbos.find(p_Habbo->GetRoomGUID());
        if (l_Itr != m_Habbos.end())
        {
            /// Notify other users that we left the room
            SendUserLeftRoom(l_Itr->second->ToHabbo()->GetRoomGUID());

            /// Set Habbo room to nullptr
            l_Itr->second->ToHabbo()->DestroyRoom();

            m_Mutex.lock();
            m_Habbos.erase(l_Itr);
            m_VisitorsNow--;
            GetRoomCategory()->GetVisitorsNow()--;
            m_Mutex.unlock();
        }
        else
            LOG_INFO << "Player " << p_Habbo->GetName() << " tried to leave Room Id " << GetId() << " but player does not exist in room!";
    }

    /// GenerateGUID 
    /// Generate a unique ID for new object in room
    uint32 Room::GenerateGUID()
    {
        /// TODO; Check whether guid already exists
        return Maths::GetRandomUint32(1, 9999);
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
                l_Packet.Gender     = p_Habbo->GetGender() == "Male" ? "M" : "F";
                l_Packet.X          = boost::lexical_cast<std::string>(p_Habbo->GetPositionX());
                l_Packet.Y          = boost::lexical_cast<std::string>(p_Habbo->GetPositionY());
                l_Packet.Z          = boost::lexical_cast<std::string>(p_Habbo->GetPositionZ());
                l_Packet.Motto      = p_Habbo->GetMotto();
                l_Habbo->ToSocket()->SendPacket(l_Packet.Write());
            }

            HabboPacket::Room::HabboRoomObject l_Packet;
            l_Packet.UniqueId       = boost::lexical_cast<std::string>(l_Habbo->GetRoomGUID());
            l_Packet.Id             = boost::lexical_cast<std::string>(l_Habbo->GetId());
            l_Packet.Name           = l_Habbo->GetName();
            l_Packet.Figure         = l_Habbo->GetFigure();
            l_Packet.Gender         = l_Habbo->GetGender() == "Male" ? "M" : "F";
            l_Packet.X              = boost::lexical_cast<std::string>(l_Habbo->GetPositionX());
            l_Packet.Y              = boost::lexical_cast<std::string>(l_Habbo->GetPositionY());
            l_Packet.Z              = boost::lexical_cast<std::string>(l_Habbo->GetPositionZ());
            l_Packet.Motto          = l_Habbo->GetMotto();
            p_Habbo->ToSocket()->SendPacket(l_Packet.Write());
        }
    }

    /// SendRoomStatuses
    /// Send Habbo Statuses to user joining room
    /// @p_Habbo : Habbo which is joining room
    void Room::SendRoomStatuses(Habbo * p_Habbo)
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
            l_Packet.Waving         = l_Itr.second->HasStatus(Status::STATUS_WAVING);

            p_Habbo->ToSocket()->SendPacket(l_Packet.Write());
        }
    }

    /// SendUserLeftRoom
    void Room::SendUserLeftRoom(uint32 const p_GUID)
    {
        for (auto const& l_Itr : m_Habbos)
        {
            Habbo* l_Habbo = l_Itr.second->ToHabbo();

            HabboPacket::Room::LeaveRoom l_Packet;
            l_Packet.GUID = p_GUID;
            l_Habbo->ToSocket()->SendPacket(l_Packet.Write());
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

            p_Habbo->ToSocket()->SendPacket(l_Packet.Write());
        }
        else
        {
            /// TODO; Flat Furniture
        }
    }

    /// SendPacketToAll
    /// Send Packet to all users in room
    /// @p_Buffer : Data being sent to users in room
    void Room::SendPacketToAll(StringBuffer const* p_Buffer)
    {
        for (auto const& l_Itr : m_Habbos)
        {
            l_Itr.second->ToHabbo()->ToSocket()->SendPacket(p_Buffer);
        }
    }

    /// SendObjects 
    /// @p_Habbo : Send Active Furniture Objects to Habbo user
    void Room::SendActiveObjects(Habbo* p_Habbo)
    {
        HabboPacket::Room::ActiveObjects l_Packet;
        p_Habbo->ToSocket()->SendPacket(l_Packet.Write());
    }

    /// Walk 
    /// @p_Habbo : Habbo user who is walking
    /// @p_EndX : End Position habbo is going to
    /// @p_EndY : End Position habbo is going to
    void Room::Walk(uint32 const p_RoomGUID, uint16 const p_EndX, uint16 const p_EndY)
    {
        auto const& l_Itr = m_Habbos.find(p_RoomGUID);
        if (l_Itr != m_Habbos.end())
            l_Itr->second->CreatePath(p_EndX, p_EndY);
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
        auto& l_Itr = m_Habbos.find(p_RoomGUID);
        if (l_Itr != m_Habbos.end())
            return l_Itr->second->HasStatus(p_Status);
    }

    /// FindHabboByName
    /// @p_Name : Name of user to find
    Habbo * Room::FindHabboByName(std::string const p_Name)
    {
        for (auto const& l_Itr : m_Habbos)
        {
            if (l_Itr.second->ToHabbo()->GetName() == p_Name)
                return l_Itr.second->ToHabbo();
        }

        return nullptr;
    }

    /// ProcessUserActions
    /// Process Habbo Actions; Status, pathfinding, etc..
    void Room::ProcessUserActions(const uint32 p_Diff)
    {
        for (auto& l_Itr : m_Habbos)
        {
            l_Itr.second->ProcessActions(p_Diff);
        }
    }

    /// Update 
    /// Update all objects in room
    /// @p_Diff : Hotel last tick time
    void Room::Update(uint32 const p_Diff)
    {
        ProcessUserActions(p_Diff);
    }
} ///< NAMESPACE STEERSTONE
