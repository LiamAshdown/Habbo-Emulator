/*
* Liam Ashdown
* Copyright(C) 2019
*
* This program is free software : you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.If not, see < http://www.gnu.org/licenses/>.
*/

#ifndef _DATABASE_MYSQL_CONNECTION_h
#define _DATABASE_MYSQL_CONNECTION_h
#include "Common/SharedDefines.h"
#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/statement.h>
#include <cppconn/connection.h>
#endif /* _DATABASE_MYSQL_CONNECTION_h */

namespace SteerStone 
{
    class MySQLConnection;

    /// Stores our SQLConnection which ConnectionPool uses to get our database which is passed into QueryDatabase
    typedef struct ConnectionStruct
    {
    public:
        friend class MySQLConnection;

    public:
        /// Constructor
        ConnectionStruct() {}
        /// Deconstructor - Close our SQLConnection
        ~ConnectionStruct() 
        {
            mSQLConnection->close();
            mSQLConnection.reset();
        }

    public:
        /// GetSQLConnection - Get Our SQLConnection
        std::shared_ptr<sql::Connection> GetSQLConnection() { return mSQLConnection; }

    private:
        std::shared_ptr<sql::Connection> mSQLConnection;                                                    ///< SQLConnection
    }Connection;
    
    /// Class which stores the database information
    class MySQLConnection
    {
    public:
        /// Constructor
        /// @p_Username : Username
        /// @p_Password : Password of our username
        /// @p_Database : Database which will be connecting to
        /// @p_Host : IP Address we are connecting to
        /// @p_Port : The port of the Ip Address
        MySQLConnection(std::string const& p_Username, std::string const& p_Password, std::string const& p_Database, std::string const& p_Host, std::string const& p_Port)
        {
            m_Username = p_Username;
            m_Password = p_Password;
            m_Database = p_Database;
            m_Port = p_Port;
            m_Host = "tcp://" + p_Host + ":" + p_Port;
        }
        /// Deconstructor
        ~MySQLConnection() {}

    public:
        /// CreateDatabase - Create a database connection and store it into ConnectionPool
        std::shared_ptr<Connection> CreateDatabase()
        {
            sql::Driver* driver = get_driver_instance();

            m_Connection = std::make_shared<Connection>();

            /// Connect to our database
            m_Connection->mSQLConnection = std::shared_ptr<sql::Connection>(driver->connect(m_Host.c_str(), m_Username.c_str(), m_Password.c_str()));
            m_Connection->mSQLConnection->setSchema(m_Database.c_str());

            return m_Connection;
        }

        std::shared_ptr<Connection> GetConnection()
        {
            return m_Connection;
        }

    private:
        std::string m_Username;                                                    ///< Username 
        std::string m_Password;                                                    ///< Password of our username
        std::string m_Port;                                                        ///< The port of the Ip Address
        std::string m_Host;                                                        ///< IP Address we are connecting to
        std::string m_Database;                                                    ///< Database which will be connecting to
        std::shared_ptr<Connection> m_Connection;                                  ///< Connection of Database
    };
} /// NAMESPACE STEERSTONE
