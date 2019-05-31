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

#ifndef _PREPARED_STATEMENT_PREPARED_STATEMENT_h
#define _PREPARED_STATEMENT_PREPARED_STATEMENT_h
#include "SharedDefines.h"
#include "MYSQLPreparedStatement.h"
#include <atomic>
#include <mutex>
#endif /* !_PREPARED_STATEMENT_PREPARED_STATEMENT_h */

namespace SteerStone
{
    class Database;

    class PreparedStatements
    {
    public:
        /// Constructor
        PreparedStatements();

        /// Constructor
        ~PreparedStatements();

    public:
        /// SetUp
        /// @p_Username : Name of user
        /// @p_Password : Password of user
        /// @p_Port     : Port we are connecting to
        /// @p_Host     : Address we are connecting to
        /// @p_Database : Database we are querying to
        /// @p_PoolSize : Amount of MYSQL connections we are spawning
        /// @p_DatabaseHolder : Reference of database
        uint32 SetUp(std::string const p_Username, std::string const p_Password,
            uint32 const p_Port, std::string const p_Host, std::string const p_Database, uint32 const p_PoolSize, Database& p_DatabaseHolder);

    public:
        /// ClosePrepareStatements
        /// Close all prepare statements
        void ClosePrepareStatements();

    public:
        /// GetPrepareStatement
        /// GetPrepareStatement a PrepareStatement
        PreparedStatement* GetPrepareStatement();

        /// FreePrepareStatement
        /// Release Prepare statement to be used again
        void FreePrepareStatement(PreparedStatement* p_PrepareStatement);

    private:
        std::vector<MYSQLPreparedStatement*> m_Pool;
        std::mutex m_Mutex;
        std::atomic<bool> m_ShutDown;
    };
} ///< NAMESPACE STEERSTONE
