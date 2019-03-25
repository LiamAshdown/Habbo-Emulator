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

namespace SteerStone
{
    namespace HabboPacket
    {
        namespace HabboInfo
        {
            //////////////////////////////////////////////////////////////////////////
            //////////////////////////////////////////////////////////////////////////

            StringBuffer const * FuseRights::Write()
            {
                for (auto const& itr : HabboFuseRights)
                    m_Buffer.AppendString(itr);

                m_Buffer.AppendSOH();

                return &m_Buffer;
            }

            //////////////////////////////////////////////////////////////////////////
            //////////////////////////////////////////////////////////////////////////

            StringBuffer const * HabboObject::Write()
            {
                m_Buffer.AppendString(Id);
                m_Buffer.AppendString(Name);
                m_Buffer.AppendString(Figure);
                m_Buffer.AppendString(Gender);
                m_Buffer.AppendString(Motto);
                m_Buffer.AppendWired(Tickets);
                m_Buffer.AppendString(PoolFigure);
                m_Buffer.AppendWired(Films);
                m_Buffer.AppendSOH();

                return &m_Buffer;
            }


            //////////////////////////////////////////////////////////////////////////
            //////////////////////////////////////////////////////////////////////////

            StringBuffer const * AccountPreferences::Write()
            {
                m_Buffer.AppendWiredBool(SoundEnabled);
                m_Buffer.AppendWiredBool(TutorialFinished);
                m_Buffer.AppendSOH();

                return &m_Buffer;
            }

            //////////////////////////////////////////////////////////////////////////
            //////////////////////////////////////////////////////////////////////////

            StringBuffer const * AvailableBadges::Write()
            {
                m_Buffer.AppendWired(Badges.size());

                for (auto const& itr : Badges)
                    m_Buffer.AppendString(itr);

                m_Buffer.AppendWired(ActiveBadges);
                m_Buffer.AppendWired(Badges.empty() ? false : true);
                m_Buffer.AppendSOH();

                return &m_Buffer;
            }

            //////////////////////////////////////////////////////////////////////////
            //////////////////////////////////////////////////////////////////////////

            StringBuffer const * CreditBalance::Write()
            {
                m_Buffer.AppendString(Credits + ".0");
                m_Buffer.AppendSOH();

                return &m_Buffer;
            }

            //////////////////////////////////////////////////////////////////////////
            //////////////////////////////////////////////////////////////////////////

        } ///< NAMESPACE HABBOINFO
    } ///< NAMESPACE HABBOPACKET
} ///< NAMESPACE STEERSTONE