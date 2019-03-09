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

#ifndef _Quad_NetWorkThread_h_
#define _Quad_NetWorkThread_h_
#include "../Common/SharedDefines.h"
#include "Socket.h"

namespace Quad
{
    template <typename SocketType>
    class NetworkThread
    {
    public:
        NetworkThread() : mWork(new boost::asio::io_service::work(mService)), mServiceThread([this] { boost::system::error_code ec; this->mService.run(ec); })
        {
            mServiceThread.detach();
        }

        ~NetworkThread()
        {
            // Allow io_service::run() to exit.
            mWork.reset();

            IF_LOG(plog::debug)
                LOG_DEBUG << "Destructor NetworkThread called!";

            // Attempt to gracefully close any open connections
            for (auto i = mSockets.begin(); i != mSockets.end();)
            {
                auto const current = i;
                ++i;

                if (!(*current)->IsClosed())
                    (*current)->CloseSocket();
            }
        }

    public:
        std::size_t Size() const { return mSockets.size(); }

        std::shared_ptr<SocketType> CreateSocket();

        void RemoveSocket(Socket *socket)
        {
            std::lock_guard<std::mutex> guard(mMutex);
            mSockets.erase(socket->Shared<SocketType>());
        }

    private:
        boost::asio::io_service mService;

        std::mutex mMutex;
        std::unordered_set<std::shared_ptr<SocketType>> mSockets;

        // Note that the work member *must* be declared after the service member for the work constructor to function correctly
        std::unique_ptr<boost::asio::io_service::work> mWork;

        std::thread mServiceThread;
    };

    template <typename SocketType>
    std::shared_ptr<SocketType> NetworkThread<SocketType>::CreateSocket()
    {
        std::lock_guard<std::mutex> guard(mMutex);

        auto const i = mSockets.emplace(std::make_shared<SocketType>(mService, [this] (Socket *socket) { this->RemoveSocket(socket); }));

        assert(i.second);

        return *i.first;
    }
}

#endif /* !_Quad_NetWorkThread_h_ */
