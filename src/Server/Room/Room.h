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

#ifndef _ROOM_ROOM_h
#define _ROOM_ROOM_h
#include "Common/SharedDefines.h"
#include "RoomCategory.h"
#include "WayPoints.h"
#include <boost/thread/shared_mutex.hpp>
#include <boost/thread/locks.hpp>
#include <memory>
#endif /* _ROOM_ROOM_h */

namespace SteerStone
{
    enum RoomType
    {
        ROOM_TYPE_FLAT                      = 0,
        ROOM_TYPE_PUBLIC                    = 1
    };

    class Habbo;
    class StringBuffer;

    typedef std::unordered_map<uint32, std::unique_ptr<WayPoints>> PathFinderMap;
    typedef std::unordered_map<uint32, Habbo*> GUIDUserMap;

    /// Class which holds information about hotel room and responsible for handling habbos inside room
    class Room
    {
    public:
        friend class RoomManager;

    public:
        /// Constructor
        Room();
        
        /// Deconstructor
        ~Room();

    public:

        /// LoadGridData
        /// Load Static and dynamic Grid Data
        void LoadGridData();

        /// EnterRoom 
        /// @p_Habbo : Habbo user entering in room
        bool EnterRoom(Habbo* p_Habbo);

        /// EnterRoom 
        /// @p_Habbo : Habbo user leaving room
        void LeaveRoom(Habbo* p_Habbo);

        /// SendNewUserObjectToRoom
        /// Send Habbo Figure object to clients in room
        /// @p_Habbo : p_Habbo
        void SendNewUserObjectToRoom(Habbo* p_Habbo);

        /// SendUserObjectLeftRoom
        /// This function is used when habbo leaves room, and we need to update habbo objects again
        void SendUserObjectLeftRoom(uint32 const p_GUID);

        /// SendWorldObjects 
        /// @p_Habbo : Send Furniture Objects to Habbo client
        void SendWorldObjects(Habbo* p_Habbo);

        /// SendPacketToAll
        /// Send Packet to all users in room
        /// @p_Buffer : Data being sent to users in room
        void SendPacketToAll(StringBuffer const* p_Buffer);

        /// SendObjects 
        /// @p_Habbo : Send Active Furniture Objects to Habbo client
        void SendActiveObjects(Habbo* p_Habbo);

        /// Walk 
        /// @p_Habbo : Habbo class which is walking
        /// @p_EndX : End Position habbo is going to
        /// @p_EndY : End Position habbo is going to
        /// @p_CheckDynamicObjects : Check for Dynamic Objects
        void Walk(Habbo* p_Habbo, uint16 const p_EndX, uint16 const p_EndY, bool p_CheckDynamicObjects = false);

        /// UpdateObjectsPaths
        /// Update all current paths
        void UpdateObjectsPaths(const uint32 p_Diff);

        /// Update 
        /// @p_Diff : Update the room
        void Update(uint32 const p_Diff);

    public:
        /// ROOM INFO
        uint32 GetId()               const { return m_Id;           }
        uint32 GetOwnerId()          const { return m_OwnerId;      }
        std::string GetOwnerName()   const { return m_OwnerName;    } 
        uint32 GetCategoryId()       const { return m_CategoryId;   }
        std::string GetName()        const { return m_Name;         }
        std::string GetDescription() const { return m_Description;  }
        std::string GetModel()       const { return m_Model;        }
        std::string GetCcts()        const { return m_Ccts;         }
        uint32 GetWallPaper()        const { return m_WallPaper;    }
        uint32 GetFloor()            const { return m_Floor;        }
        bool ShowName()              const { return m_ShowName;     }
        bool GetSuperUsers()         const { return m_SuperUsers;   }
        std::string GetAccessType()  const { return m_AccessType;   }
        std::string GetPassword()    const { return m_Password;     }
        uint32 GetVisitorsNow()      const { return m_VisitorsNow;  }
        uint32 GetVisitorsMax()      const { return m_VisitorsMax;  }
        RoomModel& GetRoomModel()          { return m_RoomModel;    }
        RoomCategory* GetRoomCategory()    { return m_RoomCategory; }

    private:
        /// GenerateGUID - Generate a unique ID for object in room
        uint32 GenerateGUID();
      
    private:
        /// ROOM INFO
        uint32 m_Id;
        uint32 m_OwnerId;
        std::string m_OwnerName;
        uint32 m_CategoryId;
        std::string m_Name;
        std::string m_Description;
        std::string m_Model;
        std::string m_Ccts;
        uint32 m_WallPaper;
        uint32 m_Floor;
        bool m_ShowName;
        bool m_SuperUsers;
        std::string m_AccessType;
        std::string m_Password;
        uint32 m_VisitorsNow;
        uint32 m_VisitorsMax;
        RoomModel m_RoomModel;
        RoomCategory* m_RoomCategory;

        GUIDUserMap m_Habbos;                    ///< Hold Habbo users in vector
        PathFinderMap m_Paths;                   ///< Stores path points for objects inside room
        boost::shared_mutex m_Mutex;             ///< Mutex
    };
} ///< NAMESPACE STEERSTONE