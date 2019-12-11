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
#include "PCH/Precompiled.hpp"

#include "Core/Core.hpp"
#include "Threading/ThrTaskManager.hpp"
#include "NetworkThread.hpp"
#include "Socket.hpp"
#include "Logger/LogDefines.hpp"

namespace SteerStone { namespace Core { namespace Network {

    template<typename T> class Listener
    {
        DISALLOW_COPY_AND_ASSIGN(Listener);

        //////////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////////

        public:
            /// Constructor
            /// @p_Address       : IP Address
            /// @p_Port          : Port
            /// @p_WorkerThreads : Amount of services to spawn
            Listener(std::string const& p_Address, const uint16& p_Port, const uint8& p_WorkerThreads) 
                : m_Service(new boost::asio::io_service()),
                  m_Acceptor(new boost::asio::ip::tcp::acceptor(*m_Service, boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string(p_Address), p_Port)))
            {
                for (uint8 l_I = 0; l_I < p_WorkerThreads; l_I++)
                    m_NetworkThreads.push_back(std::unique_ptr<NetworkThread<T>>(new NetworkThread<T>(l_I)));

                std::function<bool()> l_Service = [this]() -> bool {
                    this->m_Service->run();
                    return true;
                };

                BeginAccept();

                m_AcceptorTask = sThreadManager->PushTask("LISTENER_THREAD", Threading::TaskType::Moderate, 0, l_Service);
            }
            /// Deconstructor
            ~Listener()
            {
                m_Acceptor->close();
                m_Service->stop();
                sThreadManager->PopTask(m_AcceptorTask);
                m_Acceptor.reset();
                m_Service.reset();
            }

            //////////////////////////////////////////////////////////////////////////
            //////////////////////////////////////////////////////////////////////////

        private:
            /// Select the worker with lowest storage size (equal distrubition)
            NetworkThread<T>* SelectWorker() const
            {
                std::size_t l_MinimumSize = m_NetworkThreads.size();
                std::size_t l_Index = 0;

                for (std::size_t l_I = 0; l_I < m_NetworkThreads.size(); l_I++)
                {
                    const std::size_t l_Size = m_NetworkThreads[l_I]->GetSize();

                    if (l_Size < l_MinimumSize)
                    {
                        l_MinimumSize = l_Size;
                        l_Index = l_I;
                    }
                }

                return m_NetworkThreads[l_Index].get();
            }
            /// Accept incoming connections
            void Listener<T>::BeginAccept()
            {
                auto l_Worker = SelectWorker();
                auto l_Socket = l_Worker->CreateSocket();

                m_Acceptor->async_accept(l_Socket->GetAsioSocket(),
                    [this, l_Worker, l_Socket](const boost::system::error_code& p_ErrorCode)
                    {
                        this->OnAccept(l_Worker, l_Socket, p_ErrorCode);
                    });
            }
            /// Accept new connection and create socket
            void Listener<T>::OnAccept(NetworkThread<T>* p_Worker, std::shared_ptr<T> const& p_Socket, const boost::system::error_code& p_ErrorCode)
            {
                if (p_ErrorCode)
                    p_Worker->RemoveSocket(p_Socket.get());
                else
                    p_Socket->Open();

                /// Return back and accept any more incoming connections
                BeginAccept();
            }

        private:
            std::unique_ptr<boost::asio::io_service> m_Service;                     ///< IO Service
            std::vector<std::unique_ptr<NetworkThread<T>>> m_NetworkThreads;        ///< Worker threads
            std::unique_ptr<boost::asio::ip::tcp::acceptor> m_Acceptor;             ///< IO Acceptor
            Threading::Task::Ptr m_AcceptorTask;                                    ///< Acceptor Task
    };

}   ///< namespace Network
}   ///< namespace Core
}   ///< namespace Steerstone