#pragma once

#include "log/Logger.hpp"
#include "net/Handler.hpp"

#include <boost/asio.hpp>

namespace Net{
    class Server{
        public:
            Server(std::shared_ptr<Net::Handler> handler);
            Server(std::shared_ptr<Net::Handler> handler, const short unsigned int tcpPort);
            Server(std::shared_ptr<Net::Handler> handler, const unsigned char* unixEndpoint);
            void listen();

        private:
            std::shared_ptr<Net::Handler> m_handler;
            boost::asio::io_context m_IOContext;
            boost::asio::generic::stream_protocol::socket m_Socket;
            std::shared_ptr<boost::asio::local::stream_protocol::acceptor> m_UNIXAcceptor;
            std::shared_ptr<boost::asio::ip::tcp::acceptor> m_TCPAcceptor;
            void write(const std::string& message);
            const std::string read();
    };
}