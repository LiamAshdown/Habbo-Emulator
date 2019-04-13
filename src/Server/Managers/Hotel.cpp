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
#include "RoomManager.h"
#include "Network/StringBuffer.h"

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

    /// Load Configs from .conf file
    void Hotel::LoadConfigs()
    {
        m_BoolConfigs[BoolConfigs::CONFIG_REGISTERATION_HABBO_CLOTHING]      = sConfig->GetBoolDefault("RegisterationHabboClothing", 0);
        m_BoolConfigs[BoolConfigs::CONFIG_REGISTERATION_SOUND]               = sConfig->GetBoolDefault("RegisterationSound", 0);
        m_BoolConfigs[BoolConfigs::CONFIG_PARAMETER_COPPA]                   = sConfig->GetBoolDefault("ParameterCoppa", 0);
        m_BoolConfigs[BoolConfigs::CONFIG_PARAMETER_VOUCHER]                 = sConfig->GetBoolDefault("ParameterVoucher", 1);
        m_BoolConfigs[BoolConfigs::CONFIG_PARAMETER_REQUIRE_PARENT_EMAIL]    = sConfig->GetBoolDefault("ParamterRequireParentEmail", 0);
        m_BoolConfigs[BoolConfigs::CONFIG_PARAMETER_SEND_PARENT_EMAIL]       = sConfig->GetBoolDefault("ParamterSendParentEmail", 0);
        m_BoolConfigs[BoolConfigs::CONFIG_PARAMETER_DIRECT_MAIL]             = sConfig->GetBoolDefault("ParamterDirectMail", 0);
        m_BoolConfigs[BoolConfigs::CONFIG_PARAMETER_INTEGRATION]             = sConfig->GetBoolDefault("ParamterIntegration", 0);
        m_BoolConfigs[BoolConfigs::CONFIG_PARAMETER_PROFILE_EDITING]         = sConfig->GetBoolDefault("ParameterProfileEditing", 1);
        m_BoolConfigs[BoolConfigs::CONFIG_PARAMETER_TRACKING_HEAD]           = sConfig->GetBoolDefault("ParameterTrackingHeader", 0);
        m_BoolConfigs[BoolConfigs::CONFIG_PARAMETER_TUTORIAL]                = sConfig->GetBoolDefault("ParameterTutorial", 1);

        m_IntConfigs[IntConfigs::CONFIG_SERVER_PORT]                         = sConfig->GetIntDefault("ServerPort", DEFAULT_SERVER_PORT);
        m_IntConfigs[IntConfigs::CONFIG_NETWORK_PROCESSORS]                  = sConfig->GetIntDefault("NetworkThreadProcessors", 1);
        m_IntConfigs[IntConfigs::CONFIG_PONG_INTERVAL]                       = sConfig->GetIntDefault("PongInterval", 30000);
        m_IntConfigs[IntConfigs::CONFIG_REGISTERATION_CREDITS]               = sConfig->GetIntDefault("RegisterationCredits", 0);
        m_IntConfigs[IntConfigs::CONFIG_REGISTERATION_TICKETS]               = sConfig->GetIntDefault("RegisterationTickets", 0);
        m_IntConfigs[IntConfigs::CONFIG_REGISTERATION_FILMS]                 = sConfig->GetIntDefault("RegisterationFilms", 0);
        m_IntConfigs[IntConfigs::CONFIG_MESSENGER_MAX_FRIENDS]               = sConfig->GetIntDefault("MessengerMaxFriendsLimit", 50);
        m_IntConfigs[IntConfigs::CONFIG_MESSENGER_MAX_CLUB_FRIENDS]          = sConfig->GetIntDefault("MessgengerMaxFriendsClubLimit", 100);
        m_IntConfigs[IntConfigs::CONFIG_AFK_TIMER]                           = sConfig->GetIntDefault("WaveTimer", 4000);
        m_IntConfigs[IntConfigs::CONFIG_WAVE_TIMER]                          = sConfig->GetIntDefault("AwayFromKeyboardTimer", 900000);
    }

    /// GetBoolConfig
    /// Returns bool value
    bool Hotel::GetBoolConfig(BoolConfigs p_Index)
    {
        return p_Index < BOOL_CONFIG_MAX ? m_BoolConfigs[p_Index] : 0;
    }

    /// GetBoolConfig
   /// Returns integer value
    uint32 Hotel::GetIntConfig(IntConfigs p_Index)
    {
        return p_Index < INT_CONFIG_MAX ? m_IntConfigs[p_Index] : 0;
    }

    /// GetBoolConfig
    /// Returns float value
    float Hotel::GetFloatConfig(FloatConfigs p_Index)
    {
        return p_Index < FLOAT_CONFIG_MAX ? m_FloatConfigs[p_Index] : 0;
    }

    /// StopWorld - Stop the world updating -> Closing down server
    bool Hotel::StopWorld()
    {
        return s_StopWorld;
    }

    /// UpdateWorld
    /// @p_Diff : Diff time of updating Hotel
    void Hotel::UpdateWorld(uint32 const& p_Diff)
    {
        for (auto& itr = m_Habbos.begin(); itr != m_Habbos.end();)
        {
            Habbo* player = itr->second;

            if (!player->Update(p_Diff))
            {
                itr = m_Habbos.erase(itr);
                delete player;
            }
            else
                ++itr;
        }

        /// Update all of our rooms
        sRoomMgr->UpdateRooms(p_Diff);
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