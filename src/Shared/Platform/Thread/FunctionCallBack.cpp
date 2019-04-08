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

#include "FunctionCallBack.h"

namespace SteerStone
{
    /// Constructor
    FunctionCallBack::FunctionCallBack()
    {

    }

    /// Deconstructor
    FunctionCallBack::~FunctionCallBack()
    {

    }

    /// AddCallBack
    /// @p_Function : Function being added to storage to be processed later
    void FunctionCallBack::AddCallBack(std::function<void()> p_Function)
    {
        std::unique_lock<std::mutex> l_Guard(m_Mutex);

        m_CallBack.push_back(p_Function);
    }

    /// ProcessFunctions
    /// Execute all functions in storage
    void FunctionCallBack::ProcessFunctions()
    {
        if (m_CallBack.empty())
            return;

        std::function<void()> l_Function = m_CallBack.back();

        m_CallBack.pop_back();

        l_Function();
    }
} /// NAMESPACE STEERSTONE