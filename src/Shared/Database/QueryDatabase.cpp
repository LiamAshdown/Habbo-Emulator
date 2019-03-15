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
    QueryDatabase::QueryDatabase(const std::string database)
    {
        mDatabase = database;
        mConnection = sDatabase->GetDatabase(mDatabase)->GetConnectionPool()->Borrow();
        mSqlConnection = mConnection->SQLConnection;
        mConnection->connectionState = ConnectionState::Connection_Idle;
        mIsExecuteResult = false;
        mHasReleased = false;
    }
    QueryDatabase::~QueryDatabase()
    {
        IF_LOG(plog::debug)
            LOG_DEBUG << "Destructor QueryDatabase called!";

        if (!mHasReleased)
            sDatabase->GetDatabase(mDatabase)->GetConnectionPool()->UnBorrow(mConnection);
    }
    //-----------------------------------------------//
    void QueryDatabase::DirectExecuteQuery(const std::string& query)
    {
        try
        {
            mStatement = std::shared_ptr<sql::Statement>(mSqlConnection->createStatement());
            mExecuteResult = mStatement->execute(query.c_str());
            mConnection->connectionState = ConnectionState::Connection_Query;
            mIsExecuteResult = true;
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
        mIsExecuteResult = false;
        mConnection->connectionState = ConnectionState::Connection_Prepare;
    }
    //-----------------------------------------------//
    void QueryDatabase::ExecuteQuery()
    {
        try
        {
            mResultSet = std::unique_ptr<sql::ResultSet>(mPreparedStatement->executeQuery());
            mConnection->connectionState = ConnectionState::Connection_Query;
        }
        catch (sql::SQLException &e)
        {
            sDatabase->PrintException(e, const_cast<char*>(__FILE__), const_cast<char*>(__FUNCTION__), __LINE__);
        }
    }
    //-----------------------------------------------//
    bool QueryDatabase::GetResult()
    {
        if (mIsExecuteResult)
            return mExecuteResult;
        else if (mResultSet->next())
        {
            mField.mResultSet = std::move(mResultSet);
            return true;
        }
        return false;
    }
    void QueryDatabase::Release()
    {
        if (!mHasReleased)
        {
            sDatabase->GetDatabase(mDatabase)->GetConnectionPool()->UnBorrow(mConnection);
            mHasReleased = true;
        }
    }
    //-----------------------------------------------//
    std::shared_ptr<sql::PreparedStatement>& QueryDatabase::GetStatement()
    {
        return mPreparedStatement;
    }
    //-----------------------------------------------//
    Field* QueryDatabase::Fetch()
    {
        return &mField;
    }
    //-----------------------------------------------//
}