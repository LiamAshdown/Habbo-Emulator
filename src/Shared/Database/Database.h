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

#ifndef _DATABASE_DATABASE_h
#define _DATABASE_DATABASE_h
#include "../Common/SharedDefines.h"
#include "ConnectionPool.h"
#endif /* _DATABASE_DATABASE_h */

namespace SteerStone
{
    class Database;

    /// Hold the database information which is created in Database.cpp
    typedef struct DatabaseStruct
    {
    public:
        friend class Database;

    public:
        /// Constructor
        DatabaseStruct() {}
        /// Deconstructor
        ~DatabaseStruct() {}

    public:
        /// Get Database information
        std::shared_ptr<MySQLConnection> GetMySQLConnection() { return m_MySQLConnection; }
        std::shared_ptr<ConnectionPool>  GetConnectionPool()  { return m_Pool;            }
        std::string GetName()           const                 { return m_Username;        }
        std::string GetPassword()       const                 { return m_Password;        }
        std::string GetPort()           const                 { return m_Port;            }
        std::string GetHost()           const                 { return m_Host;            }
        std::string GetDatabase()       const                 { return m_Database;        }
        std::size_t GetPoolSize()       const                 { return m_PoolSize;        }

    private:
        std::string m_Username;                                                    ///< Username 
        std::string m_Password;                                                    ///< Password of our username
        std::string m_Port;                                                        ///< The port of the Ip Address
        std::string m_Host;                                                        ///< IP Address we are connecting to
        std::string m_Database;                                                    ///< Database which will be connecting to
        std::size_t m_PoolSize;                                                    ///< How many pool connections which will be launched
        std::shared_ptr<MySQLConnection> m_MySQLConnection;                        ///< Databse connection
        std::shared_ptr<ConnectionPool> m_Pool;                                    ///< Pool class which holds the databases

    }DatabaseHolder;

    typedef std::map<std::string, std::shared_ptr<DatabaseHolder>> DatabaseMap;

    /// Responsible for creating our database and then store database in DatabaseHolder
    /// Singleton Class
    class Database
    {
    public:
        static Database* instance();

    public:
        /// Constructor
        Database();
        /// Deconstructor
        ~Database();

    public:
        /// CreateDatabase
        /// @p_InfoString : Database user details; username, password, host, database, port
        /// @p_PoolSize : How many pool connections database will launch
        bool CreateDatabase(char const* p_InfoString, std::size_t const& p_PoolSize);

        /// PrintException
        /// @p_ErrorCode : MYSQL Error code
        /// @p_File : Which file the error occured
        /// @p_Function : Which function the error occured
        /// @p_Line : Which line the error occured
        void PrintException(sql::SQLException const& p_ErrorCode, char const* p_File, char const* p_Function, uint32 const p_Line);

        /// GetDatabase - Get the database
        std::shared_ptr<DatabaseHolder> GetDatabase(std::string const& database);

    private:
        DatabaseMap m_DatabaseCont;                                               ///< Database container holder           
    };
} ///< NAMESPACE STEERSTONE

#define sDatabase SteerStone::Database::instance()
