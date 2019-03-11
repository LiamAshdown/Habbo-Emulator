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
//-----------------------------------------------//
#include "../PlayerSocket.h"
#include "Network/StringBuffer.h"
#include "Database/QueryDatabase.h"
#include "../Entity/Player/Player.h"
#include "Common/SHA1.h"
#include "Config/Config.h"
#include "RoomManager.h"
//-----------------------------------------------//
namespace Quad
{
    //-----------------------------------------------//
    void PlayerSocket::HandleInitializeCrypto(std::unique_ptr<Packet> packet)
    {
        TempBuffer buffer;
        buffer.AppendBase64(SMSG_CRYPTO_PARAMETERS);
        buffer.AppendWired(0);
        buffer.AppendSOH();
        Write((const char*)buffer.GetContents(), buffer.GetSize());
    }
    //-----------------------------------------------//
    void PlayerSocket::HandleGenerateKey(std::unique_ptr<Packet> packet)
    {
        TempBuffer buffer;

        buffer.AppendBase64(SMSG_SESSION_PARAMETERS);
        buffer.AppendWired(0); // VOUCHER_ENABLED
        buffer.AppendWired(0); // REGISTER_REQUIRE_PARENT_EMAIL
        buffer.AppendWired(0); // REGISTER_SEND_PARENT_EMAIL
        buffer.AppendWired(0); // ALLOW_DIRECT_MAIL
        buffer.AppendWired(1); // DATE_FORMAT
        buffer.AppendWired(0); // PARTNER_INTEGRATION_ENABLED
        buffer.AppendWired(1); // ALLOW_PROFILE_EDITING
        buffer.AppendWired(0); // TRACKING_HEADER
        buffer.AppendWired(0); // TUTORIAL_ENABLED
        buffer.AppendSOH();
        Write((const char*)buffer.GetContents(), buffer.GetSize());

        buffer.Clear();

        buffer.AppendBase64(SMSG_AVAILABLE_SETS);
        buffer.AppendString("[100,105,110,115,120,125,130,135,140,145,150,155,160,165,170,175,176,177,178,180,185,190,195,200,205,206,207,210,215,220,225,230,235,240,245,250,255,260,265,266,267,270,275,280,281,285,290,295,300,305,500,505,510,515,520,525,530,535,540,545,550,555,565,570,575,580,585,590,595,596,600,605,610,615,620,625,626,627,630,635,640,645,650,655,660,665,667,669,670,675,680,685,690,695,696,700,705,710,715,720,725,730,735,740,800,801,802,803,804,805,806,807,808,809,810,811,812,813,814,815,816,817,818,819,820,821,822,823,824,825,826,827,828,829,830,831,832,833,834,835,836,837,838,839,840,841,842,843,844,845,846,847,848,849,850,851,852,853,854,855,856,857,858,859,860,861,862,863,864,865,866,867,868,869,870,871,872,873]");
        buffer.AppendSOH();
        Write((const char*)buffer.GetContents(), buffer.GetSize());
    }
    //-----------------------------------------------//
    void PlayerSocket::HandleGDate(std::unique_ptr<Packet> packet)
    {
        TempBuffer buffer;
        buffer.AppendBase64(MSG_GDATE);
        buffer.AppendString(GetDate());
        buffer.AppendSOH();
        Write((const char*)buffer.GetContents(), buffer.GetSize());
    }
    //-----------------------------------------------//
    void PlayerSocket::HandleApproveUsername(std::unique_ptr<Packet> packet)
    {
        std::string username = packet->ReadString();

        QueryDatabase database("users");
        database.PrepareQuery("SELECT user_name FROM accounts WHERE user_name = ?");
        database.GetStatement()->setString(1, username.c_str());
        database.ExecuteResultPrepareQuery();

        ApproveNameError errorCode = ApproveNameError::NAME_VALID;

        if (!database.GetExecuteQueryResult())
        {
            if (username.length() > 15)
                errorCode = ApproveNameError::NAME_TOO_LONG;
            else if (username.length() < 3)
                errorCode = ApproveNameError::NAME_UNACCEPTABLE_TO_STAFF;
            else
            {
                std::string badWords = sConfig->GetStringDefault("BannedWords");

                for (uint8 i = 0; i < badWords.length(); i++)
                {
                    if (username.find(badWords[i]) != std::string::npos)
                    {
                        errorCode = ApproveNameError::NAME_UNACCEPTABLE_TO_STAFF_2;;
                        break;
                    }
                }
            }
        }
        else
            errorCode = ApproveNameError::NAME_TAKEN;

        TempBuffer buffer;
        buffer.AppendBase64(SMSG_APPROVE_NAME_REPLY);
        buffer.AppendWired(errorCode);
        buffer.AppendSOH();
        Write((const char*)buffer.GetContents(), buffer.GetSize());
    }
    //-----------------------------------------------//
    void PlayerSocket::HandleApprovePassword(std::unique_ptr<Packet> packet)
    {
        std::string username = packet->ReadString();
        std::string password = packet->ReadString();

        ApprovePasswordError errorCode = ApprovePasswordError::PASSWORD_VALID;

        if (password.length() < 6)
            errorCode = ApprovePasswordError::PASSWORD_TOO_SHORT;
        else if (password.length() > 9)
            errorCode = ApprovePasswordError::PASSWORD_TOO_LONG;
        else
        {
            std::string badWords = sConfig->GetStringDefault("BannedWords");
            bool usedNumber = false;

            // Must contain atleast a number
            for (uint8 i = 0; i < 10; i++)
            {
                if (password.find(badWords[i]) != std::string::npos)
                {
                    usedNumber = true;
                    break;
                }
            }

            if (!usedNumber)
                errorCode = ApprovePasswordError::PASSWORD_REQUIRES_NUMBERS;

            // Username similiar to password
            if (strstr(username.c_str(), password.c_str()))
                errorCode = ApprovePasswordError::PASSWORD_USER_NAME_SIMILIAR;
        }

        TempBuffer buffer;
        buffer.AppendBase64(SMSG_APPROVE_PASSWORD_REPLY);
        buffer.AppendWired(errorCode);
        buffer.AppendSOH();
        Write((const char*)buffer.GetContents(), buffer.GetSize());
    }
    //-----------------------------------------------//
    void PlayerSocket::HandleApproveEmail(std::unique_ptr<Packet> packet)
    {
        // TODO; Check if email is valid
        TempBuffer buffer;
        buffer.AppendBase64(SMSG_APPROVE_EMAIL_REPLY);
        buffer.AppendSOH();
        Write((const char*)buffer.GetContents(), buffer.GetSize());
    }
    //-----------------------------------------------//
    void PlayerSocket::HandleRegisteration(std::unique_ptr<Packet> packet)
    {
        std::string username;
        std::string figure;
        std::string gender;
        std::string email;
        std::string password;
        std::string birth;
        bool directEmail;

        for (uint8 i = 0; i < 6; i++)
        {
            uint32 id = packet->ReadUInt();
            std::string content = packet->ReadString();

            switch (id)
            {
            case 2:
                username = content;
                break;
            case 4:
                figure = content;
                break;
            case 5:
                gender = content;
                break;
            case 7:
                email = content;
                break;
            case 8:
                birth = content;
                break;
            default:
                break;
            }
        }

        packet->ReadSkip(4);
        directEmail = packet->ReadBool();
        packet->ReadSkip(1);
        password = packet->ReadString();

        if (gender == "M")
            gender = "Male";
        else
            gender = "Female";

        QueryDatabase database("users");
        database.PrepareQuery("INSERT INTO accounts(user_name, hash_pass, email, figure, direct_mail, birthday, gender, credits) VALUES(?, ?, ?, ?, ?, ?, ?, ?)");
        database.GetStatement()->setString(1, username.c_str());
        database.GetStatement()->setString(2, (CalculateSHA1Hash(boost::to_upper_copy(username) + ":" + boost::to_upper_copy(password))).c_str());
        database.GetStatement()->setString(3, email.c_str());
        database.GetStatement()->setString(4, figure.c_str());
        database.GetStatement()->setBoolean(5, directEmail);
        database.GetStatement()->setString(6, birth.c_str());
        database.GetStatement()->setString(7, gender.c_str());
        database.GetStatement()->setUInt(8, sConfig->GetIntDefault("CustomRegCredits", 0));
        database.ExecuteBoolPrepareQuery();
    }
    //-----------------------------------------------//
}
//-----------------------------------------------//