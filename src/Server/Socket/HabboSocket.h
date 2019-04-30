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
        void ExecutePacket(const OpcodeHandler& p_Handler, ClientPacket* p_Packet);
        void HandleInitializeCrypto(ClientPacket* p_Packet);
        void HandleGenerateKey(ClientPacket* p_Packet);
        void HandleGDate(ClientPacket* p_Packet);
        void HandleApproveUsername(ClientPacket* p_Packet);
        void HandleApprovePassword(ClientPacket* p_Packet);
        void HandleApproveEmail(ClientPacket* p_Packet);
        void HandleRegisteration(ClientPacket* p_Packet);
        void HandleTryLogin(ClientPacket* p_Packet);
        void HandleGetInfoRetrieve(ClientPacket* p_Packet);
        void HandleCreditBalance(ClientPacket* p_Packet);
        void HandleNodeSpaceUsers(ClientPacket* p_Packet);
        void HandleGetClub(ClientPacket* p_Packet);
        void HandleNavigate(ClientPacket* p_Packet);
        void HandleGetUserFlatsCategories(ClientPacket* p_Packet);
        void HandleGetFavouriteRooms(ClientPacket* p_Packet);
        void HandleAddFavouriteRoom(ClientPacket* p_Packet);
        void HandleRemoveFavouriteRoom(ClientPacket* p_Packet);
        void HandleSearchFlats(ClientPacket* p_Packet);
        void HandleGetInterest(ClientPacket* p_Packet);
        void HandleMessengerInitialize(ClientPacket* p_Packet);
        void HandleRoomDirectory(ClientPacket* p_Packet);
        void HandlePong(ClientPacket* p_Packet);
        void HandleGetRoomAdd(ClientPacket* p_Packet);
        void HandleGHeightMap(ClientPacket* p_Packet);
        void HandleGetAvailableBadges(ClientPacket* p_Packet);
        void HandleGetAccountPreferences(ClientPacket* p_Packet);
        void HandleGUsers(ClientPacket* p_Packet);
        void HandleMessengerUpdate(ClientPacket* p_Packet);
        void HandleMessengerAcceptBuddy(ClientPacket* p_Packet);
        void HandleGObjects(ClientPacket* p_Packet);
        void HandleFurnitureRevisions(ClientPacket* p_Packet);
        void HandleMessengerFindUser(ClientPacket* p_Packet);
        void HandleMessengerSendRequest(ClientPacket* p_Packet);
        void HandleLeaveRoom(ClientPacket* p_Packet);
        void HandleMessengerRemoveBuddy(ClientPacket* p_Packet);
        void HandleMessengerRejectBuddy(ClientPacket* p_Packet);
        void HandleMessengerSendMessage(ClientPacket* p_Packet);
        void HandleMessengerMarkRead(ClientPacket* p_Packet);
        void HandleMove(ClientPacket* p_Packet);
        void HandleDance(ClientPacket* p_Packet);
        void HandleStop(ClientPacket* p_Packet);
        void HandleGStat(ClientPacket* p_Packet);
        void HandleWave(ClientPacket* p_Packet);
        void HandleLookTo(ClientPacket* p_Packet);
        void HandleRoomChat(ClientPacket* p_Packet);
        void HandleRoomShout(ClientPacket* p_Packet);
        void HandleRoomWhisper(ClientPacket* p_Packet);
        void HandleInfoBus(ClientPacket* p_Packet);
        void HandleChangeWorld(ClientPacket* p_Packet);
        void HandleSwimSuit(ClientPacket* p_Packet);
        void HandleCreateFlat(ClientPacket* p_Packet);
        void HandleSetFlatInfo(ClientPacket* p_Packet);
        void HandleBuyHabboClub(ClientPacket* p_Packet);
        void HandleParentEmailRequired(ClientPacket* p_Packet);
        void HandleValidateParentEmail(ClientPacket* p_Packet);
        void HandleSetBadge(ClientPacket* p_Packet);
        void HandleConsoleMotto(ClientPacket* p_Packet);
        void HandleSearchUserFlats(ClientPacket* p_Packet);
        void HandleGetFlatInfo(ClientPacket* p_Packet);
        void HandleGetFlatCategory(ClientPacket* p_Packet);
        void HandleSetFlatCategory(ClientPacket* p_Packet);
        void HandleUpdateFlat(ClientPacket* p_Packet);
        void HandleGItems(ClientPacket* p_Packet);
        void HandleTryFlat(ClientPacket* p_Packet);
        void HandleGoToFlat(ClientPacket* p_Packet);
        void HandleAssignRights(ClientPacket* p_Packet);
        void HandleRemoveRights(ClientPacket* p_Packet);
        void HandleKickUser(ClientPacket* p_Packet);
        void HandleLetInUser(ClientPacket* p_Packet);
        void HandleRemoveAllRights(ClientPacket* p_Packet);
        void HandleDeleteFlat(ClientPacket* p_Packet);
        void HandleNULL(ClientPacket* p_Packet);
        void HandleServerMessage(ClientPacket* p_Packet) {}

    private:
        /// ProcessIncomingData - Handle incoming data
        virtual bool ProcessIncomingData() override;

    private:
        Habbo* m_Habbo;                              ///< Habbo class
    };
} ///< NAMESPACE STEERSTONE