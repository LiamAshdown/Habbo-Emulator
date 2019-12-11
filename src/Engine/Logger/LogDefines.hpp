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

/// Console Output
#ifdef _DEBUG
#define LOG_INFO(p_System,      ...)        ::SteerStone::Core::Logger::Base::GetSingleton()->Report(SteerStone::Core::Logger::LogType::Info,    p_System, LOG_GET_FUNCTION(), LOG_GET_FUNCTION_LINE(), __VA_ARGS__)
#define LOG_WARNING(p_System,   ...)        ::SteerStone::Core::Logger::Base::GetSingleton()->Report(SteerStone::Core::Logger::LogType::Warning, p_System, LOG_GET_FUNCTION(), LOG_GET_FUNCTION_LINE(), __VA_ARGS__)
#define LOG_ERROR(p_System,     ...)        ::SteerStone::Core::Logger::Base::GetSingleton()->Report(SteerStone::Core::Logger::LogType::Error,   p_System, LOG_GET_FUNCTION(), LOG_GET_FUNCTION_LINE(), __VA_ARGS__)
#define LOG_VERBOSE(p_System,   ...)        ::SteerStone::Core::Logger::Base::GetSingleton()->Report(SteerStone::Core::Logger::LogType::Verbose, p_System, LOG_GET_FUNCTION(), LOG_GET_FUNCTION_LINE(), __VA_ARGS__)
#define LOG_ASSERT(p_Condition, p_System, ...) if (!(p_Condition)) ::SteerStone::Core::Logger::Base::GetSingleton()->Report(SteerStone::Core::Logger::LogType::Assert, p_System, LOG_GET_FUNCTION(), LOG_GET_FUNCTION_LINE(), __VA_ARGS__)
#else
#define LOG_INFO(p_System,      ...)        ::SteerStone::Core::Logger::Base::GetSingleton()->Report(SteerStone::Core::Logger::LogType::Info,    p_System, LOG_GET_FUNCTION(), LOG_GET_FUNCTION_LINE(), __VA_ARGS__)
#define LOG_WARNING(p_System,   ...)        ::SteerStone::Core::Logger::Base::GetSingleton()->Report(SteerStone::Core::Logger::LogType::Warning, p_System, LOG_GET_FUNCTION(), LOG_GET_FUNCTION_LINE(), __VA_ARGS__)
#define LOG_ERROR(p_System,     ...)        ::SteerStone::Core::Logger::Base::GetSingleton()->Report(SteerStone::Core::Logger::LogType::Error,   p_System, LOG_GET_FUNCTION(), LOG_GET_FUNCTION_LINE(), __VA_ARGS__)
#define LOG_VERBOSE(p_System,   ...)
#define LOG_ASSERT(p_Condition, p_System, ...) if (!(p_Condition)) ::SteerStone::Core::Logger::Base::GetSingleton()->Report(SteerStone::Core::Logger::LogType::Assert, p_System, LOG_GET_FUNCTION(), LOG_GET_FUNCTION_LINE(), __VA_ARGS__)
#endif

/// Logger Configuration
#define LOG_ENABLE_TIME(p_Enable)           ::SteerStone::Core::Logger::Base::GetSingleton()->LogTimeEnabler(p_Enable)
#define LOG_ENABLE_THREAD_ID(p_Enable)      ::SteerStone::Core::Logger::Base::GetSingleton()->LogThreadIdEnabler(p_Enable)
#define LOG_ENABLE_FUNCTION(p_Enable)       ::SteerStone::Core::Logger::Base::GetSingleton()->LogFunctionEnabler(p_Enable)
#define LOG_ADD_APPENDER(p_Appender)        ::SteerStone::Core::Logger::Base::GetSingleton()->AddAppender(p_Appender)

/// Internal Logger Helpers
#ifdef _MSC_VER
    #define LOG_GET_FUNCTION()      __FUNCTION__
#else
    #define LOG_GET_FUNCTION()      __PRETTY_FUNCTION__
#endif                

#define LOG_GET_FUNCTION_LINE()     __LINE__

namespace SteerStone { namespace Core { namespace Logger {

    /// Log type
    enum class LogType
    {
        Info,
        Warning,
        Error,
        Assert,
        Verbose
    };

} ///< namespace Logger
} ///< namespace Core
} ///< namespace SteerStone