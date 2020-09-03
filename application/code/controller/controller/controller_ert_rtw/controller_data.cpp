//
// Non-Degree Granting Education License -- for use at non-degree
// granting, nonprofit, educational organizations only. Not for
// government, commercial, or other organizational use.
//
// File: controller_data.cpp
//
// Code generated for Simulink model 'controller'.
//
// Model version                  : 1.541
// Simulink Coder version         : 9.3 (R2020a) 18-Nov-2019
// C/C++ source code generated on : Sat Aug 22 22:20:58 2020
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
  // Mask Parameter: PIDController_InitialConditionF
  //  Referenced by: '<S55>/Integrator'

  0.0F,

  // Mask Parameter: PIDController_InitialConditio_c
  //  Referenced by: '<S108>/Integrator'

  0.0F,

  // Mask Parameter: PIDController_Kt
  //  Referenced by: '<S66>/Kt'

  1.0F,

  // Mask Parameter: PIDController_Kt_j
  //  Referenced by: '<S119>/Kt'

  1.0F,

  // Mask Parameter: pressure_thres_const
  //  Referenced by: '<S20>/Constant'

  3.0F,

  // Mask Parameter: CompareToConstant1_const
  //  Referenced by: '<S4>/Constant'

  2U,

  // Mask Parameter: CompareToConstant2_const
  //  Referenced by: '<S5>/Constant'

  3U,

  // Mask Parameter: CompareToConstant3_const
  //  Referenced by: '<S6>/Constant'

  4U,

  // Mask Parameter: CompareToConstant_const
  //  Referenced by: '<S3>/Constant'

  1U,

  // Mask Parameter: Controller_control_mode
  //  Referenced by: '<S1>/Constant13'

  1U,

  // Computed Parameter: Constant8_Value
  //  Referenced by: '<S1>/Constant8'

  1.0F,

  // Computed Parameter: PC_P_Value
  //  Referenced by: '<S23>/PC_P'

  1.0F,

  // Computed Parameter: PC_I_Value
  //  Referenced by: '<S23>/PC_I'

  40.0F,

  // Computed Parameter: PC_FFW_Value
  //  Referenced by: '<S23>/PC_FFW'

  30.0F,

  // Computed Parameter: PC_P_rec_Value
  //  Referenced by: '<S23>/PC_P_rec'

  0.05F,

  // Computed Parameter: PC_I_rec_Value
  //  Referenced by: '<S23>/PC_I_rec'

  10.0F,

  // Computed Parameter: PC_FFW_rec_Value
  //  Referenced by: '<S23>/PC_FFW_rec'

  30.0F,

  // Computed Parameter: P_Y0
  //  Referenced by: '<S23>/P'

  1.0F,

  // Computed Parameter: I_Y0
  //  Referenced by: '<S23>/I'

  40.0F,

  // Computed Parameter: FFW_Y0
  //  Referenced by: '<S23>/FFW'

  30.0F,

  // Computed Parameter: Switch2_Threshold
  //  Referenced by: '<S23>/Switch2'

  980.661377F,

  // Computed Parameter: P_ref_Y0
  //  Referenced by: '<S11>/P_ref'

  0.0F,

  // Computed Parameter: IV_dc_Y0
  //  Referenced by: '<S11>/IV_dc'

  0.0F,

  // Computed Parameter: Constant_Value
  //  Referenced by: '<S11>/Constant'

  0.0F,

  // Computed Parameter: PIP_Gain
  //  Referenced by: '<S11>/PIP'

  25.0F,

  // Computed Parameter: PIP_step_perc_Gain
  //  Referenced by: '<S11>/PIP_step_perc'

  75.0F,

  // Computed Parameter: Gain3_Gain
  //  Referenced by: '<S11>/Gain3'

  0.01F,

  // Computed Parameter: Constant_Value_e
  //  Referenced by: '<S21>/Constant'

  { -150.765793F, -84.9812775F, -15.96696F },

  // Computed Parameter: Constant1_Value
  //  Referenced by: '<S21>/Constant 1'

  60.0F,

  // Computed Parameter: Constant1_Value_n
  //  Referenced by: '<S21>/Constant1'

  { 1.0E-6F, 0.0002F, 0.04F },

  // Computed Parameter: Constant2_Value
  //  Referenced by: '<S21>/Constant2'

  1.0F,

  // Computed Parameter: PIP_rise_perc_Value
  //  Referenced by: '<S21>/PIP_rise_perc'

  70.0F,

  // Computed Parameter: Gain_Gain
  //  Referenced by: '<S21>/Gain'

  0.00666666683F,

  // Computed Parameter: UnitDelay_InitialCondition
  //  Referenced by: '<S11>/Unit Delay'

  0.0F,

  // Computed Parameter: Integrator_gainval
  //  Referenced by: '<S55>/Integrator'

  0.005F,

  // Computed Parameter: Saturation_UpperSat
  //  Referenced by: '<S11>/Saturation'

  100.0F,

  // Computed Parameter: Saturation_LowerSat
  //  Referenced by: '<S11>/Saturation'

  0.0F,

  // Computed Parameter: Switch_Threshold
  //  Referenced by: '<S11>/Switch'

  0.5F,

  // Computed Parameter: Switch1_Threshold
  //  Referenced by: '<S11>/Switch1'

  0.5F,

  // Computed Parameter: VolumetricFlowrate_Gain
  //  Referenced by: '<S19>/Volumetric Flow rate'

  30.0F,

  // Computed Parameter: VC_P_Value
  //  Referenced by: '<S74>/VC_P'

  0.2F,

  // Computed Parameter: VC_I_Value
  //  Referenced by: '<S74>/VC_I'

  10.0F,

  // Computed Parameter: VC_P_rec_Value
  //  Referenced by: '<S74>/VC_P_rec'

  0.1F,

  // Computed Parameter: VC_I_rec_Value
  //  Referenced by: '<S74>/VC_I_rec'

  5.0F,

  // Computed Parameter: ffw_Y0
  //  Referenced by: '<S75>/ffw'

  0.0F,

  // Computed Parameter: Q_ref_Y0
  //  Referenced by: '<S19>/Q_ref'

  0.0F,

  // Computed Parameter: IV_dc_Y0_m
  //  Referenced by: '<S19>/IV_dc'

  0.0F,

  // Computed Parameter: Constant_Value_b
  //  Referenced by: '<S72>/Constant'

  0.0F,

  // Computed Parameter: UnitDelay_InitialCondition_n
  //  Referenced by: '<S19>/Unit Delay'

  0.0F,

  // Computed Parameter: Switch2_Threshold_b
  //  Referenced by: '<S74>/Switch2'

  980.661377F,

  // Computed Parameter: Integrator_gainval_m
  //  Referenced by: '<S108>/Integrator'

  0.005F,

  // Computed Parameter: Saturation_UpperSat_b
  //  Referenced by: '<S19>/Saturation'

  100.0F,

  // Computed Parameter: Saturation_LowerSat_f
  //  Referenced by: '<S19>/Saturation'

  0.0F,

  // Computed Parameter: DataStoreMemory_InitialValue
  //  Referenced by: '<S19>/Data Store Memory'

  0.0F,

  // Computed Parameter: IV_dc_Y0_o
  //  Referenced by: '<S1>/IV_dc'

  0.0F,

  // Computed Parameter: Q_ref_Y0_n
  //  Referenced by: '<S1>/Q_ref'

  0.0F,

  // Computed Parameter: P_ref_Y0_f
  //  Referenced by: '<S1>/P_ref'

  0.0F,

  // Computed Parameter: signals_Y0
  //  Referenced by: '<S1>/signals'

  0.0F,

  // Computed Parameter: Constant_Value_p
  //  Referenced by: '<S7>/Constant'

  0.0F,

  // Computed Parameter: Constant_Value_d
  //  Referenced by: '<S8>/Constant'

  0.0F,

  // Computed Parameter: UnitDelay1_InitialCondition
  //  Referenced by: '<S1>/Unit Delay1'

  { 8192.0F, 8192.0F, 8192.0F, 0.0F },

  // Computed Parameter: Gain_Gain_h
  //  Referenced by: '<S16>/Gain'

  6.10351562E-5F,

  // Computed Parameter: Bias_Bias
  //  Referenced by: '<S16>/Bias'

  -0.1F,

  // Computed Parameter: Gain1_Gain
  //  Referenced by: '<S16>/Gain1'

  2.5F,

  // Computed Parameter: mabr_to_cmH2O_Gain
  //  Referenced by: '<S1>/mabr_to_cmH2O'

  1.01972F,

  // Computed Parameter: Gain_Gain_i
  //  Referenced by: '<S15>/Gain'

  6.10351562E-5F,

  // Computed Parameter: Bias_Bias_h
  //  Referenced by: '<S15>/Bias'

  -0.1F,

  // Computed Parameter: Gain1_Gain_d
  //  Referenced by: '<S15>/Gain1'

  1.25F,

  // Computed Parameter: psi_cmH2O_Gain
  //  Referenced by: '<S1>/psi_cmH2O'

  70.307F,

  // Computed Parameter: UnitDelay2_InitialCondition
  //  Referenced by: '<S1>/Unit Delay2'

  0.0F,

  // Computed Parameter: Gain_Gain_m
  //  Referenced by: '<S13>/Gain'

  0.0977039561F,

  // Computed Parameter: Bias_Bias_b
  //  Referenced by: '<S13>/Bias'

  -50.0F,

  // Computed Parameter: cmH2OtoPa_Gain
  //  Referenced by: '<S2>/cmH2O to Pa'

  98.0661392F,

  // Computed Parameter: Bias_Bias_j
  //  Referenced by: '<S2>/Bias'

  101325.352F,

  // Computed Parameter: Constant1_Value_k
  //  Referenced by: '<S2>/Constant1'

  101325.352F,

  // Computed Parameter: Constant_Value_h
  //  Referenced by: '<S2>/Constant'

  293.15F,

  // Computed Parameter: K_Bias
  //  Referenced by: '<S2>/K'

  273.15F,

  // Computed Parameter: UnitDelay_InitialCondition_g
  //  Referenced by: '<S1>/Unit Delay'

  0.0F,

  // Computed Parameter: Gain_Gain_f
  //  Referenced by: '<S14>/Gain'

  6.10351562E-5F,

  // Computed Parameter: Bias_Bias_l
  //  Referenced by: '<S14>/Bias'

  -0.1F,

  // Computed Parameter: Gain1_Gain_e
  //  Referenced by: '<S14>/Gain1'

  1.25F,

  // Computed Parameter: UnitDelay3_InitialCondition
  //  Referenced by: '<S1>/Unit Delay3'

  0.0F,

  // Computed Parameter: Gain_Gain_e
  //  Referenced by: '<S1>/Gain'

  -1.0F,

  // Computed Parameter: respiratory_rate_Value
  //  Referenced by: '<S1>/respiratory_rate'

  10.0F,

  // Computed Parameter: IE_ratio_Value
  //  Referenced by: '<S1>/IE_ratio'

  0.5F,

  // Computed Parameter: DiscreteTimeIntegrator_gainval
  //  Referenced by: '<S1>/Discrete-Time Integrator'

  8.33333324E-5F,

  // Computed Parameter: DiscreteTimeIntegrator_IC
  //  Referenced by: '<S1>/Discrete-Time Integrator'

  0.0F,

  // Computed Parameter: UnitDelay4_InitialCondition
  //  Referenced by: '<S1>/Unit Delay4'

  0.0F,

  // Computed Parameter: tidal_volume_Value
  //  Referenced by: '<S1>/tidal_volume'

  0.65F,

  // Computed Parameter: TV_correction_gain_Value
  //  Referenced by: '<S1>/TV_correction_gain'

  0.25F,

  // Computed Parameter: flowby_target_Value
  //  Referenced by: '<S1>/flowby_target'

  3.0F,

  // Computed Parameter: Constant_Value_a
  //  Referenced by: '<S1>/Constant'

  1.0F,

  // Computed Parameter: Out_Y0
  //  Referenced by: '<S9>/Out'

  0,

  // Computed Parameter: OV_cmd_Y0
  //  Referenced by: '<S1>/OV_cmd'

  0,

  // Computed Parameter: enable_trigger_Value
  //  Referenced by: '<S1>/enable_trigger'

  0
};

//
// File trailer for generated code.
//
// [EOF]
//
