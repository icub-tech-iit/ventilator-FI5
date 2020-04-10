//
// Non-Degree Granting Education License -- for use at non-degree
// granting, nonprofit, educational organizations only. Not for
// government, commercial, or other organizational use.
//
// File: controller_data.cpp
//
// Code generated for Simulink model 'controller'.
//
// Model version                  : 1.147
// Simulink Coder version         : 9.3 (R2020a) 18-Nov-2019
// C/C++ source code generated on : Fri Apr 10 16:53:36 2020
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
  //  Referenced by: '<S1>/Pulse Generator'

  0.33333333333333331,

  // Mask Parameter: Controller_respiratory_rate
  //  Referenced by: '<S1>/Pulse Generator'

  8.0,

  // Mask Parameter: PIDController_InitialConditionF
  //  Referenced by: '<S38>/Integrator'

  0.0F,

  // Mask Parameter: PIDController_InitialConditio_c
  //  Referenced by: '<S89>/Integrator'

  0.0F,

  // Mask Parameter: Controller_PC_FFW
  //  Referenced by: '<S3>/FFW'

  35.0F,

  // Mask Parameter: Controller_PC_I_dw
  //  Referenced by: '<S3>/Constant5'

  100.0F,

  // Mask Parameter: Controller_PC_I_up
  //  Referenced by: '<S3>/Constant4'

  20.0F,

  // Mask Parameter: Controller_PC_P
  //  Referenced by: '<S3>/Constant2'

  0.5F,

  // Mask Parameter: Controller_PEEP
  //  Referenced by: '<S3>/Constant3'

  5.0F,

  // Mask Parameter: Controller_PIP
  //  Referenced by: '<S3>/Gain'

  25.0F,

  // Mask Parameter: Controller_VC_I
  //  Referenced by: '<S86>/Integral Gain'

  10.0F,

  // Mask Parameter: Controller_VC_P
  //  Referenced by: '<S94>/Proportional Gain'

  0.0F,

  // Expression: 1
  //  Referenced by: '<S1>/Pulse Generator'

  1.0,

  // Expression: 0
  //  Referenced by: '<S1>/Pulse Generator'

  0.0,

  // Computed Parameter: Constant_Value
  //  Referenced by: '<S3>/Constant'

  0.0F,

  // Computed Parameter: Saturation_UpperSat
  //  Referenced by: '<S3>/Saturation'

  100.0F,

  // Computed Parameter: Saturation_LowerSat
  //  Referenced by: '<S3>/Saturation'

  0.0F,

  // Computed Parameter: IV_P_ref_Y0
  //  Referenced by: '<S3>/IV_P_ref'

  0.0F,

  // Computed Parameter: IV_dc_Y0
  //  Referenced by: '<S3>/IV_dc'

  0.0F,

  // Computed Parameter: Constant1_Value
  //  Referenced by: '<S3>/Constant1'

  -3391.12183F,

  // Computed Parameter: InputShaping_NumCoef
  //  Referenced by: '<S3>/Input Shaping'

  { -0.000182973847F, -0.000548921525F, -0.000548921525F, -0.000182973847F },

  // Computed Parameter: InputShaping_DenCoef
  //  Referenced by: '<S3>/Input Shaping'

  { -8.70077801F, 24.6613693F, -23.2999535F, 7.33789825F },

  // Computed Parameter: Relay_OnVal
  //  Referenced by: '<S3>/Relay'

  2.0F,

  // Computed Parameter: Relay_OffVal
  //  Referenced by: '<S3>/Relay'

  -2.0F,

  // Computed Parameter: Relay_YOn
  //  Referenced by: '<S3>/Relay'

  1.0F,

  // Computed Parameter: Relay_YOff
  //  Referenced by: '<S3>/Relay'

  -1.0F,

  // Computed Parameter: Switch2_Threshold
  //  Referenced by: '<S3>/Switch2'

  0.0F,

  // Computed Parameter: Integrator_gainval
  //  Referenced by: '<S38>/Integrator'

  0.005F,

  // Computed Parameter: Switch_Threshold
  //  Referenced by: '<S3>/Switch'

  0.5F,

  // Computed Parameter: Switch1_Threshold
  //  Referenced by: '<S3>/Switch1'

  0.5F,

  // Computed Parameter: ffw_Y0
  //  Referenced by: '<S56>/ffw'

  0.0F,

  // Computed Parameter: IV_Vdot_ref_Y0
  //  Referenced by: '<S4>/IV_Vdot_ref'

  0.0F,

  // Computed Parameter: IV_dc_Y0_m
  //  Referenced by: '<S4>/IV_dc'

  0.0F,

  // Computed Parameter: UnitDelay_InitialCondition
  //  Referenced by: '<S4>/Unit Delay'

  0.0F,

  // Computed Parameter: VolumetricFlowrate_Gain
  //  Referenced by: '<S4>/Volumetric Flow rate'

  20.8F,

  // Computed Parameter: Integrator_gainval_m
  //  Referenced by: '<S89>/Integrator'

  0.005F,

  // Computed Parameter: Saturation_UpperSat_b
  //  Referenced by: '<S4>/Saturation'

  100.0F,

  // Computed Parameter: Saturation_LowerSat_f
  //  Referenced by: '<S4>/Saturation'

  0.0F,

  // Computed Parameter: DataStoreMemory_InitialValue
  //  Referenced by: '<S4>/Data Store Memory'

  0.0F,

  // Computed Parameter: IV_dc_Y0_o
  //  Referenced by: '<S1>/IV_dc'

  0.0F,

  // Computed Parameter: IV_Vdot_ref_Y0_n
  //  Referenced by: '<S1>/IV_Vdot_ref'

  0.0F,

  // Computed Parameter: IV_P_ref_Y0_f
  //  Referenced by: '<S1>/IV_P_ref'

  0.0F,

  // Computed Parameter: OV_cmd_Y0
  //  Referenced by: '<S1>/OV_cmd'

  0.0F,

  // Computed Parameter: signals_Y0
  //  Referenced by: '<S1>/signals'

  { 0.0F, 0.0F, 0.0F },

  // Computed Parameter: Constant_Value_a
  //  Referenced by: '<S1>/Constant'

  1.0F,

  // Computed Parameter: UnitDelay_InitialCondition_g
  //  Referenced by: '<S1>/Unit Delay'

  0.0F,

  // Computed Parameter: Gain_Gain
  //  Referenced by: '<S5>/Gain'

  6.10351563E-5F,

  // Computed Parameter: Bias_Bias
  //  Referenced by: '<S5>/Bias'

  -0.1F,

  // Computed Parameter: Gain1_Gain
  //  Referenced by: '<S5>/Gain1'

  1.25F,

  // Computed Parameter: UnitDelay1_InitialCondition
  //  Referenced by: '<S1>/Unit Delay1'

  1638.4F,

  // Computed Parameter: Gain_Gain_p
  //  Referenced by: '<S2>/Gain'

  6.10351563E-5F,

  // Computed Parameter: Bias_Bias_e
  //  Referenced by: '<S2>/Bias'

  -0.1F,

  // Computed Parameter: Gain1_Gain_c
  //  Referenced by: '<S2>/Gain1'

  1.25F,

  // Computed Parameter: cmH20_Gain
  //  Referenced by: '<S1>/cmH20'

  1.01972F,

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
