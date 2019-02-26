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
#include <numeric>
#include "../WorldSession.h"
#include "../Packet/Opcodes.h"
#include "Socket.h"
//-----------------------------------------------//
Socket::Socket(boost::asio::io_service & socket, uint32& port) : mSocket(socket), mWorldSession(nullptr), mPort(port)
{
    mOutBuffer.reset(new PacketBuffer);
    mInBuffer.reset(new PacketBuffer);
    mSocketOpen = true;
}
//-----------------------------------------------//
void Socket::ReadHandler()
{
    mSocket.async_receive(boost::asio::buffer(&mInBuffer->mBuffer[mInBuffer->mWritePosition], mInBuffer->mBuffer.size() - mInBuffer->mWritePosition),
        boost::bind(
            &Socket::ReadOnComing, shared_from_this(),
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));
}
//-----------------------------------------------//
void Socket::ReadOnComing(const boost::system::error_code& error, const std::size_t& length)
{
    if (error)
    {
        CloseSocket();
        return;
    }

    if (!IsSocketOpen())
        return;

    mInBuffer->mWritePosition += length;

    const std::size_t available = mSocket.available();

    if (available > 0 && (length + available) > mInBuffer->mBuffer.size())
    {
        mInBuffer->mBuffer.resize(mInBuffer->mBuffer.size() + available);
        ReadHandler();
        return;
    }

    while (mInBuffer->mReadPosition < mInBuffer->mWritePosition)
    {
        if (!ProcessHandler())
        {

        }
    }

    mInBuffer->mWritePosition = 0;
    mInBuffer->mReadPosition = 0;
    ReadHandler();
}
//-----------------------------------------------//
bool Socket::ProcessHandler()
{
    std::vector<uint8> buffer;
    buffer.resize(4);
    std::size_t headerSize = 4;
    Read((char*)&buffer[0], headerSize);
    // This can lead to a crash if someone the client incorrectly sends wrong packet size,
    // and we end up converting int to int, but this shouldn't happen at all
    std::size_t remaining = (std::stoi((std::string)(char*)&buffer[0]));
    if (remaining > ReadLengthRemaining())
        remaining = ((buffer.size() - (buffer.size() - ReadLengthRemaining())));

    buffer.clear();

    buffer.resize(remaining);
    Read((char*)&buffer[0], remaining);
    buffer.resize(buffer.size() + 1);
    buffer[buffer.size() - 1] = 0;

    std::string stringBuffer = (char*)&buffer[0];
    std::vector<std::string> splitData;
    boost::split(splitData, stringBuffer, boost::is_any_of("\t "));

    const uint16& opcode = static_cast<uint16>(std::accumulate(splitData[0].begin(), splitData[0].end(), 0));
    std::cout << opcode << std::endl;
    std::cout << stringBuffer << std::endl;
    PacketHandlerStruct const& opHandle = sPacketHandler->GetPacket(opcode);
    mWorldSession->ExecutePacket(opHandle, stringBuffer, splitData);
    return true;
}
//-----------------------------------------------//
void Socket::SendAuthResponse()
{
    WorldPacket data("# HELLO");
    data.AppendCarriage();
    data.AppendEndCarriage();
    SendPacket(data.Write());

    mWorldSession = new WorldSession(shared_from_this());
}
//-----------------------------------------------//
void Socket::SendPacket(const WorldPacket& packet)
{
    if (!IsSocketOpen())
        return;

    mOutBuffer->Write((const char*)packet.GetContents(), packet.GetSize());
    boost::asio::async_write(mSocket,
        boost::asio::buffer(mOutBuffer->mBuffer, mOutBuffer->mWritePosition),
        boost::bind(&Socket::LogPacket, shared_from_this(),
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));
}
//-----------------------------------------------//
void Socket::Read(char* buffer, const std::size_t& length)
{
    mInBuffer->Read(buffer, length);
}
//-----------------------------------------------//
void Socket::LogPacket(const boost::system::error_code& error, const std::size_t& length)
{
    // mOutBuffer is a smart pointer, gets deleted automatically when we allocate new
    mOutBuffer.reset(new PacketBuffer);
}
//-----------------------------------------------//
void Socket::CloseSocket()
{
    if (IsSocketOpen())
    {
        mWorldSession->LogoutPlayer(true);
        mSocketOpen = false;
        mSocket.shutdown(boost::asio::ip::tcp::socket::shutdown_both);
        mSocket.close();
    }
}
//-----------------------------------------------//
const char * Socket::GetRealmIP() const
{
    return "127.0.0.1";
}
//-----------------------------------------------//
std::size_t Socket::ReadLengthRemaining() const
{
    return mInBuffer->ReadLengthRemaining();
}
//-----------------------------------------------//