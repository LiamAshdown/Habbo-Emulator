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

#include "MYSQLPreparedStatement.h"

namespace SteerStone
{
    /// Constructor
    /// @p_MYSQLPreparedStatement : Keep reference of our connection
    PreparedStatement::PreparedStatement(MYSQLPreparedStatement* p_MySQLPreparedStatement) : m_MySQLPreparedStatement(p_MySQLPreparedStatement), m_Stmt(nullptr), m_Bind(nullptr), m_PrepareError(false)
    {
    }

    /// Deconstructor
    PreparedStatement::~PreparedStatement()
    {
    }

    /// TryLock
    /// Attempt to lock the object
    bool PreparedStatement::TryLock()
    {
        return m_Mutex.try_lock();
    }

    /// Unlock
    /// Allow the prepare statement to be accessed again
    void PreparedStatement::Unlock()
    {
        m_Mutex.unlock();
    }

    /// Prepare the statement
    /// @p_Query : Query which will be executed to database
    void PreparedStatement::PrepareStatement(char const* p_Query)
    {
        if (Prepare(p_Query))
        {
            m_PrepareError = true;
            LOG_ERROR << "Failed in Preparing Statement!";
        }
    }

    /// ExecuteStatement
    /// Execute the statement
    std::unique_ptr<PreparedResultSet> PreparedStatement::ExecuteStatement()
    {
        if (m_PrepareError)
            return nullptr;

        MYSQL_RES* l_Result = nullptr;
        uint32 l_FieldCount = 0;

        if (!Execute(&l_Result, &l_FieldCount))
            return nullptr;

        return std::make_unique<PreparedResultSet>(m_Stmt, l_Result, l_FieldCount);
    }

    /// Prepare
    /// Prepare the query
    /// @p_Query : Query which will be executed to database
    bool PreparedStatement::Prepare(char const * p_Query)
    {
        /// Remove previous binds
        RemoveBinds();
        m_Query = p_Query;

        return m_MySQLPreparedStatement->Prepare(this);
    }

    /// Execute
    /// @p_Result : Result set
    /// @p_FieldCount : How many columns
    bool PreparedStatement::Execute(MYSQL_RES ** p_Result, uint32 * p_FieldCount)
    {
        BindParameters();

        if (mysql_stmt_execute(m_Stmt))
        {
            LOG_ERROR << "mysql_stmt_execute: " << mysql_stmt_error(m_Stmt);
            RemoveBinds();
            return false;
        }

        *p_Result = mysql_stmt_result_metadata(m_Stmt);
        *p_FieldCount = mysql_stmt_field_count(m_Stmt);

        if (!*p_Result)
        {
            mysql_free_result(*p_Result);
            return false;
        }

        return true;
    }

    /// BindParameters
    /// Bind parameters from storage into SQL
    void PreparedStatement::BindParameters()
    {
        for (auto l_Itr = m_Binds.cbegin(); l_Itr != m_Binds.cend(); l_Itr++)
        {
            uint8 l_Unsigned = 0;
            m_Bind[l_Itr->first].buffer_type = l_Itr->second.GetFieldType(l_Unsigned);
            m_Bind[l_Itr->first].is_unsigned = l_Unsigned;
            m_Bind[l_Itr->first].buffer = l_Itr->second.GetBuffer();
            m_Bind[l_Itr->first].length = nullptr;
            m_Bind[l_Itr->first].buffer_length = l_Itr->second.GetSize();
        }

        if (mysql_stmt_bind_param(m_Stmt, m_Bind))
            LOG_ERROR << "mysql_stmt_bind_param: Cannot bind parameters ON " << m_Query;
    }

    /// RemoveBinds
    /// Remove previous binds
    void PreparedStatement::RemoveBinds()
    {
        if (!m_Stmt || !m_Bind)
            return;

        delete[] m_Bind;
        m_Binds.clear();
        mysql_stmt_close(m_Stmt);

        m_PrepareError = false;
        m_Bind = nullptr;
        m_Stmt = nullptr;
        m_Query = std::string();
        m_ParametersCount = 0;
    }
}