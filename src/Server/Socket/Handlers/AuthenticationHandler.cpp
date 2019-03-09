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
    void PlayerSocket::HandleVersionCheck(std::unique_ptr<Packet> packet)
    {
        if (packet->sBody[0] == "client002")
        {
            // TODO; Figure out how SECREY_KEY works
            StringBuffer buffer;
            buffer << "# ENCRYPTION_OFF\r##";
            buffer << "# SECRET_KEY\r##";
            SendPacket((char*)buffer.GetContents(), buffer.GetSize());
        }
    }
    //-----------------------------------------------//
    void PlayerSocket::HandleRegisteration(std::unique_ptr<Packet> packet)
    {
        QueryDatabase database("users");
        database.PrepareQuery("INSERT INTO accounts(user_name, hash_pass, email, figure, direct_mail, birthday, phone_number, mission, has_read_agreement, sex, country, credits) VALUES(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
        database.GetStatement()->setString(1, SplitString(packet->sFullBody, "name").c_str());
        database.GetStatement()->setString(2, CalculateSHA1Hash(boost::to_upper_copy<std::string>(SplitString(packet->sFullBody, "name")) + ":" + boost::to_upper_copy<std::string>(SplitString(packet->sFullBody, "password")).c_str()));
        database.GetStatement()->setString(3, SplitString(packet->sFullBody, "email").c_str());
        database.GetStatement()->setString(4, SplitString(packet->sFullBody, "figure").c_str());
        database.GetStatement()->setBoolean(5, boost::lexical_cast<bool>(SplitString(packet->sFullBody, "directMail").c_str()));
        database.GetStatement()->setString(6, SplitString(packet->sFullBody, "birthday").c_str());
        database.GetStatement()->setString(7, SplitString(packet->sFullBody, "phonenumber").c_str());
        database.GetStatement()->setString(8, SplitString(packet->sFullBody, "customData").c_str());
        database.GetStatement()->setBoolean(9, boost::lexical_cast<bool>(SplitString(packet->sFullBody, "has_read_agreement").c_str()));
        database.GetStatement()->setString(10, SplitString(packet->sFullBody, "sex").c_str());
        database.GetStatement()->setString(11, SplitString(packet->sFullBody, "country").c_str());
        database.GetStatement()->setUInt(12, sConfig->GetIntDefault("CustomRegCredits", 0));
        database.ExecuteBoolPrepareQuery();
    }
    //-----------------------------------------------//
    void PlayerSocket::HandleApproveUsername(std::unique_ptr<Packet> packet)
    {
        QueryDatabase database("users");
        database.PrepareQuery("SELECT user_name FROM accounts WHERE user_name = ?");
        database.GetStatement()->setString(1, packet->sFullBody.c_str());
        database.ExecuteResultPrepareQuery();

        if (!database.GetExecuteQueryResult())
        {
            StringBuffer buffer;
            buffer << (std::string)"# NAME_APPROVED\r##";
            SendPacket((char*)buffer.GetContents(), buffer.GetSize());
        }
        else
        {
            StringBuffer buffer;
            buffer << (std::string)"# ERROR: Name already exists!\r##";
            SendPacket((char*)buffer.GetContents(), buffer.GetSize());
        }
    }
    //-----------------------------------------------//
    void PlayerSocket::HandleLogin(std::unique_ptr<Packet> packet)
    {
        QueryDatabase database("users");
        database.PrepareQuery("SELECT * FROM accounts WHERE user_name = ?");
        database.GetStatement()->setString(1, packet->sBody[0].c_str());
        database.ExecuteResultPrepareQuery();

        if (!database.GetExecuteQueryResult())
        {
            LOG_ERROR << "User: " << packet->sBody[0] << " does not exist in accounts database!";
            CloseSocket();
            return;
        }

        Field* fields = database.Fetch();

        // Check if passwords match
        if (fields->GetString(3) == 
            CalculateSHA1Hash(boost::to_upper_copy<std::string>(packet->sBody[0]) + ":" + boost::to_upper_copy<std::string>(packet->sBody[1])))
        {
            mPlayer.reset(new Player(this));

            mPlayer->mId = fields->GetUint32(1);
            mPlayer->mName = fields->GetString(2);
            mPlayer->mPassword = fields->GetString(3);
            mPlayer->mEmail = fields->GetString(4);
            mPlayer->mFigure = fields->GetString(5);
            mPlayer->mDirectMail = fields->GetBool(6);
            mPlayer->mBirthday = fields->GetString(7);
            mPlayer->mPhoneNumber = fields->GetString(8);
            mPlayer->mMission = fields->GetString(9);
            mPlayer->mReadAgreement = fields->GetBool(10);
            mPlayer->mSex = fields->GetString(11);
            mPlayer->mCountry = fields->GetString(12);
            mPlayer->mCredits = fields->GetUint32(13);
            mPlayer->mInitialized = true;
            
            database.PrepareQuery("UPDATE accounts SET last_logged_in = now() WHERE user_name = ?");
            database.GetStatement()->setString(1, mPlayer->GetName());
            database.ExecuteBoolPrepareQuery();

            // If size is more than 1, this means we are entering a room
            if (packet->sBody.size() > 2)
            {
                if (std::shared_ptr<Room> room = sRoomMgr->GetRoom(GetPort()))
                {
                    mPlayer->SetRoom(room);
                    room->AddPlayer(mPlayer);
                }
            }
        }
        else
        {
            StringBuffer buffer;
            buffer << (std::string)"# ERROR: Incorrect password\r##";
            SendPacket((char*)buffer.GetContents(), buffer.GetSize());
            CloseSocket();
        }
    }
    //-----------------------------------------------//
    void PlayerSocket::HandleClientIP(std::unique_ptr<Packet> packet)
    {
        QueryDatabase database("users");
        database.PrepareQuery("INSERT INTO account_client_ip_log(ip_address, client_ip) VALUES (?, ?)");
        database.GetStatement()->setString(1, GetRemoteAddress());
        database.GetStatement()->setString(2, packet->sFullBody);
        database.ExecuteBoolPrepareQuery();
    }
    //-----------------------------------------------//
}
//-----------------------------------------------//