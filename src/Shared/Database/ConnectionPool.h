/*
* Liam Ashdown
* Copyright(C) 2019
*
* Active911 Inc.
* Copyright(C) 2013
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
#include "Common/SharedDefines.h"

enum ConnectionState
{
    Connection_Idle,               // When we borrowed the connection
    Connection_Prepare,            // When we prepared a query
    Connection_Query               // When we executed a query
};

namespace Quad
{
    class Connection 
    {
    public:
        Connection() {};
        virtual ~Connection() {};
        
    public:
        ConnectionState connectionState;
    };

    class ConnectionFactory 
    {
    public:
        virtual std::shared_ptr<Connection> Create() = 0;
    };

    struct ConnectionPoolStats 
    {
        std::size_t sPoolSize;
        std::size_t sBorrowedSize;
    };

    template<class T>
    class ConnectionPool
    {
    public:
        ConnectionPool(std::size_t poolSize, std::shared_ptr<ConnectionFactory> factory)
        {
            // Setup
            mPoolSize = poolSize;
            mFactory = factory;

            // Fill the mPool
            while (mPool.size() < mPoolSize)
            {
                mPool.push_back(mFactory->Create());
            }
        }

        ~ConnectionPool() {};

        void GetStats()
        {
            std::lock_guard<std::mutex> lock(mMutex);

            ConnectionPoolStats stats;
            stats.sPoolSize = mPool.size();
            stats.sBorrowedSize = mBorrowed.size();
        }

        /*
        * Borrow a connection for temporary use
        * When done, either (a) call unborrow() to return it, or (b) (if it's bad) just let it go out of scope.  This will cause it to automatically be replaced.
        * @retval a shared_ptr to the connection object
        */
        std::shared_ptr<T> Borrow()
        {
            std::lock_guard<std::mutex> lock(mMutex);

            // Check for a free connection
            if (mPool.size() == 0) 
            {
                // Are there any crashed connections listed as "borrowed"?
                for (std::set<std::shared_ptr<Connection>>::iterator it = mBorrowed.begin();
                    it != mBorrowed.end(); ++it) 
                {
                    if ((*it).unique() || (*it)->connectionState == ConnectionState::Connection_Query)
                    {
                        // This connection has been abandoned! Destroy it and create a new connection
                        try 
                        {
                            // If we are able to create a new connection, return it
                            IF_LOG(plog::debug)
                                LOG_DEBUG << "Creating new connection to replace discarded connection";

                            std::shared_ptr<Connection> conn = mFactory->Create();

                            mBorrowed.erase(it);
                            mBorrowed.insert(conn);

                            return std::static_pointer_cast<T>(conn);

                        }
                        catch (std::exception &e) 
                        {
                            LOG_ERROR << "Failed in borrowing a connection!";
                            assert(false);
                        }
                    }
                }
            }

            // Take one off the front
            std::shared_ptr<Connection> conn = mPool.front();
            mPool.pop_front();

            // Add it to the borrowed list
            mBorrowed.insert(conn);

            return std::static_pointer_cast<T>(conn);
        }

        /*
        * Unborrow a connection
        *
        * Only call this if you are returning a working connection.  If the connection was bad, just let it go out of scope (so the connection manager can replace it).
        * @param the connection
        */
        void UnBorrow(std::shared_ptr<T> conn) 
        {
            // Lock
            std::lock_guard<std::mutex> lock(mMutex);

            // Push onto the pool
            mPool.push_back(std::static_pointer_cast<Connection>(conn));

            // Unborrow
            mBorrowed.erase(conn);
        };

    protected:
        std::shared_ptr<ConnectionFactory> mFactory;
        std::size_t mPoolSize;
        std::deque<std::shared_ptr<Connection>> mPool;
        std::set<std::shared_ptr<Connection>> mBorrowed;
        std::mutex mMutex;
    };
}

#endif /* !_Quad_Config_h_ */
