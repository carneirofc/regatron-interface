//tag: !Version!
/*!
 * @brief SERIALIOLIB.h : declarative part of communication library: tcio.dll, tcio_stdcall.dll or libtcio.so<br>
 * @details exports functionality for device control
 * @date 2001-2020
 * @version V3.88.00
 * @copyright Regatron AG, Switzerland
 */

//****************************************************************************
/*!
 * @mainpage tcio - The TopCon input and output ANSI-C language library
 */

//****************************************************************************
/*!
 * @defgroup Mandatory Mandatory
 * These functions must be called after connection to a device has been
 * established.
 * @attention Failing to do so can result in incorrect physical values.<br>
 * This may result in damage to an attached load,
 * e.g. if a higher voltage than intended is present at the output
 * (due to incorrect setting values)
 */

/*!
 * @defgroup Initialization Initialization
 * These functions are used for internally initalisiations of the API.<br>
 * Physical Values:<br>
 * will calculate internally used factors which other TC4-API-functions will use.
 * That's why this one must always be called after DLL-initialisation.<br>
 * Every value which is not given back as a physical value can be multiplicated
 * with the compatible factor. So you get from the standardised value
 * the physical value. (standardised belong to 4000).<br>
 * @attention Every time you connect to a (different) device this function must
 * be called again. Otherwise the physical values could be incorrect!<br>
 */

/*!
 * @defgroup Finalization Finalization
 * These functions are used for internally finalization of the API.
 */

/*!
 * @defgroup ReferenceValues Reference Values
 * Reference values for e.g. output voltage or output current.<br>
 * The name of the physical value is corresponding with the function name.
 */

/*!
 * @defgroup ActualValues Actual Values
 * This group delivers information about actual values like output voltage,
 * output current, internal temperatures, supply voltages
 * and other measured values.
 */

/*!
 * @defgroup DeviceInfo Device Info
 * This group contains e.g. physical device limits, firmware versions,
 * installed options and so on.
 */

/*!
 * @defgroup Status Status
 * Info about device state (ready, runnung, error, ...), pending error
 * and/or warning flags.
 */

/*!
 * @defgroup ControllerSettings Controller Settings
 * Values to read/change controller PID parameters and other controller
 * related settings.
 */

/*!
 * @defgroup Protection Protection
 * To protect each TopCon module from overvoltage and overcurrent has
 * limits setted for error and warning.
 * @attention Changing protection limits can cause serious damage to the modules.<br>
 * Do not change protection limits without contacting Regatron before!
 */
 
/*!
 * @defgroup IBCDeviceInfo IBC Device Info
 * Functions in this group can only be used if the device is a GSS 
 * device or a connection is made
 * directly to a IBC board (which the user normally does not have access to).
 */

/*!
 * @defgroup IBCActualValues IBC Actual Values
 * This group delivers information about actual values like output
 * voltage, output current, internal temperatures, supply voltages
 * and other measured values.
 */

/*!
 * @defgroup Generic Generic
 * Functions which have a complete control from something,
 * like ClearError, ControlOn, ...
 */

/*!
 * @defgroup SystemConfiguration System Configuration
 * These functions are used to configure TopCon for single-master
 * or multi-unit operation.
 */

/*!
 * @defgroup RemoteSystemConfiguration Remote System Configuration (RSC)
 * These functions are used to reconfigure a mulit unit system automatically
 * (e.g. Reconfigure a serial to a parallel multi unit system).<br>
 * For using the option remote system configuraton is a configuration file
 * (.ini file) and a switchbox required. In the configuration file are the
 * selectable system configurations saved. The switchbox changes the
 * connections on the power side.
 * @attention These functions can only be used on a TC.MAC or a master from
 * a master-slave system (not subsystem-master).
 */

/*!
 * @defgroup TFE TopCon Function Engine (TFE) and handling AAP-Slopes
 * These functions belong to the TopCon Function Engine
 * (internal function generator).
 */

/*!
 * @defgroup OptionCapSim Capacitor Simulation
 * These functions belong to the TopCon Option CapSim.
 */

 /*!
 * @defgroup watchdog Watchdog
 * The watchdog feature is introduced, because the TopCon device can not detect if the RS232 communication failed. <br>
 * If the watchdog is activated, it must receive a reset command at regular intervals, 
 * otherwise the internal watchdog timer runs out and the device is turned off with a watchdog error.<br>
 * These functions are used to get information about watchdog
 * and configure it.
 */
//****************************************************************************
#ifndef SERIAL_IO_LIB_H__D9D3E9B6_D8C3_4599_8ED3_72949486284F__INCLUDED
#define SERIAL_IO_LIB_H__D9D3E9B6_D8C3_4599_8ED3_72949486284F__INCLUDED

//****************************************************************************
#if defined(linux)
#define LINUX_OS
#ifndef PLATFORM_NAME
#define PLATFORM_NAME "Linux"
#endif //PLATFORM_NAME
#elif defined(WIN32)
#ifndef PLATFORM_NAME
#define PLATFORM_NAME "Windows"
#endif //PLATFORM_NAME
#else
#error plattform not supported
#endif //defined(linux)
#include <windows.h>
//****************************************************************************
#ifdef LINUX_OS
#ifndef FALSE
#define FALSE 0
#endif //FALSE
#ifndef TRUE
#define TRUE 1
#endif //TRUE

typedef int BOOL;
typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned long DWORD;
typedef void* HANDLE;
#endif //LINUX_OS

//****************************************************************************
//** return values for all functions                                         *
//****************************************************************************
#define DLL_SUCCESS   0
#define DLL_FAIL      -1
#define DLL_RET_T     int

#if defined(LINUX_OS)
#define DLL_RESULT int
#elif defined(WIN32)
#define DLL_EXPORT extern "C" __declspec( dllexport )
#define DLL_RESULT DLL_EXPORT DLL_RET_T
#else
#error plattform not supported
#endif //defined(LINUX_OS)

//****************************************************************************
#pragma pack(push,1)
struct T_commParam {
  BYTE TalkVersion;     // Version des Talk Protokolls
  BYTE AddrSize;        // Anzahl Bytes für Adress-Feld (nicht beachtet! =3)
  BYTE DataSize;        // Anzahl Bytes für Datenwort (nicht beachtet! =2)
  BYTE AddrIncrement;   // Adress-Inkrement für Datenworte
  BYTE TalkSize;        // maximale Länge eines Kommunikations-Frames
  WORD ProcType;        // Prozessor-Typ (0, wenn nicht definiert)
  WORD DevType;         // Device-Typ (0, wenn nicht definiert)
  WORD DevVer;          // Device-Version (0, wenn nicht definiert)
  WORD FlashType;       // Flash-Typ (0, wenn nicht definiert)
  DWORD FlashAddr;      // Basisadresse des Flash
  char Description[20]; // Beschreibender Text
};

struct T_ErrorTree
{
  WORD group;
  WORD error[16];
};

struct T_ErrorTree32
{
  unsigned long group;
  unsigned long error[32];
};

struct T_FnSeqHeader
{
  unsigned int TotalSize;
  unsigned int UserDefSize;
  unsigned int Version; // ab 3 mit T_FnSeqHeaderExtension
  unsigned int SeqNumber;
  char         SeqName[32];
  unsigned int DateYear;
  unsigned int DateMonth;
  unsigned int DateDay;
  unsigned int TimeHour;
  unsigned int TimeMinute;
  unsigned int TimeSecond;
  unsigned int SysNomVoltage;
  unsigned int SysMaxCurrent;
  unsigned int SysNomPower;
  //Reserved//8*16Bit
  unsigned int HeaderExtensionVersion;
  unsigned int HeaderExtensionSize;
};

struct T_FnSeqHeaderExtension //tag: change headerExtension
{
  unsigned int CurveID48;
  unsigned int CurveID32;
  unsigned int CurveID16;
  unsigned int CurveID0;
  BYTE         CurveIDValid;
  float        NominalIrradiance;
  float        NominalTemperature;
  float        MPPVoltage;
  float        MPPCurrent;
  float        OpenCircuitVoltage;
  float        ShortCircuitCurrent;
  unsigned int MathModel; //0-> AAP; 1->SAS
  unsigned int CellTechnology;
  BYTE         MPPValid;
};

struct T_FnSeq
{
  unsigned int StartTrigger;
  unsigned int EndAction;
  double       Delay;
  unsigned int Repeat;
  unsigned int EnabledFnBlocks;
  int          GeneralEnable;
};

struct T_FnBlock
{
  unsigned int BaseFunction;
  double       Amplitude;
  double       Offset;
  double       Symmetry;
  double       Frequency;
  int          BipolarAmplitude;
  int          RectifyAmplitude;
  double       ExpTimeConstant;
  unsigned int NumPeriods;
  unsigned int UserDefAmplitude;
  unsigned int UserDefTimePrescaler;
  double       UserDefPeriodLength;/*Gesamt-Periodendauer*/
  unsigned int UserDefNumPoints;
  unsigned int InactiveLevelType;
  double       InactiveLevel;
  unsigned int AAPInputType;
  unsigned int AAPInputFilterKoeff;
  unsigned int AAPFlags; /*ab FnSeqVersion 1, Alle Bits Reserviert für zukünftige Erweiterungen*/
  double       AAPInputScaling; /*ab FnSeqVersion 2: Werte = [0.125...10.0]*/
};

struct T_FnBlockUserData
{
  unsigned int TimeDelta[1000];
  int          Amplitude[1000];
};

struct T_ErrorHistoryEntry {
  unsigned long entryCounter;
  unsigned int  day;
  unsigned int  hour;
  unsigned int  minute;
  unsigned int  second;
  unsigned int  counter50us;
  unsigned int  group;
  unsigned int  detail;
  unsigned int  reserved[4];
  unsigned int  identifier;
};

#define FNG_CELL_TECHNOLOGY_RESERVE_SIZE  10

struct T_SASTechnologyParamsEN50530
{
  float FFv;
  float FFi;
  float CG;
  float CV;
  float CR;
  float Alpha;
  float Beta;
  float Nu_L2H;
  float Reserved[FNG_CELL_TECHNOLOGY_RESERVE_SIZE];
};

struct T_OfflineScopeData
{
  unsigned int Status[8];
  double       TimeStamp[8];
  double       Channel1[8];
  double       Channel2[8];
  double       Channel3[8];
  double       Channel4[8];
  double       Channel5[8];
  double       Channel6[8];
  double       Channel7[8];
  double       Channel8[8];
};

struct T_OfflineScopeChannels
{
  unsigned int NumOfChannel;
  unsigned int pChannel1;
  unsigned int pChannel2;
  unsigned int pChannel3;
  unsigned int pChannel4;
  unsigned int pChannel5;
  unsigned int pChannel6;
  unsigned int pChannel7;
  unsigned int pChannel8;
};

#pragma pack(pop)

typedef enum
{
  FNG_RESULT_BUSY = 1,
  FNG_RESULT_OK = 0,
  FNG_RESULT_FLASH_TIMEOUT = -1,
  FNG_RESULT_FLASH_NOT_EMPTY = -2,
  FNG_RESULT_FNSEQ_NOT_EXISTING = -3,
  FNG_RESULT_FLASH_FULL = -4,
  FNG_RESULT_ACTION_NOT_EXISTING = -5,
  FNG_RESULT_NO_FNSEQ_AVAILABLE = -6,
  FNG_RESULT_TCLIN_NOT_RESPONDING = -7,
  FNG_RESULT_WRONG_CONTEXT = -8,
  FNG_RESULT_WRONG_VERSION = -9,
  FNG_RESULT_DELETE_CONFIRM_TIMEOUT = -10,
  FNG_RESULT_DELETE_ALL_ERRROR = -11,
  FNG_RESULT_INVALID_HEADER = -12,

  FNG_RESULT_UNKNOWN = -99,
  FNG_RESULT_OVERALL_TIMEOUT = -100
}
T_FnSeqActionResult;

typedef enum
{
  DEV_TYPE_CTR3x =        0xFFFF,
  DEV_TYPE_CTR40 =        0x0064,
  DEV_TYPE_CTR42 =        0x0066,
  DEV_TYPE_TCIBC1 =       0x0514,
  DEV_TYPE_TCLIN_C =      0x00A0,
  DEV_TYPE_TCLIN_CTR2 =   0x00A1,
  DEV_TYPE_HMI5 =         0x0096
}T_DeviceType; //tag: new Device

#define DLL_VERSIONSTRING_COPY_LENGTH   64
#define DLL_DEBUG_BUFFER_LENGTH         (5*1024)
//****************************************************************************
//** generic High level commands                                             *
//****************************************************************************

