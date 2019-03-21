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
#include "Network/StringBuffer.h"
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
        Player* ToPlayer();
        void DestroyPlayer();

    public:
        void SendPacket(StringBuffer& buffer);

        ////////////////////////
        //      HANDLERS      //
        ///////////////////////
        void ExecutePacket(const OpcodeHandler& opHandler, std::unique_ptr<Packet> packet);
        void HandleInitializeCrypto(std::unique_ptr<Packet> packet);
        void HandleGenerateKey(std::unique_ptr<Packet> packet);
        void HandleGDate(std::unique_ptr<Packet> packet);
        void HandleApproveUsername(std::unique_ptr<Packet> packet);
        void HandleApprovePassword(std::unique_ptr<Packet> packet);
        void HandleApproveEmail(std::unique_ptr<Packet> packet);
        void HandleRegisteration(std::unique_ptr<Packet> packet);
        void HandleTryLogin(std::unique_ptr<Packet> packet);
        void HandleGetInfo(std::unique_ptr<Packet> packet);
        void HandleCreditBalance(std::unique_ptr<Packet> packet);
        void HandleNoDespaceUsers(std::unique_ptr<Packet> packet);
        void HandleGetClub(std::unique_ptr<Packet> packet);
        void HandleNavigate(std::unique_ptr<Packet> packet);
        void HandleGetUserFlatsCategories(std::unique_ptr<Packet> packet);
        void HandleGetFavouriteRooms(std::unique_ptr<Packet> packet);
        void HandleAddFavouriteRoom(std::unique_ptr<Packet> packet);
        void HandleRemoveFavouriteRoom(std::unique_ptr<Packet> packet);
        void HandleSearchRooms(std::unique_ptr<Packet> packet);
        void HandleGetInterest(std::unique_ptr<Packet> packet);
        void HandleMessengerInitialize(std::unique_ptr<Packet> packet);
        void HandleRoomDirectory(std::unique_ptr<Packet> packet);
        void HandlePong(std::unique_ptr<Packet> packet);
        void HandleGetRoomAdd(std::unique_ptr<Packet> packet);
        void HandleGetRoomHeight(std::unique_ptr<Packet> packet);
        void HandleGetAvailableBadges(std::unique_ptr<Packet> packet);
        void HandleGetAccountPreferences(std::unique_ptr<Packet> packet);
        void HandleRoomUsers(std::unique_ptr<Packet> packet);
        void HandleMessengerUpdate(std::unique_ptr<Packet> packet);
        void HandleMessengerAcceptRequest(std::unique_ptr<Packet> packet);
        void HandleGameObjects(std::unique_ptr<Packet> packet);
        void HandleFurnitureRevisions(std::unique_ptr<Packet> packet);
        void HandleServerMessage(std::unique_ptr<Packet> packet);
        void HandleNULL(std::unique_ptr<Packet> packet);

    private:
        virtual bool ProcessIncomingData() override;

    private:
        Player* mPlayer;
    };
}

#endif /* _Quad_PlayerSocket_h_ */