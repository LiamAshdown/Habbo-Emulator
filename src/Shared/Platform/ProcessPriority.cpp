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

#include "ProcessPriority.h"
#include <Windows.h>

/// SetProcessPriority
/// @p_Affinity : CPUs to be launched
/// @p_HighPriority : Set process to high priority class
void SetProcessPriority(uint32 p_Affinity, bool p_HighPriority)
{
    HANDLE l_Process = GetCurrentProcess();

    if (p_Affinity > 0)
    {
        ULONG_PTR l_ProcessAffinityMask;
        ULONG_PTR l_SystemAffinityMask;

        if (GetProcessAffinityMask(l_Process, &l_ProcessAffinityMask, &l_SystemAffinityMask))
        {
            // remove non accessible processors
            ULONG_PTR l_Affinity = p_Affinity & l_ProcessAffinityMask;

            /// https://docs.microsoft.com/en-us/windows/desktop/api/winbase/nf-winbase-getprocessaffinitymask
            /// Return type is non zero
            if (l_Affinity)
                SetProcessAffinityMask(l_Process, l_Affinity);
            else
                LOG_ERROR << GetLastError();
        }
    }

    if (p_HighPriority)
    {
        if (SetPriorityClass(l_Process, HIGH_PRIORITY_CLASS))
            LOG_INFO << "Set Process Priority class to HIGH";
        else
            LOG_ERROR << GetLastError();
    }
}
