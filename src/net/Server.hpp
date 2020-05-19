#pragma once

#include "log/Logger.hpp"
#include "net/Handler.hpp"

#include <asio.hpp> // NOLINT
#include <filesystem>
#include <system_error>

namespace Net {
class Server {
  public:
    Server() = delete;
    Server &operator=(const Server &) = default;
    Server &operator=(Server &&) = default;
    ~Server();

    Server(std::shared_ptr<Net::Handler> handler,
           const short unsigned int      tcpPort);
    Server(std::shared_ptr<Net::Handler> handler, const char *unixEndpoint);
    void listen();
    void shutdown();
    void stop();


  private:
    std::shared_ptr<Net::Handler>                                  m_handler;
    std::shared_ptr<asio::io_context>                              m_IOContext;
    std::shared_ptr<asio::generic::stream_protocol::socket>        m_Socket;
    std::shared_ptr<asio::local::stream_protocol::acceptor> m_UNIXAcceptor;
    std::shared_ptr<asio::ip::tcp::acceptor>                m_TCPAcceptor;
    void              write(const std::string &message, std::error_code &ec);
    void write(const std::string &message);
    auto              read(std::error_code &ec);
    auto              read();
    bool              m_Run;
};
} // namespace Net
