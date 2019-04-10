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

namespace SteerStone
{
    class Item;
    class Habbo;
    class Room;

    /// This executes the virtual triggers
    class BaseTriggerEvent
    {
    public:
        /// Constructor
        BaseTriggerEvent() {}

        /// Virtual Deconstructor
        virtual ~BaseTriggerEvent() {}

    public:

        /// OnTriggerEventJoin
        /// This executes the virtual trigger
        /// @p_Habbo : Habbo user which activated the event
        /// @p_Room : Habbo user which is inside the room
        /// @p_Item : Item which may be apart of the trigger
        virtual void OnTriggerEventJoin(Habbo* p_Habbo, std::shared_ptr<Room> p_Room, Item* p_Item) = 0;

        /// OnTriggerEventJoin
        /// This executes the virtual trigger
        /// @p_Habbo : Habbo user which is leaving the event
        /// @p_Room : Habbo user which is inside the room
        /// @p_Item : Item which may be apart of the trigger
        virtual void OnTriggerEventLeave(Habbo* p_Habbo, std::shared_ptr<Room> p_Room, Item* p_Item) = 0;
    };
} ///< NAMESPACE STEERSTONE