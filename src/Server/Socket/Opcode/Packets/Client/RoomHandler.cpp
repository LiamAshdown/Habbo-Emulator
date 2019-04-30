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

#include "Opcode/Packets/Server/RoomPackets.h"
#include "Opcode/Packets/Server/ItemPackets.h"
#include "Opcode/Packets/Server/MiscPackets.h"
#include "Opcode/Packets/Server/NavigatorPackets.h"

namespace SteerStone
{
    void HabboSocket::HandleGetInterest(ClientPacket* p_Packet)
    {
        HabboPacket::Room::RoomInterest l_Packet;
        SendPacket(l_Packet.Write());
    }

    void HabboSocket::HandleRoomDirectory(ClientPacket* p_Packet)
    {
        bool l_IsPublic = p_Packet->ReadBase64Bool();
        uint32 l_RoomId = p_Packet->ReadWiredUint();

        /// Initialize room connection with client
        HabboPacket::Room::OpenConnection l_Packet;
        SendPacket(l_Packet.Write());

        /// Enter the room
        if (m_Habbo->SetRoom(sRoomMgr->GetRoom(l_RoomId)))
        {
            if (l_IsPublic)
            {
                HabboPacket::Room::RoomUrl l_PacketUrl;
                SendPacket(l_PacketUrl.Write());

                HabboPacket::Room::RoomReady l_PacketRoomReady;
                l_PacketRoomReady.Model = m_Habbo->GetRoom()->GetRoomModel().GetModel();
                l_PacketRoomReady.Id = m_Habbo->GetRoom()->GetId();
                SendPacket(l_PacketRoomReady.Write());
            }
        }
        else
        {
            HabboPacket::Navigator::CantConnect l_Packet;
            l_Packet.ErrorCode = ConnectionError::ROOM_IS_CLOSED;
            m_Habbo->ToSocket()->SendPacket(l_Packet.Write());
        }
    }

    void HabboSocket::HandleGetRoomAdd(ClientPacket* p_Packet)
    {
        HabboPacket::Room::RoomAdd l_Packet;

        if (RoomUrlData* l_RoomUrlData = sRoomMgr->GetRoomUrl(m_Habbo->GetRoom()->GetId()))
        {
            l_Packet.ImageUrl = l_RoomUrlData->ImageUrl;
            l_Packet.LinkUrl = l_RoomUrlData->LinkUrl;
        }

        SendPacket(l_Packet.Write());
    }

    void HabboSocket::HandleGHeightMap(ClientPacket* p_Packet)
    {
        HabboPacket::Room::RoomHeight l_Packet;
        l_Packet.HeightMap = m_Habbo->GetRoom()->GetRoomModel().GetHeightMap();
        SendPacket(l_Packet.Write());
    }

    void HabboSocket::HandleGUsers(ClientPacket* p_Packet)
    {
        /// Handled in Room::EnterRoomCallBack
    }

    void HabboSocket::HandleGObjects(ClientPacket* p_Packet)
    {
        /// Send Furniture and any active furniture to client
        m_Habbo->GetRoom()->SendWorldObjects(m_Habbo);
        m_Habbo->GetRoom()->SendActiveObjects(m_Habbo);
    }

    void HabboSocket::HandleFurnitureRevisions(ClientPacket* p_Packet)
    {
    }

    void HabboSocket::HandleLeaveRoom(ClientPacket* p_Packet)
    {
        m_Habbo->GetRoom()->LeaveRoom(m_Habbo);
    }

    void HabboSocket::HandleMove(ClientPacket* p_Packet)
    {
        if (!m_Habbo->CanWalk())
            return;

        int16 l_X = static_cast<int16>(p_Packet->ReadBase64Int());
        int16 l_Y = static_cast<int16>(p_Packet->ReadBase64Int());
        
        m_Habbo->GetRoom()->Walk(m_Habbo->GetRoomGUID(), l_X, l_Y);
    }

