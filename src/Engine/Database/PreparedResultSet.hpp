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
#include "Database/ResultSet.hpp"

namespace SteerStone { namespace Core { namespace Database {

    class PreparedStatement;

    class PreparedResultSet
    {
        DISALLOW_COPY_AND_ASSIGN(PreparedResultSet);

        //////////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////////

    public:
        /// Constructor
        /// @p_Statement : Prepare Statement
        /// @p_Result : Result
        /// @p_FieldCount : Field count
        PreparedResultSet(PreparedStatement* p_Statement, MYSQL_RES* p_Result, uint32 p_FieldCount);
        /// Deconstructor
        ~PreparedResultSet();

        //////////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////////

    public:
        /// Return result
        ResultSet* FetchResult() const;
        /// Get Next Row
        bool GetNextRow();
        /// Get Total Row Count
        uint64 GetRowCount() const { return m_RowCount; }

        /// Get Prepare Statement
        PreparedStatement* GetPreparedStatement() { return m_PreparedStatement; }
        /// [] Operator
        ResultSet const& operator[](std::size_t p_Index) const;

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    private:
        /// Free Bind Memory
        void CleanUp();
        /// Get Next Row
        bool NextRow();

    private:
        PreparedStatement* m_PreparedStatement; ///< Prepare Statement
        MYSQL_RES* m_Result;                    ///< Result
        MYSQL_FIELD* m_Fields;                  ///< Field
        uint64 m_RowCount;                      ///< Row count
        uint32 m_FieldCount;                    ///< Field count

        my_bool* m_IsNull;                      ///< Bind Null
        unsigned long* m_Length;                ///< Bind Length
        MYSQL_BIND* m_Bind;                     ///< Bind

        uint32 m_RowPosition;                   ///< Row Position
        std::vector<ResultSet> m_Results;       ///< Result set

        bool m_FreeAutomatically;               ///< Free the prepared statement on deconstructor
    };

}   ///< namespace Database
}   ///< namespace Core
}   ///< namespace SteerStone