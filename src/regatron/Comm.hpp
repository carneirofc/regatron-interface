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
    CommStatus m_CommStatus;     /** DLL communication details */
    bool       m_AutoReconnect;  /** Auto reconnect to device */
    bool       m_Connected;      /** Whether we are connected to the device */
    std::chrono::time_point<std::chrono::system_clock>
         m_AutoReconnectAttemptTime;
    std::chrono::seconds m_AutoReconnectInterval;
    void InitializeDLL();

  public:
    explicit Comm(int port);
    Comm();
    ~Comm();
    bool connect();
    bool connect(int port);
    bool connect(int fromPort, int toPort);

    /** Will call DLLClose(), reset internal usage variables, set m_PortNrFound to -1,
        set m_Connected to false and m_CommStatus to CommStatus::Disconnected.
    */
    void disconnect();

    void SetAutoReconnectInterval(std::chrono::seconds&& seconds);
    std::chrono::seconds GetAutoReconnectInterval() const;
    /**
     * This method will read and set the actual communication status
     * @return CommStatus
     * */
    void ReadCommStatus();

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
