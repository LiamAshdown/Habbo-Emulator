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

#pragma once
#include <PCH/Precompiled.hpp>

#include "Singleton/Singleton.hpp"
#include "Utility/UtiLockable.hpp"
#include "Utility/UtiObjectGuard.hpp"

namespace SteerStone { namespace Core { namespace Configuration {

    /// Configuration Map
    using ConfigMap = std::unordered_map<std::string, std::string>;

    /// Base
    class Base : private Utils::LockableReadWrite
    {
        /// Allow access to lock / unlock methods
        friend class Utils::ObjectGuard<Base>;
        friend class Utils::ObjectReadGuard<Base>;
        friend class Utils::ObjectWriteGuard<Base>;

        SINGLETON_P_D(Base);

        //////////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////////

        public:
            /// Load File
            /// @p_File : File Name
            bool SetFile(std::string const& p_File);
            /// Returns file name
            std::string GetFilename() const;
            /// Check if entry exists
            /// @p_File : File name
            bool IsSet(std::string const& p_File);
            /// Reload File
            bool Reload();

            //////////////////////////////////////////////////////////////////////////
            //////////////////////////////////////////////////////////////////////////

            /// Get String configuration
            /// @p_Name     :  Name of entry we are looking for
            /// @p_Default  :  Default value
            std::string GetString(std::string const& p_Name, std::string const& p_Default = std::string());
            /// Get Default configuration
            /// @p_Name     :  Name of entry we are looking for
            /// @p_Default  :  Default value
            bool GetBool(std::string const& p_Name, bool const& p_Default);
            /// Get Int configuration
            /// @p_Name     :  Name of entry we are looking for
            /// @p_Default  :  Default value
            int32 GetInt(std::string const& p_Name, int32 const& p_Default);
            /// Get Float configuration
            /// @p_Name     :  Name of entry we are looking for
            /// @p_Default  :  Default value
            float GetFloat(std::string const& p_Name, float const& p_Default);


        //////////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////////

        private:
            std::string m_FileName;         ///< Name of file
            ConfigMap m_Entries;            ///< Entries inside file
    };

}   ///< Configuration
}   ///< Core
}   ///< SteerStone

#define sConfigManager SteerStone::Core::Configuration::Base::GetSingleton()