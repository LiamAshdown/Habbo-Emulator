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

#include "HabboInfoPackets.h"
#include "ItemPackets.h"

namespace SteerStone
{
    namespace HabboPacket
    {
        namespace Item
        {
            //////////////////////////////////////////////////////////////////////////
            //////////////////////////////////////////////////////////////////////////

            StringBuffer const * ShowProgram::Write()
            {
                m_Buffer.AppendString(Program, false);
                m_Buffer.AppendString(" ", false);
                m_Buffer.AppendString(State, false);

                m_Buffer.AppendSOH();

                return &m_Buffer;
            }

            StringBuffer const * StuffDataUpdate::Write()
            {
                m_Buffer.AppendString(Program, false);
                m_Buffer.AppendString(" ", false);
                m_Buffer.AppendString(State, false);

                m_Buffer.AppendSOH();

                return &m_Buffer;
            }

            //////////////////////////////////////////////////////////////////////////
            //////////////////////////////////////////////////////////////////////////
        } ///< NAMESPACE ITEM
    } ///< NAMESPACE HABBOPACKET
} ///< NAMESPACE STEERSTONE