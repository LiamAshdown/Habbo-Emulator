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

#include "AuthenticationPackets.h"

namespace SteerStone
{
    namespace HabboPacket
    {
        namespace Authentication
        {
            /// SERVER_CRYPTO_PARAMETERS packet builder
            StringBuffer const* InitializeCrypto::Write()
            {
                m_Buffer.AppendWired(1);
                m_Buffer.AppendWired(0);
                m_Buffer.AppendSOH();

                return &m_Buffer;
            }

            //////////////////////////////////////////////////////////////////////////
            //////////////////////////////////////////////////////////////////////////

            /// SERVER_SESSION_PARAMETERS packet builder
            StringBuffer const * CryptoParameters::Write()
            {
                /// TODO; Find out what these does
                m_Buffer.AppendWired(1);
                m_Buffer.AppendWired(1);
                m_Buffer.AppendWired(1);
                m_Buffer.AppendWired(1);
                m_Buffer.AppendWired(1);
                m_Buffer.AppendWired(1);
                m_Buffer.AppendWired(1);
                m_Buffer.AppendWired(2);
                m_Buffer.AppendWired(1);
                m_Buffer.AppendWired(1);
                m_Buffer.AppendWired(1);
                m_Buffer.AppendWired(1);
                m_Buffer.AppendString("dd-MM-yyyy", false);
                m_Buffer.AppendSOH();

                return &m_Buffer;

            }

            //////////////////////////////////////////////////////////////////////////
            //////////////////////////////////////////////////////////////////////////


            StringBuffer const * AvailableSets::Write()
            {
                if (UseClubClothing)
                    m_Buffer.AppendString("[100,105,110,115,120,125,130,135,140,145,150,155,160,165,170,175,176,177,178,180,185,190,195,200,205,206,207,210,215,220,225,230,235,240,245,250,255,260,265,266,267,270,275,280,281,285,290,295,300,305,500,505,510,515,520,525,530,535,540,545,550,555,565,570,575,580,585,590,595,596,600,605,610,615,620,625,626,627,630,635,640,645,650,655,660,665,667,669,670,675,680,685,690,695,696,700,705,710,715,720,725,730,735,740]");
                else
                    m_Buffer.AppendString("[100,105,110,115,120,125,130,135,140,145,150,155,160,165,170,175,176,177,178,180,185,190,195,200,205,206,207,210,215,220,225,230,235,240,245,250,255,260,265,266,267,270,275,280,281,285,290,295,300,305,500,505,510,515,520,525,530,535,540,545,550,555,565,570,575,580,585,590,595,596,600,605,610,615,620,625,626,627,630,635,640,645,650,655,660,665,667,669,670,675,680,685,690,695,696,700,705,710,715,720,725,730,735,740,800,801,802,803,804,805,806,807,808,809,810,811,812,813,814,815,816,817,818,819,820,821,822,823,824,825,826,827,828,829,830,831,832,833,834,835,836,837,838,839,840,841,842,843,844,845,846,847,848,849,850,851,852,853,854,855,856,857,858,859,860,861,862,863,864,865,866,867,868,869,870,871,872,873]");

                m_Buffer.AppendSOH();

                return &m_Buffer;
            }

            //////////////////////////////////////////////////////////////////////////
            //////////////////////////////////////////////////////////////////////////

            StringBuffer const * GDate::Write()
            {
                m_Buffer.AppendString(GetDate());
                m_Buffer.AppendSOH();

                return &m_Buffer;
            }


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

            StringBuffer const * ApproveEmailReply::Write()
            {
                m_Buffer.AppendSOH();

                return &m_Buffer;
            }

            //////////////////////////////////////////////////////////////////////////
            //////////////////////////////////////////////////////////////////////////

            StringBuffer const * LocalisedError::Write()
            {
                m_Buffer.AppendString(Error);
                m_Buffer.AppendSOH();

                return &m_Buffer;
            }

            //////////////////////////////////////////////////////////////////////////
            //////////////////////////////////////////////////////////////////////////

            StringBuffer const * Login::Write()
            {
                m_Buffer.AppendSOH();

                return &m_Buffer;
            }

            //////////////////////////////////////////////////////////////////////////
            //////////////////////////////////////////////////////////////////////////

} ///< NAMESPACE AUTHENTICATION
    } ///< NAMESPACE HABBOPACKET
} ///< NAMESPACE STEERSTONE