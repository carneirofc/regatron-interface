# Regatron Notes
Version V3.86.00 24062019

## State of development

### One time
|Generic|Status|
|:----:|:----:|
|DLL Version            | ok |
|DSP Version            | ok |
|peripherieDSP          | ok |
|peripherieAuxiliaryDSP | ok |
|modulatorAuxiliaryDSP  | ok |
|bootloader             | ok |

 
### Monitor
|Generic|Status|
|:-----:|:----:|
| Temp IGBT         | ok |
| Temp Rect         | ok |
| Temp PCB          | ok |
| DCLink Volt       | ok |
| Primary Current   | ok |

|Sys/Mod Readings|
|:----:|:----:|
| Sys OutVoltage    | ok |
| Sys OutPower      | ok |
| Sys OutCurrent    | ok |
| Sys SimActualRes  | ok |
| Sys Error Tree    | ok |
| Sys Warn  Tree    | ok |
| Sys State         | ok |
| Mod OuVoltage     | ok |
| Mod OutPower      | ok |
| Mod OutCurrent    | ok |
| Mod Actual Res    | ok |
| Mod Error Tree    | ok |
| Mod Warn  Tree    | ok |
| Mod State         | ok |

#### Reference Values

#### Controller Settings
|Command|Status|
|:----: |:----:|
|Clear Error          | ok |
|Get Control Mode     | ok |
|Get Remove Ctrl. Inp | ok |
|Set Remote Ctrl. Inp |    |
|Store Parameters     | ok |    

### Readback / Setpoint

### Slopes

## Odd events

From `serialiolib.h`

What's the defference between `TC4GetPowerAct(double* pPowerAct)` and TC4GetPowerActSense `(double* pPowerActSense)`?

According to the docstring, the output value of `TC4GetPowerActSense(double*)` is `[out] pPowerActSense	Actual voltage value [kW] `. Is it a typo or the function returns a voltage? If It's a voltage, why choose GetPowerActSense as the function's name?

Can I trust that every value that depends on `ModuleSelector` is docummented? As in the function  `TC4GetPowerActSense(double*)`?
