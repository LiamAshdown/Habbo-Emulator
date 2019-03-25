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

#include "FavouriteRoom.h"
#include "Database/QueryDatabase.h"
#include "RoomManager.h"
#include "Config/Config.h"
#include "Network/StringBuffer.h"

namespace SteerStone
{
    /// Constructor
    /// @p_Id : Account Id, we don't use Habbo class here
    FavouriteRoom::FavouriteRoom(const uint32& id) : m_Id(id)
    {
    }
    
    /// Deconstructor
    FavouriteRoom::~FavouriteRoom()
    {
        m_FavouriteRooms.clear();
        m_DeletedFavouriteRooms.clear();
    }
    
    /// LoadFavouriteRooms - Load our Favourite Rooms from l_Database
    void FavouriteRoom::LoadFavouriteRooms()
    {
        m_FavouriteRooms.clear();

        QueryDatabase l_Database("rooms");
        l_Database.PrepareQuery("SELECT id, room_id, public_space FROM favourite_rooms WHERE id = ?");
        l_Database.GetStatement()->setUInt(1, m_Id);
        l_Database.ExecuteQuery();

        if (!l_Database.GetResult())
            return;

        Result* l_Result = l_Database.Fetch();

        do
        {
            FavouriteRoomsData l_FavouriteRoom;
            l_FavouriteRoom.m_Id = l_Result->GetUint32(1);
            l_FavouriteRoom.m_RoomId = l_Result->GetUint32(2);
            l_FavouriteRoom.m_RoomType = l_Result->GetBool(3);

            m_FavouriteRooms.push_back(l_FavouriteRoom);

        } while (l_Result->GetNextResult());
    }
    
    /// AddFavouriteRoom
    /// @p_IsPublic : Is Room public or flat
    /// @p_RoomId : Room Id
    void FavouriteRoom::AddFavouriteRoom(bool const& p_IsPublic, uint32 const& p_RoomId)
    {
        if (m_FavouriteRooms.size() >= sConfig->GetIntDefault("MaxFavouriteRooms", 50))
            return;

        auto const& l_Itr = std::find_if(m_FavouriteRooms.begin(), m_FavouriteRooms.end(), 
            [p_RoomId](const FavouriteRoomsData& p_FavRoom) 
            {
                return p_FavRoom.GetRoomId() == p_RoomId;
            });

        if (l_Itr != m_FavouriteRooms.end())
            return;

        FavouriteRoomsData l_FavouriteRoom;
        l_FavouriteRoom.m_Id = m_Id;
        l_FavouriteRoom.m_RoomType = p_IsPublic;
        l_FavouriteRoom.m_RoomId = p_RoomId;

        m_FavouriteRooms.push_back(l_FavouriteRoom);
    }
    
    /// RemoveFavouriteRoom
    /// @p_RoomId : Room Id
    void FavouriteRoom::RemoveFavouriteRoom(uint32 const& p_RoomId)
    {
        auto const& l_Itr = std::find_if(m_FavouriteRooms.begin(), m_FavouriteRooms.end(),
            [p_RoomId](const FavouriteRoomsData& p_FavRoom)
            {
                return p_FavRoom.GetRoomId() == p_RoomId;
            });

        if (l_Itr != m_FavouriteRooms.end())
        {
            m_DeletedFavouriteRooms.push_back(*l_Itr);
            m_FavouriteRooms.erase(l_Itr);
        }
    }
    
    /// SaveToDB - Update our favourite rooms
    void FavouriteRoom::SaveToDB()
    {
        QueryDatabase l_Database("rooms");

        for (auto const& l_Itr : m_FavouriteRooms)
        {
            FavouriteRoomsData const& l_FavouriteRoom = l_Itr;

            l_Database.PrepareQuery("INSERT INTO favourite_rooms (id, room_id, public_space) VALUES (?, ?, ?) ON DUPLICATE KEY UPDATE room_id = ?");
            l_Database.GetStatement()->setUInt(1, l_FavouriteRoom.GetId());
            l_Database.GetStatement()->setUInt(2, l_FavouriteRoom.GetRoomId());
            l_Database.GetStatement()->setBoolean(3, l_FavouriteRoom.GetRoomType());
            l_Database.GetStatement()->setUInt(4, l_FavouriteRoom.GetRoomId());
            l_Database.ExecuteQuery();
        }

        for (auto const& l_Itr : m_DeletedFavouriteRooms)
        {
            l_Database.PrepareQuery("DELETE FROM favourite_rooms WHERE room_id = ?");
            l_Database.GetStatement()->setUInt(1, l_Itr.GetRoomId());
            l_Database.ExecuteQuery();
        }
    }
    
    /// ParseSendFavouriteRooms
    /// @p_Buffer : Buffer which is being parsed
    void FavouriteRoom::ParseSendFavouriteRooms(StringBuffer& buffer)
    {
        StringBuffer l_SecondBuffer;

        uint32 l_PrivateFlatSize = 0;
        buffer.AppendWired(0);
        buffer.AppendWired(0);
        buffer.AppendWired(2);
        buffer.AppendString("");
        buffer.AppendWired(0);
        buffer.AppendWired(0);
        buffer.AppendWired(0);

        if (!m_FavouriteRooms.empty())
        {
            for (const auto& l_Itr : m_FavouriteRooms)
            {
                FavouriteRoomsData const& l_FavouriteRoom = l_Itr;
                std::shared_ptr<Room> l_Room = sRoomMgr->GetRoom(l_FavouriteRoom.GetRoomId());

                if (!l_Room)
                    continue;

                if (l_FavouriteRoom.GetRoomType() == RoomType::ROOM_TYPE_FLAT)
                {
                    l_SecondBuffer.AppendWired(l_Room->GetId());
                    l_SecondBuffer.AppendString(l_Room->GetName());
                    l_SecondBuffer.AppendString(l_Room->GetOwnerName());
                    l_SecondBuffer.AppendString(l_Room->GetAccessType());
                    l_SecondBuffer.AppendWired(l_Room->GetVisitorsNow());
                    l_SecondBuffer.AppendWired(l_Room->GetVisitorsMax());
                    l_SecondBuffer.AppendString(l_Room->GetDescription());
                    l_PrivateFlatSize++;
                }
                else
                {
                    l_SecondBuffer.AppendWired(l_Room->GetId() + PUBLIC_ROOM_OFFSET);
                    l_SecondBuffer.AppendWired(1);
                    l_SecondBuffer.AppendString(l_Room->GetName());
                    l_SecondBuffer.AppendWired(l_Room->GetVisitorsNow());
                    l_SecondBuffer.AppendWired(l_Room->GetVisitorsMax());
                    l_SecondBuffer.AppendWired(l_Room->GetCategoryId());
                    l_SecondBuffer.AppendString(l_Room->GetDescription());
                    l_SecondBuffer.AppendWired(l_Room->GetId());
                    l_SecondBuffer.AppendWired(0);
                    l_SecondBuffer.AppendString(l_Room->GetCcts());
                    l_SecondBuffer.AppendWired(0);
                    l_SecondBuffer.AppendWired(1);
                }
            }
        }

        buffer.AppendWired(l_PrivateFlatSize);
        buffer.Append(l_SecondBuffer);
    }
} ///< NAMESPACE STEERSTONE
