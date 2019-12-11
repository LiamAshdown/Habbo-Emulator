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
#include <boost/algorithm/string.hpp>

#include "Config.hpp"
#include "Base.hpp"

namespace SteerStone { namespace Core { namespace Configuration {

    SINGLETON_P_I(Base);

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    Base::Base()
    {
    }
    Base::~Base()
    {
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Load File
    /// @p_File : File Name
    bool Base::SetFile(std::string const& p_File)
    {
        m_FileName = p_File;
        
        return Reload();
    }
    /// Returns file name
    std::string Base::GetFilename() const
    {
        return m_FileName;
    }
    /// Reload File
    bool Base::Reload()
    {
        Utils::ObjectWriteGuard<Base> l_Guard(this);

        /// Reset
        m_Entries.clear();

        std::ifstream l_Stream(m_FileName, std::ifstream::in);
        
        if (l_Stream.fail())
        {
            LOG_ERROR("Configuration", "Failed to load file %0: %1", m_FileName, strerror(errno));
            return false;
        }

        ConfigMap l_NewEntries;

        do
        {
            std::string l_Line;
            std::getline(l_Stream, l_Line);

            boost::algorithm::trim_left(l_Line);

            if (!l_Line.length())
                continue;

            if (l_Line[0] == '#' || l_Line[0] == '[')
                continue;

            auto const& l_Equals = l_Line.find('=');
            if (l_Equals == std::string::npos)
            {
                LOG_INFO("Configuration", "Reached end of line of configuration file!");
                return false;
            }

            auto const l_Entry = boost::algorithm::trim_copy(boost::algorithm::to_lower_copy(l_Line.substr(0, l_Equals)));
            auto const l_Value = boost::algorithm::trim_copy_if(boost::algorithm::trim_copy(l_Line.substr(l_Equals + 1)), boost::algorithm::is_any_of("\""));

            l_NewEntries[l_Entry] = l_Value;
        } while (l_Stream.good());

        m_Entries = std::move(l_NewEntries);

        return true;
    }
    /// Check if entry exists
    /// @p_File : File name
    bool Base::IsSet(std::string const& p_File)
    {
        Utils::ObjectReadGuard<Base> l_Guard(this);

        auto const l_NameLower = boost::algorithm::to_lower_copy(p_File);
        return m_Entries.find(l_NameLower) != m_Entries.cend();
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Get String configuration
    /// @p_Name     :  Name of entry we are looking for
    /// @p_Default  :  Default value
    std::string Base::GetString(std::string const& p_Name, std::string const& p_Default /*= std::string()*/)
    {
        Utils::ObjectReadGuard<Base> l_Guard(this);

        auto const l_NameLower = boost::algorithm::to_lower_copy(p_Name);

        auto const l_Entry = m_Entries.find(l_NameLower);

        return l_Entry == m_Entries.cend() ? p_Default : l_Entry->second;
    }
    /// Get Bool configuration
    /// @p_Name     :  Name of entry we are looking for
    /// @p_Default  :  Default value
    bool Base::GetBool(std::string const& p_Name, bool const& p_Default)
    {
        auto const l_Value = GetString(p_Name, p_Default ? "true" : "false");

        std::string l_ValueLower;
        std::transform(l_Value.cbegin(), l_Value.cend(), std::back_inserter(l_ValueLower), ::tolower);

        return l_ValueLower == "true" || l_ValueLower == "1" || l_ValueLower == "yes";
    }
    /// Get Int configuration
    /// @p_Name     :  Name of entry we are looking for
    /// @p_Default  :  Default value
    int32 Base::GetInt(std::string const& p_Name, int32 const& p_Default)
    {
        auto const l_Value = GetString(p_Name, std::to_string(p_Default));

        return std::stoi(l_Value);
    }
    /// Get Float configuration
    /// @p_Name     :  Name of entry we are looking for
    /// @p_Default  :  Default value
    float Base::GetFloat(std::string const& p_Name, float const& p_Default)
    {
        auto const l_Value = GetString(p_Name, std::to_string(p_Default));

        return std::stof(l_Value);
    }

} ///< Configuration
} ///< Core
} ///< SteerStone