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

#pragma once
#include "DatabaseWorker.hpp"
#include "Database/PreparedStatements.hpp"

namespace SteerStone { namespace Core { namespace Database {

    class Base : private PreparedStatements
    {
        DISALLOW_COPY_AND_ASSIGN(Base);

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    public:
        /// Constructor
        Base();
        /// Deconstructor
        ~Base();

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

        /// Start Database
        /// @p_InfoString : Database user details; username, password, host, database, l_Port
        /// @p_PoolSize : How many pool connections database will launch
        /// @p_WorkerThreads : Amount of workers to spawn
        bool Start(char const* p_InfoString, uint32 p_PoolSize, uint32 p_WorkerThreads);

        /// Returns a Prepare Statement from Pool
        PreparedStatement* GetPrepareStatement();
        /// Free Prepare Statement
        /// @p_PreparedStatement : Connection we are freeing
        void FreePrepareStatement(PreparedStatement* p_PreparedStatement);

        /// Execute query on worker thread
        /// @p_PrepareStatementHolder : PrepareStatement which will be executed on database worker thread
        CallBackOperator PrepareOperator(PreparedStatement* p_PrepareStatementHolder);

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    private:
        /// Pass operator to worker thread
        /// @p_Operator : Operator we are adding to be processed on database worker thread
        void EnqueueOperator(Operator* p_Operator);

        /// Select the worker with lowest storage size (equal distrubition)
        DatabaseWorker* SelectWorker() const;

    private:
        std::vector<std::unique_ptr<DatabaseWorker>> m_Workers;
    };

}   ///< namespace Database
}   ///< namespace Core
}   ///< namespace SteerStone
