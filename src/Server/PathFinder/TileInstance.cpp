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

#include "Habbo.h"
#include "PathFinder.h"
#include "RoomWalkWay.h"

namespace SteerStone
{
    /// Constructor
    /// @p_X : Tile position X
    /// @p_Y : Tile position Y
    TileInstance::TileInstance(int16 const p_X, int16 const p_Y) : m_TileX(p_X), m_TileY(p_Y), m_TileHeight(0), m_TileState(TileState::TILE_STATE_CLOSED),
        m_WalkWay(nullptr), m_Item(nullptr)
    {
        m_Habbo.reset();
    }

    /// Deconstructor
    TileInstance::~TileInstance()
    {
    }

    /// AddItem
    /// @p_Item : Item being added to tile
    void TileInstance::AddItem(Item* p_Item)
    {
        m_Item = p_Item;
    }

    /// GetItem
    Item* TileInstance::GetItem()
    {
        if (m_Item)
            return m_Item;
        else
            return nullptr;
    }

    /// GetTileState
    /// Returns current tile state
    int16 TileInstance::GetTileState() const
    {
        return m_TileState;
    }

    /// GetTileHeight
    /// Returns max tile height
    int16 TileInstance::GetTileHeight() const
    {
        return m_TileHeight;
    }

    /// SetTileOccupied
    /// Set the tile is being used by another habbo or object
    /// @p_Occupied : Set tile is being occupied or not
    /// @p_Habbo : Habbo user being added to the tile
    void TileInstance::SetTileOccupied(bool p_Occupied, Habbo* p_Habbo /*= nullptr*/)
    {
        if (p_Occupied)
            m_Habbo = p_Habbo;
        else
        {
            if (p_Habbo)
                if (p_Habbo->GetRoom())
                    ExecuteTriggerLeave(p_Habbo, p_Habbo->GetRoom(), GetItem());

            m_Habbo.reset();
        }
    }

    /// AddWalkWay
    /// @p_WalkWay : Walkway tile being added to tile
    void TileInstance::AddWalkWay(WalkWay* p_WalkWay)
    {
        m_WalkWay = p_WalkWay;
    }

    /// GetWalkWay
    /// Check if tile is a walk way
    WalkWay* TileInstance::GetWalkWay()
    {
        if (m_WalkWay)
            return m_WalkWay;
        else
            return nullptr;
    }

    /// CanWalkOnTile
    /// Can Habbo walk on tile
    /// @p_Extra : If true do extra checks
    bool TileInstance::CanWalkOnTile(bool p_Extra /*= false*/)
    {
        if (m_Habbo.is_initialized())
        {
            if (Habbo* l_Habbo = m_Habbo.get())
            {
                if (GetTilePositionX() != l_Habbo->GetPositionX() && GetTilePositionY() != l_Habbo->GetPositionY()) ///< User is no longer on the tile
                    SetTileOccupied(false, l_Habbo);
                else
                    return false;
            }
            else
                SetTileOccupied(false);
        }
        else
        {
            if (Item* l_Item = GetItem())
                if (!l_Item->CanBeWalkedOn(p_Extra))
                    return false;
        }

        return true;
    }

    /// AddTrigger
    /// Add Trigger to tile
    /// @p_BaseTriggerEvent : Trigger Event which is being added to tile
    void TileInstance::AddTrigger(BaseTriggerEvent * p_BaseTriggerEvent)
    {
        m_Trigger.push_back(p_BaseTriggerEvent);
    }

    /// ExecuteTriggerJoin
    /// Execute Trigger if exists
    /// @p_Habbo : Habbo user which activated the event
    /// @p_Room : Habbo user which is inside the room
    /// @p_Item : Item which may be apart of the trigger
    void TileInstance::ExecuteTriggerJoin(Habbo* p_Habbo, std::shared_ptr<Room> p_Room /*= nullptr*/, Item* p_Item /*= nullptr*/)
    {
        for (auto const& l_Itr : m_Trigger)
        {
            l_Itr->OnTriggerEventJoin(p_Habbo, p_Room, p_Item);
        }
    }

    /// ExecuteTriggerJoin
    /// Execute Trigger if exists
    /// @p_Habbo : Habbo user which left the event
    /// @p_Room : Habbo user which is inside the room
    /// @p_Item : Item which may be apart of the trigger
    void TileInstance::ExecuteTriggerLeave(Habbo* p_Habbo, std::shared_ptr<Room> p_Room /*= nullptr*/, Item* p_Item /*= nullptr*/)
    {
        for (auto const& l_Itr : m_Trigger)
        {
            l_Itr->OnTriggerEventLeave(p_Habbo, p_Room, p_Item);
        }
    }
} ///< NAMESPACE STEERSTONE
