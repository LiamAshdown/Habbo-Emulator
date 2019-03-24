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

#include "Hotel.h"
#include "Habbo.h"
#include "Database/QueryDatabase.h"

namespace SteerStone
{
    /// Static StopWorld
    volatile bool Hotel::s_StopWorld = false;
   
    /// Singleton Class
    Hotel* Hotel::instance()
    {
        static Hotel instance;
        return &instance;
    }

    /// FindHabbo
    /// @p_Id : Account Id of Habbo
    Habbo* Hotel::FindHabbo(uint32 const& p_Id) const
    {
        auto const& itr = m_Habbos.find(p_Id);
        if (itr != m_Habbos.end() && itr->second)
            return itr->second;

        return nullptr;
    }
   
    /// FindHabboByName
    /// @p_Name : Habbo Name
    Habbo* Hotel::FindHabboByName(std::string const& p_Name) const
    {
        auto itr = std::find_if(m_Habbos.begin(), m_Habbos.end(),
            [&p_Name](std::pair<uint32, Habbo*> const& player)
            {
                return player.second != nullptr && player.second->GetName() == p_Name;
            });

        if (itr != m_Habbos.end())
            return itr->second;
        else
            return nullptr;
    }
   
    /// AddHabbo
    /// @p_Player : Habbo Class
    void Hotel::AddHabbo(Habbo* p_Habbo)
    {
        auto const& itr = m_Habbos.find(p_Habbo->GetId());
        if (itr != m_Habbos.end())
            itr->second->Logout(LOGOUT_CONCURRENT);

        m_Habbos[p_Habbo->GetId()] = p_Habbo;
    }
   
    /// RemoveHabbo
    /// @p_Player : Habbo Class
    void Hotel::RemoveHabbo(Habbo* p_Habbo)
    {
        auto const& itr = m_Habbos.find(p_Habbo->GetId());
        if (itr != m_Habbos.end() && itr->second)
            itr->second->Logout();
    }

    /// StopWorld - Stop the world updating -> Closing down server
    bool Hotel::StopWorld()
    {
        return s_StopWorld;
    }

    /// UpdateWorld
    /// @p_Diff : Diff time of updating Hotel
    void Hotel::UpdateWorld(const uint32& diff)
    {
        for (auto& itr = m_Habbos.begin(); itr != m_Habbos.end();)
        {
            Habbo* player = itr->second;

            if (!player->Update(diff))
            {
                itr = m_Habbos.erase(itr);
                delete player;
            }
            else
                ++itr;
        }
    }
   
    /// Clean Up - Clean up the objects in the hotel before closing down server
    void Hotel::CleanUp()
    {
        for (auto& itr = m_Habbos.begin(); itr != m_Habbos.end();)
        {
            delete itr->second;
            itr = m_Habbos.erase(itr);
        }

        m_Habbos.clear();
    }
} ///< NAMESPACE STEERSTONE