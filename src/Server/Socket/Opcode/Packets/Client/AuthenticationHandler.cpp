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
* along with this program.  If not, see <http:///www.gnu.org/licenses/>.
*/

#include "Habbo.h"
#include "Hotel.h"
#include "Common/SHA1.h"
#include "RoomManager.h"
#include "Opcode/Packets/Server/AuthenticationPackets.h"
#include "Opcode/Packets/Server/MiscPackets.h"

namespace SteerStone
{
    void HabboSocket::HandleInitializeCrypto(std::unique_ptr<ClientPacket> p_Packet)
    {
        HabboPacket::Authentication::InitializeCrypto l_Packet;
        SendPacket(l_Packet.Write());
    }
    
    void HabboSocket::HandleGenerateKey(std::unique_ptr<ClientPacket> p_Packet)
    {
        HabboPacket::Authentication::CryptoParameters l_Packet;
        SendPacket(l_Packet.Write());

        HabboPacket::Authentication::AvailableSets l_PacketSet;
        l_PacketSet.UseClubClothing = sConfig->GetBoolDefault("RegisterationHabboClothing", 0);
        SendPacket(l_PacketSet.Write());
    }
    
    void HabboSocket::HandleGDate(std::unique_ptr<ClientPacket> p_Packet)
    {
        HabboPacket::Authentication::GDate l_Packet;
        SendPacket(l_Packet.Write());
    }
    
    void HabboSocket::HandleApproveUsername(std::unique_ptr<ClientPacket> p_Packet)
    {
        HabboPacket::Authentication::ApproveNameReply l_Packet;
        l_Packet.Name = p_Packet->ReadString();

        l_Packet.ErrorCode = ApproveNameError::NAME_VALID;

        QueryDatabase l_Database("users");
        l_Database.PrepareQuery("SELECT user_name FROM account WHERE user_name = ?");
        l_Database.GetStatement()->setString(1, l_Packet.Name.c_str());
        l_Database.ExecuteQuery();

        if (!l_Database.GetResult())
        {
            if (l_Packet.Name.length() > 15)
                l_Packet.ErrorCode = ApproveNameError::NAME_TOO_LONG;
            else if (l_Packet.Name.length() < 3)
                l_Packet.ErrorCode = ApproveNameError::NAME_UNACCEPTABLE_TO_STAFF;
            else
            {
                std::string l_BobbaWords = sConfig->GetStringDefault("RegisterationBobbaWords");

                for (uint8 l_I = 0; l_I < l_BobbaWords.length(); l_I++)
                {
                    if (l_Packet.Name.find(l_BobbaWords[l_I]) != std::string::npos)
                    {
                        l_Packet.ErrorCode = ApproveNameError::NAME_UNACCEPTABLE_TO_STAFF_2;
                        break;
                    }
                }
            }
        }
        else
            l_Packet.ErrorCode = ApproveNameError::NAME_TAKEN;

        SendPacket(l_Packet.Write());
    }
    
    void HabboSocket::HandleApprovePassword(std::unique_ptr<ClientPacket> p_Packet)
    {
        HabboPacket::Authentication::ApprovePasswordReply l_Packet;
        l_Packet.Name = p_Packet->ReadString();
        l_Packet.Password = p_Packet->ReadString();
        l_Packet.ErrorCode = ApprovePasswordError::PASSWORD_VALID;

        if (l_Packet.Password.length() < 6)
            l_Packet.ErrorCode = ApprovePasswordError::PASSWORD_TOO_SHORT;
        else if (l_Packet.Password.length() > 9)
            l_Packet.ErrorCode = ApprovePasswordError::PASSWORD_TOO_LONG;
        else
        {
            std::string l_BobbaWords = sConfig->GetStringDefault("bobbaWords");

            bool l_UsedNumber = false;

            /// Must contain atleast a number 0-9
            for (uint8 l_I = 0; l_I < 10; l_I++)
            {
                if (l_Packet.Password.find(l_BobbaWords[l_I]) == std::string::npos)
                {
                    l_UsedNumber = true;
                    break;
                }
            }

            if (!l_UsedNumber)
                l_Packet.ErrorCode = ApprovePasswordError::PASSWORD_REQUIRES_NUMBERS;

            /// Username similiar to password
            if (strstr(l_Packet.Name.c_str(), l_Packet.Password.c_str()))
                l_Packet.ErrorCode = ApprovePasswordError::PASSWORD_USER_NAME_SIMILIAR;
        }

        SendPacket(l_Packet.Write());
    }
    
    void HabboSocket::HandleApproveEmail(std::unique_ptr<ClientPacket> p_Packet)
    {
        /// TODO; Check if email is valid
        HabboPacket::Authentication::ApproveEmailReply l_Packet;
        SendPacket(l_Packet.Write());
    }
    
