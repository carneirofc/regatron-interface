#pragma once

#include "net/Handler.hpp"
#include "regatron/Comm.hpp"
#include <chrono>
#include <iostream>
#include <optional>

namespace Regatron{
    const std::string NACK{"NACK\n"};
    const std::string ACK{"ACK\n"};

    enum class CommandType :int {
        unsupportedMessage = -10,
        invalidCommand = -1,
        getCommand = 0,
        setCommand = 1
    };

    /** 
     * @todo: impplement an elegant dispatcher? 
     * @todo: Migrate to a template class?
     * ideas: Have an object checker list that will forward the message to the correct handler method.
     * */
    class Match {
        private:
        static constexpr const char* GET = "get";
        static constexpr const char* SET = "set";

        const std::string m_BasePattern;
        const std::string m_SetFormat;
        const std::string m_GetPattern;
        const std::string m_SetPattern;
        const std::function<std::string()> m_GetHandleFunc;
        const std::function<std::string(double)> m_SetHandleFunc;

        CommandType getCommandType(const std::string& message){
            if(message.starts_with(GET)){ return CommandType::getCommand; }
            if(message.starts_with(SET)){ return CommandType::setCommand; }
            return CommandType::invalidCommand;
        }

        public:
        Match(  const std::string basePattern,
                const std::string setFormat,
                std::function<std::string()> getHandle,
                std::function<std::string(double)> setHandle)
            :m_BasePattern(basePattern),
            m_SetFormat(setFormat),
            m_GetPattern(fmt::format("{} {}\n", GET, m_BasePattern)),
            m_SetPattern(fmt::format("{} {} {}\n",SET, m_BasePattern, m_SetFormat)),
            m_GetHandleFunc(std::move(getHandle)),
            m_SetHandleFunc(std::move(setHandle)){}

        auto toString() const{ return fmt::format("Match for base pattern \"{}\". set format \"{}\"", m_BasePattern, m_SetFormat); }

        /** throws: May throw something (std::invalid_argument)! */
        auto handleSet(const char* message) const {
            double data;
            int r = std::sscanf(message, m_SetPattern.c_str(), &data);
            return r == 1 ? std::optional<double>{data} : std::nullopt;
        }

        /** Check if this object should handle the message and returns the command type */
        CommandType shouldHandle(const std::string& message, double& param) const{
            if(message.starts_with(GET)){
                if(message == m_GetPattern){
                    return CommandType::getCommand;
                } else {
                    return CommandType::unsupportedMessage;
                }

            } else if(message.starts_with(SET)){
                if(handleSet(message.c_str(), param)){
                    return CommandType::setCommand;
                } else {
                    return CommandType::unsupportedMessage;
                }

            }

            return CommandType::invalidCommand;
        }
        
        /** Respond a message according to the command type.
         * @return: A response to the client.
         * @throws:
         * */
        auto handle(const std::string& message, const CommandType commandType, double param) const {
            //std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
            std::string res;
            switch(commandType){
                default:
                    LOG_ERROR("message {} is not valid for Match {}", message, toString());
                    res = NACK;
                    break;

                case CommandType::getCommand:
                    res = m_GetHandleFunc();
                    break;

                case CommandType::setCommand:
                    res = m_SetHandleFunc(param);
                    break;
            }
            LOG_TRACE("message \"{}\", response \"{}\", commandType \"{}\"", message, res, static_cast<int>(commandType));
            
            //std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
            //std::cout << "Time difference = " <<
            //    std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[µs]" << std::endl;
            return res;
        }
        
    };

    class Handler : public Net::Handler{
        public:
            Handler(std::shared_ptr<Regatron::Comm> regatronComm);
        private:
            std::shared_ptr<Regatron::Comm> m_Regatron;
            std::vector<Match> m_Matchers;
            const std::string handle(const std::string& message) override;
    };
}