/*!
 * @ingroup Initialization
 * @brief Initialises Driver/Objects used by the API. Allocates Memory.
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT DllInit();

/*!
 * @ingroup Finalization
 * @brief Closes Driver/Objects used by the API. Releases Memory.
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT DllClose();


#ifdef LINUX_OS
//onyl for linux os
//set search string example: "/dev/ttyS0", "/dev/ttyUSB0", "/dev/ttyD00"
//call before DllSearchDevice();
DLL_RESULT DllSetSearchDevice2ttyUSB();
DLL_RESULT DllSetSearchDevice2ttyS();
DLL_RESULT DllSetSearchDevice2ttyDIGI();
#endif //LINUX_OS

/*!
 * @ingroup Initialization
 * @brief Searches for TopCon device on available serial ports between
 * the given port numbers.<br>
 * If a device is found communication will be established automatically.<br>
 * To connect on a specific port number set fromPort equal to toPort.<br>
 * Usually the two serial ports on a PC have the numbers 1 and 2.
 * @param[in] fromPort port number from which the search starts [1...255]
 * @param[in] toPort port number where the search stops [1...255]
 * @param[out] pPortNrFound receiving the port number if device found otherwise unchanged
 * @note fromPort <= toPort<br>
 * After this function call succeds, call the mandatory functions before calling others.
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT DllSearchDevice(int fromPort, int toPort, int* pPortNrFound);

/*!
 * @brief aborts the function DllSearchDevice so that it returns before all
 * COM ports are searched
 * @ingroup Initialization
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT DllAbortDeviceSearch(void);

/*!
 * @brief shows the progress on search port range
 * @ingroup Initialization
 * @param[out] pComPort actual searched on port
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT DllGetDeviceSearchProgress(int* pComPort);

/*!
 * @ingroup Initialization
 * @brief Reads actual state of communication and DLL.<br>
 * This function reports the status of the low level interface (communication or communication commands).\n
 * If an error occur the communication might be interrupted or the actual command could not be executed.
 * The internal dll communication state is resetted.
 * A new initialisation is recommended.
 * This function does not report device faults or warnings!
 * @param[out] pState variable receiving actual state<br>
 * 0:    ok<br>
 * -10:  communication error<br>
 * -100: device reported command execution error
 * @param[out] pErrorNo reserved
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT DllGetStatus(int* pState, int* pErrorNo);

/*!
 * @ingroup Initialization
 * @brief Reads version of API
 * @param[out] pVersion version of API<br>
 * upper 16 Bit: Major Version number (xx.68.00)<br>
 * lower 16 Bit: Minor Version number (03.xx.00)
 * @param[out] pBuild build counter (03.68.xx)
 * @param[out] pString version string see \c DLL_VERSIONSTRING_COPY_LENGTH
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT DllReadVersion(unsigned int* pVersion, unsigned int* pBuild, char* pString);

// function DllCommDebug
// retrieves communication debug buffer
// buffer length see DLL_DEBUG_BUFFER_LENGTH
DLL_RESULT DllCommDebug(BYTE* pTxBuffer, int* pTxLen, BYTE* pRxBuffer, int* pRxLen);

// function DllCommDebugClear
// clears buffers for debug
DLL_RESULT DllCommDebugClear();

// function DllGetCommParam
DLL_RESULT DllGetCommParam(struct T_commParam* pCommParameter);

/*!
 * @ingroup Initialization
 * @brief
 * @param[out] pDeviceType device type identifier
 * @note DllSearchDevice must be called before<br>
 * see enum T_DeviceType
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT DllGetDeviceType(unsigned int* pDeviceType);

DLL_RESULT DllIsKnownDeviceType(unsigned int* pIsKnownDeviceType);

// function DllGetCommBaudrate
// read actual baudrate
DLL_RESULT DllGetCommBaudrate(int* pActBaudRate);

/*!
 * @ingroup Initialization
 * @brief Calling this function resets the communication parameters<br>
 * of the RS232 interface to default values.<br>
 * Read: 10, Write: 10.
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT DllSetDefaultCommTimeouts(void);

/*!
 * @ingroup Initialization
 * @brief This function allows to set the timeout multiplier of the RS232 interface.
 * @param[in] ReadTimeoutMultiplier timeout multiplier for READ operation<br>
 * range: [1...5000]
 * @param[in] WriteTimeoutMultiplier timeout multiplier for WRITE operation<br>
 * range: [1...5000]
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT DllSetCommTimeouts(unsigned int ReadTimeoutMultiplier, unsigned int WriteTimeoutMultiplier);

/*!
 * @ingroup Initialization
 * @brief This function returns the currently used read and write timeout multiplier.
 * @param[out] pReadTimeoutMultiplier timeout multiplier for READ operation
 * @param[out] pWriteTimeoutMultiplier timeout multiplier for WRITE operation
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT DllGetCommTimeouts(unsigned int* pReadTimeoutMultiplier, unsigned int* pWriteTimeoutMultiplier);

//tag: !DLLObserveFunctions!
//gets the nummber of TALK commands in queue
DLL_RESULT DllGetTalkQueueCount(unsigned int* pTalkQueueCount,
                                unsigned int* pTalkQueueSendBytesCount,
                                unsigned int* pTalkQueueReceiveBytesCount);

// function DllSetComPortFastDetection
// set fastDetection:
// 0: disable
// 1: enable
// faster DllSearchDevice
DLL_RESULT DllSetComPortFastDetection(unsigned int fastDetection);

//****************************************************************************
//** High level commands up to CTR V4.00                                     *
//****************************************************************************

/*!
 * @ingroup DeviceInfo
 * @brief This function returns software versions of two auxiliary DSP's and
 * the bootloader version for the MainDSP.
 * @param[out] pVersionPeripherieDSP Software version of peripherie-DSP (e.g. 10, read as 0.10)
 * @param[out] pVersionModulatorDSP Software version of modulator-DSP (e.g. 23, read as 0.23)
 * @param[out] pVersionBootloader Bootloader version for theMainDSP (e.g. 4, read as 0.04)
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC4GetPeripherieVersion(unsigned int* pVersionPeripherieDSP,
                                   unsigned int* pVersionModulatorDSP,
                                   unsigned int* pVersionBootloader);

//Software version of peripherie-DSP
DLL_RESULT TC4GetPDSPVersion(unsigned int* pVersion);

//****************************************************************************
//** High level commands up to CTR V4.20                                     * 
//****************************************************************************

// function TC4GetDeviceDSPID
// returns version information for DSP on device
DLL_RESULT TC4GetDeviceDSPID(unsigned int* pChipID, unsigned int* pChipRev, unsigned int* pChipSubID);

/*!
 * @ingroup DeviceInfo
 * @brief This function returns the firmware version of the MainDSP.
 * All API-functions beginning with "TC4..." require *pmain to be 4 or 11
 * A main Version of 11 indicates a client specific software based upon V4.x
 * @param[out] pMain Firmware main version
 * @param[out] pSub Firmware sub version
 * @param[out] pRevision Firmware revision
 * @note This function is not appropiate to determine whether firmware V4.x
 * or V2.x is installed as this function is
 * only available on firmware V4.x (and V11.x).To find out what type of
 * firmware (V4.x or V2.x) is installed
 * please see chapter 5.
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC4GetDeviceVersion(unsigned int* pMain, unsigned int* pSub, unsigned int* pRevision);

/*!
 * @ingroup ControllerSettings
 * @brief Enables and disables output voltage.
 * @param[in] voltageOn
 * 0: power unit off<br>
 * 1: power unit on
 * @pre RemoteControlInput must be set to RS232
 * @note calling this function on a TopCon Slave will have no effect
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC4SetControlIn(unsigned int voltageOn);

/*!
 * @ingroup ControllerSettings
 * @brief With this function you can determine whether output voltage is enabled or disabled.
 * @param[out] pVoltageOn Actual output voltage state
 * @note this function also gives correct values when the output voltage was enabled or
 * disabled via analog interface or HMI/HME.
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC4GetControlIn(unsigned int* pVoltageOn);

/*!
 * @ingroup Status
 * @brief returns the actual system- or module-state, depending on
 * whether this function is called on a TopCon Master
 * (returns system state) or Slave (returns module state of that Slave).
 * @param[out] pState Actual state<br>
 * 2:  POWERUP<br>
 * 4:  READY<br>
 * 8:  RUN<br>
 * 10: WARNING<br>
 * 12: ERROR<br>
 * 14: STOP
 * @note values and corresponding states have changed since firmware 2.x
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC4StateActSystem(unsigned int* pState);

/*!
 * @ingroup Generic
 * @brief This function is used to clear all pending errors and warnings.
 * @pre Remote control input must be set to RS232 (this API)
 * @note values and corresponding states have changed since firmware 2.x
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC4ClearError();

/*!
 * @ingroup Status
 * @brief This function reads the error group- and detail-codes into a
 * structure which is defined as follows:<br>
 * @code
 * struct T_ErrorTree
 * {
 *   WORD group;
 *   WORD error[16];
 * };
 * @endcode
 * All structure elements are 16bit wide and are aligned without memory gaps.
 * @param[out] pErrorTree T_ErrorTree-structure.
 * @note There are 16 possible error-groups. Each bit of the structure element
 * group represents one error-group. To each error-group a 16-bit detail-code
 * exists describing the error more precisely. The structure elements error[]
 * contain these detail-codes.
 * If bit 0 in group is set this means that error[0] contains a corresponding
 * detail-code<br>
 * If bit 1 in group is set this means that error[1] contains a corresponding
 * detail-code<br>
 * ...<br>
 * If bit 15 in group is set this means that error[15] contains a corresponding
 * detail-code<br>
 * The following table describes what error-index belongs to what
 * error-group:<br>
 * <table>
 * <tr><td> <b>structure index</b></td> <td> <b>Error group</b></td></tr>
 * <tr><td> error[0] </td><td> Internal</td></tr>
 * <tr><td> error[1] </td><td> Internal (PDSP)</td></tr>
 * <tr><td> error[2] </td><td> Output current </td></tr>
 * <tr><td> error[3] </td><td> Output voltage</td></tr>
 * <tr><td> error[4] </td><td> Supply</td></tr>
 * <tr><td> error[5] </td><td> Temperature</td></tr>
 * <tr><td> error[6] </td><td> Communication</td></tr>
 * <tr><td> error[7] </td><td> Internal (Modulator)</td></tr>
 * <tr><td> error[8] </td><td> Internal (AD-overrange 1)</td></tr>
 * <tr><td> error[9] </td><td> Internal (AD-overrange 2)</td></tr>
 * <tr><td> error[10] </td><td> Internal (AD-underrange 1)</td></tr>
 * <tr><td> error[11] </td><td> Internal (AD-underrange 2)</td></tr>
 * <tr><td> error[12] </td><td> Login</td></tr>
 * <tr><td> error[13] </td><td> Configuration</td></tr>
 * <tr><td> error[14] </td><td> Configuration 2</td></tr>
 * <tr><td> error[15] </td><td> Miscellaneous</td></tr>
 * </table>
 * <br>
 * @see The TopCon manual chapter Error handling, in which the detail-errors
 * has described.
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 * @deprecated use #TC4ReadErrorTree32 instead
 */
 DLL_RESULT TC4ReadErrorTree(struct T_ErrorTree* pErrorTree);

/*!
 * @ingroup Status
 * @brief This function reads the error group- and detail-codes into a
 * structure which is defined as follows:<br>
 * @code
 * struct T_ErrorTree32
 * {
 *   unsigned long group;
 *   unsigned long error[32];
 * }
 * @endcode
 * All structure elements are 32bit wide and are aligned without memory gaps.
 * @param[in] pErrorTree32 T_ErrorTree-structure.
 * @note There are 32 possible error-groups. Each bit of the structure element
 * group represents one error-group.  To each error-group a 32-bit detail-code
 * exists describing the error more precisely. The structure elements error[]
 * contain these detail-codes.<br>
 * If bit 0 in group is set this means that error[0] contains a corresponding
 * detail-code<br>
 * If bit 1 in group is set this means that error[1] contains a corresponding
 * detail-code<br>
 * ...<br>
 * If bit 31 in group is set this means that error[31] contains a corresponding
 * detail-code<br>
 * The following table describes what error-index belongs to what
 * error-group:<br>
 * <table>
 * <tr><td> <b>structure index</b></td> <td> <b>Error group</b></td></tr>
 * <tr><td> error[0] </td><td> Internal</td></tr>
 * <tr><td> error[1] </td><td> Internal (PDSP)</td></tr>
 * <tr><td> error[2] </td><td> Output current</td></tr>
 * <tr><td> error[3] </td><td> Output voltage</td></tr>
 * <tr><td> error[4] </td><td> Supply</td></tr>
 * <tr><td> error[5] </td><td> Temperature</td></tr>
 * <tr><td> error[6] </td><td> Communication</td></tr>
 * <tr><td> error[7] </td><td> Internal (Modulator)</td></tr>
 * <tr><td> error[8] </td><td> AD overrange 1</td></tr>
 * <tr><td> error[9] </td><td> AD overrange 2</td></tr>
 * <tr><td> error[10] </td><td> AD underrange 1</td></tr>
 * <tr><td> error[11] </td><td> AD underrange 2</td></tr>
 * <tr><td> error[12] </td><td> Login</td></tr>
 * <tr><td> error[13] </td><td> Configuration</td></tr>
 * <tr><td> error[14] </td><td> Configuration 2</td></tr>
 * <tr><td> error[15] </td><td> Miscellaneous</td></tr>
 * <tr><td> error[16] </td><td> IBC System</td></tr>
 * <tr><td> error[17] </td><td> IBC Supply</td></tr>
 * <tr><td> error[18] </td><td> IBC Communication</td></tr>
 * <tr><td> error[19] </td><td> IBC Power</td></tr>
 * <tr><td> error[20] </td><td> IBC Inverter</td></tr>
 * <tr><td> error[21] </td><td> IBC Miscellaneous</td></tr>
 * <tr><td> error[22] </td><td> IBC Inverter 2</td></tr>
 * <tr><td> error[23] </td><td> Not def.</td></tr>
 * <tr><td> error[24] </td><td> Not def.</td></tr>
 * <tr><td> error[25] </td><td> Not def.</td></tr>
 * <tr><td> error[26] </td><td> Not def.</td></tr>
 * <tr><td> error[27] </td><td> Login 2</td></tr>
 * <tr><td> error[28] </td><td> Configuration 3</td></tr>
 * <tr><td> error[29] </td><td> Communication 3</td></tr>
 * <tr><td> error[30] </td><td> Internal 2</td></tr>
 * <tr><td> error[31] </td><td> Communication 2</td></tr>
 * </table>
 * <br>
 * @see The TopCon manual chapter Error handling, in which the
 * detail-errors has described.
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC4ReadErrorTree32(struct T_ErrorTree32* pErrorTree32);

/*!
 * @ingroup Status
 * @brief This function reads the warning group- and detail-codes into a
 * structure which is defined as follows:<br>
 * @code
 * struct T_ErrorTree
 * {
 *   WORD group;
 *   WORD error[16];
 * };
 * @endcode
 * All structure elements are 16bit wide and are aligned without memory gaps.
 * @param[in] pWarnTree T_ErrorTree-structure.
 * @note There are 16 possible error-groups. Each bit of the structure element
 * group represents one error-group. To each warning-group a 16-bit detail-code
 * exists describing the error more precisely. The structure elements
 * error[] contain these detail-codes.<br>
 * If bit 0 in group is set this means that error[0] contains a corresponding
 * detail-code<br>
 * If bit 1 in group is set this means that error[1] contains a corresponding
 * detail-code<br>
 * ...<br>
 * If bit 15 in group is set this means that error[15] contains a corresponding
 * detail-code<br>
 * The following table describes what error-index belongs to what
 * warning-group:<br>
 * <table>
 * <tr><td> <b>structure index</b></td> <td> <b>Error group</b></td></tr>
 * <tr><td> error[0] </td><td> Internal</td></tr>
 * <tr><td> error[1] </td><td> Internal (PDSP)</td></tr>
 * <tr><td> error[2] </td><td> Output current</td></tr>
 * <tr><td> error[3] </td><td> Output voltage</td></tr>
 * <tr><td> error[4] </td><td> Supply</td></tr>
 * <tr><td> error[5] </td><td> Temperature</td></tr>
 * <tr><td> error[6] </td><td> Communication</td></tr>
 * <tr><td> error[7] </td><td> Internal (Modulator)</td></tr>
 * <tr><td> error[8] </td><td> AD overrange 1</td></tr>
 * <tr><td> error[9] </td><td> AD overrange 2</td></tr>
 * <tr><td> error[10] </td><td> AD underrange 1</td></tr>
 * <tr><td> error[11] </td><td> AD underrange 2</td></tr>
 * <tr><td> error[12] </td><td> Login</td></tr>
 * <tr><td> error[13] </td><td> Configuration</td></tr>
 * <tr><td> error[14] </td><td> Configuration 2</td></tr>
 * <tr><td> error[15] </td><td> Miscellaneous</td></tr>
 * </table>
 * <br>
 * @see The TopCon manual chapter Error handling, in which the detail-errors
 * has described.
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 * @deprecated use #TC4ReadWarningTree32 instead
 */
DLL_RESULT TC4ReadWarningTree(struct T_ErrorTree* pWarnTree);

/*!
 * @ingroup Status
 * @brief This function reads the warning group- and detail-codes into a
 * structure which is defined as follows:<br>
 * @code
 * struct T_ErrorTree32
 * {
 *   unsigned long group;
 *   unsigned long error[32];
 * }
 * @endcode
 * All structure elements are 32bit wide and are aligned without memory gaps.
 * @param[in] pWarnTree32 T_ErrorTree32-structure.
 * @note There are 32 possible warn-groups. Each bit of the structure element
 * group represents one error-group. To each warning-group a 32-bit detail-code
 * exists describing the error more precisely. The structure elements error[]
 * contain these detail-codes.<br>
 * If bit 0 in group is set this means that error[0] contains a corresponding
 * detail-code<br>
 * If bit 1 in group is set this means that error[1] contains a corresponding
 * detail-code<br>
 * ...<br>
 * If bit 31 in group is set this means that error[31] contains a corresponding
 * detail-code<br>
 * The following table describes what error-index belongs to what
 * warning-group:<br>
 * <table>
 * <tr><td> <b>structure index</b></td> <td> <b>Error group</b></td></tr>
 * <tr><td> error[0] </td><td> Internal</td></tr>
 * <tr><td> error[1] </td><td> Internal (PDSP)</td></tr>
 * <tr><td> error[2] </td><td> Output current</td></tr>
 * <tr><td> error[3] </td><td> Output voltage</td></tr>
 * <tr><td> error[4] </td><td> Supply</td></tr>
 * <tr><td> error[5] </td><td> Temperature</td></tr>
 * <tr><td> error[6] </td><td> Communication</td></tr>
 * <tr><td> error[7] </td><td> Internal (Modulator)</td></tr>
 * <tr><td> error[8] </td><td> AD overrange 1</td></tr>
 * <tr><td> error[9] </td><td> AD overrange 2</td></tr>
 * <tr><td> error[10] </td><td> AD underrange 1</td></tr>
 * <tr><td> error[11] </td><td> AD underrange 2</td></tr>
 * <tr><td> error[12] </td><td> Login</td></tr>
 * <tr><td> error[13] </td><td> Configuration</td></tr>
 * <tr><td> error[14] </td><td> Configuration 2</td></tr>
 * <tr><td> error[15] </td><td> Miscellaneous</td></tr>
 * <tr><td> error[16] </td><td> IBC System</td></tr>
 * <tr><td> error[17] </td><td> IBC Supply</td></tr>
 * <tr><td> error[18] </td><td> IBC Communication</td></tr>
 * <tr><td> error[19] </td><td> IBC Power</td></tr>
 * <tr><td> error[20] </td><td> IBC Inverter</td></tr>
 * <tr><td> error[21] </td><td> IBC Miscellaneous</td></tr>
 * <tr><td> error[22] </td><td> IBC Inverter 2</td></tr>
 * <tr><td> error[23] </td><td> Not def.</td></tr>
 * <tr><td> error[24] </td><td> Not def.</td></tr>
 * <tr><td> error[25] </td><td> Not def.</td></tr>
 * <tr><td> error[26] </td><td> Not def.</td></tr>
 * <tr><td> error[27] </td><td> Login 2</td></tr>
 * <tr><td> error[28] </td><td> Configuration 3</td></tr>
 * <tr><td> error[29] </td><td> Communication 3</td></tr>
 * <tr><td> error[30] </td><td> Internal 2</td></tr>
 * <tr><td> error[31] </td><td> Communication 2</td></tr>
 * </table>
 * <br>
 * @see The TopCon manual chapter Error handling, in which the detail-errors
 * has described.
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC4ReadWarningTree32(struct T_ErrorTree32* pWarnTree32);

/*!
 * @ingroup Mandatory
 * @brief Gets the factors to calculate the physical values intern in the API.<br>
 * <b>This function has to be called at least once after DLL-initialisation.</b> It 
 * not only returns device information it also calculates important values that
 * will be used from API-functions working with  physical values.<br>
 * Every time you connect to a other module you must call
 * TC4GetPhysicalValuesIncrement again as physical
 * module values could be different. This must also be performed when changing
 * connection from a master to a slave or vice versa.<br>
 * After calling this function be sure to <b>also call API-function
 * #TC4SetModuleSelector with the selector-argument set to 64</b>. Otherwise in
 * multi-unit operation some API-functions called on TopCon Master will return
 * module-values instead of system-values.<br>
 * This API-function can be used to translate between stardardised and physical
 * values.<br>
 * No translation-factors for temperatur, dc-link voltage and primary current
 * are delivered by this API-function.
 * @param[out] pIncModV increment factor module voltage
 * @param[out] pIncModC increment factor module current
 * @param[out] pIncModP increment factor module power
 * @param[out] pIncModR increment factor module resistance
 * @param[out] pIncSysV increment factor system voltage
 * @param[out] pIncSysC increment factor system current
 * @param[out] pIncSysP increment factor system power
 * @param[out] pIncSysR increment factor system resistance
 * @note PhysicalModuleVoltage = FactorModuleVoltage * StandardisedModuleVoltage
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC4GetPhysicalValuesIncrement(double* pIncModV, double* pIncModC,
                                         double* pIncModP, double* pIncModR,
                                         double* pIncSysV, double* pIncSysC,
                                         double* pIncSysP, double* pIncSysR);

/*!
 * @ingroup Mandatory
 * @brief Set the ModuleSelector to 64. This ensures that in multi-unit operation
 * a TopCon Master will return system-values.
 * @param[in] selector
 * 64: for getting system-values<br>
 * 0: getting device-values<br>
 * [1...63]: getting slave-values<br>
 * @note To take the values from functions for example:
 * - #TC4GetVoltageAct(...)<br>
 * - #TC4GetCurrentAct(...)<br>
 * - #TC4ReadErrorTree32(...)<br>
 * - ...
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC4SetModuleSelector(unsigned int selector);

/*!
 * @ingroup DeviceInfo
 * @brief This function is used in multi-unit operation to translate between
 * <b>standardised</b> system-values and 
 * physical system-values. Only use this function on a TopCon Master (a TopCon
 * Slave will return false results)
 * @param[out] pVLimitMod Nominal output voltage [V] of that module
 * @param[out] pILimitMod Maximum output current [A] of that module
 * @param[out] pPLimitMod Nominal output power [kW] of that module
 * @param[out] pRLimitMod Nominal simulated resistance [mOhm] of that module
 * @param[out] pVLimitSys Nominal output voltage [V] of the whole system
 * @param[out] pILimitSys Maximum output current [A]of the whole system
 * @param[out] pPLimitSys Nominal output power [kW] of the whole system
 * @param[out] pRLimitSys Nominal simulated resistance [mOhm] of the whole system.
 * @note Calling this function on a TopCon Slave will return 0 in system-limit
 * values (parameters [5...8]) calling this function on a single TopCon Master
 * returns equal values for module- and system-values (the master represents
 * the system)
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 * @deprecated use<br>
 * #TC4GetModulePhysicalLimitMax<br>
 * or<br>
 * #TC4GetSystemPhysicalLimitMax<br>
 * instead
 */
