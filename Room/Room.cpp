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
#include "../Server/Packet/WorldPacket.h"
#include "../Entity/Player/Player.h"
#include "../Server/WorldSession.h"
#include "../Server/Packet/Opcodes.h"
#include "../Entity/Item/Item.h"
#include "../World/World.h"
#include "Room.h"
//-----------------------------------------------//
Room::Room()
{
}
//-----------------------------------------------//
void Room::OnEnter(Player* player)
{
    mGetPlayers.push_back(player);
    mNowIn++;

    QUAD_LOG_INFO("Player entered room");
}
//-----------------------------------------------//
void Room::OnLeave(Player * player)
{
    mGetPlayers.erase(std::remove(mGetPlayers.begin(), mGetPlayers.end(), player), mGetPlayers.end());
    mNowIn--;

    QUAD_LOG_INFO("Player left room");
}
//-----------------------------------------------//
void Room::SendPacketToAll(const WorldPacket packet)
{
    for (const auto& itr : mGetPlayers)
    {
        if (!itr)
            QUAD_LOG_ERROR("Trying to send packet to nullptr!");

        itr->GetSession()->SendPacket(packet);
    }
}
//-----------------------------------------------//
void Room::SendUpdateStatusToAll(Player* player)
{
    for (const auto& itr : mGetPlayers)
    {
        player->GetSession()->SendPacket(itr->GetUpdateStatus());
    }
}
//-----------------------------------------------//
void Room::SendUserStatusToAll(Player * player)
{
    for (const auto& itr : mGetPlayers)
    {
        if (itr->GetAccountId() != player->GetAccountId())
            itr->GetSession()->SendPacket(player->GetUserStatus());

        player->GetSession()->SendPacket(itr->GetUserStatus());
    }
}
//-----------------------------------------------//
void Room::SendChatToAll(Player* player, OpcodesList opcode, std::string message)
{
    switch (opcode)
    {
    case OpcodesList::SHOUT:
    {
        boost::erase_first(message, "SHOUT");
        WorldPacket data("SHOUT");
        data.AppendCarriage();
        data.AppendSpace();
        data << (std::string)player->GetName();
        data.AppendSpace();
        data << (std::string)message;
        data.AppendSpace();
        data.AppendEndCarriage();
        SendPacketToAll(data.Write());
    }
    break;
    case OpcodesList::WHISPER:
    {
        boost::erase_first(message, "WHISPER");
        WorldPacket data("WHISPER");
        data.AppendCarriage();
        data.AppendSpace();
        data << (std::string)player->GetName();
        data.AppendSpace();
        data << (std::string)message;
        data.AppendSpace();
        data.AppendEndCarriage();
        SendPacketToAll(data.Write());
    }
    break;
    case OpcodesList::CHAT:
    {
        boost::erase_first(message, "CHAT");
        WorldPacket data("CHAT");
        data.AppendCarriage();
        data.AppendSpace();
        data << (std::string)player->GetName();
        data.AppendSpace();
        data << (std::string)message;
        data.AppendSpace();
        data.AppendEndCarriage();
        SendPacketToAll(data.Write());
    }
    break;
    default:
        break;
    }
}
//-----------------------------------------------//
void Room::SendRoomFurniture(Player* player)
{
    if (GetType() == RoomFlags::PUBLIC_ROOM)
    {
        PublicItemMap::iterator itr = sWorld->mPublicItem.find(mId);
        if (itr != sWorld->mPublicItem.end())
        {
            WorldPacket data("# OBJECTS WORLD 0 ");
            data << (std::string)itr->second.at(0)->mItemModelRoomName;
            for (const auto& itemItr : itr->second)
            {
                data.AppendCarriage();
                data << (uint32)itemItr->mItemId;
                data.AppendSpace();
                data << (std::string)itemItr->m_itemSpriteName;
                data.AppendSpace();
                data << (uint8)itemItr->m_itemPos->x;
                data.AppendSpace(); 
                data << (uint8)itemItr->m_itemPos->y;
                data.AppendSpace();
                data << (uint8)itemItr->m_itemPos->z;
                data.AppendSpace(); 
                data << (uint8)itemItr->mItemRotation;
            }
            data.AppendEndCarriage();
            player->GetSession()->SendPacket(data.Write());
        }
    }
}
//-----------------------------------------------//
void Room::SendRoomHeight(Player* player)
{
    PublicHeightMap::iterator itr = sWorld->mPublicHeight.find(mModel);
    if (itr != sWorld->mPublicHeight.end())
    {
        std::string heightMap = itr->second.heightmap;
        boost::replace_all(heightMap, "\r", " ");
        WorldPacket data("# HEIGHTMAP\r");
        data << itr->second.heightmap;
        data.AppendEndCarriage();
        player->GetSession()->SendPacket(data.Write());
        player->SetCache(heightMap);
        player->SetPlayerPosition(itr->second.x, itr->second.y, itr->second.z);
        player->SetCurrentRoomHeight(itr->second.dir);
    }
}
//-----------------------------------------------//
mPublicHeight Room::GetRoomModel()
{
    PublicHeightMap::iterator itr = sWorld->mPublicHeight.find(mModel);
    if (itr != sWorld->mPublicHeight.end())
        return itr->second;
}
//-----------------------------------------------//
uint8 Room::GetMaxRoomCount() const
{
    return mMaxIn;
}
//-----------------------------------------------//
std::string Room::GetName() const
{
    return mName;
}
//-----------------------------------------------//
void Room::SetRoomName(const std::string roomName)
{
    mName = roomName;
}
//-----------------------------------------------//
uint32 Room::GetRoomId() const
{
    return mId;
}
//-----------------------------------------------//
void Room::SetRoomId(uint32 mId)
{
    mId = mId;
}
//-----------------------------------------------//
std::string Room::GetFloorLevel() const
{
    return mFloorLevel;
}
//-----------------------------------------------//
std::string Room::GetModel() const
{
    return mModel;
}
//-----------------------------------------------//
std::string Room::GetState() const
{
    return mState;
}
//-----------------------------------------------//
uint32 Room::GetOwnerId() const
{
    return mOwnerId;
}
//-----------------------------------------------//
std::string Room::GetOwnerName() const
{
    return mOwnerName;
}
//-----------------------------------------------//
bool Room::GetShowName() const
{
    return mShowOwnerName;
}
//-----------------------------------------------//
uint32 Room::GetRowId() const
{
    return mRowId;
}
//-----------------------------------------------//
uint8 Room::GetType() const
{
    return mType;
}
//-----------------------------------------------//
bool Room::IsHidden() const
{
    return mHidden;
}
//-----------------------------------------------//
bool Room::IsEnabled() const
{
    return mEnabled;
}
//-----------------------------------------------//
std::string Room::GetPassword() const
{
    return mPassword;
}
//-----------------------------------------------//
uint8 Room::GetNowIn() const
{
    return mNowIn;
}
//-----------------------------------------------//
uint8 Room::GetMaxIn() const
{
    return mMaxIn;
}
//-----------------------------------------------//
std::string Room::GetRoomHeightMap() const
{
    return mHeightMap;
}
//-----------------------------------------------//
void Room::SetRoomHeightMap(const std::string roomHeightMap)
{
    mHeightMap = roomHeightMap;
}
//-----------------------------------------------//
std::vector<Player*> Room::GetPlayerStorage() const
{
    return mGetPlayers;
}
//-----------------------------------------------//