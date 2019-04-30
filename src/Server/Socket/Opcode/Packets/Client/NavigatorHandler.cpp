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
#include "HabboSocket.h"
#include "RoomManager.h"
#include "Database/QueryDatabase.h"

#include "Opcode/Packets/Server/LoginPackets.h"
#include "Opcode/Packets/Server/RoomPackets.h"
#include "Opcode/Packets/Server/NavigatorPackets.h"

namespace SteerStone
{
    void HabboSocket::HandleNavigate(ClientPacket* p_Packet)
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
                    switch (l_Room->GetAccessType())
                    {
                    case RoomAccessType::ROOM_ACCESS_TYPE_OPEN:
                        l_ThirdBuffer.AppendString("open");
                        break;
                    case RoomAccessType::ROOM_ACCESS_TYPE_CLOSED:
                        l_ThirdBuffer.AppendString("closed");
                        break;
                    case RoomAccessType::ROOM_ACCESS_TYPE_PASSWORD:
                        l_ThirdBuffer.AppendString("password");
                        break;
                    default:
                        l_ThirdBuffer.AppendString("open");
                        break;
                    }

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
    
    void HabboSocket::HandleGetUserFlatsCategories(ClientPacket* p_Packet)
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
    
    void HabboSocket::HandleSearchFlats(ClientPacket* p_Packet)
    {
        std::string l_Search = p_Packet->GetContent();

        /// TODO; Clean this. We are querying the database and then finding the room in the storage = slow !!
        QueryDatabase l_Database("rooms");
        l_Database.PrepareQuery("SELECT id FROM rooms WHERE category = 2 AND (name LIKE ? OR owner_name LIKE ?)");
        l_Database.GetStatement()->setString(1, l_Search);
        l_Database.GetStatement()->setString(2, l_Search);
        l_Database.ExecuteQuery();

        /// Does any rooms match criteria?
        if (!l_Database.GetResult())
        {
            HabboPacket::Navigator::NoFlats l_Packet;
            m_Habbo->SendPacket(l_Packet.Write());
            return;
        }

        Result* l_Result = l_Database.Fetch();

        HabboPacket::Navigator::FlatResultsSearch l_Packet;

        do
        {
            std::shared_ptr<Room> l_Room = sRoomMgr->GetRoom(l_Result->GetUint32(1));

            if (!l_Room)
                return;

            FlatResultData l_FlatResult;
            l_FlatResult.RoomId     = std::to_string(l_Room->GetId());
            l_FlatResult.RoomName   = l_Room->GetName();
            l_FlatResult.OwnerName  = l_Room->GetOwnerName();

            switch (l_Room->GetAccessType())
            {
            case RoomAccessType::ROOM_ACCESS_TYPE_OPEN:
                l_FlatResult.AccessType = "open";
                break;
            case RoomAccessType::ROOM_ACCESS_TYPE_CLOSED:
                l_FlatResult.AccessType = "closed";
                break;
            case RoomAccessType::ROOM_ACCESS_TYPE_PASSWORD:
                l_FlatResult.AccessType = "password";
                break;
            default:
                l_FlatResult.AccessType = "open";
                break;
            }

            l_FlatResult.VisitorsNow = std::to_string(l_Room->GetVisitorsNow());
            l_FlatResult.VisitorsMax = std::to_string(l_Room->GetVisitorsMax());
            l_FlatResult.Description = l_Room->GetDescription();

            l_Packet.Flats.push_back(l_FlatResult);

        } while (l_Result->GetNextResult());

        m_Habbo->SendPacket(l_Packet.Write());
    }

    void HabboSocket::HandleGetFavouriteRooms(ClientPacket* p_Packet)
    {
        m_Habbo->SendFavouriteRooms();
    }

    void HabboSocket::HandleAddFavouriteRoom(ClientPacket* p_Packet)
    {
        bool l_RoomType = p_Packet->ReadWiredBool();
        uint32 l_RoomId = p_Packet->ReadWiredUint() - PUBLIC_ROOM_OFFSET;

        m_Habbo->AddFavouriteRoom(l_RoomType, l_RoomId);
    }

    void HabboSocket::HandleRemoveFavouriteRoom(ClientPacket* p_Packet)
    {
        bool l_RoomType = p_Packet->ReadWiredBool();
        uint32 l_RoomId = p_Packet->ReadWiredUint() - PUBLIC_ROOM_OFFSET;

        m_Habbo->RemoveFavouriteRoom(l_RoomId);
    }

