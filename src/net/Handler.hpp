#pragma once

#include <asio.hpp> // NO LINT

namespace Net {
class Handler {
  public:
    virtual std::string handle(const std::string &message) = 0;
    virtual ~Handler()                                     = default;

  protected:
    Handler()                = default;
    Handler(const Handler &) = default;
    Handler(Handler &&)      = default;
    Handler &operator=(const Handler &) = default;
    Handler &operator=(Handler &&) = default;
};
} // namespace Net
