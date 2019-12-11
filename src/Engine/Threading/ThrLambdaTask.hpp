/*
* Liam Ashdown
* HardCPP (Merydwin)
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
#include "Threading/ThrTask.hpp"

#include <functional>

namespace SteerStone { namespace Core { namespace Threading {

    /// Lambda task model
    class LambdaTask : public Task
    {
        DISALLOW_COPY_AND_ASSIGN(LambdaTask);

        public:
            /// Constructor
            /// @p_Name     : Task name
            /// @p_TaskType : Task Type
            /// @p_Period   : Task period
            /// @p_Function : Function
            LambdaTask(const std::string & p_Name, TaskType p_TaskType, uint64 p_Period, std::function<bool()> p_Function);
            /// Destructor
            ~LambdaTask();

            /// Get period
            uint64 GetTaskPeriod() const override final;
            /// Execute
            /// If return false, the task will stop
            bool TaskExecute() override final;

        private:
            uint64_t                m_Period;       ///< Task period
            std::function<bool()>   m_Function;     ///< Function

    };

}   ///< namespace Threading
}   ///< namespace Core
}   ///< namespace SteerStone