    void HabboSocket::HandleDance(ClientPacket* p_Packet)
    {
        if (!p_Packet->GetContent().empty() && m_Habbo->IsSubscribed())
        {
            int16 l_Id = p_Packet->ReadWiredInt();

            /// Is Id valid?
            if (l_Id < 0 || l_Id > 4)
                return;

            m_Habbo->SetDanceId(l_Id);
        }
        else
            m_Habbo->SetDanceId(0);

        m_Habbo->GetRoom()->RemoveStatus(m_Habbo->GetRoomGUID(), Status::STATUS_WAVING);
        m_Habbo->GetRoom()->AddStatus(m_Habbo->GetRoomGUID(), Status::STATUS_DANCING);
    }

    void HabboSocket::HandleStop(ClientPacket* p_Packet)
    {
        m_Habbo->GetRoom()->RemoveStatus(m_Habbo->GetRoomGUID(), Status::STATUS_DANCING);
    }

    void HabboSocket::HandleGStat(ClientPacket* p_Packet)
    {
        /// Handled in Room::EnterRoomCallBack
    }

    void HabboSocket::HandleWave(ClientPacket* p_Packet)
    {
        m_Habbo->GetRoom()->RemoveStatus(m_Habbo->GetRoomGUID(), Status::STATUS_DANCING);
        m_Habbo->GetRoom()->AddStatus(m_Habbo->GetRoomGUID(), Status::STATUS_WAVING);
    }

    void HabboSocket::HandleLookTo(ClientPacket* p_Packet)
    {
        std::vector<std::string> l_Split;
        boost::split(l_Split, p_Packet->GetContent(), boost::is_any_of(" "));

        m_Habbo->LookTo(std::stoi(l_Split[0]), std::stoi(l_Split[1]));
    }

    void HabboSocket::HandleRoomChat(ClientPacket* p_Packet)
    {
        HabboPacket::Room::Chat l_Packet;
        l_Packet.Message = p_Packet->ReadString();
        l_Packet.GUID = m_Habbo->GetRoomGUID();
        m_Habbo->GetRoom()->SendPacketToAll(l_Packet.Write());
    }

    void HabboSocket::HandleRoomShout(ClientPacket* p_Packet)
    {
        HabboPacket::Room::Shout l_Packet;
        l_Packet.Message = p_Packet->ReadString();
        l_Packet.GUID = m_Habbo->GetRoomGUID();
        m_Habbo->GetRoom()->SendPacketToAll(l_Packet.Write());
    }

    void HabboSocket::HandleRoomWhisper(ClientPacket* p_Packet)
    {
        std::string l_Contents = p_Packet->ReadString();

        HabboPacket::Room::Whisper l_Packet;
        l_Packet.GUID = m_Habbo->GetRoomGUID();

        auto const l_Position = l_Contents.find_first_of(' ');
        if (l_Position != std::string::npos)
        {
            std::string l_Name = l_Contents.substr(0, l_Position);

            if (Habbo* l_Habbo = m_Habbo->GetRoom()->FindHabboByName(l_Name))
            {
                l_Packet.Message = l_Contents.substr(l_Position + 1);
                l_Habbo->SendPacket(l_Packet.Write());
                m_Habbo->SendPacket(l_Packet.Write());
            }
            else
            {
                l_Packet.Message = l_Contents; ///< If there's no username, send it to everyone in room
                m_Habbo->GetRoom()->SendPacketToAll(l_Packet.Write());
            }
        }
    }

    void HabboSocket::HandleSetBadge(ClientPacket* p_Packet)
    {
        std::string l_Badge = p_Packet->ReadString();
        bool l_Visible = p_Packet->ReadWiredBool();
        m_Habbo->SendSetBadge(l_Badge, l_Visible);
    }

    void HabboSocket::HandleGItems(ClientPacket* p_Packet)
    {
        HabboPacket::Room::Items l_Packet;
        m_Habbo->SendPacket(l_Packet.Write());
    }

