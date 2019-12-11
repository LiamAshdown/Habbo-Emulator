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
#include <PCH/Precompiled.hpp>
#include <mutex>

#include "Core/Core.hpp"
#include "LogAppender.hpp"

namespace SteerStone { namespace Core { namespace Logger {

    /// File appender class
    class FileAppender : public Appender
    {
        DISALLOW_COPY_AND_ASSIGN(FileAppender);

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    public:
        /// Constructor
        /// @p_FileName : Output file name
        /// @p_HTML     : Should output in HTML syntax
        FileAppender(const std::string & p_FileName, bool p_HTML = true);
        /// Destructor
        ~FileAppender();

        /// On report a message
        /// @p_Logger   : Logger instance
        /// @p_Type     : Log type
        /// @p_System   : Message sender
        /// @p_Message  : Message to report
        void OnReport(Base * p_Logger, const std::string & p_Time, LogType p_Type, const std::string & p_System, const std::string & p_Message) override final;

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    private:
        FILE * m_File;                ///< Output file
        bool   m_HTML;                ///< Should output in HTML

        std::recursive_mutex m_Mutex; ///< File mutex
    };

}   ///< namespace Logger
}   ///< namespace Core
}   ///< namespace SteerStone