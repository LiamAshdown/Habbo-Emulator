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

#ifndef _Quad_Config_h_
#define _Quad_Config_h_
#include <mutex>
#include <string>
#include <unordered_map>
#include "../Common/SharedDefines.h"

namespace Quad
{
    class Config
    {
    public:
        static Config* instance();

    public:
        Config() {}
        ~Config() {}

    public:
        bool SetFile(const std::string& file);
        bool Reload();

        bool IsSet(const std::string& name) const;

        const std::string GetStringDefault(const std::string& name, const std::string& def = "") const;
        bool GetBoolDefault(const std::string& name, bool def) const;
        int32 GetIntDefault(const std::string& name, int32 def) const;
        float GetFloatDefault(const std::string& name, float def) const;

        const std::string& GetFilename() const { return mFileName; }
        std::mutex mConfigLock;

    private:
        std::string mFileName;
        std::unordered_map<std::string, std::string> mEntries; // keys are converted to lower case.  values cannot be.
    };
}
#define sConfig Quad::Config::instance()

#endif /* !_Quad_Config_h_ */
