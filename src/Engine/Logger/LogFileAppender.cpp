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

#include <Precompiled.hpp>

#include "LogFileAppender.hpp"
#include "Base.hpp"

namespace SteerStone { namespace Core { namespace Logger {

    /// Constructor
    /// @p_FileName : Output file name
    /// @p_HTML     : Should output in HTML syntax
    FileAppender::FileAppender(const std::string & p_FileName, bool p_HTML)
        : m_File(nullptr), m_HTML(p_HTML)
    {
        std::lock_guard<std::recursive_mutex> l_Lock(m_Mutex);

        m_File = fopen(p_FileName.c_str(), "a");

        if (!m_File)
        {
            LOG_ERROR("LogFileAppender", "Can't open file \"%0\" in append mode", p_FileName);
            exit(-1);
            return;
        }

        if (m_HTML)
        {
            std::string l_ToWrite = "<html><head><title>Log</title></head><body>";
            l_ToWrite += "<center><h1>Log</h1></center>";
            l_ToWrite += "<br/><br/><br/>";
            l_ToWrite += R"TPL(<table><tr valign="top"><td>Time<td/><td>Level<td/><td>System<td/><td>Message<td/></tr>)TPL";

            fwrite(l_ToWrite.c_str(), l_ToWrite.length(), 1, m_File);
        }

        LOG_INFO("LogFileAppender", "Created...");
    }
    /// Destructor
    FileAppender::~FileAppender()
    {
        std::lock_guard<std::recursive_mutex> l_Lock(m_Mutex);

        if (m_File)
        {
            LOG_INFO("LogFileAppender", "Closing...");

            if (m_HTML)
            {
                std::string l_ToWrite = "</table></body></html>";
                fwrite(l_ToWrite.c_str(), l_ToWrite.length(), 1, m_File);
            }

            fflush(m_File);
            fclose(m_File);

            m_File = nullptr;

            LOG_INFO("LogFileAppender", "Closed...");
        }
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// On report a message
    /// @p_Logger   : Logger instance
    /// @p_Type     : Log type
    /// @p_System   : Message sender
    /// @p_Message  : Message to report
    void FileAppender::OnReport(Base * p_Logger, const std::string & p_Time, LogType p_Type, const std::string & p_System, const std::string & p_Message)
    {
        std::string l_TypeStr   = "";
        std::string l_HTMLColor = "";

        switch (p_Type)
        {
            case LogType::Info:     l_TypeStr = "Info";     l_HTMLColor = "green";  break;
            case LogType::Warning:  l_TypeStr = "Warning";  l_HTMLColor = "blue";   break;
            case LogType::Error:    l_TypeStr = "Error";    l_HTMLColor = "red";    break;
            case LogType::Assert:   l_TypeStr = "Assert";   l_HTMLColor = "red";  break;
            case LogType::Verbose:  l_TypeStr = "Verbose";  l_HTMLColor = "black";  break;
        }

        std::string l_ToWrite = "";

        if (m_HTML)
        {
            static const char * sl_Template = R"TPL(<tr valign="top"><td>%0</td><td></td><td style="color:%1">%2</td><td></td><td>%3</td><td></td><td style="color:%1">%4</td>)TPL";
            l_ToWrite = Utils::StringBuilder<std::string>(sl_Template, p_Time, l_HTMLColor, l_TypeStr, p_System, p_Message);
        }
        else
        {
            static const char * sl_Template = R"TPL([%0] %1 "%2" > %3\n)TPL";
            l_ToWrite = Utils::StringBuilder<std::string>(sl_Template, p_Time, l_TypeStr, p_System, p_Message);
        }

        std::lock_guard<std::recursive_mutex> l_Lock(m_Mutex);

        if (!m_File)
            return;

        fwrite(l_ToWrite.c_str(), l_ToWrite.length(), 1, m_File);
        fflush(m_File);
    }

}   ///< namespace Logger
}   ///< namespace Core
}   ///< namespace SteerStone