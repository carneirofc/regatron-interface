//
// Created by carneirofc on 05/12/2019.
//
#include <memory>
#include <stdexcept>

#include "log/Logger.hpp"
#include "net/Server.hpp"
#include "regatron/Handler.hpp"
#include "regatron/Comm.hpp"

void help(){
    LOG_INFO(
        "Regatron Interface\n"
        "   params:\n"
        "   - port:\n"
        "   - regatron device num:\n"
    );

}

int main(const int argc, const char* argv[]){
    Utils::Logger::Init();
    int tcpPort;
    int regDevPort;

    if(argc == 3){
        try{
            tcpPort = std::stoi(argv[1]);
            regDevPort = std::stoi(argv[2]);

        }catch(const std::invalid_argument& e){
            LOG_CRITICAL("Invalid argument {}.", e.what());
            exit(-1);
        }catch(const std::logic_error& e){
            LOG_CRITICAL("Ops... a logic error x_x'\n"
            "Probably an illegal access due to wrong parameter checks.", e.what());
            exit(-1);
        }
    }else{
        help();
        exit(-1);
    }

    std::shared_ptr<Regatron::Comm> regatron = std::make_shared<Regatron::Comm>(regDevPort);
    std::shared_ptr<Regatron::Handler> handler = std::make_shared<Regatron::Handler>(regatron);
    std::shared_ptr<Net::Server> server = std::make_shared<Net::Server>(handler);
    server->listen(tcpPort);

    return 0;
}
