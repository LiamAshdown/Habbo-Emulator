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

#pragma once
#include <PCH/Precompiled.hpp>
#include "Core/Core.hpp"
#include "Database/ProducerQueue.hpp"
#include "Threading/ThrTaskManager.hpp"

namespace SteerStone { namespace Core { namespace Database {

    class Operator;

    class DatabaseWorker
    {
    DISALLOW_COPY_AND_ASSIGN(DatabaseWorker);

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    public:
        /// Constructor
        /// @p_WorkerThread : Worker thread number spawned
        DatabaseWorker(uint8 const& p_WorkerThread);
        /// Deconstructor
        ~DatabaseWorker();

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

        /// Add Operator
        /// @p_Operator : Operater being added
        void AddOperator(Operator* p_Operator);

        /// Get size of producer
        const std::size_t GetSize();

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    private:
        bool Update();

    private:
        ProducerQueue<Operator*> m_Producer;
        Threading::Task::Ptr l_Task;
    };

}   ///< namespace Database
}   ///< namespace Core
}   ///< namespace SteerStone

