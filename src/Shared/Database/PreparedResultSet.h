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

#ifndef _PREPARED_STATEMENT_RESULT_SET_h
#define _PREPARED_STATEMENT_RESULT_SET_h
#include "Common/SharedDefines.h"
#include "ResultSet.h"
#endif /* !_PREPARED_STATEMENT_RESULT_SET_h */

namespace SteerStone
{
    class PreparedResultSet
    {
    public:
        /// Constructor
        /// @p_Stmt : Prepare Statement
        /// @p_Result : Result
        /// @p_FieldCount : Field count
        PreparedResultSet(MYSQL_STMT* p_Stmt, MYSQL_RES* p_Result, uint32 p_FieldCount);

        /// Deconstructor
        ~PreparedResultSet();

    public:
        /// FetchResult
        /// Return result
        ResultSet* FetchResult() const;

        ResultSet const& operator[](std::size_t p_Index) const;

        /// GetNextRow
        /// High Level
        bool GetNextRow();

        /// GetRowCount
        /// Get total row count
        uint64 GetRowCount() const { return m_RowCount; }

    private:
        /// CleanUp
        /// Free Bind Memory
        void CleanUp();

        /// NextRow
        /// Get Next Row in result
        /// Low Level
        uint32 NextRow();

    private:
        MYSQL_STMT* m_Stmt;
        MYSQL_RES* m_Result;
        MYSQL_FIELD* m_Fields;
        uint64 m_RowCount;
        uint32 m_FieldCount;

    private:
        bool* m_IsNull; 
        unsigned long* m_Length;
        MYSQL_BIND* m_Bind;

    private:
        uint32 m_RowPosition;
        std::vector<ResultSet> m_Results;
    };
}