#pragma once

namespace Regatron {

#define IBC_STATE_INVALID  0x0001
#define IBC_STATE_POWER_UP 0x0004
#define IBC_SATE_READY     0x0010
#define IBC_STATE_RUNNING  0x010
#define IBC_STATE_WARNING  0x0400
#define IBC_STATE_ERROR    0x1000
#define IBC_STATE_STOPPED  0x4000
#define IBC_STATE_UPDATE   0x8000

  class IBC {
    private:
      // -- IBC Board related
      double m_invTemp = 0;
      double m_transformerSecCurrent = 0;
      unsigned short m_state = 0;
      unsigned short m_firmwareVer = 0;
  };

}