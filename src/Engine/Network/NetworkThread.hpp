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
#include <PCH/Precompiled.hpp>
#include <boost/asio.hpp>

#include "Core/Core.hpp"
#include "Utility/UtiString.hpp"
#include "Utility/UtiObjectGuard.hpp"
#include "Utility/UtiLockable.hpp"
#include "Logger/LogDefines.hpp"
#include "Socket.hpp"

namespace SteerStone { namespace Core { namespace Network {

    template<typename T> class NetworkThread : private Utils::LockableReadWrite
    {
        DISALLOW_COPY_AND_ASSIGN(NetworkThread);

        //////////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////////

        /// Allow access to lock / unlock methods
        friend class Utils::ObjectGuard<NetworkThread>;
        friend class Utils::ObjectReadGuard<NetworkThread>;
        friend class Utils::ObjectWriteGuard<NetworkThread>;

        public:
            /// Constructor
            /// @p_WorkerThread : Worker thread number spawned
            NetworkThread(uint8 const& p_WorkerThread) 
                : m_Worker(new boost::asio::io_service::work(m_Service))
            {
                std::function<bool()> l_Service = [this]() -> bool {
                    this->m_Service.run();
                    return true;
                };

                 l_Task = sThreadManager->PushTask(Utils::StringBuilder("NETWORK_SERVER_WORKER_THREAD_%0", p_WorkerThread), Threading::TaskType::Moderate, -1, l_Service);
            }
            /// Deconstructor
            ~NetworkThread()
            {
                /// Allow IO Service to exit
                m_Worker.reset();
                m_Service.stop();

                /// Close all active sockets
                for (auto l_Itr = m_Sockets.begin(); l_Itr != m_Sockets.end(); l_Itr++)
                {
                    if (!(*l_Itr)->IsClosed())
                        (*l_Itr)->CloseSocket();
                }

                sThreadManager->PopTask(l_Task);
            }

            //////////////////////////////////////////////////////////////////////////
            //////////////////////////////////////////////////////////////////////////

            /// Get size of Socket storage
            std::size_t GetSize() const
            {
                return m_Sockets.size();
            }

            /// Create socket
            std::shared_ptr<T> NetworkThread<T>::CreateSocket()
            {
                Utils::ObjectGuard l_Guard(this);

                m_Sockets.emplace(std::make_shared<T>(m_Service, [this](Socket* p_Socket) { this->RemoveSocket(p_Socket); }));
                
                return *m_Sockets.begin();
            }
            /// Remove socket from storage
            /// @p_Socket : Socket being removed
            void RemoveSocket(Socket* p_Socket)
            {
                Utils::ObjectGuard l_Guard(this);

                m_Sockets.erase(p_Socket->Shared<T>());
            }

        private:
            boost::asio::io_service m_Service;                          ///< IO Service
            std::unique_ptr<boost::asio::io_service::work> m_Worker;    ///< Worker of IO Service
            std::unordered_set<std::shared_ptr<T>> m_Sockets;           ///< Storage of socket classes
            Threading::Task::Ptr l_Task;                                ///< Worker task
    };

}   ///< namespace Network
}   ///< namespace Core
}   ///< namespace Steerstone