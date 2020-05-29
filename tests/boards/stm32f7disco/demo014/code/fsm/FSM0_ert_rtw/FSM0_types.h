//
// Non-Degree Granting Education License -- for use at non-degree
// granting, nonprofit, educational organizations only. Not for
// government, commercial, or other organizational use.
//
// File: FSM0_types.h
//
// Code generated for Simulink model 'FSM0'.
//
// Model version                  : 1.909
// Simulink Coder version         : 9.3 (R2020a) 18-Nov-2019
// C/C++ source code generated on : Tue May 26 21:16:38 2020
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

#ifndef DEFINED_TYPEDEF_FOR_ControlMode_
#define DEFINED_TYPEDEF_FOR_ControlMode_

typedef enum {
  IDLE = 0,                            // Default value
  CPAP,
  VCV,
  PRVC
} ControlMode;

#endif

#ifndef DEFINED_TYPEDEF_FOR_StateLed_
#define DEFINED_TYPEDEF_FOR_StateLed_

typedef enum {
  Off = 0,                             // Default value
  On
} StateLed;

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
  MAP_Transfer_Test,
  MAP_ALARMS_1,
  MAP_ALARMS_2
} MapIndex;

#endif

#ifndef DEFINED_TYPEDEF_FOR_AlarmIndex_
#define DEFINED_TYPEDEF_FOR_AlarmIndex_

typedef enum {
  ALARM_None = 0,                      // Default value
  ALARM_PressureDrop,
  ALARM_PowerSupply,
  ALARM_BreakPR1,
  ALARM_OverPressureInlet,
  ALARM_PEEP,
  ALARM_LowVolume,
  ALARM_Oxygen
} AlarmIndex;

#endif

#ifndef struct_tag_PMfBDzoakfdM9QAdfx2o6D
#define struct_tag_PMfBDzoakfdM9QAdfx2o6D

struct tag_PMfBDzoakfdM9QAdfx2o6D
{
  uint32_T f1[8];
};

#endif                                 //struct_tag_PMfBDzoakfdM9QAdfx2o6D

#ifndef typedef_cell_wrap_FSM0_T
#define typedef_cell_wrap_FSM0_T

typedef struct tag_PMfBDzoakfdM9QAdfx2o6D cell_wrap_FSM0_T;

#endif                                 //typedef_cell_wrap_FSM0_T

#ifndef struct_tag_ZiYJHmAzopopP55emjm3BH
#define struct_tag_ZiYJHmAzopopP55emjm3BH

struct tag_ZiYJHmAzopopP55emjm3BH
{
  int32_T isInitialized;
  boolean_T isSetupComplete;
  real32_T pWinLen;
  real32_T pBuf[128];
  real32_T pHeap[128];
  real32_T pMidHeap;
  real32_T pIdx[4];
  real32_T pPos[128];
  real32_T pMinHeapLength;
  real32_T pMaxHeapLength;
};

#endif                                 //struct_tag_ZiYJHmAzopopP55emjm3BH

#ifndef typedef_c_dsp_private_MedianFilterCG__T
#define typedef_c_dsp_private_MedianFilterCG__T

typedef struct tag_ZiYJHmAzopopP55emjm3BH c_dsp_private_MedianFilterCG__T;

#endif                                 //typedef_c_dsp_private_MedianFilterCG__T

#ifndef struct_tag_ux3tMAblms1yxl6Ma1ZSY
#define struct_tag_ux3tMAblms1yxl6Ma1ZSY

struct tag_ux3tMAblms1yxl6Ma1ZSY
{
  boolean_T matlabCodegenIsDeleted;
  int32_T isInitialized;
  boolean_T isSetupComplete;
  cell_wrap_FSM0_T inputVarSize;
  int32_T NumChannels;
  c_dsp_private_MedianFilterCG__T pMID;
};

#endif                                 //struct_tag_ux3tMAblms1yxl6Ma1ZSY

#ifndef typedef_dsp_MedianFilter_FSM0_T
#define typedef_dsp_MedianFilter_FSM0_T

typedef struct tag_ux3tMAblms1yxl6Ma1ZSY dsp_MedianFilter_FSM0_T;

#endif                                 //typedef_dsp_MedianFilter_FSM0_T
#endif                                 // RTW_HEADER_FSM0_types_h_

//
// File trailer for generated code.
//
// [EOF]
//
