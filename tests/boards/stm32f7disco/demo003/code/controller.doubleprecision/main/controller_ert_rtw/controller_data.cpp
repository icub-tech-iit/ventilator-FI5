//
// Non-Degree Granting Education License -- for use at non-degree
// granting, nonprofit, educational organizations only. Not for
// government, commercial, or other organizational use.
//
// File: controller_data.cpp
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
#include "controller.h"
#include "controller_private.h"

// Block parameters (default storage)
controllerModelClass::P_controller_T controllerModelClass::controller_P = {
  // Mask Parameter: Controller_IE_ratio
  //  Referenced by:
  //    '<S1>/Pulse Generator'
  //    '<S4>/Volumetric Flow rate'

  0.33333333333333331,

  // Mask Parameter: PIDController_InitialConditionF
  //  Referenced by: '<S38>/Integrator'

  0.0,

  // Mask Parameter: PIDController_InitialConditio_c
  //  Referenced by: '<S89>/Integrator'

  0.0,

  // Mask Parameter: Controller_PC_FFW
  //  Referenced by: '<S3>/FFW'

  35.0,

  // Mask Parameter: Controller_PC_I_dw
  //  Referenced by: '<S3>/Constant5'

  100.0,

  // Mask Parameter: Controller_PC_I_e_thres_sched
  //  Referenced by: '<S3>/Relay'

  2.0,

  // Mask Parameter: Controller_PC_I_up
  //  Referenced by: '<S3>/Constant4'

  20.0,

  // Mask Parameter: Controller_PC_P
  //  Referenced by: '<S3>/Constant2'

  0.5,

  // Mask Parameter: Controller_PEEP
  //  Referenced by: '<S3>/Constant3'

  5.0,

  // Mask Parameter: Controller_PIP
  //  Referenced by: '<S3>/Gain'

  25.0,

  // Mask Parameter: Controller_VC_I
  //  Referenced by: '<S86>/Integral Gain'

  10.0,

  // Mask Parameter: Controller_VC_P
  //  Referenced by: '<S94>/Proportional Gain'

  0.0,

  // Mask Parameter: Controller_respiratory_rate
  //  Referenced by:
  //    '<S1>/Pulse Generator'
  //    '<S4>/Volumetric Flow rate'

  8.0,

  // Expression: 0
  //  Referenced by: '<S3>/Constant'

  0.0,

  // Expression: 100
  //  Referenced by: '<S3>/Saturation'

  100.0,

  // Expression: 0
  //  Referenced by: '<S3>/Saturation'

  0.0,

  // Expression: 0
  //  Referenced by: '<S3>/IV_P_ref'

  0.0,

  // Expression: 0
  //  Referenced by: '<S3>/IV_dc'

  0.0,

  // Expression: PC_IST_x0
  //  Referenced by: '<S3>/Constant1'

  -3391.1217215872048,

  // Expression: PC_IST_num
  //  Referenced by: '<S3>/Input Shaping'

  { -0.00018297384201481484, -0.00054892152604444451, -0.00054892152604444451,
    -0.00018297384201481484 },

  // Expression: PC_IST_den
  //  Referenced by: '<S3>/Input Shaping'

  { -8.7007778962242366, 24.661370076091732, -23.299953999143824,
    7.3378980285402076 },

  // Expression: 1
  //  Referenced by: '<S3>/Relay'

  1.0,

  // Expression: -1
  //  Referenced by: '<S3>/Relay'

  -1.0,

  // Expression: 0
  //  Referenced by: '<S3>/Switch2'

  0.0,

  // Computed Parameter: Integrator_gainval
  //  Referenced by: '<S38>/Integrator'

  0.005,

  // Expression: 0.5
  //  Referenced by: '<S3>/Switch'

  0.5,

  // Expression: 0.5
  //  Referenced by: '<S3>/Switch1'

  0.5,

  // Expression: 0
  //  Referenced by: '<S56>/ffw'

  0.0,

  // Expression: 0
  //  Referenced by: '<S4>/IV_Vdot_ref'

  0.0,

  // Expression: 0
  //  Referenced by: '<S4>/IV_dc'

  0.0,

  // Expression: 0
  //  Referenced by: '<S4>/Unit Delay'

  0.0,

  // Computed Parameter: Integrator_gainval_m
  //  Referenced by: '<S89>/Integrator'

  0.005,

  // Expression: 100
  //  Referenced by: '<S4>/Saturation'

  100.0,

  // Expression: 0
  //  Referenced by: '<S4>/Saturation'

  0.0,

  // Expression: 0
  //  Referenced by: '<S4>/Data Store Memory'

  0.0,

  // Expression: 0
  //  Referenced by: '<S1>/IV_dc'

  0.0,

  // Expression: 0
  //  Referenced by: '<S1>/IV_Vdot_ref'

  0.0,

  // Expression: 0
  //  Referenced by: '<S1>/IV_P_ref'

  0.0,

  // Expression: 0
  //  Referenced by: '<S1>/OV_cmd'

  0.0,

  // Expression: [0 0 0]
  //  Referenced by: '<S1>/signals'

  { 0.0, 0.0, 0.0 },

  // Expression: 1
  //  Referenced by: '<S1>/Constant'

  1.0,

  // Expression: 1
  //  Referenced by: '<S1>/Pulse Generator'

  1.0,

  // Expression: 0
  //  Referenced by: '<S1>/Pulse Generator'

  0.0,

  // Expression: 0
  //  Referenced by: '<S1>/Unit Delay'

  0.0,

  // Expression: 1/16384
  //  Referenced by: '<S5>/Gain'

  6.103515625E-5,

  // Expression: -0.1
  //  Referenced by: '<S5>/Bias'

  -0.1,

  // Expression: 1/0.8
  //  Referenced by: '<S5>/Gain1'

  1.25,

  // Expression: 0.1 * 16384
  //  Referenced by: '<S1>/Unit Delay1'

  1638.4,

  // Expression: 1/16384
  //  Referenced by: '<S2>/Gain'

  6.103515625E-5,

  // Expression: -0.1
  //  Referenced by: '<S2>/Bias'

  -0.1,

  // Expression: 1/0.8
  //  Referenced by: '<S2>/Gain1'

  1.25,

  // Expression: 1.01972
  //  Referenced by: '<S1>/cmH20'

  1.01972,

  // Expression: VC_mode
  //  Referenced by: '<S1>/Constant1'

  0,

  // Expression: VC_mode
  //  Referenced by: '<S1>/Constant3'

  0,

  // Expression: PC_mode
  //  Referenced by: '<S1>/Constant2'

  1
};

//
// File trailer for generated code.
//
// [EOF]
//
