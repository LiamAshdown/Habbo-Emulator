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

        /// StopWorld - Stop the world updating -> Closing down server
        static bool StopWorld();

        /// UpdateWorld
        /// @p_Diff : Diff time of update Hotel
        void UpdateWorld(uint32 const& p_Diff);

        /// Clean Up - Clean up the objects in the hotel before closing down server
        void CleanUp();

    private:
        /// Variables
        HabboMap m_Habbos;                              ///< Container which holds Habbo Users in the hotel
        std::mutex m_Mutex;                              ///< Mutex
        static volatile bool s_StopWorld;
    };
}

#define sHotel SteerStone::Hotel::instance()