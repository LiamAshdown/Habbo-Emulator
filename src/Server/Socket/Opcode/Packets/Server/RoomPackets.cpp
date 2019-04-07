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

            StringBuffer const* HabboRoomObject::Write()
            {
                m_Buffer.AppendString("i:", false);
                m_Buffer.AppendString(UniqueId, false);
                m_Buffer.AppendString("\r", false);

                m_Buffer.AppendString("a:", false);
                m_Buffer.AppendString(Id, false);
                m_Buffer.AppendString("\r", false);

                m_Buffer.AppendString("n:", false);
                m_Buffer.AppendString(Name, false);
                m_Buffer.AppendString("\r", false);

                m_Buffer.AppendString("f:", false);
                m_Buffer.AppendString(Figure, false);
                m_Buffer.AppendString("\r", false);

                m_Buffer.AppendString("s:", false);
                m_Buffer.AppendString(Gender, false);
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
                m_Buffer.AppendString("\r", false);
                m_Buffer.AppendSOH();

                return &m_Buffer;
            }

            //////////////////////////////////////////////////////////////////////////
            //////////////////////////////////////////////////////////////////////////

            StringBuffer const* RoomInterest::Write()
            {
                m_Buffer.AppendString("0");
                m_Buffer.AppendSOH();

                return &m_Buffer;
            }

            //////////////////////////////////////////////////////////////////////////
            //////////////////////////////////////////////////////////////////////////


            StringBuffer const* OpenConnection::Write()
            {
                m_Buffer.AppendSOH();

                return &m_Buffer;
            }


            //////////////////////////////////////////////////////////////////////////
            //////////////////////////////////////////////////////////////////////////

            StringBuffer const* RoomUrl::Write()
            {
                m_Buffer.AppendString("/client/");
                m_Buffer.AppendSOH();

                return &m_Buffer;
            }

            //////////////////////////////////////////////////////////////////////////
            //////////////////////////////////////////////////////////////////////////

            StringBuffer const* RoomCantConnect::Write()
            {
                m_Buffer.AppendWired(ErrorCode);
                m_Buffer.AppendSOH();

                return &m_Buffer;
            }

            //////////////////////////////////////////////////////////////////////////
            //////////////////////////////////////////////////////////////////////////

            StringBuffer const* RoomReady::Write()
            {
                m_Buffer.AppendString(Model);
                m_Buffer.AppendString(" ");
                m_Buffer.AppendWired(Id);
                m_Buffer.AppendSOH();

                return &m_Buffer;
            }

            //////////////////////////////////////////////////////////////////////////
            //////////////////////////////////////////////////////////////////////////

            StringBuffer const* RoomAdd::Write()
            {
                m_Buffer.AppendWired(0);
                m_Buffer.AppendSOH();

                return &m_Buffer;
            }

            //////////////////////////////////////////////////////////////////////////
            //////////////////////////////////////////////////////////////////////////

            StringBuffer const* RoomHeight::Write()
            {
                m_Buffer.AppendString(HeightMap);
                m_Buffer.AppendSOH();

                return &m_Buffer;
            }

            //////////////////////////////////////////////////////////////////////////
            //////////////////////////////////////////////////////////////////////////

            StringBuffer const* ObjectsWorld::Write()
            {
                for (auto const& l_Itr : WorldObjects)
                {
                    m_Buffer.AppendStringDelimiter(l_Itr.Id, " ");
                    m_Buffer.AppendStringDelimiter(l_Itr.Sprite, " ");
                    m_Buffer.AppendStringDelimiter(l_Itr.X, " ");
                    m_Buffer.AppendStringDelimiter(l_Itr.Y, " ");
                    m_Buffer.AppendStringDelimiter(l_Itr.Z, " ");
                    m_Buffer.AppendStringDelimiter(l_Itr.Rotation, " ");
                    m_Buffer.AppendStringDelimiter(l_Itr.Length, " ");
                    m_Buffer.AppendString("\r", false);
                }

                m_Buffer.AppendSOH();

                return &m_Buffer;
            }

            //////////////////////////////////////////////////////////////////////////
            //////////////////////////////////////////////////////////////////////////

            StringBuffer const * ActiveObjects::Write()
            {
                /// TODO
                m_Buffer.AppendString("H", false);

                m_Buffer.AppendSOH();

                return &m_Buffer;
            }

            //////////////////////////////////////////////////////////////////////////
            //////////////////////////////////////////////////////////////////////////

            StringBuffer const * UserUpdateStatus::Write()
            {
                m_Buffer.AppendString(GUID, false);
                m_Buffer.AppendString(" ");
                m_Buffer.AppendStringDelimiter(CurrentX, ",");
                m_Buffer.AppendStringDelimiter(CurrentY, ",");
                m_Buffer.AppendStringDelimiter(CurrentZ, ",");
                m_Buffer.AppendStringDelimiter(HeadRotation, ",");
                m_Buffer.AppendString(BodyRotation, false);

                if (Dancing)
                    m_Buffer.AppendString("/Dance", false);
                if (Walking)
                {
                    m_Buffer.AppendString("/mv", false);
                    m_Buffer.AppendString(" ");
                    m_Buffer.AppendStringDelimiter(NewX, ",");
                    m_Buffer.AppendStringDelimiter(NewY, ",");
                    m_Buffer.AppendString(NewZ, false);
                }
                if (Waving)
                    m_Buffer.AppendString("/Wave", false);
                if (Sitting)
                    m_Buffer.AppendString("/sit 1", false);

                m_Buffer.AppendString("\r", false);
                m_Buffer.AppendSOH();

                return &m_Buffer;
            }

            //////////////////////////////////////////////////////////////////////////
            //////////////////////////////////////////////////////////////////////////

            StringBuffer const * LeaveRoom::Write()
            {
                m_Buffer.AppendWired(GUID);

                m_Buffer.AppendSOH();

                return &m_Buffer;
            }

            //////////////////////////////////////////////////////////////////////////
            //////////////////////////////////////////////////////////////////////////

            StringBuffer const * Chat::Write()
            {
                m_Buffer.AppendWired(GUID);
                m_Buffer.AppendString(Message);

                m_Buffer.AppendSOH();

                return &m_Buffer;
            }

            //////////////////////////////////////////////////////////////////////////
            //////////////////////////////////////////////////////////////////////////


            StringBuffer const * Shout::Write()
            {
                m_Buffer.AppendWired(GUID);
                m_Buffer.AppendString(Message);

                m_Buffer.AppendSOH();

                return &m_Buffer;
            }

            //////////////////////////////////////////////////////////////////////////
            //////////////////////////////////////////////////////////////////////////

            StringBuffer const * Whisper::Write()
            {
                m_Buffer.AppendWired(GUID);
                m_Buffer.AppendString(Message);

                m_Buffer.AppendSOH();

                return &m_Buffer;
            }

            //////////////////////////////////////////////////////////////////////////
            //////////////////////////////////////////////////////////////////////////

        } ///< NAMESPACE ROOM
    } ///< NAMESPACE HABBOPACKET
} ///< NAMESPACE STEERSTONE