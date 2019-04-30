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
#include "Hotel.h"
#include "Common/SHA1.h"
#include "Opcode/Packets/Server/LoginPackets.h"
#include "Opcode/Packets/Server/MiscPackets.h"
#include "Opcode/Packets/Server/PursePackets.h"

#include "Common/Maths.h"

namespace SteerStone
{
    void HabboSocket::HandleTryLogin(ClientPacket* p_Packet)
    {
        std::string l_Username = p_Packet->ReadString();
        std::string l_Password = p_Packet->ReadString();

        QueryDatabase l_Database("users");
        l_Database.PrepareQuery("SELECT id, hash_pass FROM account WHERE user_name = ?");
        l_Database.GetStatement()->setString(1, l_Username.c_str());
        l_Database.ExecuteQuery();

        /// Check user exists
        if (!l_Database.GetResult())
        {
            HabboPacket::Misc::LocalisedError l_Packet;
            l_Packet.Error = "Login Incorrect";
            SendPacket(l_Packet.Write());
            return;
        }

        Result* l_Result = l_Database.Fetch();

        /// Check Password
        if (((CalculateSHA1Hash(boost::to_upper_copy(l_Username) + ":" + boost::to_upper_copy(l_Password))).c_str())
            != l_Result->GetString(2))
        {
            HabboPacket::Misc::LocalisedError l_Packet;
            l_Packet.Error = "Login Incorrect";
            SendPacket(l_Packet.Write());
            return;
        }

        uint32 l_AccountId = l_Result->GetUint32(1);

        /// Success player has logged in
        l_Database.PrepareQuery("SELECT id, user_name, hash_pass, email, figure, pool_figure, motto, console_motto, direct_mail, birthday, gender, credits, tickets, films, sound_enabled, allow_friend_requests, rank FROM account WHERE id = ?");
        l_Database.GetStatement()->setUInt(1, l_AccountId);
        l_Database.ExecuteQuery();

        if (l_Database.GetResult())
        {
            l_Result = l_Database.Fetch();

            m_Habbo = new Habbo(this);
            m_Habbo->m_Id                   = Maths::GetRandomUint32(1, 9999999);
            m_Habbo->m_Name                 = l_Result->GetString(2);
            m_Habbo->m_Password             = l_Result->GetString(3);
            m_Habbo->m_Email                = l_Result->GetString(4);
            m_Habbo->m_Figure               = l_Result->GetString(5);
            m_Habbo->m_PoolFigure           = l_Result->GetString(6);
            m_Habbo->m_Motto                = l_Result->GetString(7);
            m_Habbo->m_ConsoleMotto         = l_Result->GetString(8);
            m_Habbo->m_DirectMail           = l_Result->GetBool(9);
            m_Habbo->m_Birthday             = l_Result->GetString(10);
            m_Habbo->m_Gender               = l_Result->GetString(11);
            m_Habbo->m_Credits              = l_Result->GetUint32(12);
            m_Habbo->m_Tickets              = l_Result->GetUint32(13);
            m_Habbo->m_Films                = l_Result->GetUint32(14);
            m_Habbo->m_SoundEnabled         = l_Result->GetBool(15);
            m_Habbo->m_AcceptFriendRequests = l_Result->GetBool(16);
            m_Habbo->m_Rank                 = l_Result->GetUint16(17);
            m_Habbo->InitializeHabboData();
            m_Habbo->m_Initialized          = true;

            m_Habbo->SendFuseRights();

            HabboPacket::Login::LoginOK l_LoginPacket;
            SendPacket(l_LoginPacket.Write());

            HabboPacket::Login::AvailableSets l_PacketSet;
            l_PacketSet.UseClubClothing = sHotel->GetBoolConfig(BoolConfigs::CONFIG_REGISTERATION_HABBO_CLOTHING);
            SendPacket(l_PacketSet.Write());

            HabboPacket::Misc::ServerAlert l_PacketAlert;
            l_PacketAlert.Message = sConfig->GetStringDefault("RegisterationWelcomeMessage");
            SendPacket(l_PacketAlert.Write());

            sHotel->AddHabbo(m_Habbo);
        }
        else
        {
            HabboPacket::Misc::LocalisedError l_Packet;
            l_Packet.Error = "Internal system error occured";
            SendPacket(l_Packet.Write());
        }
    }

