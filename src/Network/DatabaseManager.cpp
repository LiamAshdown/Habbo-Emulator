#include "DatabaseManager.h"

DatabaseManager* DatabaseManager::instance()
{
    static DatabaseManager instance;
    return &instance;
}

/*
    Constructor for database manager
*/
DatabaseManager::DatabaseManager()
{
    host = "127.0.0.1";
    port = 3306;
    username = "root";
    password = "carbon12";
    database = "habbo";
    pool_size = 5;
    tested_connection = false;
    mysql_connection_factory = nullptr;
    mysql_pool = nullptr;
}
/*
    Tests connection to the database, will attempt to make a pool and store 5 connections,
    it will print the error if test was unsuccessful.

    This can only be performed once.

    @return if connection is success
*/
bool DatabaseManager::testConnection() {

    try {

        if (this->tested_connection != true) {
            this->tested_connection = true;
            this->mysql_connection_factory = new MySQLConnectionFactory(this->host, this->port, this->username, this->password, this->database);
            this->mysql_pool = new ConnectionPool<MySQLConnection>(this->pool_size, this->mysql_connection_factory);

        }
    }
    catch (sql::SQLException &e) {
        this->printException(e, const_cast<char*>(__FILE__), const_cast<char*>(__FUNCTION__), __LINE__);
        return false;
    }

    return true;
}

void DatabaseManager::printException(sql::SQLException &e, char* file, char* function, int line) {
    std::cout << std::endl << " [ERROR] SQLException in " << file << "(" << function << ") on line " << line << std::endl;
    std::cout << " [ERROR] Message: " << e.what() << std::endl;
    std::cout << " [ERROR] Error code: " << e.getErrorCode() << std::endl;
    std::cout << " [ERROR] SQLState: " << e.getSQLState() << std::endl;
    std::cout << std::endl;

    std::string message = e.what();

    if (message.find("has gone away") != std::string::npos) {

        if (mysql_connection_factory != nullptr) {
            delete this->mysql_connection_factory;
            this->mysql_connection_factory = new MySQLConnectionFactory(this->host, this->port, this->username, this->password, this->database);
        }

        if (mysql_pool != nullptr) {
            delete this->mysql_pool;
            this->mysql_pool = new ConnectionPool<MySQLConnection>(this->pool_size, this->mysql_connection_factory);
        }
    }
}

DatabaseManager::~DatabaseManager() {

    if (this->tested_connection) {
        delete this->mysql_connection_factory;
        delete this->mysql_pool;
    }
}