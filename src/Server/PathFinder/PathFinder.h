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

#ifndef _PATHFINDER_PATHFINDER_h
#define _PATHFINDER_PATHFINDER_h
#include "Common/SharedDefines.h"
#endif /* _PATHFINDER_PATHFINDER_h */

namespace SteerStone
{
    /// This stores the 8 directions we can go
    ///<          \|/
    ///<        -- | --
    ///<          /|\
    ///<
    struct Position
    {
        int16 X;
        int16 Y;
    };

    /// Holds information about node, used to calculate which node is closest to end position
    struct Node
    {
    public:
        /// Constructor
        /// @p_X : Start Position of node
        /// @p_Y : Start Position of node
        /// @p_Parent : Parent node
        Node(int16 const& p_X, int16 const& p_Y, Node* p_Parent = nullptr) : m_F(0), m_G(0), m_H(0), m_X(p_X), m_Y(p_Y), m_Parent(p_Parent) {}
        
    public:
        /// Node Info
        int16 GetPositionX()    const { return m_X;       }         ///< Current node Position X
        int16 GetPositionY()    const { return m_Y;       }         ///< Current node Position Y
        int16 GetTotalCost()    const { return m_H + m_G; }         ///< Add H and G to get total cost
        uint32 GetHCost()       const { return m_H;       }         ///< Calculate end position to current position
        uint32 GetGCost()       const { return m_G;       }         ///< Calculate start position to current position
        Node* GetParentNode()         { return m_Parent;  }         ///< Returns our parent node

        void SetHCost(uint32 const& p_Cost)    { m_H = p_Cost;      }
        void SetGCost(uint32 const& p_Cost)    { m_G = p_Cost;      }
        void SetParentNode(Node* p_Node)       { m_Parent = p_Node; }

    private:
        /// Variables
        uint32 m_H;
        uint32 m_G;
        uint32 m_F;
        int16 m_X;
        int16 m_Y;

        Node* m_Parent;
    };

    /// Class which calculates user path to a specific grid in the room
    /// using A* Algorithm
    class PathFinder
    {
    public:
        /// Constructor
        /// @p_Grid : Multi-dimensional array which stores the heightmap
        PathFinder(GridArray const& p_Grid);

        /// Deconstructor
        ~PathFinder();

    public:
        /// CalculatePath
        /// @p_StartX : Start Position X
        /// @p_StartY : Start Position Y
        /// @p_EndX : End Position X
        /// @p_EndY : End Position Y
        void CalculatePath(int16 const& p_StartX, int16 const& p_StartY, int16 const& p_EndX, int16 const& p_EndY);

    private:
        /// CheckValidTile
        /// Check if there's any collision on this tile
        /// @p_Position : Struct which holds x, y coordinates
        bool CheckValidTile(Position const& p_Position);

        /// DoesNodeExist
        /// Check if node position exists in storage
        /// @p_Nodes : Vector which contains Nodes
        /// @p_FuturePosition : new position we are about to take
        Node* DoesNodeExist(std::vector<Node*> const& p_Nodes, Position const& p_FuturePosition);

        /// CalculateHeuristic
        /// Using Manhattan distance
        /// @p_Current : Current node
        /// @p_EndX : End Position X
        /// @p_EndY : End Position Y
        uint32 CalculateHeuristic(Node const* p_Current, int16 const& p_EndX, int16 const& p_EndY);

        /// CleanUp
        /// Clean up allocated memory in m_OpenList and m_ClosedList
        void CleanUp();

    private:
        GridArray m_Grid;                          ///< Holds height map in multi dimensional array
        std::vector<Position> m_Directions;        ///< Directions in which we can go

        Node* m_Current;                           ///< Current node we are accessing from m_OpenList or m_ClosedList
        std::vector<Node*> m_OpenList;             ///< Holds nodes which needs to evaluted
        std::vector<Node*> m_ClosedList;           ///< Holds nodes which are already evaluted
    };
} ///< NAMESPACE STEERSTONE