DLL_RESULT TC4GetPhysicalValuesLimits(double* pVLimitMod, double* pILimitMod,
                                      double* pPLimitMod, double* pRLimitMod,
                                      double* pVLimitSys, double* pILimitSys,
                                      double* pPLimitSys, double* pRLimitSys);

/*!
 * @ingroup DeviceInfo
 * @brief Gets the physical <b>min</b> values from <b>module</b>.
 * @param[out] pVPhysMin PhysicalMinVoltage [V]
 * @param[out] pIPhysMin PhysicalMinCurrent [A]
 * @param[out] pPPhysMin PhysicalMinPower [kW]
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 * @deprecated use #TC4GetModulePhysicalLimitMin instead
 */
DLL_RESULT TC4GetDevicePhysicalMinValues(int* pVPhysMin, int* pIPhysMin, int* pPPhysMin);

/*!
 * @ingroup DeviceInfo
 * @brief Gets the physical <b>min</b> values from <b>system</b>.
 * @param[out] pVPhysMin PhysicalMinVoltage [V]
 * @param[out] pIPhysmin PhysicalMinCurrent [A]
 * @param[out] pPPhysMin PhysicalMinPower [kW]
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 * @deprecated use #TC4GetSystemPhysicalLimitMin instead
 */
DLL_RESULT TC4GetSystemPhysicalMinValues(int* pVPhysMin, int* pIPhysmin, int* pPPhysMin);

/*!
 * @ingroup DeviceInfo
 * @brief Gets the physical <b>min</b> values from <b>module</b>.
 * @param[out] pVoltagePhysMin [V]
 * @param[out] pCurrentPhysMin [A]
 * @param[out] pPowerPhysMin [kW]
 * @param[out] pResistancePhysMin [mOhm]
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC4GetModulePhysicalLimitMin(double* pVoltagePhysMin, double* pCurrentPhysMin,
                                        double* pPowerPhysMin, double* pResistancePhysMin);

/*!
 * @ingroup DeviceInfo
 * @brief Gets the physical <b>max</b> values from <b>module</b>.
 * @param[out] pVoltagePhysMax [V]
 * @param[out] pCurrentPhysMax [A]
 * @param[out] pPowerPhysMax [kW]
 * @param[out] pResistancePhysMax [mOhm]
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC4GetModulePhysicalLimitMax(double* pVoltagePhysMax, double* pCurrentPhysMax,
                                        double* pPowerPhysMax, double* pResistancePhysMax);

/*!
 * @ingroup DeviceInfo
 * @brief Gets the physical <b>nominal</b> values from <b>module</b>.
 * These are the same values as from #TC4GetModulePhysicalValues but from <b>cache</b>
 * @param[out] pVoltagePhysNom [V]
 * @param[out] pCurrentPhysNom [A]
 * @param[out] pPowerPhysNom [kW]
 * @param[out] pResistancePhysNom [mOhm]
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC4GetModulePhysicalLimitNom(double* pVoltagePhysNom, double* pCurrentPhysNom,
                                        double* pPowerPhysNom, double* pResistancePhysNom);

/*!
 * @ingroup DeviceInfo
 * @brief Gets the physical <b>minimal</b> values from <b>system</b>.
 * @param[out] pVoltagePhysMin [V]
 * @param[out] pCurrentPhysMin [A]
 * @param[out] pPowerPhysMin [kW]
 * @param[out] pResistancePhysMin [mOhm]
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC4GetSystemPhysicalLimitMin(double* pVoltagePhysMin, double* pCurrentPhysMin,
                                        double* pPowerPhysMin, double* pResistancePhysMin);

/*!
 * @ingroup DeviceInfo
 * @brief Gets the physical <b>maximal</b> values from <b>system</b>.
 * @param[out] pVoltagePhysMax [V]
 * @param[out] pCurrentPhysMax [A]
 * @param[out] pPowerPhysMax [kW]
 * @param[out] pResistancePhysMax [mOhm]
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC4GetSystemPhysicalLimitMax(double* pVoltagePhysMax, double* pCurrentPhysMax,
                                        double* pPowerPhysMax, double* pResistancePhysMax);

/*!
 * @ingroup DeviceInfo
 * @brief Gets the physical <b>nominal</b> values from <b>system</b>.
 * These are the same values as from #TC4GetSystemPhysicalValues but from <b>cache</b>
 * @param[out] pVoltagePhysNom standardised module voltage [V]
 * @param[out] pCurrentPhysNom standardised module current [A]
 * @param[out] pPowerPhysNom standardised module power [kW]
 * @param[out] pResistancePhysNom standardised simulated module resistance [mOhm]
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC4GetSystemPhysicalLimitNom(double* pVoltagePhysNom, double* pCurrentPhysNom,
                                        double* pPowerPhysNom, double* pResistancePhysNom);

/*!
 * @ingroup DeviceInfo
 * @brief get the <b>standardised</b> physical values from the TopCon <b>module</b>.
 * This is not the same as the physical limit max values from module.
 * @param[out] pVoltagePhysNom standardised module voltage [V]
 * @param[out] pCurrentPhysNom standardised module current [A]
 * @param[out] pPowerPhysNom standardised module power [kW]
 * @param[out] pResistancePhysNom standardised simulated module resistance [mOhm]
 * @note see also #TC4GetPhysicalValues
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 * @deprecated use function #TC4GetModulePhysicalLimitNom
 */
DLL_RESULT TC4GetModulePhysicalValues(int* pVoltagePhysNom, int* pCurrentPhysNom,
                                      int* pPowerPhysNom, int* pResistancePhysNom);

/*!
 * @ingroup DeviceInfo
 * @brief get the <b>standardised</b> physical values from the TopCon <b>module</b>.
 * This is not the same as the physical limit max values from module.
 * @param[out] pDCLinkPhysNom standardised voltageDCLink [V]
 * @param[out] pPrimaryCurrentPhysNom standardised primaryCurrent [A]
 * @param[out] pTemperaturePhysNom standardised temperature [°C]
 * @note see also #TC4GetPhysicalValues
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC4GetAdditionalPhysicalValues(int* pDCLinkPhysNom,
                                          int* pPrimaryCurrentPhysNom,
                                          int* pTemperaturePhysNom);

/*!
 * @ingroup DeviceInfo
 * @brief get the <b>standardised</b> physical values from the TopCon <b>module</b>.
 * This is not the same as the physical limit max values from module.
 * @param[out] pVoltagePhysNom standardised module voltage [V]
 * @param[out] pCurrentPhysNom standardised module current [A]
 * @param[out] pPowerPhysNom standardised module power [kW]
 * @param[out] pResistancePhysNom standardised simulated module resistance [mOhm]
 * @param[out] pDCLinkPhysNom standardised voltageDCLink [V]
 * @param[out] pPrimaryCurrentPhysNom standardised primaryCurrent [A]
 * @param[out] pTemperaturePhysNom standardised temperature [°C]
 * @note The PhysicalSystemResistance is calculated and not messured.<br>
 * If you change the configuration, also restart this function to get the
 * correct values.<br>
 * E.g.:<br>
 * Suppose we have three TopCon modules, each with the following nominal data:<br>
 * Unom = 60V, Inom = 333A, Pnom = 16kW<br>
 * <br>
 * when connecting them in series we get a system with the following nominal data:<br>
 * Unom = 3x60V=180V, Inom = 333A, Pnom = 3x16kW = 48kW<br>
 * <br>
 * When we use the API-function TC4SetVoltageRef to set a desired voltage set
 * value of, lets say 90V, we set the system-value. Each module will deliver
 * its module-value of 30V.
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 * @deprecated use splited functions #TC4GetModulePhysicalLimitNom and #TC4GetAdditionalPhysicalValues instead
 */
DLL_RESULT TC4GetPhysicalValues(int* pVoltagePhysNom, int* pCurrentPhysNom,
                                int* pPowerPhysNom, int* pResistancePhysNom,
                                int* pDCLinkPhysNom, int* pPrimaryCurrentPhysNom,
                                int* pTemperaturePhysNom);

/*!
 * @ingroup DeviceInfo
 * @brief This function is used in multi-unit operation to translate between
 * <b>standardised</b> system-values and physical system-values. Only use this
 * function on a TopCon Master (a TopCon Slave will return false results)
 * This is not the same as the physical limit max values from system.
 * @param[out] pVoltagePhysNom standardised system voltage [V]
 * @param[out] pCurrentPhysNom standardised system current[A]
 * @param[out] pPowerPhysNom standardised system power [kW]
 * @param[out] pResistancePhysNom standardised simulated system resistance [mOhm]
 * @note Calling this function on a TopCon Master without any Slaves connected
 * will return the same physical values as when calling TC4GetPhysicalValues
 * (the master represents the whole system).
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 * @deprecated use function #TC4GetSystemPhysicalLimitNom
 */
DLL_RESULT TC4GetSystemPhysicalValues(int* pVoltagePhysNom, int* pCurrentPhysNom,
                                      int* pPowerPhysNom, int* pResistancePhysNom);

// function TC4GetSypplyPhysicalValues
// reads physical values from device supply's
// vphys.. physical value of voltage
// iphys.. physical value of current
// pphys.. physical value of power
// rphys.. physical value of resistance
DLL_RESULT TC4GetSupplyPhysicalValues(int* v5phys, int* v15pphys, int* v15nphys, int* v24phys);

/*!
 * @ingroup Generic
 * @brief
 * Reads the actual remote control input
 * @param[out] pRemoteInput
 * 0:     set active interface to analog<br>
 * 1:     set active interface to HMI/HME<br>
 * 2:     set active interface to RS232 (this API)<br>
 * 32767: no active interface will be selected<br>
 * @note Reading remote control input on a TopCon Slave can return any of the
 * above values, but has no significance.
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC4GetRemoteControlInput(unsigned int* pRemoteInput);

/*!
 * @ingroup Generic
 * @brief Sets the remote control input which defines the active interface.
 * @param[in] remoteInput
 * 0:     set active interface to analog<br>
 * 1:     set active interface to HMI/HME<br>
 * 2:     set active interface to RS232 (this API)<br>
 * 32767: no active interface will be selected<br>
 * @note calling this function on a TopCon Slave will have no effect.
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC4SetRemoteControlInput(unsigned int remoteInput);

// function to set enable/disable exclusive control over RS232
DLL_RESULT TC4SetRS232ExclusiveControl(unsigned int enableExclusiveControl);

/*!
 * @ingroup ActualValues
 * @brief
 * @param[out] pVoltageAct Actual voltage value [V]
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC4GetVoltageAct(double* pVoltageAct);

/*!
 * @ingroup ActualValues
 * @brief
 * @param[out] pCurrentAct Actual current value [A]
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC4GetCurrentAct(double* pCurrentAct);

/*!
 * @ingroup ActualValues
 * @brief
 * @param[out] pPowerAct Actual power value [kW]
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC4GetPowerAct(double* pPowerAct);

/*!
 * @ingroup ActualValues
 * @brief
 * @param[out] pResistanceAct Actual simulated resistance value
 * @note resistance is not measured - this will just read the set value
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC4GetResistanceAct(double* pResistanceAct);

/*!
 * @ingroup ReferenceValues
 * @brief
 * @param[out] pVoltageRef PhysicalVoltage [V]
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC4GetVoltageRef(double* pVoltageRef);

/*!
 * @ingroup ReferenceValues
 * @brief
 * @param[out] pCurrentRef PhysicalCurrent [A]
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC4GetCurrentRef(double* pCurrentRef);

/*!
 * @ingroup ReferenceValues
 * @brief
 * @param[out] pPowerRef PhysicalPower [kW]
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC4GetPowerRef(double* pPowerRef);

/*!
 * @ingroup ReferenceValues
 * @brief
 * @param[out] pResistanceRef PhysicalResistance [mOhm]
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC4GetResistanceRef(double* pResistanceRef);

/*!
 * @ingroup ReferenceValues
 * @brief
 * @param[in] voltageRef PhysicalVoltage [V]
 * @pre Remote control input must be set to RS232
 * @note Calling these functions on a TopCon Slave will have no effect.
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC4SetVoltageRef(double voltageRef);

/*!
 * @ingroup ReferenceValues
 * @brief
 * @param[in] currentRef PhysicalCurrent [A]
 * @pre Remote control input must be set to RS232
 * @note Calling these functions on a TopCon Slave will have no effect.
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC4SetCurrentRef(double currentRef);

/*!
 * @ingroup ReferenceValues
 * @brief
 * @param[in] powerRef PhysicalPower [kW]
 * @pre Remote control input must be set to RS232
 * @note Calling these functions on a TopCon Slave will have no effect.
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC4SetPowerRef(double powerRef);

/*!
 * @ingroup ReferenceValues
 * @brief
 * @param[in] resistanceRef PhysicalResistance [mOhm]
 * @pre Remote control input must be set to RS232
 * @note Calling these functions on a TopCon Slave will have no effect.
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC4SetResistanceRef(double resistanceRef);

/*!
 * @ingroup Generic
 * @brief Reads the actual mode of the controller.<br>
 * Reading controller mode on a TopCon Slave returns the actual controller mode of that Slave.<br>
 * Reading controller mode on a TopCon Master returns the actual system control mode.
 * @param[out] pControlMode
 * 0: no controller selected (output voltage is disabled)<br>
 * 1: constant voltage<br>
 * 2: constant current<br>
 * 4: constant power<br>
 * @note In a multiload system there is no system control mode defined and the result will be 0.
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC4GetControlMode(unsigned int* pControlMode);

/*!
 * @ingroup Generic
 * @brief Stores all settings in a non-volatile memory 
 * If TopCon Slaves are connected to the TopCon Master, the Slaves will store
 * their settings in non-volatile memory too.
 * @note calling this function on a TopCon Slave will have no effect
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC4StoreParameters();

// function TC4GetTemperature
// gets actual surface temperature from device
DLL_RESULT TC4GetTemperature(double* pTemperature);

/*!
 * @ingroup DeviceInfo
 * @brief Reads the ModuleID which is set by the ID-selector (AH and AL)
 * on the front panel of each module.<br>
 * The ModuleID is calculated as follows: ModuleID = (16*AH) + AL
 * @param *pModuleId Module ID
 * @note By definition a TopCon Master always has ModuleID 0 (zero).
 * Don't confuse it with the ModuleSelector.
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC4GetModuleID(unsigned int* pModuleId);

/*!
 * @ingroup ControllerSettings
 * @brief Reads the actual controller parameters of the voltage
 * @param[out] pPGain Actual P-gain
 * @param[out] pIGain Actual I-gain
 * @param[out] pDGain Actual D-gain
 * @param[out] pFeed Actual feed-forward-gain
 * @param[out] pTime1 Actual time delay for D-gain
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC4GetVoltageControlSettings(unsigned int* pPGain, unsigned int* pIGain,
                                        unsigned int* pDGain, unsigned int* pFeed,
                                        unsigned int* pTime1);

/*!
 * @ingroup ControllerSettings
 * @brief
 * Reads the actual controller parameters of the current
 * @param[out] pPGain Actual P-gain
 * @param[out] pIGain Actual I-gain
 * @param[out] pDGain Actual D-gain
 * @param[out] pFeed Actual feed-forward-gain
 * @param[out] pTime1 Actual time delay for D-gain
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC4GetCurrentControlSettings(unsigned int* pPGain, unsigned int* pIGain,
                                        unsigned int* pDGain, unsigned int* pFeed,
                                        unsigned int* pTime1);

/*!
 * @ingroup ControllerSettings
 * @brief Reads the actual controller parameters of the voltage
 * @param[out] pPGain Actual P-gain
 * @param[out] pIGain Actual I-gain
 * @note This is only a PI-controller.
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC4GetPowerControlSettings(unsigned int* pPGain, unsigned int* pIGain);

/*!
 * @ingroup ControllerSettings
 * @brief Sets controller parameters voltage
 * @param[in] pGain P-gain [0...32767]
 * @param[in] iGain I-gain [0...32767]
 * @param[in] dGain D-gain [0...32767]
 * @param[in] feed Feed-forward-gain [0...32767]
 * @param[in] time1 Time delay for D-gain [0...16383]
 * @note Setting controller parameters on a TopCon Master will automatically
 * transfers them to all connected Slaves.<br>
 * Setting controller parameters on a TopCon Slave will have no effect.
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC4SetVoltageControlSettings(unsigned int pGain, unsigned int iGain,
                                        unsigned int dGain, unsigned int feed,
                                        unsigned int time1);

/*!
 * @ingroup ControllerSettings
 * @brief Sets controller parameters current
 * @param[in] pGain P-gain [0...32767]
 * @param[in] iGain I-gain [0...32767]
 * @param[in] dGain D-gain [0...32767]
 * @param[in] feed Feed-forward-gain [0...32767]
 * @param[in] time1 Time delay for D-gain [0...16383]
 * @note Setting controller parameters on a TopCon Master will automatically
 * transfers them to all connected Slaves.<br>
 * Setting controller parameters on a TopCon Slave will have no effect.
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC4SetCurrentControlSettings(unsigned int pGain, unsigned int iGain,
                                        unsigned int dGain, unsigned int feed,
                                        unsigned int time1);

/*!
 * @ingroup ControllerSettings
 * @brief
 * Sets controller parameters power
 * @param[in] pGain P-gain [0...32767]
 * @param[in] iGain I-gain [0...32767]
 * @note Setting controller parameters on a TopCon Master will automatically
 * transfers them to all connected Slaves.<br>
 * Setting controller parameters on a TopCon Slave will have no effect.
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC4SetPowerControlSettings(unsigned int pGain, unsigned int iGain);

/*!
 * @ingroup Protection
 * @brief These function read the actual protection limits for output-overvoltage.
 * @param[out] pLimit Actual overvoltage error level
 * @param[out] pLimitDelay Actual delay for the error level in [50us]
 * @param[out] pWarn Actual overvoltage warning level
 * @param[out] pWarnDelay Actual delay for the warning level in [50us]
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC4GetOvervoltageParam(double* pLimit, unsigned int* pLimitDelay,
                                  double* pWarn, unsigned int* pWarnDelay);

/*!
 * @ingroup Protection
 * @brief These functions set the protection limits for overvoltage on the output stage.
 * Maximum allowed physical value for overvoltage-limit see API-function #TC4GetMaximas.
 * @param[in] limit overvoltage error level
 * @param[in] limitDelay delay for error level in [50us]
 * @param[in] warn overvoltage warning level
 * @param[in] warnDelay delay for warning level in [50us]
 * @pre Remote control input must be set to RS232 (this API)
 * @note Setting overvoltage parameters on a TopCon Master will automatically
 * transfers them to all connected Slaves.<br>
 * Setting overvoltage parameters on a TopCon Slave will have no effect.
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC4SetOvervoltageParam(double limit, unsigned int limitDelay,
                                  double warn, unsigned int warnDelay);

/*!
 * @ingroup Protection
 * @brief These function read the actual protection limits for output-overcurrent.
 * @param[out] pLimit Actual overcurrent error level
 * @param[out] pLimitDelay Actual delay for the error level in [50us]
 * @param[out] pWarn Actual overcurrent warning level
 * @param[out] pWarnDelay Actual delay for the warning level in [50us]
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */                          
DLL_RESULT TC4GetOvercurrentParam(double* pLimit, unsigned int* pLimitDelay,
                                  double* pWarn, unsigned int* pWarnDelay);
