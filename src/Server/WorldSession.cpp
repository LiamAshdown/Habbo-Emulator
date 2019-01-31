/*
* Liam Ashdown
* Copyright (C) 2018
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
#include "WorldSession.h"
#include "../Entity/Player/Player.h"
#include "../Server/Packet/Opcodes.h"
#include "../Network/DatabaseManager.h"
#include "../Manager/RoomManager.h"
#include "../Server/Thread/ThreadPool.h"
#include "../Room/Room.h"
#include "../World/World.h"
//-----------------------------------------------//
inline std::string SplitString(std::string string, std::string key)
{
    std::size_t startPosition = string.find(key);

    if (string[startPosition + key.length()] == '=')
    {
        std::string keyString;
        for (std::size_t i = startPosition + (key.length() + 1); i < string.length(); i++)
        {
            if (string[i] == '\r')
                break;

            keyString += string[i];
        }

        return keyString;
    }

    return "0";
}
//-----------------------------------------------//
WorldSession::WorldSession(std::shared_ptr<Socket> socket) : mSocket(socket)
{
    mPlayer = new Player(this);
}
//-----------------------------------------------//
WorldSession::~WorldSession()
{
    delete mPlayer;
}
//-----------------------------------------------//
void WorldSession::SendPacket(const WorldPacket packet)
{
    mSocket->SendPacket(packet);
}
//-----------------------------------------------//
void WorldSession::ExecutePacket(PacketHandlerStruct const & opHandle, std::string& packet, std::vector<std::string> packetStorage)
{
    (this->*opHandle.handler)(packet, packetStorage);
}
//-----------------------------------------------//
void WorldSession::CloseSocket()
{
    mSocket->CloseSocket();
}
//-----------------------------------------------//
std::shared_ptr<Socket> WorldSession::GetSocket()
{
    return mSocket;
}
//-----------------------------------------------//
Player * WorldSession::GetPlayer()
{
    return mPlayer;
}
//-----------------------------------------------//
void WorldSession::SendSystemBroadCast(const std::string& broadcast)
{
    WorldPacket data("# SYSTEMBROADCAST");
    data.AppendCarriage();
    data << (std::string)broadcast;
    data.AppendEndCarriage();
    SendPacket(data.Write()); 
}
//-----------------------------------------------//
uint32 WorldSession::GetAccountId()
{
    return mId;
}
//-----------------------------------------------//
void WorldSession::LogoutPlayer(bool save)
{
    sWorld->RemoveSession(GetPlayer()->GetAccountId());
}
//-----------------------------------------------//
bool WorldSession::InitializePlayerData(const std::string& username)
{
    std::shared_ptr<MySQLConnection> connection = sDBManager->getConnectionPool()->borrow();

    try
    {
        std::shared_ptr<sql::Connection> sql_connection = connection->sql_connection;
        std::shared_ptr<sql::PreparedStatement> statement = std::shared_ptr<sql::PreparedStatement>(sql_connection->prepareStatement("SELECT * FROM users WHERE name = ?"));
        statement->setString(1, username);
        std::shared_ptr<sql::ResultSet> result_set = std::shared_ptr<sql::ResultSet>(statement->executeQuery());
        result_set->next();
        GetPlayer()->mId = result_set->getInt(1);
        GetPlayer()->mUserName = result_set->getString(2);
        GetPlayer()->mEmail = result_set->getString(5);
        GetPlayer()->mCredits = result_set->getInt(4);
        GetPlayer()->mFigure = result_set->getString(6);
        GetPlayer()->mBirthday = result_set->getString(7);
        GetPlayer()->mPhoneNumber = result_set->getString(8);
        GetPlayer()->mCustomData = result_set->getString(9);
        GetPlayer()->mHadReadAgreement = result_set->getString(10);
        GetPlayer()->mSex = result_set->getString(11);
        GetPlayer()->mHasSpecialRights = result_set->getString(13);
        GetPlayer()->mBadgeType = result_set->getString(14);
        GetPlayer()->mInRoom = result_set->getBoolean(15);
        GetPlayer()->mRoomCache = "";
        mPlayerInitialized = true;
        sWorld->AddSession(GetPlayer()->GetAccountId(), this);
        return true;
    }
    catch (sql::SQLException &e)
    {
        sDBManager->printException(e, const_cast<char*>(__FILE__), const_cast<char*>(__FUNCTION__), __LINE__);

        return false;
    }
}
//-----------------------------------------------//
bool WorldSession::IsPlayerInitialized()
{
    return mPlayerInitialized;
}
//-----------------------------------------------//
void WorldSession::SendLoginFailure()
{
    WorldPacket data("# ERROR: login incorrect\r##");
    SendPacket(data.Write());
}
//-----------------------------------------------//