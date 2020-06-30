#pragma once
#include "utils/Instrumentator.hpp"
#include <stdexcept>
#include <string>

namespace Regatron {
enum class CommStatus {
    Ok,
    IOCCommmandFail,
    DLLCommunicationFail,
    DLLCommandExecutionFail,
    Disconncted
};

class CommException : public std::runtime_error {
  protected:
    std::string      m_Message;
    const CommStatus m_CommStatus;

  public:
    CommException(std::string message, CommStatus commstatus)
        : std::runtime_error(""), m_Message(std::move(message)),
          m_CommStatus(commstatus) {}

    CommException(std::string message)
        : std::runtime_error(""), m_Message(std::move(message)),
          m_CommStatus(CommStatus::IOCCommmandFail) {}

    CommException(CommStatus commstatus)
        : std::runtime_error(""), m_CommStatus(commstatus) {
        if (commstatus == CommStatus::Disconncted) {
            m_Message = "Regatron communication is disconnected";
        } else if (commstatus == CommStatus::IOCCommmandFail) {
            m_Message = "ioc: failed to execute an IOC command";
        } else if (commstatus == CommStatus::DLLCommunicationFail) {
            m_Message = "dll status report: communication error";
        } else if (commstatus == CommStatus::DLLCommandExecutionFail) {
            m_Message =
                "dll status report: device reported command execution error";
        } else {
            m_Message = "comm ok?";
        }
    }
    CommStatus getCommStatus() const noexcept { return this->m_CommStatus; }
    virtual const char *what() const noexcept {
        return this->m_Message.c_str();
    }
};
} // namespace Regatron