/*!
 * @ingroup Protection
 * @brief These functions set the protection limits for overcurrent on the output stage.
 * Maximum allowed physical value for overcurrent-limit see API-function #TC4GetMaximas.
 * @param[in] limit overcurrent error level
 * @param[in] limitDelay delay for error level in [50us]
 * @param[in] warn overcurrent warning level
 * @param[in] warnDelay delay for warning level in [50us]
 * @pre Remote control input must be set to RS232 (this API)
 * Setting overcurrent parameters on a TopCon Slave will have no effect.
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC4SetOvercurrentParam(double limit, unsigned int limitDelay,
                                  double warn, unsigned int warnDelay);


/*!
* @ingroup Protection
* @brief This function is used to get the maximum allowed nominal current, error
* and warn limits of the i2t monitoring.
* @param[out] pI2tMaxInom Maximum allowed nominal current
* @param[out] pI2tMaxLimit Maximum allowed error and warn limit
* @attention These values are not physical values - so translate to correct
* physical values
* @note Setting protection limits on a single master unit: treat pI2tMaxInom and
* pI2tMaxLimit as module-values.<br>
* Setting protection limits in multi-unit operation treat pI2tMaxInom and pI2tMaxLimit
* as system-values.
* @see chapter Translate between standardised values and physical values
* @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
*/
DLL_RESULT TC4GetI2tAbsLimits(unsigned short *pI2tMaxInom, unsigned short *pI2tMaxLimit);


/*!
 * @ingroup Protection
 * @brief This function reads the actual settings for I^2t-monitoring on output current.
 * @param[out] pI2tCurrent Actual nominal current [A]
 * @param[out] pLimit Actual I^2t error-limit
 * @param[out] pWarn Actual I^2t warning-limit
 * @see chapter Translate between standardised values and physical values
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC4GetI2tCurrentParam(double* pI2tCurrent, unsigned int* pLimit, unsigned int* pWarn);

/*!
 * @ingroup Protection
 * @brief This function sets parameters for I^2t-monitoring on output-current.
 * Maximum allowed physical value for i2tcurrent see API-function #TC4GetI2tAbsLimits.
 * @param[in] i2tCurrent Nominal current [A]
 * @param[in] limit Error-limit for I^2t
 * @param[in] warn Warning limit for I^2t
 * @pre Remote control input must be set to RS232 (this API)
 * @note Setting I2t-parameters on a TopCon Master will automatically transfers
 * them to all connected Slaves.<br>
 * Setting I2t-parameters on a TopCon Slave will have no effect.
 * @see chapter Translate between standardised values and physical values
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC4SetI2tCurrentParam(double i2tCurrent, unsigned int limit, unsigned int warn);

// function TC4GetAnalogInDigitalValues
// gets raw values of analog reference inputs
DLL_RESULT TC4GetAnalogInDigitalValues(int* pVoltage, int* pCurrent, int* pPower, int* pResistance);

// function GetX105AnalogInPin__(2, 3, 14, 15)
// gets 0-10 voltage values of analog reference inputs
DLL_RESULT TC4GetX105Pin2AnalogIn(double* pVoltage);
DLL_RESULT TC4GetX105Pin3AnalogIn(double* pVoltage);
DLL_RESULT TC4GetX105Pin14AnalogIn(double* pVoltage);
DLL_RESULT TC4GetX105Pin15AnalogIn(double* pVoltage);

// function TC4GetMeasurementDigitalValues
// gets raw values of measurement inputs
DLL_RESULT TC4GetMeasurementDigitalValues(int* pVActDigital, int* pIActDigital);

// function TC4GetDigitalInputs
// gets digital input information (direct and latch)
// appVoltageOn = pDirect & (0x01 << 0);
// interlock    = pDirect & (0x01 << 1);
//
// appDigitalIn1  = pLatch & (0x01 << 1);
// appDigitalIn2  = pLatch & (0x01 << 2);
// appDigitalIn3  = pLatch & (0x01 << 3);
// appDigitalIn4  = pLatch & (0x01 << 4);
// securityRelais = pLatch & (0x01 << 5)
DLL_RESULT TC4GetDigitalInputs(unsigned int* pDirect, unsigned int* pLatch);

/*!
 * @ingroup ActualValues
 * @brief This function reads the actual heat sink temperature of IGBT and rectifier.
 * @param[out] pIgbtTemp Actual IGBT heat sink temperature
 * @param[out] pRectifierTemp Actual rectifier heat sink temperature
 * @attention These values are not physical values - so translate to correct
 * physical values.
 * @see chapter Translate between standardised values and physical values
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC4GetTempDigital(int* pIgbtTemp, int* pRectifierTemp);

// function TC4GetCaseInsideTempEnabled
DLL_RESULT TC4GetCaseInsideTempEnabled(unsigned int* pValue);

// function TC4GetCaseInsideTempDigital
// gets digital values for caseInside Temperature and caseInside filtered Temperatur
DLL_RESULT TC4GetCaseInsideTempDigital(int* pCaseInsideTemp, int* pCaseInsideTempFiltered);

// function TC4GetSupplyDigital
// gets digital values for VZ, 5V Supply, +15V Supply, -15V Supply and 24V Supply
DLL_RESULT TC4GetSupplyDigital(int* v5, int* v15p, int* v15n, int* v24);

/*!
 * @ingroup ActualValues
 * @brief This function reads the actual transformer primary current.
 * @param[out] pPrimaryCurrent Actual transformator primary current
 * @attention These values is not a physical value - so translate to correct physical value.
 * @see chapter Translate between standardised values and physical values
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC4GetIPrimDigital(int* pPrimaryCurrent);

/*!
 * @ingroup ActualValues
 * @brief This function reads the actual dc-link voltage.
 * @param[out] pDCLinkVoltage Actual dc.link voltage
 * @attention These values is not a physical value - so translate to correct physical value.
 * @see chapter Translate between standardised values and physical values
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC4GetDCLinkDigital(int* pDCLinkVoltage);

/*!
 * @ingroup ActualValues
 * @brief This function reads actual values of an externally connected Remote
 * Measure Box (RMB). This RMB is not shipped with TopCon by default.
 * @param[out] pRMBSystemVoltage Actual RMB-voltage
 * @param[out] pRMBSystemCurrent Actual RMB-current
 * @attention These values are not physical values - so translate to correct
 * physical values and treat vsystem and isystem as system-values.
 * @note Calling this function on a TopCon Slave will always deliver values
 * of 0 as an RMB will always be connected to a TopCon Master. The RMB
 * functionality is obsolete and not supported on CTR4.20 devices
 * (devices with firmware 4.2x.yy).
 * @see chapter Translate between standardised values and physical values
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC4GetSystemDigital(int* pRMBSystemVoltage, int* pRMBSystemCurrent);

/*!
 * @ingroup ActualValues
 * @brief function to determine whether sense functionality is installed
 * @param[out] pSupported
 * 1: Enabled<br>
 * 0: Disabled
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC4GetSenseSupported(unsigned int* pSupported);

/*!
 * @ingroup ActualValues
 * @brief check usage of voltage-sense-input
 * @param[out] pSenseEnable
 * 1: Enabled<br>
 * 0: Disabled
 * @attention check proor if sense is supported
 * @see Check if sense is supported #TC4GetSenseSupported
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC4GetSenseEnable(char* pSenseEnable);

/*!
 * @ingroup ActualValues
 * @brief set usage of voltage-sense-input
 * @param[in] senseEnable
 * 1: Enabled<br>
 * 0: Disabled
 * @attention check proor if sense is supported
 * @see Check if sense is supported #TC4GetSenseSupported
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC4SetSenseEnable(char senseEnable);

/*!
 * @ingroup ActualValues
 * @brief Gets sense output voltage, depends on ModuleSelector
 * @param[out] pVolategeActSense Actual voltage value [V]
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC4GetVoltageActSense(double* pVolategeActSense);

/*!
 * @ingroup ActualValues
 * @brief Gets sense output power, depends on ModuleSelector
 * @param[out] pPowerActSense Actual voltage value [kW]
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC4GetPowerActSense(double* pPowerActSense);

// function TC4___SenseDeltaUError
// gets, sets parameters for sense-delta obvservation
DLL_RESULT TC4GetSenseDeltaUError(double* pErrorLevel, unsigned int* pErrorDelay, char* pErrorDisable);
DLL_RESULT TC4SetSenseDeltaUError(double errorLevel, unsigned int errorDelay, char errorDisable);

// function TC4___SenseDeltaUMax
DLL_RESULT TC4GetSenseDeltaUMax(double* pVoltageMaxDelta);
DLL_RESULT TC4SetSenseDeltaUMax(double volatgeMaxDelta);

DLL_RESULT TC42GetUsenseSerieSupported(unsigned short* pUsenseSerieSupported);

/*!
 * @ingroup ActualValues
 * @brief This function reads the actual value of the sense input voltage
 * @param[out] pSenseVoltage Actual sense voltage value
 * @attention This value is not a physical value - so translate to correct physical value.
 * @see chapter Translate between standardised values and physical values
 * or #TC4GetVoltageActSense and #TC4GetPowerActSense
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC4GetSenseDigital(int* pSenseVoltage);

/*!
 * @ingroup ControllerSettings
 * @brief Reads the actual slopes and ramps for voltage 
 * @param[out] pSlope Actual startup ramp value (this ramp will be applied to
 * set values when enabling output voltage)
 * @param[out] pRamp Actual ramp value (this ramp will be applied to set
 * values when they are changed)
 * @note slope and ramp:<br>
 * 1:     slowest set value ramp: 0-100% (full scale) in 1.6 seconds<br>
 * 32000: fastest set value ramp: 0-100% (full scale) in 50us
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC4GetVoltageSlopeRamp(unsigned int* pSlope, unsigned int* pRamp);

/*!
 * @ingroup ControllerSettings
 * @brief Set slope and ramp values for voltage set values.
 * @param[in] slope ramp value for ramp at voltage on [1...32000]
 * @param[in] ramp ramp value for ramp on changing set values [1...32000]
 * @note slope and ramp:<br>
 * 1:     slowest set value ramp: 0-100% (full scale) in 1.6 seconds<br>
 * 32000: fastest set value ramp: 0-100% (full scale) in 50us<br>
 * Setting slope and ramp parameters on a TopCon Master will automatically
 * transfers them to all connected Slaves.<br>
 * Setting slope and ramp parameters on a TopCon Slave will have no effect.
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC4SetVoltageSlopeRamp(unsigned int slope, unsigned int ramp);

// function set TC4___CurrentSlopeRamp
// gets, sets max. slope for current reference (normal and startup)
DLL_RESULT TC4GetCurrentSlopeRamp(unsigned int* pSlope, unsigned int* pRamp);

/*!
 * @ingroup ControllerSettings
 * @brief Set slope and ramp values for current set values.
 * @param[in] slope ramp value for ramp at voltage on [1...32000]
 * @param[in] ramp ramp value for ramp on changing set values [1...32000]
 * @note slope and ramp:<br>
 * 1:     slowest set value ramp: 0-100% (full scale) in 1.6 seconds<br>
 * 32000: fastest set value ramp: 0-100% (full scale) in 50us<br>
 * Setting slope and ramp parameters on a TopCon Master will automatically
 * transfers them to all connected Slaves.<br>
 * Setting slope and ramp parameters on a TopCon Slave will have no effect.
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC4SetCurrentSlopeRamp(unsigned int slope, unsigned int ramp);

/*!
 * @ingroup ControllerSettings
 * @brief Reads the actual bandwidth limit for set values and actual values
 * on analog interface.
 * @param[out] pAnalogIn Actual bandwidth limit for analog set values
 * @param[out] pAnalogOut Actual bandwidth limit for analog actual values
 * @note filter frequency steps:
 * 0 = filter is turned off
 * 1 = 1600Hz, 2 = 800Hz, 3 = 400Hz, ..., 15 = 0.1Hz
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC4GetAnalogBW(unsigned int* pAnalogIn, unsigned int* pAnalogOut);

/*!
 * @ingroup ControllerSettings
 * @brief Sets bandwidth limit for analog set values and analog actual values
 * @param[in] analogIn bandwidth limit for analog set values [0...15]
 * @param[in] analogOut bandwidth limit for analog actual values [0...15]
 * @pre For firmware V4.11.28 or later remote control input must be set to analog or RS232<br>
 * For firmware V4.11.27 and earlier remote control input must be set to RS232
 * @note filter frequency steps:
 * 0 = filter is turned off
 * 1 = 1600Hz, 2 = 800Hz, 3 = 400Hz, ..., 15 = 0.1Hz
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC4SetAnalogBW(unsigned int analogIn, unsigned int analogOut);

/*!
 * @ingroup Protection
 * @brief This function is used to get the maximum allowed voltage- and current-limits.
 * @param[out] pVMax Maximum allowed voltage limit
 * @param[out] pIMax Maximum allowed current limit
 * @attention These values are not physical values - so translate to correct
 * physical values
 * @note Setting protection limits on a single master unit: treat pVMax and
 * pIMax as module-values.<br>
 * Setting protection limits in multi-unit operation treat pVMax and pIMax as system-values
 * @see chapter Translate between standardised values and physical values
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC4GetMaximas(unsigned int* pVMax, unsigned int* pIMax);

/*!
 * @ingroup SystemConfiguration
 * @brief This function reads the actually detected number of modules by theTopCon Master.
 * @param[out] pNumSeries Detected number of modules in series operation
 * @param[out] pNumParallel Detected number of modules in parallel operation
 * @param[out] pNumMultiLoad Detected number of modules in multiload operation
 * @note Calling this function on a TopCon Slave will return unreliable results.
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC4GetSystemConfigDetected(unsigned int* pNumSeries,
                                      unsigned int* pNumParallel,
                                      unsigned int* pNumMultiLoad);

/*!
 * @ingroup SystemConfiguration
 * @brief This function reads the actual preset system configuration.
 * @param[out] pNumSeries Preset number of modules in series-operation.
 * @param[out] pNumParallel Preset number of modules in parallel-operation
 * @param[out] pNumMultiLoad Preset number of modules in multiload-operation
 *.@note If called on a TC.MAC device the function will deliver the number of sub-systems
 * instead of number of modules.
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC4GetSystemConfig(unsigned int* pNumSeries,
                              unsigned int* pNumParallel,
                              unsigned int* pNumMultiLoad);

/*!
 * @ingroup SystemConfiguration
 * @brief This function sets the desired system configuration.<br>
 * These settings only take effect after a restart, so after calling this
 * function make sure the parameters are saved to non-volatile memory by
 * calling API-function TC4StoreParameters. Then power off the system and
 * turn it on again.
 * @param[in] numSeries number of modules in series operation [1...8]
 * @param[in] numParallel number of muldues in parallel operation [1...8]
 * @param[in] numMultiLoad number of modules in multiload operation [1...64]
 * @attention After powerup make sure you call the initialising functions 
 * @note Setting system configuration is only required on a TopCon Master or TC.MAC.<br>
 * Slaves automatically adjust  their operation mode on next login (powerup).<br>
 * If called on a TC.MAC this function will configure the number of sub-systems instead of number of modules.
 * @see chapter DLL initialisation and please see TopCon manual for a list of allowed combinations
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC4SetSystemConfig(unsigned int numSeries,
                              unsigned int numParallel,
                              unsigned int numMultiLoad);


// function TC4GetSlaveID
// gets slave ID via index
DLL_RESULT TC4GetSlaveID(unsigned int index, unsigned int* pSlaveId);


/*!
 * @ingroup ControllerSettings
 * @brief Reads actual adaptiv controller settings.
 * @param[out] pVPAdaptiv Adaptive gain for voltage controller P-gain
 * @param[out] pVIAdaptiv Adaptive gain for voltage controller I-gain
 * @param[out] pCPAdaptiv Adaptive gain for current controller P-gain
 * @param[out] pCIAdaptiv Adaptive gain for current controller I-gain
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC4GetAdaptivControlSettings(unsigned int* pVPAdaptiv, unsigned int* pVIAdaptiv,
                                        unsigned int* pCPAdaptiv, unsigned int* pCIAdaptiv);

/*!
 * @ingroup ControllerSettings
 * @brief Sets adaptive controller parameters. Adaptiv controller parameters
 * can improve controller behaviour when load impedance changes.
 * @param[in] vPAdaptiv Adaptive gain for voltage controller P-gain
 * @param[in] vIAdaptiv Adaptive gain for voltage controller I-gain
 * @param[in] cPAdaptiv Adaptive gain for current controller P-gain
 * @param[in] cIAdaptiv Adaptive gain for current controller I-gain
 * @note Setting adaptive controller parameters on a TopCon Master will
 * automatically transfers them to all connected Slaves.<br>
 * Setting adaptive controller parameters on a TopCon Slave will have no effect.<br>
 * Adaptive controller parameters are supported from firmware V4.11.23 or later
 * On firmware V4.11.16 this function is also implemented but in an experimental way
 * and will not work the same as in V4.11.23 or later.
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC4SetAdaptivControlSettings(unsigned int vPAdaptiv, unsigned int vIAdaptiv,
                                        unsigned int cPAdaptiv, unsigned int cIAdaptiv);

/*!
* @ingroup ControllerSettings
* @brief Check if load rejection is enabled
* @param[out] pIsLoadRejectionEnabled 0: Not enabled<br>
* 1: Enabled
* @note Load reject detection is supported from firmware V4.11.20 or later and
* is only effective in Q1 operating mode.
* @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
*/
DLL_RESULT TC4GetLoadRejectionEnabled(unsigned short *pIsLoadRejectionEnabled);


