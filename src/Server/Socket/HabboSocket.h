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

#pragma once
#include "Network/Listener.h"
#include "Opcode/Opcodes.h"
#include "Network/ServerPacket.h"

namespace SteerStone
{
    class Habbo;

    /// Class which inherits from Socket class when a successfull connection is made
    class HabboSocket : public Socket
    {
    public:
        /// Constructor 
        /// @p_Service : Boost Service
        /// @p_CloseHandler : Close Handler Custom function
        HabboSocket(boost::asio::io_service& p_Service, std::function<void(Socket*)> p_CloseHandler);
        /// Deconstructor
        ~HabboSocket() {}

    public:
        /// ToHabbo - Return our Habbo Class
        Habbo* ToHabbo();

        /// DestroyHabbo - Set our Habbo class to nullptr
        void DestroyHabbo();

    public:
        /// SendPacket 
        /// @p_Buffer : Buffer which holds our data to be send to the client
        void SendPacket(StringBuffer const* p_Buffer);

        /// Client Handlers
        void ExecutePacket(const OpcodeHandler& p_Handler, std::unique_ptr<ClientPacket> p_Packet);
        void HandleInitializeCrypto(std::unique_ptr<ClientPacket> p_Packet);
        void HandleGenerateKey(std::unique_ptr<ClientPacket> p_Packet);
        void HandleGDate(std::unique_ptr<ClientPacket> p_Packet);
        void HandleApproveUsername(std::unique_ptr<ClientPacket> p_Packet);
        void HandleApprovePassword(std::unique_ptr<ClientPacket> p_Packet);
        void HandleApproveEmail(std::unique_ptr<ClientPacket> p_Packet);
        void HandleRegisteration(std::unique_ptr<ClientPacket> p_Packet);
        void HandleTryLogin(std::unique_ptr<ClientPacket> p_Packet);
        void HandleGetInfoRetrieve(std::unique_ptr<ClientPacket> p_Packet);
        void HandleCreditBalance(std::unique_ptr<ClientPacket> p_Packet);
        void HandleNodeSpaceUsers(std::unique_ptr<ClientPacket> p_Packet);
        void HandleGetClub(std::unique_ptr<ClientPacket> p_Packet);
        void HandleNavigate(std::unique_ptr<ClientPacket> p_Packet);
        void HandleGetUserFlatsCategories(std::unique_ptr<ClientPacket> p_Packet);
        void HandleGetFavouriteRooms(std::unique_ptr<ClientPacket> p_Packet);
        void HandleAddFavouriteRoom(std::unique_ptr<ClientPacket> p_Packet);
        void HandleRemoveFavouriteRoom(std::unique_ptr<ClientPacket> p_Packet);
        void HandleSearchRooms(std::unique_ptr<ClientPacket> p_Packet);
        void HandleGetInterest(std::unique_ptr<ClientPacket> p_Packet);
        void HandleMessengerInitialize(std::unique_ptr<ClientPacket> p_Packet);
        void HandleRoomDirectory(std::unique_ptr<ClientPacket> p_Packet);
        void HandlePong(std::unique_ptr<ClientPacket> p_Packet);
        void HandleGetRoomAdd(std::unique_ptr<ClientPacket> p_Packet);
        void HandleGHeightMap(std::unique_ptr<ClientPacket> p_Packet);
        void HandleGetAvailableBadges(std::unique_ptr<ClientPacket> p_Packet);
        void HandleGetAccountPreferences(std::unique_ptr<ClientPacket> p_Packet);
        void HandleGUsers(std::unique_ptr<ClientPacket> p_Packet);
        void HandleMessengerUpdate(std::unique_ptr<ClientPacket> p_Packet);
        void HandleMessengerAcceptBuddy(std::unique_ptr<ClientPacket> p_Packet);
        void HandleGObjects(std::unique_ptr<ClientPacket> p_Packet);
        void HandleFurnitureRevisions(std::unique_ptr<ClientPacket> p_Packet);
        void HandleMessengerFindUser(std::unique_ptr<ClientPacket> p_Packet);
        void HandleMessengerSendRequest(std::unique_ptr<ClientPacket> p_Packet);
        void HandleLeaveRoom(std::unique_ptr<ClientPacket> p_Packet);
        void HandleMessengerRemoveBuddy(std::unique_ptr<ClientPacket> p_Packet);
        void HandleMessengerRejectBuddy(std::unique_ptr<ClientPacket> p_Packet);
        void HandleMessengerSendMessage(std::unique_ptr<ClientPacket> p_Packet);
        void HandleMessengerMarkRead(std::unique_ptr<ClientPacket> p_Packet);
        void HandleMove(std::unique_ptr<ClientPacket> p_Packet);
        void HandleDance(std::unique_ptr<ClientPacket> p_Packet);
        void HandleStop(std::unique_ptr<ClientPacket> p_Packet);
        void HandleGStat(std::unique_ptr<ClientPacket> p_Packet);
        void HandleWave(std::unique_ptr<ClientPacket> p_Packet);
        void HandleLookTo(std::unique_ptr<ClientPacket> p_Packet);
        void HandleRoomChat(std::unique_ptr<ClientPacket> p_Packet);
        void HandleRoomShout(std::unique_ptr<ClientPacket> p_Packet);
        void HandleRoomWhisper(std::unique_ptr<ClientPacket> p_Packet);
        void HandleInfoBus(std::unique_ptr<ClientPacket> p_Packet);
        void HandleChangeWorld(std::unique_ptr<ClientPacket> p_Packet);
        void HandleSwimSuit(std::unique_ptr<ClientPacket> p_Packet);
        void HandleCreateFlat(std::unique_ptr<ClientPacket> p_Packet);
        void HandleSetFlatCategory(std::unique_ptr<ClientPacket> p_Packet);
        void HandleBuyHabboClub(std::unique_ptr<ClientPacket> p_Packet);
        void HandleNULL(std::unique_ptr<ClientPacket> p_Packet);
        void HandleServerMessage(std::unique_ptr<ClientPacket> p_Packet) {}

    private:
        /// ProcessIncomingData - Handle incoming data
        virtual bool ProcessIncomingData() override;

    private:
        Habbo* m_Habbo;                              ///< Habbo class
    };
} ///< NAMESPACE STEERSTONE