    void HabboSocket::HandleNodeSpaceUsers(ClientPacket* p_Packet)
    {
        uint32 l_RoomId = p_Packet->ReadWiredUint() - PUBLIC_ROOM_OFFSET;

        std::shared_ptr<Room> l_Room = sRoomMgr->GetRoom(l_RoomId);

        if (!l_RoomId)
            return;

        l_Room->SendNodeSpaceUsers(m_Habbo);
    }

    void HabboSocket::HandleCreateFlat(ClientPacket* p_Packet)
    {
        std::vector<std::string> l_Split;
        boost::split(l_Split, p_Packet->GetContent(), boost::is_any_of("/"));

        std::string l_FloorType = l_Split[1];
        std::string l_RoomName = l_Split[2];
        std::string l_RoomModel = l_Split[3];
        std::string l_RoomAccess = l_Split[4];
        bool l_ShowOwnerName = l_Split[5] == "1" ? true : false;

        bool l_CreateFlat = true;

        if (l_FloorType != "first floor")
            l_CreateFlat = false;

        if (l_RoomName.empty())
            l_CreateFlat = false;

        if (l_RoomModel.empty())
            l_CreateFlat = false;

        /// Credits Quackster for this part
        std::string l_RoomModelContains = l_RoomModel.substr(6);
        if (l_RoomModelContains != "a" &&
            l_RoomModelContains != "c" &&
            l_RoomModelContains != "d" &&
            l_RoomModelContains != "e" &&
            l_RoomModelContains != "f" &&
            l_RoomModelContains != "i" &&
            l_RoomModelContains != "j" &&
            l_RoomModelContains != "k" &&
            l_RoomModelContains != "l" &&
            l_RoomModelContains != "m" &&
            l_RoomModelContains != "n" &&
            !m_Habbo->HasFuseRight(Fuse::UseSpecialRoomLayOuts))
            l_CreateFlat = false;

        /// Error creating flat? let player know
        if (!l_CreateFlat)
        {
            HabboPacket::Login::LocalisedError l_Packet;
            l_Packet.Error = "Error creating a private room";
            m_Habbo->SendPacket(l_Packet.Write());
            return;
        }

        uint16 l_AccessType = RoomAccessType::ROOM_ACCESS_TYPE_OPEN;

        if (l_RoomAccess == "closed")
            l_AccessType = RoomAccessType::ROOM_ACCESS_TYPE_CLOSED;

        if (l_RoomAccess == "password")
            l_AccessType = RoomAccessType::ROOM_ACCESS_TYPE_PASSWORD;

        /// Okay all good! Lets insert our new room into database
        QueryDatabase l_Database("rooms");
        l_Database.PrepareQuery("INSERT INTO rooms (owner_id, owner_name, name, model, show_name, description, access_type) VALUES (?, ?, ?, ?, ?, ?, ?)");
        l_Database.GetStatement()->setUInt(1, m_Habbo->GetId());
        l_Database.GetStatement()->setString(2, m_Habbo->GetName());
        l_Database.GetStatement()->setString(3, l_RoomName);
        l_Database.GetStatement()->setString(4, l_RoomModel);
        l_Database.GetStatement()->setBoolean(5, l_ShowOwnerName);
        l_Database.GetStatement()->setString(6, "");
        l_Database.GetStatement()->setUInt(7, l_AccessType);
        l_Database.ExecuteQuery();

        /// Get the Room Id of room we just created
        l_Database.ClearParameters();

        l_Database.PrepareQuery("SELECT LAST_INSERT_ID() as id");
        l_Database.ExecuteQuery();

        if (l_Database.GetResult())
            m_Habbo->m_LastCreatedRoomId = l_Database.Fetch()->GetInt32(1);

        sRoomMgr->AddRoom(m_Habbo->m_LastCreatedRoomId);

        /// Let client know created room successfully!
        HabboPacket::Room::GoToFlat l_Packet;
        l_Packet.Id = std::to_string(m_Habbo->m_LastCreatedRoomId);
        l_Packet.Name = l_RoomName;
        m_Habbo->SendPacket(l_Packet.Write());
    }