/*!
 * @ingroup ControllerSettings
 * @brief Reads load reject settings.
 * @param[out] pReduction Controller output reduction [0...4096]<br>
 * 0: 0%<br>
 * 4096: 100%
 * @param[out] pCycleDifference Current difference within 50us for load reject detection [0...4000]
 * @attention pCycleDifference is not a physical value - so translate to correct physical value
 * @note Load reject detection is supported from firmware V4.11.20 or later and
 * is only effective in Q1 operating mode.
 * @see chapter Translate between standardised values and physical values
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC4GetLoadReject(unsigned int* pReduction, unsigned int* pCycleDifference);


/*!
 * @ingroup ControllerSettings
 * @brief Sets load reject parameters. These parameters can improve controller
 * behaviour on load rejection.
 * @param[in] reduction Reduces controller output to X% of actual value when 
 * detecting load rejection [0...4096]<br>
 * 0: 0%<br>
 * 4096: 100%
 * @param[in] cycleDifference Minimal current difference between two cycles (50us)
 * to detect load rejection [0...4000].
 * @attention cycleDifference is not a physical value - so translate to correct
 * physical value
 * @note Setting load reject parameters on a TopCon Master will automatically
 * transfers them to all connected Slaves.<br>
 * Setting load reject parameters on a TopCon Slave will have no effect.<br>
 * Load reject detection is supported from firmware V4.11.20 or later and is 
 * only effective in Q1 operating mode.
 * @see chapter Translate between standardised values and physical values
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC4SetLoadReject(unsigned int reduction, unsigned int cycleDifference);


/*!
* @ingroup ControllerSettings
* @brief Check if short circuit detection is enabled
* @param[out] pIsShortCircuitDetectionEnabled 0: Not enabled<br>
* 1: Enabled
* @note Short circuit detection is supported from firmware V4.22.30 or later and
* is only effective in Q1 operating mode.
* @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
*/
DLL_RESULT TC42GetShortCircuitDetectionEnabled(unsigned short *pIsShortCircuitDetectionEnabled);


/*!
* @ingroup ControllerSettings
* @brief Reads short circuit detection settings.
* @param[out] pMaxPWM Maximum controller output [0...4096]<br>
* 0: 0%<br>
* 4096: 100%
* @param[out] pVoltageDifference Voltage difference within 50us for short circuit detection [0...4000]
* @attention pVoltageDifference is not a physical value - so translate to correct physical value
* @note Short circuit detection is supported from firmware V4.22.30 or later and
* is only effective in Q1 operating mode.
* @see Chapter Translate between standardised values and physical values
* @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
*/
DLL_RESULT TC42GetShortCircuitDetectionSettings(unsigned short *pMaxPWM, unsigned short *pVoltageDifference);


/*!
* @ingroup ControllerSettings
* @brief Sets short circuit detection parameters. This function can improve the control behavior
* in the event of a short circuit.
* @param[in] maxPWM Reduces controller output to X% of actual value when 
* detecting a short circuit [0...4096]<br>
* 0: 0%<br>
* 4096: 100%
* @param[in] voltageDifference Minimal current difference between two cycles (50us)
* to detect a short circuit[0...4000].
* @attention voltageDifference is not a physical value - so translate to correct
* physical value
* @note Setting short circuit detection parameters on a TopCon Master will automatically
* transfers them to all connected Slaves.<br>
* Setting short circuit detection parameters on a TopCon Slave will have no effect.<br>
* Short circuit detection is supported from firmware V4.22.30 or later and is 
* only effective in Q1 operating mode.
* @see Chapter Translate between standardised values and physical values
* @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
*/
DLL_RESULT TC42SetShortCircuitDetectionSettings(unsigned short maxPWM, unsigned short voltageDifference);


// funtion TC4GetErrorHistoryHeader
DLL_RESULT TC4GetErrorHistoryHeader(unsigned int* number, unsigned int* startIndex);

// function TC4GetErrorHistoryEntry
DLL_RESULT TC4GetErrorHistoryEntry(unsigned int index, unsigned int* day, unsigned int* hour, unsigned int* minute,
                                   unsigned int* second, unsigned int* counter50us, unsigned int* group, unsigned int* detail);

