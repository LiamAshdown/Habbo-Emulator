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

#ifndef _NETWORK_CLIENT_PACKET_h
#define _NETWORK_CLIENT_PACKET_h
#include "Common/SharedDefines.h"
#endif /* _NETWORK_CLIENT_PACKET_h */

namespace SteerStone
{
    /// ClientPacket class which handles in decoding incoming packets
    class ClientPacket
    {
    public:
        /// Constructor
        /// @p_Contents : Contents of packet we have just recieved
        ClientPacket(std::string const p_Contents) : m_ReadPosition(0), m_Header(0), m_Content("") 
        {
            /// Seperate the buffer into Header and content
            m_Header = DecodeBase64(p_Contents.substr(0, 2));
            m_Content = p_Contents.substr(2);
        }

        ~ClientPacket() {}

    public:
       /// ReadString
        /// Read the string
        std::string ReadString()
        {
            std::size_t l_Length = DecodeBase64(m_Content.substr(m_ReadPosition, 2));
            std::string l_Temp = m_Content.substr(2 + m_ReadPosition, l_Length);
            m_ReadPosition += (l_Length + 2);

            return l_Temp;
        }

        /// ReadBase64Int
        /// Read a base64 interger
        int32 ReadBase64Int()
        {
            int32 l_I = DecodeBase64(m_Content.substr(m_ReadPosition, 2));
            m_ReadPosition += 2;

            return l_I;
        }

        /// ReadWiredInt
        /// Read a wired interger
        int32 ReadWiredInt()
        {
            int32 l_I = DecodeWired(m_Content.substr(m_ReadPosition));
            m_ReadPosition += EncodeWired(l_I).length();

            return l_I;
        }


        /// ReadBase64Uint
        /// Read a wired unsigned interger
        uint32 ReadBase64Uint()
        {
            uint32 i = DecodeBase64(m_Content.substr(m_ReadPosition, 2));
            m_ReadPosition += 2;

            return i;
        }

        /// ReadWiredInt
        /// Read a wired unsigned interger
        uint32 ReadWiredUint()
        {
            uint32 i = DecodeWired(m_Content.substr(m_ReadPosition));
            m_ReadPosition += EncodeWired(i).length();

            return i;
        }

        /// ReadWiredBool
        /// Read bool
        bool ReadWiredBool()
        {
            int64 i = DecodeWired(m_Content.substr(m_ReadPosition));
            m_ReadPosition += EncodeWired(i).length();

            if (i == 1)
                return true;
            else
                return false;
        }

        /// ReadBase64Bool
        /// Read bool in base64
        bool ReadBase64Bool()
        {
            int64 i = DecodeBase64(m_Content.substr(m_ReadPosition++, 1));

            if (i == 1)
                return true;
            else
                return false;
        }


        /// ReadSkip
        /// @p_Value : Amount of bytes to skip reading
        void ReadSkip(uint8 const p_Value)
        {
            for (uint8 i = 0; i < p_Value; i++)
            {
                DecodeBase64(m_Content.substr(m_ReadPosition, 2));
                m_ReadPosition += 2;
            }
        }

        /// GetReadLength
        /// Get read length of storage
        std::size_t GetReadLength() const
        {
            return m_ReadPosition;
        }

        /// GetHeader
        /// Get the header of the packet
        uint32 GetHeader() const
        {
            return m_Header;
        }

        /// GetContent
        /// Returns body content
        std::string GetContent()
        {
            return m_Content;
        }

    private:
        std::size_t m_ReadPosition;                                                    ///< Read position in our storage
        std::string m_Content;                                                         ///< Body content of packet
        uint32 m_Header;                                                               ///< Header ID of packet
    };
} ///< NAMESPACE STEERSTONE