    void HabboSocket::HandleSetFlatInfo(ClientPacket* p_Packet)
    {
        std::string l_Body = p_Packet->GetContent();

        /// Get rid of useless character at start
        if (l_Body[0] == '/')
            l_Body = l_Body.substr(1);

        std::vector<std::string> l_Split;
        boost::split(l_Split, l_Body, boost::is_any_of("/"));

        uint32 l_RoomId = std::stoi(l_Split[0]);

        std::shared_ptr<Room> l_Room = sRoomMgr->GetRoom(l_RoomId);

        if (!l_Room)
            return;

        if (l_Room->GetOwnerId() != m_Habbo->GetId())
            return;

        std::string l_Description = SplitString(l_Split[1], "description");
        std::string l_Password = SplitString(l_Split[1], "password");
        std::string l_MaxVisitors = SplitString(l_Split[1], "maxvisitors");
        bool l_AllSuperUser = SplitString(l_Split[1], "allsuperuser")[0] == '1' ? true : false;

        /// Update our room
        QueryDatabase l_Database("rooms");
        l_Database.PrepareQuery("UPDATE rooms SET description = ?, password = ?, super_users = ?, visitors_max = ? WHERE id = ?");
        l_Database.GetStatement()->setString(1, l_Description);
        l_Database.GetStatement()->setString(2, l_Password);
        l_Database.GetStatement()->setBoolean(3, l_AllSuperUser);
        l_Database.GetStatement()->setUInt(4, l_MaxVisitors.empty() ? l_Room->GetVisitorsMax() : std::stoi(l_MaxVisitors));
        l_Database.GetStatement()->setUInt(5, l_RoomId);
        l_Database.ExecuteQuery();

        /// Okay now reload our room with new details
        sRoomMgr->ReloadRoom(l_Room->GetId());
    }

    void HabboSocket::HandleSearchUserFlats(ClientPacket* p_Packet)
    {
        /// TODO; Clean this. We are querying the database and then finding the room in the storage = slow !!
        QueryDatabase l_Database("rooms");
        l_Database.PrepareQuery("SELECT id FROM rooms WHERE owner_id = ?");
        l_Database.GetStatement()->setUInt(1, m_Habbo->GetId());
        l_Database.ExecuteQuery();

        /// Does user have any flats? if not send packet to inform he/she has no flats
        if (!l_Database.GetResult())
        {
            HabboPacket::Navigator::NoFlatsForUser l_Packet;
            l_Packet.Name = m_Habbo->GetName();
            m_Habbo->SendPacket(l_Packet.Write());
            return;
        }

        Result* l_Result = l_Database.Fetch();

        HabboPacket::Navigator::FlatResults l_Packet;

        do
        {
            std::shared_ptr<Room> l_Room = sRoomMgr->GetRoom(l_Result->GetUint32(1));

            if (!l_Room)
                return;

            FlatResultData l_FlatResult;
            l_FlatResult.RoomId    = std::to_string(l_Room->GetId());
            l_FlatResult.RoomName  = l_Room->GetName();
            l_FlatResult.OwnerName = l_Room->GetOwnerName();

            switch (l_Room->GetAccessType())
            {
            case RoomAccessType::ROOM_ACCESS_TYPE_OPEN:
                l_FlatResult.AccessType = "open";
                break;
            case RoomAccessType::ROOM_ACCESS_TYPE_CLOSED:
                l_FlatResult.AccessType = "closed";
                break;
            case RoomAccessType::ROOM_ACCESS_TYPE_PASSWORD:
                l_FlatResult.AccessType = "password";
                break;
            default:
                l_FlatResult.AccessType = "open";
                break;
            }

            l_FlatResult.VisitorsNow = std::to_string(l_Room->GetVisitorsNow());
            l_FlatResult.VisitorsMax = std::to_string(l_Room->GetVisitorsMax());
            l_FlatResult.Description = l_Room->GetDescription();

            l_Packet.Flats.push_back(l_FlatResult);

        } while (l_Result->GetNextResult());

        m_Habbo->SendPacket(l_Packet.Write());
    }

    void HabboSocket::HandleGetFlatInfo(ClientPacket* p_Packet)
    {
        int32 l_RoomId = std::stoi(p_Packet->GetContent());

        std::shared_ptr<Room> l_Room = sRoomMgr->GetRoom(l_RoomId);

        if (!l_Room)
            return;

        HabboPacket::Navigator::FlatInfo l_Packet;
        l_Packet.AllowSuperUsers    = l_Room->AllowSuperRights();
        l_Packet.AccessType         = l_Room->GetAccessType();
        l_Packet.RoomId             = l_Room->GetId();

        if (!l_Room->ShowName() && m_Habbo->GetId() != l_Room->GetOwnerId() && m_Habbo->GetRank() < AccountRank::HABBO_MODERATOR)
            l_Packet.OwnerName      = "-";
        else
            l_Packet.OwnerName      = l_Room->GetOwnerName();

        l_Packet.Model              = l_Room->GetModel();
        l_Packet.Name               = l_Room->GetName();
        l_Packet.Description        = l_Room->GetDescription();
        l_Packet.ShowOwnerName      = l_Room->ShowName();
        l_Packet.AllowTrading       = l_Room->GetRoomCategory()->CanTrade();
        l_Packet.HasCategory        = l_Room->GetCategoryId() == 2 ? true : false;
        l_Packet.MaxVisitors        = l_Room->GetVisitorsMax();
        l_Packet.NowVisitors        = l_Room->GetVisitorsNow();

        m_Habbo->SendPacket(l_Packet.Write());
    }

