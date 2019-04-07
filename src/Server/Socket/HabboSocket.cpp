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

#include "HabboSocket.h"
#include "Database/QueryDatabase.h"
#include "Common/SHA1.h"
#include "Habbo.h"
#include "Opcodes.h"

namespace SteerStone
{
    /// Constructor 
    /// @p_Service : Boost Service
    /// @p_CloseHandler : Close Handler Custom function
    HabboSocket::HabboSocket(boost::asio::io_service& p_Service, std::function<void(Socket*)> p_CloseHandler) :
        Socket(p_Service, std::move(p_CloseHandler)), m_Habbo(nullptr)
    {
    }
   
    /// ToHabbo - Return our Habbo Class
    Habbo* HabboSocket::ToHabbo()
    {
        return m_Habbo;
    }
    
    /// DestroyHabbo - Set our Habbo class to nullptr
    void HabboSocket::DestroyHabbo()
    {
        m_Habbo = nullptr;
    }
    
    /// SendPacket 
    /// @p_Buffer : Buffer which holds our data to be send to the client
    void HabboSocket::SendPacket(StringBuffer const* p_Buffer)
    {
        Write((const char*)p_Buffer->GetContents(), p_Buffer->GetSize());
    }
    
    /// ProcessIncomingData - Handle incoming data
    bool HabboSocket::ProcessIncomingData()
    {
        std::vector<uint8> l_BufferVec;
        l_BufferVec.resize(ReadLengthRemaining());
        if (Read((char*)&l_BufferVec[0], ReadLengthRemaining()))
        {
            l_BufferVec.resize(l_BufferVec.size() + 1);
            l_BufferVec[l_BufferVec.size() - 1] = 0;

            std::string l_TempBuffer = (char*)&l_BufferVec[0];

            while (l_TempBuffer.length() > 0)
            {
                std::string l_TrimBuffer = l_TempBuffer.substr(3, DecodeBase64(l_TempBuffer.substr(1, 2)));

                std::unique_ptr<ClientPacket> l_Packet = std::make_unique<ClientPacket>(l_TrimBuffer);

                LOG_INFO << "[INCOMING]: " << "[" << l_Packet->GetHeader() << "] [" << sOpcode->GetClientPacket(l_Packet->GetHeader()).name << "]";

                ExecutePacket(sOpcode->GetClientPacket(l_Packet->GetHeader()), std::move(l_Packet));

                l_TempBuffer = l_TempBuffer.substr(l_TrimBuffer.length() + 3);
            }

            return true;
        }
        else
            return false;
    }
    
    /// Client Handlers
    void HabboSocket::ExecutePacket(OpcodeHandler const& p_Handler, std::unique_ptr<ClientPacket> p_Packet)
    {
        (this->*p_Handler.handler)(std::move(p_Packet));
    }
    
    /// ClientPacket is not handled yet
    void HabboSocket::HandleNULL(std::unique_ptr<ClientPacket> p_Packet)
    {

    }
}
