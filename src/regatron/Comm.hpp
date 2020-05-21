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
    static constexpr std::chrono::seconds AUTOCONNECT_INTERVAL{2};
    static constexpr auto         DELAY_RS232              = std::chrono::seconds{2};
    static constexpr const char*  DEVICE_PREFIX = "/dev/ttyD";
    static constexpr int          DLL_STATUS_OK                  = 0;
    static constexpr int          DLL_STATUS_COMMUNICATION_ERROR = -10;
    static constexpr int          DLL_STATUS_COMMAND_EXECUTION_ERROR = -100;
    static constexpr unsigned int READ_TIMEOUT_MULTIPLIER                = 500;
    static constexpr unsigned int WRITE_TIMEOUT_MULTIPLIER               = 500;

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

    CommStatus  getCommStatus() const;
    bool        getAutoReconnect() const;
    void        setAutoReconnect(bool autoReconnect);
    void        autoConnect();

    /** Regatron Readings */
    std::optional<std::shared_ptr<Regatron::Readings>> getReadings();
};

} // namespace Regatron
