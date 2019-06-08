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
#include "Common/SharedDefines.h"
#include "BindData.h"
#endif /* !_DATABASE_RESULT_h */

namespace SteerStone
{
    /// MySQLTypeToFieldType
    /// Convert MySQL type to Field Type
    /// @p_Type : MySQL Type we are converting
    static FieldType MySQLTypeToFieldType(enum_field_types p_Type, bool p_UnSigned)
    {
        switch (p_Type)
        {
        case enum_field_types::MYSQL_TYPE_NULL:
            return FieldType::FIELD_NONE;
        case enum_field_types::MYSQL_TYPE_TINY:
        {
            if (p_UnSigned)
                return FieldType::FIELD_UI8;
            else
                return FieldType::FIELD_I8;
        }
        case enum_field_types::MYSQL_TYPE_YEAR:
        case enum_field_types::MYSQL_TYPE_SHORT:
        {
            if (p_UnSigned)
                return FieldType::FIELD_I16;
            else
                return FieldType::FIELD_UI16;
        }
        case enum_field_types::MYSQL_TYPE_INT24:
        case enum_field_types::MYSQL_TYPE_LONG:
        {
            if (p_UnSigned)
                return FieldType::FIELD_I32;
            else
                return FieldType::FIELD_UI32;
        }
        case enum_field_types::MYSQL_TYPE_LONGLONG:
        case enum_field_types::MYSQL_TYPE_BIT:
        {
            if (p_UnSigned)
                return FieldType::FIELD_UI64;
            else
                return FieldType::FIELD_UI64;
        }
        case enum_field_types::MYSQL_TYPE_FLOAT:
            return FieldType::FIELD_FLOAT;
        case enum_field_types::MYSQL_TYPE_DOUBLE:
            return FieldType::FIELD_DOUBLE;
        case enum_field_types::MYSQL_TYPE_DECIMAL:
        case enum_field_types::MYSQL_TYPE_NEWDECIMAL:
            return FieldType::FIELD_DECIMAL;
        case enum_field_types::MYSQL_TYPE_TIMESTAMP:
        case enum_field_types::MYSQL_TYPE_DATE:
        case enum_field_types::MYSQL_TYPE_TIME:
        case enum_field_types::MYSQL_TYPE_DATETIME:
            return FieldType::FIELD_DATE;
        case enum_field_types::MYSQL_TYPE_TINY_BLOB:
        case enum_field_types::MYSQL_TYPE_MEDIUM_BLOB:
        case enum_field_types::MYSQL_TYPE_LONG_BLOB:
        case enum_field_types::MYSQL_TYPE_BLOB:
        case enum_field_types::MYSQL_TYPE_STRING:
        case enum_field_types::MYSQL_TYPE_VAR_STRING:
            return FieldType::FIELD_BINARY;
        default:
            throw std::runtime_error((std::string("MySQLTypeToFieldType: invalid MySQL type %u", uint32(p_Type))));
            break;
        }
    }

    /// FieldTypeToString
    /// Convert Field Type to string
    /// @p_FieldType : Field type we are converting to string
    static std::string FieldTypeToString(FieldType p_FieldType)
    {
        switch (p_FieldType)
        {
            case FieldType::FIELD_BOOL:
                return "FIELD_BOOl";
            case FieldType::FIELD_UI8:
                return "FIELD_UI8";
            case FieldType::FIELD_UI16:
                return "FIELD_UI16";
            case FieldType::FIELD_UI32:
                return "FIELD_UI32";      
            case FieldType::FIELD_UI64:
                return "FIELD_UI64";   
            case FieldType::FIELD_I8:
                return "FIELD_I8"; 
            case FieldType::FIELD_I16:
                return "FIELD_I16";      
            case FieldType::FIELD_I32:
                return "FIELD_I32";            
            case FieldType::FIELD_I64:
                return "FIELD_I64";         
            case FieldType::FIELD_FLOAT:
                return "FIELD_FLOAT";
            case FieldType::FIELD_DOUBLE:
                return "FIELD_DOUBLE";         
            case FieldType::FIELD_DECIMAL:
                return "FIELD_DECIMAL";      
            case FieldType::FIELD_DATE:
                return "FIELD_DATE";       
            case FieldType::FIELD_BINARY:
                return "FIELD_BINARY";     
            case FieldType::FIELD_STRING:
                return "FIELD_STRING";
            default:
                throw std::runtime_error(std::string("FieldTypeToString: Unknown Field Type %u", uint32(p_FieldType)));
                break;
        }
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

            if (Data.Type != FieldType::FIELD_FLOAT)
                LOG_WARNING << "Converting " << FieldTypeToString(Data.Type) << " to FIELD_FLOAT";

            if (Data.Raw)
                return *reinterpret_cast<float*>(Data.Buffer);
            return static_cast<float>(atof((char*)Data.Buffer));
        }

