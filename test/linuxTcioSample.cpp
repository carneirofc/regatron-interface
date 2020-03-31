#include <iostream>
#include <unistd.h>
#include "serialiolib.h"

using namespace std;

int main() {
  int result = 0;
  int portNrFound = 0;
  unsigned int connectionType = 0;
  unsigned int version; //major and minor
  unsigned int build;
  char dllString[128];
  double incDevVoltage=0.0;
  double incDevCurrent=0.0;
  double incDevPower=0.0;
  double incDevResistance=0.0;
  double incSysVoltage=0.0;
  double incSysCurrent=0.0;
  double incSysPower=0.0;
  double incSysResistance=0.0;
  int voltagePhys;
  int currentPhys;
  int powerPhys;
  int resistancePhys;
  double voltage;
  double current;
  double power;
  double resitance;
  double randFactor;
  double randValue;

  //read out library version
  result = DllReadVersion(&version, &build, dllString);
  if (DLL_SUCCESS == result){
      cout << "Version:" << (version >> 16) << "." << (version & 0xff)<< " build:" << build << " dllString:" << dllString << endl;
  }
    else{
      cout << "DllReadVersion result:" << result << endl;
  }

  //loop connections: ttyS, ttyUSB, ttyDIGI
  for (connectionType = 0; connectionType < 3; connectionType++){
    voltage = 0.0;
    current = 0.0;
    power = 0.0;
    resitance = 0.0;

    //init connection
    result = DllInit();

    switch(connectionType){
      case 0:
        result = DllSetSearchDevice2ttyS();//search device connected via serial port -> /dev/ttyS0
        cout << endl << "new connection DllSetSearchDevice2ttyS" << endl << endl;
        DllSetCommTimeouts(40, 40); //use this function for VM or rs232 over ethernet
        break;
      case 1:
        result = DllSetSearchDevice2ttyUSB();//search device connected via USB to serial port adapter -> /dev/ttyUSB0
        cout << endl << "new connection DllSetSearchDevice2ttyUSB()" << endl << endl;
        break;
      case 2:
      default:
        result = DllSetSearchDevice2ttyDIGI();//search device connected via DIGI RealPort adapter -> /dev/ttyD00
        cout << endl << "new connection DllSetSearchDevice2ttyDIGI()" << endl << endl;
        break;
    }

    //search device
    if (DLL_SUCCESS == result){
      usleep((__useconds_t)(1000*1000*2));//hack: while eth and rs232 at the same tc device: wait 2 sec
      result = DllSearchDevice(1, 9, &portNrFound);
      cout << "result DllSearchDevice: " << result << "\tportNrFound: "<< portNrFound << endl;
    }

    //set remote control to RS232
    if (DLL_SUCCESS == result){
      result = TC4SetRemoteControlInput(2);
      if (DLL_SUCCESS != result)
      { cout << "result TC4SetRemoteControlInput: " << result << endl; }
    }

    //init lib
    if (DLL_SUCCESS == result){
      result = TC4GetPhysicalValuesIncrement(
        &incDevVoltage, &incDevCurrent,
        &incDevPower, &incDevResistance,
        &incSysVoltage, &incSysCurrent,
        &incSysPower, &incSysResistance);
      if (DLL_SUCCESS != result)
      { cout << "result TC4GetPhysicalValuesIncrement: " << result << endl; }
    }

    //get system physical values
    if (DLL_SUCCESS == result){
      result = TC4GetSystemPhysicalValues(&voltagePhys, &currentPhys, &powerPhys, &resistancePhys);
      if (DLL_SUCCESS != result){
        cout << "result TC4GetSystemPhysicalValues: " << result << endl;
      }
    }

   //get systemvalues
    if (DLL_SUCCESS == result){
      result = TC4SetModuleSelector(64);
      if (DLL_SUCCESS != result)
      { cout << "result TC4SetModuleSelector: " << result << endl; }
    }

    //also check sense use state if sense is in use with:
    // - TC4GetVoltageActSense(double *p_vactsense)
    // - TC4GetPowerActSense(double *p_pactsense)
    // - TC4GetSenseEnable(bool *p_senseenable)
    // - TC4SetSenseEnable(bool senseenable)

    //read out some values
    if (DLL_SUCCESS == result){
      cout << "result TC4GetVoltageAct: " << TC4GetVoltageAct(&voltage) << endl;
      cout << "result TC4GetCurrentAct: " << TC4GetCurrentAct(&current) << endl;
      cout << "result TC4GetPowerAct  : " << TC4GetPowerAct(&power) << endl;
      cout << "Voltage Act: " << voltage << endl;
      cout << "Current Act: " << current << endl;
      cout << "Power Act  : " << power << endl;
    }

    /*
    if (DLL_SUCCESS == result){
      randFactor = 0.5;

      randValue = randFactor * voltagePhys;
      cout << "result TC4SetVoltageRef("<< randValue <<"): " << TC4SetVoltageRef(randValue) << endl;
      randValue = randFactor * currentPhys;
      cout << "result TC4SetCurrentRef("<< randValue <<"): " << TC4SetCurrentRef(randValue) << endl;
      randValue = randFactor * powerPhys;
      cout << "result TC4SetPowerRef("<< randValue <<"): " << TC4SetPowerRef(randValue) << endl;
      randValue = randFactor * resistancePhys;
      cout << "result TC4SetResistanceRef("<< randValue <<"): " << TC4SetResistanceRef(randValue) << endl;
    }
    */

    //power on and of
    //TC4SetControlIn(1);
    //usleep(1000 * 1000);
    //TC4SetControlIn(0);

    //close connection
    result = DllClose();
    cout << "result DllClose: " << result << endl;
  }

  return 0;
}
