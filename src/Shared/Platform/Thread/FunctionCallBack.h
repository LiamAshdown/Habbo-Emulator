/*
* Liam Ashdown
* Copyright (C) 2018
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

#ifndef _THREAD_FUNCTION_CALL_BACK_h
#define _THREAD_FUNCTION_CALL_BACK_h   
#include "Common/SharedDefines.h"
#include <mutex>
#endif /* _THREAD_FUNCTION_CALL_BACK_h */

namespace SteerStone
{
    /// This is used when you want the function to be processed on one thread
    class FunctionCallBack
    {
    public:
        /// Constructor
        FunctionCallBack();

        /// Deconstructor
        ~FunctionCallBack();

    public:
        /// AddCallBack
        /// @p_Function : Function being added to storage to be processed later
        void AddCallBack(std::function<void()> p_Function);

        /// ProcessFunctions
        /// Execute all functions in storage
        void ProcessFunctions();

    private:
        /// Cannot be copyable
        FunctionCallBack(FunctionCallBack const&) = delete;
        FunctionCallBack& operator=(FunctionCallBack const&) = delete;

    private:
        std::deque<std::function<void()>> m_CallBack;           ///< Stores the functions
        std::mutex m_Mutex;                                     ///< Mutex lock AddCallBack
    };
    
} /// NAMESPACE STEERSTONE