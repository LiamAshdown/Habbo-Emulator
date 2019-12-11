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

#include "Database/PreparedStatement.hpp"
#include "Database/SQLCommon.hpp"

#include <assert.h>

namespace SteerStone { namespace Core { namespace Database {

    /// Constructor
    /// @p_PrepareStatement : PrepareStatement
    /// @p_Stmt : Prepare Statement
    /// @p_Result : Result
    /// @p_FieldCount : Field count
    /// @p_FreeAutomatically : Free the preparedstatement on PreparedResultSet deconstructor
    PreparedResultSet::PreparedResultSet(PreparedStatement* p_Statement, MYSQL_RES* p_Result, uint32 p_FieldCount)
        : m_PreparedStatement(p_Statement), m_Result(p_Result), m_Fields(nullptr), m_Bind(nullptr), m_RowCount(0), m_FieldCount(p_FieldCount), m_RowPosition(0)
    {
        if (!m_Result)
            return;

        /// IsNull and mLength is deleted at end of constructor when buffer is loaded into our storage
        m_IsNull = new my_bool[m_FieldCount];
        m_Length = new unsigned long[m_FieldCount];
        m_Bind = new MYSQL_BIND[m_FieldCount];

        memset(m_IsNull, 0, sizeof(bool) * m_FieldCount);
        memset(m_Length, 0, sizeof(unsigned long) * m_FieldCount);
        memset(m_Bind, 0, sizeof(MYSQL_BIND) * m_FieldCount);

        if (mysql_stmt_store_result(m_PreparedStatement->GetStatement()))
        {
            LOG_ERROR("Database", "mysql_stmt_store_result: Cannot store result from MySQL Server. Error: %0"), mysql_stmt_error(m_PreparedStatement->GetStatement());            
            delete[] m_Bind;
            delete[] m_IsNull;
            delete[] m_Length;
            return;
        }

        m_Fields = mysql_fetch_fields(m_Result);

        m_RowCount = mysql_stmt_num_rows(m_PreparedStatement->GetStatement());

        std::size_t l_SizeType = 0;
        for (std::size_t l_I = 0; l_I < m_FieldCount; l_I++)
        {
            std::size_t l_Size = SizeForType(&m_Fields[l_I]);
            l_SizeType += l_Size;

            m_Bind[l_I].buffer_type = m_Fields[l_I].type;
            m_Bind[l_I].buffer_length = l_Size;
            m_Bind[l_I].length = &m_Length[l_I];
            m_Bind[l_I].is_null = &m_IsNull[l_I];
            m_Bind[l_I].error = nullptr;
            m_Bind[l_I].is_unsigned = m_Fields[l_I].flags & UNSIGNED_FLAG;
        }

        /// m_Bind handles the deletion of the buffer
        char* l_DataBuffer = new char[l_SizeType * m_RowCount];
        for (std::size_t l_I = 0, l_Offset = 0; l_I < m_FieldCount; l_I++)
        {
            m_Bind[l_I].buffer = l_DataBuffer + l_Offset;
            l_Offset += m_Bind[l_I].buffer_length;
        }

        if (mysql_stmt_bind_result(m_PreparedStatement->GetStatement(), m_Bind))
        {
            LOG_ERROR("mysql_stmt_bind_result: Cannot bind result from MySQL server. Error: %0", mysql_stmt_error(m_PreparedStatement->GetStatement()));
            mysql_stmt_free_result(m_PreparedStatement->GetStatement());
            CleanUp();
            delete[] m_IsNull;
            delete[] m_Length;
            return;
        }

        /// Buffer all rows in result
        m_Results.resize(uint32(m_RowCount) * m_FieldCount);
        while (NextRow())
        {
            for (std::size_t l_I = 0; l_I < m_FieldCount; ++l_I)
            {
                unsigned long l_BufferLength = m_Bind[l_I].buffer_length;
                unsigned long l_FetchedLength = *m_Bind[l_I].length;

                if (!*m_Bind[l_I].is_null)
                {
                    /// Retrieve our buffer
                    void* l_Buffer = m_PreparedStatement->GetStatement()->bind[l_I].buffer;

                    switch (m_Bind[l_I].buffer_type)
                    {
                    case enum_field_types::MYSQL_TYPE_TINY_BLOB:
                    case enum_field_types::MYSQL_TYPE_MEDIUM_BLOB:
                    case enum_field_types::MYSQL_TYPE_LONG_BLOB:
                    case enum_field_types::MYSQL_TYPE_BLOB:
                    case enum_field_types::MYSQL_TYPE_STRING:
                    case enum_field_types::MYSQL_TYPE_VAR_STRING:
                    {
                        /// If our Fetched length is less than buffer length then we assume
                        /// there's no null terminator, so add null terminator at end of buffer
                        if (l_FetchedLength < l_BufferLength)
                            *((char*)l_Buffer + l_FetchedLength) = '\0';
                    }
                    break;
                    default:
                        break;
                    }

                    /// Insert buffer into storage
                    m_Results[uint32(m_RowPosition) * m_FieldCount + l_I].SetValue(l_Buffer, MySQLTypeToFieldType(m_Bind[l_I].buffer_type, m_Fields[l_I].flags & UNSIGNED_FLAG ? false : true),
                        l_FetchedLength);

                    /// Move buffer pointer to next part
                    m_PreparedStatement->GetStatement()->bind[l_I].buffer = (char*)l_Buffer + l_SizeType;
                }
                else
                {
                    /// Insert buffer into storage
                    m_Results[m_RowPosition * m_FieldCount + l_I].SetValue(nullptr, MySQLTypeToFieldType(m_Bind[l_I].buffer_type, m_Fields[l_I].flags & UNSIGNED_FLAG ? false : true),
                        l_FetchedLength);
                }
            }

            m_RowPosition++;
        }

        m_RowPosition = 0;

        /// All data is buffered, let go of mysql c api structures
        mysql_stmt_free_result(m_PreparedStatement->GetStatement());
    }
    /// Deconstructor
    PreparedResultSet::~PreparedResultSet() 
    {
        CleanUp();
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Return result
    ResultSet* PreparedResultSet::FetchResult() const
    {
        LOG_ASSERT(m_RowPosition < m_RowCount, "Database", "Row Position is more than Row count!");
        return const_cast<ResultSet*>(&m_Results[m_RowPosition * m_FieldCount]);
    }
    /// [] Operator
    ResultSet const& PreparedResultSet::operator[](std::size_t p_Index) const
    {
        LOG_ASSERT(m_RowPosition < m_RowCount, "Database", "Row Position is higher than Row count!");
        LOG_ASSERT(p_Index < m_FieldCount, "Database", "Index is higher than field count!");
        return m_Results[m_RowPosition * m_FieldCount + p_Index];
    }

    /// Get Next Row
    bool PreparedResultSet::GetNextRow()
    {
        if (++m_RowPosition >= m_RowCount)
            return false;

        return true;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Free Bind Memory
    void PreparedResultSet::CleanUp()
    {
        if (m_Result)
            mysql_free_result(m_Result);

        if (m_Bind)
        {
            delete[](char*)m_Bind->buffer;
            delete[] m_Bind;
            m_Bind = nullptr;
        }
        
        m_PreparedStatement->Clear();
    }
    /// Get Next Row
    bool PreparedResultSet::NextRow()
    {
        if (m_RowPosition >= m_RowCount)
            return false;

        int32 l_Code = mysql_stmt_fetch(m_PreparedStatement->GetStatement());
        return l_Code == 0 || l_Code == MYSQL_DATA_TRUNCATED; ///< Success
    }

}   ///< namespace Database
}   ///< namespace Core
}   ///< namespace SteerStone