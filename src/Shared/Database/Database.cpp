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
    /// Constructor
    Database::Database() {}

    /// Deconstructor
    Database::~Database()
    {
    }

    /// Initialize
    /// @p_Username : Name of user
    /// @p_Password : Password of user
    /// @p_Port     : Port we are connecting to
    /// @p_Host     : Address we are connecting to
    /// @p_Database : Database we are querying to
    /// @p_PoolSize : Amount of MYSQL connections we are spawning
    void Database::StartUp(std::string const p_Username, std::string const p_Password, uint32 const p_Port, std::string const p_Host, std::string const p_Database, uint32 const p_PoolSize)
    {
        /// Check if pool size is within our requirements
        if (p_PoolSize < MIN_CONNECTION_POOL_SIZE)
            m_PoolSize = MIN_CONNECTION_POOL_SIZE;
        else if (p_PoolSize > MAX_CONNECTION_POOL_SIZE)
            m_PoolSize = MAX_CONNECTION_POOL_SIZE;
        else
            m_PoolSize = p_PoolSize;

        m_PreparedStatements.SetUp(p_Username, p_Password, p_Port, p_Host, p_Database, p_PoolSize, *this);
    }

    /// ShutDown
    /// Shutdown all connections
    void Database::ShutDown()
    {
        m_DatabaseWorker.m_Queue.ShutDown();
        CloseConnections();
    }

    /// GetPreparedStatement
    /// Returns a Prepare Statement from Pool
    PreparedStatement* Database::GetPrepareStatement()
    {
       return m_PreparedStatements.GetPrepareStatement();
    }

    /// FreePrepareStatement
    /// Free Prepare Statement
    /// @p_PreparedStatement : Connection we are freeing
    void Database::FreePrepareStatement(PreparedStatement* p_PreparedStatement)
    {
        m_PreparedStatements.FreePrepareStatement(p_PreparedStatement);
    }

    /// PrepareOperator
    /// Execute query on worker thread
    /// @p_PrepareStatementHolder : PrepareStatement which will be executed on database worker thread
    CallBackOperator Database::PrepareOperator(PreparedStatement* p_PrepareStatementHolder)
    {
        /// PrepareStatement keeps reference of MYSQLConnection -- keep note
        PrepareStatementOperator* l_PrepareStatementOperator = new PrepareStatementOperator(p_PrepareStatementHolder);

        /// Keep reference of statement operator to execute query
        EnqueueOperator(l_PrepareStatementOperator);

        /// Return our CallBackOperator which gets the result from database worker thread
        return CallBackOperator(std::move(l_PrepareStatementOperator->GetFuture()));
    }

    /// EnqueueOperator
    /// @p_Operator : Operator we are adding to be processed on database worker thread
    void Database::EnqueueOperator(Operator * p_Operator)
    {
        m_DatabaseWorker.m_Queue.Push(p_Operator);
    }

    /// CloseConnections
    /// Close all MySQL connections
    void Database::CloseConnections()
    {
        m_PreparedStatements.ClosePrepareStatements();
    }
} ///< NAMESPACE STEERSTONE