#pragma once

namespace Regatron {


class IBC {
  private:
    // -- IBC Board related
    double         m_InvTemp               = 0;
    double         m_TransformerSecCurrent = 0;
    unsigned short m_State                 = 0;
    unsigned short m_FirmwareVersion       = 0;

  public:
    static constexpr unsigned int IBC_STATE_INVALID  = 0x0001;
    static constexpr unsigned int IBC_STATE_POWER_UP = 0x0004;
    static constexpr unsigned int IBC_STATE_READY    = 0x0010;
    static constexpr unsigned int IBC_STATE_RUNNING  = 0x0010;
    static constexpr unsigned int IBC_STATE_WARNING  = 0x0400;
    static constexpr unsigned int IBC_STATE_ERROR    = 0x1000;
    static constexpr unsigned int IBC_STATE_STOPPED  = 0x4000;
    static constexpr unsigned int IBC_STATE_UPDATE   = 0x8000;
};

} // namespace Regatron
