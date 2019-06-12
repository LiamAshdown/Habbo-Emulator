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

#ifndef _DATABASE_DATABASE_h
#define _DATABASE_DATABASE_h
#include "Common/SharedDefines.h"
#include "PreparedStatements.h"
#include "DatabaseWorker.h"
#include <mutex>
#endif /* !_DATABASE_DATABASE_h */

namespace SteerStone
{
    class Database
    {
    public:
        /// Constructor
        Database();

        /// Deconstructor
        ~Database();

    public:
        /// StartUp
        /// @p_InfoString : Database user details; username, password, host, database, l_Port
        /// @p_PoolSize : How many pool connections database will launch
        uint32 StartUp(char const* p_InfoString, uint32 const p_PoolSize);

    public:
        /// ShutDown
        /// Shutdown all connections
        void ShutDown();

        /// GetDatabasename 
        /// Return database name
        std::string GetName() const { return m_Database; }

    public:
        /// GetPreparedStatement
        /// Returns a Prepare Statement from Pool
        PreparedStatement* GetPrepareStatement();

        /// FreePrepareStatement
        /// Free Prepare Statement
        /// @p_PreparedStatement : Connection we are freeing
        void FreePrepareStatement(PreparedStatement* p_PreparedStatement);

    public:
        /// PrepareOperator
        /// Execute query on worker thread
        /// @p_PrepareStatementHolder : PrepareStatement which will be executed on database worker thread
        CallBackOperator PrepareOperator(PreparedStatement* p_PrepareStatementHolder);

    private:
        /// EnqueueOperator
        /// @p_Operator : Operator we are adding to be processed on database worker thread
        void EnqueueOperator(Operator* p_Operator);

        /// CloseConnections
        /// Close all MySQL connections
        void CloseConnections();

    private:
        /// Database Variables
        std::string m_Username;
        std::string m_Password;
        std::string m_Host;
        uint32 m_Port;
        std::string m_Database;
        uint32 m_PoolSize;

    private:
        PreparedStatements m_PreparedStatements;
        DatabaseWorker* m_DatabaseWorker;
    };
}

