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

#include <Precompiled.hpp>

#include "Base.hpp"

#ifdef _WIN32
#   include <windows.h>
#   define COLOR_BASE  FOREGROUND_GREEN | FOREGROUND_RED        | FOREGROUND_BLUE   | FOREGROUND_INTENSITY
#   define COLOR_GREEN FOREGROUND_GREEN                                             | FOREGROUND_INTENSITY
#   define COLOR_BLUE  FOREGROUND_BLUE                                              | FOREGROUND_INTENSITY
#   define COLOR_RED   FOREGROUND_RED                                               | FOREGROUND_INTENSITY
#   define COLOR_RGB   FOREGROUND_RED   | FOREGROUND_GREEN      | FOREGROUND_BLUE
#   define SET_COLOR(p_Color) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), p_Color);
#else
#   define COLOR_BASE   39
#   define COLOR_GREEN  32
#   define COLOR_BLUE   34
#   define COLOR_RED    31
#   define COLOR_RGB    39
#   define SET_COLOR(p_Color) std::cout << "\033[" << p_Color << "m" << std::flush;
#endif
#define GET_COLOR(Mode) ((Mode) == LogType::Assert ? COLOR_RED : (Mode) == LogType::Error ? COLOR_RED : ((Mode) == LogType::Warning ? COLOR_BLUE : ((Mode) == LogType::Info ? COLOR_GREEN : COLOR_BASE)))

namespace SteerStone  { namespace Core { namespace Logger {

    SINGLETON_P_I(Base)
    
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    Base::Base() 
        : m_LogLevel(LogType::Verbose), m_LogTime(true), m_LogThreadId(true), m_LogFunction(true)
    {
    }

