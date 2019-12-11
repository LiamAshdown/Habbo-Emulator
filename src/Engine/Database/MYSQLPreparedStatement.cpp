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

#include "Database/Database.hpp"
#include "Logger/LogDefines.hpp"

namespace SteerStone { namespace Core { namespace Database {

    /// Constructor
    /// @p_Base : Database
    MYSQLPreparedStatement::MYSQLPreparedStatement(Base* p_Base) 
        : m_Base(p_Base)
    {
        #ifdef STEERSTONE_CORE_DEBUG
            LOG_INFO("PreparedStatements", "MYSQLPreparedStatement Initialized");
        #endif

        m_Connection = nullptr;

        for (uint32 l_I = 0; l_I < MAX_PREPARED_STATEMENTS; l_I++)
            m_Statements[l_I] = nullptr;
    }
    /// Deconstructor
    MYSQLPreparedStatement::~MYSQLPreparedStatement()
    {
        mysql_close(m_Connection);
    }

    /// Connect
    /// @p_Username : Name of user
    /// @p_Password : Password of user
    /// @p_Port     : Port we are connecting to
    /// @p_Host     : Address we are connecting to
    /// @p_Database : Database we are querying to
    /// @p_PoolSize : Amount of MYSQL connections we are spawning
    uint32 MYSQLPreparedStatement::Connect(std::string const p_Username, std::string const p_Password, uint32 const p_Port, std::string const p_Host, std::string const p_Database)
    {
        /// Initialize connection
        MYSQL* l_Connection = mysql_init(NULL);

        if (!l_Connection)
        {
            LOG_INFO("Database", "Could not initialize MySQL connection to database: %0", p_Database);
            return 2000; ///< CR_UNKNOWN_ERROR
        }

        /// We handle data by utf8 - so do same for database
        mysql_options(l_Connection, MYSQL_SET_CHARSET_NAME, "utf8");

        /// Connect to database
        m_Connection = mysql_real_connect(l_Connection, p_Host.c_str(), p_Username.c_str(), p_Password.c_str(), p_Database.c_str(), p_Port, NULL, NULL);

        if (m_Connection)
        {
            static bool l_Logged = false;

            if (!l_Logged)
            {
                LOG_INFO("Database", "MySQL Client Library: %0", mysql_get_client_info());
                LOG_INFO("Database", "MySQL Server Version: %0", mysql_get_server_info(m_Connection));
                LOG_INFO("Database", "Connected to MYSQL Database at %0", m_Connection->host);

                l_Logged = true;
            }

            /// Set up prepare statements
            for (uint32 l_I = 0; l_I < MAX_PREPARED_STATEMENTS; l_I++)
                m_Statements[l_I] = new PreparedStatement(shared_from_this());

            mysql_set_character_set(m_Connection, "utf8");

            return 0;
        }
        else
        {
            /// Free connection and report error
            mysql_close(l_Connection);
            return mysql_errno(l_Connection);
        }
    }
    /// Prepare the statement
    /// @p_StatementHolder : Statement being prepared
    bool MYSQLPreparedStatement::Prepare(PreparedStatement* p_StatementHolder)
    {
        Utils::ObjectGuard l_Guard(this);

        p_StatementHolder->m_Stmt = mysql_stmt_init(m_Connection);

        if (!p_StatementHolder->m_Stmt)
        {
            LOG_INFO("Database", "Failed in initializing MYSQL. Error: %0", mysql_error(m_Connection));
            return true;
        }

        /// Set buffer max value
        bool l_Temp = true;
        mysql_stmt_attr_set(p_StatementHolder->m_Stmt, STMT_ATTR_UPDATE_MAX_LENGTH, &l_Temp);

        if (mysql_stmt_prepare(p_StatementHolder->m_Stmt, p_StatementHolder->m_Query.c_str(), static_cast<unsigned long>(p_StatementHolder->m_Query.length())))
        {
            LOG_ERROR("Database", "%0 on %1", mysql_error(m_Connection), p_StatementHolder->m_Query);

            mysql_stmt_close(p_StatementHolder->m_Stmt);

            return true;
        }

        p_StatementHolder->m_ParametersCount = mysql_stmt_param_count(p_StatementHolder->m_Stmt);

        if (p_StatementHolder->m_ParametersCount)
        {
            p_StatementHolder->m_Bind = new MYSQL_BIND[p_StatementHolder->m_ParametersCount];
            memset(p_StatementHolder->m_Bind, 0, sizeof(MYSQL_BIND) * p_StatementHolder->m_ParametersCount);
        }

        p_StatementHolder->m_Prepared = true;

        return false;
    }
    /// Execute the statement
    /// @p_Stmt : Statement being executed
    /// @p_Result : Result set
    /// @p_FieldCount : Field count
    bool MYSQLPreparedStatement::Execute(MYSQL_STMT* p_Stmt, MYSQL_RES ** p_Result, uint32 * p_FieldCount)
    {
        Utils::ObjectGuard l_Guard(this);

        if (mysql_stmt_execute(p_Stmt))
        {
            LOG_ASSERT(false, "Database", "Failed to execute statement. Error: %0",mysql_stmt_error(p_Stmt));
            return false;
        }

        *p_Result = mysql_stmt_result_metadata(p_Stmt);
        *p_FieldCount = mysql_stmt_field_count(p_Stmt);

        return true;
    }

    /// Returns database
    Base* MYSQLPreparedStatement::GetDatabase() const
    {
        return m_Base;
    }

}   ///< namespace Database
}   ///< namespace Core
}   ///< namespace SteerStone