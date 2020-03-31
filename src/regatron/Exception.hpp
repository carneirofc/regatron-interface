#pragma once
#include <exception>

namespace Regatron{

    class Exception : public std::exception {
        private:
            const char * m_message;
        public:
            Exception(const char* message): m_message(message){}
            const char* what(){
                return m_message;
            }
    };
}