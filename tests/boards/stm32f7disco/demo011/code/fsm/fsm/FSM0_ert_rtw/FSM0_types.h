//
// Non-Degree Granting Education License -- for use at non-degree
// granting, nonprofit, educational organizations only. Not for
// government, commercial, or other organizational use.
//
// File: FSM0_types.h
//
// Code generated for Simulink model 'FSM0'.
//
// Model version                  : 1.697
// Simulink Coder version         : 9.3 (R2020a) 18-Nov-2019
// C/C++ source code generated on : Thu May  7 10:44:33 2020
//
// Target selection: ert.tlc
// Embedded hardware selection: Intel->x86-64 (Windows64)
// Code generation objectives: Unspecified
// Validation result: Not run
//
#ifndef RTW_HEADER_FSM0_types_h_
#define RTW_HEADER_FSM0_types_h_
#include "rtwtypes.h"

// Model Code Variants
#ifndef DEFINED_TYPEDEF_FOR_StateButton_
#define DEFINED_TYPEDEF_FOR_StateButton_

typedef enum {
  Released = 0,                        // Default value
  Pressed
} StateButton;

#endif

#ifndef DEFINED_TYPEDEF_FOR_StateLed_
#define DEFINED_TYPEDEF_FOR_StateLed_

typedef enum {
  Off = 0,                             // Default value
  On
} StateLed;

#endif

#ifndef DEFINED_TYPEDEF_FOR_ControlMode_
#define DEFINED_TYPEDEF_FOR_ControlMode_

typedef enum {
  IDLE = 0,                            // Default value
  CPAP,
  VCV,
  PRVC
} ControlMode;

#endif

#ifndef DEFINED_TYPEDEF_FOR_MapIndex_
#define DEFINED_TYPEDEF_FOR_MapIndex_

typedef enum {
  MAP_Transfer_None = 0,               // Default value
  MAP_Transfer_Standard,
  MAP_Transfer_VCV,
  MAP_Transfer_PRVC_1,
  MAP_Transfer_PRVC_2,
  MAP_Transfer_AutoTrigger,
  MAP_Transfer_Test
} MapIndex;

#endif
#endif                                 // RTW_HEADER_FSM0_types_h_

//
// File trailer for generated code.
//
// [EOF]
//
