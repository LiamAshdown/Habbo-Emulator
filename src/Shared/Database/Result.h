/*
* Liam Ashdown
* Copyright(C) 2019
*
* This program is free software : you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
*(at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
*but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.If not, see < http://www.gnu.org/licenses/>.
*/

#ifndef _Quad_result_h_
#define _Quad_result_h_
#include "../Common/SharedDefines.h"
#include "mysql_connection.h"
#include "MYSQLConnection.h"
#include "QueryDatabase.h"

namespace Quad
{
    class Result
    {
    public:
        friend class QueryDatabase;

    public:
        Result() {}
        ~Result() {}

    public:
        std::string GetString(uint8 i) const { return mResultSet->getString(i).c_str(); }
        uint16 GetUint8(uint8 i) const { return (uint8)mResultSet->getUInt(i); }
        uint16 GetUint16(uint8 i) const { return (uint16)mResultSet->getUInt(i); }
        int32 GetInt32(uint8 i) const { return (int32)mResultSet->getInt(i); }
        int64 GetInt64(uint8 i) const { return (int64)mResultSet->getInt64(i); }
        uint32 GetUint32(uint8 i) const { return (uint32)mResultSet->getUInt(i); }
        uint32 GetUint64(uint8 i) const { return (uint64)mResultSet->getUInt64(i); }
        bool GetBool(uint8 i) const { return (bool)mResultSet->getBoolean(i); }
        double GetDouble(uint8 i) const { return (double)mResultSet->getDouble(i); }
        float GetFloat(uint8 i) const { return (float)mResultSet->getDouble(i); }
        bool GetNextResult() const { return mResultSet->next(); }
        uint32 GetRowCount() const { return (uint32)mResultSet->rowsCount(); }

    public:
        std::unique_ptr<sql::ResultSet> mResultSet;
    };
}

#endif /* !_Quad_result_h_ */
