#pragma once

#include "Readings.hpp"
#include "Regatron.hpp"
#include "Version.hpp"

#include "fmt/format.h"
#include "serialiolib.h" // NOLINT
#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <memory>
#include <string>
#include <thread>

namespace Regatron {

class Comm {
    static constexpr std::chrono::seconds AUTOCONNECT_INTERVAL{15};
    static constexpr auto         DELAY_RS232              = std::chrono::seconds{2};
    static constexpr const char*  DEVICE_PREFIX = "/dev/ttyD";
    static constexpr int          DLL_STATUS_OK                  = 0;
    static constexpr int          DLL_STATUS_COMMUNICATION_ERROR = -10;
    static constexpr int          DLL_STATUS_COMMAND_EXECUTION_ERROR = -100;
    static constexpr unsigned int READ_TIMEOUT_MULTIPLIER  = 100;
    static constexpr unsigned int WRITE_TIMEOUT_MULTIPLIER = 100;

  private:
    // Connection
    int m_Port        = 0;
    int m_PortNrFound = -1;

    // Readings
    std::shared_ptr<Regatron::Readings> m_readings;

    // Increment (internal usage)
    double incDevVoltage    = 0.0;
    double incDevCurrent    = 0.0;
    double incDevPower      = 0.0;
    double incDevResistance = 0.0;
    double incSysVoltage    = 0.0;
    double incSysCurrent    = 0.0;
    double incSysPower      = 0.0;
    double incSysResistance = 0.0;

    CommStatus m_CommStatus;
    bool       m_AutoReconnect;
    bool       m_Connected;
    std::chrono::time_point<std::chrono::system_clock>
        m_AutoReconnectAttemptTime;
    void CheckDLLStatus();
    void InitializeDLL();

  public:
    explicit Comm(int port);
    Comm();
    ~Comm();
    bool connect();
    bool connect(int port);
    bool connect(int fromPort, int toPort);
    void disconnect();

    CommStatus getCommStatus() const { return m_CommStatus; }
    bool       getAutoReconnect() const { return m_AutoReconnect; };
    void       setAutoReconnect(bool autoReconnect) {
        m_AutoReconnect = autoReconnect;
        LOG_TRACE(R"(autoReconnect: "{}")", m_AutoReconnect);
    }
    void inline autoConnect() {
        if (m_AutoReconnect && !m_Connected) {
            auto now = std::chrono::system_clock::now();
            auto timeDelta = now - m_AutoReconnectAttemptTime;

            if (timeDelta < AUTOCONNECT_INTERVAL) {
                LOG_TRACE(
                    R"(autoconnect: timeout is active for more "{} s", ignoring atempt.)",
                    (std::chrono::duration_cast<std::chrono::seconds>(
                         AUTOCONNECT_INTERVAL - timeDelta))
                        .count());
                return;
            }

            LOG_TRACE(
                R"(autoconnect: attempting to connect after "{} s".)",
                std::chrono::duration_cast<std::chrono::seconds>(timeDelta)
                    .count());
            try {
                m_AutoReconnectAttemptTime = now;
                connect();
            } catch (const CommException &e) {
                LOG_ERROR(R"(autoconnect: Failed to connect "{}")", e.what());
            }
        }
    }

    /** Regatron Readings */
    std::optional<std::shared_ptr<Regatron::Readings>> getReadings() {
        if (m_CommStatus != CommStatus::Ok) {
            LOG_ERROR(R"(getReadings failed with invalid comm status "{}")",
                      static_cast<int>(m_CommStatus));
            return {};
        }
        return {m_readings};
    };
};

} // namespace Regatron
