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

#include "DatabaseTypes.h"

namespace SteerStone
{
    /// Constructor
    CallBackOperator::CallBackOperator(std::future<std::unique_ptr<PreparedResultSet>> p_PreparedFuture) : m_PreparedFuture(std::move(p_PreparedFuture)), m_OperatorFunction(nullptr)
    {
    }

    /// Deconstructor
    CallBackOperator::~CallBackOperator()
    {
    }

    /// AddFunction
    /// p_CallBack : Function which we will be doing a call back on
    CallBackOperator&& CallBackOperator::AddFunction(std::function<void(std::unique_ptr<PreparedResultSet>)> p_CallBack)
    {
        m_OperatorFunction = std::move(p_CallBack);
        return std::move(*this);
    }

    /// InvokeOperator
    /// Check if operator is ready to be called
    bool CallBackOperator::InvokeOperator()
    {
        /// Is our promise ready?
        if (m_PreparedFuture.wait_for(std::chrono::seconds(0)) == std::future_status::ready)
        {
            std::unique_ptr<PreparedResultSet> l_PreparedResultSet = m_PreparedFuture.get();

            /// If there's a function, then execute the function with our result set
            if (m_OperatorFunction)
                m_OperatorFunction(std::move(l_PreparedResultSet));
            else ///< If there's no function, then free the prepared statement here
            {
                if (l_PreparedResultSet->GetPreparedStatement()->GetMYSQLStatement()->GetDatabase().GetName() == "users")
                    UserDatabase.FreePrepareStatement(std::move(l_PreparedResultSet->GetPreparedStatement()));
                else
                    RoomDatabase.FreePrepareStatement(std::move(l_PreparedResultSet->GetPreparedStatement()));
            }

            return true;
        }
        return false;
    }
} ///< NAMESPACE STEERSTONE
