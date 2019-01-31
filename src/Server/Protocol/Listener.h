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

#ifndef _QuadEmu_Listener_h_
#define _QuadEmu_Listener_h_
#include "Socket.h"
#endif /* _QuadEmu_Listener_ */

class Listener
{
public:

    Listener(boost::asio::io_service& ioService, uint32 port) : mIoService(ioService), mAcceptor(ioService, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)),
        mPort(port)
    {

        AcceptConnection();
    }

    ~Listener() {}

private:
    void AcceptConnection();
    void HandleAcceptConnection(std::shared_ptr<Socket> newSocket, const boost::system::error_code& errorCode);

protected:
    boost::asio::io_service& mIoService;
    boost::asio::ip::tcp::acceptor mAcceptor;
    uint32 mPort;
};
