#pragma once

#include "log/Logger.hpp"
#include "serialiolib.h"

namespace Regatron {

  constexpr unsigned int SYS_VALUES = 64;
  constexpr unsigned int MOD_VALUES = 0;

  class Readings{
    private:
      bool m_Q4Enable = false;
    /** One time readings */
      // Additional
      int m_DCLinkPhysNom;             // [V]
      int m_PrimaryCurrentPhysNom;     // [A]
      int m_TemperaturePhysNom;        // [°C]

      // System
      double m_SysVoltagePhysMax;     // [V]
      double m_SysCurrentPhysMax;     // [A]
      double m_SysPowerPhysMax;       // [kW]
      double m_SysResistancePhysMax;  // [mOhm]

      double m_SysVoltagePhysMin;     // [V]
      double m_SysCurrentPhysMin;     // [A]
      double m_SysPowerPhysMin;       // [kW]
      double m_SysResistancePhysMin;  // [mOhm]

      double m_SysVoltagePhysNom;     // [V]
      double m_SysCurrentPhysNom;     // [A]
      double m_SysPowerPhysNom;       // [kW]
      double m_SysResistancePhysNom;  // [mOhm]

      // -- Module 
      double m_ModVoltagePhysMax;     // [V]
      double m_ModCurrentPhysMax;     // [A]
      double m_ModPowerPhysMax;       // [kW]
      double m_ModResistancePhysMax;  // [mOhm]

      double m_ModVoltagePhysMin;     // [V]
      double m_ModCurrentPhysMin;     // [A]
      double m_ModPowerPhysMin;       // [kW]
      double m_ModResistancePhysMin;  // [mOhm]

      double m_ModVoltagePhysNom;     // [V]
      double m_ModCurrentPhysNom;     // [A]
      double m_ModPowerPhysNom;       // [kW]
      double m_ModResistancePhysNom;  // [mOhm]

    /** Monitor readings */
      // Additional
      double m_DCLinkVoltageMon;      // [V]
      double m_PrimaryCurrentMon;     // [A] Tranformer primary current
      double m_IGBTTempMon;           // [°C] heat sink of IGBT
      double m_RectifierTempMon;      // [°C] heat sink of rectifier
      double m_PCBTempMon;            // [°C] PCB Controller board temperature

      // System
      struct T_ErrorTree32* m_SysErrorTreeMon;
      struct T_ErrorTree32* m_SysWarningTreeMon;

      double m_SysActualOutVoltageMon;
      double m_SysActualOutPowerMon;
      double m_SysActualOutCurrentMon;
      double m_SysActualQ4CurrentMon;
      double m_SysActualResMon;


      // Module
      struct T_ErrorTree32* m_ModErrorTreeMon;
      struct T_ErrorTree32* m_ModWarningTreeMon;
      double m_ModActualOutVoltage;
      double m_ModActualOutPower;
      double m_ModActualOutCurrent;

      // Generic
      double m_DCLingVoltageMon;


      // -- Regatron Module
      unsigned int m_moduleID = 0;

    public:
        /** Getters and Setters */
        unsigned int getModuleID(){ return m_moduleID; }
        double getDCLinkVoltage(){ return m_DCLinkVoltageMon; }
        double getPrimaryCurrent(){ return m_PrimaryCurrentMon; }
        double getIGBTemp(){ return m_IGBTTempMon; }
        double getRectifierTemp(){ return m_RectifierTempMon; }

        /**
         * Read and convert to physical value DCLinkVoltage
         * @throw std::runtime_exception when dll fails
        */
        void readDCLinkVoltage(){
          int DCLinkVoltStd;

          if(TC4GetDCLinkDigital(&DCLinkVoltStd) != DLL_SUCCESS){
            throw std::runtime_error("failed to read DCLink digital voltage.");
          }
          m_DCLinkVoltageMon = (DCLinkVoltStd*m_DCLinkPhysNom)/4000.;
        }

        void readPrimaryCurrent(){
          int primaryCurrent;
          if(TC4GetIPrimDigital(&primaryCurrent) != DLL_SUCCESS){
            throw std::runtime_error("failed to read transformer primary current.");
          }
          m_PrimaryCurrentMon = (primaryCurrent*m_PrimaryCurrentPhysNom)/4000.;
        }

        /**
         * Read and convert IGBT, Rectifier and PCB temperatures.
         * @throw std::runtime_error
         */
        void readTemperature(){
          int igbtTemp, rectTemp;
          if(TC4GetTempDigital(&igbtTemp, &rectTemp) != DLL_SUCCESS){
            throw std::runtime_error("failed to read IGBT and Rectifier temperature.");
          }
          if(TC42GetTemperaturePCB(&m_PCBTempMon) != DLL_SUCCESS){
            throw std::runtime_error("failed to read PCB temperature.");
          }
          m_IGBTTempMon = (igbtTemp * m_TemperaturePhysNom)/4000.;
          m_RectifierTempMon = (rectTemp * m_TemperaturePhysNom)/4000.;
        }

        // @todo: Restrict read/write if master ...? Here or upper layer?
        bool isMaster();

        /** One time reading - Additional @throw std::runtime_exception */
        void readAdditionalPhys();
        /** One time reading - System @throw std::runtime_exception */
        void readSystemPhys();
        /** One time reading - Module @throw std::runtime_exception */
        void readModulePhys();
        /** One time reading - Module ID*/
        void readModuleID();

        /** Monitor Generi @throw: std::runtime_exception */
        void readGeneric();
        /** Monitor System @throw: std::runtime_exception */
        void readSystem();
        /** Monitor Module @throw: std::runtime_exception */ 
        void readModule();


    void selectSystem(){
      this->selectModule(SYS_VALUES);
    }
    void selectDevice(){
      this->selectModule(MOD_VALUES);
    }

    void selectModule(unsigned int module){
      if(TC4SetModuleSelector(module) != DLL_SUCCESS){
        throw std::runtime_error(fmt::format("failed to set module selector to {} (code {})", ((module==SYS_VALUES)?"system":"device"), module));
      }
    }
  };

}