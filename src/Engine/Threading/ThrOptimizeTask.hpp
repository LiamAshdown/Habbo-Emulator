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

namespace SteerStone { namespace Core { namespace Threading {

    /// Lambda task model
    class OptimizeTask : public Task
    {
        DISALLOW_COPY_AND_ASSIGN(OptimizeTask);

        //////////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////////

        public:
            /// Constructor
            /// @p_Period : Task period
            OptimizeTask(uint64 p_Period);
            /// Destructor
            ~OptimizeTask();

            /// Get period
            uint64 GetTaskPeriod() const override final;
            /// Get period
            /// @p_Period : New period
            void SetTaskPeriod(uint64 p_Period);
            /// Execute
            /// If return false, the task will stop
            bool TaskExecute() override final;

        private:
            std::atomic_uint64_t m_Period;  ///< Task period

    };

    using OptimizeTaskPtr = std::shared_ptr<OptimizeTask>;

}   ///< namespace Threading
}   ///< namespace Core
}   ///< namespace SteerStone
