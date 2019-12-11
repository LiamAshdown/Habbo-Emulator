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

#include "Database/OperatorProcessor.hpp"

namespace SteerStone { namespace Core { namespace Database {

    /// Constructor
    OperatorProcessor::OperatorProcessor()
    {
    }

    /// Deconstructor
    OperatorProcessor::~OperatorProcessor()
    {
    }

    /// AddOperator
    /// @p_CallBackOperator : Add Operator which will get result from async database worker thread
    void OperatorProcessor::AddOperator(CallBackOperator&& p_QueryCallBack)
    {
        m_Operators.push_back(std::move(p_QueryCallBack));
    }

    /// ProcessOperators
    /// Process any operators which are not pending
    void OperatorProcessor::ProcessOperators()
    {
        if (m_Operators.empty())
            return;

        std::vector<CallBackOperator> l_UpdateOperators{ std::move(m_Operators) };

        l_UpdateOperators.erase(std::remove_if(l_UpdateOperators.begin(), l_UpdateOperators.end(), [](CallBackOperator& p_Operator)
            {
                return p_Operator.InvokeOperator();
            }), l_UpdateOperators.end());

        m_Operators.insert(m_Operators.end(), std::make_move_iterator(l_UpdateOperators.begin()), std::make_move_iterator(l_UpdateOperators.end()));
    }

}   ///< namespace Database
}   ///< namespace Core
}   ///< namespace SteerStone