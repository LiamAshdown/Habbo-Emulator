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

#ifndef _NETWORK_SOCKET_h
#define _NETWORK_SOCKET_h
#include "Common/SharedDefines.h"
#include "PacketBuffer.h"
#include <boost/asio.hpp>
#include <windows.h>
#include <mutex>
#endif /* _NETWORK_SOCKET_h */

namespace SteerStone
{
    /// Write States
    enum class WriteState
    {
        Idle,                       ///< NO WRITING IN PROGRESS
        Buffering,                  ///< A WRITE OPERATION HAS BEEN PERFORMED, AND WE ARE CURRENTLY AWAITING OTHERS BEFORE SENDING
        Sending,                    ///< A SEND OPERATION IS CURRENTLY UNDERWAY
    };

    /// Read States
    enum class ReadState
    {
        Idle,                       ///< CURRENTLY NOT READING
        Reading                     ///< CURRENTLY READING
    };

    /// Handling incoming/sending data
    class Socket : public std::enable_shared_from_this<Socket>
    {
    public:
        /// Constructor
        /// @p_Service : Socket to pass
        /// @p_CloseHandler : Custom Handler to handle our function
        Socket(boost::asio::io_service &p_Service, std::function<void(Socket *)> p_CloseHandler);
        virtual ~Socket() = default;

    public:
        /// Open - Open our socket to listen for incoming packets
        /// @p_Port : Pass our port which is defined by Listener class
        virtual bool Open(uint16 const& p_Port);

        /// CloseSocket - Close our socket to stop recieving incoming packets
        void CloseSocket();

        /// IsClosed - Check if our socket is open or closed
        bool IsClosed() const;

        /// Deletable - Check if our socket can be deleted
        virtual bool Deletable() const;

        /// Read - Read the packet
        /// @p_Buffer : Buffer which holds the data
        /// @p_Length : The length of the data
        bool Read(char* p_Buffer, std::size_t const& p_Length);

        /// ReadSkip - Skip parts of the packet
        /// @p_Length : The length of the data to skip
        void ReadSkip(std::size_t const&  p_Length);

        /// Write - Write the data to be sent
        /// @p_Buffer : Buffer which holds the data
        /// @p_Length : The length of the data
        void Write(const char* p_Buffer, std::size_t const&  p_Length);

        /// ReadLength - Get the total read length of the packet
        std::size_t const ReadLength();

        /// ReadLengthRemaining - Get the length remaining to read
        std::size_t const ReadLengthRemaining();

        /// GetPort - Get the port we are listening on
        uint16 const GetPort();

        /// GetAsioSocket - Get our AsioSocket
        boost::asio::ip::tcp::socket& GetAsioSocket();

        /// GetAsioSocket - Get our EndPoint
        std::string const& GetRemoteEndpoint();

        /// GetAsioSocket - Get our Remote Address
        std::string const& GetRemoteAddress();

        /// MyMethods described here.
        /// @param T Class which derives from Socket class
        template <typename T>
        std::shared_ptr<T> Shared();

    protected:
        /// ProcessIncomingData - Virtual Function which passes into our derived class Socket
        virtual bool ProcessIncomingData() = 0;

        /// ProcessIncomingData - Get the current read position
         uint8 const* InPeak();

        /// ForceFlushOut - Send our current data in our buffer
        void ForceFlushOut();

    private:
        /// InitializeClientServer - Initialize client/server response
        void InitializeClientServer();

        /// StartAsyncRead - Read incoming packets
        void StartAsyncRead();

        /// OnRead - Handle the incoming packet
        /// @p_Error : Error code
        /// @p_Length : Length of failed buffer
        void OnRead(boost::system::error_code const& p_ErrorCode, std::size_t const& p_Length);

        /// OnWriteComplete - Finished sending out our buffer
        /// @p_Error : Error code
        /// @p_Length : Length of failed buffer
        void OnWriteComplete(boost::system::error_code const& p_ErrorCode, std::size_t const& p_Length);

        /// FlushOut - Begin to send out our data in our buffer
        void FlushOut();

        /// StartWriteFlushTimer - Start the time to send out our data in interval
        void StartWriteFlushTimer();

