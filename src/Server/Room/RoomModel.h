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

#ifndef _ROOM_ROOM_MODEL_h
#define _ROOM_ROOM_MODEL_h
#include "Common/SharedDefines.h"
#include "TileInstance.h"
#endif /* _ROOM_ROOM_MODEL_h */

namespace SteerStone
{
    typedef boost::multi_array<TileInstance*, 2> DynamicTileGridArray;

    /// Class which holds Room Model information
    class RoomModel
    {
    public:
        friend class RoomManager;
        friend class Room;

    public:
        /// Constructor
        RoomModel() {}
        
        /// Deconstructor
        ~RoomModel() 
        {
            /// Delete Tile Instances
            for (int32 l_Y = 0; l_Y < m_MapSizeY; l_Y++)
            {
                for (int32 l_X = 0; l_X < m_MapSizeX; l_X++)
                {
                    delete DynamicTileGrid[l_X][l_Y];
                }
            }
        }

    public:
        /// Room Info
        uint32 GetId()              const { return m_Id;              }
        std::string GetModelId()    const { return m_ModelId;         }
        std::string GetModel()      const { return m_Model;           }
        int32 GetDoorX()            const { return m_DoorX;           }
        int32 GetDoorY()            const { return m_DoorY;           }
        float GetDoorZ()            const { return m_DoorZ;           }
        int32 GetDoorOrientation()  const { return m_DoorOrientation; }
        std::string GetHeightMap()  const { return m_HeightMap;       }
        int32 GetMaxGridX()         const { return m_MapSizeX;        }
        int32 GetMaxGridY()         const { return m_MapSizeY;        }

        DynamicTileGridArray DynamicTileGrid;

    private:
        /// Variables
        uint32 m_Id;
        std::string m_ModelId;
        std::string m_Model;
        int32 m_DoorX;
        int32 m_DoorY;
        float m_DoorZ;
        int32 m_DoorOrientation;
        std::string m_HeightMap;
        int32 m_MapSizeX;
        int32 m_MapSizeY;
    };
} ///< NAMESPACE STEERSTONE