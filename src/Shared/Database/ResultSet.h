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

#ifndef _DATABASE_RESULT_h
#define _DATABASE_RESULT_h
#include "SharedDefines.h"
#include "BindData.h"
#endif /* !_DATABASE_RESULT_h */

namespace SteerStone
{
    static FieldType MysqlTypeToFieldType(enum_field_types type)
    {
        switch (type)
        {
        case MYSQL_TYPE_NULL:
            return FieldType::FIELD_NONE;
        case MYSQL_TYPE_TINY:
            return FieldType::FIELD_I8;
        case MYSQL_TYPE_YEAR:
        case MYSQL_TYPE_SHORT:
            return FieldType::FIELD_I16;
        case MYSQL_TYPE_INT24:
        case MYSQL_TYPE_LONG:
            return FieldType::FIELD_I32;
        case MYSQL_TYPE_LONGLONG:
        case MYSQL_TYPE_BIT:
            return FieldType::FIELD_I64;
        case MYSQL_TYPE_FLOAT:
            return FieldType::FIELD_FLOAT;
        case MYSQL_TYPE_DOUBLE:
            return FieldType::FIELD_DOUBLE;
        case MYSQL_TYPE_DECIMAL:
        case MYSQL_TYPE_NEWDECIMAL:
            return FieldType::FIELD_DECIMAL;
        case MYSQL_TYPE_TIMESTAMP:
        case MYSQL_TYPE_DATE:
        case MYSQL_TYPE_TIME:
        case MYSQL_TYPE_DATETIME:
            return FieldType::FIELD_DATE;
        case MYSQL_TYPE_TINY_BLOB:
        case MYSQL_TYPE_MEDIUM_BLOB:
        case MYSQL_TYPE_LONG_BLOB:
        case MYSQL_TYPE_BLOB:
        case MYSQL_TYPE_STRING:
        case MYSQL_TYPE_VAR_STRING:
            return FieldType::FIELD_BINARY;
        default:
            LOG_INFO << ("MysqlTypeToFieldType(): invalid field type %u", uint32(type));
            break;
        }

        return FieldType::FIELD_NONE;
    }

    class ResultSet
    {
    public:
        /// Constructor
        ResultSet()
        {
            Data.Buffer = NULL;
            Data.Type = FieldType::FIELD_NONE;
            Data.Length = 0;
            Data.Raw = false;
        }

        /// Deconstructor
        ~ResultSet(){}

    public:
        void SetValue(void* p_Value, FieldType p_FieldType, uint32 p_Length) 
        { 
            Data.Buffer = p_Value;
            Data.Length = p_Length;
            Data.Type = p_FieldType;
            Data.Raw = true;
        }

        float GetFloat() const
        {
            if (!Data.Buffer)
                return 0.0f;

            if (Data.Raw)
                return *reinterpret_cast<float*>(Data.Buffer);
            return static_cast<float>(atof((char*)Data.Buffer));
        }

        double GetDouble() const
        {
            if (!Data.Buffer)
                return 0.0f;

            if (Data.Raw)
                return *reinterpret_cast<double*>(Data.Buffer);
            return static_cast<double>(atof((char*)Data.Buffer));
        }

        char const* GetChar() const
        {
            if (!Data.Buffer)
                return NULL;

            return static_cast<char const*>(Data.Buffer);
        }

        std::string GetString() const
        {
            if (!Data.Buffer)
                return "";

            char const* l_String = GetChar();
            if (!l_String)
                return "";

            return std::string(l_String, Data.Length);
        }

        bool GetBool() const
        {
            if (!Data.Buffer)
                return false;

            int8 m_Bool = GetInt8();

            if (m_Bool == 1)
                return true;
            else
                return false;
        }

        int8 GetInt8() const 
        { 
            if (!Data.Buffer)
                return 0;

            if (Data.Raw)
                return *reinterpret_cast<int8*>(Data.Buffer);
            return static_cast<int8>(strtol((char*)Data.Buffer, nullptr, 10));
        }

        int16 GetInt16() const
        {
            if (!Data.Buffer)
                return 0;

            if (Data.Raw)
                return *reinterpret_cast<int16*>(Data.Buffer);
            return static_cast<int16>(strtol((char*)Data.Buffer, nullptr, 10));
        }

        int32 GetInt32() const
        {
            if (!Data.Buffer)
                return 0;

            if (Data.Raw)
                return *reinterpret_cast<int32*>(Data.Buffer);
            return static_cast<int32>(strtol((char*)Data.Buffer, nullptr, 10));
        }

        int64 GetInt64() const
        {
            if (!Data.Buffer)
                return 0;

            if (Data.Raw)
                return *reinterpret_cast<int64*>(Data.Buffer);
            return static_cast<int64>(strtoll((char*)Data.Buffer, nullptr, 10));
        }

        uint8 GetUInt8() const
        {
            if (!Data.Buffer)
                return 0;

            if (Data.Raw)
                return *reinterpret_cast<uint8*>(Data.Buffer);
            return static_cast<uint8>(strtoul((char*)Data.Buffer, nullptr, 10));
        }

        uint16 GetUInt16() const
        {
            if (!Data.Buffer)
                return 0;

            if (Data.Raw)
                return *reinterpret_cast<uint16*>(Data.Buffer);
            return static_cast<uint16>(strtoul((char*)Data.Buffer, nullptr, 10));
        }

        uint32 GetUInt32() const
        {
            if (!Data.Buffer)
                return 0;

            if (Data.Raw)
                return *reinterpret_cast<uint32*>(Data.Buffer);
            return static_cast<uint32>(strtoul((char*)Data.Buffer, nullptr, 10));
        }

        uint64 GetUInt64() const
        {
            if (!Data.Buffer)
                return 0;

            if (Data.Raw)
                return *reinterpret_cast<uint64*>(Data.Buffer);
            return static_cast<uint64>(strtoull((char*)Data.Buffer, nullptr, 10));
        }

        private:
#pragma pack(push, 1)
            struct
            {
                uint32 Length;
                void* Buffer;
                FieldType Type;
                bool Raw;
            }Data;
#pragma pack(pop)

    private:
        char const* m_Value;
    };
} ///< NAMESPACE STEERSTONE