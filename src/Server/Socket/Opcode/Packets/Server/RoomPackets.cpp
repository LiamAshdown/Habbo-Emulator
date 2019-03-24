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

#include "RoomPackets.h"

namespace SteerStone
{
    namespace HabboPacket
    {
        namespace Room
        {
            //////////////////////////////////////////////////////////////////////////
            //////////////////////////////////////////////////////////////////////////

            StringBuffer const * HabboRoomObject::Write()
            {
                m_Buffer.AppendString("i:", false);
                m_Buffer.AppendString(UniqueId, false);
                m_Buffer.AppendString("\r", false);

                m_Buffer.AppendString("a:", false);
                m_Buffer.AppendString(Id, false);
                m_Buffer.AppendString("\r", false);

                m_Buffer.AppendString("n:", false);
                m_Buffer.AppendString(Gender, false);
                m_Buffer.AppendString("\r", false);

                m_Buffer.AppendString("f:", false);
                m_Buffer.AppendString(Figure, false);
                m_Buffer.AppendString("\r", false);

                m_Buffer.AppendString("s:", false);
                m_Buffer.AppendString(Gender == "Male" ? "M" : "F", false);
                m_Buffer.AppendString("\r", false);

                m_Buffer.AppendString("l:", false);
                m_Buffer.AppendString(X, false);
                m_Buffer.AppendString(" ", false);
                m_Buffer.AppendString(Y, false);
                m_Buffer.AppendString(" ", false);
                m_Buffer.AppendString(Z, false);
                m_Buffer.AppendString("\r", false);

                if (Motto.is_initialized())
                {
                    m_Buffer.AppendString("c:", false);
                    m_Buffer.AppendString(Motto.get(), false);
                    m_Buffer.AppendString("\r", false);
                }

                m_Buffer.AppendString("b:", false);
                m_Buffer.AppendSOH();
            }

            //////////////////////////////////////////////////////////////////////////
            //////////////////////////////////////////////////////////////////////////

        } ///< NAMESPACE ROOM
    } ///< NAMESPACE HABBOPACKET
} ///< NAMESPACE STEERSTONE