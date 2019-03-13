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
#include "PlayerSocket.h"
#include "Database/QueryDatabase.h"
#include "Database/Fields.h"
#include "Common/SHA1.h"
#include "Player.h"
#include "Network/StringBuffer.h"
//-----------------------------------------------//
namespace Quad
{
    //-----------------------------------------------//
    PlayerSocket::PlayerSocket(boost::asio::io_service& service, std::function<void(Socket*)> closeHandler) :
        Socket(service, std::move(closeHandler)), mPlayer(nullptr)
    {

    }
    //-----------------------------------------------//
    PlayerSocket::~PlayerSocket()
    {
        IF_LOG(plog::debug)
            LOG_DEBUG << "Destructor PlayerSocket called!";

        if (mPlayer != nullptr)
            delete mPlayer;
    }
    //-----------------------------------------------//
    Player* PlayerSocket::ToPlayer()
    {
        return mPlayer;
    }
    //-----------------------------------------------//
    void PlayerSocket::SendPacket(const char* buffer, const std::size_t& length)
    {
        Write(buffer, length);
    }
    //-----------------------------------------------//
    bool PlayerSocket::ProcessIncomingData()
    {
        if (std::unique_ptr<Packet> packet = DecryptPacket())
        {
            LOG_INFO << "Processing packet: " << packet->GetHeader();

            ExecutePacket(sOpcode->GetClientPacket(packet->GetHeader()), std::move(packet));
            return true;
        }
        else
            return false;
    }
    //-----------------------------------------------//
    std::unique_ptr<Packet> PlayerSocket::DecryptPacket()
    {
        std::vector<uint8> bufferVec;
        bufferVec.resize(ReadLengthRemaining());
        if (Read((char*)&bufferVec[0], ReadLengthRemaining()))
        {
            std::unique_ptr<Packet> packet = std::make_unique<Packet>();

            packet->Parse(((std::string)(char*)&bufferVec[0])
                .substr(3, DecodeBase64(((std::string)(char*)&bufferVec[0]).substr(1, 2))));

            return packet;
        }
        else
            return nullptr;
    }
    //-----------------------------------------------//
    void PlayerSocket::ExecutePacket(const OpcodeHandler& opHandler, std::unique_ptr<Packet> packet)
    {
        (this->*opHandler.handler)(std::move(packet));
    }
    //-----------------------------------------------//
    void PlayerSocket::HandleNULL(std::unique_ptr<Packet> packet)
    {
    }
    //-----------------------------------------------//
    void PlayerSocket::HandleServerMessage(std::unique_ptr<Packet> packet)
    {
    }
    //-----------------------------------------------//
}