#pragma once

#include "net/Handler.hpp"
#include "regatron/Comm.hpp"

namespace Regatron{
    constexpr const char* NACK = "NACK\n";
    constexpr const char* ACK = "ACK\n";

    /** 
     * @todo: impplement an elegant dispatcher? 
     * @todo: Migrate to a template class?
     * ideas: Have an object checker list that will forward the message to the correct handler method.
     * */
    class Match {
        private:
        static constexpr const char* GET = "get ";
        static constexpr const char* SET = "set ";

        static constexpr int unsupportedMessage = -10;
        static constexpr int invalidCommand = -1;
        static constexpr int getCommand = 0;
        static constexpr int setCommand = 1;

        const std::string m_BasePattern;
        const std::string m_SetFormat;
        const std::string m_GetPattern;
        const std::string m_SetPattern;
        const std::function<std::string()> m_GetHandleFunc;
        const std::function<std::string(float)> m_SetHandleFunc;

        static auto getCommandType(const std::string& message){
            if(message.starts_with(GET)){ return getCommand; }
            if(message.starts_with(SET)){ return setCommand; }
            return invalidCommand;
        }

        public:
        Match(const std::string basePattern, const std::string setFormat,
                std::function<std::string()> getHandle,
                std::function<std::string(float)> setHandle)
            :m_BasePattern(basePattern),
            m_SetFormat(setFormat),
            m_GetPattern(GET + m_BasePattern + "\n"),
            m_SetPattern(SET + m_BasePattern + m_SetFormat + "\n"),
            m_GetHandleFunc(getHandle),
            m_SetHandleFunc(setHandle){}

        auto toString(){ return fmt::format("Match for base pattern {}. Set format {}", m_BasePattern, m_SetFormat); }

        /** Check if this object should handle the message and returns the command type */
        int shouldHandle(const std::string& message){
            return true;
        }
        
        /** Respond a message according to the command type.
         * @return: A response to the client.
         * @throws:
         * */
        const std::string handle(const std::string& message, const int commandType){
            switch(commandType){
                default:
                    break;

                case getCommand:
                    break;
                case setCommand:
                      break;
            }

            LOG_ERROR("message {} is not valid for Match {}", message, toString());
            return NACK;
        }

        /** throws: May throw something (std::invalid_argument)! */
        auto handleGet(const std::string& message, const float& res){
            return (std::sscanf(message.c_str(), m_GetPattern.c_str(), &res) != 1);
        }
        
    };

    class Handler : public Net::Handler{
        public:
            Handler(std::shared_ptr<Regatron::Comm> regatron);
        private:
            std::shared_ptr<Regatron::Comm> m_regatron;
            std::vector<Match> m_Matchers;

            bool getParam(const std::string message, const char* beg_pattern, const char* complete_pattern, float* res);
            const std::string handle(const std::string& message) override;
    };
}
