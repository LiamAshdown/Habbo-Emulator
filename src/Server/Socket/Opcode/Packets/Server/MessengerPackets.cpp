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

#include "MessengerPackets.h"

namespace SteerStone
{
    namespace HabboPacket
    {
        namespace Messenger
        {
            //////////////////////////////////////////////////////////////////////////
            //////////////////////////////////////////////////////////////////////////

            StringBuffer const * MessengerInitialize::Write()
            {
                m_Buffer.AppendString(ConsoleMotto);
                m_Buffer.AppendWired(FriendsLimit);
                m_Buffer.AppendWired(FriendsLimit);
                m_Buffer.AppendWired(ClubFriendsLimit);
                m_Buffer.AppendWired(MessengerSize);

                /// Handled in Messenger::ParseMessengerFriends
                m_Buffer.Append(m_SecondaryBuffer);

                m_Buffer.AppendSOH();

                return &m_Buffer;
            }

            //////////////////////////////////////////////////////////////////////////
            //////////////////////////////////////////////////////////////////////////

            StringBuffer const * MessengerUpdate::Write()
            {
                /// Handled in Messenger::ParseMessengerUpdate
                m_Buffer.Append(m_SecondaryBuffer);

                m_Buffer.AppendSOH();


                return &m_Buffer;
            }

            //////////////////////////////////////////////////////////////////////////
            //////////////////////////////////////////////////////////////////////////

            StringBuffer const * MessengerSendFriendRequest::Write()
            {
                m_Buffer.AppendWired(Id);
                m_Buffer.AppendString(Name);

                m_Buffer.AppendSOH();

                return &m_Buffer;
            }

            //////////////////////////////////////////////////////////////////////////
            //////////////////////////////////////////////////////////////////////////

            StringBuffer const * MessengerFindUserResult::Write()
            {
                m_Buffer.AppendString(Messenger);

                /// Handled in Messenger::ParseMessengerFriendRequests
                m_Buffer.Append(m_SecondaryBuffer);

                m_Buffer.AppendSOH();


                return &m_Buffer;
            }

            //////////////////////////////////////////////////////////////////////////
            //////////////////////////////////////////////////////////////////////////

            StringBuffer const * MessengerError::Write()
            {
                m_Buffer.AppendWired(Error);

                m_Buffer.AppendSOH();

                return &m_Buffer;
            }

            //////////////////////////////////////////////////////////////////////////
            //////////////////////////////////////////////////////////////////////////

            StringBuffer const * MessengerRequestBuddyError::Write()
            {
                m_Buffer.AppendWired(Error);

                m_Buffer.AppendSOH();


                return &m_Buffer;
            }

            //////////////////////////////////////////////////////////////////////////
            //////////////////////////////////////////////////////////////////////////

            StringBuffer const * MessengerAddFriend::Write()
            {
                /// Handled in Messenger::ParseMessengerAcceptFriendRequest
                m_Buffer.Append(m_SecondaryBuffer);

                m_Buffer.AppendSOH();


                return &m_Buffer;
            }

            //////////////////////////////////////////////////////////////////////////
            //////////////////////////////////////////////////////////////////////////

            StringBuffer const * MessengerRemoveFriend::Write()
            {
                m_Buffer.AppendWired(FriendsId.size());

                for (auto const& l_Itr : FriendsId)
                    m_Buffer.AppendWired(l_Itr);

                m_Buffer.AppendSOH();

                return &m_Buffer;
            }

            //////////////////////////////////////////////////////////////////////////
            //////////////////////////////////////////////////////////////////////////

} ///< NAMESPACE MESSENGER
    } ///< NAMESPACE HABBOPACKET
} ///< NAMESPACE STEERSTONE
