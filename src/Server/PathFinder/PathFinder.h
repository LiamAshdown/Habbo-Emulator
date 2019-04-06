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
#include "RoomModel.h"
#endif /* _PATHFINDER_PATHFINDER_h */

namespace SteerStone
{
    /// Determines if tile state is open to walk on or closed
    enum TileState
    {
        TILE_STATE_OPEN         = 0,            ///< User can walk on tile
        TILE_STATE_CLOSED       = 1,            ///< User cannot walk on tile
        TILE_STATE_SIT          = 2,            ///< User can sit on tile
        TILE_STATE_OCCUPIED     = 3             ///< Tile is being used by another object            
    };

    /// This stores the 8 directions we can go
    ///<          \|/
    ///<        -- | --
    ///<          /|\
    ///<
    struct Position
    {
        int16 X;
        int16 Y;
        int16 Z;
    };

    /// Holds information about node, used to calculate which node is closest to end position
    struct Node
    {
    public:
        /// Constructor
        /// @p_X : Start Position of node X
        /// @p_Y : Start Position of node Y
        /// @p_Parent : Parent node
        Node(int16 const p_X, int16 const p_Y, int16 p_Z = 0, Node* p_Parent = nullptr) : m_Position{ p_X, p_Y, p_Z }, m_F(0), m_G(0), m_H(0), m_Parent(p_Parent){}
        
    public:
        /// Node Info
        int16 GetTotalCost()             const { return m_H + m_G; }
        uint32 GetHCost()                const { return m_H;       }
        uint32 GetGCost()                const { return m_G;       }
        Node* GetParentNode()                  { return m_Parent;  }
        Position GetPosition()                 { return m_Position;}

        void SetHCost(uint32 const& p_Cost)    { m_H = p_Cost;      }
        void SetGCost(uint32 const& p_Cost)    { m_G = p_Cost;      }
        void SetParentNode(Node* p_Node)       { m_Parent = p_Node; }

    private:
        /// Variables
        uint32 m_H;            ///< Estimated movement cost from current position to end position                     
        uint32 m_G;            ///< Movement cost from start position to current position
        uint32 m_F;            ///< Total cost of m_H + m_G
        Position m_Position;   ///< Node Position
        Node* m_Parent;        ///< Parent node which is used to gather all path points
    };

    class RoomModel;

    /// Caculates a path using A* Algorithm
    class PathFinder
    {
    public:
        /// Constructor
        /// @p_RoomModel : Room Model of the room we are calculating a path from
        PathFinder(RoomModel* p_RoomModel);

        /// Deconstructor
        ~PathFinder();

    public:
        /// CalculatePath
        /// @p_StartX : Start Position X
        /// @p_StartY : Start Position Y
        /// @p_EndX : End Position X
        /// @p_EndY : End Position Y
        bool CalculatePath(int16 const p_StartX, int16 const p_StartY, int16 const p_EndX, int16 const p_EndY);

        /// ReCalculatePath
        /// Recaculate 1 waypoint if there's a object blocking our original path
        /// @p_Position : Holds current position of X and Y
        /// @p_NextX : Next Position X
        /// @p_NextY : Next Position Y
        bool ReCalculatePath(Position& p_Position, int16 const p_NextX, int16 const p_NextY);

        /// GetPath
        /// Returns path points we've found
        std::deque<Position>& GetPath();

    private:
        /// CheckValidTile
        /// Check if there's any collision on this tile
        /// @p_FuturePosition : Struct which holds future x, y coordinates
        /// @p_CurrentPosition : Struct which holds current x, y coordinates
        bool CheckValidTile(Position const& p_FuturePosition, Position const& p_CurrentPosition);

        /// CheckDestination
        /// Check if destination coordinates is valid to make a path
        /// @p_Position : Struct which holds x, y coordinates
        bool CheckDestination(Position const& p_Position);

        /// DoesNodeExist
        /// Check if node position exists in storage
        /// @p_Nodes : Vector which contains Nodes
        /// @p_FuturePosition : New position we are about to take
        Node* DoesNodeExist(std::vector<Node*> const& p_Nodes, Position const& p_FuturePosition);

        /// CalculateHeuristic
        /// Using Manhattan distance
        /// @p_Current : Current node
        /// @p_EndX : End Position X
        /// @p_EndY : End Position Y
        uint32 CalculateHeuristic(Node* p_Current, int16 const& p_EndX, int16 const& p_EndY);

        /// CheckDistanceBetween
        /// Check the distance between two way points
        /// if the distance is 1 grid apart, return true if not return false
        /// @m_CurrentX : Current X axis
        /// @m_CurrentY : Current X axis
        /// @p_NextX : Next X Axis
        /// @p_NextY : Next Y Axis
        bool CheckDistanceBetween(int16 const m_CurrentX, int16 m_CurrentY, int16 const p_NextX, int16 const p_NextY);

        /// CleanUp
        /// Clean up allocated memory in m_OpenList and m_ClosedList
        void CleanUp();

    private:
        RoomModel* m_RoomModel;                    ///< Holds dynamic objects in multi dimensional array
        Node* m_Current;                           ///< Current node we are accessing from m_OpenList or m_ClosedList
        std::vector<Position> m_Directions;        ///< Directions in which we can go
        std::deque<Position> m_Path;               ///< Holds our path points
        std::vector<Node*> m_OpenList;             ///< Holds nodes which needs to evaluted
        std::vector<Node*> m_ClosedList;           ///< Holds nodes which are already evaluted

        int32 m_MaxGridX;                          ///< Holds Max Grid of X axis
        int32 m_MaxGridY;                          ///< Holds Max Grid of Y axis
    };
} ///< NAMESPACE STEERSTONE