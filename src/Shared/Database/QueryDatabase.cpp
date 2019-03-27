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

#include "QueryDatabase.h"

namespace SteerStone
{
    /// Constructor
    /// @p_Database : Database we are going to query
    QueryDatabase::QueryDatabase(std::string const& p_Database)
    {
        m_Database = p_Database;
        m_Connection = sDatabase->GetDatabase(m_Database)->GetConnectionPool()->Borrow();
    }
    
    /// Deconstructor
    QueryDatabase::~QueryDatabase()
    {
        sDatabase->GetDatabase(m_Database)->GetConnectionPool()->UnBorrow(m_Connection);
    }
    
    /// DirectExecuteQuery
    /// @p_Query : Query which will be executed to the database
    void QueryDatabase::DirectExecuteQuery(std::string const p_Query)
    {
        try
        {
            m_IsExecuteResult = false;
            m_SqlConnection = m_Connection->GetSQLConnection();

            m_Statement = std::shared_ptr<sql::Statement>(m_SqlConnection->createStatement());
            m_ExecuteResult = m_Statement->execute(p_Query.c_str());
            m_IsExecuteResult = true;
        }
        catch (sql::SQLException &e)
        {
            sDatabase->PrintException(e, const_cast<char*>(__FILE__), const_cast<char*>(__FUNCTION__), __LINE__);
        }
    }
    
    /// PrepareQuery
    /// @p_Query : Query which will be prepared to be executed into the database
    void QueryDatabase::PrepareQuery(std::string const& p_Query)
    {
        m_SqlConnection = m_Connection->GetSQLConnection();

        m_PreparedStatement = std::shared_ptr<sql::PreparedStatement>(m_SqlConnection->prepareStatement(p_Query.c_str()));
        m_IsExecuteResult = false;
    }
    
    /// ExecuteQuery
    /// Execute Prepare query into database
    void QueryDatabase::ExecuteQuery()
    {
        try
        {
            m_ResultSet = std::unique_ptr<sql::ResultSet>(m_PreparedStatement->executeQuery());
        }
        catch (sql::SQLException &e)
        {
            sDatabase->PrintException(e, const_cast<char*>(__FILE__), const_cast<char*>(__FUNCTION__), __LINE__);
        }
    }
    
    /// GetResult
    /// Returns if query is successful or not
    bool QueryDatabase::GetResult()
    {
        if (m_IsExecuteResult)
            return m_ExecuteResult;
        else if (m_ResultSet->next())
        {
            m_Result.m_ResultSet = std::move(m_ResultSet);
            return true;
        }
        return false;
    }
    
    /// GetStatement - Get Statement of query
    std::shared_ptr<sql::PreparedStatement>& QueryDatabase::GetStatement()
    {
        return m_PreparedStatement;
    }
    
    /// ClearParameters
    /// Reset our connection, this is used when we want to use the same object
    /// and want to query multiple times
    void QueryDatabase::ClearParameters()
    {
        m_PreparedStatement->clearParameters();
        m_IsExecuteResult = false;
    }

    /// Fetch
    /// Returns pointer
    Result* QueryDatabase::Fetch()
    {
        return &m_Result;
    }
} ///< NAMESPACE STEERSTONE