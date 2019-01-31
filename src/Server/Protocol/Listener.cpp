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
#include <thread>
#include "Listener.h"
//-----------------------------------------------//
void Listener::AcceptConnection()
{
    std::shared_ptr<Socket> socket = std::make_shared<Socket>(mIoService, mPort);
    mAcceptor.async_accept(socket->GetSocket(),
        boost::bind(&Listener::HandleAcceptConnection, this, socket,
            boost::asio::placeholders::error));
}
//-----------------------------------------------//
void Listener::HandleAcceptConnection(std::shared_ptr<Socket> newSocket, const boost::system::error_code & errorCode)
{
    if (!errorCode)
    {
        newSocket->SendAuthResponse();
        newSocket->ReadHandler();
    }

    AcceptConnection();
}
//-----------------------------------------------//---------------------------------------------//