/*!
 * @ingroup ControllerSettings
 * @brief Reads actual settings for slave characteristics.
 * @param[out] pSlaveVoltErr Actual allowed slave voltage error [-4000...4000].<br>
 * (the values correspond in percentage from [-100%...100.0%] of nominal voltage)
 * @param[out] pSlaveCurrErr Actual allowed slave current error [-4000...4000]<br>
 * (the values correspond in percentage from [-100%...100.0%] of maximum current)
 * @param[out] pTurnOffSlaves Actual setting whether slaves are automatically
 * turned off on small or zero load current.
 * @note Slave characteristics are supported from firmware V4.11.30 or later.
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC4GetSlaveCharacteristics(int* pSlaveVoltErr, int* pSlaveCurrErr, unsigned int* pTurnOffSlaves);

/*!
 * @ingroup ControllerSettings
 * @brief Set parameters for slave characteristics. These parameters determine
 * whether TopCon Slaves try to reach set values fast or slow, depending on their
 * actual control-error. Automatically turning off the slave controller when load
 * current is very small or zero can improve output voltage stability.
 * @param[in] slaveVoltErr Allowed slave voltage error [-4000...4000]<br>
 * (the values correspond in percentage from [-100%...100%])
 * of nominal voltage) If the actual voltage error is smaller than this limit
 * the slave reaches the set values slowly.
 * @param[in] slaveCurrErr Allowed slave current error [-4000...4000]<br>
 * (the values correspond in percentage from [-100%...100%])
 * of maximum current) If the actual current error is smaller that this limit
 * the slave reaches the set values slowly.
 * @param[in] turnOffSlaves Turn off slaves on small or zero load current:<br>
 * 0: no<br>
 * 1: yes
 * @note Setting slave characteristics on a TopCon Master will automatically
 * transfers them to all connected Slaves.<br>
 * Setting slave characteristics on a TopCon Slave will have no effect.<br>
 * On a single-master-system slave characteristics will have no effect.<br>
 * Slave characteristics are supported from firmware V4.11.30 or later.
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC4SetSlaveCharacteristics(int slaveVoltErr, int slaveCurrErr, unsigned int turnOffSlaves);

// function TC4GetDeviceBaseVersion
// gets base device-version, version is diffenent from base version on client specific software modifications
DLL_RESULT TC4GetDeviceBaseVersion(unsigned int* pBaseMain, unsigned int* pBaseSub, unsigned int* pBaseRevision);
DLL_RESULT TC4GetDeviceBaseVersionLong(unsigned long* pBaseVersion);

/*!
 * @ingroup TFE
 * @brief This function allows to check the possibilities of the different TFE
 * versions
 * @param[out] pMaxVersion
 * @see chapter Compatibility of TFE (function generator) functionality in the
 * tcio documentation.
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC4GetFnSeqMaxSupportedVersion(unsigned int* pMaxVersion);

DLL_RESULT TC4GetFnSeqMaxSupportedHeaderVersion(unsigned int* pMaxSupportedHeaderVersion);// firmware
DLL_RESULT TC4GetFnSeqMaxSupportedHeaderExtensionVersion(unsigned int* pMaxSupportedHeaderExtensionVersion);// firmware

/*!
 * @ingroup TFE
 * @brief This function allows to check the possibilities of the different TFE versions
 * @param[in] MaxUserVersion
 * 0: first Version<br>
 * 1: additional FnBlock for AAP-Flags<br>
 * 2: additional FnBlock for AAP-Input-Scaling (std.)<br>
 * 3: user knows header extension version (for SAS-Option only)<br>
 * @see chapter Compatibility of TFE (function generator) functionality in
 * the tcio documentation.
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC4SetFnSeqMaxUserCompatibleVersion(unsigned int MaxUserVersion);

// functions to read/write header-data from/to Controllerboard-RAM
DLL_RESULT TC4GetFnSeqHeader(struct T_FnSeqHeader* pHeader);
DLL_RESULT TC4GetFnSeqHeaderOfActualCurve(struct T_FnSeqHeader* pHeader);

/*!
 * @ingroup TFE
 * @brief Storing header information
 * @param[in] pHeader
 * @see chapter Compatibility of TFE (function generator) functionality in
 * the tcio documentation.
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC4SetFnSeqHeader(struct T_FnSeqHeader* pHeader);

//get from ram
DLL_RESULT TC4GetFnSeqHeaderExtension(unsigned int HeaderExtensionVersion,
                                      struct T_FnSeqHeaderExtension* pHeaderExt,
                                      struct T_SASTechnologyParamsEN50530* pCellTechnologyParams);

//get from flash
DLL_RESULT TC4GetFnSeqHeaderExtensionOfActualCurve(unsigned int HeaderExtensionVersion,
                                                   struct T_FnSeqHeaderExtension* pHeaderExt,
                                                   struct T_SASTechnologyParamsEN50530* pCellTechnologyParams);

DLL_RESULT TC4SetFnSeqHeaderExtension(unsigned int HeaderExtensionVersion,
                                      struct T_FnSeqHeaderExtension* pHeaderExt,
                                      struct T_SASTechnologyParamsEN50530* pCellTechnologyParams);

DLL_RESULT TC4GetFnSeqHeaderExtensionVersion(unsigned int* pHeaderExtensionVersion);//RAM
DLL_RESULT TC4GetFnSeqHeaderExtensionVersionOfActualCurve(unsigned int* pHeaderExtensionVersion);//FLASH
DLL_RESULT TC4GetFnSeqHeaderExtensionVersionGenerator(unsigned int* pHeaderExtensionVersion);//LIB

/*!
 * @ingroup TFE
 * @brief Read header information
 * @param[out] pFnseq
 * @see chapter Compatibility of TFE (function generator) functionality in
 * the tcio documentation.
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC4GetFnSeqSettings(struct T_FnSeq* pFnseq);

/*!
 * @ingroup TFE
 * @brief Storing header information
 * @param[in] pFnseq
 * @see chapter Compatibility of TFE (function generator) functionality in
 * the tcio documentation.
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC4SetFnSeqSettings(struct T_FnSeq* pFnseq);

DLL_RESULT TC4GetFnSeqEnabledFnBlocks(unsigned int* pGeneralEnable, unsigned int* pEnabledFnBlocks);

/*!
 * @ingroup TFE
 * @brief Amplitude -, offset - and AAP-input-scaling
 * @param[out] pFnblock
 * @param[in] type
 * @see chapter Compatibility of TFE (function generator) functionality in
 * the tcio documentation.
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC4GetFnBlockSettings(struct T_FnBlock* pFnblock, unsigned int type);

/*!
 * @ingroup TFE
 * @brief Amplitude -, offset - and AAP-input-scaling
 * @param[in] pFnblock
 * @param[in] type
 * @see chapter Compatibility of TFE (function generator) functionality in
 * the tcio documentation.
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC4SetFnBlockSettings(struct T_FnBlock* pFnblock, unsigned int type);

/*!
 * @ingroup TFE
 * @brief Get amplitude and AAP input scale actual values of a selected function block
 * (e.g. if ramps active)
 * @param[in] type select the function block:<br>
 * 0: voltage<br>
 * 1: current<br>
 * 2: power
 * @param[out] pAmplitudeAct physical value of selected block type
 * @param[out] pInputScalingAct as normalized vales [0.125...10.0]<br>
 * (the values correspond in percentage from [12.5%...1000.0%])
 * @see chapter Compatibility of TFE (function generator) functionality in
 * the tcio documentation.
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC4GetFnBlockAmplitudeAndInputScalingAct(unsigned int type, double* pAmplitudeAct, double* pInputScalingAct);

/*!
 * @ingroup TFE
 * @brief Set amplitude and AAP input scale reference value of a selected function block
 * (e.g. if ramps active)
 * Not passible if calculated SAS curve active
 * @param[in] type select the function block:<br>
 * 0: voltage<br>
 * 1: current<br>
 * 2: power
 * @param[in] amplitudeRef physical value of selected block type
 * @param[in] inputScalingRef  as normalized vales [0.125...10.0]<br>
 * (the values correspond in percentage from [12.5%...1000.0%])
 * @note prior this function call the function TC4SetFuncgenRampTime to set the transition time to enter the new reference values
 * @see chapter Compatibility of TFE (function generator) functionality in
 * the tcio documentation.
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC4SetFnBlockAmplitudeAndInputScalingRef(unsigned int type, double amplitudeRef, double inputScalingRef);

/*!
 * @ingroup TFE
 * @brief Get amplitude and AAP input scale reference value of a selected function block
 * (e.g. if ramps active)
 * @param[in] type select the function block:<br>
 * 0: voltage<br>
 * 1: current<br>
 * 2: power
 * @param[out] pAmplitudeRef physical value of selected block type
 * @param[out] pInputScalingRef  as normalized vales [0.125...10.0]<br>
 * (the values correspond in percentage from [12.5%...1000.0%])
 * @see chapter Compatibility of TFE (function generator) functionality in
 * the tcio documentation.
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC4GetFnBlockAmplitudeAndInputScalingRef(unsigned int type, double* pAmplitudeRef, double* pInputScalingRef);

// function to get actual value of curve transition ramp time 
DLL_RESULT TC4GetFnBlockCurveTransitionRampTimeAct(unsigned int type, double* pRamptime);

/*!
 * @ingroup TFE
 * @brief Storing user-defined AAP values<br>
 * After copying of settings to flash, the slope values have to be stored also.
 * The values are copied into flash memory. In order to provide empty space on
 * reserved locations, the function TC4SetFnSeqActionStore does it.
 * @param[in] type
 * Defines the assignment of data to a function block:<br>
 * 0: voltage<br>
 * 1: current<br>
 * 2: power
 * @note Use this function to prepare the type of the data to storing.
 * @see The function TC4SetFnSeqActionStore, TC4SetNextUserAAPData
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC4SetFnBlockUserDataStartAddr(unsigned int type);

// functions to read/write user defined data (time based) from/to flash
DLL_RESULT TC4GetNextUserTimeData(unsigned int type, unsigned int* pTimeDelta, int* pAmplitude, unsigned int maxAmplitude, unsigned int* readSize);
DLL_RESULT TC4SetNextUserTimeData(unsigned int type, unsigned int* pTimeDelta, int* pAmplitude, unsigned int maxAmplitude, unsigned int writeSize);

// functions to read/write user defined AAP data from/to flash
DLL_RESULT TC4GetNextUserAAPData(unsigned int type, int* pInput, int* pAmplitude, unsigned int maxAmplitude, unsigned int* pReadSize);

/*!
 * @ingroup TFE
 * @brief This function ist used to store user-defined AAP values into
 * non-volatile flash memory.
 * @param[in] type Identical to type-value of DLL function
 * TC4SetFnBlockUserDataStartAddr<br>
 * 0: voltage<br>
 * 1: current<br>
 * 2: power<br>
 * @param[in] pInput AAP input value (voltage, current, power, depending
 * on AAPInputType)<br>
 * All values normalized to system nominal data.<br>
 * If AAPInputType:<br>
 * 0: 4000 equals system nominal voltage<br>
 * 1: 4000 equals system nominal current<br>
 * 2: 4000 equals system nominal power<br>
 * <b>See also</b> the discription of Structure T_FnBlock<br>
 * @param[in] pAmplitude Amplitude value (voltage, current power according to
 * type) Use normalized values [0...4000]<br>
 * If type:<br>
 * 0: 4000 equals system maximum voltage<br>
 * 1: 4000 equals system maximum current<br>
 * 2: 4000 equals system maximum power
 * @param[in]  maxAmplitude Maximum value of all user-defined amplitudes of the
 * used function block.<br>
 * Use normalized values [0...4000] - Calculate like pAmlitude above.<br>
 * <b>See also</b> the discription of Structure T_FnBlock<br>
 * @param[in] writeSize Value range:<br>
 * [1...8] up to 8 data points may be stored per one DLL-call.<br>
 * If value is between [2...8], pInput and pAmplitude will be treated as an array
 * with the size of writeSize.<br>
 * If more than the writeSize values need to be saved, use this DLL-function
 * repeatedly.<br>
 * <b>Example</b><br>
 * Number of data points (UserDefNumPoints) = 20<br>
 * 1) Call DLL-function 20 times (once for each point) with writeSize = 1 or<br>
 * 2) Call DLL-function 2 times with writeSite = 8 and once with writeSize = 4
 * @pre - The function TC4SetFnSeqActionStore must be called before.<br>
 *      - After that the function TC4SetFnBlockUserDataStartAddr must be called.<br>
 *     - Now the DLL and the device are ready for the function described here
 *        (one or more times, as necessary).
 * @note According to writeSize, the function has to be called once or several times.<br>
 * If attempting to store more values as previously defined in (UserDefNumPoints),
 * the function will return DLL-FAIL and the actual function call has no effect.
 * In this case call the DLL function again with the correct setting of writeSize value.
 * If more than 1 function block with user-defined data does exist, the next block
 * may now be stored by calling the DLL again <b>but keep attention</b> to use a
 * different type value.<br>
 * After completion of data storage, the function sequence is ready to use.
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC4SetNextUserAAPData(unsigned int type, int* pInput, int* pAmplitude, unsigned int maxAmplitude, unsigned int writeSize);

/*!
 * @ingroup TFE
 * @brief Loading a slope
 * @param[in] SeqNr Sequence number [1...999]
 * @note The DLL call returns immediately even if the slope has not been
 * completely loaded. In order to check for the state of loading action,
 * use the this DLL-function.
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC4SetFnSeqActionLoad(unsigned int SeqNr);

/*!
 * @ingroup TFE
 * @brief Transfer of settings into flash
 * @param[in] SeqNr Sequence number of the stored slope
 * @param[in] copyUserData This value has to be set to 0<br>
 * @note This function stores the settings of<br>
 * - TC4SetFnSeqHeader<br>
 * - TC4SetFnSeqSettings<br>
 * - TC4SetFnBlockSettings<br>
 * into flash memory.
 * @note After calling this function the copy process will start immediately.
 * Also reserves the necessary space in flash.
 * @see In order to check whether the storing process has already finished,
 * use the TC4GetFnSeqActionResult function
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC4SetFnSeqActionStore(unsigned int SeqNr, int copyUserData);
DLL_RESULT TC4SetFnSeqActionDelete(unsigned int SeqNr);
DLL_RESULT TC4SetFnSeqActionReadFirstHeader(void);
DLL_RESULT TC4SetFnSeqActionReadNextHeader(void);
DLL_RESULT TC4SetFnSeqActionDeleteAllFromFlash(void);

/*!
 * @ingroup TFE
 * @brief Check state of last started FnSeqAction (e.g. loading, storing,
 * deleting funtion sequences)
 * @param[out] pResult
 *  1: Unit busy, action not finished yet<br>
 *  0: Action completed, no errors<br>
 * <0: Error while execution of action:<br>
 * -1: Timeout during reading from flash<br>
 * -2: Flash location not empty<br>
 * -3: Sequence number doesn't exist<br>
 * -4: Flash full - erase some unused slopes<br>
 * -5: Requested action does not exist<br>
 * -6: No function sequence avalable<br>
 * -7: TCLIN is not responding<br>
 * -8: Wrong context (precondition of requested action not fulfilled)<br>
 * -9: Wrong version (action could not be executed because of an internel
 *     version conflict)<br>
 * -10: Tried to confirm "DeleteAllFromFlash"-action after internal
 *      timeout (1s)<br>
 * -11: Error on deleting all funcion sequences from flash<br>
 * -12: Invalid internal header detected, execution aborted to ensure data
 *      consistency<br>
 * -99: Reserved for internal porposes<br>
 * -100: overall timeout<br>
 * @note After loading, the internal structures T_FnSeqHeader, T_FnSeq and
 * T_FnBlock  are set with the new data values.
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC4GetFnSeqActionResult(int* pResult);

// function to read actual function-sequence state
DLL_RESULT TC4GetFnSeqState(unsigned int* pState);

// function to manually trigger start of function-sequence
DLL_RESULT TC4SetFnSeqManualTrigger();

// function to get actual function-generator limits
DLL_RESULT TC4GetFnSeqLimits(double* pFreqMax, double* pFreqMin);

// function to determine if actual function-sequence has changed outside TopControl (e.g. by HMI)
// use set-command to reset variable to 0
DLL_RESULT TC4GetFnSeqUpdateRequired(int* pUpdateRequired);
DLL_RESULT TC4SetFnSeqUpdateRequired(int updateRequired);

// function to determine if negative reference values are allowed
DLL_RESULT TC4GetBipolarEnable(int* pBipolarEnable);

// functions to read/set Function Sequence for auto load at startup
DLL_RESULT TC4GetFnSeqAutoLoad(int* pSeqNr);
DLL_RESULT TC4SetFnSeqAutoLoad(int seqNr);

// function to determine if a new function sequence was loaded (couter is increased)
DLL_RESULT TC4GetFnSeqLoadCounter(unsigned int* pLoadCounter);

/*!
 * @ingroup TFE
 * @brief Start / Stop of a function sequence
 * @param cmd Valid values:<br>
 * 1: stop   Stops the function sequence<br>
 * 2: start  Starts function sequence from beginning<br>
 * 3: pause  Pauses the execution of the function sequence<br>
 * 4: resume continues the execution of the function sequence (if paused)
 * @attention DLL version V3.00.00 or greater required.
 * @note With firmware versions V4.11.44 and older only the start command is supported.<br>
 * Other commands will be ignored.<br>
 * If paused the actual function value is stored and the execution is stopped.
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC4SetFnSeqCommand(unsigned int cmd);

// function to get/set ramp time for Amplitude, Offset, AAPInputScaling and FnSeq transition when loading a FnSeq from flash
DLL_RESULT TC4GetFuncgenRampTime(double* pRampTime);

/*!
 * @ingroup TFE
 * @brief Ramp time definition
 * @param[in] pRampTime in [s]
 * @see chapter Compatibility of TFE (function generator) functionality in the tcio documentation.
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC4SetFuncgenRampTime(double pRampTime);

/*!
 * @ingroup TFE
 * @brief Determine if any funcgen ramp is active
 * @param[out] pIsActive
 * @see chapter Compatibility of TFE (function generator) functionality in the tcio documentation.
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC4GetIsAnyFuncgenRampActive(unsigned int* pIsActive);

/*!
 * @ingroup TFE
 * @brief Determine if any funcgen ramp is active
 * @param[out] pActiveRampsBitmask
 * [0...3] Voltage, 0:Amplitude, 1: Offset, 2: InputScaling, 3: OutputTransistion<br>
 * [4...7] Current, 4:Amplitude, 5: Offset, 6: InputScaling, 7: OutputTransistion<br>
 * [8...11] Power, 8:Amplitude, 9: Offset, 10: InputScaling, 11: OutputTransistion<br>
 * @see chapter Compatibility of TFE (function generator) functionality in the tcio documentation.
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC4GetFuncgenActiveRampsBitmask(unsigned int* pActiveRampsBitmask);

// function to determine how long a specific ramp will last
DLL_RESULT TC4GetFuncgenRampTimeRef(unsigned int RampSelector, double* pTotalSeconds);

/*!
 * @ingroup TFE
 * @brief Ramp time definition
 * @param[in] RampSelector
 * [0...3] Voltage, 0: Amplitude, 1: Offset, 2: InputScaling, 3: OutputTransistion<br>
 * [4...7] Current, 4: Amplitude, 5: Offset, 6: InputScaling, 7: OutputTransistion<br>
 * [8...11] Power, 8: Amplitude, 9: Offset, 10: InputScaling, 11: OutputTransistion<br>
 * 12: SAS Irradiance<br>
 * 13: SAS Temperature<br>
 * @param[out] pRemainingSeconds time in seconds
 * @see chapter Compatibility of TFE (function generator) functionality in the tcio documentation.
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC4GetFuncgenRampTimeAct(unsigned int RampSelector, double* pRemainingSeconds);

// function to determine if Funcgen is supportet on actual device
DLL_RESULT TC4GetFuncgenSupported(int* pSupported);

// function to read the sequnece number of the actually loaded function sequence, zero if no function sequence loaded
DLL_RESULT TC4GetFnSeqActualSequenceNumber(unsigned int* pActualSequenceNumber);

// function to get the base function (Sine, Rect, Triangle, Userdef [timebased], AAP) of the selected function block
DLL_RESULT TC4GetFnSeqActualBaseFunctionType(unsigned int fnBlockType, unsigned int* pBaseFunctionType);

// function to read serial number of device
DLL_RESULT TC4GetSerialNr(unsigned long* pSerialNr);

/*!
 * @ingroup DeviceInfo
 * @brief This function reads serial number string of device.
 * @param[in] sizeInBytes size of string [byte] <br>
 * The string must have a minimum size of 10 bytes, otherwise the dll returns an error.
 * @param[out] pSerialNr the requested serial number string
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC4GetSerialNrString(unsigned short sizeInBytes, char* pSerialNr);

// function to read installed (evaluation) options and remaining evaluation time in case of evaluation version
DLL_RESULT TC4GetInstalledOptions(unsigned int* pOptions);
DLL_RESULT TC4GetInstalledEvaluationOptions(unsigned int* pOptions);
DLL_RESULT TC4GetActiveEvaluationOptions(unsigned int* pOptions);
DLL_RESULT TC4GetEvaluationTimeRemaining(unsigned int optionNr, double* pRemainingSeconds);

// functions to read error history from flash
DLL_RESULT TC4GetFlashErrorHistorySize(unsigned int* pEntries);
DLL_RESULT TC4GetFlashErrorHistoryFirstEntry(struct T_ErrorHistoryEntry *pEntry, int *pError);
DLL_RESULT TC4GetFlashErrorHistoryNextEntry(struct T_ErrorHistoryEntry *pEntry, int *pError);

// function to read actual operating hour counter (counts seconds)
DLL_RESULT TC4GetOperatingSeconds(unsigned long* pSeconds);

// function to get operating hour counter (in seconds) at powerup
DLL_RESULT TC4GetPowerupTime(unsigned long* pSeconds);

// function to get/set actual baudrate default value
DLL_RESULT TC4GetRS232BaudrateDefault(int* pBaudrate);
DLL_RESULT TC4SetRS232BaudrateDefault(int Baudrate);

// function to read version and type of option board (CAN/USB, GPIB)
DLL_RESULT TC4GetOptionBoardInfo(unsigned int* pType, unsigned int* pVersion);

// function to get/set VLS Settings:
DLL_RESULT TC4GetVLSInputSelector(unsigned int* pSelector);
DLL_RESULT TC4SetVLSInputSelector(unsigned int selector);

DLL_RESULT TC4GetVLSFunctionSelector(unsigned int* pSelector);
DLL_RESULT TC4SetVLSFunctionSelector(unsigned int selector);

DLL_RESULT TC4GetVLSLimits(int* pLowerLimit, int* pUpperLimit);
DLL_RESULT TC4SetVLSLimits(int lowerLimit, int upperLimit);

DLL_RESULT TC4GetVLSLimitHysteresis(int* pLowerLimitHysteresis, int* pUpperLimitHysteresis);
DLL_RESULT TC4SetVLSLimitHysteresis(int lowerLimitHysteresis, int upperLimitHysteresis);

DLL_RESULT TC4GetVLSOutputBehaviour(unsigned int* pOutputSelector, unsigned int* pInvertOutput);
DLL_RESULT TC4SetVLSOutputBehaviour(unsigned int outputSelector, unsigned int invertOutput);

DLL_RESULT TC4GetVLSDelays(unsigned long* pActiveToInactiveDelay, unsigned long* pInactiveToActiveDelay);
DLL_RESULT TC4SetVLSDelays(unsigned long activeToInactiveDelay, unsigned long inactiveToActiveDelay);

DLL_RESULT TC4GetVLSMaxRelaisSwitchingFrequency(unsigned int* pMaxSwitchingsPerSecond);
DLL_RESULT TC4SetVLSMaxRelaisSwitchingFrequency(unsigned int maxSwitchingsPerSecond);

DLL_RESULT TC4GetVLSEditableByHMI(unsigned int* pVLSEditableByHMI);
DLL_RESULT TC4GetVLSSupported(unsigned int* pIsSupported);

DLL_RESULT TC4GetSecurityRelaisPresent(unsigned int* pSecurityRelaisPresent);

DLL_RESULT TC4GetI2TScaleFactor(double* pI2TScaleFactor);


//****************************************************************************
//** High level commands up to CTR V4.20                                     *
//****************************************************************************
DLL_RESULT TC4GetOutputCharge(double* pChargeAh);
DLL_RESULT TC4GetOutputEnergy(double* pEnergyWs);
DLL_RESULT TC4GetSASMppEnergy(double* pEnergyWs);

DLL_RESULT TC4GetOffLineScopeValue(struct T_OfflineScopeData* pEntry, unsigned int* pNumOfReads);
DLL_RESULT TC4SetOffLineScopeChannels(struct T_OfflineScopeChannels* pChannels);
DLL_RESULT TC4GetOffLineScopeChannels(struct T_OfflineScopeChannels* pChannels);

DLL_RESULT TC4SetOffLineScopeControl(unsigned int Control);
DLL_RESULT TC4GetOffLineScopeControl(unsigned int* pControl);
DLL_RESULT TC4SetOffLineScopeTriggerVariable(unsigned int Address);
DLL_RESULT TC4SetOffLineScopeTriggerLevel(double Level);

DLL_RESULT TC4SetOffLineScopeTimeSlice(unsigned int Time);
DLL_RESULT TC4SetOffLineScopeTimeFactor(unsigned int Time);

DLL_RESULT TC4GetOffLineScopeNumOfValidSamples(unsigned int* pNumOfSamples);
DLL_RESULT TC4GetOffLineScopeNumChannels(unsigned int* pNumOfChannels);
DLL_RESULT TC4GetOffLineScopeStatus(unsigned int* pStatus);

//****************************************************************************
//** function to read interface version of Options or modules               */
//****************************************************************************
DLL_RESULT TC4GetSASControlInterfaceVersion(unsigned int* pInterfaceVersion);
DLL_RESULT TC4GetAccuControlInterfaceVersion(unsigned int* pInterfaceVersion);
DLL_RESULT TC4GetTCLINSystemInterfaceVersion(unsigned int* pInterfaceVersion);
DLL_RESULT TC4GetOfflineScopeInterfaceVersion(unsigned int* pInterfaceVersion);
DLL_RESULT TC4GetAuxilaryFunctionsInterfaceVersion(unsigned int* pInterfaceVersion);

//****************************************************************************
//** functions to read TC.MAC values on a TopCon Master device              */
//****************************************************************************
DLL_RESULT TC4GetTCMACMode(unsigned int* pMode);

//****************************************************************************
//** functions to read/write TCLIN values over TopCon Master device         */
//****************************************************************************
DLL_RESULT TC4GetTCLINSupported(unsigned int* pIsSupported);
DLL_RESULT TC4GetTCLINActiveDevices(unsigned int* pActiveBitmask);
DLL_RESULT TC4GetTCLINDeviceCount(unsigned int* pCount);

DLL_RESULT TC4GetTCLINNominalVoltage(unsigned int* pNominalVoltage);
DLL_RESULT TC4GetTCLINNominalCurrent(unsigned int* pNominalCurrent);

DLL_RESULT TC4GetTCLINVoltageControlSettings(unsigned int moduleID, unsigned int* pVoltagePGain, unsigned int* pVoltageIGain);
DLL_RESULT TC4GetTCLINCurrentControlSettings(unsigned int moduleID, unsigned int* pCurrentPGain, unsigned int* pCurrentIGain);
DLL_RESULT TC4SetTCLINVoltageControlSettings(unsigned int moduleID, unsigned int voltagePGain, unsigned int voltageIGain);
DLL_RESULT TC4SetTCLINCurrentControlSettings(unsigned int moduleID, unsigned int currentPGain, unsigned int currentIGain);

DLL_RESULT TC4SetTCLINSystemControlSettingsEnable(unsigned short enable);
DLL_RESULT TC4GetTCLINSystemControlSettingsEnable(unsigned short* pEnable);
DLL_RESULT TC4GetTCLINSystemVoltageControlSettings(unsigned int* pVoltagePGain, unsigned int* pVoltageIGain);
DLL_RESULT TC4GetTCLINSystemCurrentControlSettings(unsigned int* pCurrentPGain, unsigned int* pCurrentIGain);
DLL_RESULT TC4SetTCLINSystemVoltageControlSettings(unsigned int voltagePGain, unsigned int voltageIGain);
DLL_RESULT TC4SetTCLINSystemCurrentControlSettings(unsigned int currentPGain, unsigned int currentIGain);

DLL_RESULT TC4GetTCLINOutputVoltageStats(unsigned int moduleID, double* pVoltageMean, double* pVoltageMin, double* pVoltageMax);
DLL_RESULT TC4GetTCLINOutputCurrentStats(unsigned int moduleID, double* pCurrentMean, double* pCurrentMin, double* pCurrentMax);
DLL_RESULT TC4GetTCLINOutputPowerStats(unsigned int moduleID, double* pPowerMean, double* pPowerMin, double* pPowerMax);

DLL_RESULT TC4ReadTCLINOutputSystemStats();
DLL_RESULT TC4GetTCLINOutputVoltageSystemStatsShadow(double* pVoltageMean, double* pVoltageMin, double* pVoltageMax);
DLL_RESULT TC4GetTCLINOutputCurrentSystemStatsShadow(double* pCurrentMean, double* pCurrentMin, double* pCurrentMax);
DLL_RESULT TC4GetTCLINOutputPowerSystemStatsShadow(double* pPowerMean, double* pPowerMin, double* pPowerMax);
DLL_RESULT TC4GetTCLINOutputVoltageSystemStats(double* pVoltageMean, double* pVoltageMin, double* pVoltageMax);
DLL_RESULT TC4GetTCLINOutputCurrentSystemStats(double* pCurrentMean, double* pCurrentMin, double* pCurrentMax);
DLL_RESULT TC4GetTCLINOutputPowerSystemStats(double* pPowerMean, double* pPowerMin, double* pPowerMax);

