#pragma once
#include <../../dep/CppConnector/include/mysql_connection.h>
#include <../../dep/CppConnector/include/cppconn/driver.h>
#include <../../dep/CppConnector/include/cppconn/driver.h>
#include <../../dep/CppConnector/include/cppconn/exception.h>
#include <../../dep/CppConnector/include/cppconn/resultset.h>
#include <../../dep/CppConnector/include/cppconn/prepared_statement.h>
#include <../../dep/CppConnector/include/cppconn/statement.h>
#include "ConnectionPool.h"

class MySQLConnection : public Connection {

public:
    ~MySQLConnection() {
        if (this->sql_connection) {
            this->sql_connection->close();
            this->sql_connection.reset();     // Release and destruct    
        }
    };

    int a;
    std::shared_ptr<sql::Connection> sql_connection;
};


class MySQLConnectionFactory : public ConnectionFactory {

public:
    MySQLConnectionFactory(std::string server, std::string port, std::string username, std::string password, std::string database) {
        this->server = "tcp://" + server + ":" + port;
        this->username = username;
        this->password = password;
        this->database = database;

    };

    // Any exceptions thrown here should be caught elsewhere
    std::shared_ptr<Connection> create() {

        // Get the driver
        sql::Driver *driver = get_driver_instance();

        // Create the connection
        std::shared_ptr<MySQLConnection>conn(new MySQLConnection());

        // Connect
        conn->sql_connection = std::shared_ptr<sql::Connection>(driver->connect(this->server, this->username, this->password));
        conn->sql_connection->setSchema(this->database);

        return std::static_pointer_cast<Connection>(conn);
    };

private:
    std::string server;
    std::string username;
    std::string password;
    std::string database;
};