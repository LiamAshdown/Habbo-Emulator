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
#include "ConnectionPool.h"

#include <mutex>

namespace Quad
{
    class Database;

    typedef struct DatabaseStruct
    {
    public:
        friend class Database;

    public:
        DatabaseStruct() {}
        ~DatabaseStruct() {}

    public:
        std::shared_ptr<MySQLConnection> GetMySQLConnection() { return mMySQLConnection; }
        std::shared_ptr<ConnectionPool> GetConnectionPool() {return mPool; }
        std::string GetName() const { return mUsername; }
        std::string GetPassword() const { return mPassword; }
        std::string GetPort() const { return mPort; }
        std::string GetHost() const { return mHost; }
        std::string GetDatabase() const { return mDatabaseName; }
        uint32 GetPoolSize() const { return mPoolSize; }

    private:
        std::shared_ptr<MySQLConnection> mMySQLConnection;
        std::shared_ptr<ConnectionPool> mPool;

        std::string mUsername;
        std::string mPassword;
        std::string mPort;
        std::string mHost;
        std::string mDatabaseName;
        uint32 mPoolSize;

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
        bool CreateDatabase(const char* infoString, const uint32& poolSize);
        void PrintException(sql::SQLException &e, char* file, char* function, const uint32 line);

        std::shared_ptr<DatabaseHolder> GetDatabase(const std::string& database);

    private:
        DatabaseMap mDatabaseCont;
    };
}

#define sDatabase Quad::Database::instance()

#endif /* !_Quad_Database_h_ */
