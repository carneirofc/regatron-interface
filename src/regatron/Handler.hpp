#include "net/Handler.hpp"
#include "regatron/Comm.hpp"

namespace Regatron{
    class Handler : public Net::Handler{
        public:
            Handler(std::shared_ptr<Regatron::Comm> regatron);
        private:
            std::shared_ptr<Regatron::Comm> m_regatron;

            bool getParam(const std::string message, const char* beg_pattern, const char* complete_pattern, float* res);
            /** Try to get a float from the message based on a patter.
             * This should be used inside Reg::Handler::handle() method!
             * return: true or false
             * throws: May throw something (std::invalid_argument)!
             */
            #define MATCH_SET(pattern) getParam(message, SET pattern, SET pattern "%f\n", &res) 
            #define MATCH_GET(pattern) (message == GET pattern "\n")
            
            const std::string handle(const std::string& message) override;
    };
}
