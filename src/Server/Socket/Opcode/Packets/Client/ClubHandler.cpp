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

#include "HabboSocket.h"
#include "Habbo.h"

namespace SteerStone
{
    void HabboSocket::HandleGetClub(std::unique_ptr<ClientPacket> p_Packet)
    {
        m_Habbo->SendClubStatus();
    }

    void HabboSocket::HandleBuyHabboClub(std::unique_ptr<ClientPacket> p_Packet)
    {
        std::string l_Subscription = p_Packet->ReadString();
        uint16 l_SubscriptionType = p_Packet->ReadWiredUint();

        int16 l_Days = 0;
        int16 l_Price = 0;

        switch (l_SubscriptionType)
        {
            case 1:
            {
                l_Days = 31;
                l_Price = 25;
            }
            break;

            case 2:
            {
                l_Days = 93;
                l_Price = 60;
            }
            break;

            case 3:
            {
                l_Days = 186;
                l_Price = 105;
            }
            break;

            default:
                break;
        }

        m_Habbo->m_Club.AddSubscription(l_Days, l_Price);
    }
} ///< NAMESPACE STEERSTONE
