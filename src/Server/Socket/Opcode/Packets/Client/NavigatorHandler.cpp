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

#include "HabboSocket.h"
#include "Habbo.h"
#include "Database/QueryDatabase.h"
#include "RoomManager.h"

namespace SteerStone
{
    void HabboSocket::HandleNavigate(std::unique_ptr<ClientPacket> p_Packet)
    {
        bool l_HideFullRooms = p_Packet->ReadWiredBool();
        int32 l_CategoryId = p_Packet->ReadWiredInt();

        RoomCategory* l_RoomCategory = sRoomMgr->GetRoomCategory(l_CategoryId);

        StringBuffer l_Buffer;
        StringBuffer l_SecondBuffer;
        StringBuffer l_ThirdBuffer;

        uint32 l_NowVisitors = 0;
        uint32 l_MaxVisitors = 0;
        uint32 l_GuestRooms = 0;

        for (auto const& l_Itr : *sRoomMgr->GetRooms())
        {
            RoomCategory* l_Category = sRoomMgr->GetRoomCategory(l_Itr.second->GetCategoryId());
            std::shared_ptr<Room> l_Room = l_Itr.second;

            if (l_HideFullRooms)
                if (l_Room->GetVisitorsNow() == l_Room->GetVisitorsMax())
                    continue;

            if (!l_Room->IsRoomVisible() && m_Habbo->GetRank() < AccountRank::HABBO_MODERATOR)
                continue;

            if (l_Room->GetCategoryId() == l_CategoryId)
            {
                /// Total Now/Max Visitors in specific category
                l_NowVisitors += l_Room->GetVisitorsNow();
                l_MaxVisitors += l_Room->GetVisitorsMax();

                if (l_Category->GetRoomType() == RoomType::ROOM_TYPE_PUBLIC)
                {
                    l_ThirdBuffer.AppendWired(l_Room->GetId() + PUBLIC_ROOM_OFFSET);
                    l_ThirdBuffer.AppendWired(1);
                    l_ThirdBuffer.AppendString(l_Room->GetName());
                    l_ThirdBuffer.AppendWired(l_Room->GetVisitorsNow());
                    l_ThirdBuffer.AppendWired(l_Room->GetVisitorsMax());
                    l_ThirdBuffer.AppendWired(l_Room->GetCategoryId());
                    l_ThirdBuffer.AppendString(l_Room->GetDescription());
                    l_ThirdBuffer.AppendWired(l_Room->GetId());
                    l_ThirdBuffer.AppendWired(0);
                    l_ThirdBuffer.AppendString(l_Room->GetCcts());
                    l_ThirdBuffer.AppendWired(0);
                    l_ThirdBuffer.AppendWired(1);
                }
                else
                {
                    l_ThirdBuffer.AppendWired(l_Room->GetId());
                    l_ThirdBuffer.AppendString(l_Room->GetName());
                    l_ThirdBuffer.AppendString(l_Room->GetOwnerName());
                    l_ThirdBuffer.AppendString(l_Room->GetAccessType());
                    l_ThirdBuffer.AppendWired(l_Room->GetVisitorsNow());
                    l_ThirdBuffer.AppendWired(l_Room->GetVisitorsMax());
                    l_ThirdBuffer.AppendString(l_Room->GetDescription());
                    l_GuestRooms++;
                }
            }
        }

        if (l_RoomCategory->GetRoomType() == RoomType::ROOM_TYPE_FLAT)
            l_SecondBuffer.AppendWired(l_GuestRooms);

        l_Buffer.AppendBase64(PacketServerHeader::SERVER_NAVIGATE_NODE_INFO);
        l_Buffer.AppendWired(l_HideFullRooms);
        l_Buffer.AppendWired(l_CategoryId);
        l_Buffer.AppendWired(l_RoomCategory->GetRoomType() ? 0 : 2);
        l_Buffer.AppendString(l_RoomCategory->GetName());
        l_Buffer.AppendWired(l_NowVisitors);
        l_Buffer.AppendWired(l_MaxVisitors);
        l_Buffer.AppendWired(l_RoomCategory->GetParentCategoryId());

        l_Buffer.Append(l_SecondBuffer);
        l_Buffer.Append(l_ThirdBuffer);

        for (auto& l_Itr : *sRoomMgr->GetRoomCategories())
        {
            RoomCategory* l_RoomCategory = &l_Itr.second;

            if (m_Habbo->GetRank() < l_RoomCategory->GetMinRoleAccess())
                continue;

            l_Buffer.AppendWired(l_RoomCategory->GetCategoryId());
            l_Buffer.AppendWired(0);
            l_Buffer.AppendString(l_RoomCategory->GetName());
            l_Buffer.AppendWired(l_RoomCategory->GetVisitorsNow());
            l_Buffer.AppendWired(l_RoomCategory->GetVisitorsMax());
            l_Buffer.AppendWired(l_RoomCategory->GetParentCategoryId());
        }

        l_Buffer.AppendSOH();
        SendPacket(&l_Buffer);
    }
    
