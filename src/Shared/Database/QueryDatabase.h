/*
* Liam Ashdown
* Copyright(C) 2019
*
* This program is free software : you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
*(at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
*but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.If not, see < http://www.gnu.org/licenses/>.
*/

#ifndef _DATABASE_QUERY_DATABASE_h
#define _DATABASE_QUERY_DATABASE_h
#include "../Common/SharedDefines.h"
#include "Database.h"
#include "Result.h"
#endif /* _DATABASE_QUERY_DATABASE_h */

namespace SteerStone
{
    /// Query the database
    class QueryDatabase
    {
    public:
        /// Constructor
        /// @p_Database : Database we are going to query
       explicit QueryDatabase(std::string const& p_Database);
       /// Deconstructor
        ~QueryDatabase();

    public:
        /// DirectExecuteQuery
        /// @p_Query : Query which will be executed to the database
        void DirectExecuteQuery(std::string const p_Query);

        /// PrepareQuery
        /// @p_Query : Query which will be prepared to be executed into the database
        void PrepareQuery(std::string const& p_Query);

        /// ExecuteQuery
        /// Execute Prepare query into database
        void ExecuteQuery();

        /// GetResult
        /// Returns if query is successful or not
        bool GetResult();

        /// GetStatement - Get Statement of query
        std::shared_ptr<sql::PreparedStatement>& GetStatement();

        /// ClearParameters 
        /// Reset our connection, this is used when we want to use the same object
        /// and want to query multiple times
        void ClearParameters();

        /// Fetch
        /// Return resultset fromq query
        Result* Fetch();

    private:
        /// Database
        std::shared_ptr<Connection> m_Connection;                                  ///< Database Connection
        std::shared_ptr<sql::Connection> m_SqlConnection;                          ///< Database Sql Connection
        std::shared_ptr<sql::Statement> m_Statement;                               ///< Database Statement
        std::shared_ptr<sql::PreparedStatement> m_PreparedStatement;               ///< Database PreparedStatement
        std::unique_ptr<sql::ResultSet> m_ResultSet;                               ///< ResultSet from query

        /// Variables
        Result m_Result;                                                           ///< ResultSet which will be passed into our Result Class
        std::string m_Database;                                                    ///< Database name we are going to query
        bool m_ExecuteResult;                                                      ///< If DirectExecuteQuery was used
        bool m_IsExecuteResult;                                                    ///< If DirectExecuteQuery was used
    }; 
} ///< NAMESPACE STEERSTONE
