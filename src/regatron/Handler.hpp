#pragma once

#include "net/Handler.hpp"
#include "regatron/Comm.hpp"
#include "regatron/Match.hpp"

#include <chrono>
#include <iostream>
#include <optional>

namespace Regatron {
const std::string NACK{"NACK\n"};
const std::string ACK{"ACK\n"};

class Handler : public Net::Handler {
  public:
    Handler(std::shared_ptr<Regatron::Comm> regatronComm);

  private:
    std::shared_ptr<Regatron::Comm> m_Regatron;
    std::vector<Match>              m_Matchers;
    const std::string               handle(const std::string &message) override;
};
} // namespace Regatron
