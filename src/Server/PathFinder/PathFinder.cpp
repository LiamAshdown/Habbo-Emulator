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

#include "PathFinder.h"
#include <chrono>

namespace SteerStone
{
    /// Constructor
    /// @p_Grid : Multi-dimensional array which stores the heightmap
    PathFinder::PathFinder(GridArray const& p_TileGrid, GridArray const& p_HeightGrid) : m_TileGrid(p_TileGrid), m_HeightGrid(p_HeightGrid)
    {
        /// 8 directions we can go
        m_Directions =
        {
            { 0, 1 }, { 1, 0 }, { 0, -1 }, { -1, 0 },
            { -1, -1 }, { 1, 1 }, { -1, 1 }, { 1, -1 }
        };

        /// Reserve our storage, which prevents from having to resize every time we push a new element = performance increase!
        m_OpenList.reserve(200);        
        m_ClosedList.reserve(200);
    }

    /// Deconstructor
    PathFinder::~PathFinder()
    {
    }

    /// CalculatePath
    /// @p_StartX : Start Position X
    /// @p_StartY : Start Position Y
    /// @p_EndX : End Position X
    /// @p_EndY : End Position Y
    bool PathFinder::CalculatePath(int16 const& p_StartX, int16 const& p_StartY, int16 const & p_EndX, int16 const & p_EndY)
    {
        /// Check if the destination tile is valid
        if (!CheckDestination(Position({ p_EndX, p_EndY })))
            return false;

        /// Insert our first node into the storage
        m_OpenList.push_back(new Node(p_StartX, p_StartY));

        while (!m_OpenList.empty())
        {
            /// Retreive our first node from the storage
            auto& l_CurrentItr = m_OpenList.begin();
            m_Current = *l_CurrentItr;

            /// Go through our OpenList storage and find the lowest f cost
            /// The lowest means its the closest to the end position
            for (auto& l_Itr = m_OpenList.begin(); l_Itr != m_OpenList.end(); l_Itr++)
            {
                if ((*l_Itr)->GetTotalCost() <= m_Current->GetTotalCost())
                {
                    m_Current = *l_Itr;
                    l_CurrentItr = l_Itr;
                }
            }

            /// Check if we had reached our final destination
            if (m_Current->GetPosition().X == p_EndX && m_Current->GetPosition().Y == p_EndY)
                break;

            /// Insert our node into the ClosedList (already evaluted) 
            /// and erase from our OpenList (to be evaluted)
            m_ClosedList.push_back(m_Current);
            m_OpenList.erase(l_CurrentItr);

            /// Loop through all 8 directions
            for (int16 l_I = 0; l_I < m_Directions.size(); l_I++)
            {
                /// Create our new future position
                Position l_FuturePosition;
                l_FuturePosition.X = m_Current->GetPosition().X + m_Directions[l_I].X;
                l_FuturePosition.Y = m_Current->GetPosition().Y + m_Directions[l_I].Y;

                /// Check if our future position has any collision
                if (!CheckValidTile(l_FuturePosition, m_Current->GetPosition()) || DoesNodeExist(m_ClosedList, l_FuturePosition))
                    continue;

                /// Work out our G Cost
                /// If we are moving diagnol our cost would be 14 since diagnol will be the closest
                /// to the end point
                uint32 l_GCost = m_Current->GetGCost() + (l_I < 4 ? 10 : 14);

                /// Does the node already exist in the OpenList?
                if (Node* l_Node = DoesNodeExist(m_OpenList, l_FuturePosition))
                {
                    /// If our node is lower than the one in the storage
                    /// update the node with our newest g cost
                    if (l_GCost < m_Current->GetGCost())
                    {
                        l_Node->SetGCost(l_GCost);
                        l_Node->SetParentNode(m_Current);
                    }
                }
                else
                {
                    /// Node doesn't exist, create a new node and push it into our open list to be evaluted
                    Node* l_NewNode = new Node(l_FuturePosition.X, l_FuturePosition.Y, m_Current);
                    l_NewNode->SetGCost(l_GCost);
                    l_NewNode->SetHCost(CalculateHeuristic(l_NewNode, p_EndX, p_EndY)); ///< Calculate our H cost from end position to our current position 
                    m_OpenList.push_back(l_NewNode);

                    /// If our new node is already at the final destination then exit the loop
                    if (l_NewNode->GetPosition().X == p_EndX && l_NewNode->GetPosition().Y == p_EndY)
                    {
                        m_Current = l_NewNode;
                        m_OpenList.clear();
                        break;
                    }
                }
            }
        }

        while (m_Current != nullptr)
        {
            Position l_Position;
            l_Position.X = m_Current->GetPosition().X;
            l_Position.Y = m_Current->GetPosition().Y;
            l_Position.Z = static_cast<int16>(m_HeightGrid[l_Position.X][l_Position.Y]);
            m_Path.push_back(l_Position);
            m_Current = m_Current->GetParentNode();
        }

        CleanUp();

        return true;
    }

