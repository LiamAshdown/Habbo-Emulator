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

#ifndef _Quad_RoomModel_h_
#define _Quad_RoomModel_h_
#include "Common/SharedDefines.h"
#endif /* _Quad_RoomModel_h_ */

namespace SteerStone
{
    class RoomModel
    {
    public:
        friend class RoomManager;

    public:
        RoomModel();
        ~RoomModel();

    public:
        uint32 GetId() const;
        std::string GetModelId() const;
        std::string GetModel() const;
        int32 GetDoorX() const;
        int32 GetDoorY() const;
        float GetDoorZ() const;
        int32 GetDoorOrientation() const;
        std::string GetHeightMap() const;

    private:
        uint32 m_Id;
        std::string mModelId;
        std::string mModel;
        int32 mDoorX;
        int32 mDoorY;
        float mDoorZ;
        int32 mMapSizeX;
        int32 mMapSizeY;
        int32 mDoorOrientation;
        std::string mHeightMap;
    };
}