    void HabboSocket::HandleTryFlat(ClientPacket* p_Packet)
    {
        std::string l_Body = p_Packet->GetContent();

        std::vector<std::string> l_Split;
        boost::split(l_Split, l_Body, boost::is_any_of("/"));

        uint32 l_RoomId = std::stoi(l_Split[0]);

        std::shared_ptr<Room> l_Room = sRoomMgr->GetRoom(l_RoomId);

        /// Does room exist?
        if (!l_Room)
        {
            LOG_ERROR << "Player " << m_Habbo->GetId() << " tried to enter a non existent room!";
            HabboPacket::Room::FlatNotAllowedToEnter l_Packet;
            m_Habbo->SendPacket(l_Packet.Write());
            return;
        }

        /// Check if we have one of the following: Owner, Fuse Right or Super Rights
        if (m_Habbo->HasFuseRight(Fuse::EnterLockedRooms) || m_Habbo->GetId() == l_Room->GetOwnerId() || m_Habbo->GetRoom()->IsSuperUser(m_Habbo->GetId()))
        {
            HabboPacket::Room::FlatLetIn l_Packet;
            m_Habbo->SendPacket(l_Packet.Write());
            return;
        }

        switch (l_Room->GetAccessType())
        {
        case RoomAccessType::ROOM_ACCESS_TYPE_CLOSED:
        {
            if (l_Room->GetVisitorsNow() > 0)
            {
                /// Send door bell ringing to user joining room
                HabboPacket::Room::DoorBellRinging l_Packet;
                m_Habbo->SendPacket(l_Packet.Write());

                /// IMPORTANT:
                /*on showDoorBellDialog(me, tName)
                    tOwnUser = me.getComponent().getOwnUser()
                    if tOwnUser = 0 then
                        return(error(me, "Own user not found!", #showDoorBell, #major))
                        end if
                        if tOwnUser.getInfo().ctrl = 0 then
                            return(1)
                            end if
                            if objectExists(pDoorBellID) then
                                return(getObject(pDoorBellID).addDoorbellRinger(tName))
                                end if
                                exit
                                end*/
                /// Doorbell wont ring for people inside room with super rights if user who is joining
                /// has super rights
                if (Habbo* l_Habbo = l_Room->FindHabboById(l_Room->GetOwnerId()))
                {
                    HabboPacket::Room::DoorBellRinging l_PacketOwner;
                    l_PacketOwner.Name = m_Habbo->GetName();
                    l_Habbo->SendPacket(l_PacketOwner.Write());
                }
                else
                {
                    /// Is there anyone else inside room with rights?
                    for (auto const& l_Itr : *l_Room->GetRoomRightsUsers())
                    {
                        if (Habbo* l_RightsHabbo = l_Room->FindHabboById(l_Itr))
                        {
                            HabboPacket::Room::DoorBellRinging l_PacketRights;
                            l_PacketRights.Name = m_Habbo->GetName();
                            l_RightsHabbo->SendPacket(l_PacketRights.Write());
                        }
                    }
                }

                return;
            }
            else
            {
                /// No one inside room
                HabboPacket::Room::FlatNotAllowedToEnter l_Packet;
                m_Habbo->SendPacket(l_Packet.Write());
                return;
            }
        }
        break;
        case RoomAccessType::ROOM_ACCESS_TYPE_PASSWORD:
        {
            /// Room has password lock
            std::string l_Password = l_Split[1];

            /// Does password match?
            if (l_Password != l_Room->GetPassword() && !m_Habbo->HasFuseRight(Fuse::AnyRoomController))
            {
                HabboPacket::Misc::LocalisedError l_Packet;
                l_Packet.Error = "Incorrect flat password";
                m_Habbo->SendPacket(l_Packet.Write());
                return;
            }
        }
        break;
        case RoomAccessType::ROOM_ACCESS_TYPE_OPEN:
        {
            HabboPacket::Room::FlatLetIn l_Packet;
            m_Habbo->SendPacket(l_Packet.Write());
            return;
        }
        break;
        default:
        {
            /// This should never happen but good to check
            HabboPacket::Misc::LocalisedError l_Packet;
            l_Packet.Error = "weird error";
            m_Habbo->SendPacket(l_Packet.Write());
            return;
        }
        break;
        }

        /// Okay we can go in!
        HabboPacket::Room::FlatLetIn l_Packet;
        m_Habbo->SendPacket(l_Packet.Write());
    }

