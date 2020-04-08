
#include "Handler.hpp"

#include "log/Logger.hpp"
#include <string>
#include <array>

namespace Regatron {

    Handler::Handler(std::shared_ptr<Regatron::Comm> regatronComm):
            m_Regatron(std::move(regatronComm)),
            m_Matchers({
                Match{
                "voltage",
                "%lf", [this](){
                    auto readings = this->m_Regatron->getReadings();
                    return fmt::format("{} {}", ACK, readings->m_ModActualOutVoltageMon);
                }, [this](double param){
                    auto readings = this->m_Regatron->getReadings();
                    readings->m_ModActualOutVoltageMon += param;
                    LOG_INFO("SET voltage {} res={}", param, readings->m_ModActualOutVoltageMon);
                    return ACK;
                }}
            }){}

    const std::string Handler::handle(const std::string& message){
        try{
            for(const auto& m: m_Matchers){
                if(auto response = m.handle(message)){
                    return response.value();
                }
            }

            // Default not found message
            return NACK;
            
            LOG_WARN("No compatible action for {}!", message);
        } catch(const std::invalid_argument& e){
            LOG_CRITICAL("Exception: Invalid argument {} {}.", message, e.what());
        } catch(const std::runtime_error& e){
            LOG_CRITICAL("Exception: A bug !{} {}.", message, e.what());
        }
        return NACK;
    }
}
