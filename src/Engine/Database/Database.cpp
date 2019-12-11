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
#include "Database/SQLCommon.hpp"
#include "Utility/UtiString.hpp"
#include "Threading/ThrTaskManager.hpp"

namespace SteerStone { namespace Core { namespace Database {

    /// Constructor
    Base::Base()
    {
    }

    /// Deconstructor
    Base::~Base()
    {
        m_Workers.clear();
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Start Database
    /// @p_InfoString : Database user details; username, password, host, database, l_Port
    /// @p_PoolSize : How many pool connections database will launch
    /// @p_WorkerThreads : Amount of workers to spawn
    bool Base::Start(char const* p_InfoString, uint32 p_PoolSize, uint32 p_WorkerThreads)
    {
        /// Check if pool size is within our requirements
        if (p_PoolSize < MIN_CONNECTION_POOL_SIZE)
            p_PoolSize = MIN_CONNECTION_POOL_SIZE;
        else if (p_PoolSize > MAX_CONNECTION_POOL_SIZE)
            p_PoolSize = MAX_CONNECTION_POOL_SIZE;

        std::string l_Username;
        std::string l_Password;
        std::string l_Database;
        std::string l_Host;
        std::string l_Port;

        std::vector<std::string> l_Tokens = Utils::SplitAll(p_InfoString, ";", false);

        auto& l_Itr = l_Tokens.begin();

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

        if (!Connect(l_Username, l_Password, std::stoi(l_Port), l_Host, l_Database, p_PoolSize, this))
        {
            for (uint8 l_I = 0; l_I < p_WorkerThreads; l_I++)
                m_Workers.push_back(std::make_unique<DatabaseWorker>(l_I));

            return true;
        }
        else
            return false;
    }

    /// Returns a Prepare Statement from Pool
    PreparedStatement* Base::GetPrepareStatement()
    {
       return Prepare();
    }
    /// @p_PreparedStatement : Connection we are freeing
    void Base::FreePrepareStatement(PreparedStatement* p_PreparedStatement)
    {
        /// This is handled automatically, but I'll leave this for now
        Free(p_PreparedStatement);
    }

    /// Execute query on worker thread
    /// @p_PrepareStatementHolder : PrepareStatement which will be executed on database worker thread
    CallBackOperator Base::PrepareOperator(PreparedStatement* p_PrepareStatementHolder)
    {
        /// PrepareStatement keeps reference of MYSQLConnection -- keep note
        PrepareStatementOperator* l_PrepareStatementOperator = new PrepareStatementOperator(p_PrepareStatementHolder);

        /// Keep reference of statement operator to execute query
        EnqueueOperator(l_PrepareStatementOperator);

        /// Return our CallBackOperator which gets the result from database worker thread
        return CallBackOperator(std::move(l_PrepareStatementOperator->GetFuture()));
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Pass operator to worker thread
    /// @p_Operator : Operator we are adding to be processed on database worker thread
    void Base::EnqueueOperator(Operator* p_Operator)
    {
        auto l_Worker = SelectWorker();

        l_Worker->AddOperator(p_Operator);
    }

    /// Select the worker with lowest storage size (equal distrubition)
    DatabaseWorker* Base::SelectWorker() const
    {
        std::size_t l_MinimumSize = m_Workers.size();
        int32 l_Index = 0;

        for (std::size_t l_I = 0; l_I < m_Workers.size(); l_I++)
        {
            const std::size_t l_Size = m_Workers[l_I]->GetSize();

            if (l_Size < l_MinimumSize)
            {
                l_MinimumSize = l_Size;
                l_Index = l_I;
            }
        }

        return m_Workers[l_Index].get();
    }

}   ///< namespace Database
}   ///< namespace Core
}   ///< namespace SteerStone