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
        IF_LOG(plog::debug)
            LOG_DEBUG << "Destructor Database called!";

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
    void Database::RemoveDatabase(const std::string& database)
    {
        DatabaseMap::iterator itr = mDatabaseCont.find(database);
        if (itr != mDatabaseCont.end())
            mDatabaseCont.erase(itr);
    }
    //-----------------------------------------------//
    bool Database::InitializeConnectionPool(const char* infoString, const uint32 poolSize)
    {
        try 
        {
            std::string username, password, database, host;
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

            std::shared_ptr<DatabaseHolder> newDatabase = std::make_shared<DatabaseHolder>();
            newDatabase->sUsername = username;
            newDatabase->sPassword = password;
            newDatabase->sDatabaseName = database;
            newDatabase->sHost = host;
            newDatabase->sPort = port;
            newDatabase->sPoolSize = poolSize;

            newDatabase->sConnectionFactory = std::make_shared<MySQLConnectionFactory>(newDatabase->sUsername, newDatabase->sPassword, newDatabase->sDatabaseName,
                newDatabase->sHost, newDatabase->sPort);
            newDatabase->sPool = std::make_shared<ConnectionPool<MySQLConnection>>(newDatabase->sPoolSize, newDatabase->sConnectionFactory);

            mDatabaseCont[newDatabase->sDatabaseName] = newDatabase;
            return true;
        }
        catch (sql::SQLException &e) 
        {
            this->PrintException(e, const_cast<char*>(__FILE__), const_cast<char*>(__FUNCTION__), __LINE__);
            return false;
        }
    }
    //-----------------------------------------------//
    void Database::PrintException(sql::SQLException& e, char* file, char* function, uint32 line)
    {
        std::string message = e.what();

        LOG_ERROR << message << std::endl;

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