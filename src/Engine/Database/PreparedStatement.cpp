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

#include "Database/MYSQLPreparedStatement.hpp"
#include "Database.hpp"
#include "Database/SQLCommon.hpp"
#include "Logger/LogDefines.hpp"

namespace SteerStone { namespace Core { namespace Database {

    /// Constructor
    /// @p_MYSQLPreparedStatement : Reference
    PreparedStatement::PreparedStatement(std::shared_ptr<MYSQLPreparedStatement> p_MySQLPreparedStatement) 
        : m_MYSQLPreparedStatement(p_MySQLPreparedStatement), m_Stmt(nullptr), m_Bind(nullptr), m_PrepareError(false), m_Prepared(false), m_ParametersCount(0)
    {
        #ifdef STEERSTONE_CORE_DEBUG
            LOG_INFO("PreparedStatement", "PreparedStatement initialized!");
        #endif
    }

    /// Deconstructor
    PreparedStatement::~PreparedStatement()
    {
        RemoveBinds();
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Attempt to lock
    bool PreparedStatement::TryLockMutex()
    {
        return m_Mutex.try_lock();
    }
    /// Allow to be accessed
    void PreparedStatement::UnlockMutex()
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

            LOG_ASSERT(false, "Database", "Failed in Preparing Statement!");
        }
    }
    /// ExecuteStatement
    /// Execute the statement
    /// @p_FreeStatementAutomatically : Free the prepared statement when PreparedResultSet deconstructors
    std::unique_ptr<PreparedResultSet> PreparedStatement::ExecuteStatement(bool p_FreeStatementAutomatically)
    {
        if (m_PrepareError)
            return nullptr;
            
        MYSQL_RES* l_Result = nullptr;
        uint32 l_FieldCount = 0;

        BindParameters();

        if (m_MYSQLPreparedStatement->Execute(m_Stmt, &l_Result, &l_FieldCount))
        {
            std::unique_ptr<PreparedResultSet> l_PreparedResultSet = std::make_unique<PreparedResultSet>(this, l_Result, l_FieldCount);

            if (l_PreparedResultSet && l_PreparedResultSet->GetRowCount() || p_FreeStatementAutomatically)
                return std::move(l_PreparedResultSet);
        }

        return nullptr;
    }

    /// Clear Prepare Statement
    void PreparedStatement::Clear()
    {
        if (m_Stmt->bind_result_done)
        {
            if (m_Stmt->bind->length)
                delete m_Stmt->bind->length;
            if( m_Stmt->bind->is_null)
                delete m_Stmt->bind->is_null;
        }

        m_Prepared = false;

        /// Free the statement
        m_MYSQLPreparedStatement->GetDatabase()->FreePrepareStatement(this);
    }

    MYSQL_STMT* PreparedStatement::GetStatement()
    {
        return m_Stmt;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Prepare the query
    /// @p_Query : Query which will be executed to database
    bool PreparedStatement::Prepare(char const* p_Query)
    {
        if (m_Prepared)
        {
            LOG_ASSERT(false, "Database", "Trying to prepare a statement but statement is already in use!");
            return true;
        }

        RemoveBinds();

        m_Query = p_Query;

        return m_MYSQLPreparedStatement->Prepare(this);
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
            LOG_ERROR("Database", "Cannot bind parameters on %0", m_Query);
    }

    /// Remove previous binds
    void PreparedStatement::RemoveBinds()
    {
        if (!m_Stmt)
            return;

        if (m_ParametersCount)
        {
            delete[] m_Bind;
            m_Bind = nullptr;
            m_Binds.clear();
        }

        m_PrepareError = false;
        m_ParametersCount = 0;
        m_Query.clear();
        mysql_stmt_close(m_Stmt);
        m_Stmt = nullptr;
    }

}   ///< namespace Database
}   ///< namespace Core
}   ///< namespace SteerStone