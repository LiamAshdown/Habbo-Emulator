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
#include "Network/Listener.hpp"
#include "Diagnostic/DiaStopWatch.hpp"

namespace SteerStone { namespace Game { namespace Server {

    enum class Authenticated
    {
        Authenticed,            ///< Authenticated from server
        NotAuthenticated,       ///< Is not authenticated yet from server
    };

    class GameSocket : public Core::Network::Socket
    {
        DISALLOW_COPY_AND_ASSIGN(GameSocket);

        //////////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////////

        public:
            /// Constructor 
            /// @p_Service : Boost Service
            /// @p_CloseHandler : Close Handler Custom function
            GameSocket(boost::asio::io_service& p_Service, std::function<void(Socket*)> p_CloseHandler);
            /// Deconstructor
            ~GameSocket() {}

        //////////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////////


        //////////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////////

        private:
            /// Handle incoming data
            virtual Core::Network::ProcessState ProcessIncomingData() override;

        private:
            Authenticated m_AuthenticateState;
    };

}   ///< namespace Server
}   ///< namespace Game
}   ///< namespace Steerstone