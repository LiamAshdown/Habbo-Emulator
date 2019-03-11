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
//-----------------------------------------------//
#include "Socket.h"
#include "StringBuffer.h"
//-----------------------------------------------//
namespace Quad
{
    //-----------------------------------------------//
    Socket::Socket(boost::asio::io_service& service, std::function<void(Socket*)> closeHandler)
        : mWriteState(WriteState::Idle), mReadState(ReadState::Idle), mSocket(service),
        mCloseHandler(std::move(closeHandler)), mOutBufferFlushTimer(service), m_address("0.0.0.0") {}
    //-----------------------------------------------//
    bool Socket::Open(uint16 port)
    {
        try
        {
            mPort = port;
            const_cast<std::string&>(m_address) = mSocket.remote_endpoint().address().to_string();
            const_cast<std::string&>(m_remoteEndpoint) = boost::lexical_cast<std::string>(mSocket.remote_endpoint());
        }
        catch (boost::system::error_code& error)
        {
            return false;
        }

        mOutBuffer.reset(new PacketBuffer);
        mInBuffer.reset(new PacketBuffer);

        InitializeClientServer();

        StartAsyncRead();

        return true;
    }
    //-----------------------------------------------//
    void Socket::CloseSocket()
    {
        std::lock_guard<std::mutex> guard(mClosedMutex);

        IF_LOG(plog::debug)
            LOG_DEBUG << "Closing Socket!";

        if (IsClosed())
            return;

        boost::system::error_code ec;
        mSocket.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
        mSocket.close();

        if (mCloseHandler)
            mCloseHandler(this);
    }
    //-----------------------------------------------//
    bool Socket::IsClosed() const
    {
        return !mSocket.is_open();
    }
    //-----------------------------------------------//
    bool Socket::Deletable() const
    {
        return IsClosed();
    }
    //-----------------------------------------------//
    void Socket::InitializeClientServer()
    {
        TempBuffer buffer;
        buffer.AppendBase64(0);
        buffer.AppendSOH();

        Write((const char*)buffer.GetContents(), buffer.GetSize());
    }
    //-----------------------------------------------//
    void Socket::StartAsyncRead()
    {
        if (IsClosed())
        {
            mReadState = ReadState::Idle;
            return;
        }

        std::shared_ptr<Socket> ptr = Shared<Socket>();
        mReadState = ReadState::Reading;
        mSocket.async_read_some(boost::asio::buffer(&mInBuffer->mBuffer[mInBuffer->mWritePosition], mInBuffer->mBuffer.size() - mInBuffer->mWritePosition),
            make_custom_alloc_handler(m_allocator,
                [ptr](const boost::system::error_code & error, size_t length) { ptr->OnRead(error, length); }));
    }
    //-----------------------------------------------//
    void Socket::OnRead(const boost::system::error_code& error, const std::size_t& length)
    {
        if (error)
        {
            mReadState = ReadState::Idle;
            OnError(error);
            return;
        }

        if (IsClosed())
        {
            mReadState = ReadState::Idle;
            return;
        }

        mInBuffer->mWritePosition += length;

        const size_t available = mSocket.available();

        // if there is still data to read, increase the buffer size and do so (if necessary)
        if (available > 0 && (length + available) > mInBuffer->mBuffer.size())
        {
            mInBuffer->mBuffer.resize(mInBuffer->mBuffer.size() + available);
            StartAsyncRead();
            return;
        }

        // we must repeat this in case we have read in multiple messages from the client
        while (mInBuffer->mReadPosition < mInBuffer->mWritePosition)
        {
            if (!ProcessIncomingData())
            {
                // this errno is set when there is not enough buffer data available to either complete a header, or the packet length
                // specified in the header goes past what we've read.  in this case, we will reset the buffer with the remaining data
                if (errno == EBADMSG)
                {
                    const std::size_t bytesRemaining = mInBuffer->mWritePosition - mInBuffer->mReadPosition;

                    ::memmove(&mInBuffer->mBuffer[0], &mInBuffer->mBuffer[mInBuffer->mReadPosition], bytesRemaining);

                    mInBuffer->mReadPosition = 0;
                    mInBuffer->mWritePosition = bytesRemaining;

                    StartAsyncRead();
                }
                else 
                    if (!IsClosed())
                        CloseSocket();

                return;
            }
        }

        // Reset to read next packet
        mInBuffer->mWritePosition = 0;
        mInBuffer->mReadPosition = 0;

        StartAsyncRead();
    }
    //-----------------------------------------------//
    void Socket::OnError(const boost::system::error_code& error)
    {
        if (!IsClosed())
            CloseSocket();
    }
    //-----------------------------------------------//
    bool Socket::Read(char* buffer, const std::size_t& length)
    {
        if (ReadLengthRemaining() < length)
            return false;

        mInBuffer->Read(buffer, length);

        return true;
    }
    //-----------------------------------------------//
    void Socket::ReadSkip(const std::size_t& length)
    {
        mInBuffer->Read(nullptr, length);
    }
    //-----------------------------------------------//
    boost::asio::ip::tcp::socket& Socket::GetAsioSocket()
    {
        return mSocket;
    }
    //-----------------------------------------------//
    const std::string & Socket::GetRemoteEndpoint()
    {
        return m_remoteEndpoint;
    }
    //-----------------------------------------------//
    const std::string & Socket::GetRemoteAddress()
    {
        return m_address;
    }
    //-----------------------------------------------//
    void Socket::Write(const char* buffer, const std::size_t& length)
    {
        std::lock_guard<std::mutex> guard(mMutex);

        // Write the header
        mOutBuffer->Write(buffer, length);

        // Flush data if need
        if (mWriteState == WriteState::Idle)
            StartWriteFlushTimer();
    }
    //-----------------------------------------------//
    void Socket::FlushOut()
    {
        // If the socket is closed, silently fail
        if (IsClosed())
        {
            mWriteState = WriteState::Idle;
            return;
        }

        std::lock_guard<std::mutex> guard(mMutex);

        assert(mWriteState == WriteState::Buffering);

        // at this point we are guarunteed that there is data to send in the primary buffer.  send it.
        mWriteState = WriteState::Sending;

        std::shared_ptr<Socket> ptr = Shared<Socket>();
        mSocket.async_write_some(boost::asio::buffer(mOutBuffer->mBuffer, mOutBuffer->mWritePosition),
            make_custom_alloc_handler(m_allocator,
                [ptr](const boost::system::error_code& error, const std::size_t& length) { ptr->OnWriteComplete(error, length); }));
    }
    //-----------------------------------------------//
    void Socket::StartWriteFlushTimer()
    {
        if (mWriteState == WriteState::Buffering)
            return;

        // if the socket is closed, silently fail
        if (IsClosed())
        {
            mWriteState = WriteState::Idle;
            return;
        }

        mWriteState = WriteState::Buffering;

        std::shared_ptr<Socket> ptr = Shared<Socket>();
        mOutBufferFlushTimer.expires_from_now(boost::posix_time::milliseconds(int32(BufferTimeout)));
        mOutBufferFlushTimer.async_wait([ptr](const boost::system::error_code & error) { ptr->FlushOut(); });
    }
    //-----------------------------------------------//
    const uint8 * Socket::InPeak() const
    {
        return &mInBuffer->mBuffer[mInBuffer->mReadPosition];
    }
    //-----------------------------------------------//
    std::size_t Socket::ReadLength() const
    {
        return mInBuffer->ReadLength();
    }
    //-----------------------------------------------//
    std::size_t Socket::ReadLengthRemaining() const
    {
        return mInBuffer->ReadLengthRemaining();
    }
    //-----------------------------------------------//
    uint16 Socket::GetPort() const
    {
        return mPort;
    }
    //-----------------------------------------------//
    // If the write state is idle, this will do nothing, which is correct
    // If the write state is sending, this will do nothing, which is correct
    // If the write state is buffering, this will cancel the running timer, which will immediately trigger FlushOut()
    void Socket::ForceFlushOut()
    {
        mOutBufferFlushTimer.cancel();
    }
    //-----------------------------------------------//
    void Socket::OnWriteComplete(const boost::system::error_code& error, const std::size_t& length)
    {
        // we must check this before locking the mutex because the connection will be closed,
        // which leads to a locked mutex being destroyed.  not good!
        if (error)
        {
            OnError(error);
            return;
        }

        if (IsClosed())
        {
            mWriteState = WriteState::Idle;
            return;
        }

        std::lock_guard<std::mutex> guard(mMutex);

        assert(mWriteState == WriteState::Sending);
        assert(length <= mOutBuffer->mWritePosition);

        // if there is data left to write, move it to the start of the buffer
        if (length < mOutBuffer->mWritePosition)
        {
            memcpy(&(mOutBuffer->mBuffer[0]), &(mOutBuffer->mBuffer[length]), (mOutBuffer->mWritePosition - length) * sizeof(mOutBuffer->mBuffer[0]));
            mOutBuffer->mWritePosition -= length;
        }
        // if not, reset the write pointer
        else
            mOutBuffer->mWritePosition = 0;

        std::shared_ptr<Socket> ptr = Shared<Socket>();
        // if there is any data to write, do so immediately
        if (mOutBuffer->mWritePosition > 0)
            mSocket.async_write_some(boost::asio::buffer(mOutBuffer->mBuffer, mOutBuffer->mWritePosition),
                make_custom_alloc_handler(m_allocator,
                    [ptr](const boost::system::error_code & error, size_t length) { ptr->OnWriteComplete(error, length); }));
        else
            mWriteState = WriteState::Idle;
    }
    //-----------------------------------------------//
}
//-----------------------------------------------//
