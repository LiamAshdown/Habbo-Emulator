/*
* Liam Ashdown
* Copyright(C) 2019
*
* Active911 Inc.
* Copyright(C) 2013
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
#include "ConnectionPool.h"
#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/statement.h>

namespace Quad 
{
    class MySQLConnection : public Connection 
    {
    public:
        ~MySQLConnection()
        {
            if (SQLConnection)
            {
                SQLConnection->close();
                SQLConnection.reset();    // Release and destruct
            }
        }

    public:
        std::shared_ptr<sql::Connection> SQLConnection;
    };

    class MySQLConnectionFactory : public ConnectionFactory 
    {

    public:
        MySQLConnectionFactory(const std::string username, const std::string password, const std::string database, const std::string host, const std::string port)
        {
            mUsername = username;
            mPassword = password;
            mDatabase = database;
            mPort = port;
            mHost = "tcp://" + host + ":" + port;
        }

    public:
        // Any exceptions thrown here should be caught elsewhere
        std::shared_ptr<Connection> Create() 
        {
            // Get the driver
            sql::Driver *driver = get_driver_instance();

            // Create the connection
            std::shared_ptr<MySQLConnection> conn(new MySQLConnection());

            // Connect to the database
            conn->SQLConnection = std::shared_ptr<sql::Connection>(driver->connect(mHost.c_str(), mUsername.c_str(), mPassword.c_str()));
            conn->SQLConnection->setSchema(mDatabase.c_str());

            IF_LOG(plog::debug)
                LOG_DEBUG << "Sucessfully connected to " << mDatabase << " database";

            return std::static_pointer_cast<Connection>(conn);
        }

    private:
        std::string mUsername;
        std::string mPassword;
        std::string mPort;
        std::string mHost;
        std::string mDatabase;
    };
}

#endif /* !_Quad_MYSQLConnection_h_ */
