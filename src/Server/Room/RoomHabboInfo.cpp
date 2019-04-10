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

#include "Common/SharedDefines.h"
#include "Habbo.h"
#include "Room.h"
#include "Config/Config.h"
#include "Common/Maths.h"
#include "Opcode/Packets/Server/RoomPackets.h"

namespace SteerStone
{
    /// Constructor
    /// @p_Habbo - User which joined the room
    RoomHabboInfo::RoomHabboInfo(Habbo* p_Habbo) : m_Habbo(p_Habbo)
    {
        m_Path = std::make_unique<WayPoints>(m_Habbo, &m_Habbo->GetRoom()->GetRoomModel());
        m_Statuses = Status::STATUS_NONE;
        
        m_WaveTimer = sConfig->GetIntDefault("WaveTimer", 4000);
        m_AFKTimer = sConfig->GetIntDefault("AwayFromKeyboardTimer", 900000);
    }

    /// Deconstructor
    RoomHabboInfo::~RoomHabboInfo()
    {
        m_Path.reset();
    }

    /// AddStatus
    /// @p_Status : Add Status to be processed on next room update
    void RoomHabboInfo::AddStatus(uint32 const p_Status)
    {
        m_Statuses |= p_Status;

        m_UpdateClient = true;
    }

    /// RemoveStatus
    /// @p_Status : Status to be removed
    void RoomHabboInfo::RemoveStatus(uint32 const p_Status)
    {
        m_Statuses &= ~p_Status;

        m_UpdateClient = true;
    }

    /// CreatePath
    /// @p_EndX : End position X
    /// @p_EndY : End position Y
    bool RoomHabboInfo::CreatePath(uint16 const p_EndX, uint16 const p_EndY)
    {
        m_Path->GetPath().clear();

        if (m_Path->CalculatePath(m_Habbo->GetPositionX(), m_Habbo->GetPositionY(), p_EndX, p_EndY))
        {
            m_Path->SetActivePath(true);

            RemoveStatus(Status::STATUS_SITTING);
            AddStatus(Status::STATUS_WALKING);

            m_Path->SetEndPosition(p_EndX, p_EndY); ///< Set end position for WayPoints class to

            /// Remove iteration which is the current user position
            m_Path->GetPath().pop_back();

            return true;
        }

        return false;
    }

    /// ProcessActions 
    /// Process all actions
    /// @p_Diff : Hotel last tick time
    void RoomHabboInfo::ProcessActions(uint32 const p_Diff)
    {
        ProcessNextWayPoint();
        ProcessStatusUpdates();
        CheckTimers(p_Diff);
    }

    /// HasStatus
    /// Check if user has an active status
    /// @p_Status : Status to check
    bool RoomHabboInfo::HasStatus(uint32 p_Status) const
    {
        return (m_Statuses & p_Status) == p_Status ? true : false;
    }

