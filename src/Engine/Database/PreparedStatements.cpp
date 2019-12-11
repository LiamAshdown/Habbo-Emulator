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
#include "Logger/Base.hpp"
#include "SQLCommon.hpp"

namespace SteerStone { namespace Core { namespace Database {

    /// Constructor
    PreparedStatements::PreparedStatements()
    {
    }
    /// Deconstructor
    PreparedStatements::~PreparedStatements()
    {
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// @p_Username : Name of user
    /// @p_Password : Password of user
    /// @p_Port     : Port we are connecting to
    /// @p_Host     : Address we are connecting to
    /// @p_Database : Database we are querying to
    /// @p_PoolSize : Amount of MYSQL connections we are spawning
    /// @p_Base     : Database
    uint32 PreparedStatements::Connect(std::string const p_Username, std::string const p_Password, uint32 const p_Port, std::string const p_Host, 
        std::string const p_Database, uint32 const p_PoolSize, Base* p_Base)
    {
        for (uint32 l_I = 0; l_I < p_PoolSize; l_I++)
        {
            std::shared_ptr<MYSQLPreparedStatement> l_PreparedStatement = std::make_shared<MYSQLPreparedStatement>(p_Base);

            uint32 l_Success = l_PreparedStatement->Connect(p_Username, p_Password, p_Port, p_Host, p_Database);
            
            /// If > 0 - Failed to connect
            if (l_Success)
            {
                LOG_ERROR("Database", "Failed to create connection. MySQL Error: %0. Please refer to MYSQL Documentation.", l_Success);
                return l_Success;
            }

            m_ConnectionPool.push_back(l_PreparedStatement);
        }

        return 0;
    }

    /// Attempt to get a Prepared Statement
    PreparedStatement * PreparedStatements::Prepare()
    {
        for(;;)
        {
            for (auto l_Itr : m_ConnectionPool)
            {
                std::shared_ptr<MYSQLPreparedStatement> l_PreparedStatement = l_Itr;

                for (uint32 l_I = 0; l_I < MAX_PREPARED_STATEMENTS; l_I++)
                {
                    PreparedStatement* l_PrepareStatement = l_PreparedStatement->m_Statements[l_I];

                    if (l_PrepareStatement->TryLockMutex())
                        return l_PrepareStatement;
                }
            }

            LOG_WARNING("PreparedStatements", "Could not get a prepare statement... trying again!");
        }

        return nullptr;
    }
    /// Release Prepare statement to be used again
    void PreparedStatements::Free(PreparedStatement* p_PrepareStatement)
    {
        p_PrepareStatement->UnlockMutex();
    }

}   ///< namespace Database
}   ///< namespace Core
}   ///< namespace SteerStone
