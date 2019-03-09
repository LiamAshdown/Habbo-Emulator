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
//-----------------------------------------------//
#include "QueryDatabase.h"
//-----------------------------------------------//
namespace Quad
{
    //-----------------------------------------------//
    void QueryDatabase::DirectExecuteQuery(const std::string& query)
    {
        try
        {
            mStatement = std::shared_ptr<sql::Statement>(mSqlConnection->createStatement());
            mExecuteResult = mStatement->execute(query.c_str());
        }
        catch (sql::SQLException &e)
        {
            sDatabase->PrintException(e, const_cast<char*>(__FILE__), const_cast<char*>(__FUNCTION__), __LINE__);
        }
    }
    //-----------------------------------------------//
    void QueryDatabase::PrepareQuery(const std::string& query)
    {
        mPreparedStatement = std::shared_ptr<sql::PreparedStatement>(mSqlConnection->prepareStatement(query.c_str()));
    }
    //-----------------------------------------------//
    void QueryDatabase::ExecuteResultPrepareQuery()
    {
        try
        {
            mResultSet = std::shared_ptr<sql::ResultSet>(mPreparedStatement->executeQuery());
        }
        catch (sql::SQLException &e)
        {
            sDatabase->PrintException(e, const_cast<char*>(__FILE__), const_cast<char*>(__FUNCTION__), __LINE__);
        }
    }
    //-----------------------------------------------//
    void QueryDatabase::ExecuteBoolPrepareQuery()
    {
        try
        {
            mExecuteResult = mPreparedStatement->execute();
        }
        catch (sql::SQLException &e)
        {
            sDatabase->PrintException(e, const_cast<char*>(__FILE__), const_cast<char*>(__FUNCTION__), __LINE__);
        }
    }
    //-----------------------------------------------//
    bool QueryDatabase::GetExecuteQueryResult()
    {
        if (mResultSet->next())
        {
            mField.mResultSet = mResultSet;
            return true;
        }
        return false;
    }
    //-----------------------------------------------//
    std::shared_ptr<sql::PreparedStatement>& QueryDatabase::GetStatement()
    {
        return mPreparedStatement;
    }
    //-----------------------------------------------//
    bool QueryDatabase::GetExecuteResult() const
    {
        return mExecuteResult;
    }
    //-----------------------------------------------//
    Field* QueryDatabase::Fetch()
    {
        return &mField;
    }
    //-----------------------------------------------//
}