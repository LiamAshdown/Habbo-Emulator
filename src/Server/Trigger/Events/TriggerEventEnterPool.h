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
    class TriggerEventEnterPool : public BaseTriggerEvent
    {
    public:
        /// Constructor
        TriggerEventEnterPool() {}

    public:
        /// OnTriggerEventJoin
        /// @p_Habbo : Habbo user which activated the event
        /// @p_Room : Habbo user which is inside the room
        /// @p_Item : Item which may be apart of the trigger
        void OnTriggerEventJoin(Habbo* p_Habbo, std::shared_ptr<Room> p_Room, Item* p_Item) 
        {
            /// We shoulnd't be able to enter the pool if we are swimming
            if (p_Habbo->GetRoom()->HasStatus(p_Habbo->GetRoomGUID(), Status::STATUS_WALKING))
                return;

            /// We can't enter the pool if we don't have a swiming suit on
            if (p_Habbo->GetPoolFigure().empty())
                return;

            p_Habbo->GetRoom()->AddStatus(p_Habbo->GetRoomGUID(), Status::STATUS_SWIMMING);

            if (p_Habbo->GetPositionX() == 20 && p_Habbo->GetPositionY() == 28)
                p_Habbo->TeleportTo(22, 28, 3, p_Habbo->GetBodyRotation());

            else if (p_Habbo->GetPositionX() == 17 && p_Habbo->GetPositionY() == 21)
                p_Habbo->TeleportTo(16, 23, 3, p_Habbo->GetBodyRotation());

            else if (p_Habbo->GetPositionX() == 31 && p_Habbo->GetPositionY() == 10)
                p_Habbo->TeleportTo(30, 12, 3, p_Habbo->GetBodyRotation());

            else if (p_Habbo->GetPositionX() == 11 && p_Habbo->GetPositionY() == 11)
                p_Habbo->TeleportTo(13, 12, 3, p_Habbo->GetBodyRotation());
        }


        /// OnTriggerEventJoin
        /// @p_Habbo : Habbo user which is leaving the event
        /// @p_Room : Habbo user which is inside the room
        /// @p_Item : Item which may be apart of the trigger
        void OnTriggerEventLeave(Habbo* p_Habbo, std::shared_ptr<Room> p_Room, Item* p_Item) {}
    };
}