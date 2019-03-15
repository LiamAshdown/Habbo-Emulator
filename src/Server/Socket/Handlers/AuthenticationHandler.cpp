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
#include "World.h"
//-----------------------------------------------//
namespace Quad
{
    //-----------------------------------------------//
    void PlayerSocket::HandleInitializeCrypto(std::unique_ptr<Packet> packet)
    {
        StringBuffer buffer;
        buffer.AppendBase64(OpcodesServer::SMSG_CRYPTO_PARAMETERS);
        buffer.AppendWired(1);
        buffer.AppendWired(0);
        buffer.AppendSOH();
        SendPacket((const char*)buffer.GetContents(), buffer.GetSize());
    }
    //-----------------------------------------------//
    void PlayerSocket::HandleGenerateKey(std::unique_ptr<Packet> packet)
    {
        StringBuffer buffer;

        buffer.AppendBase64(SMSG_SESSION_PARAMETERS);

        buffer.AppendWired(6);
        buffer.AppendWired(0);
        buffer.AppendWired(1);
        buffer.AppendWired(1);
        buffer.AppendWired(1);
        buffer.AppendWired(3);
        buffer.AppendWired(0);
        buffer.AppendWired(2);
        buffer.AppendWired(1);
        buffer.AppendWired(4);
        buffer.AppendWired(1);
        buffer.AppendWired(5);
        buffer.AppendString("dd-MM-yyyy", false);
        buffer.AppendSOH();
        SendPacket((const char*)buffer.GetContents(), buffer.GetSize());

        buffer.Clear();

        buffer.AppendBase64(OpcodesServer::SMSG_AVAILABLE_SETS);
        buffer.AppendString("[100,105,110,115,120,125,130,135,140,145,150,155,160,165,170,175,176,177,178,180,185,190,195,200,205,206,207,210,215,220,225,230,235,240,245,250,255,260,265,266,267,270,275,280,281,285,290,295,300,305,500,505,510,515,520,525,530,535,540,545,550,555,565,570,575,580,585,590,595,596,600,605,610,615,620,625,626,627,630,635,640,645,650,655,660,665,667,669,670,675,680,685,690,695,696,700,705,710,715,720,725,730,735,740,800,801,802,803,804,805,806,807,808,809,810,811,812,813,814,815,816,817,818,819,820,821,822,823,824,825,826,827,828,829,830,831,832,833,834,835,836,837,838,839,840,841,842,843,844,845,846,847,848,849,850,851,852,853,854,855,856,857,858,859,860,861,862,863,864,865,866,867,868,869,870,871,872,873]");
        buffer.AppendSOH();
        SendPacket((const char*)buffer.GetContents(), buffer.GetSize());
    }
    //-----------------------------------------------//
    void PlayerSocket::HandleGDate(std::unique_ptr<Packet> packet)
    {
        StringBuffer buffer;
        buffer.AppendBase64(OpcodesClient::MSG_GDATE);
        buffer.AppendString(GetDate());
        buffer.AppendSOH();
        SendPacket((const char*)buffer.GetContents(), buffer.GetSize());
    }
    //-----------------------------------------------//
    void PlayerSocket::HandleApproveUsername(std::unique_ptr<Packet> packet)
    {
        std::string username = packet->ReadString();

        QueryDatabase database("users");
        database.PrepareQuery("SELECT user_name FROM accounts WHERE user_name = ?");
        database.GetStatement()->setString(1, username.c_str());
        database.ExecuteQuery();

        ApproveNameError errorCode = ApproveNameError::NAME_VALID;

        if (!database.GetResult())
        {
            if (username.length() > 15)
                errorCode = ApproveNameError::NAME_TOO_LONG;
            else if (username.length() < 3)
                errorCode = ApproveNameError::NAME_UNACCEPTABLE_TO_STAFF;
            else
            {
                std::string badWords = sConfig->GetStringDefault("BobbaWords");

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

        StringBuffer buffer;
        buffer.AppendBase64(OpcodesServer::SMSG_APPROVE_NAME_REPLY);
        buffer.AppendWired(errorCode);
        buffer.AppendSOH();
        SendPacket((const char*)buffer.GetContents(), buffer.GetSize());
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
            std::string badWords = sConfig->GetStringDefault("BobbaWords");
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

        StringBuffer buffer;
        buffer.AppendBase64(OpcodesServer::SMSG_APPROVE_PASSWORD_REPLY);
        buffer.AppendWired(errorCode);
        buffer.AppendSOH();
        SendPacket((const char*)buffer.GetContents(), buffer.GetSize());
    }
    //-----------------------------------------------//
    void PlayerSocket::HandleApproveEmail(std::unique_ptr<Packet> packet)
    {
        // TODO; Check if email is valid
        StringBuffer buffer;
        buffer.AppendBase64(OpcodesServer::SMSG_APPROVE_EMAIL_REPLY);
        buffer.AppendSOH();
        SendPacket((const char*)buffer.GetContents(), buffer.GetSize());
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
            uint32 id = packet->ReadBase64Int();
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
        directEmail = packet->ReadWiredBool();
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
        database.GetStatement()->setUInt(8, sConfig->GetIntDefault("RegisterationCredits", 0));
        database.ExecuteQuery();
    }
    //-----------------------------------------------//
    void PlayerSocket::HandleTryLogin(std::unique_ptr<Packet> packet)
    {
        std::string username = packet->ReadString();
        std::string password = packet->ReadString();

        QueryDatabase database("users");
        database.PrepareQuery("SELECT id, hash_pass FROM accounts WHERE user_name = ?");
        database.GetStatement()->setString(1, username.c_str());
        database.ExecuteQuery();

        // User doesn't exist
        if (!database.GetResult())
        {
            StringBuffer buffer;
            buffer.AppendBase64(OpcodesServer::SMSG_MODERATOR_ALERT);
            buffer.AppendString("Login Incorrect: Wrong Username");
            buffer.AppendSOH();
            SendPacket((const char*)buffer.GetContents(), buffer.GetSize());
            return;
        }

        Field* fields = database.Fetch();

        // Check Password
        if (((CalculateSHA1Hash(boost::to_upper_copy(username) + ":" + boost::to_upper_copy(password))).c_str())
            != fields->GetString(2))
        {
            StringBuffer buffer;
            buffer.AppendBase64(OpcodesServer::SMSG_MODERATOR_ALERT);
            buffer.AppendString("Login Incorrect: Wrong Password");
            buffer.AppendSOH();
            SendPacket((const char*)buffer.GetContents(), buffer.GetSize());
            return;
        }

        uint32 accountId = fields->GetUint32(1);

        // Check if account is banned
        database.PrepareQuery("SELECT ban_date, unban_date, ban_reason FROM account_banned WHERE "
            "id = ? AND active = 1 AND (unban_date > UNIX_TIMESTAMP()) OR unban_date = ban_date");
        database.GetStatement()->setUInt(1, accountId);
        database.ExecuteQuery();

        if (database.GetResult())
        {
            fields = database.Fetch();

            StringBuffer buffer;
            buffer.AppendBase64(OpcodesServer::SMSG_NOTICE);
            buffer.AppendString("Banned Reason: ");
            buffer.AppendString(fields->GetString(3));
            buffer.AppendSOH();
            SendPacket((const char*)buffer.GetContents(), buffer.GetSize());
            return;
        }

        // Success player has logged in
        database.PrepareQuery("SELECT id, user_name, hash_pass, email, figure, pool_figure, motto, console_motto, direct_mail, birthday, gender, credits, tickets, films FROM accounts WHERE id = ?");
        database.GetStatement()->setUInt(1, accountId);
        database.ExecuteQuery();

        database.Release();

        if (database.GetResult())
        {
            fields = database.Fetch();

            mPlayer = new Player(this);
            mPlayer->mId = fields->GetUint32(1);
            mPlayer->mName = fields->GetString(2);
            mPlayer->mPassword = fields->GetString(3);
            mPlayer->mEmail = fields->GetString(4);
            mPlayer->mFigure = fields->GetString(5);
            mPlayer->mPoolFigure = fields->GetString(6);
            mPlayer->mMotto = fields->GetString(7);
            mPlayer->mConsoleMotto = fields->GetString(8);
            mPlayer->mDirectMail = fields->GetBool(9);
            mPlayer->mBirthday = fields->GetString(10);
            mPlayer->mGender = fields->GetString(11);
            mPlayer->mCredits = fields->GetUint32(12);
            mPlayer->mTickets = fields->GetUint32(13);
            mPlayer->mFilms = fields->GetUint32(14);
            mPlayer->mInitialized = true;

            StringBuffer buffer;
            buffer.AppendBase64(2); // TODO; find out packet name
            buffer.AppendWired(0);
            buffer.AppendSOH();

            buffer.AppendBase64(3); // TODO; find out packet name
            buffer.AppendSOH();

            buffer.AppendBase64(8); // TODO; find out packet name
            buffer.AppendString("[100, 105, 110, 115, 120, 125, 130, 135, 140, 145, 150, 155, 160, 165, 170, 175, 176, 177, 178, 180, 185, 190, 195, 200, 205, 206, 207, 210, 215, 220, 225, 230, 235, 240, 245, 250, 255, 260, 265, 266, 267, 270, 275, 280, 281, 285, 290, 295, 300, 305, 500, 505, 510, 515, 520, 525, 530, 535, 540, 545, 550, 555, 565, 570, 575, 580, 585, 590, 595, 596, 600, 605, 610, 615, 620, 625, 626, 627, 630, 635, 640, 645, 650, 655, 660, 665, 667, 669, 670, 675, 680, 685, 690, 695, 696, 700, 705, 710, 715, 720, 725, 730, 735, 740, 800, 801, 802, 803, 804, 805, 806, 807, 808, 809, 810, 811, 812, 813, 814, 815, 816, 817, 818, 819, 820, 821, 822, 823, 824, 825, 826, 827, 828, 829, 830, 831, 832, 833, 834, 835, 836, 837, 838, 839, 840, 841, 842, 843, 844, 845, 846, 847, 848, 849, 850, 851, 852, 853, 854, 855, 856, 857, 858, 859, 860, 861, 862, 863, 864, 865, 866, 867, 868, 869, 870, 871, 872, 873]");
            buffer.AppendSOH();

            buffer.AppendBase64(OpcodesServer::SMSG_ALERT);
            buffer.AppendString(sConfig->GetStringDefault("WelcomeMessage"));
            buffer.AppendSOH();

            SendPacket((const char*)buffer.GetContents(), buffer.GetSize());

            sWorld->AddPlayer(mPlayer->GetId(), mPlayer);
        }
    }
}
//-----------------------------------------------//