DLL_RESULT TC4GetTCLINSASActualMPP(unsigned int moduleID, double* pVoltageMPP, double* pCurrentMPP);
DLL_RESULT TC4GetTCLINSASActualMPPSystem(double* pVoltageMPP, double* pCurrentMPP, double* pPowerMPP);
DLL_RESULT TC4GetTCLINOutputEnergy(unsigned int moduleID, double* pOutputEnergyWs);

DLL_RESULT TC4SetTCLINSASMppEfficiencyMeasurementStart(void);
DLL_RESULT TC4GetTCLINSASMppEfficiency(double* pEfficiency);
DLL_RESULT TC4GetTCLINOutputAndMPPEnergyAll(double* pOutputEnergyWs, double* pMPPEnergyWs);

DLL_RESULT TC4GetTCLINErrorStats(unsigned int TCLINModuleID, unsigned int* pCANErrorFlags, unsigned int* pErrorFlags);
DLL_RESULT TC4GetTCLINTemperatures(unsigned int TCLINModuleID, double* pTemperatureK1, double* pTemperatureK2, double* pTemperaturePCB);
DLL_RESULT TC4GetTCLINInternalSupplyVoltages(unsigned int TCLINModuleID, double* pSupplyVoltage5V, double* pSupplyVoltage15V);

//****************************************************************************
//** High level commands up to CTR V4.20                                     *
//****************************************************************************
/*!
 * @ingroup watchdog
 * @brief This function checks if watchdog is supported on actual device
 * @param[out] pIsSupported
 * 0: false<br>
 * 1: true
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC4GetWatchdogSupported(unsigned int* pIsSupported);

/*!
 * @ingroup watchdog
 * @brief This function checks if watchdog is enabled
 * @param[out] pWatchdogEnable 0: Disable<br>
 * 1: Enabled
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC4GetWatchdogEnable(unsigned int* pWatchdogEnable);

/*!
* @ingroup watchdog
* @brief This function is used to set watchdog enabled.
* @param[in] watchdogEnable 0: Disable<br>
* 1: Enabled
* @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
*/
DLL_RESULT TC4SetWatchdogEnable(unsigned int watchdogEnable);

/*!
* @ingroup watchdog
* @brief This function checks if watchdog is active.
* The watchdog is active when this parameter and the "WatchdogEnable" parameter are set to 1.
* @param[out] pWatchdogActive 0: Inactive<br>
* 1: Active
* @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
*/
DLL_RESULT TC4GetWatchdogActive(unsigned int* pWatchdogActive);

/*!
* @ingroup watchdog
* @brief This function is used to set watchdog active.
* The watchdog is active when this parameter and the "watchdogEnable" parameter are both set to 1.
* The user can turn the watchdog on or off by setting this parameter.
* To turn the watchdog on or off temporarily, this parameter should be used instead of "watchdogEnable", 
* because it is not permanently stored in the flash.
* @param[in] watchdogActive 0: Inactive<br>
* 1: Active
* @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
*/
DLL_RESULT TC4SetWatchdogActive(unsigned int watchdogActive);

/*!
* @ingroup watchdog
* @brief This function is used to reset the internal watchdog timer.
* The watchdog must be reset at regular intervals. If not the internal watchdog timer runs out and 
* the device is turned off with a watchdog error.
* @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
*/
DLL_RESULT TC4SetWatchdogReset(void);

/*!
* @ingroup watchdog
* @brief This function gets the time, at which the watchdog will start after Powerup automatically.
* (The watchdog only starts automatically when it is enable and the AutoActiveTime is higher than 0.)
* @param[out] pTimeInSeconds [100ms]
* @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
*/
DLL_RESULT TC4GetWatchdogAutoActiveTime(double* pTimeInSeconds);

/*!
* @ingroup watchdog
* @brief This function sets the time, at which the watchdog will start after Powerup automatically.
* (The watchdog only starts automatically when it is enable and the AutoActiveTime is higher than 0.)
* @param[in] timeInSeconds [100ms]
* @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
*/
DLL_RESULT TC4SetWatchdogAutoActiveTime(double timeInSeconds);

/*!
* @ingroup watchdog
* @brief This function gets the watchdog time out.
* If this time runs out without resetting watchdog, then watchdog error occurs.
* @param[out] pTimeInSeconds [100ms]
* @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
*/
DLL_RESULT TC4GetWatchdogTimeoutTime(double* pTimeInSeconds);

/*!
* @ingroup watchdog
* @brief This function sets the watchdog time out.
* If this time runs out without resetting watchdog, then watchdog error occurs.
* @param[in] timeInSeconds [100ms]
* @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
*/
DLL_RESULT TC4SetWatchdogTimeoutTime(double timeInSeconds);

//****************************************************************************
//** sas                                                                     *
//****************************************************************************
DLL_RESULT TC4GetSASActualMPP(double* pMppVoltage, double* pMppCurrent, double* pMppPower);

DLL_RESULT TC4GetSASIsValidCalculatedCurveLoaded(unsigned int* pIsValid);

DLL_RESULT TC4GetSASIrradianceAct(double* pIrradianceAct, double* pRamptimeAct);
DLL_RESULT TC4GetSASIrradianceRef(double* pIrradianceRef, double* pRamptimeRef);
DLL_RESULT TC4SetSASIrradianceRef(double irradianceRef, double ramptimeRef);

DLL_RESULT TC4GetSASTemperatureAct(double* pTemperatureAct, double* pRamptimeAct);
DLL_RESULT TC4GetSASTemperatureRef(double* pTemperatureRef, double* pRamptimeRef);
DLL_RESULT TC4SetSASTemperatureRef(double temperatureRef, double ramptimeRef);

DLL_RESULT TC4SetSASMppEfficiencyMeasurementStart(void);
DLL_RESULT TC4GetSASMppEfficiency(double* pEfficiency);
DLL_RESULT TC4GetOutputAndMPPEnergyAll(double* pOutputEnergy_Ws, double* pMPPEnergy_Ws);

DLL_RESULT TC4CreateSASCalculatedCurveIU(const void* pInCellTechnologyParams, struct T_FnSeqHeader* pInOutHeader,
                                         struct T_FnSeqHeaderExtension* pInOutHeaderExt, struct T_FnSeq* pOutFnSeq,
                                         struct T_FnBlock* pOutFnBlock, unsigned int maxArraySize, double* pOutVoltageArray,
                                         double* pOutCurrentArray, int* pError);

DLL_RESULT TC4CreateSASIrradianceAndTemperatureLookup(const void* pInCellTechnologyParams, unsigned int headerExtensionVersion,
                                                      struct T_FnSeqHeaderExtension* pInOutHeaderExt, int* pError);

DLL_RESULT TC4GetSASStandardCellTechnologyParamsEN50530(unsigned int CellTechnologyType,
                                                        struct T_SASTechnologyParamsEN50530* pCellTechnologyParams);

DLL_RESULT TC4SetSASFuncgenInterfaceEnabled(void);

//****************************************************************************
//** High level commands up to CTR V4.20                                     *
//****************************************************************************
DLL_RESULT TC4GetUIPMinMax(double* pVoltageActMin, double* pVoltageActMax,
                           double* pCurrentActMin, double* pCurrentActMax,
                           double* pPowerActMin, double* pPowerActMax);
DLL_RESULT TC4GetUIPMinMaxSettings(double* pMinMaxTimeWindowInSeconds, unsigned int* pMixMaxFilterKoeff);
DLL_RESULT TC4SetUIPMinMaxSettings(double minMaxTimeWindowInSeconds, unsigned int mixMaxFilterKoeff);

DLL_RESULT TC4GetUIPActFilterBW(unsigned int* pFilterBW);
DLL_RESULT TC4SetUIPActFilterBW(unsigned int filterBW);

DLL_RESULT TC4GetX105AppDigIn1OutputSelector(unsigned int* pOutputSelector);
DLL_RESULT TC4SetX105AppDigIn1OutputSelector(unsigned int outputSelector);

DLL_RESULT TC4GetX105AppDigIn1ManualOutputState(unsigned int* pManualOutputState);
DLL_RESULT TC4SetX105AppDigIn1ManualOutputState(unsigned int manualOutputState);

//****************************************************************************
//** TC4__ regen / TC42__ GSS extension                                      *
//****************************************************************************

/*!
 * @brief Set P-Gain of Q4 voltage controller [0...32767]
 * @ingroup ControllerSettings
 * @param[in] gain [#]
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC42SetQ4ControllerVoltagePGain(short gain);

/*!
 * @ingroup ControllerSettings
 * @brief Get P-Gain of Q4 voltage controller
 * @param[out] pGain [#]
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC42GetQ4ControllerVoltagePGain(short* pGain);

/*!
 * @ingroup ControllerSettings
 * @brief Set I-Gain of Q4 voltage controller [0...32767]
 * @param[in] gain [#]
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC42SetQ4ControllerVoltageIGain(short gain);

/*!
 * @ingroup ControllerSettings
 * @brief Get I-Gain of Q4 voltage controller
 * @param[out] pGain [#]
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC42GetQ4ControllerVoltageIGain(short* pGain);

/*!
 * @ingroup ControllerSettings
 * @brief Set Feedforward-Gain of Q4 voltage controller [0...32767]
 * @param[in] gain [#]
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC42SetQ4ControllerVoltageFFWDGain(short gain);

/*!
 * @ingroup ControllerSettings
 * @brief Get Feedforward-Gain of Q4 voltage controller
 * @param[out] pGain [#]
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC42GetQ4ControllerVoltageFFWDGain(short* pGain);

/*!
 * @ingroup ControllerSettings
 * @brief Set P-Gain of Q4 current controller [0...32767]
 * @param[in] gain [#]
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC42SetQ4ControllerCurrentPGain(short gain);

/*!
 * @ingroup ControllerSettings
 * @brief Get P-Gain of Q4 current controller
 * @param[out] pGain [#]
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC42GetQ4ControllerCurrentPGain(short* pGain);

/*!
 * @ingroup ControllerSettings
 * @brief Set I-Gain of Q4 current controller [0...32767]
 * @param[in] gain [#]
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC42SetQ4ControllerCurrentIGain(short gain);

/*!
 * @ingroup ControllerSettings
 * @brief Get I-Gain of Q4 current controller
 * @param[out] pGain [#]
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC42GetQ4ControllerCurrentIGain(short* pGain);

/*!
 * @ingroup ControllerSettings
 * @brief Set Feedforward-Gain of Q4 current controller [0...32767]
 * @param[in] gain [#]
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC42SetQ4ControllerCurrentFFWDGain(short gain);

/*!
 * @ingroup ControllerSettings
 * @brief Get Feedforward-Gain of Q4 current controller
 * @param[out] pGain [#]
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC42GetQ4ControllerCurrentFFWDGain(short* pGain);

/*!
 * @ingroup ControllerSettings
 * @brief Set P-Gain of Q4 power controller [0...32767]
 * @param[in] gain [#]
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC42SetQ4ControllerPowerPGain(short gain);

/*!
 * @ingroup ControllerSettings
 * @brief Get P-Gain of Q4 power controller
 * @param[out] pGain [#]
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC42GetQ4ControllerPowerPGain(short* pGain);

/*!
 * @ingroup ControllerSettings
 * @brief Set I-Gain of Q4 power controller [0...32767]
 * @param[in] gain [#]
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC42SetQ4ControllerPowerIGain(short gain);

/*!
 * @ingroup ControllerSettings
 * @brief Get I-Gain of Q4 power controller
 * @param[out] pGain [#]
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC42GetQ4ControllerPowerIGain(short* pGain);

/*!
 * @ingroup ReferenceValues
 * @brief Set Q4 controller voltage limit (parameter must be positive!)
 * @param[in] voltage PhysicalVoltage [V]
 * @note This is only possible if it is a GSS Device or a ReGen Device with<br>
 * the Version >= 121068 and Version <= 121099
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC42SetQ4LimitVoltage(double voltage);

/*!
 * @ingroup ReferenceValues
 * @brief Get Q4 controller current limit (parameter will be positive)
 * @param[out] pVoltage PhysicalVoltage [V]
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC42GetQ4LimitVoltage(double* pVoltage);

/*!
 * @ingroup ReferenceValues
 * @brief Set Q4 controller current limit (parameter must be negative!))
 * @param[in] current PhysicalCurrent [A]
 * @note This is only possible if it is a GSS Device or a ReGen Device with<br>
 * the Version >= 121068 and Version <= 121099
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC4SetQ4LimitCurrent(double current);

/*!
 * @ingroup ReferenceValues
 * @brief Get Q4 controller current limit (parameter will be negative)
 * @param[out] pCurrent PhysicalCurrent [A]
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC4GetQ4LimitCurrent(double* pCurrent);

/*!
 * @ingroup ReferenceValues
 * @brief Set Q4 controller power limit (parameter must be negative!)
 * @param[in] power PhysicalPower[kW]
 * @note This is only possible if it is a GSS Device or a ReGen Device with
 * the Version >= 121068 and Version <= 121099
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC4SetQ4LimitPower(double power);

/*!
 * @ingroup ReferenceValues
 * @brief Get Q4 controller power limit (parameter will be negative)
 * @param[out] pPower PhysicalPower [kW]
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC4GetQ4LimitPower(double* pPower);

/*!
 * @ingroup Status
 * @brief read if device is operating in Q4 mode (sinking)<br>
 * Always reads zero if device is not a GSS device
 * @param[out] pActive
 * 1: Enabled<br>
 * 0: Disabled
 * @note This is only possible if it is a GSS Device or a ReGen Device with the
 * (Version >= 111000 and Version <= 111099) or (Version >= 121000 and Version <= 121099)
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC4GetQ4ControllerActive(unsigned short* pActive);

/*!
 * @ingroup ControllerSettings
 * @brief Set operating mode to Q1 only.<br>
 * Be sure to not disable operating range to Q4 only at the same time.<br>
 * Otherwise output is disabled (as nether Q1 nor Q4 operating mode is allowed)
 * @param[in] enable
 * 0: false<br>
 * 1: true
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC4SetOperatingModeQ1Only(unsigned short enable);

/*!
 * @ingroup ControllerSettings
 * @brief Query if operating mode is limited to Q1 only
 * @param[out] pEnable
 * 0: false<br>
 * 1: true
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC4GetOperatingModeQ1Only(unsigned short* pEnable);

/*!
 * @ingroup ControllerSettings
 * @brief Set operating mode to Q4 only.<br>
 * Be sure to not disable operating range to Q1 only at the same time.<br>
 * Otherwise output is disabled (as nether Q1 nor Q4 operating mode is allowed)
 * @param[in] enable
 * 0: false<br>
 * 1: true
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC4SetOperatingModeQ4Only(unsigned short enable);

/*!
 * @ingroup ControllerSettings
 * @brief Query if operating mode is limited to Q4 only
 * @param[out] pEnable
 * 0: false<br>
 * 1: true
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC4GetOperatingModeQ4Only(unsigned short* pEnable);

/*!
 * @ingroup DeviceInfo
 * @brief Query if Q4 user limits are supported
 * @param[out] pIsSupported
 * 0: false<br>
 * 1: true
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC4GetIsSupportedQ4UserLimits(unsigned short* pIsSupported);

/*!
 * @ingroup DeviceInfo
 * @brief Query if Q4 voltage user limit is supported
 * @param[out] pIsSupported
 * 0: false<br>
 * 1: true
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC42GetIsSupportedQ4VoltageUserLimit(unsigned short* pIsSupported);

/*!
 * @ingroup DeviceInfo
 * @brief Query if Q4 current sink is supported
 * @param[out] pIsSupported
 * 0: false<br>
 * 1: true
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC4GetIsSupportedQ4CurrentSinkActiveQuery(unsigned short* pIsSupported);

/*!
 * @ingroup DeviceInfo
 * @brief Query if voltage on hours counter is supported
 * @param[out] pIsSupported
 * 0: false<br>
 * 1: true
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC4GetIsSupportedVoltageOnHoursCounter(unsigned short* pIsSupported);

/*!
 * @ingroup DeviceInfo
 * @brief Query if limiting operating mode to Q1only and Q4only is supported
 * @param[out] pIsSupported
 * 0: false<br>
 * 1: true
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC4GetIsSupportedQ1Q4LimitMode(unsigned short* pIsSupported);

//****************************************************************************
//** TC42__ GSS extension                                                    *
//****************************************************************************

/*!
 * @ingroup DeviceInfo
 * @brief Get operating hours the device was voltage-on
 * @param[out] pOperatingHoursAtVoltageOn voltage-on operating hours [0...596523h]
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC42GetOperatingHoursAtVoltageOn(float* pOperatingHoursAtVoltageOn);

/*!
 * @ingroup DeviceInfo
 * @brief Read out the PLD version
 * @param[out] pVersionPLD: PLD version (e.g. 123 means V1.23)
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC42GetFirmwareVersionPLD(unsigned short* pVersionPLD);

/*!
 * @ingroup Status
 * @brief Get internal controllerboard temperature
 * @param[out] pTemperature [Degree-Celsius]
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC42GetTemperaturePCB(double* pTemperature);

/*!
 * @ingroup ActualValues
 * @brief Get actual current in sinking operation (Q4).
 * Reads zero in Q1 operation.<br>
 * Use function GetCurrentAct instead to get actual current in bidirectional
 * operation (Q1 and Q4)
 * @param[out] pCurrent [A]
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC42GetQ4CurrentAct(double* pCurrent);

/*!
 * @ingroup DeviceInfo
 * @brief Get info if device is a GSS device or not
 * @param[out] pEnabled
 * 1: Enabled<br>
 * 0: Disabled
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC42GetIsGSSDevice(unsigned short* pEnabled);

/*!
 * @ingroup DeviceInfo
 * @brief Get info if device is a GSS device and internal communication to
 * IBC board (needed for Q4 operation) is ok.
 * @param[out] pReady
 * 1: Enabled<br>
 * 0: Disabled
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC42GetIsGSSDeviceReady(unsigned short* pReady);

/*!
 * @ingroup DeviceInfo
 * @brief Get firmware version of IBC board (e.g. 123 means V1.23)<br>
 * Reads zero if device is not a GSS device.
 * @param[out] pVersion Version of IBC
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC42GetFirmwareVersionIBC(unsigned short* pVersion);

/*!
 * @ingroup Protection
 * @brief Set Q4 over current error limit for Q4 operating mode (negative!)
 * @param[in] Current [A]
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC42SetQ4LimitOverCurrentError(double Current);

/*!
 * @ingroup Protection
 * @brief Get Q4 over current error limit for Q4 operating mode (will be negative)
 * @param[out] pCurrent [A]
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC42GetQ4LimitOverCurrentError(double* pCurrent);

/*!
 * @ingroup Protection
 * @brief Set Q4 over current error delay for Q4 operating mode
 * @param[in] Time [ms]
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC42SetQ4LimitOverCurrentErrorDelay(double Time);

/*!
 * @ingroup Protection
 * @brief Get Q4 over current error delay for Q4 operating mode
 * @param[out] pTime [ms]
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC42GetQ4LimitOverCurrentErrorDelay(double* pTime);

/*!
 * @ingroup Protection
 * @brief Set Q4 over current warning limit for Q4 operating mode (negative!)
 * @param[in] Current [A]
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC42SetQ4LimitOverCurrentWarn(double Current);

/*!
 * @ingroup Protection
 * @brief Get Q4 over current warning limit for Q4 operating mode (negative!)
 * @param[out] pCurrent [A]
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC42GetQ4LimitOverCurrentWarn(double* pCurrent);

/*!
 * @ingroup Protection
 * @brief Set Q4 over current warning delay for Q4 operating mode
 * @param[in] Time [ms]
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC42SetQ4LimitOverCurrentWarnDelay(double Time);

/*!
 * @ingroup Protection
 * @brief Get Q4 over current warning delay for Q4 operating mode
 * @param[out] pTime [ms]
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC42GetQ4LimitOverCurrentWarnDelay(double* pTime);

/*!
 * @ingroup Protection
 * @brief Get Q4 over current absolute error limit for Q4 operating mode
 * (Q4 over current limit settings are limited to this value)
 * @param[out] pCurrent [A]
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC42GetQ4LimitOverCurrentAbs(double* pCurrent);

/*!
 * @ingroup Protection
 * @brief This function reads the actual protection limits for output-undervoltage.
 * @param[out] pLimit Actual undervoltage error level [V]
 * @param[out] pDelay Actual delay for the error level in [50us]
 * @param[out] pWarn Actual undervoltage warning level [V]
 * @param[out] pWarnDelay Actual delay for the warning level in [50us]
 * @param[out] pEnableDelay Time for activation of undervoltage monitoring, after voltage on [ms]
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
*/
DLL_RESULT TC42GetUnderVoltageParam(double* pLimit, unsigned short* pDelay, double* pWarn, unsigned short* pWarnDelay, unsigned short* pEnableDelay);

