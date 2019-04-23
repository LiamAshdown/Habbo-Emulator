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

#include "NavigatorPackets.h"

namespace SteerStone
{
    namespace HabboPacket
    {
        namespace Navigator
        {
          
            //////////////////////////////////////////////////////////////////////////
            //////////////////////////////////////////////////////////////////////////

            StringBuffer const* CantConnect::Write()
            {
                m_Buffer.AppendWired(ErrorCode);

                m_Buffer.AppendSOH();

                return &m_Buffer;
            }

            //////////////////////////////////////////////////////////////////////////
            //////////////////////////////////////////////////////////////////////////

            StringBuffer const * FavouriteRoomResult::Write()
            {
                /// Handled in FavouriteRoom::ParseSendFavouriteRooms

                m_Buffer.AppendSOH();

                return &m_Buffer;
            }

            //////////////////////////////////////////////////////////////////////////
            //////////////////////////////////////////////////////////////////////////

            StringBuffer const * NoFlatsForUser::Write()
            {
                m_Buffer.AppendString(Name, false);

                m_Buffer.AppendSOH();

                return &m_Buffer;
            }

            //////////////////////////////////////////////////////////////////////////
            //////////////////////////////////////////////////////////////////////////

            StringBuffer const * FlatResults::Write()
            {
                for (auto const& l_Itr : Flats)
                {
                    m_Buffer.AppendStringDelimiter(l_Itr.RoomId, "\t");
                    m_Buffer.AppendStringDelimiter(l_Itr.RoomName, "\t");
                    m_Buffer.AppendStringDelimiter(l_Itr.OwnerName, "\t");
                    m_Buffer.AppendStringDelimiter(l_Itr.AccessType, "\t");
                    m_Buffer.AppendStringDelimiter("x", "\t");
                    m_Buffer.AppendStringDelimiter(l_Itr.VisitorsNow, "\t");
                    m_Buffer.AppendStringDelimiter(l_Itr.VisitorsMax, "\t");
                    m_Buffer.AppendStringDelimiter("null", "\t");
                    m_Buffer.AppendStringDelimiter(l_Itr.Description, "\t");
                    m_Buffer.AppendString("\r", false);
                }

                m_Buffer.AppendSOH();

                return &m_Buffer;
            }

            //////////////////////////////////////////////////////////////////////////
            //////////////////////////////////////////////////////////////////////////

            StringBuffer const * NoFlats::Write()
            {
                m_Buffer.AppendSOH();

                return &m_Buffer;
            }

            //////////////////////////////////////////////////////////////////////////
            //////////////////////////////////////////////////////////////////////////

            StringBuffer const * FlatResultsSearch::Write()
            {
                for (auto const& l_Itr : Flats)
                {
                    m_Buffer.AppendStringDelimiter(l_Itr.RoomId, "\t");
                    m_Buffer.AppendStringDelimiter(l_Itr.RoomName, "\t");
                    m_Buffer.AppendStringDelimiter(l_Itr.OwnerName, "\t");
                    m_Buffer.AppendStringDelimiter(l_Itr.AccessType, "\t");
                    m_Buffer.AppendStringDelimiter("x", "\t");
                    m_Buffer.AppendStringDelimiter(l_Itr.VisitorsNow, "\t");
                    m_Buffer.AppendStringDelimiter(l_Itr.VisitorsMax, "\t");
                    m_Buffer.AppendStringDelimiter("null", "\t");
                    m_Buffer.AppendStringDelimiter(l_Itr.Description, "\t");
                    m_Buffer.AppendString("\r", false);
                }

                m_Buffer.AppendSOH();

                return &m_Buffer;
            }

            //////////////////////////////////////////////////////////////////////////
            //////////////////////////////////////////////////////////////////////////

            StringBuffer const * FlatInfo::Write()
            {
                m_Buffer.AppendWiredBool(AllowSuperUsers);
                m_Buffer.AppendWired(AccessType);
                m_Buffer.AppendWired(RoomId);

                m_Buffer.AppendString(OwnerName);

                m_Buffer.AppendString(Model);
                m_Buffer.AppendString(Name);
                m_Buffer.AppendString(Description);
                m_Buffer.AppendWiredBool(ShowOwnerName);
                m_Buffer.AppendWiredBool(AllowTrading);
                m_Buffer.AppendWiredBool(HasCategory);
                m_Buffer.AppendWired(NowVisitors);
                m_Buffer.AppendWired(MaxVisitors);

                m_Buffer.AppendSOH();

                return &m_Buffer;
            }

            //////////////////////////////////////////////////////////////////////////
            //////////////////////////////////////////////////////////////////////////

            StringBuffer const * FlatCategory::Write()
            {
                m_Buffer.AppendWired(RoomId);
                m_Buffer.AppendWired(CategoryId);

                m_Buffer.AppendSOH();

                return &m_Buffer;
            }

            //////////////////////////////////////////////////////////////////////////
            //////////////////////////////////////////////////////////////////////////

            StringBuffer const * NodeSpaceUsers::Write()
            {
                for (auto const& l_Itr : Names)
                    m_Buffer.AppendString(l_Itr);

                m_Buffer.AppendSOH();

                return &m_Buffer;
            }

} ///< NAMESPACE NAVIGATOR
    } ///< NAMESPACE HABBOPACKET
} ///< NAMESPACE STEERSTONE