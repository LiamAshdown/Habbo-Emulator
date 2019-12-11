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
#include "Database/PreparedStatement.hpp"
#include "Database/SQLCommon.hpp"
#include "Utility/UtiLockable.hpp"
#include "Utility/UtiObjectGuard.hpp"

namespace SteerStone { namespace Core { namespace Database {

    class Base;

    class MYSQLPreparedStatement : public std::enable_shared_from_this<MYSQLPreparedStatement>, private Utils::LockableReadWrite
    {
        /// Allow access to lock / unlock methods
        friend class Utils::ObjectGuard<MYSQLPreparedStatement>;
        friend class Utils::ObjectReadGuard<MYSQLPreparedStatement>;
        friend class Utils::ObjectWriteGuard<MYSQLPreparedStatement>;

        friend class PreparedStatements;

        //////////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////////

    public:
        /// Constructor
        /// @p_Base : Database
        MYSQLPreparedStatement(Base* p_Base);
        /// Deconstructor
        ~MYSQLPreparedStatement();

        //////////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////////

        /// Connect to the database
        /// @p_Username : Name of user
        /// @p_Password : Password of user
        /// @p_Port     : Port we are connecting to
        /// @p_Host     : Address we are connecting to
        /// @p_Database : Database we are querying to
        uint32 Connect(std::string const p_Username, std::string const p_Password,
            uint32 const p_Port, std::string const p_Host, std::string const p_Database);

        /// Prepare the statement
        /// @p_StatementHolder : Statement being prepared
        bool Prepare(PreparedStatement* p_StatementHolder);
        /// Execute the statement
        /// @p_Stmt : Statement being executed
        /// @p_Result : Result set
        /// @p_FieldCount : Field count
        bool Execute(MYSQL_STMT* p_Stmt, MYSQL_RES ** p_Result, uint32* p_FieldCount);

        /// Returns database
        Base* GetDatabase() const;

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    private:
        MYSQL* m_Connection;                                       ///< MYSQL Connection
        PreparedStatement* m_Statements[MAX_PREPARED_STATEMENTS];  ///< Prepared Statements storage
        Base* m_Base;                                              ///< Database
    };

}   ///< namespace Database
}   ///< namespace Core
}   ///< namespace SteerStone