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

#ifndef _CALL_BACK_QUERY_CALL_BACK_h
#define _CALL_BACK_QUERY_CALL_BACK_h
#include "SharedDefines.h"
#include "PrepareStatementOperator.h"
#include <future>
#endif /* !_CALL_BACK_QUERY_CALL_BACK_h */

namespace SteerStone
{
    class CallBackOperator
    {
    public:
        friend class OperatorProcessor;

    public:
        /// Constructor
        explicit CallBackOperator(std::future<PreparedResultSet*> p_PreparedFuture);

        /// Move Constructor
        CallBackOperator(CallBackOperator&& p_Other) : m_PreparedFuture(std::move(p_Other.m_PreparedFuture)), m_OperatorFunction(std::move(p_Other.m_OperatorFunction)){}
        CallBackOperator& operator=(CallBackOperator&& p_Other)  { this->m_PreparedFuture = std::move(p_Other.m_PreparedFuture); this->m_OperatorFunction = std::move(p_Other.m_OperatorFunction); return *this; }

        /// Deconstructor
        ~CallBackOperator();

    public:
        /// InvokeOperator
        /// Check if operator is ready to be called
        bool InvokeOperator();

        CallBackOperator&& AddFunction(std::function<void(PreparedResultSet*)>&& p_CallBack);

    private:
        std::future<PreparedResultSet*> m_PreparedFuture;
        std::function<void(PreparedResultSet*)> m_OperatorFunction;
    };
}