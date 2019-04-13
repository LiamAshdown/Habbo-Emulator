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

#ifndef _MANAGERS_HOTEL_h
#define _MANAGERS_HOTEL_h
#include "Common/SharedDefines.h"
#include "Common/Timer.h"
#include "Config/Config.h"
#include "Database/QueryDatabase.h"
#include "Platform/Thread/ThreadPool.h"
#include <mutex>
#endif /* _MANAGERS_HOTEL_h */

namespace SteerStone
{
    /// Holds bool configs values
    enum BoolConfigs
    {
        CONFIG_REGISTERATION_HABBO_CLOTHING,
        CONFIG_REGISTERATION_SOUND,
        CONFIG_REGISTERATION_REQUIRE_PARENT_EMAIL,
        CONFIG_PARAMETER_COPPA,
        CONFIG_PARAMETER_VOUCHER,
        CONFIG_PARAMETER_REQUIRE_PARENT_EMAIL,
        CONFIG_PARAMETER_SEND_PARENT_EMAIL,
        CONFIG_PARAMETER_DIRECT_MAIL,
        CONFIG_PARAMETER_INTEGRATION,
        CONFIG_PARAMETER_PROFILE_EDITING,
        CONFIG_PARAMETER_TRACKING_HEAD,
        CONFIG_PARAMETER_TUTORIAL,
        BOOL_CONFIG_MAX
    };

    /// Holds int configs values
    enum IntConfigs
    {
        CONFIG_SERVER_PORT,
        CONFIG_NETWORK_PROCESSORS,
        CONFIG_PONG_INTERVAL,
        CONFIG_REGISTERATION_CREDITS,
        CONFIG_REGISTERATION_TICKETS,
        CONFIG_REGISTERATION_FILMS,
        CONFIG_MESSENGER_MAX_FRIENDS,
        CONFIG_MESSENGER_MAX_CLUB_FRIENDS,
        CONFIG_AFK_TIMER,
        CONFIG_WAVE_TIMER,
        INT_CONFIG_MAX
    };

    /// Holds float configs values
    enum FloatConfigs
    {
        FLOAT_CONFIG_MAX
    };

    class Habbo;

    typedef std::unordered_map<uint32, Habbo*> HabboMap;

    /// Class which runs the hotel
    /// Singleton Class
    class Hotel
    {
    public:
        static Hotel* instance();

    public:
        /// Constructor
        Hotel() {}
        /// Deconstructor
        ~Hotel() {}

    public:
        /// FindHabbo
        /// @p_Id : Account Id of Habbo
        Habbo* FindHabbo(uint32 const& p_Id) const;

        /// FindHabboByName
        /// @p_Name : Habbo Name
        Habbo* FindHabboByName(std::string const& p_Name) const;

        /// AddHabbo
        /// @p_Player : Habbo Class
        void AddHabbo(Habbo* p_Habbo);

        /// FindHabbo
        /// @p_Player : Habbo Class
        void RemoveHabbo(Habbo* p_Habbo);

        /// Load Configs from .conf file
        void LoadConfigs();

        /// GetBoolConfig
        /// Returns bool value
        bool GetBoolConfig(BoolConfigs p_Index);

        /// GetBoolConfig
        /// Returns integer value
        uint32 GetIntConfig(IntConfigs p_Index);

        /// GetBoolConfig
        /// Returns float value
        float GetFloatConfig(FloatConfigs p_Index);

        /// StopWorld - Stop the world updating -> Closing down server
        static bool StopWorld();

        /// UpdateWorld
        /// @p_Diff : Diff time of update Hotel
        void UpdateWorld(uint32 const& p_Diff);

        /// Clean Up - Clean up the objects in the hotel before closing down server
        void CleanUp();

    private:
        /// Variables
        HabboMap m_Habbos;                               ///< Container which holds Habbo Users in the hotel
        std::mutex m_Mutex;                              ///< Mutex
        uint32 m_IntConfigs[INT_CONFIG_MAX];             ///< Holds config settings for int
        bool m_BoolConfigs[BOOL_CONFIG_MAX];             ///< Holds config settings for bool
        float m_FloatConfigs[FLOAT_CONFIG_MAX];           ///< Holds config settings for float
        static volatile bool s_StopWorld;
    };
}

#define sHotel SteerStone::Hotel::instance()