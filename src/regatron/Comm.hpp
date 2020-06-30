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
    static constexpr std::chrono::seconds DELAY_RS232{5};
    static constexpr const char *         DEVICE_PREFIX = "/dev/ttyD";
    static constexpr int                  DLL_STATUS_OK = 0;
    static constexpr int                  DLL_STATUS_COMMUNICATION_ERROR = -10;
    static constexpr int          DLL_STATUS_COMMAND_EXECUTION_ERROR     = -100;
    static constexpr unsigned int READ_TIMEOUT_MULTIPLIER                = 10;
    static constexpr unsigned int WRITE_TIMEOUT_MULTIPLIER               = 10;

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

    /** This method will signal if a new initialization is required
     * @return true if DLL is OK otherwise false*/
    bool IsCommOk() {

        int pState{-1};
        int pErrorNo{0};

        if (DllGetStatus(&pState, &pErrorNo) != DLL_SUCCESS) {
            LOG_WARN("Failed to get DLL status.");
            return false;
        }
        if (pState == DLL_STATUS_COMMUNICATION_ERROR) {
            LOG_WARN(R"(DLL Communication Error. State="{}", ErrorNo="{}")",
                     pState, pErrorNo);
            return false;
        }
        if (pState == DLL_STATUS_COMMAND_EXECUTION_ERROR) {
            LOG_WARN(R"(DLL Command Execution Error. State="{}", ErrorNo="{}")",
                     pState, pErrorNo);
            return false;
        }
        return true;
    }
    CommStatus getCommStatus() const;
    bool       getAutoReconnect() const;
    void       setAutoReconnect(bool autoReconnect);
    void       autoConnect();

    /** Regatron Readings */
    std::optional<std::shared_ptr<Regatron::Readings>> getReadings();
};

} // namespace Regatron