    void HabboSocket::HandleRegisteration(std::unique_ptr<ClientPacket> p_Packet)
    {
        std::string l_Username;
        std::string l_Figure;
        std::string l_Gender;
        std::string l_Email;
        std::string l_Password;
        std::string l_Birthday;
        bool l_DirectEmail;

        for (uint8 l_I = 0; l_I < 6; l_I++)
        {
            uint32 l_Id = p_Packet->ReadBase64Int();
            std::string l_Content = p_Packet->ReadString();

            switch (l_Id)
            {
            case 2:
                l_Username = l_Content;
                break;
            case 4:
                l_Figure = l_Content;
                break;
            case 5:
                l_Gender = l_Content;
                break;
            case 7:
                l_Email = l_Content;
                break;
            case 8:
                l_Birthday = l_Content;
                break;
            default:
                break;
            }
        }

        p_Packet->ReadSkip(4);
        l_DirectEmail = p_Packet->ReadWiredBool();
        p_Packet->ReadSkip(1);
        l_Password = p_Packet->ReadString();

        if (l_Gender == "M")
            l_Gender = "Male";
        else
            l_Gender = "Female";

        QueryDatabase l_Database("users");
        l_Database.PrepareQuery("INSERT INTO account(user_name, hash_pass, email, figure, motto, console_motto, direct_mail, birthday, gender, credits, tickets, films, sound_enabled) VALUES(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
        l_Database.GetStatement()->setString(1,   l_Username.c_str());
        l_Database.GetStatement()->setString(2,   (CalculateSHA1Hash(boost::to_upper_copy(l_Username) + ":" + boost::to_upper_copy(l_Password))).c_str());
        l_Database.GetStatement()->setString(3,   l_Email.c_str());
        l_Database.GetStatement()->setString(4,   l_Figure.c_str());
        l_Database.GetStatement()->setString(5,   sConfig->GetStringDefault("RegisterationMotto", "I'm a new user!"));
        l_Database.GetStatement()->setString(6,   sConfig->GetStringDefault("RegisterationConsoleMotto", "I'm looking for friends!"));
        l_Database.GetStatement()->setBoolean(7,  l_DirectEmail);
        l_Database.GetStatement()->setString(8,   l_Birthday.c_str());
        l_Database.GetStatement()->setString(9,   l_Gender.c_str());
        l_Database.GetStatement()->setUInt(10,    sConfig->GetIntDefault("RegisterationCredits", 0));
        l_Database.GetStatement()->setUInt(11,    sConfig->GetIntDefault("RegisterationTickets", 0));
        l_Database.GetStatement()->setUInt(12,    sConfig->GetIntDefault("RegisterationFilms", 0));
        l_Database.GetStatement()->setBoolean(13, sConfig->GetIntDefault("RegisterationSound", 0));
        l_Database.ExecuteQuery();
    }
    
    void HabboSocket::HandleTryLogin(std::unique_ptr<ClientPacket> p_Packet)
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
            HabboPacket::Authentication::LocalisedError l_Packet;
            l_Packet.Error = "Login Incorrect";
            SendPacket(l_Packet.Write());
            return;
        }

        Result* l_Result = l_Database.Fetch();

        /// Check Password
        if (((CalculateSHA1Hash(boost::to_upper_copy(l_Username) + ":" + boost::to_upper_copy(l_Password))).c_str())
            != l_Result->GetString(2))
        {
            HabboPacket::Authentication::LocalisedError l_Packet;
            l_Packet.Error = "Login Incorrect";
            SendPacket(l_Packet.Write());
            return;
        }

        uint32 l_AccountId = l_Result->GetUint32(1);

        /// Success player has logged in
        l_Database.PrepareQuery("SELECT account.id, account.user_name, account.hash_pass, account.email, account.figure, account.pool_figure, account.motto, account.console_motto, account.direct_mail, account.birthday, account.gender, account.credits, account.tickets, account.films, account.sound_enabled, account_badges.badge, account_badges.active, rank_fuserights.rank, rank_fuserights.fuseright FROM account LEFT JOIN account_badges ON account.id = account_badges.id LEFT JOIN rank_fuserights ON account.rank = rank_fuserights.rank WHERE account.id = ?");
        l_Database.GetStatement()->setUInt(1, l_AccountId);
        l_Database.ExecuteQuery();

        if (l_Database.GetResult())
        {
            l_Result = l_Database.Fetch();

            m_Habbo = new Habbo(this);
            m_Habbo->m_Id                   = l_Result->GetUint32(1);
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
            m_Habbo->m_Rank                 = l_Result->GetUint8(17);

            while (l_Result->GetNextResult())
            {
                if (l_Result->GetString(16) != "")
                {
                    HabboBadgesStruct l_HabboBadge;
                    l_HabboBadge.m_Badge    = l_Result->GetString(16);
                    l_HabboBadge.m_IsActive = l_Result->GetBool(17);
                    m_Habbo->m_Badges.push_back(l_HabboBadge);
                }

                /// Double check if user has rank even though it's set in database as default
                if (l_Result->GetUint8(18))
                {
                    HabboFuseRightsData l_HabboFuseRight;
                    l_HabboFuseRight.m_Rank      = l_Result->GetUint8(18);
                    l_HabboFuseRight.m_FuseRight = l_Result->GetString(19);
                    m_Habbo->m_FuseRights.push_back(l_HabboFuseRight);
                }
            }

            m_Habbo->InitializeHabboData();
            m_Habbo->m_Initialized = true;

            HabboPacket::Authentication::Login l_LoginPacket;
            SendPacket(l_LoginPacket.Write());

            m_Habbo->SendFuseRights();

            HabboPacket::Authentication::AvailableSets l_PacketSet;
            l_PacketSet.UseClubClothing = sConfig->GetBoolDefault("RegisterationHabboClothing", 0);
            SendPacket(l_PacketSet.Write());

            HabboPacket::Misc::ServerAlert l_PacketAlert;
            l_PacketAlert.Message = sConfig->GetStringDefault("RegisterationWelcomeMessage");
            SendPacket(l_PacketAlert.Write());

            sHotel->AddHabbo(m_Habbo);
        }
        else
        {
            HabboPacket::Authentication::LocalisedError l_Packet;
            l_Packet.Error = "Internal system error occured";
            SendPacket(l_Packet.Write());
        }
    }
}
