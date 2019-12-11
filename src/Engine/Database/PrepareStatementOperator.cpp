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

#include "Database/PreparedStatement.hpp"

namespace SteerStone { namespace Core { namespace Database {

    /// Constructor
    /// @p_PrepareStatementHolder : Keep reference of statement to be accessed later
    PrepareStatementOperator::PrepareStatementOperator(PreparedStatement * p_PreparedStatementHolder) : m_PreparedStatementHolder(p_PreparedStatementHolder)
    {
        m_PromiseResultSet = new std::promise<std::unique_ptr<PreparedResultSet>>();
    }
    /// Deconstructor
    PrepareStatementOperator::~PrepareStatementOperator()
    {
        /// Delete our promise since we sent the result to the future
        if (m_PromiseResultSet)
            delete m_PromiseResultSet;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Get Future set
    std::future<std::unique_ptr<PreparedResultSet>> PrepareStatementOperator::GetFuture()
    {
        return m_PromiseResultSet->get_future();
    }
    /// Execute Query
    bool PrepareStatementOperator::Execute()
    {
        m_PromiseResultSet->set_value(m_PreparedStatementHolder->ExecuteStatement(true));

        return true;
    }

}   ///< namespace Database
}   ///< namespace Core
}   ///< namespace SteerStone