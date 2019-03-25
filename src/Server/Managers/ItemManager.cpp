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

#include "ItemManager.h"
#include "Network/StringBuffer.h"
#include "Database/QueryDatabase.h"

namespace SteerStone
{
    /// Singleton
    ItemManager* ItemManager::instance()
    {
        static ItemManager instance;
        return &instance;
    }
    
    /// Constructor
    ItemManager::ItemManager()
    {
    }

    /// Deconstructor
    ItemManager::~ItemManager()
    {
    }
    
    /// LoadPublicRoomItems - Load Public items from l_Database
    void ItemManager::LoadPublicRoomItems()
    {
        QueryDatabase l_Database("rooms");
        l_Database.PrepareQuery("SELECT id, room_model, sprite, x, y, z, rotation, top_height, length, width, behaviour FROM public_items");
        l_Database.ExecuteQuery();

        if (!l_Database.GetResult())
        {
            LOG_ERROR << "public_items is empty!";
            return;
        }

        Result* l_Result = l_Database.Fetch();

        std::vector<PublicItem> l_PublicItemVec;

        do
        {
            PublicItem l_PublicItem;
            l_PublicItem.m_Id = l_Result->GetUint32(1);
            l_PublicItem.mRoomModel = l_Result->GetString(2);
            l_PublicItem.mSprite = l_Result->GetString(3);
            l_PublicItem.mX = l_Result->GetInt32(4);
            l_PublicItem.mY = l_Result->GetInt32(5);
            l_PublicItem.mZ = l_Result->GetDouble(6);
            l_PublicItem.mRotation = l_Result->GetInt32(7);
            l_PublicItem.mTopHeight = l_Result->GetDouble(8);
            l_PublicItem.mLength = l_Result->GetInt32(9);
            l_PublicItem.mWidth = l_Result->GetInt32(10);
            l_PublicItem.mBehaviour = l_Result->GetString(11);

            l_PublicItemVec.push_back(l_PublicItem);

        } while (l_Result->GetNextResult());

        std::vector<PublicItem> picnic;
        std::vector<PublicItem> newbie_lobby;
        std::vector<PublicItem> theater;
        std::vector<PublicItem> libary;
        std::vector<PublicItem> floatinggarden;
        std::vector<PublicItem> sunset_cafe;
        std::vector<PublicItem> pool_a;
        std::vector<PublicItem> pub_a;
        std::vector<PublicItem> md_a;
        std::vector<PublicItem> park_a;
        std::vector<PublicItem> park_b;
        std::vector<PublicItem> pool_b;
        std::vector<PublicItem> ballroom;
        std::vector<PublicItem> cafe_gold0;
        std::vector<PublicItem> gate_park;
        std::vector<PublicItem> gate_park_2;
        std::vector<PublicItem> sun_terrace;
        std::vector<PublicItem> space_cafe;
        std::vector<PublicItem> beauty_salon0;
        std::vector<PublicItem> chill;
        std::vector<PublicItem> dusty_lounge;
        std::vector<PublicItem> cr_staff;
        std::vector<PublicItem> rooftop;
        std::vector<PublicItem> rooftop_2;
        std::vector<PublicItem> tearoom;
        std::vector<PublicItem> cafe_ole;
        std::vector<PublicItem> cr_cafe;
        std::vector<PublicItem> lobby_a;
        std::vector<PublicItem> floorlobby_c;
        std::vector<PublicItem> floorlobby_b;
        std::vector<PublicItem> cinema_a;
        std::vector<PublicItem> old_skool0;
        std::vector<PublicItem> malja_bar_a;
        std::vector<PublicItem> malja_bar_b;
        std::vector<PublicItem> bar_a;
        std::vector<PublicItem> bar_b;
        std::vector<PublicItem> habburger;
        std::vector<PublicItem> pizza;
        std::vector<PublicItem> bb_lobby_1;
        std::vector<PublicItem> snowwar_lobby_1;
        std::vector<PublicItem> tv_studio;
        std::vector<PublicItem> club_mammoth;
        std::vector<PublicItem> ice_cafe;
        std::vector<PublicItem> netcafe;
        std::vector<PublicItem> hallway0;
        std::vector<PublicItem> hallway9;
        std::vector<PublicItem> hallway2;
        std::vector<PublicItem> hallway1;
        std::vector<PublicItem> hallway3;
        std::vector<PublicItem> hallway4;
        std::vector<PublicItem> hallway5;
        std::vector<PublicItem> hallway8;
        std::vector<PublicItem> hallway7;
        std::vector<PublicItem> hallway6;
        std::vector<PublicItem> hallway10;
        std::vector<PublicItem> hallway11;
        std::vector<PublicItem> star_lounge;
        std::vector<PublicItem> orient;
        std::vector<PublicItem> entryhall;
        std::vector<PublicItem> hallA;
        std::vector<PublicItem> hallB;
        std::vector<PublicItem> hallD;
        std::vector<PublicItem> emperors;
        std::vector<PublicItem> beauty_salon1;

        for (std::vector<PublicItem>::iterator itr = l_PublicItemVec.begin(); itr != l_PublicItemVec.end(); itr++)
        {
            if (itr->GetRoomModel() == "picnic")
                picnic.push_back(*itr);
            else if (itr->GetRoomModel() == "newbie_lobby")
                newbie_lobby.push_back(*itr);
            else if (itr->GetRoomModel() == "theater")
                theater.push_back(*itr);
            else if (itr->GetRoomModel() == "libary")
                libary.push_back(*itr);
            else if (itr->GetRoomModel() == "floatinggarden")
                floatinggarden.push_back(*itr);
            else if (itr->GetRoomModel() == "sunset_cafe")
                sunset_cafe.push_back(*itr);
            else if (itr->GetRoomModel() == "pool_a")
                pool_a.push_back(*itr);
            else if (itr->GetRoomModel() == "pub_a")
                pub_a.push_back(*itr);
            else if (itr->GetRoomModel() == "md_a")
                md_a.push_back(*itr);
            else if (itr->GetRoomModel() == "picnic")
                picnic.push_back(*itr);
            else if (itr->GetRoomModel() == "park_a")
                park_a.push_back(*itr);
            else if (itr->GetRoomModel() == "park_b")
                park_b.push_back(*itr);
            else if (itr->GetRoomModel() == "ballroom")
                ballroom.push_back(*itr);
            else if (itr->GetRoomModel() == "cafe_gold0")
                cafe_gold0.push_back(*itr);
            else if (itr->GetRoomModel() == "gate_park")
                gate_park.push_back(*itr);
            else if (itr->GetRoomModel() == "gate_park_2")
                gate_park_2.push_back(*itr);
            else if (itr->GetRoomModel() == "sun_terrace")
                sun_terrace.push_back(*itr);
            else if (itr->GetRoomModel() == "beauty_salon0")
                beauty_salon0.push_back(*itr);
            else if (itr->GetRoomModel() == "chill")
                chill.push_back(*itr);
            else if (itr->GetRoomModel() == "dusty_lounge")
                dusty_lounge.push_back(*itr);
            else if (itr->GetRoomModel() == "cr_staff")
                cr_staff.push_back(*itr);
            else if (itr->GetRoomModel() == "rooftop")
                rooftop.push_back(*itr);
            else if (itr->GetRoomModel() == "rooftop_2")
                rooftop_2.push_back(*itr);
            else if (itr->GetRoomModel() == "tearoom")
                tearoom.push_back(*itr);
            else if (itr->GetRoomModel() == "cafe_ole")
                cafe_ole.push_back(*itr);
            else if (itr->GetRoomModel() == "cr_cafe")
                cr_cafe.push_back(*itr);
            else if (itr->GetRoomModel() == "lobby_a")
                lobby_a.push_back(*itr);
            else if (itr->GetRoomModel() == "floorlobby_c")
                floorlobby_c.push_back(*itr);
            else if (itr->GetRoomModel() == "floorlobby_b")
                floorlobby_b.push_back(*itr);
            else if (itr->GetRoomModel() == "cinema_a")
                cinema_a.push_back(*itr);
            else if (itr->GetRoomModel() == "old_skool0")
                old_skool0.push_back(*itr);
            else if (itr->GetRoomModel() == "malja_bar_a")
                malja_bar_a.push_back(*itr);
            else if (itr->GetRoomModel() == "malja_bar_b")
                malja_bar_b.push_back(*itr);
            else if (itr->GetRoomModel() == "bar_a")
                bar_a.push_back(*itr);
            else if (itr->GetRoomModel() == "bar_b")
                bar_b.push_back(*itr);
            else if (itr->GetRoomModel() == "habburger")
                habburger.push_back(*itr);
            else if (itr->GetRoomModel() == "pizza")
                pizza.push_back(*itr);
            else if (itr->GetRoomModel() == "bb_lobby_1")
                bb_lobby_1.push_back(*itr);
            else if (itr->GetRoomModel() == "snowwar_lobby_1")
                snowwar_lobby_1.push_back(*itr);
            else if (itr->GetRoomModel() == "tv_studio")
                tv_studio.push_back(*itr);
            else if (itr->GetRoomModel() == "club_mammoth")
                club_mammoth.push_back(*itr);
            else if (itr->GetRoomModel() == "ice_cafe")
                ice_cafe.push_back(*itr);
            else if (itr->GetRoomModel() == "netcafe")
                netcafe.push_back(*itr);
            else if (itr->GetRoomModel() == "hallway0")
                hallway0.push_back(*itr);
            else if (itr->GetRoomModel() == "hallway9")
                hallway9.push_back(*itr);
            else if (itr->GetRoomModel() == "hallway2")
                hallway2.push_back(*itr);
            else if (itr->GetRoomModel() == "hallway1")
                hallway1.push_back(*itr);
            else if (itr->GetRoomModel() == "hallway3")
                hallway3.push_back(*itr);
            else if (itr->GetRoomModel() == "hallway5")
                hallway5.push_back(*itr);
            else if (itr->GetRoomModel() == "hallway8")
                hallway8.push_back(*itr);
            else if (itr->GetRoomModel() == "hallway7")
                hallway7.push_back(*itr);
            else if (itr->GetRoomModel() == "hallway6")
                hallway6.push_back(*itr);
            else if (itr->GetRoomModel() == "hallway10")
                hallway10.push_back(*itr);
            else if (itr->GetRoomModel() == "hallway11")
                hallway11.push_back(*itr);
            else if (itr->GetRoomModel() == "star_lounge")
                star_lounge.push_back(*itr);
            else if (itr->GetRoomModel() == "orient")
                orient.push_back(*itr);
            else if (itr->GetRoomModel() == "entryhall")
                entryhall.push_back(*itr);
            else if (itr->GetRoomModel() == "hallA")
                hallA.push_back(*itr);
            else if (itr->GetRoomModel() == "hallB")
                hallB.push_back(*itr);
            else if (itr->GetRoomModel() == "hallD")
                hallD.push_back(*itr);
            else if (itr->GetRoomModel() == "emperors")
                emperors.push_back(*itr);
            else if (itr->GetRoomModel() == "beauty_salon1")
                beauty_salon1.push_back(*itr);
        }

        m_PublicItems["picnic"] = picnic;
        m_PublicItems["newbie_lobby"] = newbie_lobby;
        m_PublicItems["theater"] = theater;
        m_PublicItems["libary"] = libary;
        m_PublicItems["floatinggarden"] = floatinggarden;
        m_PublicItems["sunset_cafe"] = sunset_cafe;
        m_PublicItems["pool_a"] = pool_a;
        m_PublicItems["pub_a"] = pub_a;
        m_PublicItems["md_a"] = md_a;
        m_PublicItems["park_a"] = park_a;
        m_PublicItems["park_b"] = park_b;
        m_PublicItems["pool_b"] = pool_b;
        m_PublicItems["ballroom"] = ballroom;
        m_PublicItems["cafe_gold0"] = cafe_gold0;
        m_PublicItems["gate_park"] = gate_park;
        m_PublicItems["gate_park_2"] = gate_park_2;
        m_PublicItems["sun_terrace"] = sun_terrace;
        m_PublicItems["space_cafe"] = space_cafe;
        m_PublicItems["beauty_salon0"] = beauty_salon0;
        m_PublicItems["chill"] = chill;
        m_PublicItems["dusty_lounge"] = dusty_lounge;
        m_PublicItems["cr_staff"] = cr_staff;
        m_PublicItems["rooftop"] = rooftop;
        m_PublicItems["rooftop_2"] = rooftop_2;
        m_PublicItems["tearoom"] = tearoom;
        m_PublicItems["cafe_ole"] = cafe_ole;
        m_PublicItems["cr_cafe"] = cr_cafe;
        m_PublicItems["lobby_a"] = lobby_a;
        m_PublicItems["floorlobby_c"] = floorlobby_c;
        m_PublicItems["floorlobby_b"] = floorlobby_b;
        m_PublicItems["cinema_a"] = cinema_a;
        m_PublicItems["old_skool0"] = old_skool0;
        m_PublicItems["malja_bar_a"] = malja_bar_a;
        m_PublicItems["malja_bar_b"] = malja_bar_b;
        m_PublicItems["bar_a"] = bar_a;
        m_PublicItems["bar_b"] = bar_b;
        m_PublicItems["habburger"] = habburger;
        m_PublicItems["pizza"] = pizza;
        m_PublicItems["bb_lobby_1"] = bb_lobby_1;
        m_PublicItems["snowwar_lobby_1"] = snowwar_lobby_1;
        m_PublicItems["tv_studio"] = tv_studio;
        m_PublicItems["club_mammoth"] = club_mammoth;
        m_PublicItems["ice_cafe"] = ice_cafe;
        m_PublicItems["netcafe"] = netcafe;
        m_PublicItems["hallway0"] = hallway0;
        m_PublicItems["hallway9"] = hallway9;
        m_PublicItems["hallway2"] = hallway2;
        m_PublicItems["hallway1"] = hallway1;
        m_PublicItems["hallway3"] = hallway3;
        m_PublicItems["hallway4"] = hallway4;
        m_PublicItems["hallway5"] = hallway5;
        m_PublicItems["hallway8"] = hallway8;
        m_PublicItems["hallway7"] = hallway7;
        m_PublicItems["hallway6"] = hallway6;
        m_PublicItems["hallway10"] = hallway10;
        m_PublicItems["hallway11"] = hallway11;
        m_PublicItems["star_lounge"] = star_lounge;
        m_PublicItems["orient"] = orient;
        m_PublicItems["entryhall"] = entryhall;
        m_PublicItems["hallA"] = hallA;
        m_PublicItems["hallB"] = hallB;
        m_PublicItems["hallD"] = hallD;
        m_PublicItems["emperors"] = emperors;
        m_PublicItems["beauty_salon1"] = beauty_salon1;

        LOG_INFO << "Loaded " << l_PublicItemVec.size() << " Room Public Items";
    }
    
    /// GetPublicRoomItems#
    /// @p_Model : Room Model to retrieve furniture data
    PublicItemVec ItemManager::GetPublicRoomItems(const std::string& model)
    {
        PublicItemMap::iterator itr = m_PublicItems.find(model);
        if (itr != m_PublicItems.end())
            return itr->second;
        else
            return PublicItemVec{};
    }
} ///< NAMESPACE STEERSTONE
