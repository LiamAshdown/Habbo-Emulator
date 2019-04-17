/*
* Liam Ashdown
* Copyright (C) 2019
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as publ      ished by
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

#pragma once
#include "Common/SharedDefines.h"
#include "Habbo.h"
#include "RoomManager.h"

#include "Opcode/Packets/Server/ItemPackets.h"
#include "Opcode/Packets/Server/RoomPackets.h"

namespace SteerStone
{
    /// This is called when user enters a booth
    class TriggerEventPoolBooth : public BaseTriggerEvent
    {
    public:
        /// Constructor
        TriggerEventPoolBooth() {}

    public:
        /// OnTriggerEventJoin
        /// @p_Habbo : Habbo user which activated the event
        /// @p_Room : Habbo user which is inside the room
        /// @p_Item : Item which may be apart of the trigger
        void OnTriggerEventJoin(Habbo* p_Habbo, std::shared_ptr<Room> p_Room, Item* p_Item) 
        {
            /// Send our UI Makeup for swim wear
            HabboPacket::Room::OpenUIMakeOPPI l_PacketMakeUp;
            p_Habbo->SendPacket(l_PacketMakeUp.Write());

            p_Item->SetState("close");
            HabboPacket::Item::ShowProgram l_Packet;
            l_Packet.Program = p_Item->GetProgram();
            l_Packet.State   = p_Item->GetState();
            p_Room->SendPacketToAll(l_Packet.Write());
        }

        /// OnTriggerEventJoin
        /// @p_Habbo : Habbo user which is leaving the event
        /// @p_Room : Habbo user which is inside the room
        /// @p_Item : Item which may be apart of the trigger
        void OnTriggerEventLeave(Habbo* p_Habbo, std::shared_ptr<Room> p_Room, Item* p_Item) 
        {
            p_Item->SetState("open");

            /// Time to open the curtains so other habbos can use
            HabboPacket::Item::ShowProgram l_Packet;
            l_Packet.Program = p_Item->GetProgram();
            l_Packet.State   = p_Item->GetState();
            p_Habbo->GetRoom()->SendPacketToAll(l_Packet.Write());

            /// Send our updated Figure
            HabboPacket::Room::HabboRoomObject l_PacketFigure;
            l_PacketFigure.UniqueId     = boost::lexical_cast<std::string>(p_Habbo->GetRoomGUID());
            l_PacketFigure.Id           = boost::lexical_cast<std::string>(p_Habbo->GetId());
            l_PacketFigure.Name         = p_Habbo->GetName();
            l_PacketFigure.Figure       = p_Habbo->GetFigure();
            l_PacketFigure.PoolFigure   = p_Habbo->GetRoom()->GetCcts("hh_people_pool").empty() ? std::string() : p_Habbo->GetPoolFigure();
            l_PacketFigure.Gender       = p_Habbo->GetGender() == "Male" ? "M" : "F";
            l_PacketFigure.X            = boost::lexical_cast<std::string>(p_Habbo->GetPositionX());
            l_PacketFigure.Y            = boost::lexical_cast<std::string>(p_Habbo->GetPositionY());
            l_PacketFigure.Z            = boost::lexical_cast<std::string>(p_Habbo->GetPositionZ());
            l_PacketFigure.Motto        = p_Habbo->GetMotto();
            p_Habbo->SendPacket(l_PacketFigure.Write());

            p_Habbo->GetRoom()->Walk(p_Habbo->GetRoomGUID(), p_Habbo->GetPositionX() + 2, p_Habbo->GetPositionY());
        }
    };
}