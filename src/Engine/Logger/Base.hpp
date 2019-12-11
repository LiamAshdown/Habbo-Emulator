/*
* Liam Ashdown
* HardCPP (Merydwin)
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
#include "Singleton/Singleton.hpp"
#include "Utility/UtiString.hpp"
#include "Utility/UtiSystem.hpp"
#include "Logger/LogFileAppender.hpp"

namespace SteerStone { namespace Core { namespace Logger { 

    /// Main entry point
    class Base
    {
        SINGLETON_P_D(Base);

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    public:
        /// Report a message
        /// @p_Type         : Log type
        /// @p_System       : Message sender
        /// @p_Function     : Function name of caller
        /// @p_FunctionLine : Function line of caller
        /// @p_Message      : Message to report
        /// @p_Args...      : Message arguments
        template<class... Args> void Report(LogType p_Type, const std::string& p_System, std::string_view const p_Function, int32 const p_FunctionLine, const std::string& p_Message, Args ...p_Args)
        {
            Report(p_Type, p_System, p_Function, p_FunctionLine, Utils::StringBuilder(p_Message, p_Args...));
        }
        /// Report a message
        /// @p_Type         : Log type
        /// @p_System       : Message sender
        /// @p_Function     : Function name of caller
        /// @p_FunctionLine : Function line of caller
        /// @p_Message      : Message to report
        void Report(LogType p_LogType, std::string const& p_System, std::string_view const p_Function, int32 const p_FunctionLine, std::string const& p_Message);
        /// Report assert message
        /// @p_System       : Message sender
        /// @p_Function     : Function name of caller
        /// @p_FunctionLine : Function line of caller
        /// @p_Message      : Message to report
        void ReportAssert(std::string const& p_System, std::string_view const p_Function, int32 const p_FunctionLine, std::string const& p_Message);

        /// Output Server Banner
        /// @p_ExtraInfo : Extra info to output
        void ShowBanner(void(*p_ExtraInfo)());

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

        /// Enable/Disable Timer output to console
        /// @p_Enabler  : Enable/Disable output to console
        void LogTimeEnabler(bool const p_Enabler);
        /// Enable/Disable Thread Id to console
        /// @p_Enabler  : Enable/Disable output to console
        void LogThreadIdEnabler(bool const p_Enabler);
        /// Enable/Disable Function output to console
        /// @p_Enabler  : Enable/Disable output to console
        void LogFunctionEnabler(bool const p_Enabler);

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    private:
        /// Set log mode
        /// @p_Level : New log level
        void SetLogMode(LogType p_Level);

        /// Get log mode
        LogType GetLogLevel();
        /// Return current time
        std::string GetServerTime();

    public:
        /// Add appender to the logger
        /// @p_Appender : New appender to add
        void AddAppender(Appender* p_Appender);
    private:
        /// Remove all appenders
        void RemoveAllAppenders();

    private:
        LogType                 m_LogLevel;             ///< Log Mode
        std::recursive_mutex    m_Mutex;                ///< Global Mutex
        std::vector<Appender*>  m_Appenders; ///< All appenders

        bool                    m_LogTime;              ///< Output Time
        bool                    m_LogThreadId;          ///< Output Thread Id
        bool                    m_LogFunction;          ///< Output Function Name
    };
} ///< SteerStone 
} ///< Core
} ///< Logger