    void HabboSocket::HandleGetFlatCategory(ClientPacket* p_Packet)
    {
        uint32 l_RoomId = p_Packet->ReadWiredUint();

        std::shared_ptr<Room> l_Room = sRoomMgr->GetRoom(l_RoomId);

        if (!l_Room)
            return;

        HabboPacket::Navigator::FlatCategory l_Packet;
        l_Packet.RoomId     = l_RoomId;
        l_Packet.CategoryId = l_Room->GetCategoryId();

        m_Habbo->SendPacket(l_Packet.Write());
    }

    void HabboSocket::HandleSetFlatCategory(ClientPacket* p_Packet)
    {
        uint32 l_RoomId = p_Packet->ReadWiredUint();
        uint32 l_CategoryId = p_Packet->ReadWiredUint();

        std::shared_ptr<Room> l_Room = sRoomMgr->GetRoom(l_RoomId);
        RoomCategory* l_RoomCategory = sRoomMgr->GetRoomCategory(l_CategoryId);

        if (!l_Room || !l_RoomCategory)
            return;

        if (l_RoomCategory->GetMinRoleSetFlat() > m_Habbo->GetRank())
            return;
        else if (l_RoomCategory->GetRoomType() != RoomType::ROOM_TYPE_FLAT)
            return;
        else if (l_Room->GetOwnerId() != m_Habbo->GetId())
            return;

        QueryDatabase l_Database("rooms");
        l_Database.PrepareQuery("UPDATE rooms SET category = ? WHERE id = ?");
        l_Database.GetStatement()->setUInt(1, l_CategoryId);
        l_Database.GetStatement()->setUInt(2, l_Room->GetId());
        l_Database.ExecuteQuery();
    }

    void HabboSocket::HandleUpdateFlat(ClientPacket* p_Packet)
    {
        std::string l_Body = p_Packet->GetContent();

        std::vector<std::string> l_Split;
        boost::split(l_Split, l_Body, boost::is_any_of("/"));

        uint32 l_RoomId = std::stoi(l_Split[0]);

        std::shared_ptr<Room> l_Room = sRoomMgr->GetRoom(l_RoomId);

        if (!l_Room)
            return;

        if (l_Room->GetOwnerId() != m_Habbo->GetId())
            return;

        std::string l_RoomName = l_Split[1];
        std::string l_AccessType = l_Split[2];
        bool l_ShowOwner = l_Split[3] == "1" ? true : false;

        uint16 l_NewAcessType = RoomAccessType::ROOM_ACCESS_TYPE_OPEN;

        if (l_AccessType == "closed")
            l_NewAcessType = RoomAccessType::ROOM_ACCESS_TYPE_CLOSED;
        else if (l_AccessType == "password")
            l_NewAcessType = RoomAccessType::ROOM_ACCESS_TYPE_PASSWORD;

        QueryDatabase l_Database("rooms");
        l_Database.PrepareQuery("UPDATE rooms SET name = ?, access_type = ?, show_name = ?  WHERE id = ?");
        l_Database.GetStatement()->setString(1, l_RoomName);
        l_Database.GetStatement()->setUInt(2, l_NewAcessType);
        l_Database.GetStatement()->setBoolean(3, l_ShowOwner);
        l_Database.GetStatement()->setUInt(4, l_Room->GetId());
        l_Database.ExecuteQuery();
    }

    void HabboSocket::HandleRemoveAllRights(ClientPacket* p_Packet)
    {
        uint32 l_RoomId = p_Packet->ReadWiredUint();

        std::shared_ptr<Room> l_Room = sRoomMgr->GetRoom(l_RoomId);
        
        /// Does room exist?
        if (!l_Room)
            return;

        /// Remove rights from all users
        l_Room->RemoveAllUserRights();
    }

    void HabboSocket::HandleDeleteFlat(ClientPacket* p_Packet)
    {
        uint32 l_RoomId = std::stoi(p_Packet->GetContent());
        sRoomMgr->ScheduleDeleteRoom(l_RoomId);
    }

} ///< NAMESPACE STEERSTONE
