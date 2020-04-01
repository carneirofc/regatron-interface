#pragma once

#include "boost/asio.hpp"

namespace Net{
    class Handler{
        public:
            virtual const std::string handle(const std::string& message);
            virtual ~Handler(){}
    };
} // namespace nameNe
