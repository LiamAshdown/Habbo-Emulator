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
//-----------------------------------------------//
namespace Quad
{
    //-----------------------------------------------//
    PlayerSocket::PlayerSocket(boost::asio::io_service& service, std::function<void(Socket*)> closeHandler) :
        Socket(service, std::move(closeHandler))
    {
        IF_LOG(plog::debug)
            sDatabase->GetDatabase("users")->GetConnectionPool()->GetStats();
    }
    //-----------------------------------------------//
    PlayerSocket::~PlayerSocket()
    {
        IF_LOG(plog::debug)
            LOG_DEBUG << "Destructor PlayerSocket called!";

        mPlayer.reset();
    }
    //-----------------------------------------------//
    std::shared_ptr<Player> PlayerSocket::ToPlayer()
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
            LOG_INFO << "Processing packet: " << packet->sHeader;

            ExecutePacket(sOpcode->GetPacket(packet->sCmd), std::move(packet));
            return true;
        }
        else
            return false;
    }
    //-----------------------------------------------//
    std::unique_ptr<Packet> PlayerSocket::DecryptPacket()
    {

        std::vector<uint8> buffer;
        buffer.resize(4);
        if (Read((char*)&buffer[0], 4))
        {
            std::unique_ptr<Packet> packet = std::make_unique<Packet>();

            packet->sLength = std::stoi((std::string)(char*)&buffer[0]);

            buffer.clear();

            buffer.resize(packet->sLength);

            Read((char*)&buffer[0], packet->sLength);

            // Get rid of junk characters
            buffer.resize(buffer.size() + 1);
            buffer[buffer.size() - 1] = 0;

            std::string tempString = (char*)&buffer[0];

            packet->sLength = ReadLength();
            packet->sHeader = tempString.substr(0, tempString.find(' '));
            tempString = tempString.erase(0, tempString.find_first_of(" \t") + 1);
            packet->sFullBody = tempString;

            boost::split(packet->sBody, tempString, boost::is_any_of("\t "));
            packet->sCmd = static_cast<uint16>(std::accumulate(packet->sHeader.begin(), packet->sHeader.end(), 0));

            return std::move(packet);
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