    void HabboSocket::HandleGetAvailableBadges(ClientPacket* p_Packet)
    {
        m_Habbo->SendAccountBadges();
    }

    void HabboSocket::HandleGenerateKey(ClientPacket* p_Packet)
    {
        HabboPacket::Login::CryptoParameters l_Packet;
        l_Packet.Parameters[SessionParameters::PARAMETER_COPPA]                 = sHotel->GetBoolConfig(BoolConfigs::CONFIG_PARAMETER_COPPA) ? '2' : '0';
        l_Packet.Parameters[SessionParameters::PARAMETER_VOUCHER]               = sHotel->GetBoolConfig(BoolConfigs::CONFIG_PARAMETER_VOUCHER) ? '1' : '0';
        l_Packet.Parameters[SessionParameters::PARAMETER_REQUIRE_PARENT_EMAIL]  = sHotel->GetBoolConfig(BoolConfigs::CONFIG_PARAMETER_REQUIRE_PARENT_EMAIL) ? '1' : '0';
        l_Packet.Parameters[SessionParameters::PARAMETER_SEND_PARENT_EMAIL]     = sHotel->GetBoolConfig(BoolConfigs::CONFIG_PARAMETER_SEND_PARENT_EMAIL) ? '1' : '0';
        l_Packet.Parameters[SessionParameters::PARAMETER_DIRECT_MAIL]           = sHotel->GetBoolConfig(BoolConfigs::CONFIG_PARAMETER_DIRECT_MAIL) ? '1' : '0';
        l_Packet.Parameters[SessionParameters::PARAMETER_INTEGRATION_ENALBED]   = sHotel->GetBoolConfig(BoolConfigs::CONFIG_PARAMETER_INTEGRATION) ? '1' : '0';
        l_Packet.Parameters[SessionParameters::PARAMETER_DATE_FORMAT]           = sConfig->GetStringDefault("ParameterDateFormat", "dd-mm-yyy");
        l_Packet.Parameters[SessionParameters::PARAMETER_PROFILE_EDITING]       = sHotel->GetBoolConfig(BoolConfigs::CONFIG_PARAMETER_PROFILE_EDITING) ? '1' : '0';
        l_Packet.Parameters[SessionParameters::PARAMETER_TRACKING_HEADER]       = sHotel->GetBoolConfig(BoolConfigs::CONFIG_PARAMETER_TRACKING_HEAD) ? '1' : '0';
        l_Packet.Parameters[SessionParameters::PARAMETER_TUTORIAL]              = sHotel->GetBoolConfig(BoolConfigs::CONFIG_PARAMETER_TUTORIAL) ? '1' : '0';
        SendPacket(l_Packet.Write());

        HabboPacket::Login::AvailableSets l_PacketSet;
        l_PacketSet.UseClubClothing = sHotel->GetBoolConfig(BoolConfigs::CONFIG_REGISTERATION_HABBO_CLOTHING);
        SendPacket(l_PacketSet.Write());
    }

    void HabboSocket::HandleInitializeCrypto(ClientPacket* p_Packet)
    {
        HabboPacket::Login::InitializeCrypto l_Packet;
        SendPacket(l_Packet.Write());
    }

    void HabboSocket::HandleGetAccountPreferences(ClientPacket* p_Packet)
    {
        m_Habbo->SendAccountPreferences();
    }

    void HabboSocket::HandlePong(ClientPacket* p_Packet)
    {
        m_Habbo->m_Ponged = true;
    }

    void HabboSocket::HandleGetInfoRetrieve(ClientPacket* p_Packet)
    {
        if (m_Habbo && m_Habbo->IsInitialized())
            m_Habbo->SendHabboObject();
    }

} ///< NAMESPACE STEERSTONE
