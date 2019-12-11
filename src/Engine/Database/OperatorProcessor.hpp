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

#include "Database/CallBackOperator.hpp"

namespace SteerStone { namespace Core { namespace Database {

    /// Stores the operators and calls the operator once query is done on database thread worker function
    class OperatorProcessor
    {
        DISALLOW_COPY_AND_ASSIGN(OperatorProcessor);

        //////////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////////

    public:
        /// Constructor
        OperatorProcessor();
        
        /// Deconstructor
        ~OperatorProcessor();

        //////////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////////

    public:
        /// AddOperator
        /// @p_CallBackOperator : Add Operator which will get result from async database worker thread
        void AddOperator(CallBackOperator&& p_CallBackOperator);

        /// ProcessOperators
        /// Process any operators which are not pending
        void ProcessOperators();

        //////////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////////

    private:
        std::vector<CallBackOperator> m_Operators; ///< Holds the operators
    };

}   ///< namespace Database
}   ///< namespace Core
}   ///< namespace SteerStone