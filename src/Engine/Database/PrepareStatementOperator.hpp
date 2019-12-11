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
#include "Database/Operator.hpp"
#include "Database/PreparedResultSet.hpp"
#include <future>

namespace SteerStone { namespace Core { namespace Database {

    class PreparedStatement;

    class PrepareStatementOperator : public Operator
    {
    public:
        /// Constructor
        /// @p_PrepareStatementHolder : Keep reference of statement to be accessed later
        PrepareStatementOperator(PreparedStatement* p_PreparedStatementHolder);
        /// Deconstructor
        ~PrepareStatementOperator() override;

        //////////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////////

        /// Get Future set
        std::future<std::unique_ptr<PreparedResultSet>> GetFuture();
        /// Execute Query
        virtual bool Execute() override;

    private:
        PreparedStatement* m_PreparedStatementHolder;                         ///< Holds query and stores result set if any
        std::promise<std::unique_ptr<PreparedResultSet>>* m_PromiseResultSet; ///< Promise which the non database worker thread will hold, database worker thread holds the future
    };

}   ///< namespace Database
}   ///< namespace Core
}   ///< namespace SteerStone

