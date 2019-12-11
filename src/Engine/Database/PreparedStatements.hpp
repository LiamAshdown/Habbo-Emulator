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
#include <PCH/Precompiled.hpp>

#include "Core/Core.hpp"
#include "Database/MYSQLPreparedStatement.hpp"

namespace SteerStone { namespace Core { namespace Database {

    class Base;

    class PreparedStatements
    {
        DISALLOW_COPY_AND_ASSIGN(PreparedStatements);

        //////////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////////

    public:
        /// Constructor
        PreparedStatements();

        /// Constructor
        ~PreparedStatements();

        //////////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////////

        /// @p_Username : Name of user
        /// @p_Password : Password of user
        /// @p_Port     : Port we are connecting to
        /// @p_Host     : Address we are connecting to
        /// @p_Database : Database we are querying to
        /// @p_PoolSize : Amount of MYSQL connections we are spawning
        /// @p_Base     : Database
        uint32 Connect(std::string const p_Username, std::string const p_Password,
            uint32 const p_Port, std::string const p_Host, std::string const p_Database, uint32 const p_PoolSize, Base* p_Base);

        /// Attempt to get a Prepared Statement
        PreparedStatement* Prepare();
        /// FreePrepareStatement
        /// Release Prepare statement to be used again
        void Free(PreparedStatement* p_PrepareStatement);

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    private:
        std::vector<std::shared_ptr<MYSQLPreparedStatement>> m_ConnectionPool;    ///< Storage for Prepare Statements

    };

}   ///< namespace Database
}   ///< namespace Core
}   ///< namespace SteerStone
