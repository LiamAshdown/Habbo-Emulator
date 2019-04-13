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

#ifndef _NETWORK_STRING_BUFFER_h_
#define _NETWORK_STRING_BUFFER_h_
#include "Common/SharedDefines.h"
#endif /* _NETWORK_STRING_BUFFER_h_ */

#define STORAGE_INITIAL_SIZE 4096

namespace SteerStone
{
    /// Buffer which is used to hold data to be sent to the client
    class StringBuffer
    {
    public:
        /// Constructor
        /// @p_ReserveSize : Reserve size for our m_Storage
        explicit StringBuffer(std::size_t p_ReserveSize = STORAGE_INITIAL_SIZE) : m_WritePosition(0), m_ReadPosition(0)
        {
            m_Storage.reserve(STORAGE_INITIAL_SIZE);
        }
        /// Deconstructor
        ~StringBuffer() {}

    public:
        /// AppendWiredBool
        /// @p_Value : Append a true or false p_Value to our storage
        void AppendWiredBool(bool p_Value)
        {
            if (p_Value)
                AppendWired(1);
            else
                AppendWired(0);
        }
        /// AppendString
        /// @p_Value : Append a string to our storage
        /// @p_Delimeter : Append "/x2" to our storage if true
        void AppendString(std::string const p_Buffer, bool p_Delimeter = true)
        {
            if (std::size_t l_Length = p_Buffer.length())
                Append((uint8 const*)p_Buffer.c_str(), l_Length);

            if (p_Delimeter)
                AppendSOT();
        }

        /// AppendStringDelimiter
        /// @p_Buffer : Append a string to our storage
        /// @p_Delimeter : Append a delimeter to our storage
        void AppendStringDelimiter(const std::string p_Buffer, const std::string p_Delimeter)
        {
            if (std::size_t l_Length = p_Buffer.length())
                Append((uint8 const*)p_Buffer.c_str(), l_Length);
            AppendString(p_Delimeter, false);
        }

        /// AppendBase64
        /// @p_Value : Append p_Value in base64
        void AppendBase64(uint32 const p_Value)
        {
            std::string l_Encoder = EncodeBase64(p_Value);

            if (std::size_t l_Length = l_Encoder.length())
                Append((uint8 const*)l_Encoder.c_str(), l_Length);
        }

        /// AppendWired
        /// @p_Value : Append p_Value in wired
        void AppendWired(int64 const p_Value)
        {
            std::string l_Wired = EncodeWired(p_Value);

            if (std::size_t l_Length = l_Wired.length())
                Append((uint8 const*)l_Wired.c_str(), l_Length);
        }

        /// Append
        /// @p_Buffer : Append another StringBuffer into our storage
        void Append(StringBuffer const& p_Buffer)
        {
            if (p_Buffer.GetWritePosition())
                Append(p_Buffer.GetContents(), p_Buffer.GetWritePosition());
        }

        /// AppendSOH - Append Start of Header to end of our packet before it will be sent
        void AppendSOH()
        {
            std::string l_SOH = "\u0001";
            if (std::size_t l_Length = l_SOH.length())
                Append(l_SOH.c_str(), l_Length);
        }

        /// AppendSOH - Append Start of Text after each string is appended
        void AppendSOT()
        {
            std::string l_SOT = "\u0002";
            if (std::size_t l_Length = l_SOT.length())
                Append(l_SOT.c_str(), l_Length);
        }

        /// Append
        /// @param T the source type to convert.
        void Append(const char* p_Buffer, const std::size_t& p_Size)
        {
            return Append((uint8 const*)p_Buffer, p_Size);
        }

        /// Append
        /// @p_Buffer : Buffer which will be appended to into our storage
        /// @p_Size : Size of our p_Buffer
        void Append(uint8 const* p_Buffer, std::size_t const& p_Size)
        {
            if (!p_Size)
                return;

            assert(GetSize() < 10000000);

            if (m_Storage.size() < m_WritePosition + p_Size)
                m_Storage.resize(m_WritePosition + p_Size);

            memcpy(&m_Storage[m_WritePosition], p_Buffer, p_Size);
            m_WritePosition += p_Size;
        }

        /// Resize
        /// @p_NewSize : Resize our storage
        void Resize(std::size_t const p_NewSize)
        {
            m_Storage.reserve(p_NewSize);
        }

        /// Resize
        /// @p_NewSize : Reserve our storage
        void Reserve(std::size_t const p_NewSize)
        {
            if (p_NewSize > GetSize())
                m_Storage.reserve(p_NewSize);
        }

        /// GetWritePosition - Get our m_WritePosition;
        std::size_t GetWritePosition() const
        {
            return m_WritePosition;
        }

        /// GetReadPositino - Get our m_ReadPosition;
        std::size_t GetReadPositino()
        {
            return m_ReadPosition;
        }

        /// GetSize - Get size of our storage
        std::size_t GetSize() const
        {
            return m_Storage.size();
        }

        /// GetContents - Get the whole contents of our storage
        uint8 const* GetContents() const
        {
            return &m_Storage[0];
        }

        /// Clear - clear our storage and our m_ReadPosition and m_WritePosition
        void Clear()
        {
            m_WritePosition = 0;
            m_ReadPosition = 0;
            m_Storage.clear();
        }


        std::size_t m_WritePosition;                                                    ///< Write position in our stroage
        std::size_t m_ReadPosition;                                                     ///< Read position in our storage
        std::vector<uint8> m_Storage;                                                   ///< Vector Storage
    };
} ///< NAMESPACE STEERSTONE