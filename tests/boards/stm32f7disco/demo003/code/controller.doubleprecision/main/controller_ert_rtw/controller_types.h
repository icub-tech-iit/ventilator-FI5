//
// Non-Degree Granting Education License -- for use at non-degree
// granting, nonprofit, educational organizations only. Not for
// government, commercial, or other organizational use.
//
// File: controller_types.h
//
// Code generated for Simulink model 'controller'.
//
// Model version                  : 1.141
// Simulink Coder version         : 9.3 (R2020a) 18-Nov-2019
// C/C++ source code generated on : Fri Apr 10 12:31:52 2020
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
#ifndef struct_tag_VDe5tX5WXsdfXSZIK6zY8B
#define struct_tag_VDe5tX5WXsdfXSZIK6zY8B

struct tag_VDe5tX5WXsdfXSZIK6zY8B
{
  int32_T S0_isInitialized;
  real_T W0_ZERO_STATES[2];
  real_T W1_POLE_STATES[2];
  int32_T W2_PreviousNumChannels;
  real_T P0_ICRTP;
  real_T P1_RTP1COEFF[3];
  real_T P2_RTP2COEFF[2];
  real_T P3_RTP3COEFF[2];
  boolean_T P4_RTP_COEFF3_BOOL[2];
  real_T P5_IC2RTP;
};

#endif                                 //struct_tag_VDe5tX5WXsdfXSZIK6zY8B

#ifndef typedef_b_dsp_BiquadFilter_0_controll_T
#define typedef_b_dsp_BiquadFilter_0_controll_T

typedef struct tag_VDe5tX5WXsdfXSZIK6zY8B b_dsp_BiquadFilter_0_controll_T;

#endif                                 //typedef_b_dsp_BiquadFilter_0_controll_T

#ifndef struct_tag_kdevC3uom1kByYeBjpmd4F
#define struct_tag_kdevC3uom1kByYeBjpmd4F

struct tag_kdevC3uom1kByYeBjpmd4F
{
  boolean_T matlabCodegenIsDeleted;
  int32_T isInitialized;
  boolean_T isSetupComplete;
  b_dsp_BiquadFilter_0_controll_T cSFunObject;
};

#endif                                 //struct_tag_kdevC3uom1kByYeBjpmd4F

#ifndef typedef_b_dspcodegen_BiquadFilter_con_T
#define typedef_b_dspcodegen_BiquadFilter_con_T

typedef struct tag_kdevC3uom1kByYeBjpmd4F b_dspcodegen_BiquadFilter_con_T;

#endif                                 //typedef_b_dspcodegen_BiquadFilter_con_T

#ifndef struct_tag_yiBU5azbGqcZi4dsJZv4qF
#define struct_tag_yiBU5azbGqcZi4dsJZv4qF

struct tag_yiBU5azbGqcZi4dsJZv4qF
{
  boolean_T matlabCodegenIsDeleted;
  int32_T isInitialized;
  boolean_T isSetupComplete;
  boolean_T TunablePropsChanged;
  int32_T NumChannels;
  b_dspcodegen_BiquadFilter_con_T *FilterObj;
};

#endif                                 //struct_tag_yiBU5azbGqcZi4dsJZv4qF

#ifndef typedef_dsp_LowpassFilter_controller_T
#define typedef_dsp_LowpassFilter_controller_T

typedef struct tag_yiBU5azbGqcZi4dsJZv4qF dsp_LowpassFilter_controller_T;

#endif                                 //typedef_dsp_LowpassFilter_controller_T
#endif                                 // RTW_HEADER_controller_types_h_

//
// File trailer for generated code.
//
// [EOF]
//
