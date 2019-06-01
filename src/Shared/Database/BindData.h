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

#ifndef _DATABASE_SQL_BIND_DATA_h
#define _DATABASE_SQL_BIND_DATA_h
#include "Common/SharedDefines.h"
#include <mysql.h>
#endif /* !_DATABASE_SQL_BIND_DATA_h */

namespace SteerStone
{
    enum FieldType
    {
        FIELD_BOOL,
        FIELD_UI8,
        FIELD_UI16,
        FIELD_UI32,
        FIELD_UI64,
        FIELD_I8,
        FIELD_I16,
        FIELD_I32,
        FIELD_I64,
        FIELD_FLOAT,
        FIELD_DOUBLE,
        FIELD_DECIMAL,
        FIELD_DATE,
        FIELD_BINARY,
        FIELD_STRING,
        FIELD_NONE
    };

    union DataType
    {
        bool Boolean;
        uint8 Uint8;
        int8 Int8;
        uint16 Uint16;
        int16 Int16;
        uint32 Uint32;
        int32 Int32;
        uint64 Uint64;
        int64 Int64;
        float Float;
        double Double;
    };

    class SQLBindData
    {
    public:
        template<typename T>
        SQLBindData(T p_Data) { Set(p_Data); }

    public:
        enum_field_types GetFieldType(uint8& p_Unsigned) const
        {
            enum_field_types l_DataType = enum_field_types::MYSQL_TYPE_NULL;
            switch (m_Type)
            {
            case FieldType::FIELD_NONE:   l_DataType = enum_field_types::MYSQL_TYPE_NULL;                     break;
            case FieldType::FIELD_BOOL:   l_DataType = enum_field_types::MYSQL_TYPE_BIT;                      break;
            case FieldType::FIELD_UI8:    l_DataType = enum_field_types::MYSQL_TYPE_TINY;     p_Unsigned = 1; break;
            case FieldType::FIELD_UI16:   l_DataType = enum_field_types::MYSQL_TYPE_SHORT;    p_Unsigned = 1; break;
            case FieldType::FIELD_UI32:   l_DataType = enum_field_types::MYSQL_TYPE_LONG;     p_Unsigned = 1; break;
            case FieldType::FIELD_UI64:   l_DataType = enum_field_types::MYSQL_TYPE_LONGLONG; p_Unsigned = 1; break;
            case FieldType::FIELD_I8:     l_DataType = enum_field_types::MYSQL_TYPE_TINY;                     break;
            case FieldType::FIELD_I16:    l_DataType = enum_field_types::MYSQL_TYPE_SHORT;                    break;
            case FieldType::FIELD_I32:    l_DataType = enum_field_types::MYSQL_TYPE_LONG;                     break;
            case FieldType::FIELD_I64:    l_DataType = enum_field_types::MYSQL_TYPE_LONGLONG;                 break;
            case FieldType::FIELD_FLOAT:  l_DataType = enum_field_types::MYSQL_TYPE_FLOAT;                    break;
            case FieldType::FIELD_DOUBLE: l_DataType = enum_field_types::MYSQL_TYPE_DOUBLE;                   break;
            case FieldType::FIELD_STRING: l_DataType = enum_field_types::MYSQL_TYPE_STRING;                   break;
            }

            return l_DataType;
        }

        std::size_t GetSize() const
        {
            switch (m_Type)
            {
            case FieldType::FIELD_NONE:    return 0;
            case FieldType::FIELD_BOOL:    return sizeof(bool);
            case FieldType::FIELD_UI8:     return sizeof(uint8);
            case FieldType::FIELD_UI16:    return sizeof(uint16);
            case FieldType::FIELD_UI32:    return sizeof(uint32);
            case FieldType::FIELD_UI64:    return sizeof(uint64);
            case FieldType::FIELD_I8:      return sizeof(int8);
            case FieldType::FIELD_I16:     return sizeof(int16);
            case FieldType::FIELD_I32:     return sizeof(int32);
            case FieldType::FIELD_I64:     return sizeof(int64);
            case FieldType::FIELD_FLOAT:   return sizeof(float);
            case FieldType::FIELD_DOUBLE:  return sizeof(double);
            case FieldType::FIELD_STRING:  return m_StringData.length();

            default:
                throw std::runtime_error("unrecognized type of SqlStmtFieldType obtained");
            }
        }

        void* GetBuffer() const
        {
            return m_Type == FIELD_STRING ? (void*)m_StringData.c_str() : (void*)&m_BinaryData;
        }

    public:
        inline void Set(bool p_Data)        { m_Type = FieldType::FIELD_BOOL;   m_BinaryData.Boolean = p_Data; }
        inline void Set(uint8 p_Data)       { m_Type = FieldType::FIELD_UI8;    m_BinaryData.Uint8 = p_Data; }
        inline void Set(uint16 p_Data)      { m_Type = FieldType::FIELD_UI16;   m_BinaryData.Uint16 = p_Data; }
        inline void Set(uint32 p_Data)      { m_Type = FieldType::FIELD_UI32;   m_BinaryData.Uint32 = p_Data; }
        inline void Set(uint64 p_Data)      { m_Type = FieldType::FIELD_UI64;   m_BinaryData.Uint64 = p_Data; }
        inline void Set(int8 p_Data)        { m_Type = FieldType::FIELD_I8;     m_BinaryData.Int8 = p_Data; }
        inline void Set(int16 p_Data)       { m_Type = FieldType::FIELD_I16;    m_BinaryData.Int16 = p_Data; }
        inline void Set(int32 p_Data)       { m_Type = FieldType::FIELD_I32;    m_BinaryData.Int32 = p_Data; }
        inline void Set(int64 p_Data)       { m_Type = FieldType::FIELD_I64;    m_BinaryData.Int64 = p_Data; }
        inline void Set(float p_Data)       { m_Type = FieldType::FIELD_FLOAT;  m_BinaryData.Float = p_Data; }
        inline void Set(double p_Data)      { m_Type = FieldType::FIELD_DOUBLE; m_BinaryData.Double = p_Data; }
        inline void Set(std::string p_Data) { m_Type = FieldType::FIELD_STRING; m_StringData = p_Data; }

    private:
        FieldType m_Type;
        DataType m_BinaryData;
        std::string m_StringData;
    };
} ///< NAMESPACE STEERSTONE