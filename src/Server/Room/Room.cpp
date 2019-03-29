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
#include "ItemManager.h"
#include "Opcode/Packets/Server/RoomPackets.h"
#include "Common/Maths.h"

namespace SteerStone
{
    /// Constructor
    Room::Room(){}
    
    /// Deconstructor
    Room::~Room(){}
    
    /// SendNewUserObjectToRoom
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

        /// Update our Habbo Position so habbo appears at the door
        p_Habbo->UpdatePosition(GetRoomModel().GetDoorX(), GetRoomModel().GetDoorY(), GetRoomModel().GetDoorZ(),
            GetRoomModel().GetDoorOrientation());

        p_Habbo->SetRoomGUID(GenerateGUID());

        m_Mutex.lock();
        /// Push our habbo into our room storage
        m_Habbos[p_Habbo->GetRoomGUID()] = p_Habbo;
        /// Increment our visitor counter
        m_VisitorsNow++;
        GetRoomCategory()->GetVisitorsNow()++; ///< Also Increment our category visitor counter
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
            /// Check if there's any active path, and clear it
            auto const& l_ItrPath = m_Paths.find(p_Habbo->GetRoomGUID());
            if (l_ItrPath != m_Paths.end())
            {
                /// Gets removed on next room update
                l_ItrPath->second.Path.clear();
            }

            /// Set Habbo room to nullptr
            l_Itr->second->DestroyRoom();

            /// Erase Habbo from room
            m_Habbos.erase(l_Itr);

            /// Send User Objects packet again to all players, so habbo who left no longer appears on other clients
            SendUserObjectToRoom();

            /// Decrement our VisitorNow counter, habbo is leaving
            m_Mutex.lock();
            m_VisitorsNow--;
            GetRoomCategory()->GetVisitorsNow()--; ///< Also Decrement our category visitor counter
            m_Mutex.unlock();
        }
        else
            LOG_INFO << "Player " << p_Habbo->GetName() << " tried to leave Room Id " << GetId() << " but player does not exist in room!";
    }

    /// GenerateGUID - Generate a unique ID for object in room
    uint32 Room::GenerateGUID()
    {
        /// TODO; Check whether guid already exists
        return Maths::GetRandomUint32(1, 9999);
    }
    
    /// Send Habbo Figure object to clients in room
     /// @p_Habbo : Habbo
    void Room::SendNewUserObjectToRoom(Habbo* p_Habbo)
    {
        for (auto const& l_Itr : m_Habbos)
        {
            Habbo* l_Habbo = l_Itr.second;

            if (l_Habbo->GetId() != p_Habbo->GetId())
            {
                HabboPacket::Room::HabboRoomObject l_Packet;
                l_Packet.UniqueId   = boost::lexical_cast<std::string>(l_Itr.first);
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
            l_Packet.UniqueId       = boost::lexical_cast<std::string>(l_Itr.first);
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

    /// SendUserObjectToRoom - This function is used when habbo leaves room, and we need to update habbo objects again
    void Room::SendUserObjectToRoom()
    {
        for (auto const& l_Itr : m_Habbos)
        {
            Habbo* l_Habbo = l_Itr.second;
            SendNewUserObjectToRoom(l_Habbo);
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
                PublicItem const* l_Item = &l_Itr;

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

    /// SendObjects 
    /// @p_Habbo : Send Active Furniture Objects to Habbo client
    void Room::SendActiveObjects(Habbo* p_Habbo)
    {
        HabboPacket::Room::ActiveObjects l_Packet;
        p_Habbo->ToSocket()->SendPacket(l_Packet.Write());
    }

    /// Walk 
    /// @p_Habbo : Habbo class which is walking
    /// @p_EndX : End Position habbo is going to
    /// @p_EndY : End Position habbo is going to
    void Room::Walk(Habbo* p_Habbo, uint16 const p_EndX, uint16 const p_EndY)
    {
        boost::shared_lock<boost::shared_mutex> l_Lock(m_Mutex);

        /// Calculate the path
        PathFinder l_Path(GetRoomModel().GetGrid());
        l_Path.CalculatePath(p_Habbo->GetPositionX(), p_Habbo->GetPositionY(), p_EndX, p_EndY);

        /// Check if user already has an existing path
        auto& l_Itr = m_Paths.find(p_Habbo->GetRoomGUID());
        if (l_Itr != m_Paths.end())
        {
            /// Clear our paths, and insert in our new path
            l_Itr->second.Path.clear();
            l_Itr->second.Path = l_Path.GetPath();
            return;
        }
        
        PathFindingData& l_Points = m_Paths[p_Habbo->GetRoomGUID()];
        l_Points.Habbo            = p_Habbo;
        l_Points.Path             = l_Path.GetPath();
    }

    /// Update 
    /// @p_Diff : Update the room
    void Room::Update(uint32 const p_Diff)
    {
        boost::shared_lock<boost::shared_mutex> l_Lock(m_Mutex);

        /// Loop through all current paths
        for (auto& l_Itr = m_Paths.begin(); l_Itr != m_Paths.end();)
        {
            PathFindingData& l_Path = l_Itr->second;
            if (l_Path.Path.empty())
                l_Itr = m_Paths.erase(l_Itr);
            else
            {
                /// Retrieve our next way point
                Position& l_Position = l_Path.Path.back();

                /// Remove our way point
                l_Path.Path.pop_back();

                LOG_DEBUG << l_Position.X << " " << l_Position.Y;

                ++l_Itr;
            }
        }
    }
} ///< NAMESPACE STEERSTONE
