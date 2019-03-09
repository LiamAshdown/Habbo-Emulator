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

namespace Quad
{
    class Player;

    class PlayerSocket : public Socket
    {
    public:
        PlayerSocket(boost::asio::io_service& service, std::function<void(Socket*)> closeHandler);
        ~PlayerSocket();

    public:
        std::shared_ptr<Player> ToPlayer();

    public:
        void SendPacket(const char* buffer, const std::size_t& length);

        ////////////////////////
        //      HANDLERS      //
        ///////////////////////
        void ExecutePacket(const OpcodeHandler& opHandler, std::unique_ptr<Packet> packet);
        void HandleVersionCheck(std::unique_ptr<Packet> packet);
        void HandleRegisteration(std::unique_ptr<Packet> packet);
        void HandleApproveUsername(std::unique_ptr<Packet> packet);
        void HandleClientIP(std::unique_ptr<Packet> packe);
        void HandleLogin(std::unique_ptr<Packet> packet);
        void HandleGetCredits(std::unique_ptr<Packet> packet);
        void HandleServerMessage(std::unique_ptr<Packet> packet);
        void HandleNULL(std::unique_ptr<Packet> packet);

    private:
        virtual bool ProcessIncomingData() override;

    private:
        std::unique_ptr<Packet> DecryptPacket();

    private:
        std::shared_ptr<Player> mPlayer;
    };
}

#endif /* _Quad_PlayerSocket_h_ */