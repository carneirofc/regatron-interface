#pragma once

#include "Readings.hpp"
#include "Regatron.hpp"
#include "Version.hpp"

#include "fmt/format.h"
#include "serialiolib.h" // NOLINT
#include <iostream>
#include <memory>
#include <string>
#include <unistd.h>

namespace Regatron {

class Comm {
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

    void       InitializeDLL();
    CommStatus m_CommStatus;
    bool       m_AutoReconnect;

  public:
    explicit Comm(int port);
    Comm();
    ~Comm();
    void connect();
    void connect(int port);
    void connect(int fromPort, int toPort);
    void disconnect();

    CommStatus getCommStatus() const { return m_CommStatus; }
    bool       getAutoReconnect() const { return m_AutoReconnect; };
    void       setAutoReconnect(bool autoReconnect) {
        m_AutoReconnect = autoReconnect;
    }

    /** Regatron Readings */
    auto getReadings() {
        if (m_CommStatus != CommStatus::Ok) {
            throw CommException(m_CommStatus);
        }
        return m_readings;
    };
};

} // namespace Regatron
