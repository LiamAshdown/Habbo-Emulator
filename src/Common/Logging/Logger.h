/*
* Liam Ashdown
* Copyright (C) 2018
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

#ifndef _QuadEmu_Logger_h_
#define _QuadEmu_Logger_h_
#endif /* _QuadEmu_Logger_ */

enum LogLevels
{
    LOG_DEBUG = 0,
    LOG_INFO  = 1,
    LOG_ERROR = 2
};

class Logger
{
public:
    static Logger* instance();

    Logger() {}
    ~Logger() {}

    void Log(const char* log, LogLevels level);

};
#define sLog Logger::instance()

#define QUAD_LOG_INFO(S) sLog->Log(#S, LOG_INFO)      
#define QUAD_LOG_DEBUG(S) sLog->Log(#S, LOG_DEBUG)       
#define QUAD_LOG_ERROR(S) sLog->Log(#S, LOG_ERROR)       