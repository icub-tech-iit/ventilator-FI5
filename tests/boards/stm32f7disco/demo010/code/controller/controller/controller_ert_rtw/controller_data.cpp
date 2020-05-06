//
// Non-Degree Granting Education License -- for use at non-degree
// granting, nonprofit, educational organizations only. Not for
// government, commercial, or other organizational use.
//
// File: controller_data.cpp
//
// Code generated for Simulink model 'controller'.
//
// Model version                  : 1.456
// Simulink Coder version         : 9.3 (R2020a) 18-Nov-2019
// C/C++ source code generated on : Wed May  6 09:35:15 2020
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
  //    '<S18>/T_monostable'

  0.5,

  // Mask Parameter: Monostable1_Ts
  //  Referenced by:
  //    '<S18>/Constant2'
  //    '<S18>/Constant3'

  0.01,

  // Mask Parameter: Controller_respiratory_rate
  //  Referenced by:
  //    '<S1>/Pulse Generator'
  //    '<S18>/T_monostable'

  10.0,

  // Mask Parameter: PIDController_InitialConditionF
  //  Referenced by: '<S55>/Integrator'

  0.0F,

  // Mask Parameter: PIDController_InitialConditio_c
  //  Referenced by: '<S107>/Integrator'

  0.0F,

  // Mask Parameter: PIDController_Kt
  //  Referenced by: '<S66>/Kt'

  1.0F,

  // Mask Parameter: PIDController_Kt_j
  //  Referenced by: '<S118>/Kt'

  1.0F,

  // Mask Parameter: Controller_PC_FFW
  //  Referenced by:
  //    '<S23>/FFW'
  //    '<S23>/PC_FFW'

  30.0F,

  // Mask Parameter: Controller_PC_FFW_rec
  //  Referenced by: '<S23>/PC_FFW_rec'

  30.0F,

  // Mask Parameter: Controller_PC_I
  //  Referenced by:
  //    '<S23>/I'
  //    '<S23>/PC_I'

  40.0F,

  // Mask Parameter: Controller_PC_I_rec
  //  Referenced by: '<S23>/PC_I_rec'

  10.0F,

  // Mask Parameter: Controller_PC_P
  //  Referenced by:
  //    '<S23>/P'
  //    '<S23>/PC_P'

  1.0F,

  // Mask Parameter: Controller_PC_P_rec
  //  Referenced by: '<S23>/PC_P_rec'

  0.05F,

  // Mask Parameter: Controller_PIP
  //  Referenced by: '<S11>/Gain'

  25.0F,

  // Mask Parameter: Controller_trigger_thres
  //  Referenced by: '<S17>/Constant'

  3.0F,

  // Mask Parameter: Controller_enable_trigger
  //  Referenced by: '<S1>/Constant4'

  1,

  // Mask Parameter: Monostable1_ic
  //  Referenced by: '<S19>/Unit Delay'

  0,

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

  4U,

  // Expression: [1 1]
  //  Referenced by: '<S19>/either edge'

  { 1.0, 1.0 },

  // Expression: [0 1]
  //  Referenced by: '<S19>/neg. edge'

  { 0.0, 1.0 },

  // Expression: [1 0]
  //  Referenced by: '<S19>/pos. edge'

  { 1.0, 0.0 },

  // Expression: model
  //  Referenced by: '<S19>/Constant1'

  1.0,

  // Expression: 0
  //  Referenced by: '<S18>/Unit Delay'

  0.0,

  // Expression: 1
  //  Referenced by: '<S1>/Pulse Generator'

  1.0,

  // Expression: 0
  //  Referenced by: '<S1>/Pulse Generator'

  0.0,

  // Computed Parameter: Constant8_Value
  //  Referenced by: '<S1>/Constant8'

  1.0F,

  // Computed Parameter: Out_Y0
  //  Referenced by: '<S8>/Out'

  0.0F,

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

  // Computed Parameter: Gain1_Gain
  //  Referenced by: '<S11>/Gain1'

  0.75F,

  // Computed Parameter: Gain2_Gain
  //  Referenced by: '<S11>/Gain2'

  -669.199463F,

  // Computed Parameter: InputShaping_NumCoef
  //  Referenced by: '<S11>/Input Shaping'

  { -0.000185435347F, -0.00055630604F, -0.00055630604F, -0.000185435347F },

  // Computed Parameter: InputShaping_DenCoef
  //  Referenced by: '<S11>/Input Shaping'

  { -8.70399475F, 24.6642303F, -23.2967472F, 7.33502722F },

  // Computed Parameter: UnitDelay_InitialCondition_m
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

  // Computed Parameter: VC_P_Value
  //  Referenced by: '<S73>/VC_P'

  0.2F,

  // Computed Parameter: VC_I_Value
  //  Referenced by: '<S73>/VC_I'

  10.0F,

  // Computed Parameter: VC_P_rec_Value
  //  Referenced by: '<S73>/VC_P_rec'

  0.1F,

  // Computed Parameter: VC_I_rec_Value
  //  Referenced by: '<S73>/VC_I_rec'

  5.0F,

  // Computed Parameter: ffw_Y0
  //  Referenced by: '<S74>/ffw'

  0.0F,

  // Computed Parameter: Q_ref_Y0
  //  Referenced by: '<S16>/Q_ref'

  0.0F,

  // Computed Parameter: IV_dc_Y0_m
  //  Referenced by: '<S16>/IV_dc'

  0.0F,

  // Computed Parameter: UnitDelay_InitialCondition_n
  //  Referenced by: '<S16>/Unit Delay'

  0.0F,

  // Computed Parameter: VolumetricFlowrate_Gain
  //  Referenced by: '<S16>/Volumetric Flow rate'

  30.0F,

  // Computed Parameter: Switch2_Threshold_b
  //  Referenced by: '<S73>/Switch2'

  980.661377F,

  // Computed Parameter: Integrator_gainval_m
  //  Referenced by: '<S107>/Integrator'

  0.005F,

  // Computed Parameter: Saturation_UpperSat_b
  //  Referenced by: '<S16>/Saturation'

  100.0F,

  // Computed Parameter: Saturation_LowerSat_f
  //  Referenced by: '<S16>/Saturation'

  0.0F,

  // Computed Parameter: DataStoreMemory_InitialValue
  //  Referenced by: '<S16>/Data Store Memory'

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

  // Computed Parameter: UnitDelay1_InitialCondition
  //  Referenced by: '<S1>/Unit Delay1'

  { 8192.0F, 8192.0F, 8192.0F, 0.0F },

  // Computed Parameter: Gain_Gain
  //  Referenced by: '<S15>/Gain'

  6.10351563E-5F,

  // Computed Parameter: Bias_Bias
  //  Referenced by: '<S15>/Bias'

  -0.1F,

  // Computed Parameter: Gain1_Gain_o
  //  Referenced by: '<S15>/Gain1'

  2.5F,

  // Computed Parameter: mabr_to_cmH2O_Gain
  //  Referenced by: '<S1>/mabr_to_cmH2O'

  1.01972F,

  // Computed Parameter: Gain_Gain_i
  //  Referenced by: '<S14>/Gain'

  6.10351563E-5F,

  // Computed Parameter: Bias_Bias_h
  //  Referenced by: '<S14>/Bias'

  -0.1F,

  // Computed Parameter: Gain1_Gain_d
  //  Referenced by: '<S14>/Gain1'

  1.25F,

  // Computed Parameter: psi_cmH2O_Gain
  //  Referenced by: '<S1>/psi_cmH2O'

  70.307F,

  // Computed Parameter: UnitDelay2_InitialCondition
  //  Referenced by: '<S1>/Unit Delay2'

  0.0F,

  // Computed Parameter: Gain_Gain_m
  //  Referenced by: '<S12>/Gain'

  0.781631649F,

  // Computed Parameter: Bias_Bias_b
  //  Referenced by: '<S12>/Bias'

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
  //  Referenced by: '<S13>/Gain'

  6.10351563E-5F,

  // Computed Parameter: Bias_Bias_l
  //  Referenced by: '<S13>/Bias'

  -0.1F,

  // Computed Parameter: Gain1_Gain_e
  //  Referenced by: '<S13>/Gain1'

  1.25F,

  // Computed Parameter: UnitDelay3_InitialCondition
  //  Referenced by: '<S1>/Unit Delay3'

  0.0F,

  // Computed Parameter: Gain_Gain_e
  //  Referenced by: '<S1>/Gain'

  -1.0F,

  // Computed Parameter: DiscreteTimeIntegrator_gainval
  //  Referenced by: '<S1>/Discrete-Time Integrator'

  8.33333324E-5F,

  // Computed Parameter: DiscreteTimeIntegrator_IC
  //  Referenced by: '<S1>/Discrete-Time Integrator'

  0.0F,

  // Computed Parameter: Constant_Value_a
  //  Referenced by: '<S1>/Constant'

  1.0F,

  // Computed Parameter: TidalVolume_Gain
  //  Referenced by: '<S1>/Tidal Volume'

  0.65F,

  // Computed Parameter: OUT_Y0
  //  Referenced by: '<S20>/OUT'

  0,

  // Computed Parameter: OUT_Y0_h
  //  Referenced by: '<S21>/OUT'

  0,

  // Computed Parameter: OV_cmd_Y0
  //  Referenced by: '<S1>/OV_cmd'

  0,

  // Computed Parameter: delay_InitialCondition
  //  Referenced by: '<S18>/delay'

  0
};

//
// File trailer for generated code.
//
// [EOF]
//
