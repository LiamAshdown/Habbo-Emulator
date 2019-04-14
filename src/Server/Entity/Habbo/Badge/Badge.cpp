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

#include "Habbo.h"
#include "RoomManager.h"
#include "Database/QueryDatabase.h"

#include "Opcode/Packets/Server/LoginPackets.h"
#include "Opcode/Packets/Server/RoomPackets.h"

namespace SteerStone
{
    /// Constructor
    /// @p_Habbo : Fuse rights belonging to habbo
    Badge::Badge(Habbo* p_Habbo) : m_Habbo(p_Habbo)
    {
    }

    /// Deconstructor
    Badge::~Badge()
    {

    }

    /// LoadBadges
    /// Load user badges
    void Badge::LoadBadges()
    {
        QueryDatabase l_Database("users");
        l_Database.PrepareQuery("SELECT badge FROM account_badges WHERE rank <= ?");
        l_Database.GetStatement()->setUInt(1, m_Habbo->GetRank());
        l_Database.ExecuteQuery();

        if (!l_Database.GetResult())
            return;

        Result* l_Result = l_Database.Fetch();

        /// Set our first result as our current badge
        m_CurrentBadge = std::make_tuple(l_Result->GetString(1), true);

        do
        {
            m_Badges[l_Result->GetString(1)] = true;
        } while (l_Result->GetNextResult());
    }

    /// SendBadges
    /// Send user badges to client
    void Badge::SendBadges()
    {
        HabboPacket::Login::AvailableBadges l_Packet;
        l_Packet.CurrentBadge = GetCurrentBadgeName();
        l_Packet.BadgeVisible = IsCurrentBadgeVisible();

        for (auto& l_Itr : m_Badges)
        {
            l_Packet.Badges[l_Itr.first] = l_Itr.second;
        }

        m_Habbo->ToSocket()->SendPacket(l_Packet.Write());
    }

    /// SendSetBadge
    /// Update new current badge
    /// @p_Badge : New badge we are setting to
    /// @p_Visible : Is new badge visible
    void Badge::SendSetBadge(std::string const p_Badge, bool const p_Visible)
    {
        /// Does user badge exist?
        auto const& l_Itr = m_Badges.find(p_Badge);
        if (l_Itr != m_Badges.end())
        {
            m_CurrentBadge = std::make_tuple(l_Itr->first, l_Itr->second = p_Visible);

            /// Inform players inside room we have changed badge
            HabboPacket::Room::UseBadge l_Packet;
            l_Packet.GUID = m_Habbo->GetRoomGUID();
            l_Packet.Badge = GetCurrentBadgeName();
            l_Packet.BadgeVisible = IsCurrentBadgeVisible();
            m_Habbo->GetRoom()->SendPacketToAll(l_Packet.Write());
        }
    }

    /// IsBadgeVisible
    /// Is badge visible to other users
    bool Badge::IsCurrentBadgeVisible() const
    {
        return std::get<1>(m_CurrentBadge);
    }

    /// GetCurrentBadge
    /// Returns current badge user is wearing
    std::string Badge::GetCurrentBadgeName() const
    {
        return std::get<0>(m_CurrentBadge);
    }

} ///< NAMESPACE STEERSTONE