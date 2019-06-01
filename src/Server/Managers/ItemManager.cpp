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
#include "Database/DatabaseTypes.h"

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
    
    /// LoadPublicRoomItems
    /// Load Public items from l_Database
    void ItemManager::LoadPublicRoomItems()
    {
        PreparedStatement* l_PreparedStatement = RoomDatabase.GetPrepareStatement();
        l_PreparedStatement->PrepareStatement("SELECT id, room_model, sprite, x, y, z, rotation, top_height, length, width, trigger_state, current_program FROM public_items");
        PreparedResultSet* l_PreparedResultSet = l_PreparedStatement->ExecuteStatement();

        if (!l_PreparedResultSet)
        {
            LOG_ERROR << "public_items is empty!";
            return;
        }

        std::vector<Item> l_PublicItemVec;

        do
        {
            ResultSet* l_Result = l_PreparedResultSet->FetchResult();

            Item l_PublicItem;
            l_PublicItem.m_Id           = l_Result[1].GetUInt32();
            l_PublicItem.m_RoomModel    = l_Result[2].GetString();
            l_PublicItem.m_Sprite       = l_Result[3].GetString();
            l_PublicItem.m_X            = l_Result[4].GetInt32();
            l_PublicItem.m_Y            = l_Result[5].GetInt32();
            l_PublicItem.m_Z            = l_Result[6].GetDouble();
            l_PublicItem.m_Rotation     = l_Result[7].GetInt32();
            l_PublicItem.m_TopHeight    = l_Result[8].GetDouble();
            l_PublicItem.m_Length       = l_Result[9].GetInt32();
            l_PublicItem.m_Width        = l_Result[10].GetInt32();

            if (!l_Result[11].GetString().empty())
            {
                std::vector<std::string> l_Split;
                boost::split(l_Split, l_Result[11].GetString(), boost::is_any_of(","));

                for (auto& l_Itr : l_Split)
                {
                    l_PublicItem.m_Trigger.push_back(l_Itr);
                }
            }

            l_PublicItem.m_Program      = l_Result[12].GetString();

            l_PublicItemVec.push_back(l_PublicItem);

        } while (l_PreparedResultSet->GetNextRow());

        delete l_PreparedResultSet;
        UserDatabase.FreePrepareStatement(l_PreparedStatement);

        std::vector<Item> picnic;
        std::vector<Item> newbie_lobby;
        std::vector<Item> theater;
        std::vector<Item> libary;
        std::vector<Item> floatinggarden;
        std::vector<Item> sunset_cafe;
        std::vector<Item> pool_a;
        std::vector<Item> pub_a;
        std::vector<Item> md_a;
        std::vector<Item> park_a;
        std::vector<Item> park_b;
        std::vector<Item> pool_b;
        std::vector<Item> ballroom;
        std::vector<Item> cafe_gold0;
        std::vector<Item> gate_park;
        std::vector<Item> gate_park_2;
        std::vector<Item> sun_terrace;
        std::vector<Item> space_cafe;
        std::vector<Item> beauty_salon0;
        std::vector<Item> chill;
        std::vector<Item> dusty_lounge;
        std::vector<Item> cr_staff;
        std::vector<Item> rooftop;
        std::vector<Item> rooftop_2;
        std::vector<Item> tearoom;
        std::vector<Item> cafe_ole;
        std::vector<Item> cr_cafe;
        std::vector<Item> lobby_a;
        std::vector<Item> floorlobby_c;
        std::vector<Item> floorlobby_b;
        std::vector<Item> cinema_a;
        std::vector<Item> old_skool0;
        std::vector<Item> malja_bar_a;
        std::vector<Item> malja_bar_b;
        std::vector<Item> bar_a;
        std::vector<Item> bar_b;
        std::vector<Item> habburger;
        std::vector<Item> pizza;
        std::vector<Item> bb_lobby_1;
        std::vector<Item> snowwar_lobby_1;
        std::vector<Item> tv_studio;
        std::vector<Item> club_mammoth;
        std::vector<Item> ice_cafe;
        std::vector<Item> netcafe;
        std::vector<Item> hallway0;
        std::vector<Item> hallway9;
        std::vector<Item> hallway2;
        std::vector<Item> hallway1;
        std::vector<Item> hallway3;
        std::vector<Item> hallway4;
        std::vector<Item> hallway5;
        std::vector<Item> hallway8;
        std::vector<Item> hallway7;
        std::vector<Item> hallway6;
        std::vector<Item> hallway10;
        std::vector<Item> hallway11;
        std::vector<Item> star_lounge;
        std::vector<Item> orient;
        std::vector<Item> entryhall;
        std::vector<Item> hallA;
        std::vector<Item> hallB;
        std::vector<Item> hallD;
        std::vector<Item> emperors;
        std::vector<Item> beauty_salon1;

        for (std::vector<Item>::iterator l_Itr = l_PublicItemVec.begin(); l_Itr != l_PublicItemVec.end(); l_Itr++)
        {
            if (l_Itr->GetRoomModel() == "picnic")
                picnic.push_back(*l_Itr);
            else if (l_Itr->GetRoomModel() == "newbie_lobby")
                newbie_lobby.push_back(*l_Itr);
            else if (l_Itr->GetRoomModel() == "theater")
                theater.push_back(*l_Itr);
            else if (l_Itr->GetRoomModel() == "libary")
                libary.push_back(*l_Itr);
            else if (l_Itr->GetRoomModel() == "floatinggarden")
                floatinggarden.push_back(*l_Itr);
            else if (l_Itr->GetRoomModel() == "sunset_cafe")
                sunset_cafe.push_back(*l_Itr);
            else if (l_Itr->GetRoomModel() == "pool_a")
                pool_a.push_back(*l_Itr);
            else if (l_Itr->GetRoomModel() == "pool_b")
                pool_b.push_back(*l_Itr);
            else if (l_Itr->GetRoomModel() == "pub_a")
                pub_a.push_back(*l_Itr);
            else if (l_Itr->GetRoomModel() == "md_a")
                md_a.push_back(*l_Itr);
            else if (l_Itr->GetRoomModel() == "picnic")
                picnic.push_back(*l_Itr);
            else if (l_Itr->GetRoomModel() == "park_a")
                park_a.push_back(*l_Itr);
            else if (l_Itr->GetRoomModel() == "park_b")
                park_b.push_back(*l_Itr);
            else if (l_Itr->GetRoomModel() == "ballroom")
                ballroom.push_back(*l_Itr);
            else if (l_Itr->GetRoomModel() == "cafe_gold0")
                cafe_gold0.push_back(*l_Itr);
            else if (l_Itr->GetRoomModel() == "gate_park")
                gate_park.push_back(*l_Itr);
            else if (l_Itr->GetRoomModel() == "gate_park_2")
                gate_park_2.push_back(*l_Itr);
            else if (l_Itr->GetRoomModel() == "sun_terrace")
                sun_terrace.push_back(*l_Itr);
            else if (l_Itr->GetRoomModel() == "beauty_salon0")
                beauty_salon0.push_back(*l_Itr);
            else if (l_Itr->GetRoomModel() == "chill")
                chill.push_back(*l_Itr);
            else if (l_Itr->GetRoomModel() == "dusty_lounge")
                dusty_lounge.push_back(*l_Itr);
            else if (l_Itr->GetRoomModel() == "cr_staff")
                cr_staff.push_back(*l_Itr);
            else if (l_Itr->GetRoomModel() == "rooftop")
                rooftop.push_back(*l_Itr);
            else if (l_Itr->GetRoomModel() == "rooftop_2")
                rooftop_2.push_back(*l_Itr);
            else if (l_Itr->GetRoomModel() == "tearoom")
                tearoom.push_back(*l_Itr);
            else if (l_Itr->GetRoomModel() == "cafe_ole")
                cafe_ole.push_back(*l_Itr);
            else if (l_Itr->GetRoomModel() == "cr_cafe")
                cr_cafe.push_back(*l_Itr);
            else if (l_Itr->GetRoomModel() == "lobby_a")
                lobby_a.push_back(*l_Itr);
            else if (l_Itr->GetRoomModel() == "floorlobby_c")
                floorlobby_c.push_back(*l_Itr);
            else if (l_Itr->GetRoomModel() == "floorlobby_b")
                floorlobby_b.push_back(*l_Itr);
            else if (l_Itr->GetRoomModel() == "cinema_a")
                cinema_a.push_back(*l_Itr);
            else if (l_Itr->GetRoomModel() == "old_skool0")
                old_skool0.push_back(*l_Itr);
            else if (l_Itr->GetRoomModel() == "malja_bar_a")
                malja_bar_a.push_back(*l_Itr);
            else if (l_Itr->GetRoomModel() == "malja_bar_b")
                malja_bar_b.push_back(*l_Itr);
            else if (l_Itr->GetRoomModel() == "bar_a")
                bar_a.push_back(*l_Itr);
            else if (l_Itr->GetRoomModel() == "bar_b")
                bar_b.push_back(*l_Itr);
            else if (l_Itr->GetRoomModel() == "habburger")
                habburger.push_back(*l_Itr);
            else if (l_Itr->GetRoomModel() == "pizza")
                pizza.push_back(*l_Itr);
            else if (l_Itr->GetRoomModel() == "bb_lobby_1")
                bb_lobby_1.push_back(*l_Itr);
            else if (l_Itr->GetRoomModel() == "snowwar_lobby_1")
                snowwar_lobby_1.push_back(*l_Itr);
            else if (l_Itr->GetRoomModel() == "tv_studio")
                tv_studio.push_back(*l_Itr);
            else if (l_Itr->GetRoomModel() == "club_mammoth")
                club_mammoth.push_back(*l_Itr);
            else if (l_Itr->GetRoomModel() == "ice_cafe")
                ice_cafe.push_back(*l_Itr);
            else if (l_Itr->GetRoomModel() == "netcafe")
                netcafe.push_back(*l_Itr);
            else if (l_Itr->GetRoomModel() == "hallway0")
                hallway0.push_back(*l_Itr);
            else if (l_Itr->GetRoomModel() == "hallway9")
                hallway9.push_back(*l_Itr);
            else if (l_Itr->GetRoomModel() == "hallway2")
                hallway2.push_back(*l_Itr);
            else if (l_Itr->GetRoomModel() == "hallway1")
                hallway1.push_back(*l_Itr);
            else if (l_Itr->GetRoomModel() == "hallway3")
                hallway3.push_back(*l_Itr);
            else if (l_Itr->GetRoomModel() == "hallway5")
                hallway5.push_back(*l_Itr);
            else if (l_Itr->GetRoomModel() == "hallway8")
                hallway8.push_back(*l_Itr);
            else if (l_Itr->GetRoomModel() == "hallway7")
                hallway7.push_back(*l_Itr);
            else if (l_Itr->GetRoomModel() == "hallway6")
                hallway6.push_back(*l_Itr);
            else if (l_Itr->GetRoomModel() == "hallway10")
                hallway10.push_back(*l_Itr);
            else if (l_Itr->GetRoomModel() == "hallway11")
                hallway11.push_back(*l_Itr);
            else if (l_Itr->GetRoomModel() == "star_lounge")
                star_lounge.push_back(*l_Itr);
            else if (l_Itr->GetRoomModel() == "orient")
                orient.push_back(*l_Itr);
            else if (l_Itr->GetRoomModel() == "entryhall")
                entryhall.push_back(*l_Itr);
            else if (l_Itr->GetRoomModel() == "hallA")
                hallA.push_back(*l_Itr);
            else if (l_Itr->GetRoomModel() == "hallB")
                hallB.push_back(*l_Itr);
            else if (l_Itr->GetRoomModel() == "hallD")
                hallD.push_back(*l_Itr);
            else if (l_Itr->GetRoomModel() == "emperors")
                emperors.push_back(*l_Itr);
            else if (l_Itr->GetRoomModel() == "beauty_salon1")
                beauty_salon1.push_back(*l_Itr);
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
    
    /// GetPublicRoomItems
    /// @p_Model : Room Model to retrieve furniture data
    PublicItemVec ItemManager::GetPublicRoomItems(const std::string& model)
    {
        PublicItemMap::iterator l_Itr = m_PublicItems.find(model);
        if (l_Itr != m_PublicItems.end())
            return l_Itr->second;
        else
            return PublicItemVec{};
    }

    /// GetPublicItemByPosition
    /// @p_Model : Room Model
    /// @p_X : X axis of public item position
    /// @p_X : Y axis of public item position
    Item* ItemManager::GetPublicItemByPosition(std::string const p_Model, int16 const p_X, int16 const p_Y)
    {
        PublicItemMap::iterator l_Itr = m_PublicItems.find(p_Model);
        if (l_Itr != m_PublicItems.end())
        {
            for (auto& l_ItemItr : l_Itr->second)
            {
                if (l_ItemItr.GetPositionX() == p_X && l_ItemItr.GetPositionY() == p_Y)
                    return &l_ItemItr;
            }
        }
        return nullptr;
    }
} ///< NAMESPACE STEERSTONE