        /// OnError - Catch an error if packet is corrupted
        /// @p_Error : Error code
        void OnError(boost::system::error_code const& p_ErrorCode);

    private:
        /// Socket
        boost::asio::ip::tcp::socket m_Socket;                                    ///< Socket
        std::function<void(Socket*)> m_CloseHandler;                              ///< Socket Handler         
        std::string const m_Address;                                              ///< Address of our Listener                           
        std::string const m_RemoteEndPoint;                                       ///< End point of our Listener
        uint16 m_Port;                                                            ///< Port Listener is listening on
        /// Buffer
        std::unique_ptr<PacketBuffer> m_InBuffer;                                 ///< In Buffer - recieving incoming packets
        std::unique_ptr<PacketBuffer> m_OutBuffer;                                ///< Out Buffer - sending our packets
        boost::asio::deadline_timer m_OutBufferFlushTimer;                        ///< Time to send out packets
        static int32 const m_BufferTimeout = 50;                                  ///< Interval of our flush out timer
        /// Mutex
        std::mutex m_Mutex;                                                       ///< Mutex Guard
        std::mutex m_ClosedMutex;                                                 ///< Mutex Guard
        /// Write State
        WriteState m_WriteState;                                                  ///< State of where are at; idle, reading
        ReadState m_ReadState;                                                    ///< State of where are at; idle, reading, buffering

#pragma region
        private:
            // custom allocator based on example from http://www.boost.org/doc/libs/1_62_0/doc/html/boost_asio/example/cpp11/allocation/server.cpp

            // Class to manage the memory to be used for handler-based custom allocation.
            // It contains a single block of memory which may be returned for allocation
            // requests. If the memory is in use when an allocation request is made, the
            // allocator delegates allocation to the global heap.
            class handler_allocator
            {
                private:
                    static const size_t BufferSize = 512;

                    // Storage space used for handler-based custom memory allocation.
                    std::aligned_storage<BufferSize>::type mBuffer;

                    // Whether the handler-based custom allocation storage has been used.
                    bool m_inUse;

                public:
                    handler_allocator() : m_inUse(false) {}

                    handler_allocator(const handler_allocator&) = delete;
                    handler_allocator& operator=(const handler_allocator&) = delete;

                    void* allocate(std::size_t size)
                    {
                        if (!m_inUse && size <= sizeof(mBuffer))
                        {
                            m_inUse = true;
                            return &mBuffer;
                        }

                        return ::operator new(size);
                    }

                    void deallocate(void* pointer)
                    {
                        if (pointer == &mBuffer)
                            m_inUse = false;
                        else
                            ::operator delete(pointer);
                    }
            };

            // Wrapper class template for handler objects to allow handler memory
            // allocation to be customised. Calls to operator() are forwarded to the
            // encapsulated handler.
            template <typename Handler>
            class custom_alloc_handler
            {
                private:
                    handler_allocator& m_allocator;
                    Handler m_handler;

                    public:
                        custom_alloc_handler(handler_allocator& a, Handler h) : m_allocator(a), m_handler(h) {}

                        template <typename ...Args>
                        void operator()(Args&&... args)
                        {
                            m_handler(std::forward<Args>(args)...);
                        }

                        friend void* asio_handler_allocate(std::size_t size, custom_alloc_handler<Handler>* this_handler)
                        {
                            return this_handler->m_allocator.allocate(size);
                        }

                        friend void asio_handler_deallocate(void* pointer, std::size_t /*size*/, custom_alloc_handler<Handler>* this_handler)
                        {
                            this_handler->m_allocator.deallocate(pointer);
                        }
                };

                // Helper function to wrap a handler object to add custom allocation.
                template <typename Handler>
            static custom_alloc_handler<Handler> make_custom_alloc_handler(handler_allocator& a, Handler h)
            {
                return custom_alloc_handler<Handler>(a, h);
            }

            handler_allocator m_allocator;
    };

    template<typename T>
    inline std::shared_ptr<T> Socket::Shared()
    {
        return std::static_pointer_cast<T>(shared_from_this());
    }
#pragma endregion CustomHandler; TODO
} ///< NAMESPACE STEERSTONE