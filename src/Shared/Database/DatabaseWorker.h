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

#ifndef _DATABASE_DATABASE_WORKER_h
#define _DATABASE_DATABASE_WORKER_h
#include "SharedDefines.h"
#include "ProducerQueue.h"
#include <thread>
#endif /* !_DATABASE_DATABASE_WORKER_h */

namespace SteerStone
{
    class Operator;

    class DatabaseWorker
    {
    public:
        friend class Database;

    public:
        /// Constructor
        DatabaseWorker();

        /// Deconstructor
        ~DatabaseWorker();

    private:
        /// WorkerThread
        /// Executes asynchronous queries
        void WorkerThread();

    private:
        ProducerQueue<Operator*> m_Queue;
        std::thread m_WorkerThread;
    };

} ///< NAMESPACE STEERSTONE

