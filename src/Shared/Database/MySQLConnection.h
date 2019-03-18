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

#ifndef _Quad_MYSQLConnection_h_
#define _Quad_MYSQLConnection_h_

#include "Common/SharedDefines.h"
#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/statement.h>
#include <cppconn/connection.h>

namespace Quad 
{
    class MySQLConnection;

    typedef struct ConnectionStruct
    {
    public:
        friend class MySQLConnection;

    public:
        ConnectionStruct() {}
        ~ConnectionStruct() 
        {
            mSQLConnection->close();
            mSQLConnection.reset();
        }

    public:
        std::shared_ptr<sql::Connection> GetSQLConnection() { return mSQLConnection; }

    private:
        std::shared_ptr<sql::Connection> mSQLConnection;
    }Connection;

    class MySQLConnection
    {
    public:
        MySQLConnection(const std::string& username, const std::string& password, const std::string& database, const std::string& host, const std::string& port)
        {
            mUsername = username;
            mPassword = password;
            mDatabase = database;
            mPort = port;
            mHost = "tcp://" + host + ":" + port;
        }
        ~MySQLConnection() {}

    public:
        std::shared_ptr<Connection> CreateDatabase()
        {
            sql::Driver* driver = get_driver_instance();

            mConnection = std::make_shared<Connection>();

            // Connect our database
            mConnection->mSQLConnection = std::shared_ptr<sql::Connection>(driver->connect(mHost.c_str(), mUsername.c_str(), mPassword.c_str()));
            mConnection->mSQLConnection->setSchema(mDatabase.c_str());

            return mConnection;
        }

        std::shared_ptr<Connection> GetConnection()
        {
            return mConnection;
        }

    private:
        std::string mUsername;
        std::string mPassword;
        std::string mPort;
        std::string mHost;
        std::string mDatabase;

        std::shared_ptr<Connection> mConnection;
    };
}

#endif /* !_Quad_MYSQLConnection_h_ */
