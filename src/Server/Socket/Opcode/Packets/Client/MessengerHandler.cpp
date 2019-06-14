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

#include "HabboSocket.h"
#include "Habbo.h"

#include "Opcode/Packets/Server/MessengerPackets.h"

namespace SteerStone
{
    void HabboSocket::HandleMessengerInitialize(ClientPacket* p_Packet)
    {
        m_Habbo->SendInitializeMessenger();
    }

    void HabboSocket::HandleMessengerUpdate(ClientPacket* p_Packet)
    {
        m_Habbo->SendMessengerUpdate();
    }

    void HabboSocket::HandleMessengerAcceptBuddy(ClientPacket* p_Packet)
    {
        uint32 l_SenderId = p_Packet->ReadWiredInt();

        m_Habbo->MessengerAcceptRequest(l_SenderId);
    }

    void HabboSocket::HandleMessengerFindUser(ClientPacket* p_Packet)
    {
        std::string l_Name = p_Packet->ReadString();

        m_Habbo->SendSearchUserResults(l_Name);
    }

    void HabboSocket::HandleMessengerSendRequest(ClientPacket* p_Packet)
    {
        std::string l_Name = p_Packet->ReadString();

        m_Habbo->MessengerBuddyRequest(l_Name);
    }

    void HabboSocket::HandleMessengerRemoveBuddy(ClientPacket* p_Packet)
    {
        m_Habbo->MessengerRemoveBuddy(std::move(p_Packet));
    }

    void HabboSocket::HandleMessengerRejectBuddy(ClientPacket* p_Packet)
    {
        m_Habbo->MessengerRejectRequest(std::move(p_Packet));
    }

    void HabboSocket::HandleMessengerSendMessage(ClientPacket* p_Packet)
    {
        m_Habbo->MessengerSendMessage(std::move(p_Packet));
    }

    void HabboSocket::HandleMessengerMarkRead(ClientPacket* p_Packet)
    {
        uint32 l_MessageId = p_Packet->ReadWiredUint();

        m_Habbo->MessengerReply(l_MessageId);
    }

    void HabboSocket::HandleConsoleMotto(ClientPacket* p_Packet)
    {
        /// TODO; Filter motto
        std::string l_Motto = p_Packet->ReadString();

        /// Set our new motto
        m_Habbo->m_ConsoleMotto = l_Motto;

        /// Update client with our new motto
        HabboPacket::Messenger::PersistentMessage l_Packet;
        l_Packet.Motto = l_Motto;
        SendPacket(l_Packet.Write());
    }
} /// NAMESPACE STEERSTONE
