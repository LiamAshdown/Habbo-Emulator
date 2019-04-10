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

namespace SteerStone
{
    /// Triggers the sit animation
    class TriggerEventSit : public BaseTriggerEvent
    {
    public:
        /// Constructor
        TriggerEventSit() {}

    public:
        /// OnTriggerEventJoin
        /// Execute the sit animation
        /// @p_Habbo : Habbo user which activated the event
        /// @p_Room : Habbo user which is inside the room
        /// @p_Item : Item which may be apart of the trigger
        void OnTriggerEventJoin(Habbo* p_Habbo, std::shared_ptr<Room> p_Room, Item* p_Item)
        {
            p_Habbo->GetRoom()->RemoveStatus(p_Habbo->GetRoomGUID(), Status::STATUS_DANCING);
            p_Habbo->GetRoom()->AddStatus(p_Habbo->GetRoomGUID(), Status::STATUS_SITTING);
            p_Habbo->UpdatePosition(p_Item->GetPositionX(), p_Item->GetPositionY(), p_Item->GetPositionZ(), p_Item->GetRotation());
        }

        /// OnTriggerEventJoin
        /// @p_Habbo : Habbo user which is leaving the event
        /// @p_Room : Habbo user which is inside the room
        /// @p_Item : Item which may be apart of the trigger
        void OnTriggerEventLeave(Habbo* p_Habbo, std::shared_ptr<Room> p_Room, Item* p_Item) 
        {
        }
    };
}