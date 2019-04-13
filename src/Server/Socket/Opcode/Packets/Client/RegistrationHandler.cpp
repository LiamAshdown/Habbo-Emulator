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
#include "Opcode/Packets/Server/RegistrationPackets.h"
#include "Opcode/Packets/Server/LoginPackets.h"

namespace SteerStone
{    
    void HabboSocket::HandleGDate(std::unique_ptr<ClientPacket> p_Packet)
    {
        HabboPacket::Login::GDate l_Packet;
        SendPacket(l_Packet.Write());
    }
    
    void HabboSocket::HandleApproveUsername(std::unique_ptr<ClientPacket> p_Packet)
    {
        HabboPacket::Registration::ApproveNameReply l_Packet;
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
        HabboPacket::Registration::ApprovePasswordReply l_Packet;
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
        HabboPacket::Registration::ApproveEmailReply l_Packet;
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
        l_Database.GetStatement()->setUInt(10,    sHotel->GetIntConfig(CONFIG_REGISTERATION_CREDITS));
        l_Database.GetStatement()->setUInt(11,    sHotel->GetIntConfig(CONFIG_REGISTERATION_TICKETS));
        l_Database.GetStatement()->setUInt(12,    sHotel->GetIntConfig(CONFIG_REGISTERATION_FILMS));
        l_Database.GetStatement()->setBoolean(13, sHotel->GetBoolConfig(BoolConfigs::CONFIG_REGISTERATION_SOUND));
        l_Database.ExecuteQuery();
    }
}
