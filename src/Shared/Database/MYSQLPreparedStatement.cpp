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
    volatile bool MYSQLPreparedStatement::s_Logged = false;

    /// Constructor
    /// @p_Database : Keep reference of database
    MYSQLPreparedStatement::MYSQLPreparedStatement(Database& p_Database) : m_Database(p_Database)
    {
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
        MYSQL* l_Connection;
        l_Connection = mysql_init(NULL);

        if (!l_Connection)
        {
            LOG_INFO << ("Could not initialize MySQL connection to database: %s", p_Database);
            return 2000; ///< CR_UNKNOWN_ERROR
        }

        /// We handle data by utf8 - so do same for database
        mysql_options(l_Connection, MYSQL_SET_CHARSET_NAME, "utf8");

        /// Connect to database
        m_Connection = mysql_real_connect(l_Connection, p_Host.c_str(), p_Username.c_str(), p_Password.c_str(), p_Database.c_str(), p_Port, NULL, NULL);

        if (m_Connection)
        {
            /// Only do this once
            if (!s_Logged)
            {
                LOG_INFO << "MySQL Client Library: " << mysql_get_client_info();
                LOG_INFO << "MySQL Server Version: " << mysql_get_server_info(m_Connection);
                LOG_INFO << "Connected to MYSQL Database at " << m_Connection->host;

                s_Logged = true;
            }

            /// Set up prepare statements
            for (uint32 l_I = 0; l_I < MAX_PREPARED_STATEMENTS; l_I++)
                m_Statements[l_I] = new PreparedStatement(this);

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
    /// @p_Query : Query which will be executed to database
    bool MYSQLPreparedStatement::Prepare(PreparedStatement* p_StatementHolder)
    {
        std::unique_lock<std::mutex> l_Guard(m_Mutex);

        p_StatementHolder->m_Stmt = mysql_stmt_init(m_Connection);

        if (!p_StatementHolder->m_Stmt)
        {
            LOG_ERROR << "mysql_stmt_init: " << mysql_error(m_Connection);
            return true;
        }

        /// Set buffer max value
        bool l_Temp = true;
        mysql_stmt_attr_set(p_StatementHolder->m_Stmt, STMT_ATTR_UPDATE_MAX_LENGTH, &l_Temp);

        if (mysql_stmt_prepare(p_StatementHolder->m_Stmt, p_StatementHolder->m_Query.c_str(), p_StatementHolder->m_Query.length()))
        {
            LOG_ERROR << "mysql_stmt_prepare: " << mysql_error(m_Connection) << " ON " << p_StatementHolder->m_Query;
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

    /// Execute
    /// @p_Stmt : Statement we are executing
    /// @p_Result : Result set
    /// @p_FieldCount : How many columns
    bool MYSQLPreparedStatement::Execute(MYSQL_STMT* p_Stmt, MYSQL_RES ** p_Result, uint32 * p_FieldCount)
    {
        std::unique_lock<std::mutex> l_Guard(m_Mutex);

        if (mysql_stmt_execute(p_Stmt))
        {
            LOG_ERROR << "mysql_stmt_execute: " << mysql_stmt_error(p_Stmt);
            return false;
        }

        *p_Result = mysql_stmt_result_metadata(p_Stmt);
        *p_FieldCount = mysql_stmt_field_count(p_Stmt);

        return true;
    }
}