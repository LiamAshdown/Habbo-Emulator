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
#include "Room.h"
#include "Player.h"
#include "PlayerSocket.h"
#include "Network/StringBuffer.h"
#include "RoomManager.h"
#include "ItemManager.h"
//-----------------------------------------------//
namespace Quad
{
    //-----------------------------------------------//
    Room::Room()
    {
    }
    //-----------------------------------------------//
    Room::~Room()
    {
    }
    //-----------------------------------------------//
    uint32 Room::GetId() const
    {
        return mId;
    }
    //-----------------------------------------------//
    uint16 Room::GetServerPort() const
    {
        return mServerPort;
    }
    //-----------------------------------------------//
    std::string Room::GetName() const
    {
        return mName;
    }
    //-----------------------------------------------//
    std::string Room::GetPassword() const
    {
        return mPassword;
    }
    //-----------------------------------------------//
    std::string Room::GetOwnerName() const
    {
        return mOwnerName;
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
    uint8 Room::GetType() const
    {
        return mType;
    }
    //-----------------------------------------------//
    bool Room::IsEnabled() const
    {
        return mEnabled;
    }
    //-----------------------------------------------//
    bool Room::IsShowOwnerName() const
    {
        return mShowOwnerName;
    }
    //-----------------------------------------------//
    bool Room::IsSuperUser() const
    {
        return mSuperUser;
    }
    //-----------------------------------------------//
    uint32 Room::GetNowIn() const
    {
        return mNowIn;
    }
    //-----------------------------------------------//
    uint32 Room::GetMaxIn() const
    {
        return mMaxIn;
    }
    //-----------------------------------------------//
    void Room::AddPlayer(Player* player)
    {
        std::vector<Player*>::iterator itr = std::find(mPlayers.begin(), mPlayers.end(), player);

        if (itr != mPlayers.end())
        {
            LOG_ERROR << "Tried to add player: " << player->GetName() << " but already exists in room!";
            return;
        }

        mPlayers.push_back(player);

        // Send Room Height to player
        SendRoomHeight(player);
        SendRoomFurniture(player);
    }
    //-----------------------------------------------//
    void Room::RemovePlayer(Player* player)
    {
        std::vector<Player*>::iterator itr = std::find(mPlayers.begin(), mPlayers.end(), player);

        if (itr != mPlayers.end())
        {
            mPlayers.erase(itr);
            return;
        }
        
        LOG_ERROR << "Tried to remove player: " << player->GetName() << " but doesn't exist in room!";
    }
    //-----------------------------------------------//
    void Room::SendRoomHeight(Player* player)
    {
        if (std::shared_ptr<RoomModels> roomModel = sRoomMgr->GetRoomModel(player->GetRoom()->GetModel()))
        {
            std::string roomHeight = roomModel->sHeightMap;
            boost::replace_all(roomHeight, " ", "\r");

            StringBuffer buffer;
            buffer << (std::string)"# HEIGHTMAP\r";
            buffer << (std::string)roomHeight;
            buffer.AppendEndCarriage();
            player->UpdatePosition(roomModel->sDoorX, roomModel->sDoorY, roomModel->sDoorZ, roomModel->sOrientation);
            player->ToSocket()->SendPacket((char*)buffer.GetContents(), buffer.GetSize());
        }
        else
            player->ToSocket()->CloseSocket();
    }
    //-----------------------------------------------//
    void Room::SendRoomFurniture(Player * player)
    {
        if (GetType() == RoomFlag::ROOM_TYPE_PUBLIC)
        {
            StringBuffer buffer;
            buffer << (std::string)"# OBJECTS WORLD 0 ";
            buffer << (std::string)GetModel();

            for (auto& itr : sItemMgr->GetRoomPublicItems(player->ToSocket()->GetPort()))
            {
                buffer.AppendCarriage();
                buffer << (uint32)itr->GetId();
                buffer.AppendSpace();
                buffer << (std::string)itr->GetSprite();
                buffer.AppendSpace();
                buffer << (uint8)itr->GetPositionX();
                buffer.AppendSpace();
                buffer << (uint8)itr->GetPositionY();
                buffer.AppendSpace();
                buffer << (uint8)itr->GetPositionZ();
                buffer.AppendSpace();
                buffer << (uint8)itr->GetRotation();
            }

            buffer.AppendEndCarriage();
            player->ToSocket()->SendPacket((char*)buffer.GetContents(), buffer.GetSize());
        }
    }
    //-----------------------------------------------//
}
//-----------------------------------------------//