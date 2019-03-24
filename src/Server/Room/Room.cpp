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
#include "HabboSocket.h"
#include "Network/StringBuffer.h"
#include "RoomManager.h"
#include "ItemManager.h"
//-----------------------------------------------//
namespace SteerStone
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
    void Room::EnterRoom(Habbo* player)
    {
        if (std::find(mPlayers.begin(), mPlayers.end(), player) != mPlayers.end())
        {
            LOG_ERROR << "Habbo tried to enter room " << GetId() << " but is already inside room!";
            return;
        }

        player->UpdatePosition(GetRoomModel()->GetDoorX(), GetRoomModel()->GetDoorY(), GetRoomModel()->GetDoorZ(),
            GetRoomModel()->GetDoorOrientation());

        mPlayers.push_back(player);
        SendUserObjects(player);
        mVisitorsNow++;
    }
    //-----------------------------------------------//
    void Room::LeaveRoom(Habbo* player)
    {
        std::vector<Habbo*>::const_iterator& itr = std::find(mPlayers.begin(), mPlayers.end(), player);

        if (itr != mPlayers.end())
        {
            mPlayers.erase(itr);
            mVisitorsMax--;
        }
        else
            LOG_INFO << "Player " << player->GetName() << " tried to leave room but player does not exist in room!";
    }
    //-----------------------------------------------//
    void Room::SendUserObjects(Habbo* player)
    {
        for (std::vector<Habbo*>::const_iterator& itr = mPlayers.begin(); itr != mPlayers.end(); itr++)
        {
            Habbo* roomPlayer = *itr;

            if (roomPlayer->GetId() != player->GetId())
                roomPlayer->ToSocket()->SendPacket(player->GetUserRoomObject());

            player->ToSocket()->SendPacket(roomPlayer->GetUserRoomObject());
            break;
        }
    }
    //-----------------------------------------------//
    void Room::SendObjectsWorld(Habbo * player)
    {
        StringBuffer buffer;
        buffer.AppendBase64(PacketServerHeader::SERVER_OBJECTS_WORLD);
        for (auto& itr : sItemMgr->GetPublicRoomItems(GetModel()))
        {
            PublicItem* item = &itr;

            buffer.AppendStringDelimiter(boost::lexical_cast<std::string>(item->GetId()), " ");
            buffer.AppendStringDelimiter(item->GetSprite(), " ");
            buffer.AppendStringDelimiter(boost::lexical_cast<std::string>(item->GetPositionX()), " ");
            buffer.AppendStringDelimiter(boost::lexical_cast<std::string>(item->GetPositionY()), " ");
            buffer.AppendStringDelimiter(boost::lexical_cast<std::string>(item->GetPositionZ()), " ");
            buffer.AppendStringDelimiter(boost::lexical_cast<std::string>(item->GetRotation()), " ");
            buffer.AppendStringDelimiter(boost::lexical_cast<std::string>(item->GetLength()), " ");
            buffer.AppendString("\r", false);
        }

        buffer.AppendSOH();

        player->ToSocket()->SendPacket(buffer);
    }
    //-----------------------------------------------//
    RoomModel* Room::GetRoomModel()
    {
        return &mRoomModel;
    }
    //-----------------------------------------------//
    uint32 Room::GetId() const
    {
        return m_Id;
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
    uint32 Room::GetCategory() const
    {
        return mCategory;
    }
    //-----------------------------------------------//
    std::string Room::GetName() const
    {
        return m_Name;
    }
    //-----------------------------------------------//
    std::string Room::GetDescription() const
    {
        return mDescription;
    }
    //-----------------------------------------------//
    std::string Room::GetModel() const
    {
        return mModel;
    }
    //-----------------------------------------------//
    std::string Room::GetCcts() const
    {
        return mCcts;
    }
    //-----------------------------------------------//
    uint32 Room::GetWallPaper() const
    {
        return mWallPaper;
    }
    //-----------------------------------------------//
    uint32 Room::GetFloor() const
    {
        return mFloor;
    }
    //-----------------------------------------------//
    bool Room::ShowName() const
    {
        return mShowName;
    }
    //-----------------------------------------------//
    bool Room::GetSuperUsers() const
    {
        return mSuperUsers;
    }
    //-----------------------------------------------//
    std::string Room::GetAccessType() const
    {
        return mAccessType;
    }
    //-----------------------------------------------//
    std::string Room::GetPassword() const
    {
        return m_Password;
    }
    //-----------------------------------------------//
    uint32 Room::GetVisitorsNow() const
    {
        return mVisitorsNow;
    }
    //-----------------------------------------------//
    uint32 Room::GetVisitorsMax() const
    {
        return mVisitorsMax;
    }
    //-----------------------------------------------//
}
//-----------------------------------------------//