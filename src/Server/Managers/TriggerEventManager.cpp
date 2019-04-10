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

#include "TriggerEventManager.h"

#include "Events/TriggerEventSit.h"
#include "Events/TriggerEventPoolBooth.h"
#include "Events/TriggerEventDefault.h"
#include "Events/TriggerEventEnterPool.h"
#include "Events/TriggerEventExitPool.h"

namespace SteerStone
{
    /// Singleton
    TriggerEventManager* TriggerEventManager::instance()
    {
        static TriggerEventManager l_Instance;
        return &l_Instance;
    }

    /// Constructor
    TriggerEventManager::TriggerEventManager()
    {
    }

    /// Deconstructor
    /// Clear our trigger storage
    TriggerEventManager::~TriggerEventManager()
    {
        for (auto l_Itr : m_Triggers)
            delete l_Itr.second;

        m_Triggers.clear();
    }

    /// LoadTriggerEvents
    /// Load the triggers into the map storage
    void TriggerEventManager::LoadTriggerEvents()
    {
        m_Triggers["default"]        = new TriggerEventDefault;
        m_Triggers["can_sit_on_top"] = new TriggerEventSit;
        m_Triggers["pool_booth"]     = new TriggerEventPoolBooth;
        m_Triggers["pool_enter"]     = new TriggerEventEnterPool;
        m_Triggers["pool_exit"]      = new TriggerEventExitPool;

        LOG_INFO << "Loaded " << m_Triggers.size() << " Trigger Events";
    }

    /// GetTrigger
    /// Returns trigger
    BaseTriggerEvent* TriggerEventManager::GetTrigger(std::string const p_Trigger) const
    {
        auto& l_Itr = m_Triggers.find(p_Trigger);
        if (l_Itr != m_Triggers.end())
            return l_Itr->second;
        else
            return m_Triggers.begin()->second; ///< Returns default trigger if none specified
    }
} ///< NAMESPACE STEERSTONE