    /// Deconstructor
    Base::~Base() 
    {
        RemoveAllAppenders();
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Report a message
    /// @p_Type         : Log type
    /// @p_System       : Message sender
    /// @p_Function     : Function name of caller
    /// @p_FunctionLine : Function line of caller
    /// @p_Message      : Message to report
    void Base::Report(LogType p_LogType, std::string const& p_System, std::string_view const p_Function, int32 const p_FunctionLine, std::string const& p_Message)
    {
        if (p_LogType > m_LogLevel)
            return;

        std::lock_guard<std::recursive_mutex> l_Lock(m_Mutex);

        if (p_LogType == LogType::Assert)
        {
            ReportAssert(p_System, p_Function, p_FunctionLine, p_Message);
            return;
        }

        /// Constant always true variables
        const std::string l_Time      = GetServerTime();
        const std::string l_System    = "[" + p_System + "]";
        const std::string l_Message   = " << " + p_Message;
        const std::string l_LogLevel  = (p_LogType == LogType::Verbose ? "[VERBOSE]" : (p_LogType == LogType::Error) ? "[ERROR]"
                                       : p_LogType == LogType::Warning ? "[WARNING]" : "[INFO]");

        /// Message Output
        {
            SET_COLOR(COLOR_BASE);

            if (m_LogTime)
                std::clog << l_Time + " | ";

            if (m_LogThreadId)
                std::clog << "Thread Id: " << Utils::GetThreadId() << " | ";

            if (m_LogFunction)
                std::clog << p_Function << "::" << p_FunctionLine << " | ";

            SET_COLOR(GET_COLOR(p_LogType));
            std::clog << l_LogLevel;
            std::clog << l_System;
            std::clog << l_Message << std::endl;
            SET_COLOR(COLOR_RGB);
        }

        for (auto l_Appender : m_Appenders)
            l_Appender->OnReport(this, l_Time, p_LogType, p_System, p_Message);
    }
    /// Report assert message
    /// @p_System       : Message sender
    /// @p_Function     : Function name of caller
    /// @p_FunctionLine : Function line of caller
    /// @p_Message      : Message to report
    void Base::ReportAssert(std::string const& p_System, std::string_view const p_Function, int32 const p_FunctionLine, std::string const& p_Message)
    {
        /// Constant always true variables
        const std::string l_Time = GetServerTime();
        const std::string l_System = "[" + p_System + "]";
        const std::string l_Message = " << " + p_Message;

        /// Message Output
        {
            SET_COLOR(COLOR_BASE);

            if (m_LogTime)
                std::clog << l_Time + " | ";

            if (m_LogThreadId)
                std::clog << "Thread Id: " << Utils::GetThreadId() << " | ";

            if (m_LogFunction)
                std::clog << p_Function << "::" << p_FunctionLine << " | ";

            SET_COLOR(GET_COLOR(LogType::Assert));
            std::clog << "[ASSERT]";
            std::clog << l_System;
            std::clog << l_Message << std::endl;
            SET_COLOR(COLOR_RGB);
        }

        /// Crash
        *((volatile int*)nullptr) = 0;
        exit(1);
    }

    /// Output Server Banner
    /// @p_ExtraInfo : Extra info to output
    void Base::ShowBanner(void(*p_ExtraInfo)())
    {
        std::clog << " ______     ______   ______     ______     ______     ______     ______   ______     __   __     ______ " << std::endl;
        std::clog << "/\\  ___\\   /\\__  _\\ /\\  ___\\   /\\  ___\\   /\\  == \\   /\\  ___\\   /\\__  _\\ /\\  __ \\   /\\ ' - .\\ \ / \\  ___\\     " << std::endl;
        std::clog << "\\ \\___  \\  \\/_/\\ \\/ \\ \\  __\\   \\ \\  __\\   \\ \\  __<   \\ \\___  \\  \\/_/\\ \\/ \\ \\ \\/\\ \\  \\ \\ \\-.  \\  \\ \\  __\\     " << std::endl;
        std::clog << " \\/\\_____\\    \\ \\_\\  \\ \\_____\\  \\ \\_____\\  \\ \\_\\ \\_\\  \\/\\_____\\    \\ \\_\\  \\ \\_____\\  \\ \\_\\\\'\\_\\  \\ \\_____\\   " << std::endl;
        std::clog << "  \\/_____/     \\/_/   \\/_____/   \\/_____/   \\/_/ /_/   \\/_____/     \\/_/   \\/_____/   \\/_/ \\/_/   \\/_____/    " << std::endl;
        std::clog << "                                                                                  Powered by Boost & MySQL" << std::endl;

        if (p_ExtraInfo)
            p_ExtraInfo();
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Enable/Disable Timer output to console
    /// @p_Enabler  : Enable/Disable output to console
    void Base::LogTimeEnabler(bool const p_Enabler)
    {
        m_LogTime = p_Enabler;
    }
    /// Enable/Disable Thread Id to console
    /// @p_Enabler  : Enable/Disable output to console
    void Base::LogThreadIdEnabler(bool const p_Enabler)
    {
        m_LogThreadId = p_Enabler;
    }
    /// Enable/Disable Function output to console
    /// @p_Enabler  : Enable/Disable output to console
    void Base::LogFunctionEnabler(bool const p_Enabler)
    {
        m_LogFunction = p_Enabler;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Set log mode
    /// @p_Level : New log level
    void Base::SetLogMode(LogType p_Level)
    {
        std::lock_guard<std::recursive_mutex> l_Lock(m_Mutex);
        m_LogLevel = p_Level;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Get log mode
    LogType Base::GetLogLevel()
    {
        return m_LogLevel;
    }
    /// Return current time
    std::string Base::GetServerTime()
    {
        time_t l_RawTime;
        struct tm* l_TimeInfo;

        time(&l_RawTime);
        l_TimeInfo = localtime(&l_RawTime);

        char l_Time[14];
        sprintf_s(l_Time, "%02d:%02d:%02d", l_TimeInfo->tm_hour, l_TimeInfo->tm_min, l_TimeInfo->tm_sec);

        return l_Time;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////


    /// Add appender to the logger
    /// @p_Appender : New appender to add
    void Base::AddAppender(Appender* p_Appender)
    {
        std::lock_guard<std::recursive_mutex> l_Lock(m_Mutex);
        m_Appenders.push_back(p_Appender);
    }
    /// Remove all appenders
    void Base::RemoveAllAppenders()
    {
        std::lock_guard<std::recursive_mutex> l_Lock(m_Mutex);

        for (auto l_Appender : m_Appenders)
            delete l_Appender;

        m_Appenders.clear();
    }

} ///< Logger
} ///< COre
} ///< SteerStone