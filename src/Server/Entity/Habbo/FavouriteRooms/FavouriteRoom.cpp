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

#include "Opcode/Packets/Server/MiscPackets.h"
#include "Database/DatabaseTypes.h"
#include "RoomManager.h"
#include "Config/Config.h"
#include "Habbo.h"


namespace SteerStone
{
    /// Constructor
    /// @p_Habbo : Habbo class which owns the favourite rooms
    FavouriteRoom::FavouriteRoom(Habbo* p_Habbo) : m_Habbo(p_Habbo)
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

        PreparedStatement* l_PreparedStatement = RoomDatabase.GetPrepareStatement();
        l_PreparedStatement->PrepareStatement("SELECT id, room_id, public_space FROM favourite_rooms WHERE id = ?");
        l_PreparedStatement->SetUint32(0, m_Habbo->GetId());
        std::unique_ptr<PreparedResultSet> l_PreparedResultSet = l_PreparedStatement->ExecuteStatement();

        if (!l_PreparedResultSet)
        {
            RoomDatabase.FreePrepareStatement(l_PreparedStatement);
            return;
        }

        do
        {
            ResultSet* l_Result = l_PreparedResultSet->FetchResult();

            FavouriteRoomsData l_FavouriteRoom;
            l_FavouriteRoom.m_Id        = l_Result[0].GetUInt32();
            l_FavouriteRoom.m_RoomId    = l_Result[1].GetUInt32();
            l_FavouriteRoom.m_RoomType  = l_Result[2].GetBool();

            m_FavouriteRooms.push_back(l_FavouriteRoom);

        } while (l_PreparedResultSet->GetNextRow());

        UserDatabase.FreePrepareStatement(l_PreparedStatement);
    }
    
    /// AddFavouriteRoom
    /// @p_IsPublic : Is room public or flat
    /// @p_RoomId : Room Id
    void FavouriteRoom::AddFavouriteRoom(bool const& p_IsPublic, uint32 const& p_RoomId)
    {
        if (m_FavouriteRooms.size() >= MAX_FAVOURITE_ROOMS)
        {
            HabboPacket::Misc::LocalisedError l_Packet;
            l_Packet.Error = "nav_error_toomanyfavrooms";
            m_Habbo->SendPacket(l_Packet.Write());
            return;
        }

        auto const& l_Itr = std::find_if(m_FavouriteRooms.begin(), m_FavouriteRooms.end(), 
            [p_RoomId](const FavouriteRoomsData& p_FavRoom) 
            {
                return p_FavRoom.GetRoomId() == p_RoomId;
            });

        if (l_Itr != m_FavouriteRooms.end())
            return;

        FavouriteRoomsData l_FavouriteRoom;
        l_FavouriteRoom.m_Id        = m_Habbo->GetId();
        l_FavouriteRoom.m_RoomType  = p_IsPublic;
        l_FavouriteRoom.m_RoomId    = p_RoomId;

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
        PreparedStatement* l_PreparedStatement = RoomDatabase.GetPrepareStatement();

        for (auto const& l_Itr : m_FavouriteRooms)
        {
            FavouriteRoomsData const& l_FavouriteRoom = l_Itr;

            l_PreparedStatement->PrepareStatement("INSERT INTO favourite_rooms (id, room_id, public_space) VALUES (?, ?, ?) ON DUPLICATE KEY UPDATE room_id = VALUES(room_id)");
            l_PreparedStatement->SetUint32(0, l_FavouriteRoom.GetId());
            l_PreparedStatement->SetUint32(1, l_FavouriteRoom.GetRoomId());
            l_PreparedStatement->SetBool(2, l_FavouriteRoom.GetRoomType());
            l_PreparedStatement->ExecuteStatement();
        }

        for (auto const& l_Itr : m_DeletedFavouriteRooms)
        {
            l_PreparedStatement->PrepareStatement("DELETE FROM favourite_rooms WHERE room_id = ?");
            l_PreparedStatement->SetUint32(0, l_Itr.GetRoomId());
            l_PreparedStatement->ExecuteStatement();

        }

        RoomDatabase.FreePrepareStatement(l_PreparedStatement);
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

                    switch (l_Room->GetAccessType())
                    {
                    case RoomAccessType::ROOM_ACCESS_TYPE_OPEN:
                        l_SecondBuffer.AppendString("open");
                        break;
                    case RoomAccessType::ROOM_ACCESS_TYPE_CLOSED:
                        l_SecondBuffer.AppendString("closed");
                        break;
                    case RoomAccessType::ROOM_ACCESS_TYPE_PASSWORD:
                        l_SecondBuffer.AppendString("password");
                        break;
                    default:
                        l_SecondBuffer.AppendString("open");
                        break;
                    }

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
