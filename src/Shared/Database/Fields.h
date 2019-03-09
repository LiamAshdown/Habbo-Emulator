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

#ifndef _Quad_Fields_h_
#define _Quad_Fields_h_
#include "../Common/SharedDefines.h"
#include "mysql_connection.h"
#include "MYSQLConnection.h"
#include "QueryDatabase.h"

namespace Quad
{
    class Field
    {
    public:
        friend class QueryDatabase;

    public:
        Field(){}
        ~Field() 
        {
            IF_LOG(plog::debug)
                LOG_DEBUG << "Destructor Field called!";
        }

    public:
        std::string GetString(uint8 i)
        {
            return mResultSet->getString(i);
        }

        int32 GetInt32(uint8 i)
        {
            return mResultSet->getInt(i);
        }

        int64 GetInt64(uint8 i)
        {
            return mResultSet->getInt64(i);
        }

        uint32 GetUint32(uint8 i)
        {
            return mResultSet->getUInt(i);
        }

        uint32 GetUint64(uint8 i)
        {
            return mResultSet->getUInt64(i);
        }

        bool GetBool(uint8 i)
        {
            return mResultSet->getBoolean(i);
        }

        double GetDouble(uint8 i)
        {
            return mResultSet->getDouble(i);
        }

        bool GetNextResult()
        {
            return mResultSet->next();
        }

        bool RowExists() const
        {
            return mResultSet->rowsCount() > 0 ? true : false;
        }

    public:
        std::shared_ptr<sql::ResultSet> mResultSet;
    };
}

#endif /* !_Quad_Fields_h_ */
