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

#ifndef _Quad_PlayerSocket_h_
#define _Quad_PlayerSocket_h_
#include "Network/Socket.h"
#include "Opcodes.h"
#endif /* _Quad_PlayerSocket_h_ */

typedef struct PacketReceivingStruct
{
    uint8 sLength[4];
}PacketReceiving;

typedef struct PacketStruct
{
    PacketStruct() : sLength(0), sCmd(0){}

    uint32 sLength;
    uint16 sCmd;
    std::string sHeader;
    std::string sFullBody;
    std::vector<std::string> sBody;

}Packet;