/*!
 * @ingroup Protection
 * @brief This function sets the actual protection limits for output-undervoltage.
 * @param[in] limit Actual undervoltage error level [V]
 * @param[in] delay Actual delay for the error level in [50us]
 * @param[in] warn Actual undervoltage warning level [V]
 * @param[in] warnDelay Actual delay for the warning level in [50us]
 * @param[in] enableDelay Time for activation of undervoltage monitoring, after voltage on [ms]
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
*/
DLL_RESULT TC42SetUnderVoltageParam(double limit, unsigned short delay, double warn, unsigned short warnDelay, unsigned short enableDelay);

/*!
 * @ingroup Protection
 * @brief This function gets the actual protection limits for output-power overload in Q1.
 * @param[out] pLimit Actual power overload error level [kW]
 * @param[out] pDelay Actual delay for the error level in [50us]
 * @param[out] pWarn Actual power overload warning level [kW]
 * @param[out] pWarnDelay Actual delay for the warning level in [50us]
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
*/
DLL_RESULT TC42GetOverLoadPowerQ1Param(double* pLimit, unsigned short* pDelay, double* pWarn, unsigned short* pWarnDelay);

/*!
 * @ingroup Protection
 * @brief This function sets the actual protection limits for output-power overload in Q1.
 * @param[in] limit Actual power overload error level [kW]
 * @param[in] delay Actual delay for the error level in [50us]
 * @param[in] warn Actual power overload warning level [kW]
 * @param[in] warnDelay Actual delay for the warning level in [50us]
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
*/
DLL_RESULT TC42SetOverLoadPowerQ1Param(double limit, unsigned short delay, double warn, unsigned short warnDelay);

/*!
 * @ingroup Protection
 * @brief This function gets the actual protection limits for output-power overload in Q4.
 * @param[out] pLimit Actual power overload error level [kW]
 * @param[out] pDelay Actual delay for the error level in [50us]
 * @param[out] pWarn Actual power overload warning level [kW]
 * @param[out] pWarnDelay Actual delay for the warning level in [50us]
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
*/
DLL_RESULT TC42GetOverLoadPowerQ4Param(double* pLimit, unsigned short* pDelay, double* pWarn, unsigned short* pWarnDelay);

/*!
 * @ingroup Protection
 * @brief This function sets the actual protection limits for output-power overload in Q4.
 * @param[in] limit Actual power overload error level [kW]
 * @param[in] delay Actual delay for the error level in [50us]
 * @param[in] warn Actual power overload warning level [kW]
 * @param[in] warnDelay Actual delay for the warning level in [50us]
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
*/
DLL_RESULT TC42SetOverLoadPowerQ4Param(double limit, unsigned short delay, double warn, unsigned short warnDelay);

/*!
* @ingroup Protection
* @brief This function is used to check if the extended protect is supported.
* (output-undervoltage, output-power overload in Q1 and output-power overload in Q4)
* @param[out] pSupport 0: false<br>
* 1: true
* @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
*/
DLL_RESULT TC42GetExtenedProtectSupported(unsigned short* pSupport);

/*!
* @ingroup Protection
* @brief This function is used to get the maximum allowed power of Q1 and Q4.
* @param[out] pPabsQ1 Maximum allowed power of Q1
* @param[out] pPabsQ4 Maximum allowed power of Q4
* @attention These values are not physical values - so translate to correct
* physical values
* @see chapter Translate between standardised values and physical values
* @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
*/
DLL_RESULT TC42GetMaximasPower(short* pPabsQ1, short* pPabsQ4);
//****************************************************************************
//** CapSim                                                                  *
//****************************************************************************

/*!
 * @ingroup OptionCapSim
 * @brief check if TopCon device has CapSim supported
 * @param[out] pSupport
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC42GetCapSimOptionSupported(unsigned short* pSupport);
DLL_RESULT TC42SetEDLCSimEnable(unsigned short enable);
DLL_RESULT TC42GetEDLCSimEnable(unsigned short* pEnable);
DLL_RESULT TC42SetEDLCSimMainCapacity(double capacity);
DLL_RESULT TC42GetEDLCSimMainCapacity(double* pCapacity);
DLL_RESULT TC42SetEDLCSimDynamicCapacity(double capacity);
DLL_RESULT TC42GetEDLCSimDynamicCapacity(double* pCapacity);
DLL_RESULT TC42SetEDLCSimMainResistance(double resistance);
DLL_RESULT TC42GetEDLCSimMainResistance(double* pResistance);
DLL_RESULT TC42SetEDLCSimDynamicResistance(double resistance);
DLL_RESULT TC42GetEDLCSimDynamicResistance(double* pResistance);
DLL_RESULT TC42SetEDLCSimESR(double resistance);
DLL_RESULT TC42GetEDLCSimESR(double* pResistance);
DLL_RESULT TC42SetEDLCSimApplyNewKoeff();

//****************************************************************************
//** TCIBC functions                                                         *
//****************************************************************************

/*!
 * @ingroup IBCDeviceInfo
 * @brief Get firmware version of IBC board (e.g. 123 means V1.23)
 * @param[out] *pVersion
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TCIBCGetFirmwareVersion(unsigned short* pVersion);

/*!
 * @ingroup IBCActualValues
 * @brief Get state of IBC board
 * @param[out] pState Possible values:<br>
 * 0x0001: invalid state<br>
 * 0x0004: power up<br>
 * 0x0010: ready<br>
 * 0x0100: running (power stage enabled)<br>
 * 0x0400: warning (power stage enabled)<br>
 * 0x1000: error<br>
 * 0x4000: stopped<br>
 * 0x8000: firmware update mode (communication not posible)
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TCIBCGetState(unsigned short* pState);

/*!
 * @ingroup IBCActualValues
 * @brief Get transformer secondary current
 * @param[out] pCurrent [A]
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TCIBCGetTransformerSecondaryCurrent(double* pCurrent);

/*!
 * @brief Get booster reactor current
 * @ingroup IBCActualValues
 * @param[out] pCurrent [A]
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TCIBCGetBoosterReactorCurrent(double* pCurrent);

/*!
 * @ingroup IBCActualValues
 * @brief Get Clamp Voltage
 * @param[out] pVoltage [V]
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TCIBCGetClampVoltage(double* pVoltage);

/*!
 * @ingroup IBCDeviceInfo
 * @brief Get inverter firmware version (e.g. 123 means V1.23)
 * @param[out] pVersion
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TCIBCGetFirmwareVersionInverter(unsigned long* pVersion);

/*!
 * @ingroup IBCActualValues
 * @brief Get supply voltages(5V, 15V, 24V)
 * @param[out] p5V [V]
 * @param[out] p15V [V]
 * @param[out] p24V [V]
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TCIBCGetSupplyVoltages(double* p5V, double* p15V, double* p24V);

/*!
 * @ingroup IBCActualValues
 * @brief Get Digital Inputs
 * @param[out] pInputs [binary]
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TCIBCGetDigitalInputs(unsigned long* pInputs);

/*!
 * @ingroup IBCActualValues
 * @brief Get maximum allowed inverter switching frequency
 * @param[out] pFrequency electable levels<br>
 * 0:  8kHz<br>
 * 1:  12kHz<br>
 * 2:  16kHz<br>
 * 3:  20kHz<br>
 * 4:  9kHz<br>
 * 5:  10kHz<br>
 * 6:  11kHz<br>
 * 7:  13kHz<br>
 * 8:  14kHz<br>
 * 9:  15kHz<br>
 * 10: 17kHz<br>
 * 11: 18kHz<br>
 * 12: 19kHz
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TCIBCGetInverterSwitchingFrequencyMax(unsigned short* pFrequency);

/*!
 * @ingroup IBCActualValues
 * @brief Get inverter line frequency
 * @param[out] pFrequency [Hz]
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TCIBCGetInverterLineFrequency(float* pFrequency);

/*!
 * @ingroup IBCActualValues
 * @brief Get inverter RMS values of line voltages L1, L2, L3
 * @param[out] pUL1 [V]
 * @param[out] pUL2 [V]
 * @param[out] pUL3 [V]
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TCIBCGetInverterLineVoltageRMS(float* pUL1, float* pUL2, float* pUL3);

/*!
 * @ingroup IBCActualValues
 * @brief Get inverter RMS values of line currents L1, L2, L3
 * @param[out] pCL1 [A]
 * @param[out] pCL2 [A]
 * @param[out] pCL3 [A]
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TCIBCGetInverterLineCurrentRMS(float* pCL1, float* pCL2, float* pCL3);

/*!
 * @ingroup IBCActualValues
 * @brief Get inverter RMS values of line powers L1, L2, L3
 * @param[out] pPL1 [kW]
 * @param[out] pPL2 [kW]
 * @param[out] pPL3 [kW]
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TCIBCGetInverterLinePowerRMS(float* pPL1, float* pPL2, float* pPL3);

/*!
 * @ingroup IBCActualValues
 * @brief Get temperature of inverter heatsink
 * @param[out] pTemperatur [Degree-Celsius]
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TCIBCGetInverterTemperatureHeatsink(float* pTemperatur);

/*!
 * @ingroup IBCActualValues
 * @brief Get power factors of lines L1, L2, L3
 * @param[out] pAvgL1 [#]
 * @param[out] pAvgL2 [#]
 * @param[out] pAvgL3 [#]
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TCIBCGetInverterLinePowerFactor(float* pAvgL1, float* pAvgL2, float* pAvgL3);

//****************************************************************************
//** TC.RSC functions                                                        *
//****************************************************************************

/*!
 * @ingroup RemoteSystemConfiguration
 * @brief Initialising the remote system configuration (RSC)
 * @param[in]  pStrPathConfigFile: Path where the conifguration file is saved
 * @param[out] pRSCInitState returns the initialisation state<br>
 * -8: Switchbox ID's from device and from configuration file not identical<br>
 * -7: Reading configuration file failed<br>
 * -6: Invalid configuration file name<br>
 * -5: Serial number from device and from configuration file not identical<br>
 * -4: Set switchbox data failed<br>
 * -3: Set device data failed<br>
 * -2: Device is a slave or a subsystem-master<br>
 * -1: RSC not enabled or supported<br>
 *  0: RSC initialisation successful<br>
 *  1: RSC initialisation is running
 * @attention Every time a connection to a device is established,
 * this function must be called before a reconfiguration of the system can be performed.
 * @note The name of the configuration file must consist of the following parts:<br>
 * - Part 1: The naming: "RemoteSystemConfiguration"<br>
 * - Part 2: Serial number of the connected device (e.g. 1640CC787)<br>
 * - Part 3: File extension .ini<br>
 * Example for a configuration file name: RemoteSystemConfiguration1640CC787.ini
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC42RSCInitialisation(char *pStrPathConfigFile, short *pRSCInitState);


/*!
 * @ingroup RemoteSystemConfiguration
 * @brief Get the state of the system configuration and the switchbox
 * @param[out] pSystemConfigState returns the state of the system configuration<br>
 * 0: System configuration failed<br>
 * 1: System configuration successful
 * @param[out] pSwitchBoxState returns the switch box state
 * 0: Switchbox state not ok<br>
 * 1: Switchbx state ok
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC42RSCGetConfigurationAndSwitchboxState(unsigned short *pSystemConfigState,
                                                    unsigned short *pSwitchBoxState);


/*!
 * @ingroup RemoteSystemConfiguration
 * @brief Get the actual remote system configuration (RSC)
 * @param[in]  descriptionStrSize: Length (number of bytes) of the string for saving
 * the actual configuration description
 * @param[out] pStrActConfigDescription returns the string with the actual configuration
 * description
 * @param[out] pActConfigNumber returns the actual configuration number
 * @param[out] pActRedirectToSubSystem returns if the actual configuration is valid
 * for TC.MAC system, subsystems or master-slave system<br>
 * 0: Configuration valid for TC.MAC system<br>
 * 1: Configuration valid for subsystems or master-slave system
 * @param[out] pActMatrixConnectionType returns the actual system connection type
 * 0: Parallel connection of serial lines
 * 1: Series connection of parallel strings
 * @param[out] pDefaultSwitchBoxID returns the switchbox ID which is connected to
 * the system
 * @note The parameter pActRedirectToSubSystem has only an effect on a TC.MAC.<br>
 * The parameter pActMatrixConnectionType has only an effect in a matrix system
 * (not only parallel or only serial connected devices).
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC42RSCGetActualConfiguration(unsigned short descriptionStrSize, char *pStrActConfigDescription,
                                         unsigned short *pActConfigNumber, unsigned short *pActRedirectToSubSystem,
                                         unsigned short *pActMatrixConnectionType, unsigned short *pDefaultSwitchBoxID);


/*!
 * @ingroup RemoteSystemConfiguration
 * @brief Get informations about the configurations in the configuration file
 * @param[out] pSwitchBoxID returns the switchbox ID for which the configurations
 * in the configuration file are valid
 * @param[out] pRedirectToSubSystem returns if the configurations are valid
 * for TC.MAC system, subsystems or master-slave system<br>
 * 0: Configurations valid for TC.MAC system<br>
 * 1: Configurations valid for subsystems or master-slave system
 * @param[out] pNumberOfConfigurations returns the number of configurations in
 * the configuration file
 * @note The parameter pRedirectToSubSystem has only an effect on a TC.MAC.
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC42RSCGetConfigurationFileInfo(unsigned short *pSwitchBoxID, unsigned short *pRedirectToSubSystem,
                                           unsigned short *pNumberOfConfigurations);


/*!
 * @ingroup RemoteSystemConfiguration
 * @brief Get the description of a configuration
 * @param[in]  configurationNumber: Configuration number to select the description
 * @param[in]  descriptionStrSize: Length (number of bytes) of the string for saving
 * the configuration description
 * @param[out] pStrConfigDescription returns the string with the configuration
 * description, depending on the configuration number
 * @param[out] pMatrixConnectionType returns the system connection type
 * 0: Parallel connection of serial lines
 * 1: Series connection of parallel strings
 * @note The parameter pMatrixConnectionType has only an effect in a matrix system
 * (not only parallel or only serial connected devices).
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC42RSCGetConfigurationDescription(unsigned short configurationNumber, unsigned short descriptionStrSize,
                                              char *pStrConfigDescription, unsigned short *pMatrixConnectionType);


/*!
 * @ingroup RemoteSystemConfiguration
 * @brief Set a configuration
 * @param[in]  configurationNumber: Number of configuration to set
 * @param[in]  automaticRestart: Automatic restart of the system
 * 0: System must be restarted manually
 * 1: System restarts automatically
 * @param[out] pSetConfigState returns the set configuration state<br>
 * -4: Write command restart failed<br>
 * -3: Save configuration failed<br>
 * -2: Saving the configuration not possible until output is turned off<br>
 * -1: Configuration number out of range<br>
 *  1: Set configuration running<br>
 *  2: Configuration successfully saved<br>
 *  3: System is restarting
 * @attention The saved configuration will only be active after the system is
   restarted.<br>
   The system must be reconnected after restarting.
 * @returns \c DLL_SUCCESS for success or \c DLL_FAIL or other value if an error occurs
 */
DLL_RESULT TC42RSCSetConfiguration(unsigned short configurationNumber, unsigned short automaticRestart,
                                   short *pSetConfigState);

//****************************************************************************
#endif //SERIAL_IO_LIB_H__D9D3E9B6_D8C3_4599_8ED3_72949486284F__INCLUDED