    /// GetPath
    /// Returns path we've found
    std::deque<Position> PathFinder::GetPath()
    {
        return m_Path;
    }

    /// CheckValidTile
    /// Check if there's any collision on this tile
    /// @p_Position : Struct which holds x, y coordinates
    bool PathFinder::CheckValidTile(Position const& p_FuturePosition, Position const& p_CurrentPosition)
    {
        int16 l_FutureTileGrid = m_TileGrid[p_FuturePosition.X][p_FuturePosition.Y];
        int16 l_FutureHeightGrid = m_HeightGrid[p_FuturePosition.X][p_FuturePosition.Y];

        int16 l_CurrentTileGrid = m_TileGrid[p_CurrentPosition.X][p_CurrentPosition.Y];
        int16 l_CurrentHeightGrid = m_HeightGrid[p_CurrentPosition.X][p_CurrentPosition.Y];

        if (l_FutureTileGrid == TileState::TILE_STATE_CLOSED)
            return false;

        /// Check height, height is incremented. We cannot walk from a height 1 to height 5, must be 1, 2, 3, 4 ,5
        if (l_FutureHeightGrid > l_CurrentHeightGrid && l_CurrentHeightGrid + 1 != l_FutureHeightGrid)
            return false;
        else if (l_CurrentHeightGrid > l_FutureHeightGrid && l_FutureHeightGrid + 1 != l_CurrentHeightGrid) ///< and vice versa
            return false;

        return true;
    }

    /// CheckDestination
    /// Check if destination coordinates is valid to make a path
    /// @p_Position : Struct which holds x, y coordinates
    bool PathFinder::CheckDestination(Position const& p_Position)
    {
        int16 l_TileGrid = m_TileGrid[p_Position.X][p_Position.Y];

        if (l_TileGrid == TileState::TILE_STATE_CLOSED)
            return false;

        return true;
    }

    /// DoesNodeExist
    /// Check if node position exists in storage
    /// @p_Nodes : Vector which contains Nodes
    /// @p_FuturePosition : new position we are about to take
    Node* PathFinder::DoesNodeExist(std::vector<Node*> const& p_Nodes, Position const& p_FuturePosition)
    {
        for (auto const& l_Itr : p_Nodes)
        {
            if (l_Itr->GetPosition().X == p_FuturePosition.X && l_Itr->GetPosition().Y == p_FuturePosition.Y)
                return l_Itr;
        }
        return nullptr;
    }

    /// CalculateHeuristic
    /// Using Manhattan distance
    /// @p_Current : Current node
    /// @p_EndX : End Position X
    /// @p_EndY : End Position Y
    uint32 PathFinder::CalculateHeuristic(Node* p_Current, int16 const& p_EndX, int16 const& p_EndY)
    {
        return (static_cast<unsigned int>(10 * (abs(p_Current->GetPosition().X - p_EndX) + abs(p_Current->GetPosition().Y - p_EndY))));
    }

    /// CleanUp
    /// Clean up allocated memory in m_OpenList and m_ClosedList
    void PathFinder::CleanUp()
    {
        for (auto& l_Itr : m_ClosedList)
            delete l_Itr;

        for (auto& l_Itr : m_OpenList)
            delete l_Itr;
    }

} ///< NAMESPACE STEERSTONE
