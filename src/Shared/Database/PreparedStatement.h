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

#ifndef _PREPARED_STATEMENT_PREPARED_STATEMENT_HOLDER_h
#define _PREPARED_STATEMENT_PREPARED_STATEMENT_HOLDER_h
#include "SharedDefines.h"
#include "OperatorProcessor.h"
#include <mutex>
#endif /* !_PREPARED_STATEMENT_PREPARED_STATEMENT_HOLDER_h */

namespace SteerStone
{
    class MYSQLPreparedStatement;

    class PreparedStatement
    {
    public:
        friend class MYSQLPreparedStatement;

    public:
        /// Constructor
        /// @p_MYSQLPreparedStatement : Keep reference of our connection
        PreparedStatement(MYSQLPreparedStatement* p_MySQLPreparedStatement);

        /// Deconstructor
        ~PreparedStatement();

    public:
        /// GetStatement
        /// Returns MYSQL Statement
        MYSQL_STMT* GetStatement() const { return m_Stmt; }

        /// TryLock
        /// Attempt to lock the object
        bool TryLock();

        /// Unlock
        /// Allow the prepare statement to be accessed again
        void Unlock();

    public:
        /// Prepare the statement
        /// @p_Query : Query which will be executed to database
        void PrepareStatement(char const* p_Query);

        /// ExecuteStatement
        /// Execute the statement
        PreparedResultSet* ExecuteStatement();

    public:
        /// Set our prepared values
        void SetBool(uint8 p_Index, bool p_Value)          { m_Binds.push_back(std::make_pair(p_Index, SQLBindData(p_Value))); }
        void SetUint8(uint8 p_Index, uint8 p_Value)        { m_Binds.push_back(std::make_pair(p_Index, SQLBindData(p_Value))); }
        void SetUint16(uint8 p_Index, uint16 p_Value)      { m_Binds.push_back(std::make_pair(p_Index, SQLBindData(p_Value))); }
        void SetUint32(uint8 p_Index, uint32 p_Value)      { m_Binds.push_back(std::make_pair(p_Index, SQLBindData(p_Value))); }
        void SetUint64(uint8 p_Index, uint64 p_Value)      { m_Binds.push_back(std::make_pair(p_Index, SQLBindData(p_Value))); }
        void SetInt8(uint8 p_Index, int8 p_Value)          { m_Binds.push_back(std::make_pair(p_Index, SQLBindData(p_Value))); }
        void SetInt16(uint8 p_Index, int16 p_Value)        { m_Binds.push_back(std::make_pair(p_Index, SQLBindData(p_Value))); }
        void SetInt32(uint8 p_Index, int32 p_Value)        { m_Binds.push_back(std::make_pair(p_Index, SQLBindData(p_Value))); }
        void SetInt64(uint8 p_Index, int64 p_Value)        { m_Binds.push_back(std::make_pair(p_Index, SQLBindData(p_Value))); }
        void SetFloat(uint8 p_Index, float p_Value)        { m_Binds.push_back(std::make_pair(p_Index, SQLBindData(p_Value))); }
        void SetDouble(uint8 p_Index, double p_Value)      { m_Binds.push_back(std::make_pair(p_Index, SQLBindData(p_Value))); }
        void SetString(uint8 p_Index, std::string p_Value) { m_Binds.push_back(std::make_pair(p_Index, SQLBindData(p_Value))); }

    private:
        /// Prepare
        /// Prepare the query
        /// @p_Query : Query which will be executed to database
        bool Prepare(char const * p_Query);

        /// Execute
        /// @p_Result : Result set
        /// @p_Fields : Fields
        /// @p_FieldCount : How many columns
        bool Execute(MYSQL_RES** p_Result, MYSQL_FIELD** p_Fields, uint32* p_FieldCount);

        /// BindParameters
        /// Bind parameters from storage into SQL
        void BindParameters();

        /// RemoveBinds
        /// Remove previous binds
        void RemoveBinds();

    private:
        MYSQLPreparedStatement* m_MySQLPreparedStatement;

    private:
        MYSQL_STMT* m_Stmt;
        MYSQL_BIND* m_Bind;
        uint32 m_ParametersCount;
        std::string m_Query;
        bool m_PrepareError;
        std::vector<std::pair<uint8, SQLBindData>> m_Binds;
        std::mutex m_Mutex; ///< We use this to prevent other uses trying to access a already accessed object
    };

} ///< NAMESPACE STEERSTONE