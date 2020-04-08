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
        std::optional<double> handleSet(const char* message) const {
            double data{0};
            int r = std::sscanf(message, m_SetPattern.c_str(), &data);
            std::cout << "Pattern: "  << m_SetPattern.c_str()
                << "\nMessage: "<< message
                << "\nr: "<< r
                << "\nsscanf: " << data 
                << "\nDereference: " << &data << std::endl;
            return r == 1 ? std::optional<double>{data} : std::nullopt;
        }

        /** Respond a message according to the command type.
         * @return: A response to the client.
         * @throws:
         * */
        std::optional<std::string> handle(const std::string& message) const {
            CommandType commandType;

            std::optional<double> param;
        
            if(message.starts_with(GET) && message == m_GetPattern){
                commandType = CommandType::getCommand;

            } else if(message.starts_with(SET) && (param = handleSet(message.c_str()))){
                commandType = CommandType::setCommand;
            }else {
                commandType = CommandType::invalidCommand;
            }

            switch(commandType){
               default:
                   LOG_ERROR("message {} is not valid for Match {}", message, toString());
                   return {};

                case CommandType::getCommand:
                    return {m_GetHandleFunc()};

                case CommandType::setCommand:
                    return {m_SetHandleFunc(param.value())};
            }  
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
