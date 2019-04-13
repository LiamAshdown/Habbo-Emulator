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

#include "RegistrationPackets.h"

namespace SteerStone
{
    namespace HabboPacket
    {
        namespace Registration
        {
            //////////////////////////////////////////////////////////////////////////
            //////////////////////////////////////////////////////////////////////////

            StringBuffer const * ApproveNameReply::Write()
            {
                m_Buffer.AppendWired(ErrorCode);

                m_Buffer.AppendSOH();

                return &m_Buffer;
            }

            //////////////////////////////////////////////////////////////////////////
            //////////////////////////////////////////////////////////////////////////

            StringBuffer const * ApprovePasswordReply::Write()
            {
                m_Buffer.AppendWired(ErrorCode);

                m_Buffer.AppendSOH();

                return &m_Buffer;
            }

            //////////////////////////////////////////////////////////////////////////
            //////////////////////////////////////////////////////////////////////////

            StringBuffer const * ApproveEmail::Write()
            {
                m_Buffer.AppendSOH();

                return &m_Buffer;
            }

            //////////////////////////////////////////////////////////////////////////
            //////////////////////////////////////////////////////////////////////////

            StringBuffer const * RejectedEmail::Write()
            {
                m_Buffer.AppendSOH();

                return &m_Buffer;
            }

            //////////////////////////////////////////////////////////////////////////
            //////////////////////////////////////////////////////////////////////////

            StringBuffer const * ParentEmailRequired::Write()
            {
                m_Buffer.AppendWiredBool(RequireParentEmail);

                m_Buffer.AppendSOH();

                return &m_Buffer;
            }

            //////////////////////////////////////////////////////////////////////////
            //////////////////////////////////////////////////////////////////////////

            StringBuffer const * ValidateParentEmail::Write()
            {
                m_Buffer.AppendWiredBool(Validate);

                m_Buffer.AppendSOH();

                return &m_Buffer;
            }

        } ///< NAMESPACE REGISTRATION
    } ///< NAMESPACE HABBOPACKET
} ///< NAMESPACE STEERSTONE