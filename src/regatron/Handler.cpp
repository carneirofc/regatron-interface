
#include "Handler.hpp"

#include "log/Logger.hpp"
#include <string>
#include <array>

namespace Regatron {

    Handler::Handler(std::shared_ptr<Regatron::Comm> regatron):
            m_regatron(std::move(regatron)),
            m_Matchers({
                Match{"voltage", "%f", [](){ return "GET ACK\n"; }, [](float){ return "SET ACK\n"; }}
            }){}

    const std::string Handler::handle(const std::string& message){
        try{
            for(auto matcher: m_Matchers){
                if(int commandType = matcher.shouldHandle(message)){
                    return matcher.handle(message, commandType);
                }
            }
            return ACK;
            
            LOG_WARN("No compatible action for {}!", message);
        } catch(const std::invalid_argument& e){
            LOG_CRITICAL("Exception: Invalid argument {} {}.", message, e.what());
        } catch(const std::runtime_error& e){
            LOG_CRITICAL("Exception: A bug !{} {}.", message, e.what());
        }
        return NACK;
    }
}
