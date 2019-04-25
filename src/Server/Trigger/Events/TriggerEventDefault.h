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

#pragma once
#include "Common/SharedDefines.h"
#include "Habbo.h"
#include "RoomManager.h"

#include "Opcode/Packets/Server/RoomPackets.h"

namespace SteerStone
{
    /// Dummy template if no trigger is set
    class TriggerEventDefault : public BaseTriggerEvent
    {
    public:
        /// Constructor
        TriggerEventDefault() {}

    public:
        /// OnTriggerEventJoin
        /// @p_Habbo : Habbo user which activated the event
        /// @p_Room : Habbo user which is inside the room
        /// @p_Item : Item which may be apart of the trigger
        void OnTriggerEventJoin(Habbo* p_Habbo, std::shared_ptr<Room> p_Room, Item* p_Item) 
        {
            /// If we are at the door, then leave the room
            if (p_Habbo->GetRoom())
                if (p_Habbo->GetPositionX() == p_Habbo->GetRoom()->GetRoomModel().GetDoorX() &&
                    p_Habbo->GetPositionY() == p_Habbo->GetRoom()->GetRoomModel().GetDoorY())
                    p_Habbo->GetRoom()->LeaveRoom(p_Habbo, true);
        }


        /// OnTriggerEventJoin
        /// @p_Habbo : Habbo user which is leaving the event
        /// @p_Room : Habbo user which is inside the room
        /// @p_Item : Item which may be apart of the trigger
        void OnTriggerEventLeave(Habbo* p_Habbo, std::shared_ptr<Room> p_Room, Item* p_Item) {}
    };
}