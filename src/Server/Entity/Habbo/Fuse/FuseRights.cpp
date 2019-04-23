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
#include "Database/QueryDatabase.h"

#include "Opcode/Packets/Server/LoginPackets.h"

namespace SteerStone
{
    /// Constructor
    /// @p_Habbo : Fuse rights belonging to habbo
    FuseRights::FuseRights(Habbo* p_Habbo) : m_Habbo(p_Habbo)
    {

    }

    FuseRights::~FuseRights()
    {

    }

    /// LoadFuseRights
    /// Load User Fuse Rights from database
    void FuseRights::LoadFuseRights()
    {
        QueryDatabase l_Database("users");
        l_Database.PrepareQuery("SELECT fuseright FROM rank_fuserights WHERE rank <= ?");
        l_Database.GetStatement()->setUInt(1, m_Habbo->GetRank());
        l_Database.ExecuteQuery();

        if (!l_Database.GetResult())
            return;

        Result* l_Result = l_Database.Fetch();

        do
        {
            m_FuseRights.push_back(l_Result->GetString(1));
            
        } while (l_Result->GetNextResult());
    }

    /// SendFuseRights
    /// Send User fuse rights to client
    void FuseRights::SendFuseRights()
    {
        HabboPacket::Login::FuseRights l_Packet;

        for (auto& l_Itr : m_FuseRights)
            l_Packet.HabboFuseRights.push_back(l_Itr);

        m_Habbo->SendPacket(l_Packet.Write());
    }

    /// HasFuseRight
    /// @p_Fuse : Fuse right we are checking for
    bool FuseRights::HasFuseRight(std::string const p_Fuse)
    {
        for (auto const& l_Itr : m_FuseRights)
        {
            if (l_Itr == p_Fuse)
                return true;
        }
        return false;
    }

} ///< NAMESPACE STEERSTONE