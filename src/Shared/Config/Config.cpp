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

#include <fstream>
#include "Config.h"

namespace SteerStone
{
    /// Singleton Instance
    Config* Config::instance()
    {
        static Config instance;
        return &instance;
    }

    /// SetFile
    /// @p_File : File name
    bool Config::SetFile(std::string const& p_File)
    {
        m_FileName = p_File;

        return Reload();
    }

    /// Reload - Read our server.conf file
    bool Config::Reload()
    {
        std::ifstream l_In(m_FileName, std::ifstream::in);

        if (l_In.fail())
            return false;

        std::unordered_map<std::string, std::string> l_NewEntries;
        std::lock_guard<std::mutex> l_Guard(m_ConfigLock);

        do
        {
            std::string l_Line;
            std::getline(l_In, l_Line);

            boost::algorithm::trim_left(l_Line);

            if (!l_Line.length())
                continue;

            if (l_Line[0] == '#' || l_Line[0] == '[')
                continue;

            auto const& l_Equals = l_Line.find('=');
            if (l_Equals == std::string::npos)
                return false;

            auto const l_Entry = boost::algorithm::trim_copy(boost::algorithm::to_lower_copy(l_Line.substr(0, l_Equals)));
            auto const l_Value = boost::algorithm::trim_copy_if(boost::algorithm::trim_copy(l_Line.substr(l_Equals + 1)), boost::algorithm::is_any_of("\""));

            l_NewEntries[l_Entry] = l_Value;
        } while (l_In.good());

        m_Entries = std::move(l_NewEntries);

        return true;
    }

    /// IsSet - Check if l_Entry exists
    /// @p_File : File name
    bool Config::IsSet(std::string const& p_File) const
    {
        auto const l_NameLower = boost::algorithm::to_lower_copy(p_File);
        return m_Entries.find(l_NameLower) != m_Entries.cend();
    }

    /// GetStringDefault
    /// @p_Name : Name of l_Entry we are looking for
    /// @p_Default : If we cannot find p_Name then return the default string
    std::string Config::GetStringDefault(std::string const& p_Name, std::string const& p_Default) const
    {
        auto const l_NameLower = boost::algorithm::to_lower_copy(p_Name);

        auto const l_Entry = m_Entries.find(l_NameLower);

        return l_Entry == m_Entries.cend() ? p_Default : l_Entry->second;
    }

    /// GetBoolDefault
    /// @p_Name : Name of l_Entry we are looking for
    /// @p_Default : If we cannot find p_Name then return the default string
    bool Config::GetBoolDefault(std::string const& p_Name, bool const& p_Default) const
    {
        auto const l_Value = GetStringDefault(p_Name, p_Default ? "true" : "false");

        std::string l_ValueLower;
        std::transform(l_Value.cbegin(), l_Value.cend(), std::back_inserter(l_ValueLower), ::tolower);

        return l_ValueLower == "true" || l_ValueLower == "1" || l_ValueLower == "yes";
    }

    /// GetIntDefault
    /// @p_Name : Name of l_Entry we are looking for
    /// @p_Default : If we cannot find p_Name then return the default string
    int32 Config::GetIntDefault(std::string const& p_Name, int32 const& p_Default) const
    {
        auto const l_Value = GetStringDefault(p_Name, std::to_string(p_Default));

        return std::stoi(l_Value);
    }

    /// GetFloatDefault
    /// @p_Name : Name of l_Entry we are looking for
    /// @p_Default : If we cannot find p_Name then return the default string
    float Config::GetFloatDefault(std::string const& p_Name, float const& p_Default) const
    {
        auto const l_Value = GetStringDefault(p_Name, std::to_string(p_Default));

        return std::stof(l_Value);
    }

    /// GetFileName - Return file name
    std::string Config::GetFilename() const
    {
        return m_FileName;
    }
} ///< NAMESPACE STEERSTONE