    void HabboSocket::HandleGetUserFlatsCategories(std::unique_ptr<ClientPacket> p_Packet)
    {
        StringBuffer l_Buffer;
        StringBuffer l_SecondBuffer;
        uint32 l_FlatCategorySize = 0;

        for (auto const& l_Itr : *sRoomMgr->GetRoomCategories())
        {
            RoomCategory l_RoomCategory = l_Itr.second;

            if (l_RoomCategory.GetRoomType() == RoomType::ROOM_TYPE_FLAT)
            {
                l_SecondBuffer.AppendWired(l_RoomCategory.GetCategoryId());
                l_SecondBuffer.AppendString(l_RoomCategory.GetName());
                l_FlatCategorySize++;
            }
        }
        l_Buffer.AppendBase64(PacketServerHeader::SERVER_USER_FLAT_CATEGORIES);
        l_Buffer.AppendWired(l_FlatCategorySize);
        l_Buffer.Append(l_SecondBuffer);
        l_Buffer.AppendSOH();
        SendPacket(&l_Buffer);
    }
    
    void HabboSocket::HandleSearchRooms(std::unique_ptr<ClientPacket> p_Packet)
    {
        std::string l_Search = p_Packet->ReadString();

        StringBuffer l_Buffer;
        l_Buffer.AppendBase64(PacketServerHeader::SERVER_FLAT_RESULTS_1);
        for (auto const& l_Itr : *sRoomMgr->GetRooms())
        {
            std::shared_ptr<Room> l_Room = l_Itr.second;

            // TODO fix this structure
            if (l_Room->GetName().find(l_Search) != std::string::npos)
            {
                l_Buffer.AppendWired(l_Room->GetId());
                l_Buffer.AppendString(l_Room->GetName());
                l_Buffer.AppendString(l_Room->GetOwnerName());
                l_Buffer.AppendString(l_Room->GetAccessType());
                l_Buffer.AppendString("x");
                l_Buffer.AppendWired(l_Room->GetVisitorsNow());
                l_Buffer.AppendWired(l_Room->GetVisitorsMax());
                l_Buffer.AppendString("null");
                l_Buffer.AppendString(l_Room->GetDescription());
                l_Buffer.AppendString(l_Room->GetDescription());
            }
        }

        l_Buffer.AppendSOH();
        SendPacket(&l_Buffer);
    }

    void HabboSocket::HandleGetFavouriteRooms(std::unique_ptr<ClientPacket> p_Packet)
    {
        m_Habbo->SendFavouriteRooms();
    }

    void HabboSocket::HandleAddFavouriteRoom(std::unique_ptr<ClientPacket> p_Packet)
    {
        bool l_RoomType = p_Packet->ReadWiredBool();
        uint32 l_RoomId = p_Packet->ReadWiredUint() - PUBLIC_ROOM_OFFSET;

        m_Habbo->AddFavouriteRoom(l_RoomType, l_RoomId);
    }

    void HabboSocket::HandleRemoveFavouriteRoom(std::unique_ptr<ClientPacket> p_Packet)
    {
        bool l_RoomType = p_Packet->ReadWiredBool();
        uint32 l_RoomId = p_Packet->ReadWiredUint() - PUBLIC_ROOM_OFFSET;

        m_Habbo->RemoveFavouriteRoom(l_RoomId);
    }

    void HabboSocket::HandleNodeSpaceUsers(std::unique_ptr<ClientPacket> p_Packet)
    {
        uint32 l_RoomId = p_Packet->ReadWiredUint() - PUBLIC_ROOM_OFFSET;

        /* StringBuffer buffer;
         buffer.AppendBase64(PacketClientHeader::CLIENT_NO_DESPACE_USERS);
         buffer.AppendString(mPlayer->GetName());
         buffer.AppendSOH();
         SendPacket(buffer);*/
    }
    
} ///< NAMESPACE STEERSTONE
