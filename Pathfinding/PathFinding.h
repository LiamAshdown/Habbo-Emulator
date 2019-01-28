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

#ifndef _QuadEmu_PathFinding_h_
#define _QuadEmu_PathFinding_h_
#include "../Common/SharedDefines.h"
#endif /* _QuadEmu_QuadEngine_ */

class Player;

typedef struct XYPosition
{
    int x;
    int y;

}XYPositionStruct;

class Node
{
public:
    Node(const uint8& x, const uint8& y, Node* node = nullptr) : m_x(x), m_y(y), m_parent(node) {}
    ~Node() {}
    uint32 m_f; // How many squares it takes to get to destination without accounting any collision
    uint32 m_g; // How many squares it takes to get to destination accounting collision
    uint32 m_h; // H(How many squares it takes to get to destination without accounting any collision) +
           // G(How many squares it takes to get to destination accounting collision)

    uint8 m_x; // Position of where we are in the multidimensional char array
    uint8 m_y; // Position of where we ar ein the multidimensional char array

    Node* m_parent;

    uint8 GetScore()
    {
        return m_f + m_g;
    }
};

class PathFinder
{
public:
    PathFinder(Player* player, std::string map);
    ~PathFinder() {}
    
public:
    void FindPath(uint8 endPositionX, uint8 endPositionY);
    char Map[34][34];

    std::vector <XYPositionStruct> GetPath() { return path; }
    std::vector <XYPositionStruct> path;
protected:
    uint32 CalculateHeuristic(Node* current, uint8& endPositionX, uint8& endPositionY);
    bool CheckValidStep(uint8& x, uint8& y, Node* current, uint8& endX, uint8& endY);
    bool CheckValidPosition(const uint8& x, const uint8& y);
    Node* DoesNodeExist(std::set<Node*> nodes, const XYPositionStruct& pos);
    void DeleteNodes(std::set<Node*> nodes);

protected:
    std::vector<XYPositionStruct> m_XYPos;
    XYPositionStruct m_PlayerPosition;
    Player* mPlayer;
    uint8 m_Directions;
};