    void HabboSocket::HandleGoToFlat(ClientPacket* p_Packet)
    {
        uint32 l_RoomId = std::stoi(p_Packet->GetContent());

        /// Enter the room
        if (m_Habbo->SetRoom(sRoomMgr->GetRoom(l_RoomId)))
        {
            HabboPacket::Room::RoomReady l_PacketRoomReady;
            l_PacketRoomReady.Model = m_Habbo->GetRoom()->GetRoomModel().GetModel();
            l_PacketRoomReady.Id = m_Habbo->GetRoom()->GetId();
            m_Habbo->SendPacket(l_PacketRoomReady.Write());
        }
        else
        {
            HabboPacket::Navigator::CantConnect l_Packet;
            l_Packet.ErrorCode = ConnectionError::ROOM_IS_CLOSED;
            m_Habbo->ToSocket()->SendPacket(l_Packet.Write());
        }
    }

    void HabboSocket::HandleAssignRights(ClientPacket* p_Packet)
    {
        std::string l_Username = p_Packet->GetContent();

        /// Is the user allowed to give rights?
        if (m_Habbo->GetId() != m_Habbo->GetRoom()->GetOwnerId() && !m_Habbo->HasFuseRight(Fuse::AnyRoomController))
            return;

        /// Does user exist?
        if (Habbo* l_Habbo = m_Habbo->GetRoom()->FindHabboByName(l_Username))
            m_Habbo->GetRoom()->AddUserRights(l_Habbo);
    }

    void HabboSocket::HandleRemoveRights(ClientPacket* p_Packet)
    {
        std::string l_Username = p_Packet->GetContent();

        /// Is the user allowed to remove rights?
        if (m_Habbo->GetId() != m_Habbo->GetRoom()->GetOwnerId() && !m_Habbo->HasFuseRight(Fuse::AnyRoomController))
            return;

        /// Does user exist?
        if (Habbo* l_Habbo = m_Habbo->GetRoom()->FindHabboByName(l_Username))
            m_Habbo->GetRoom()->RemoveUserRights(l_Habbo->GetId());
    }

    void HabboSocket::HandleKickUser(ClientPacket* p_Packet)
    {
        std::string l_Username = p_Packet->GetContent();

        /// Cannot kick if you are not a super user, owner, or have fuse right
        if (m_Habbo->GetId() != m_Habbo->GetRoom()->GetOwnerId() && !m_Habbo->GetRoom()->IsSuperUser(m_Habbo->GetId()) && !m_Habbo->HasFuseRight(Fuse::Kick))
            return;

        /// Cannot kick yourself
        if (m_Habbo->GetName() == l_Username)
            return;

        /// Cannot kick room owner if you are not moderator
        if (m_Habbo->GetRoom()->GetOwnerName() == l_Username && !m_Habbo->HasFuseRight(Fuse::Kick))
            return;

        /// Cannot kick user who has fuse right
        if (Habbo* l_Habbo = m_Habbo->GetRoom()->FindHabboByName(l_Username))
            if (l_Habbo->HasFuseRight(Fuse::Kick))
                return;
            else
            {
                /// Make user walk to door to leave room
                if (l_Habbo->GetRoom()->Walk(l_Habbo->GetRoomGUID(), l_Habbo->GetRoom()->GetRoomModel().GetDoorX(), l_Habbo->GetRoom()->GetRoomModel().GetDoorY()))
                    /// Prevent user from creating a new path
                    l_Habbo->SetCanWalk(false);
                else ///< If we cannot form a path, just kick the user
                    l_Habbo->GetRoom()->LeaveRoom(l_Habbo, true);
            }
    }

    void HabboSocket::HandleLetInUser(ClientPacket* p_Packet)
    {
        std::string l_Username = p_Packet->ReadString();
        l_Username.pop_back(); /// Remove last character from string since it's garbage
        bool l_Allowed = p_Packet->ReadWiredBool();

        Habbo* l_Habbo = m_Habbo->GetRoom()->FindHabboByName(l_Username);

        /// Should never happpen
        if (!l_Habbo)
            return;

        if (l_Allowed)
        {
            HabboPacket::Room::FlatLetIn l_Packet;
            l_Habbo->SendPacket(l_Packet.Write());
        }
        else
        {
            HabboPacket::Room::FlatNotAllowedToEnter l_Packet;
            l_Habbo->SendPacket(l_Packet.Write());
        }
    }
}