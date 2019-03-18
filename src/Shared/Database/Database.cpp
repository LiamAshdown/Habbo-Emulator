/*
* Liam Ashdown
* Copyright (C) 2019
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
//-----------------------------------------------//
#include "Database.h"
//-----------------------------------------------//
namespace Quad
{
    //-----------------------------------------------//
    Tokens StrSplit(const std::string& src, const std::string& sep);
    //-----------------------------------------------//
    Database* Database::instance()
    {
        static Database instance;
        return &instance;
    }
    //-----------------------------------------------//
    Database::Database()
    {

    }
    //-----------------------------------------------//
    Database::~Database()
    {
        mDatabaseCont.clear();
    }
    //-----------------------------------------------//
    std::shared_ptr<DatabaseHolder> Database::GetDatabase(const std::string& database)
    {
        DatabaseMap::iterator itr = mDatabaseCont.find(database);
        if (itr != mDatabaseCont.end())
            return itr->second;

        return nullptr;
    }
    //-----------------------------------------------//
    bool Database::CreateDatabase(const char* infoString, const uint32& poolSize)
    {
        try 
        {
            std::string username;
            std::string password;
            std::string database;
            std::string host;
            std::string port; 

            Tokens tokens = StrSplit(infoString, ";");

            Tokens::iterator iter = tokens.begin();

            if (iter != tokens.end())
                host = *iter++;
            if (iter != tokens.end())
                port = *iter++;
            if (iter != tokens.end())
                username = *iter++;
            if (iter != tokens.end())
                password = *iter++;
            if (iter != tokens.end())
                database = *iter++;

            // Create our database, and store the database in a map = key "database name", storage "database structure"
            std::shared_ptr<DatabaseHolder> newDatabase = std::make_shared<DatabaseHolder>();
            newDatabase->mUsername = username;
            newDatabase->mPassword = password;
            newDatabase->mDatabaseName = database;
            newDatabase->mHost = host;
            newDatabase->mPort = port;
            newDatabase->mPoolSize = poolSize;

            newDatabase->mMySQLConnection = std::make_shared<MySQLConnection>(newDatabase->GetName(), newDatabase->GetPassword(), newDatabase->GetDatabase(),
                newDatabase->GetHost(), newDatabase->GetPort());
            newDatabase->mPool = std::make_shared<ConnectionPool>(newDatabase->GetMySQLConnection(), newDatabase->GetPoolSize());

            mDatabaseCont[newDatabase->GetDatabase()] = newDatabase;

            database[0] = std::toupper(database[0]);
            LOG_INFO << "Connected to Database " << database << " Successfully With " << poolSize << " Pool Connections";

            return true;
        }
        catch (sql::SQLException &e) 
        {
            this->PrintException(e, const_cast<char*>(__FILE__), const_cast<char*>(__FUNCTION__), __LINE__);
            return false;
        }
    }
    //-----------------------------------------------//
    void Database::PrintException(sql::SQLException& e, char* file, char* function, const uint32 line)
    {
        std::string message = e.what();

        // Shut down server if database can no longer be reached
        if (message.find("has gone away") != std::string::npos)
            assert(false);
    }
    //-----------------------------------------------//
    Tokens StrSplit(const std::string& src, const std::string& sep)
    {
        Tokens r;
        std::string s;
        for (char i : src)
        {
            if (sep.find(i) != std::string::npos)
            {
                if (s.length()) r.push_back(s);
                s.clear();
            }
            else
            {
                s += i;
            }
        }
        if (s.length()) r.push_back(s);
        return r;
    }
    //-----------------------------------------------//
}