    /// ProcessNextWayPoint
    /// Process next waypoint
    void RoomHabboInfo::ProcessNextWayPoint()
    {
        if (!m_Path->HasActivePath())
            return;

        if (m_Path->GetPath().empty())
        {
            m_Path->SetActivePath(false);
            RemoveStatus(Status::STATUS_WALKING);
            m_Path->CheckForInteractiveObjects();
        }
        else
        {
            Position& l_Position = m_Path->GetPath().back();

            TileInstance* l_CurrTileInstance = m_Habbo->GetRoom()->GetRoomModel().GetTileInstance(l_Position.X, l_Position.Y);
            TileInstance* l_PrevTileInstance = m_Habbo->GetRoom()->GetRoomModel().GetTileInstance(m_Habbo->GetPositionX(), m_Habbo->GetPositionY());

            if (!l_CurrTileInstance)
                return;

            if (l_PrevTileInstance)
                l_PrevTileInstance->SetTileOccupied(false);

            if (!l_CurrTileInstance->CanWalkOnTile())
            {
                /// If there's a next waypoint calculate new nearest waypoint to next waypoint
                if (m_Path->GetPath().size() > 2)
                {
                    if (!m_Path->ReCalculatePath(l_Position = Position({ m_Habbo->GetPositionX(), m_Habbo->GetPositionY() }), (m_Path->GetPath().end() - 2)->X, (m_Path->GetPath().end() - 2)->Y))
                    {
                        if (!CreatePath(m_Path->GetEndPositionX(), m_Path->GetEndPositionY()))
                            return;

                        l_Position = m_Path->GetPath().back();
                    }
                }
                else
                    if (!m_Path->ReCalculatePath(l_Position = Position({ m_Habbo->GetPositionX(), m_Habbo->GetPositionY() }), m_Path->GetEndPositionX(), m_Path->GetEndPositionY()))
                    {
                        if (!CreatePath(m_Path->GetEndPositionX(), m_Path->GetEndPositionY()))
                            return;

                        l_Position = m_Path->GetPath().back();
                    }
            }
            else
                l_CurrTileInstance->SetTileOccupied(true, m_Habbo);

            int16 l_Rotation = Maths::CalculateWalkDirection(m_Habbo->GetPositionX(), m_Habbo->GetPositionY(), l_Position.X, l_Position.Y);

            HabboPacket::Room::UserUpdateStatus l_Packet;
            l_Packet.GUID           = std::to_string(m_Habbo->GetRoomGUID());
            l_Packet.CurrentX       = std::to_string(m_Habbo->GetPositionX());
            l_Packet.CurrentY       = std::to_string(m_Habbo->GetPositionY());
            l_Packet.CurrentZ       = std::to_string(m_Habbo->GetPositionZ());
            l_Packet.BodyRotation   = std::to_string(l_Rotation);
            l_Packet.HeadRotation   = std::to_string(l_Rotation);
            l_Packet.NewX           = std::to_string(l_Position.X);
            l_Packet.NewY           = std::to_string(l_Position.Y);
            l_Packet.NewZ           = std::to_string(l_Position.Z);
            l_Packet.Sitting        = HasStatus(Status::STATUS_SITTING);
            l_Packet.Walking        = HasStatus(Status::STATUS_WALKING);
            l_Packet.Dancing        = HasStatus(Status::STATUS_DANCING);
            l_Packet.Waving         = HasStatus(Status::STATUS_WAVING);
            l_Packet.Swimming       = HasStatus(Status::STATUS_SWIMMING);
            m_Habbo->GetRoom()->SendPacketToAll(l_Packet.Write());

            m_Habbo->UpdatePosition(l_Position.X, l_Position.Y, l_Position.Z, l_Rotation);

            m_Path->GetPath().pop_back();
        } 
    }

    /// ProcessStatusUpdates
    /// Process Habbo Status
    void RoomHabboInfo::ProcessStatusUpdates()
    {
        if (!CanSendStatusUpdate() || HasStatus(Status::STATUS_WALKING))
            return;

        HabboPacket::Room::UserUpdateStatus l_Packet;
        l_Packet.GUID               = std::to_string(m_Habbo->GetRoomGUID());
        l_Packet.CurrentX           = std::to_string(m_Habbo->GetPositionX());
        l_Packet.CurrentY           = std::to_string(m_Habbo->GetPositionY());
        l_Packet.CurrentZ           = std::to_string(m_Habbo->GetPositionZ());
        l_Packet.BodyRotation       = std::to_string(m_Habbo->GetBodyRotation());
        l_Packet.HeadRotation       = std::to_string(m_Habbo->GetHeadRotation());
        l_Packet.Sitting            = HasStatus(Status::STATUS_SITTING);
        l_Packet.Walking            = HasStatus(Status::STATUS_WALKING);
        l_Packet.Dancing            = HasStatus(Status::STATUS_DANCING);
        l_Packet.Waving             = HasStatus(Status::STATUS_WAVING);
        l_Packet.Swimming           = HasStatus(Status::STATUS_SWIMMING);
        m_Habbo->GetRoom()->SendPacketToAll(l_Packet.Write());

        m_UpdateClient = false;
    }

    /// CheckTimers
    /// Check durations of user; Waving, AFK etc..
    void RoomHabboInfo::CheckTimers(uint32 const p_Diff)
    {
        if (HasStatus(Status::STATUS_WAVING))
        {
            if (m_WaveTimer <= p_Diff)
            {
                RemoveStatus(Status::STATUS_WAVING);
                m_WaveTimer = sConfig->GetIntDefault("WaveTimer", 4000);
            }
            else
                m_WaveTimer -= p_Diff;
        }

        if (CanSendStatusUpdate())
            m_AFKTimer = sConfig->GetIntDefault("AwayFromKeyboardTimer", 900000);
        else
        {
            if (m_AFKTimer <= p_Diff)
                m_Habbo->Logout();
            else
                m_AFKTimer -= p_Diff;
        }
    }

    /// ToHabbo
    /// Returns Habbo class
    Habbo* RoomHabboInfo::ToHabbo() const
    {
        return m_Habbo;
    }

} ///< NAMESPACE STEERSTONE