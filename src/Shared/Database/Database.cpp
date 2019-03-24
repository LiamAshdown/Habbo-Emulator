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

#include "Database.h"

namespace SteerStone
{ 
    /// Singleton class
    Database* Database::instance()
    {
        static Database instance;
        return &instance;
    }
    
    /// Constructor
    Database::Database()
    {

    }
    
    /// Deconstructor
    Database::~Database()
    {
        mDatabaseCont.clear();
    } 
    
    /// CreateDatabase
    /// @p_InfoString : Database user details; username, password, host, database, l_Port
    /// @p_PoolSize : How many pool connections database will launch
    bool Database::CreateDatabase(char const* p_InfoString, std::size_t const& p_PoolSize)
    {
        try 
        {
            std::string l_Username;
            std::string l_Password;
            std::string l_Database;
            std::string l_Host;
            std::string l_Port; 

            Tokens l_Tokens = StrSplit(p_InfoString, ";");

            Tokens::iterator l_Itr = l_Tokens.begin();

            if (l_Itr != l_Tokens.end())
                l_Host = *l_Itr++;
            if (l_Itr != l_Tokens.end())
                l_Port = *l_Itr++;
            if (l_Itr != l_Tokens.end())
                l_Username = *l_Itr++;
            if (l_Itr != l_Tokens.end())
                l_Password = *l_Itr++;
            if (l_Itr != l_Tokens.end())
                l_Database = *l_Itr++;

            /// Create our database, and store the database in a map = key "database name", storage "database structure"
            std::shared_ptr<DatabaseHolder> newDatabase = std::make_shared<DatabaseHolder>();
            newDatabase->m_Username = l_Username;
            newDatabase->m_Password = l_Password;
            newDatabase->m_Database = l_Database;
            newDatabase->m_Host = l_Host;
            newDatabase->m_Port = l_Port;
            newDatabase->m_PoolSize = p_PoolSize;

            newDatabase->m_MySQLConnection = std::make_shared<MySQLConnection>(newDatabase->GetName(), newDatabase->GetPassword(), newDatabase->GetDatabase(),
                newDatabase->GetHost(), newDatabase->GetPort());
            newDatabase->m_Pool = std::make_shared<ConnectionPool>(newDatabase->GetMySQLConnection(), newDatabase->GetPoolSize());

            mDatabaseCont[newDatabase->GetDatabase()] = newDatabase;

            l_Database[0] = std::toupper(l_Database[0]);
            LOG_INFO << "Connected to Database " << l_Database << " Successfully With " << p_PoolSize << " Pool Connections";

            return true;
        }
        catch (sql::SQLException const& p_ErrorCode) 
        {
            PrintException(p_ErrorCode, const_cast<char*>(__FILE__), const_cast<char*>(__FUNCTION__), __LINE__);
            return false;
        }
    }
    
    /// PrintException
    /// @p_ErrorCode : MYSQL Error code
    /// @p_File : Which file the error occured
    /// @p_Function : Which function the error occured
    /// @p_Line : Which line the error occured
    void Database::PrintException(sql::SQLException const& p_ErrorCode, char const* p_File, char const* p_Function, uint32 const p_Line)
    {
        std::string const& message = p_ErrorCode.what();

        /// Shut down server if database can no longer be reached
        if (message.find("has gone away") != std::string::npos)
            assert(false); ///< Assert if one of our database connections are no longer reachable
    }   

    /// GetDatabase - Get the database
    std::shared_ptr<DatabaseHolder> Database::GetDatabase(const std::string& database)
    {
        DatabaseMap::iterator l_Itr = mDatabaseCont.find(database);
        if (l_Itr != mDatabaseCont.end())
            return l_Itr->second;

        return nullptr;
    }
} ///< NAMESPACE STEERSTONE