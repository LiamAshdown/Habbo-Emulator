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

#ifndef _Quad_QueryDatabase_h_
#define _Quad_QueryDatabase_h_
#include "../Common/SharedDefines.h"
#include "Database.h"
#include "Result.h"

namespace Quad
{
    class QueryDatabase
    {
    public:
       explicit QueryDatabase(const std::string database);
        ~QueryDatabase();

    public:
        void DirectExecuteQuery(const std::string& query);

        void PrepareQuery(const std::string& query);
        void ExecuteQuery();
        bool GetResult();
        std::shared_ptr<sql::PreparedStatement>& GetStatement();

        Result* Fetch();

    private:
        std::string mDatabase;
        std::shared_ptr<Connection> mConnection;
        std::shared_ptr<sql::Connection> mSqlConnection;

        std::shared_ptr<sql::Statement> mStatement;

        std::shared_ptr<sql::PreparedStatement> mPreparedStatement;

        bool mExecuteResult;
        bool mIsExecuteResult;
        std::unique_ptr<sql::ResultSet> mResultSet;
        Result mResult;
    }; 
}

#endif /* !_Quad_QueryDatabase_h_ */
