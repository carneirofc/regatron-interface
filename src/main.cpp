//
// Created by carneirofc on 05/12/2019.
//
#include <csignal>
#include <iostream>
#include <map>
#include <memory>
#include <stdexcept>
#include <chrono>

#include "docopt/docopt.h"
#include "log/Logger.hpp"
#include "net/Server.hpp"
#include "regatron/Comm.hpp"
#include "regatron/Handler.hpp"
#include "utils/Instrumentator.hpp"

constexpr const char *VERSION_STRING = "CONS - Regatron Interface v1.0.1";
constexpr const char *USAGE =
    R"(Regatron Interface.
Will start a TCP or an UNIX server and listen to commands. On Windows, as to be expected, only TCP servers are available.
One client is supported at time. Tries to connect to the device defined by the pattern /dev/ttyUSBxx or COMx,
where xx is an integer defined by the <regatron_port> argument.
<endpoint> may be a port or a file, according to the socket type (tcp|unix).
When using TCP connections, the server welcome port will be 20000 + <regatron_port>.

    Usage:
)"
#if __linux__
    R"(      main (tcp|unix) <regatron_port>)"
#else
    R"(      main <regatron_port> [--reconnect_interval=<sec>])"
#endif
    R"(
      main (-h | --help)
      main --version

    Options:
      -h --help                   Show this screen.
      --version                   Show version.
      --reconnect_interval=<sec>  Interval in seconds between reconnect attempts [default: 15].

)";

int main(const int argc, const char *argv[]) {
    std::map<std::string, docopt::value> args =
        docopt::docopt(USAGE, {argv + 1, argv + argc},
                       true,            // show help if requested
                       VERSION_STRING); // version string
#if __linux__
    bool tcp = args.at("tcp").asBool();
#else
    bool tcp = true;
#endif
    int regDevPort = static_cast<int>(args.at("<regatron_port>").asLong());

    Utils::Logger::Init(spdlog::level::level_enum::trace, fmt::format("RegatronCOM{:03}Log.txt", regDevPort).c_str());

    static std::shared_ptr<Regatron::Comm> regatron =
        std::make_shared<Regatron::Comm>(regDevPort);
    static std::shared_ptr<Regatron::Handler> handler =
        std::make_shared<Regatron::Handler>(regatron);
    static std::shared_ptr<Net::Server> server = nullptr;

    if (args.count("--reconnect_interval")) {
        try {
            if (auto reconnectInterval =
                    args.at("--reconnect_interval").asLong()) {
                regatron->SetAutoReconnectInterval(
                    std::chrono::seconds{reconnectInterval});
            }
        } catch (const std::runtime_error &e) {
        }
        LOG_INFO(R"(Regatron reconnect interval at "{} seconds")",
            regatron->GetAutoReconnectInterval().count());
    }

    auto sighandler = +[](int signum) -> void {
        /**
         * @fixme:
         * The shutdown function is being called twice.
         * Once by this and once at a try{} block inside Server::listen().
         * */
        LOG_WARN(R"(Capture signal "{}", gracefully shutting down...)", signum);
        if (server != nullptr) {
            regatron->setAutoReconnect(false);
            regatron->disconnect();

            server->stop();
            server->shutdown();
        }

        INSTRUMENTATOR_PROFILE_END_SESSION();
        exit(SIGINT);
    };
    signal(SIGINT, sighandler);

#if __linux__
    if (!tcp) {
        const std::string unixEndpoint =
            fmt::format("/var/tmp/REG{:02}", regDevPort);
        LOG_INFO("Using unix endpoint at {}", unixEndpoint);
        server = std::make_shared<Net::Server>(handler, unixEndpoint.c_str());
    }
#endif

    if (tcp) {
        int tcpServerPort = 20000 + regDevPort;
        server = std::make_shared<Net::Server>(handler, tcpServerPort);
    }
    INSTRUMENTATOR_PROFILE_BEGIN_SESSION(
        "Listen", "cons_regatron_interface_results.json");
    server->listen();
    INSTRUMENTATOR_PROFILE_END_SESSION();
    return 0;
}
