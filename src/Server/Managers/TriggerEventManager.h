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

#ifndef _MANAGERS_TRIGGER_EVENT_MANAGER_h
#define _MANAGERS_TRIGGER_EVENT_MANAGER_h
#include "Common/SharedDefines.h"
#include "BaseTriggerEvent.h"
#endif /* _MANAGERS_TRIGGER_EVENT_MANAGER_h */

namespace SteerStone
{
    typedef std::unordered_map<std::string, BaseTriggerEvent*> TriggerMap;

    /// Stores Trigger Event
   class TriggerEventManager
   {
   public:
       /// Singleton
       static TriggerEventManager* instance();

   public:
       /// Constructor
       TriggerEventManager();

       /// Deconstructor
       /// Clear our trigger storage
       ~TriggerEventManager();

   public:
       /// LoadTriggerEvents
       /// Load the triggers into the map storage
       void LoadTriggerEvents();

       /// GetTrigger
       /// Returns trigger
       BaseTriggerEvent* GetTrigger(std::string const p_Trigger) const;

   private:
       TriggerMap m_Triggers;           ///< Stores trigger events
   };
} ///< NAMESPACE STEERSTONE

#define sTriggerMgr SteerStone::TriggerEventManager::instance()