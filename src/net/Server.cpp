#include "Server.hpp"

namespace Net {

Server::Server(std::shared_ptr<Net::Handler> handler, const char *unixEndpoint)
    : m_handler(std::move(handler)),
      m_IOContext(std::make_shared<boost::asio::io_context>()),
      m_Socket(std::make_shared<boost::asio::generic::stream_protocol::socket>(
          *m_IOContext)),
      m_UNIXAcceptor(nullptr), m_TCPAcceptor(nullptr), m_Run{false} {
    if (std::filesystem::exists(unixEndpoint)) {
        LOG_WARN("File {} already exists... Trying to delete it ...",
                 unixEndpoint);
        std::filesystem::remove(unixEndpoint);
    }
    m_UNIXAcceptor =
        std::make_shared<boost::asio::local::stream_protocol::acceptor>(
            *m_IOContext,
            boost::asio::local::stream_protocol::endpoint{unixEndpoint});
    LOG_INFO("Server at endpoint {}", unixEndpoint);
}
Server::Server(std::shared_ptr<Net::Handler> handler,
               const short unsigned int      tcpPort)
    : m_handler(std::move(handler)),
      m_IOContext(std::make_shared<boost::asio::io_context>()),
      m_Socket(std::make_shared<boost::asio::generic::stream_protocol::socket>(
          *m_IOContext)),
      m_UNIXAcceptor(nullptr),
      m_TCPAcceptor(std::make_shared<boost::asio::ip::tcp::acceptor>(
          *m_IOContext,
          boost::asio::ip::tcp::endpoint{boost::asio::ip::tcp::v4(), tcpPort})),
      m_Run{false} {
    LOG_INFO("Server at port \"{}\"", tcpPort);
}

Server::~Server() {
    // Shudown socket
    shutdown();

    // Delete UNIX endpoint
    if (m_UNIXAcceptor != nullptr) {
        try {
            std::error_code   stde;
            const std::string endpoint =
                m_UNIXAcceptor->local_endpoint().path();
            LOG_INFO("Removing UNIX socket endpoint \"{}\"", endpoint);
            std::filesystem::remove(endpoint, stde);

        } catch (std::error_code &e) {
            LOG_ERROR("Failed to remove UNIX endpoint");
        }
    }
}

void Server::shutdown() {
    // Shutdown socket
    boost::system::error_code ec;
    m_Socket->shutdown(boost::asio::socket_base::shutdown_send, ec);
    if (ec) {
        LOG_WARN("Failed to shutdown socket. Error {}.", ec.message());
    } else {
        LOG_INFO("Socket shutdown.");
    }
    m_Socket->close(ec);
    if (ec) {
        LOG_ERROR("Failed to close socket. Error {}.", ec.message());
    } else {
        LOG_INFO("Socket closed.");
    }
}

auto Server::read() {
    boost::asio::streambuf buf;
    boost::asio::read_until(*m_Socket, buf, "\n");
    return std::string{boost::asio::buffer_cast<const char *>(buf.data())};
}
auto Server::read(boost::system::error_code &ec) {
    boost::asio::streambuf buf;
    boost::asio::read_until(*m_Socket, buf, "\n", ec);
    return std::string{boost::asio::buffer_cast<const char *>(buf.data())};
}

void Server::write(const std::string &message) {
    boost::asio::write(*m_Socket, boost::asio::buffer(message));
}
void Server::write(const std::string &message, boost::system::error_code &ec) {
    boost::asio::write(*m_Socket, boost::asio::buffer(message), ec);
}

void Server::stop() { m_Run = false; }

void Server::listen() {
    m_Run = true;

    while (m_Run) {
        if (m_TCPAcceptor != nullptr) {
            LOG_INFO("Waiting for client via TCP socket");
            m_TCPAcceptor->accept(*m_Socket);
        } else if (m_UNIXAcceptor != nullptr) {
            LOG_INFO("Waiting for client via UNIX socket");
            m_UNIXAcceptor->accept(*m_Socket);
        } else {
            throw std::runtime_error("No acceptor available!");
        }

        try {
            LOG_INFO("Client connected.");

            while (m_Run) {
                const std::string message = Server::read();
                this->write(this->m_handler->handle(message));
            }
        } catch (const boost::system::system_error &e) {
            if (e.code() == boost::asio::error::eof) {
                LOG_WARN("Connection closed by the client. {}", e.what());
            } else if (e.code() == boost::asio::error::operation_aborted) {
                LOG_WARN("Connection closed by the server. {}", e.what());
            } else {
                throw;
            }

            shutdown();
        }
    }
}
} // namespace Net
