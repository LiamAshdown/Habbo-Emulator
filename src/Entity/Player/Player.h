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

#ifndef _QuadEmu_Player_h_
#define _QuadEmu_Player_h_
#include "SharedDefines.h"
#endif /* _QuadEmu_Common_ */

class WorldSession;
class Player;
class WorldPacket;
class Room;

typedef struct PlayerPosition
{
    uint8 x;
    uint8 y;
    uint8 z;

} PlayerPositionStruct;

typedef struct PendingPurchase
{
    uint32 itemId;
    uint32 itemCredits;
} PendingPurchaseStruct;

class Player
{
    friend class WorldSession;

public:
    Player(WorldSession* session);
    ~Player();
    void SendObjectData();

    // Movement
    void Move(uint8 x, uint8 y);
    int GetPlayerPositionX() const;
    int GetPlayerPositionY() const;
    int GetPlayerPositionZ() const;
    void SetPlayerPosition(const uint8& x, const uint8& y, const uint8& z);
    uint8 CalculateRotation(const uint8& x, const uint8& y, const uint8& toX, const uint8& toY);
    void StopWalking();
    void SetCurrentRoomHeight(const uint8& height);
    uint8 GetCurrentRoomHeight() const;
    bool IsDancing() const;
    void SetIsDancing(bool dancing);

    uint32 GetCredits() const;
    void SetCredits(const uint32& credits);

    std::string GetName() const;
    std::string GetEmail() const;
    std::string GetFigure() const;
    std::string GetBirthday() const;
    std::string GetPhoneNumber() const;
    std::string GetCustomData() const;
    std::string GetReadAgreement() const;
    std::string GetSex() const;
    std::string GetSpecialRights() const;
    std::string GetBadgeType() const;
    uint32 GetAccountId() const;

    std::string GetCache() const;
    void SetCache(const std::string& roomCache);

    WorldPacket GetUserStatus() const;
    WorldPacket GetUpdateStatus() const;

    Room* GetRoom();
    void CreateRoom(std::vector<std::string> room);
    void SetRoomInfo(const std::string& room);
    void SetRoom(Room* room);
    bool IsLeavingRoom() const;
    void SetLeavingRoom(bool leaving);
    void LeaveRoom();
    bool IsInRoom() const;

    void SetPendingPurchase(uint32 itemId, uint32 credits);
    PendingPurchase GetPendingPurchase();

    WorldSession* GetSession() const;

protected:
    WorldSession* mSession;

protected:
    std::string mUserName;
    std::string mEmail;
    uint32 mCredits;
    std::string mFigure;
    std::string mBirthday;
    std::string mPhoneNumber;
    std::string mCustomData;
    std::string mHadReadAgreement;
    std::string mSex;
    std::string mHasSpecialRights;
    std::string mBadgeType;
    bool mInRoom;
    PlayerPositionStruct mPosition;
    std::string mRoomCache;
    Room* mRoom;
    uint8 mCurrentRoomHeight;
    std::string mRoomHeight;
    uint32 mId;
    bool mLeavingRoom;
    uint32 mLastRoomCreated;
    bool mDancing;

    PendingPurchase mPendingItemPurchase;
};

