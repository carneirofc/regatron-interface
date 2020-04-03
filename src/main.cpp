//
// Created by carneirofc on 05/12/2019.
//
#include <csignal>
#include <iostream>
#include <map>
#include <memory>
#include <stdexcept>

#include "docopt/docopt.h"
#include "log/Logger.hpp"
#include "net/Server.hpp"
#include "regatron/Comm.hpp"
#include "regatron/Handler.hpp"

static const char* USAGE =
R"(Regatron Interface.
Will start a TCP or an UNIX server and listen to commands.
Only one client is supported at time. Use Regatron's serialiolib.
Tries to connect to the device defined by the pattern /dev/ttyDxx,
where xx is a zero padded integer defined by the <regatron_port> argument.
<endpoint> may be a port or a file, according to the socket type (tcp|unix).

    Usage:
      main (tcp|unix) <endpoint> <regatron_port>
      main (-h | --help)
      main --version

    Options:
      -h --help     Show this screen.
      --version     Show version.
)";


int main(const int argc, const char* argv[]){
    std::map<std::string, docopt::value> args
        = docopt::docopt(USAGE,
                         { argv + 1, argv + argc },
                         true,                              // show help if requested
                         "CONS - Regatron Interface v1.0"); // version string

    Utils::Logger::Init();

    bool tcp = args.at("tcp").asBool();
    int regDevPort = static_cast<int>(args.at("<regatron_port>").asLong());
    int tcpPort = tcp ? static_cast<int>(args.at("<endpoint>").asLong()) : -1;
    const std::string unixEndpoint = args.at("<endpoint>").asString();

    std::shared_ptr<Regatron::Comm> regatron = std::make_shared<Regatron::Comm>(regDevPort);
    std::shared_ptr<Regatron::Handler> handler = std::make_shared<Regatron::Handler>(regatron);
    static std::shared_ptr<Net::Server> server = nullptr;

    auto sighandler = +[](int signum)->void{
        /**
         * @fixme:
         * The shutdown function is being called twice.
         * Once by this and once at a try{} block inside Server::listen().
         * */
        LOG_WARN("Capture signal \"{}\", gracefully shutting down...", signum);
        if(server != nullptr){
            server->stop();
            server->shutdown();
        }
        exit(SIGINT);
    };
    signal(SIGINT, sighandler);

    if(tcp){
        server = std::make_shared<Net::Server>(handler, tcpPort);
    }else{
        server = std::make_shared<Net::Server>(handler, unixEndpoint.c_str());
    }

    server->listen();
    return 0;
}
