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
#include "RoomWalkWay.h"
#include "RoomHabboInfo.h"
#include "Platform/Thread/FunctionCallBack.h"
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

    enum RoomAccessType
    {
        ROOM_ACCESS_TYPE_OPEN               = 0,
        ROOM_ACCESS_TYPE_CLOSED             = 1,
        ROOM_ACCESS_TYPE_PASSWORD           = 2
    };


    class Habbo;
    class StringBuffer;

    typedef std::unordered_map<int32, std::unique_ptr<RoomHabboInfo>> GUIDUserMap;

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
        void LoadGridData();

        /// EnterRoom 
        /// @p_Habbo : Habbo user entering in room
        /// @p_WalkWay : Walkway tile position 
        bool EnterRoom(Habbo* p_Habbo, WalkWay* p_WalkWay = nullptr);
       
        /// EnterRoomCallBack 
        /// Gets processed on Room::Update
        /// @p_Habbo : Habbo user entering in room
        void EnterRoomCallBack(Habbo* p_Habbo);

        /// LeaveRoom 
        /// @p_Habbo : Habbo user leaving room
        /// @p_HotelView : Kick user to hotel view
        void LeaveRoom(Habbo* p_Habbo, bool p_HotelView = false);

        /// LeaveRoomCallBack
        /// Gets processed on Room::Update
        /// @p_Itr : Iteration of habbo which going to be removed
        void LeaveRoomCallBack(GUIDUserMap::iterator& p_Itr);

        /// AddFigure
        /// Send Habbo Figure to clients in room
        /// @p_Habbo : Habbo which is joining room
        void AddFigure(Habbo* p_Habbo);

        /// SendRoomStatuses
        /// Send Habbo Statuses to user joining room
        /// @p_Habbo : User joining room
        void SendRoomStatuses(Habbo* p_Habbo);

        /// SendUserLeftRoom
        void SendUserLeftRoom(uint32 const p_GUID);

        /// SendWorldObjects 
        /// @p_Habbo : Send Furniture Objects to Habbo client
        void SendWorldObjects(Habbo* p_Habbo);

        /// SendPacketToAll
        /// Send Packet to all users in room
        /// @p_Buffer : Data being sent to users in room
        void SendPacketToAll(StringBuffer const* p_Buffer);

        /// SendObjects 
        /// @p_Habbo : Send Active Furniture Objects to Habbo user
        void SendActiveObjects(Habbo* p_Habbo);

        /// Walk 
        /// @p_Habbo : Habbo user who is walking
        /// @p_EndX : End Position habbo is going to
        /// @p_EndY : End Position habbo is going to
        bool Walk(uint32 const p_RoomGUID, uint16 const p_EndX, uint16 const p_EndY);

        /// AddStatus
        /// @p_RoomGUID : Room GUID of user
        /// @p_Status : Habbo Status to be added
        void AddStatus(uint32 const p_RoomGUID, uint32 const p_Status);

        /// RemoveStatus
        /// @p_RoomGUID : Room GUID of user
        /// @p_Status : Habbo Status to be removed
        void RemoveStatus(uint32 const p_RoomGUID, uint32 const p_Status);

        /// HasStatus
        /// Check if user has an active status
        /// @p_RoomGUID : Room GUID of user
        /// @p_Status : Status to check
        bool HasStatus(uint32 const p_RoomGUID, uint32 const p_Status) const;

        /// ProcessUserActions
        /// Process Habbo Actions; Status, pathfinding, etc..
        void ProcessUserActions(const uint32 p_Diff);

        /// GetCcts
        /// Get Ccts
        /// @p_Specified : Return specified Ccts
        std::string GetCcts(std::string const p_Specified = std::string());

        /// SendNodeSpaceUsers
        /// @p_Habbo : Packet to send to
        void SendNodeSpaceUsers(Habbo* p_Habbo);

        /// FindHabboByName
        /// @p_Name : Name of user to find
        Habbo* FindHabboByName(std::string const p_Name);

        /// FindHabboByGuid
        /// @p_Id : Room GUID we are searching for
        Habbo* FindHabboByGuid(uint32 const p_GUID);

        /// FindHabboById
        /// @p_Id : Id of user we are searching for
        Habbo* FindHabboById(uint32 const p_Id);

        ///////////////////////////////////////////
        //             SUPER RIGHTS
        ///////////////////////////////////////////

        /// RefreshRights
        /// Send Rights to user whether they are owner, super user or normal user
        /// @p_Habbo : Rights we are sending to
        void RefreshRights(Habbo* p_Habbo);

        /// GetRoomUsers
        /// Returns storage of users with room rights
        std::set<uint32>* GetRoomRightsUsers();

        /// AddFuserRights
        /// @p_Habbo : Habbo user we are adding user rights to
        void AddUserRights(Habbo* p_Habbo);

        /// RemoveUserRights
        /// @p_Id : User we are removing user rights from
        void RemoveUserRights(uint32 const p_Id);

        /// IsSuperUser
        /// @p_Id : Id we are checking if user has super rights
        bool IsSuperUser(uint32 const p_Id);

        /// RemoveAllUserRights
        /// Remove all users who have super rights
        void RemoveAllUserRights();

        /// Update 
        /// Update all objects in room
        /// @p_Diff : Hotel last tick time
        void Update(uint32 const p_Diff);

    public:
        /// Room Info
        uint32 GetId()               const { return m_Id;           }
        uint32 GetOwnerId()          const { return m_OwnerId;      }
        std::string GetOwnerName()   const { return m_OwnerName;    } 
        uint32 GetCategoryId()       const { return m_CategoryId;   }
        std::string GetName()        const { return m_Name;         }
        std::string GetDescription() const { return m_Description;  }
        std::string GetModel()       const { return m_Model;        }
        uint32 GetWallPaper()        const { return m_WallPaper;    }
        uint32 GetFloor()            const { return m_Floor;        }
        bool ShowName()              const { return m_ShowName;     }
        bool AllowSuperRights()      const { return m_AllowSuperRights;   }
        uint16 GetAccessType()       const { return m_AccessType;   }
        std::string GetPassword()    const { return m_Password;     }
        uint32 GetVisitorsNow()      const { return m_VisitorsNow;  }
        uint32 GetVisitorsMax()      const { return m_VisitorsMax;  }
        bool IsRoomVisible()         const { return m_RoomVisible;  }
        RoomModel& GetRoomModel()          { return m_RoomModel;    }
        RoomCategory* GetRoomCategory()    { return m_RoomCategory; }
      
    private:
        /// Room Info
        uint32 m_Id;
        uint32 m_OwnerId;
        std::string m_OwnerName;
        uint32 m_CategoryId;
        std::string m_Name;
        std::string m_Description;
        std::string m_Model;
        std::vector<std::string> m_Ccts;
        uint32 m_WallPaper;
        uint32 m_Floor;
        bool m_ShowName;
        bool m_AllowSuperRights;
        uint16 m_AccessType;
        std::string m_Password;
        uint32 m_VisitorsNow;
        uint32 m_VisitorsMax;
        bool m_RoomVisible;
        RoomModel m_RoomModel;
        RoomCategory* m_RoomCategory;
        GUIDUserMap m_Habbos;                    ///< Hold Habbo users
        std::set<uint32> m_SuperRights;          ///< Holds users who have room rights
        FunctionCallBack m_FunctionCallBack;     ///< Execute functions on Room::Update

        boost::shared_mutex m_Mutex;
    };
} ///< NAMESPACE STEERSTONE