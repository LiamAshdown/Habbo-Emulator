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

#include "DatabaseWorker.hpp"
#include "Operator.hpp"
#include "Utility/UtiString.hpp"

namespace SteerStone { namespace Core { namespace Database {
    
    /// Constructor
    /// @p_WorkerThread : Worker thread number spawned
    DatabaseWorker::DatabaseWorker(uint8 const& p_WorkerThread)
    {
        l_Task = sThreadManager->PushTask(Utils::StringBuilder("DATABASE_WORKER_THREAD_%0", p_WorkerThread), Threading::TaskType::Moderate, -1, std::bind(&DatabaseWorker::Update, this));
    }
    /// Deconstructor
    DatabaseWorker::~DatabaseWorker()
    {
        m_Producer.ShutDown();
        sThreadManager->PopTask(l_Task);
    }

    /// Add Operator
    /// @p_Operator : Operater being added
    void DatabaseWorker::AddOperator(Operator* p_Operator)
    {
        m_Producer.Push(p_Operator);
    }

    /// Get size of producer
    const std::size_t DatabaseWorker::GetSize()
    {
        return m_Producer.GetSize();
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    bool DatabaseWorker::Update()
    {
        while (true)
        {
            Operator* l_Operator = nullptr;
            m_Producer.WaitAndPop(l_Operator);

            if (l_Operator)
            {
                l_Operator->Execute();

                delete l_Operator;
            }
        }

        return false;
    }

}   ///< namespace Database
}   ///< namespace Core
}   ///< namespace SteerStone