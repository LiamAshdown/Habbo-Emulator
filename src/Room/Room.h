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

#ifndef _QuadEmu_Room_h_
#define _QuadEmu_Room_h_
#include "../Common/SharedDefines.h"
#endif /* _QuadEmu_Room_ */

enum RoomFlags
{
    PLAYER_ROOM = 0,
    PUBLIC_ROOM = 1,
};

class Player;
class WorldPacket;
enum OpcodesList : uint16;
struct mPublicHeight;

class Room
{
public:
    friend class RoomManager;
    friend class World;
    Room();
    ~Room() {}

    // Script Wrappers
    void OnEnter(Player* player);
    void OnLeave(Player* player);

    // Packet Wrapper
    void SendPacketToAll(const WorldPacket packet);
    void SendUpdateStatusToAll(Player* player);
    void SendUserStatusToAll(Player* player);
    void SendChatToAll(Player* player, OpcodesList opcode, std::string message);
    void SendRoomFurniture(Player* player);
    void SendRoomHeight(Player* player);
    mPublicHeight GetRoomModel();

    // Player Wrapper
    uint8 GetMaxRoomCount() const;
    std::string GetName() const;
    void SetRoomName(const std::string roomName);
    
    // Room Wrapper
    uint32 GetRoomId() const;
    void SetRoomId(uint32 mId);

    std::string GetFloorLevel() const;
    std::string GetModel() const;
    std::string GetState() const;
    uint32 GetOwnerId() const;
    std::string GetOwnerName() const;
    bool GetShowName() const;
    uint32 GetRowId() const;
    uint8 GetType() const;
    bool IsHidden() const;
    bool IsEnabled() const;
    std::string GetPassword() const;
    
    uint8 GetNowIn() const;
    uint8 GetMaxIn() const;

    std::string GetRoomHeightMap() const;
    void SetRoomHeightMap(const std::string roomHeightMap);
    
    // Storage
    std::vector<Player*> GetPlayerStorage() const;

protected:
    std::vector<Player*> mGetPlayers;
    uint32 mId;
    uint32 mRowId;
    std::string mName;
    std::string mOwnerName;
    uint8 mType;
    bool mEnabled;
    bool mHidden;
    int32 mOwnerId;
    std::string mDescription;
    std::string mPassword;
    std::string mState;
    bool mShowOwnerName;
    bool mAllSuperUser;
    uint8 mNowIn;
    uint8 mMaxIn;
    std::string mModel;
    std::string mFloorLevel;
    std::string mHeightMap;
    uint32 mPort;
};