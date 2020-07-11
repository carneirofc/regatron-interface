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
#include <optional>
#include <string>
#include <thread>

namespace Regatron {

class Comm {
    static constexpr std::chrono::seconds AUTOCONNECT_INTERVAL{15};
    static constexpr std::chrono::seconds DELAY_RS232{5};
#if __linux__
    static constexpr const char *DEVICE_PREFIX = "/dev/ttyD";
#else
    static constexpr const char *DEVICE_PREFIX = "COM";
#endif
    static constexpr int          DLL_STATUS_OK                      = 0;
    static constexpr int          DLL_STATUS_COMMUNICATION_ERROR     = -10;
    static constexpr int          DLL_STATUS_COMMAND_EXECUTION_ERROR = -100;
    static constexpr unsigned int READ_TIMEOUT_MULTIPLIER            = 10;
    static constexpr unsigned int WRITE_TIMEOUT_MULTIPLIER           = 10;

  private:
    int                                 m_Port;        /** comm port */
    int                                 m_PortNrFound; /** detected comm port */
    std::shared_ptr<Regatron::Readings> m_readings;    /** Readings*/
    double     incDevVoltage;    /** Increment (internal usage) */
    double     incDevCurrent;    /** Increment (internal usage) */
    double     incDevPower;      /** Increment (internal usage) */
    double     incDevResistance; /** Increment (internal usage) */
    double     incSysVoltage;    /** Increment (internal usage) */
    double     incSysCurrent;    /** Increment (internal usage) */
    double     incSysPower;      /** Increment (internal usage) */
    double     incSysResistance; /** Increment (internal usage) */
    CommStatus m_CommStatus;
    bool       m_AutoReconnect;
    bool       m_Connected;
    std::chrono::time_point<std::chrono::system_clock>
         m_AutoReconnectAttemptTime;
    void InitializeDLL();

  public:
    explicit Comm(int port);
    Comm();
    ~Comm();
    bool connect();
    bool connect(int port);
    bool connect(int fromPort, int toPort);
    void disconnect();

    /**
     * This method will read and set the actual communication status
     * @return CommStatus
     * */
    CommStatus ReadCommStatus() {

        int pState{-1};
        int pErrorNo{0};

        if (DllGetStatus(&pState, &pErrorNo) != DLL_SUCCESS) {
            LOG_WARN("DLL: Failed to get DLL status.");
            m_CommStatus = CommStatus::IOCCommmandFail;
            return m_CommStatus;
        }

        switch (pState) {
        case DLL_STATUS_OK:
            m_CommStatus = CommStatus::Ok;
            break;

        case DLL_STATUS_COMMUNICATION_ERROR:
            LOG_WARN(R"(DLL Communication Error. State="{}", ErrorNo="{}")",
                     pState, pErrorNo);
            m_CommStatus = CommStatus::DLLCommunicationFail;
            break;

        case DLL_STATUS_COMMAND_EXECUTION_ERROR:
            LOG_WARN(R"(DLL Command Execution Error. State="{}", ErrorNo="{}")",
                     pState, pErrorNo);
            m_CommStatus = CommStatus::DLLCommandExecutionFail;
            break;
        default:
            LOG_WARN(R"(Unknown DLL Status. State="{}", ErrorNo="{}")", pState,
                     pErrorNo);
            m_CommStatus = CommStatus::IOCCommmandFail;
        }
        return m_CommStatus;
    }

    CommStatus getCommStatus() const;
    bool       getAutoReconnect() const;
    void       setAutoReconnect(bool autoReconnect);
    void       autoConnect();

    /**
     *   Regatron Readings
     * This method will return the Redings object if the
     * actual communication state is valid.
     */
    std::optional<std::shared_ptr<Regatron::Readings>> getReadings();
};

} // namespace Regatron
