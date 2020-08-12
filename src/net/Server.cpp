#include "Server.hpp"

namespace Net {

#if __linux__
    Server::Server(std::shared_ptr<Net::Handler> handler, const char *unixEndpoint)
    : m_handler(std::move(handler)),
      m_IOContext(std::make_shared<asio::io_context>()),
      m_Socket(std::make_shared<asio::generic::stream_protocol::socket>(
          *m_IOContext)),

      m_UNIXAcceptor(nullptr),
    m_TCPAcceptor(nullptr), m_Run{false} {
    if (std::filesystem::exists(unixEndpoint)) {
        LOG_WARN("File {} already exists... Trying to delete it ...",
                 unixEndpoint);
        std::filesystem::remove(unixEndpoint);
    }
    m_UNIXAcceptor = std::make_shared<asio::local::stream_protocol::acceptor>(
        *m_IOContext, asio::local::stream_protocol::endpoint{unixEndpoint});
    LOG_INFO("UNIX Server at endpoint {}", unixEndpoint);
}
#endif

Server::Server(std::shared_ptr<Net::Handler> handler,
               const short unsigned int      tcpPort)
    : m_handler(std::move(handler)),
      m_IOContext(std::make_shared<asio::io_context>()),
      m_Socket(std::make_shared<asio::generic::stream_protocol::socket>(
          *m_IOContext)),
#if __linux__
    m_UNIXAcceptor(nullptr),
#endif
      m_TCPAcceptor(std::make_shared<asio::ip::tcp::acceptor>(
          *m_IOContext, asio::ip::tcp::endpoint{asio::ip::tcp::v4(), tcpPort})),
      m_Run{false} {

    LOG_INFO(R"(Server Socket: TCP Server at port "{}")", tcpPort);
}

Server::~Server() {
    // Shudown socket
    shutdown();
#if __linux__
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
#endif
}

void Server::stop(){
    m_Run = false;
}

void Server::shutdown() {
    // Shutdown socket connections /** if m_Run is true, the socket will start again... @todo: Fix names !*/
    std::error_code ec;
    m_Socket->shutdown(asio::socket_base::shutdown_send, ec);
    if (ec) {
        LOG_DEBUG("Failed to shutdown socket. Error {}.", ec.message());
    } else {
        LOG_INFO("Socket shutdown.");
    }
    m_Socket->close(ec);
    if (ec) {
        LOG_DEBUG(R"(Failed to close socket. "{}".)", ec.message());
    } else {
        LOG_INFO("Socket closed.");
    }
}

auto Server::read() {
    asio::streambuf buf;
    asio::read_until(*m_Socket, buf, "\n");
    return std::string{asio::buffer_cast<const char *>(buf.data())};
}
auto Server::read(std::error_code &ec) {
    asio::streambuf buf;
    asio::read_until(*m_Socket, buf, "\n", ec);
    return std::string{asio::buffer_cast<const char *>(buf.data())};
}

void Server::write(const std::string &message) {
    asio::write(*m_Socket, asio::buffer(message));
}
void Server::write(const std::string &message, std::error_code &ec) {
    asio::write(*m_Socket, asio::buffer(message), ec);
}

void Server::listen() {
    m_Run = true;

    while (m_Run) {
        if (m_TCPAcceptor != nullptr) {
            LOG_INFO("Waiting for client via TCP socket");
            m_TCPAcceptor->accept(*m_Socket);
#if __linux__
        } else if (m_UNIXAcceptor != nullptr) {
            LOG_INFO("Waiting for client via UNIX socket");
            m_UNIXAcceptor->accept(*m_Socket);
#endif
        } else {
            throw std::runtime_error("No acceptor available!");
        }

        try {
            LOG_INFO("Server Socket: Client connected.");

            while (m_Run) {
                const std::string message = Server::read();
                this->write(this->m_handler->handle(message));
            }
        } catch (const std::system_error &e) {
            LOG_CRITICAL(R"(Server Socket: Connection closed. "{}".)", e.what());
            shutdown();
        }
    }
}
} // namespace Net
