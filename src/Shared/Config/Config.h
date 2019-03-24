/*
* Liam Ashdown
* Copyright(C) 2019
*
* This program is free software : you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
*(at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
*but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.If not, see < http://www.gnu.org/licenses/>.
*/

#ifndef _CONFIG_CONFIG_h
#define _CONFIG_CONFIG_h
#include "../Common/SharedDefines.h"
#include <mutex>
#endif /* _CONFIG_CONFIG_h */

namespace SteerStone
{
    /// Class which retrieves information from our server.conf file
    /// Singleton Class 
    class Config
    {
    public:
        static Config* instance();

    public:
        /// Constructor
        Config() {}
        /// Deconstructor
        ~Config() {}

    public:
        /// SetFile
        /// @p_File : File name
        bool SetFile(std::string const& p_File);

        /// Reload - Read our server.conf file
        bool Reload();

        /// IsSet - Check if entry exists
        /// @p_File : File name
        bool IsSet(std::string const& p_File) const;

        /// GetStringDefault
        /// @p_Name : Name of entry we are looking for
        /// @p_Default : If we cannot find p_Name then return the default string
        std::string GetStringDefault(std::string const& p_Name, std::string const& p_Default = "") const;

        /// GetBoolDefault
        /// @p_Name : Name of entry we are looking for
        /// @p_Default : If we cannot find p_Name then return the default string
        bool GetBoolDefault(std::string const& p_Name, bool const& p_Default) const;

        /// GetIntDefault
        /// @p_Name : Name of entry we are looking for
        /// @p_Default : If we cannot find p_Name then return the default string
        int32 GetIntDefault(std::string const& p_Name, int32 const& p_Default) const;

        /// GetFloatDefault
        /// @p_Name : Name of entry we are looking for
        /// @p_Default : If we cannot find p_Name then return the default string
        float GetFloatDefault(std::string const& p_Name, float const& p_Default) const;

        /// GetFileName - Return file name
        std::string GetFilename() const;

    private:
        std::string m_FileName;                                                  ///< File Name
        std::unordered_map<std::string, std::string> m_Entries;                  ///< Keys are converted to lower case.  values cannot be.
        std::mutex m_ConfigLock;                                                 ///< Mutex Lock
    };
} ///< NAMESPACE STEERSTONE
#define sConfig SteerStone::Config::instance()
