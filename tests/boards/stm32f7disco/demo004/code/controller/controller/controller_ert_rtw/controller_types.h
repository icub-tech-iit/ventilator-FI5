//
// Non-Degree Granting Education License -- for use at non-degree
// granting, nonprofit, educational organizations only. Not for
// government, commercial, or other organizational use.
//
// File: controller_types.h
//
// Code generated for Simulink model 'controller'.
//
// Model version                  : 1.171
// Simulink Coder version         : 9.3 (R2020a) 18-Nov-2019
// C/C++ source code generated on : Sun Apr 12 12:02:58 2020
//
// Target selection: ert.tlc
// Embedded hardware selection: Intel->x86-64 (Windows64)
// Code generation objectives: Unspecified
// Validation result: Not run
//
#ifndef RTW_HEADER_controller_types_h_
#define RTW_HEADER_controller_types_h_
#include "rtwtypes.h"
#include "zero_crossing_types.h"

// Model Code Variants
#ifndef struct_tag_UuU9lPeGj5cFyj3YQzVuKB
#define struct_tag_UuU9lPeGj5cFyj3YQzVuKB

struct tag_UuU9lPeGj5cFyj3YQzVuKB
{
  int32_T S0_isInitialized;
  real32_T W0_ZERO_STATES[4];
  real32_T W1_POLE_STATES[4];
  int32_T W2_PreviousNumChannels;
  real32_T P0_ICRTP;
  real32_T P1_RTP1COEFF[6];
  real32_T P2_RTP2COEFF[4];
  real32_T P3_RTP3COEFF[3];
  boolean_T P4_RTP_COEFF3_BOOL[3];
  real32_T P5_IC2RTP;
};

#endif                                 //struct_tag_UuU9lPeGj5cFyj3YQzVuKB

#ifndef typedef_b_dsp_BiquadFilter_0_controll_T
#define typedef_b_dsp_BiquadFilter_0_controll_T

typedef struct tag_UuU9lPeGj5cFyj3YQzVuKB b_dsp_BiquadFilter_0_controll_T;

#endif                                 //typedef_b_dsp_BiquadFilter_0_controll_T

#ifndef struct_tag_Ogr6DMfaaQClSdW7dLFxqH
#define struct_tag_Ogr6DMfaaQClSdW7dLFxqH

struct tag_Ogr6DMfaaQClSdW7dLFxqH
{
  boolean_T matlabCodegenIsDeleted;
  int32_T isInitialized;
  boolean_T isSetupComplete;
  b_dsp_BiquadFilter_0_controll_T cSFunObject;
};

#endif                                 //struct_tag_Ogr6DMfaaQClSdW7dLFxqH

#ifndef typedef_b_dspcodegen_BiquadFilter_con_T
#define typedef_b_dspcodegen_BiquadFilter_con_T

typedef struct tag_Ogr6DMfaaQClSdW7dLFxqH b_dspcodegen_BiquadFilter_con_T;

#endif                                 //typedef_b_dspcodegen_BiquadFilter_con_T

#ifndef struct_tag_JwiFNpJGFOhIr5wM0kByJE
#define struct_tag_JwiFNpJGFOhIr5wM0kByJE

struct tag_JwiFNpJGFOhIr5wM0kByJE
{
  boolean_T matlabCodegenIsDeleted;
  int32_T isInitialized;
  boolean_T isSetupComplete;
  boolean_T TunablePropsChanged;
  int32_T NumChannels;
  b_dspcodegen_BiquadFilter_con_T *FilterObj;
};

#endif                                 //struct_tag_JwiFNpJGFOhIr5wM0kByJE

#ifndef typedef_dsp_LowpassFilter_controller_T
#define typedef_dsp_LowpassFilter_controller_T

typedef struct tag_JwiFNpJGFOhIr5wM0kByJE dsp_LowpassFilter_controller_T;

#endif                                 //typedef_dsp_LowpassFilter_controller_T
#endif                                 // RTW_HEADER_controller_types_h_

//
// File trailer for generated code.
//
// [EOF]
//
