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

#ifndef _QuadEmu_WorldSession_h_
#define _QuadEmu_WorldSession_h_
#include "../Common/SharedDefines.h"
#endif /* _QuadEmu_WorldSession_h_ */

class Player;
class WorldPacket;
class Socket;
struct PacketHandlerStruct;

class WorldSession
{
public:
    WorldSession(std::shared_ptr<Socket> socket);
    ~WorldSession();

    // Packet Wrappers
    void SendPacket(const WorldPacket packet);
    void ExecutePacket(PacketHandlerStruct const& opHandle, std::string& packet, std::vector<std::string> packetStorage);
    // Socket Wrappers
    void CloseSocket();
    std::shared_ptr<Socket> GetSocket();

    // Misc
    Player* GetPlayer();
    void SendSystemBroadCast(const std::string& broadcast);
    uint32 GetAccountId();
    void LogoutPlayer(bool save);
    bool InitializePlayerData(const std::string& username);
    bool IsPlayerInitialized();
    void SendLoginFailure();

 ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    void HandleVersionCheck(std::string& packetBuffer, std::vector<std::string>& packetStorage);
    void HandleKeyEncrypted(std::string& packetBuffer, std::vector<std::string>& packetStorage);
    void HandleClientIp(std::string& packetBuffer, std::vector<std::string>& packetStorage);
    void HandleApproveName(std::string& packetBuffer, std::vector<std::string>& packetStorage);
    void HandleRegister(std::string& packetBuffer, std::vector<std::string>& packetStorage);
    void HandleLogin(std::string& packetBuffer, std::vector<std::string>& packetStorage);
    void HandleGetCredits(std::string& packetBuffer, std::vector<std::string>& packetStorage);
    void HandleStatusOk(std::string& packetBuffer, std::vector<std::string>& packetStorage);
    void HandleInfoRetrieve(std::string& packetBuffer, std::vector<std::string>& packetStorage);
    void HandleSearchBusyFlats(std::string& packetBuffer, std::vector<std::string>& packetStorage);
    void HandleInitUnitListener(std::string& packetBuffer, std::vector<std::string>& packetStorage);
    void HandleMove(std::string& packetBuffer, std::vector<std::string>& packetStorage);
    void HandleChat(std::string& packetBuffer, std::vector<std::string>& packetStorage);
    void HandleShout(std::string& packetBuffer, std::vector<std::string>& packetStorage);
    void HandleWhisper(std::string& packetBuffer, std::vector<std::string>& packetStorage);
    void HandleCreateFlat(std::string& packetBuffer, std::vector<std::string>& packetStorage);
    void HandleSetFlatInfo(std::string& packetBuffer, std::vector<std::string>& packetStorage);
    void HandleTryFlat(std::string& packetBuffer, std::vector<std::string>& packetStorage);
    void HandleGoToFlat(std::string& packetBuffer, std::vector<std::string>& packetStorage);
    void HandleGetFlatInfo(std::string& packetBuffer, std::vector<std::string>& packetStorage);
    void HandleUpdateFlat(std::string& packetBuffer, std::vector<std::string>& packetStorage);
    void HandleDeleteFlat(std::string& packetBuffer, std::vector<std::string>& packetStorage);
    void HandleGoAway(std::string& packetBuffer, std::vector<std::string>& packetStorage);
    void HandleDance(std::string& packetBuffer, std::vector<std::string>& packetStorage);
    void HandleStop(std::string& packetBuffer, std::vector<std::string>& packetStorage);
    void HandleLookTo(std::string& packetBuffer, std::vector<std::string>& packetStorage);
    void HandleNULL(std::string& packetBuffer, std::vector<std::string>& packetStorage) {}

protected:
    std::shared_ptr<Socket> mSocket;
    Player* mPlayer;
    bool mPlayerInitialized;
    uint32 mId;
};

