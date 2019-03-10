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

#ifndef _Quad_Socket_h_
#define _Quad_Socket_h_
#include <boost/asio.hpp>
#include <windows.h>
#include <mutex>

#include "../Common/SharedDefines.h"
#include "PacketBuffer.h"

namespace Quad
{
    enum class WriteState
    {
        Idle,                       ///< NO WRITING IN PROGRESS
        Buffering,                  ///< A WRITE OPERATION HAS BEEN PERFORMED, AND WE ARE CURRENTLY AWAITING OTHERS BEFORE SENDING
        Sending,                    ///< A SEND OPERATION IS CURRENTLY UNDERWAY
    };

    enum class ReadState
    {
        Idle,                       ///< CURRENTLY NOT READING
        Reading                     ///< CURRENTLY READING
    };

    class Socket : public std::enable_shared_from_this<Socket>
    {
    public:
        Socket(boost::asio::io_service &service, std::function<void(Socket *)> closeHandler);
        virtual ~Socket() = default;

    public:
        virtual bool Open(uint16 port);
        void CloseSocket();

        bool IsClosed() const;
        virtual bool Deletable() const;

        bool Read(char* buffer, const std::size_t& length);
        void ReadSkip(const std::size_t& length);

        void Write(const char *buffer, const std::size_t& length);

        std::size_t ReadLength() const;
        std::size_t ReadLengthRemaining() const;
        uint16 GetPort() const;

        boost::asio::ip::tcp::socket &GetAsioSocket();

        const std::string &GetRemoteEndpoint();
        const std::string &GetRemoteAddress();

        template <typename T>
        std::shared_ptr<T> Shared();

    private:
        void InitializeClientServer();

        void StartAsyncRead();
        void OnRead(const boost::system::error_code &error, const std::size_t& length);

        void OnWriteComplete(const boost::system::error_code &error, const std::size_t& length);
        void FlushOut();

        void StartWriteFlushTimer();

        void OnError(const boost::system::error_code &error);

    private:
        boost::asio::ip::tcp::socket mSocket;
        std::function<void(Socket*)> mCloseHandler;
        std::unique_ptr<PacketBuffer> mInBuffer;
        std::unique_ptr<PacketBuffer> mOutBuffer;
        boost::asio::deadline_timer mOutBufferFlushTimer;
        std::mutex mMutex;
        std::mutex mClosedMutex;

        WriteState mWriteState;
        ReadState mReadState;

        const std::string m_address;
        const std::string m_remoteEndpoint;

        static const int32 BufferTimeout = 50;

        uint16 mPort;

    protected:
        virtual bool ProcessIncomingData() = 0;

        const uint8* InPeak() const;
        void ForceFlushOut();

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
}

#endif /* _Quad_Socket_h_ */