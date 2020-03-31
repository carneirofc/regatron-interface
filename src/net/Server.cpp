#include "Server.hpp"

namespace Net{

    Server::Server(
            std::shared_ptr<Net::Handler> handler,
            const short unsigned int tcpPort):
        m_handler(std::move(handler)),
        m_IOContext(),
        m_Socket(boost::asio::generic::stream_protocol::socket{m_IOContext}),
        m_UNIXAcceptor(nullptr),
        m_TCPAcceptor(nullptr)
       {
            //socket creation
            // m_Socket = boost::asio::generic::stream_protocol::socket(m_IOContext);
            //listen for new connection
            m_TCPAcceptor = std::make_shared<boost::asio::ip::tcp::acceptor>
                (m_IOContext, boost::asio::ip::tcp::endpoint{boost::asio::ip::tcp::v4(), tcpPort});
        }

    const std::string Server::read(){
        boost::asio::streambuf buf;
        boost::asio::read_until(m_Socket, buf, "\n" );
        const std::string data = boost::asio::buffer_cast<const char*>(buf.data());
        return data;
    }

    void Server::write(const std::string& message){
       boost::asio::write(m_Socket, boost::asio::buffer(message));
    }

    void Server::listen(){

        while (true){
            if(m_TCPAcceptor != nullptr){
                m_TCPAcceptor->accept(m_Socket);
            }else if(m_UNIXAcceptor != nullptr){
                m_UNIXAcceptor->accept(m_Socket);
            }else{ throw std::runtime_error("No acceptor available!"); }

        // LOG_INFO("Listening on port {}.", port);
        //listen for new connection
        // boost::asio::ip::tcp::acceptor acceptor_(m_IOContext,
        //         boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port));
        // boost::asio::basic_socket_acceptor acceptor_(boost::asio::ip::tcp::v4(), port);
            //waiting for connection
            // acceptor_.accept(m_Socket);
            try{
                LOG_INFO("Client connected.");
                //read operation
                while(true){
                    const std::string message = Server::read();
                    this->write(this->m_handler->handle(message));
                }
            }catch(std::runtime_error &e){
                LOG_CRITICAL("Exception: {}", e.what());
                boost::system::error_code ec;
                m_Socket.shutdown(boost::asio::socket_base::shutdown_send, ec);
                if (ec){ LOG_ERROR("Failed to shutdown socket. Error {}.", ec.message()); }
                m_Socket.close(ec);
                if (ec){ LOG_ERROR("Failed to close socket. Error {}.", ec.message()); }
            }
        }
    }
}