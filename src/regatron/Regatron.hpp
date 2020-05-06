#pragma once
#include <exception>
#include <string>

namespace Regatron {
enum class CommStatus { Ok, CommFail, DLLFail, DLLCommFail, Disconncted };

class CommException : public std::exception {
  protected:
    const char *     m_Message;
    const CommStatus m_CommStatus;

  public:
    virtual ~CommException() throw() = default;

    CommException(const std::string &message, CommStatus commstatus)
        : CommException(message.c_str(), commstatus) {}
    CommException(const char *message, CommStatus commstatus)
        : m_Message(message), m_CommStatus(commstatus) {}

    CommException(const std::string &message)
        : CommException(message.c_str()) {}
    CommException(const char *message)
        : m_Message(message), m_CommStatus(CommStatus::CommFail) {}

    explicit CommException(CommStatus commstatus) : m_CommStatus(commstatus) {
        if (commstatus == CommStatus::Disconncted) {
            m_Message = "comm is disconnected";
        } else if (commstatus == CommStatus::CommFail) {
            m_Message = "comm operation failed";
        } else if (commstatus == CommStatus::DLLFail) {
            m_Message = "dll status report: communication error";
        } else if (commstatus == CommStatus::DLLCommFail) {
            m_Message =
                "dll status report: device reported command execution error";
        } else {
            m_Message = "comm ok?";
        }
    }
    virtual CommStatus  getCommStatus() const throw() { return m_CommStatus; }
    virtual const char *what() const throw() { return m_Message; }
};
} // namespace Regatron
