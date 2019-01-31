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

#ifndef _QuadEmu_Socket_h_
#define _QuadEmu_Socket_h_
#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>
#include "PacketBuffer.h"
#include "WorldPacket.h"
#include "SharedDefines.h"
#endif /* _QuadEmu_Socket_h_ */

class WorldSession;

typedef struct Packet
{
    std::vector<std::string> PacketData;
    std::size_t PacketSize;
}PacketStruct;

class Socket : public std::enable_shared_from_this<Socket>
{
public:
   explicit Socket(boost::asio::io_service& socket, uint32& port);
    ~Socket() {}

    // Socket handlers
    void ReadHandler();
    void ReadOnComing(const boost::system::error_code& error, const std::size_t& length);
    bool ProcessHandler();
    void SendAuthResponse();
    std::size_t ReadLengthRemaining() const;

    // Send/Read packet
    void SendPacket(const WorldPacket& packet);
    void Read(char* buffer, const std::size_t& length);
    void LogPacket(const boost::system::error_code& error, const std::size_t& length);

    // Misc
    int GetPort() const { return mPort; }
    boost::asio::ip::tcp::socket& GetSocket() { return mSocket; }
    bool IsSocketOpen() const { return mSocket.is_open(); }
    void CloseSocket();
    const char* GetRealmIP() const;

protected:
    boost::asio::ip::tcp::socket mSocket;
    std::unique_ptr<PacketBuffer> mOutBuffer;
    std::unique_ptr<PacketBuffer> mInBuffer;
    const int mPort;
    WorldSession* mWorldSession;
};

