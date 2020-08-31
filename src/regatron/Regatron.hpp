#pragma once
#include "utils/Instrumentator.hpp"
#include <stdexcept>
#include <string>

namespace Regatron {
enum class CommStatus {
    Ok,
    DLLCommunicationFail,
    DLLCommandExecutionFail,
    Disconncted
};

class CommException : public std::runtime_error {
  protected:
    std::string      m_Message;
    CommStatus m_CommStatus;

  public:
    CommException(std::string message, CommStatus commstatus = CommStatus::DLLCommandExecutionFail)
        : std::runtime_error(""), m_Message(std::move(message)),
          m_CommStatus(commstatus) {}

    CommException(CommStatus commstatus)
        : std::runtime_error(""), m_CommStatus(commstatus) {
        if (commstatus == CommStatus::Disconncted) {
            m_Message = "Regatron communication is disconnected";
        } else if (commstatus == CommStatus::DLLCommunicationFail) {
            m_Message = "dll status report: communication error";
        } else if (commstatus == CommStatus::DLLCommandExecutionFail) {
            m_Message =
                "dll status report: device reported command execution error";
        } else {
            m_Message = "Unknown comm status. ok?";
        }
    }
    CommStatus getCommStatus() const noexcept { return this->m_CommStatus; }
    virtual const char *what() const noexcept {
        return this->m_Message.c_str();
    }
};
} // namespace Regatron
