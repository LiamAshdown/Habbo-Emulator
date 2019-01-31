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
#include "../Entity/Player/Player.h"
#include "../Room/Room.h"
#include "PathFinding.h"
//-----------------------------------------------//
PathFinder::PathFinder(Player * player, std::string map) : mPlayer(player)
{
    std::vector<std::string> splitData;
    boost::split(splitData, map, boost::is_any_of("\t "));
    uint8 counter = 0;
    for (auto itrStr : splitData)
    {
        uint8 counter1 = 0;
        for (char& charItr : itrStr)
        {
            Map[counter][counter1] = charItr;
            counter1++;
        }
        counter++;
    }

   /* for (uint8 i = 0; i < 34; i++)
    {
        for (uint8 j = 0; j < 34; j++)
            std::cout << Map[i][j] << " ";

        std::cout << std::endl;
    }*/

    m_XYPos = 
    {
        { 0, 1 }, { 1, 0 }, { 0, -1 }, { -1, 0 },
        { -1, -1 }, { 1, 1 }, { -1, 1 }, { 1, -1 }
    };

    m_PlayerPosition.x = player->GetPlayerPositionX();
    m_PlayerPosition.y = player->GetPlayerPositionY();
}
//-----------------------------------------------//
void PathFinder::FindPath(uint8 endPositionX, uint8 endPositionY)
{
    // Check ifs selected tile is valid, prevents from doing extra work
   if (!CheckValidPosition(endPositionX, endPositionY))
        return;

    Node* current = nullptr;
    std::set<Node*> openList;
    std::set<Node*> closedList;
    openList.insert(new Node(m_PlayerPosition.x, m_PlayerPosition.y));

    while (!openList.empty())
    {
        current = *openList.begin();
        for (auto node : openList)
        {
            if (node->GetScore() <= current->GetScore()) 
                current = node;
        }

        if (current->m_x == endPositionX && current->m_y == endPositionY)
            break;

        closedList.insert(current);
        openList.erase(std::find(openList.begin(), openList.end(), current));

        for (uint8 i = 0; i < 8; ++i) 
        {
            std::shared_ptr<XYPositionStruct> pos = std::make_unique<XYPositionStruct>();
            uint8 newPosX = pos->x = current->m_x + m_XYPos[i].x;
            uint8 newPosY = pos->y = current->m_y + m_XYPos[i].y;

            if (!CheckValidStep(newPosX, newPosY, current, endPositionX,
                endPositionY) || DoesNodeExist(closedList, *pos))
                continue;

            uint32 gCost = current->m_g + ((i < 4) ? 10 : 14);
            Node* newNode = DoesNodeExist(openList, *pos);
            if (newNode == nullptr)
            {
                newNode = new Node(pos->x, pos->y, current);
                newNode->m_g = gCost;
                newNode->m_h = CalculateHeuristic(current, endPositionX, endPositionY);
                openList.insert(newNode);
            }
            else if (gCost <= current->m_g)
            {
                newNode->m_parent = current;
                newNode->m_g = gCost;
            }
        }
    }

    while (current != nullptr) 
    {
        std::shared_ptr<XYPositionStruct> pos = std::make_shared<XYPositionStruct>();
        pos->x = current->m_x;
        pos->y = current->m_y;
        mPath.push_back(*pos);
        current = current->m_parent;
    }

    std::reverse(mPath.begin(), mPath.end());

    DeleteNodes(openList);
    DeleteNodes(closedList);
}
//-----------------------------------------------//
std::vector<XYPositionStruct> PathFinder::GetPath()
{
    return mPath;
}
//-----------------------------------------------//
Player * PathFinder::GetPlayer()
{
    return mPlayer;
}
//-----------------------------------------------//
uint32 PathFinder::CalculateHeuristic(Node* current, uint8& endPositionX, uint8& endPositionY)
{
    // Manhattan Distance Formula
    //< http://artis.imag.fr/~Xavier.Decoret/resources/maths/manhattan/html/
    return (static_cast<unsigned int>(10 *(abs(current->m_x - endPositionX) + abs(current->m_y - endPositionY))));
}
//-----------------------------------------------//
bool PathFinder::CheckValidStep(uint8& x, uint8& y, Node* current, uint8& endX, uint8& endY)
{
    if (y < 0 || x < 0)
        return false;

    if (Map[current->m_y][current->m_x] != 'X')
    {
        // If our previous posit2 is 0 and current position is 2 - we cannot formulate a mPath, player height is incremented by 1
        if ((Map[y][x] > Map[current->m_y][current->m_x] + 1) || (Map[y][x] + 1 < Map[current->m_y][current->m_x]))
            return false;
    }

    // Collision are highlighted as X on the map
    if (Map[y][x] == 'X')
        return false;

    // We need to loop through current players, so we don't walk through them
    // This is dynamic checking, we check each tile whether there is a player occupying the tile
    for (const auto& itr : mPlayer->GetRoom()->GetPlayerStorage())
    {
        if (itr->GetPlayerPositionX() == x && itr->GetPlayerPositionY() == y)
            return false;
    }

    return true;
}
//-----------------------------------------------//
bool PathFinder::CheckValidPosition(uint8& x, uint8& y)
{
    // If there's a player already occupying the grid, then we adjust our grid
    // to move next to the player
    for (const auto& itr : mPlayer->GetRoom()->GetPlayerStorage())
    {
        if (itr->GetPlayerPositionX() == x && itr->GetPlayerPositionY() == y)
        {
            uint8 newX = x;
            uint8 newY = y;

            if (Map[newY][++newX] != 'X')
            {
                x = newX;
                return true;
            }
            else if (Map[++newY][x] != 'X')
            {
                y = newY;
                return true;
            }
            else if (Map[newY][newX] != 'X')
            {
                x = newX;
                y = newY;
                return true;
            }
            else
                return false;
        }
    }
    return true;
}
//-----------------------------------------------//
Node* PathFinder::DoesNodeExist(std::set<Node*> nodes, const XYPositionStruct& pos)
{
    for (auto& itr : nodes)
    {
        if (itr->m_x == pos.x && itr->m_y == pos.y)
            return itr;
    }
    return nullptr;
}
//-----------------------------------------------//
void PathFinder::DeleteNodes(std::set<Node*> nodes)
{
    for (auto& itr : nodes)
    {
        delete itr;
        nodes.erase(itr);
    }
}
//-----------------------------------------------//