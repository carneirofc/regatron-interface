#pragma once

#include "boost/asio.hpp"

namespace Net {
class Handler {
  public:
    virtual const std::string handle(const std::string &message);
    Handler()                = default;
    Handler(const Handler &) = default;
    Handler(Handler &&)      = default;
    Handler &operator=(const Handler &) = default;
    Handler &operator=(Handler &&) = default;
    virtual ~Handler()             = default;
};
} // namespace Net
