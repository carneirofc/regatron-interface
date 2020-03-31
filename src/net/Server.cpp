#include "Server.hpp"

namespace Net{

    Server::Server(std::shared_ptr<Net::Handler> m_handler): m_handler(std::move(m_handler)){}

    const std::string Server::read(boost::asio::ip::tcp::socket& socket){
        boost::asio::streambuf buf;
        boost::asio::read_until(socket, buf, "\n" );
        const std::string data = boost::asio::buffer_cast<const char*>(buf.data());
        return data;
    }

    void Server::write(boost::asio::ip::tcp::socket& socket, const std::string& message){
       boost::asio::write(socket, boost::asio::buffer(message));
    }

    void Server::listen(const int port){
        LOG_INFO("Listening on port {}.", port);
        boost::asio::io_service io_service;
        //listen for new connection
        boost::asio::ip::tcp::acceptor acceptor_(io_service,
                boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port));
        //socket creation
        boost::asio::ip::tcp::socket socket_(io_service);

        while (true){
            //waiting for connection
            acceptor_.accept(socket_);
            try{
                LOG_INFO("Client connected.");
                //read operation
                while(true){
                    const std::string message = Server::read(socket_);
                    this->write(socket_, this->m_handler->handle(message));
                }
            }catch(std::runtime_error &e){
                LOG_CRITICAL("Exception: {}", e.what());
                boost::system::error_code ec;
                socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_send, ec);
                if (ec){ LOG_ERROR("Failed to shutdown socket. Error {}.", ec.message()); }
                socket_.close(ec);
                if (ec){ LOG_ERROR("Failed to close socket. Error {}.", ec.message()); }
            }
        }
    }
}