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
    /// Dummy template if no trigger is set
    class TriggerEventExitPool : public BaseTriggerEvent
    {
    public:
        /// Constructor
        TriggerEventExitPool() {}

    public:
        /// OnTriggerEventJoin
        /// @p_Habbo : Habbo user which activated the event
        /// @p_Room : Habbo user which is inside the room
        /// @p_Item : Item which may be apart of the trigger
        void OnTriggerEventJoin(Habbo* p_Habbo, std::shared_ptr<Room> p_Room, Item* p_Item) 
        {
            /// If we are walking then don't execute the trigger
            if (p_Habbo->GetRoom()->HasStatus(p_Habbo->GetRoomGUID(), Status::STATUS_WALKING))
                return;

            if (!p_Habbo->GetRoom()->HasStatus(p_Habbo->GetRoomGUID(), Status::STATUS_SWIMMING))
                return;

            p_Habbo->GetRoom()->RemoveStatus(p_Habbo->GetRoomGUID(), Status::STATUS_SWIMMING);

            if (p_Habbo->GetPositionX() == 21 && p_Habbo->GetPositionY() == 28)
                p_Habbo->TeleportTo(19, 28, 7, p_Habbo->GetBodyRotation());

            else if (p_Habbo->GetPositionX() == 17 && p_Habbo->GetPositionY() == 22)
                p_Habbo->TeleportTo(17, 20, 7, p_Habbo->GetBodyRotation());

            else if (p_Habbo->GetPositionX() == 31 && p_Habbo->GetPositionY() == 11)
                p_Habbo->TeleportTo(31, 9, 7, p_Habbo->GetBodyRotation());

            else if (p_Habbo->GetPositionX() == 20 && p_Habbo->GetPositionY() == 19)
                p_Habbo->TeleportTo(18, 19, 7, p_Habbo->GetBodyRotation());

            else if (p_Habbo->GetPositionX() == 12 && p_Habbo->GetPositionY() == 11)
                p_Habbo->TeleportTo(10, 11, 7, p_Habbo->GetBodyRotation());
        }

        /// OnTriggerEventJoin
        /// @p_Habbo : Habbo user which is leaving the event
        /// @p_Room : Habbo user which is inside the room
        /// @p_Item : Item which may be apart of the trigger
        void OnTriggerEventLeave(Habbo* p_Habbo, std::shared_ptr<Room> p_Room, Item* p_Item) {}
    };
}