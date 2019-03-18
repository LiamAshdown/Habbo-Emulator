/*
* Liam Ashdown
* Copyright(C) 2019
*
* This program is free software : you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.If not, see < http://www.gnu.org/licenses/>.
*/

/* ConnectionPool manages a connection mPool of some kind.  Worker threads can ask for a connection, and must return it when done.
 * Each connection is guaranteed to be healthy, happy, and free of disease.
 *
 * Connection and ConnectionFactory are virtual classes that should be overridden to their actual type.
 *
 * NOTE: To avoid using templates AND inheritance at the same time in the ConnectionFactory, ConnectionFactory::create must create a derved type
 * but return the base class.
 */

#ifndef _Quad_ConnectionPool_h_
#define _Quad_ConnectionPool_h_
#include <deque>
#include <set>
#include <exception>
#include <mutex>
#include <thread>
#include "MySQLConnection.h"
#include "Common/SharedDefines.h"

namespace Quad
{
    class ConnectionPool
    {
    public:
        ConnectionPool(std::shared_ptr<MySQLConnection> mySQLConnection, const std::size_t& poolSize)
            : mMySQLConnection(mySQLConnection), mPoolSize(poolSize)
        {
            for (std::size_t i = 0; i < mPoolSize; i++)
                mPool.push_back(mMySQLConnection->CreateDatabase());
        }

        std::shared_ptr<Connection> Borrow()
        {
            std::lock_guard<std::mutex> guard(mMutex);

            if (mPool.size() == 0)
            {
                for (std::set<std::shared_ptr<Connection>>::iterator itr = mBorrowedPool.begin(); itr != mBorrowedPool.end(); itr++)
                {
                    std::shared_ptr<Connection> connection = (*itr);

                    // If our connection is unique this means that the QueryDatabase is not sharing a pointer from this connection,
                    // so we will erase the borrowed connection and put back into the pool so QueryDatabase can share it
                    if (connection.unique())
                    {
                        try
                        {
                            std::shared_ptr<Connection> newConnection = mMySQLConnection->CreateDatabase();

                            mBorrowedPool.erase(connection);
                            mBorrowedPool.insert(newConnection);

                            return newConnection;
                        }
                        catch (const std::exception&)
                        {
                            assert(false);
                        }
                    }

                }

                // If all connections are borrowed, then we create a temporary connection and log the error as fatal
                LOG_FATAL << "All connections are borrowed... attempting to create new temporary connection";
                try
                {
                    std::shared_ptr<Connection> newConnection = mMySQLConnection->CreateDatabase();
                    mBorrowedPool.insert(newConnection);
                    return newConnection;
                }
                catch (const std::exception&)
                {
                    assert(false);
                }
            }

            // We are now borrowing a connection, remove 1 connection from our thread pool
            std::shared_ptr<Connection> connection = mPool.front();
            mPool.pop_front();

            // Insert our borrowed connection into the borrowed pool
            mBorrowedPool.insert(connection);

            return connection;
        }

        void UnBorrow(std::shared_ptr<Connection> connection)
        {
            std::lock_guard<std::mutex> guard(mMutex);

            mBorrowedPool.erase(connection);
            mPool.push_back(connection);
        }

    private:
        std::shared_ptr<MySQLConnection> mMySQLConnection;
        std::deque<std::shared_ptr<Connection>> mPool;
        std::set<std::shared_ptr<Connection>> mBorrowedPool;

        std::size_t mPoolSize;
        std::mutex mMutex;
    };
}

#endif /* !_Quad_Config_h_ */
