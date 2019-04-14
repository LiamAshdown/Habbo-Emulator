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
#include <cmath>

#include "Opcode/Packets/Server/ClubPackets.h"

namespace SteerStone
{
    /// Constructor
    /// @p_Habbo : Habbo class being added
    HabboClub::HabboClub(Habbo* p_Habbo) : m_Habbo(p_Habbo)
    {
        m_Subscribed            = false;
        m_DaysMonthRemaining    = 0;
        m_RemainingDays         = 0;
        m_PrePaidMonths         = 0;
        m_DaysPassed            = 0;
        m_MonthsPassed          = 0;

    }

    /// Deconstructor
    HabboClub::~HabboClub()
    {

    }

    /// AddSubscription
    /// Add Club Subscription to Habbo
    /// @p_Days : Amount of days to subscribed for
    /// @p_Price : Price of club subscription
    void HabboClub::AddSubscription(uint32 p_Days, uint32 p_Price)
    {
        /// Do we have enough credits?
        //if (m_Habbo->GetCredits() < p_Price)
            //return;

        /// Is user already subscribed to habbo club? if so add more days
        if (IsSubscribed())
        {
            QueryDatabase l_Database("users");
            l_Database.PrepareQuery("UPDATE club_subscription SET subscription_expires = subscription_expires + ?  WHERE id = ?");
            l_Database.GetStatement()->setUInt(1, ((p_Days * 60) * 60) * 24);
            l_Database.GetStatement()->setUInt(2, m_Habbo->GetId());
            l_Database.ExecuteQuery();
        }
        else ///< Else first time user subscribed to habbo club
        {
            QueryDatabase l_Database("users");
            l_Database.PrepareQuery("INSERT INTO club_subscription (id, first_subscription, subscription_expires) VALUES (?, ?, ?)");
            l_Database.GetStatement()->setUInt(1, m_Habbo->GetId());
            l_Database.GetStatement()->setUInt(2, std::time(0));
            l_Database.GetStatement()->setUInt(3, std::time(0) + (((p_Days * 60) * 60) * 24));
            l_Database.ExecuteQuery();

            /// Set user rank to habbo club
            if (m_Habbo->GetRank() < AccountRank::HABBO_CLUB)
                m_Habbo->SetRank(AccountRank::HABBO_CLUB);
        }

        LoadSubscription();

    }

    /// LoadSubscription
    /// Load Habbo Subscription
    void HabboClub::LoadSubscription()
    {
        QueryDatabase l_Database("users");
        l_Database.PrepareQuery("SELECT first_subscription, subscription_expires FROM club_subscription WHERE id = ?");
        l_Database.GetStatement()->setUInt(1, m_Habbo->GetId());
        l_Database.ExecuteQuery();

        /// Is user already subscribed?
        if (l_Database.GetResult())
        {
            Result* l_Result = l_Database.Fetch();

            m_RemainingDays      = ((l_Result->GetUint64(2) - std::time(0)) / 60 / 60 / 24);
            m_DaysMonthRemaining = ((m_RemainingDays - 1) % 31) + 1;
            m_PrePaidMonths      = (m_RemainingDays - m_DaysMonthRemaining) / 31;
            m_MonthsPassed       = ((std::time(0) - l_Result->GetUint64(1)) / 60 / 60 / 24 / 31);

            /// If our remaining days is lower than 0, this means user is no longer subscribed
            if (m_RemainingDays < 0)
            {
                m_Habbo->SetRank(AccountRank::HABBO_NORMAL);
                m_Habbo->SendAccountBadges(); ///< Update our badges for user to recieve HC badge
                
                l_Database.ClearParameters();

                l_Database.PrepareQuery("DELETE FROM club_subscription WHERE id = ?");
                l_Database.GetStatement()->setUInt(1, m_Habbo->GetId());
                l_Database.ExecuteQuery();

                /// Initialize default values
                m_DaysMonthRemaining    = 0;
                m_RemainingDays         = 0;
                m_PrePaidMonths         = 0;
                m_DaysPassed            = 0;
                m_MonthsPassed          = 0;
               
            }
            else
                m_Subscribed = true; ///< User is subscribed to habbo club
        }

        HabboPacket::Club::ClubInfo l_Packet;
        l_Packet.ProductName   = "club_habbo"; 
        l_Packet.DaysLeft      = m_DaysMonthRemaining;
        l_Packet.ElaspedMonths = m_MonthsPassed;
        l_Packet.PrepaidMonths = m_PrePaidMonths;
        l_Packet.ResponseFlag  = 1;
        m_Habbo->ToSocket()->SendPacket(l_Packet.Write());
    }
    
    /// IsSubscribed
    /// Returns if user is subscribed to habbo club
    bool HabboClub::IsSubscribed() const
    {
        return m_Subscribed;
    }
    
} ///< NAMESPACE STEEwTONE