        double GetDouble() const
        {
            if (!Data.Buffer)
                return 0.0f;

            if (Data.Type != FieldType::FIELD_DOUBLE)
                LOG_WARNING << "Converting " << FieldTypeToString(Data.Type) << " to FIELD_DOUBLE";

            if (Data.Raw)
                return *reinterpret_cast<double*>(Data.Buffer);
            return static_cast<double>(atof((char*)Data.Buffer));
        }

        char const* GetChar() const
        {
            if (!Data.Buffer)
                return NULL;

            if (Data.Type != FieldType::FIELD_BINARY)
                LOG_WARNING << "Converting " << FieldTypeToString(Data.Type) << " to FIELD_STRING";

            return static_cast<char const*>(Data.Buffer);
        }

        std::string GetString() const
        {
            if (!Data.Buffer)
                return "";

            if (Data.Type != FieldType::FIELD_BINARY)
                LOG_WARNING << "Converting " << FieldTypeToString(Data.Type) << " to FIELD_STRING";

            char const* l_String = GetChar();
            if (!l_String)
                return "";

            return std::string((char*)Data.Buffer, Data.Length);
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

            if (Data.Type != FieldType::FIELD_I8)
                LOG_WARNING << "Converting " << FieldTypeToString(Data.Type) << " to FIELD_I8";

            if (Data.Raw)
                return *reinterpret_cast<int8*>(Data.Buffer);
            return static_cast<int8>(strtol((char*)Data.Buffer, nullptr, 10));
        }

        int16 GetInt16() const
        {
            if (!Data.Buffer)
                return 0;

            if (Data.Type != FieldType::FIELD_I16)
                LOG_WARNING << "Converting " << FieldTypeToString(Data.Type) << " to FIELD_I16";

            if (Data.Raw)
                return *reinterpret_cast<int16*>(Data.Buffer);
            return static_cast<int16>(strtol((char*)Data.Buffer, nullptr, 10));
        }

        int32 GetInt32() const
        {
            if (!Data.Buffer)
                return 0;

            if (Data.Type != FieldType::FIELD_I32)
                LOG_WARNING << "Converting " << FieldTypeToString(Data.Type) << " to FIELD_I32";

            if (Data.Raw)
                return *reinterpret_cast<int32*>(Data.Buffer);
            return static_cast<int32>(strtol((char*)Data.Buffer, nullptr, 10));
        }

        int64 GetInt64() const
        {
            if (!Data.Buffer)
                return 0;

            if (Data.Type != FieldType::FIELD_I64)
                LOG_WARNING << "Converting " << FieldTypeToString(Data.Type) << " to FIELD_I64";

            if (Data.Raw)
                return *reinterpret_cast<int64*>(Data.Buffer);
            return static_cast<int64>(strtoll((char*)Data.Buffer, nullptr, 10));
        }

        uint8 GetUInt8() const
        {
            if (!Data.Buffer)
                return 0;

            if (Data.Type != FieldType::FIELD_UI8)
                LOG_WARNING << "Converting " << FieldTypeToString(Data.Type) << " to FIELD_UI8";

            if (Data.Raw)
                return *reinterpret_cast<uint8*>(Data.Buffer);
            return static_cast<uint8>(strtoul((char*)Data.Buffer, nullptr, 10));
        }

        uint16 GetUInt16() const
        {
            if (!Data.Buffer)
                return 0;

            if (Data.Type != FieldType::FIELD_UI16)
                LOG_WARNING << "Converting " << FieldTypeToString(Data.Type) << " to FIELD_UI16";

            if (Data.Raw)
                return *reinterpret_cast<uint16*>(Data.Buffer);
            return static_cast<uint16>(strtoul((char*)Data.Buffer, nullptr, 10));
        }

        uint32 GetUInt32() const
        {
            if (!Data.Buffer)
                return 0;

            if (Data.Type != FieldType::FIELD_UI32)
                LOG_WARNING << "Converting " << FieldTypeToString(Data.Type) << " to FIELD_UI32";

            if (Data.Raw)
                return *reinterpret_cast<uint32*>(Data.Buffer);
            return static_cast<uint32>(strtoul((char*)Data.Buffer, nullptr, 10));
        }

        uint64 GetUInt64() const
        {
            if (!Data.Buffer)
                return 0;

            if (Data.Type != FieldType::FIELD_UI64)
                LOG_WARNING << "Converting " << FieldTypeToString(Data.Type) << " to FIELD_UI64";

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