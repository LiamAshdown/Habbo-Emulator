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
#include <boost/range/adaptor/reversed.hpp>
#include "../../Manager/RoomManager.h"
#include "../../Network/DatabaseManager.h"
#include "../../Manager/ScheduleWalker.h"
#include "../../Server/WorldSession.h"
#include "../../World/World.h"
#include "../../Room/Room.h"
#include "Player.h"
//-----------------------------------------------//
Player::Player(WorldSession* session) : mSession(session)
{
    mInRoom = false;
    mLeavingRoom = false;
    mDancing = false;
    sWorld->IncreasePlayerCount();
}
//-----------------------------------------------//
Player::~Player()
{
    QUAD_LOG_INFO("Deleted player session");
    if (IsInRoom())
        GetRoom()->OnLeave(this);
    sWorld->DecreasePlayerCount();
}
//-----------------------------------------------//
void Player::SendObjectData()
{
    WorldPacket data("# USEROBJECT");
    data.AppendCarriage();
    data << "name=";
    data << (std::string)GetName();
    data.AppendCarriage();
    data << "email=";
    data << (std::string)GetEmail();
    data.AppendCarriage();
    data << "figure=";
    data << (std::string)GetFigure();
    data.AppendCarriage();
    data << "birthday=";
    data << (std::string)GetBirthday();
    data.AppendCarriage();
    data << "phonenumber=";
    data << (std::string)GetPhoneNumber();
    data.AppendCarriage();
    data << "customData=";
    data << (std::string)GetCustomData();
    data.AppendCarriage();
    data << "had_read_agreement=";
    data << (std::string)GetReadAgreement();
    data.AppendCarriage();
    data << "sex=";
    data << (std::string)GetSex();
    data.AppendCarriage();
    data << "country=";
    data << ""; // TODO
    data.AppendCarriage();
    data << "has_special_rights=";
    data << (std::string)GetSpecialRights();
    data.AppendCarriage();
    data << "badge_type=";
    data << (std::string)GetBadgeType();
    data.AppendCarriage();
    data.AppendEndCarriage();
    GetSession()->SendPacket(data.Write());
}
//-----------------------------------------------//
void Player::Move(uint8 x, uint8 y)
{
    sScheduleWalker->ScheduleWalk(this, x, y);
}
//-----------------------------------------------//
bool Player::IsLeavingRoom() const
{
    return mLeavingRoom;
}
//-----------------------------------------------//
void Player::SetLeavingRoom(bool leaving)
{
    mLeavingRoom = leaving;
}
//-----------------------------------------------//
int Player::GetPlayerPositionX() const
{
    return mPosition.x;
}
//-----------------------------------------------//
int Player::GetPlayerPositionY() const
{
    return mPosition.y;
}
//-----------------------------------------------//
int Player::GetPlayerPositionZ() const
{
    return mPosition.z;
}
//-----------------------------------------------//
void Player::SetPlayerPosition(const uint8& x, const uint8& y, const uint8& z)
{
    mPosition.x = x;
    mPosition.y = y;
    mPosition.z = z;
}
//-----------------------------------------------//
void Player::LeaveRoom()
{
    if (IsLeavingRoom())
    {
        WorldPacket data("# STATUS");
        data.AppendSpace();
        data.AppendCarriage();
        data << (std::string)GetName();
        data.AppendSpace();
        data << (uint8)GetPlayerPositionX();
        data.AppendComma();
        data << (uint8)GetPlayerPositionY();
        data.AppendComma();
        data << "99";
        data.AppendComma();
        data << (uint8)GetPlayerPositionZ();
        data.AppendComma();
        data << (uint8)GetPlayerPositionZ();
        data.AppendForwardSlash();
        data.AppendEndCarriage();
        GetRoom()->SendPacketToAll(data.Write());
        
        GetSession()->CloseSocket();
    }
}
//-----------------------------------------------//
uint32 Player::GetCredits() const
{
    return mCredits;
}
//-----------------------------------------------//
void Player::SetCredits(const uint32& credits)
{
    mCredits = credits;
}
//-----------------------------------------------//
std::string Player::GetName() const
{
    return mUserName;
}
//-----------------------------------------------//
std::string Player::GetEmail() const
{
    return mEmail;
}
//-----------------------------------------------//
std::string Player::GetFigure() const
{
    return mFigure;
}
//-----------------------------------------------//
std::string Player::GetBirthday() const
{
    return mBirthday;
}
//-----------------------------------------------//
std::string Player::GetPhoneNumber() const
{
    return mPhoneNumber;
}
//-----------------------------------------------//
std::string Player::GetCustomData() const
{
    return mCustomData;
}
//-----------------------------------------------//
std::string Player::GetReadAgreement() const
{
    return mHadReadAgreement;
}
//-----------------------------------------------//
std::string Player::GetSex() const
{
    return mSex;
}
//-----------------------------------------------//
std::string Player::GetSpecialRights() const
{
    return mHasSpecialRights;
}
//-----------------------------------------------//
std::string Player::GetBadgeType() const
{
    return mBadgeType;
}
//-----------------------------------------------//
std::string Player::GetCache() const
{
    return mRoomCache;
}
//-----------------------------------------------//
void Player::SetCache(const std::string& roomCache)
{
    mRoomCache = roomCache;
}
//-----------------------------------------------//
void Player::SetCurrentRoomHeight(const uint8 & height)
{
    mCurrentRoomHeight = height;
}
//-----------------------------------------------//
uint8 Player::GetCurrentRoomHeight() const
{
    return mCurrentRoomHeight;
}
//-----------------------------------------------//
WorldPacket Player::GetUserStatus() const
{
    WorldPacket data("# USERS");
    data.AppendCarriage();
    data.AppendSpace();
    data << (std::string)GetName();
    data.AppendSpace();

    std::string figure = mFigure;
    boost::algorithm::replace_first(figure, "figure=", "");

    data << (std::string)figure;
    data.AppendSpace();
    data << (uint8)GetPlayerPositionX();
    data.AppendSpace();
    data << (uint8)GetPlayerPositionY();
    data.AppendSpace();
    data << (uint8)GetCurrentRoomHeight();
    data.AppendSpace();
    data << (std::string)GetCustomData();
    data.AppendEndCarriage();
    return data;
}
//-----------------------------------------------//
WorldPacket Player::GetUpdateStatus() const
{
    WorldPacket data("# STATUS \r");
    data << (std::string)GetName();
    data.AppendSpace();
    data << (uint8)GetPlayerPositionX();
    data.AppendComma();
    data << (uint8)GetPlayerPositionY();
    data.AppendComma();
    data << (uint8)GetCurrentRoomHeight();
    data.AppendComma();
    data << (uint8)GetPlayerPositionZ();
    data.AppendComma();
    data << (uint8)GetPlayerPositionZ();
    data.AppendForwardSlash();
    data.AppendEndCarriage();
    return data;
}
//-----------------------------------------------//
uint8 Player::CalculateRotation(const uint8& x, const uint8& y, const uint8& toX, const uint8& toY)
{
    uint8 result = 0;
    if (x > toX && y > toY)
    {
        result = 7;
    }
    else if (x < toX && y < toY)
    {
        result = 3;
    }
    else if (x > toX && y < toY)
    {
        result = 5;
    }
    else if (x < toX && y > toY)
    {
        result = 1;
    }
    else if (x > toX)
    {
        result = 6;
    }
    else if (x < toX)
    {
        result = 2;
    }
    else if (y < toY)
    {
        result = 4;
    }
    else if (x > toY)
    {
        result = 0;
    }

    return result;
}
//-----------------------------------------------//
void Player::StopWalking()
{
    WorldPacket data("# STATUS");
    data.AppendSpace();
    data.AppendCarriage();
    data << (std::string)GetName();
    data.AppendSpace();
    data << (uint8)GetPlayerPositionX();
    data.AppendComma();
    data << (uint8)GetPlayerPositionY();
    data.AppendComma();
    data << (uint8)GetCurrentRoomHeight();
    data.AppendComma();
    data << (uint8)GetPlayerPositionZ();
    data.AppendComma();
    data << (uint8)GetPlayerPositionZ();
    data.AppendForwardSlash();
    if (IsDancing())
        data << (std::string)"dance/";
    data.AppendEndCarriage();
    GetRoom()->SendPacketToAll(data.Write());
}
//------------------------------------------z-----//
void Player::CreateRoom(std::vector<std::string> room)
{
    std::string floor;
    std::string name;
    std::string model;
    std::string state;
    bool showName;

    std::vector<std::string> roomDetails;
    boost::split(roomDetails, room[2], boost::is_any_of("/"));
    floor = room[1].erase(0, 1) + roomDetails[0];
    name = roomDetails[1];
    model = roomDetails[2];
    state = roomDetails[3];
    showName = std::stoi(roomDetails[4]);

    if (name.length() <= 2)
    {
        GetSession()->SendSystemBroadCast("Room name requires atleast 3 characters or more!");
        return;
    }

    std::shared_ptr<MySQLConnection> connection = sDBManager->getConnectionPool()->borrow();
    try
    {
        sql::PreparedStatement* prepareStatement = connection->sql_connection->prepareStatement
        ("INSERT INTO rooms(name, owner_id, state, floor_level, model, show_owner_name, ownerName) VALUES(?, ?, ?, ?, ?, ?, ?)");

        prepareStatement->setString(1, name);
        prepareStatement->setInt(2, mId);
        prepareStatement->setString(3, state);
        prepareStatement->setString(4, floor);
        prepareStatement->setString(5, model);
        prepareStatement->setBoolean(6, showName == true ? 1 : 0);
        prepareStatement->setString(7, GetName());
        prepareStatement->executeQuery();

        // Last inserted id
        std::shared_ptr<sql::Statement> stmnt = std::shared_ptr<sql::Statement>(connection->sql_connection->createStatement()); {
            auto result_set = stmnt->executeQuery("SELECT LAST_INSERT_ID() as id;");
            result_set->next();
            mLastRoomCreated = result_set->getInt("id");
        }

    }
    catch (sql::SQLException &e) 
    {
        sDBManager->printException(e, const_cast<char*>(__FILE__), const_cast<char*>(__FUNCTION__), __LINE__);
    }

    sDBManager->getConnectionPool()->unborrow(connection);

    WorldPacket data("# FLATCREATED");
    data.AppendCarriage();
    data << (uint32)mLastRoomCreated;
    data.AppendSpace();
    data << (std::string)"'s Room";
    data.AppendEndCarriage();
    GetSession()->SendPacket(data.Write());

    sRoomManager->CreateRoom(this, mLastRoomCreated, floor, name, model, state, showName);
}
//-----------------------------------------------//
void Player::SetRoomInfo(const std::string& room)
{
    // TODO; Check with Roommanager if room exists
    // If player updates their flat and did not set a description - we shouldn't update the description, instead update description with original
    // And also to implement is owner wants to show his name or not

    std::vector<std::string> roomDetails;
    boost::split(roomDetails, room, boost::is_any_of("\r"));
    std::string roomDescription = roomDetails[0];
    roomDescription.erase(0, 28);
    std::string roomPassword = roomDetails[1];
    roomPassword.erase(0, 9);

    std::string roomSuperRights = roomDetails[2];
    roomSuperRights.erase(0, 13);

    std::shared_ptr<MySQLConnection> connection = sDBManager->getConnectionPool()->borrow();
    try
    {
        sql::PreparedStatement* prepareStatement = connection->sql_connection->prepareStatement
        ("UPDATE rooms SET description = ?, password = ?, allsuperuser = ? WHERE id = ?");

        prepareStatement->setString(1, roomDescription);
        prepareStatement->setString(2, roomPassword);
        prepareStatement->setBoolean(3, roomSuperRights == "1" ? 1 : 0);
        if (mLastRoomCreated)
            prepareStatement->setInt(4, mLastRoomCreated);
        else
        {
            roomDetails.clear();
            boost::split(roomDetails, room, boost::is_any_of("/"));
            prepareStatement->setInt(4, std::stoi(roomDetails[1]));
        }
        prepareStatement->executeQuery();

        mLastRoomCreated = 0;
    }
    catch (sql::SQLException &e)
    {
        sDBManager->printException(e, const_cast<char*>(__FILE__), const_cast<char*>(__FUNCTION__), __LINE__);
    }
}
//-----------------------------------------------//
void Player::SetRoom(Room* room)
{
    mRoom = room;
    mInRoom = true;
    mRoom->OnEnter(this);
}
//-----------------------------------------------//
Room* Player::GetRoom()
{
    return mRoom;
}
//-----------------------------------------------//
bool Player::IsInRoom() const
{
    return mInRoom;
}
//-----------------------------------------------//
void Player::SetPendingPurchase(uint32 itemId, uint32 credits)
{
    mPendingItemPurchase.itemCredits = credits;
    mPendingItemPurchase.itemId = itemId;
}
//-----------------------------------------------//
PendingPurchase Player::GetPendingPurchase()
{
    return mPendingItemPurchase;
}
//-----------------------------------------------//
WorldSession* Player::GetSession() const
{
    return mSession;
}
//-----------------------------------------------//
uint32 Player::GetAccountId() const
{
    return mId;
}
bool Player::IsDancing() const
{
    return mDancing;
}
//-----------------------------------------------//
void Player::SetIsDancing(bool dancing)
{
    mDancing = dancing;

    WorldPacket data("# STATUS");
    data.AppendSpace();
    data.AppendCarriage();
    data << (std::string)GetName();
    data.AppendSpace();
    data << (uint8)GetPlayerPositionX();
    data.AppendComma();
    data << (uint8)GetPlayerPositionY();
    data.AppendComma();
    data << (uint8)GetCurrentRoomHeight();
    data.AppendComma();
    data << (uint8)GetPlayerPositionZ();
    data.AppendComma();
    data << (uint8)GetPlayerPositionZ();
    data.AppendForwardSlash();
    if (IsDancing())
        data << (std::string)"dance/";
    data.AppendEndCarriage();
    GetRoom()->SendPacketToAll(data.Write());
}
//-----------------------------------------------//