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

#ifndef _DATABASE_CONNECTION_POOL_h
#define _DATABASE_CONNECTION_POOL_h
#include "Common/SharedDefines.h"
#include "MySQLConnection.h"
#include <exception>
#include <mutex>
#include <thread>
#endif /* _DATABASE_CONNECTION_POOL_h */

namespace SteerStone
{
    /// Class which stores databases in a storage
    class ConnectionPool
    {
    public:
        /// Constructor
        /// @p_MysqlConnection : Connection of the database
        /// @p_PoolSize : Amount of connections to launch database
        ConnectionPool(std::shared_ptr<MySQLConnection> p_MysqlConnection, std::size_t const& p_PoolSize)
            : m_MySQLConnection(p_MysqlConnection), m_PoolSize(p_PoolSize)
        {
            for (std::size_t l_I = 0; l_I < m_PoolSize; l_I++)
                m_Pool.push_back(m_MySQLConnection->CreateDatabase());
        }

        /// Deconstructor
        ~ConnectionPool() {}

        /// Borrow
        /// Borrow a connection in our m_Pool
        std::shared_ptr<Connection> Borrow()
        {
            std::lock_guard<std::mutex> l_Guard(m_Mutex);

            if (m_Pool.size() == 0)
            {
                for (auto& itr = m_BorrowedPool.begin(); itr != m_BorrowedPool.end(); itr++)
                {
                    std::shared_ptr<Connection> l_Connection = (*itr);

                    /// If our connection is unique this means that the QueryDatabase is not sharing a pointer from this connection,
                    /// so we will erase the borrowed connection and put back into the pool so QueryDatabase can share it
                    if (l_Connection.unique())
                    {
                        try
                        {
                            std::shared_ptr<Connection> l_NewConnection = m_MySQLConnection->CreateDatabase();

                            m_BorrowedPool.erase(l_Connection);
                            m_BorrowedPool.insert(l_NewConnection);

                            return l_NewConnection;
                        }
                        catch (const std::exception&)
                        {
                            assert(false); ///< Assert if we fail in creating a new database, CreateDatabase already throws an error, so don't need
                                           ///< To log the error twice
                        }
                    }

                }

                /// If all connections are borrowed, then we create a temporary connection and log the error as fatal
                LOG_FATAL << "All connections are borrowed... attempting to create new temporary connection";
                try
                {
                    std::shared_ptr<Connection> l_NewConnection = m_MySQLConnection->CreateDatabase();
                    m_BorrowedPool.insert(l_NewConnection);
                    return l_NewConnection;
                }
                catch (const std::exception&)
                {
                    assert(false);
                }
            }

            /// Declare our connection from Pool
            std::shared_ptr<Connection> l_Connection = m_Pool.front();

            /// Insert our borrowed connection into the borrowed pool
            m_BorrowedPool.insert(l_Connection);

            /// Remove connection from Pool
            m_Pool.pop_front();

            return l_Connection;
        }

        /// Constructor
        /// @p_Connection : Remove Connection from Borrowed Pool
        void UnBorrow(std::shared_ptr<Connection> p_Connection)
        {
            std::lock_guard<std::mutex> l_Guard(this->m_Mutex);

            m_BorrowedPool.erase(p_Connection);
            m_Pool.push_back(p_Connection);
        }

    private:
        /// Database Connections
        std::shared_ptr<MySQLConnection> m_MySQLConnection;           ///< Mysql Database Conncection 
        std::deque<std::shared_ptr<Connection>> m_Pool;               ///< MysqlConnections Pool
        std::set<std::shared_ptr<Connection>> m_BorrowedPool;         ///< MysqlConnections Borrowed Pool               
        /// Variables
        std::size_t m_PoolSize;                                       ///< Pool Size of MysqlConnections  
        std::mutex m_Mutex;                                           ///< Mutex
    };
} ///< NAMESPACE STEERSTONE
