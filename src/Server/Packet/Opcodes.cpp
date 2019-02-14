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
#include "../WorldSession.h"
#include "Opcodes.h"
//-----------------------------------------------//
PacketTable * PacketTable::instance()
{
    static PacketTable instance;
    return &instance;
}
//-----------------------------------------------//
PacketHandlerStruct const PacketTable::emptyHandler =
{
    "<none>",
    &WorldSession::HandleNULL
};
//-----------------------------------------------//
void PacketTable::InitializePackets()
{
    StorePacket(OpcodesList::VERSIONCHECK,               "VERSIONCHECK",             &WorldSession::HandleVersionCheck       );
    StorePacket(OpcodesList::KEYENCRYPTED,               "KEYENCRYPTED",             &WorldSession::HandleKeyEncrypted       );
    StorePacket(OpcodesList::CLIENTIP,                   "CLIENTIP",                 &WorldSession::HandleClientIp           );
    StorePacket(OpcodesList::APPROVENAME,                "APPROVENAME",              &WorldSession::HandleApproveName        );
    StorePacket(OpcodesList::REGISTER,                   "REGISTER",                 &WorldSession::HandleRegister           );
    StorePacket(OpcodesList::LOGIN,                      "LOGIN",                    &WorldSession::HandleLogin              );
    StorePacket(OpcodesList::GETCREDITS,                 "GETCREDITS",               &WorldSession::HandleGetCredits         );
    StorePacket(OpcodesList::STATUSOK,                   "STATUSOK",                 &WorldSession::HandleStatusOk           );
    StorePacket(OpcodesList::SEARCHBUSYFLATS,            "SEARCHBUSYFLATS",          &WorldSession::HandleSearchBusyFlats    );
    StorePacket(OpcodesList::INFORETRIEVE,               "INFORETRIEVE",             &WorldSession::HandleInfoRetrieve       );
    StorePacket(OpcodesList::INITUNITLISTENER,           "INITUNITLISTENER",         &WorldSession::HandleInitUnitListener   );
    StorePacket(OpcodesList::MOVE,                       "MOVE",                     &WorldSession::HandleMove               );
    StorePacket(OpcodesList::CHAT,                       "CHAT",                     &WorldSession::HandleChat               );
    StorePacket(OpcodesList::SHOUT,                      "SHOUT",                    &WorldSession::HandleShout              );
    StorePacket(OpcodesList::WHISPER,                    "WHISPER",                  &WorldSession::HandleWhisper            );
    StorePacket(OpcodesList::CREATEFLAT,                 "CREATEFLAT",               &WorldSession::HandleCreateFlat         );
    StorePacket(OpcodesList::SETFLATINFO,                "SETFLATINFO",              &WorldSession::HandleSetFlatInfo        );
    StorePacket(OpcodesList::TRYFLAT,                    "TRYFLAT",                  &WorldSession::HandleTryFlat            );
    StorePacket(OpcodesList::GOTOFLAT,                   "GOTOFLAT",                 &WorldSession::HandleGoToFlat           );
    StorePacket(OpcodesList::GETFLATINFO,                "GETFLATINFO",              &WorldSession::HandleGetFlatInfo        );
    StorePacket(OpcodesList::UPDATEFLAT,                 "UPDATEFLAT",               &WorldSession::HandleUpdateFlat         );
    StorePacket(OpcodesList::DELETEFLAT,                 "DELETEFLAT",               &WorldSession::HandleDeleteFlat         );
    StorePacket(OpcodesList::GOAWAY,                     "GOAWAY",                   &WorldSession::HandleGoAway             );
    StorePacket(OpcodesList::DANCE,                      "DANCE",                    &WorldSession::HandleDance              );
    StorePacket(OpcodesList::STOP,                       "STOP",                     &WorldSession::HandleStop               );
    StorePacket(OpcodesList::LOOKTO,                     "LOOKTO",                   &WorldSession::HandleLookTo             );
    StorePacket(OpcodesList::GETORDERINFO,               "GETORDERINFO",             &WorldSession::HandleOrderInfo          );
    StorePacket(OpcodesList::PURCHASE,                   "PURCHASE",                 &WorldSession::HandlePurchase           );
    StorePacket(OpcodesList::GETSTRIP,                   "GETSTRIP",                 &WorldSession::HandleGetStrip           );
}
//-----------------------------------------------//
void PacketTable::StorePacket(uint16 opcode, char const * name, void(WorldSession::* handler)(std::string &packet, std::vector<std::string>& packetStorage))
{
    PacketHandlerStruct& ref = mPacketMap[opcode];
    ref.name = name;
    ref.handler = handler;
}
//-----------------------------------------------//
PacketHandlerStruct const & PacketTable::GetPacket(uint16 Id)
{
    OpcodeMap::const_iterator itr = mPacketMap.find(Id);
    if (itr != mPacketMap.end())
        return itr->second;
    return emptyHandler;
}
//-----------------------------------------------//