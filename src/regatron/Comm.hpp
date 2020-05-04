#pragma once

#include <memory>
#include <unistd.h>

#include "IBC.hpp"
#include "Readings.hpp"
#include "Version.hpp"

namespace Regatron {

enum DllStatus {
    OK            = 0,
    DLL_FAILURE   = -1,
    COMM_ERROR    = -10,
    COMMAND_ERROR = -100
};

class Comm {
  private:
    // Connection
    int m_port        = 0;
    int m_portNrFound = -1;

    // Readings
    std::shared_ptr<Regatron::Readings> m_readings;

    // IBC
    std::shared_ptr<Regatron::IBC> m_ibc;

    // Increment (internal usage)
    double incDevVoltage    = 0.0;
    double incDevCurrent    = 0.0;
    double incDevPower      = 0.0;
    double incDevResistance = 0.0;
    double incSysVoltage    = 0.0;
    double incSysCurrent    = 0.0;
    double incSysPower      = 0.0;
    double incSysResistance = 0.0;

    DllStatus getDllStatus();

  public:
    explicit Comm(int port);
    Comm();
    ~Comm();
    void connect();
    void connect(int port);
    void connect(int fromPort, int toPort);
    bool isMaster();
    void moduleIDInfo();
    void readSystem();
    void readDevice();
    void readTemps();
    auto getReadings() { return m_readings; };
};

} // namespace Regatron
