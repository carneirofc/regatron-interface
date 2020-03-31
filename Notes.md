# Regatron Notes
Version V3.86.00 24062019

## Wierd shit

From `serialiolib.h`

What's the defference between `TC4GetPowerAct(double* pPowerAct)` and TC4GetPowerActSense `(double* pPowerActSense)`?

According to the docstring, the output value of `TC4GetPowerActSense(double*)` is `[out] pPowerActSense	Actual voltage value [kW] `. Is it a typo or the function returns a voltage? If It's a voltage, why choose GetPowerActSense as the function's name?

Can I trust that every value that depends on `ModuleSelector` is docummented? As in the function  `TC4GetPowerActSense(double*)`?