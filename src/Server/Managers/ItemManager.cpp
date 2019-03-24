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
//-----------------------------------------------//
#include "ItemManager.h"
#include "Network/StringBuffer.h"
#include "Database/QueryDatabase.h"
//-----------------------------------------------//
namespace SteerStone
{
    //-----------------------------------------------//
    ItemManager* ItemManager::instance()
    {
        static ItemManager instance;
        return &instance;
    }
    //-----------------------------------------------//
    ItemManager::ItemManager()
    {
    }
    ItemManager::~ItemManager()
    {
    }
    //-----------------------------------------------//
    void ItemManager::LoadPublicRoomItems()
    {
        QueryDatabase database("rooms");
        database.PrepareQuery("SELECT id, room_model, sprite, x, y, z, rotation, top_height, length, width, behaviour FROM public_items");
        database.ExecuteQuery();

        if (!database.GetResult())
        {
            LOG_ERROR << "public_items is empty!";
            return;
        }

        Result* result = database.Fetch();

        std::vector<PublicItem> publicItemVec;

        do
        {
            PublicItem publicItem;
            publicItem.m_Id = result->GetUint32(1);
            publicItem.mRoomModel = result->GetString(2);
            publicItem.mSprite = result->GetString(3);
            publicItem.mX = result->GetInt32(4);
            publicItem.mY = result->GetInt32(5);
            publicItem.mZ = result->GetDouble(6);
            publicItem.mRotation = result->GetInt32(7);
            publicItem.mTopHeight = result->GetDouble(8);
            publicItem.mLength = result->GetInt32(9);
            publicItem.mWidth = result->GetInt32(10);
            publicItem.mBehaviour = result->GetString(11);

            publicItemVec.push_back(publicItem);

        } while (result->GetNextResult());

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

        for (std::vector<PublicItem>::iterator itr = publicItemVec.begin(); itr != publicItemVec.end(); itr++)
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

        mPublicItems["picnic"] = picnic;
        mPublicItems["newbie_lobby"] = newbie_lobby;
        mPublicItems["theater"] = theater;
        mPublicItems["libary"] = libary;
        mPublicItems["floatinggarden"] = floatinggarden;
        mPublicItems["sunset_cafe"] = sunset_cafe;
        mPublicItems["pool_a"] = pool_a;
        mPublicItems["pub_a"] = pub_a;
        mPublicItems["md_a"] = md_a;
        mPublicItems["park_a"] = park_a;
        mPublicItems["park_b"] = park_b;
        mPublicItems["pool_b"] = pool_b;
        mPublicItems["ballroom"] = ballroom;
        mPublicItems["cafe_gold0"] = cafe_gold0;
        mPublicItems["gate_park"] = gate_park;
        mPublicItems["gate_park_2"] = gate_park_2;
        mPublicItems["sun_terrace"] = sun_terrace;
        mPublicItems["space_cafe"] = space_cafe;
        mPublicItems["beauty_salon0"] = beauty_salon0;
        mPublicItems["chill"] = chill;
        mPublicItems["dusty_lounge"] = dusty_lounge;
        mPublicItems["cr_staff"] = cr_staff;
        mPublicItems["rooftop"] = rooftop;
        mPublicItems["rooftop_2"] = rooftop_2;
        mPublicItems["tearoom"] = tearoom;
        mPublicItems["cafe_ole"] = cafe_ole;
        mPublicItems["cr_cafe"] = cr_cafe;
        mPublicItems["lobby_a"] = lobby_a;
        mPublicItems["floorlobby_c"] = floorlobby_c;
        mPublicItems["floorlobby_b"] = floorlobby_b;
        mPublicItems["cinema_a"] = cinema_a;
        mPublicItems["old_skool0"] = old_skool0;
        mPublicItems["malja_bar_a"] = malja_bar_a;
        mPublicItems["malja_bar_b"] = malja_bar_b;
        mPublicItems["bar_a"] = bar_a;
        mPublicItems["bar_b"] = bar_b;
        mPublicItems["habburger"] = habburger;
        mPublicItems["pizza"] = pizza;
        mPublicItems["bb_lobby_1"] = bb_lobby_1;
        mPublicItems["snowwar_lobby_1"] = snowwar_lobby_1;
        mPublicItems["tv_studio"] = tv_studio;
        mPublicItems["club_mammoth"] = club_mammoth;
        mPublicItems["ice_cafe"] = ice_cafe;
        mPublicItems["netcafe"] = netcafe;
        mPublicItems["hallway0"] = hallway0;
        mPublicItems["hallway9"] = hallway9;
        mPublicItems["hallway2"] = hallway2;
        mPublicItems["hallway1"] = hallway1;
        mPublicItems["hallway3"] = hallway3;
        mPublicItems["hallway4"] = hallway4;
        mPublicItems["hallway5"] = hallway5;
        mPublicItems["hallway8"] = hallway8;
        mPublicItems["hallway7"] = hallway7;
        mPublicItems["hallway6"] = hallway6;
        mPublicItems["hallway10"] = hallway10;
        mPublicItems["hallway11"] = hallway11;
        mPublicItems["star_lounge"] = star_lounge;
        mPublicItems["orient"] = orient;
        mPublicItems["entryhall"] = entryhall;
        mPublicItems["hallA"] = hallA;
        mPublicItems["hallB"] = hallB;
        mPublicItems["hallD"] = hallD;
        mPublicItems["emperors"] = emperors;
        mPublicItems["beauty_salon1"] = beauty_salon1;

        LOG_INFO << "Loaded " << publicItemVec.size() << " Room Public Items";
    }
    //-----------------------------------------------//
    PublicItemVec ItemManager::GetPublicRoomItems(const std::string& model)
    {
        PublicItemMap::iterator itr = mPublicItems.find(model);
        if (itr != mPublicItems.end())
            return itr->second;
        else
            return PublicItemVec{};
    }
    //-----------------------------------------------//
}
//-----------------------------------------------//