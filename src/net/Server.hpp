#pragma once

#include "log/Logger.hpp"
#include "net/Handler.hpp"

#include <boost/asio.hpp>

namespace Net{
    class Server{
        public:
            Server(std::shared_ptr<Net::Handler> m_handler);
            void listen(const int port);

        private:
            void write(boost::asio::ip::tcp::socket& socket,const std::string& message);
            const std::string read(boost::asio::ip::tcp::socket& socket);
            std::shared_ptr<Net::Handler> m_handler;
    };
}