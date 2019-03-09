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

#ifndef _Quad_Database_h_
#define _Quad_Database_h_
#include "../Common/SharedDefines.h"
#include "cppconn/exception.h"
#include "mysql_connection.h"
#include "MYSQLConnection.h"

namespace Quad
{
    typedef struct DatabaseStruct
    {
    public:
        DatabaseStruct() : sConnectionFactory(nullptr), sPool(nullptr) {}
        ~DatabaseStruct()
        {
            IF_LOG(plog::debug)
                LOG_DEBUG << "Destructor DatabaseStruct called!";
        }

        std::shared_ptr<ConnectionPool<MySQLConnection>> GetConnectionPool()
        {
            return sPool;
        }

        std::shared_ptr<MySQLConnectionFactory> sConnectionFactory;
        std::shared_ptr<ConnectionPool<MySQLConnection>> sPool;

        std::string sUsername;
        std::string sPassword;
        std::string sPort;
        std::string sHost;
        std::string sDatabaseName;
        uint32 sPoolSize;

    }DatabaseHolder;

    typedef std::map<std::string, std::shared_ptr<DatabaseHolder>> DatabaseMap;

    class Database
    {
    public:
        static Database* instance();

    public:
        Database();
        ~Database();

    public:
        bool InitializeConnectionPool(const char* infoString, const uint32 poolSize);
        void PrintException(sql::SQLException &e, char* file, char* function, uint32 line);

        std::shared_ptr<DatabaseHolder> GetDatabase(const std::string& database);

    protected:
        void RemoveDatabase(const std::string& database);

    private:
        DatabaseMap mDatabaseCont;
    };
}
#define sDatabase Quad::Database::instance()

#endif /* !_Quad_Database_h_ */
