//
// Non-Degree Granting Education License -- for use at non-degree
// granting, nonprofit, educational organizations only. Not for
// government, commercial, or other organizational use.
//
// File: FSM0_data.cpp
//
// Code generated for Simulink model 'FSM0'.
//
// Model version                  : 1.910
// Simulink Coder version         : 9.3 (R2020a) 18-Nov-2019
// C/C++ source code generated on : Wed Jul  8 15:26:02 2020
//
// Target selection: ert.tlc
// Embedded hardware selection: Intel->x86-64 (Windows64)
// Code generation objectives: Unspecified
// Validation result: Not run
//
#include "FSM0.h"
#include "FSM0_private.h"

// Block parameters (default storage)
FSM0ModelClass::P_FSM0_T FSM0ModelClass::FSM0_P = {
  // Computed Parameter: FSM_Ts
  //  Referenced by: '<S1>/FSM'

  0.01F,

  // Computed Parameter: FSM_blinking_period
  //  Referenced by: '<S1>/FSM'

  1.0F,

  // Computed Parameter: FSM_debounce_tmo
  //  Referenced by: '<S1>/FSM'

  0.5F,

  // Computed Parameter: FSM_test_tmo
  //  Referenced by: '<S1>/FSM'

  5.0F,

  // Computed Parameter: RateTransition_InitialCondition
  //  Referenced by: '<S1>/Rate Transition'

  0.0F,

  // Computed Parameter: BPM1min_Value
  //  Referenced by: '<S11>/BPM [1//min]'

  { 5.0F, 40.0F, 200.0F },

  // Computed Parameter: PIPcmH2O_Value
  //  Referenced by: '<S11>/PIP [cmH2O]'

  { 5.0F, 60.0F, 200.0F },

  // Computed Parameter: IE_ratio_Value
  //  Referenced by: '<S11>/IE_ratio [-]'

  { 0.25F, 1.0F, 200.0F },

  // Computed Parameter: TDml_Value
  //  Referenced by: '<S11>/TD [ml]'

  { 200.0F, 2000.0F, 200.0F },

  // Computed Parameter: Proportional_Value
  //  Referenced by: '<S13>/Proportional'

  { 0.0F, 10.0F, 200.0F },

  // Computed Parameter: Integral_Value
  //  Referenced by: '<S13>/Integral'

  { 0.0F, 50.0F, 200.0F },

  // Computed Parameter: ProportionalRecovery_Value
  //  Referenced by: '<S13>/Proportional Recovery'

  { 0.0F, 10.0F, 200.0F },

  // Computed Parameter: IntegralRecovery_Value
  //  Referenced by: '<S13>/Integral Recovery'

  { 0.0F, 50.0F, 200.0F },

  // Computed Parameter: Proportional_Value_j
  //  Referenced by: '<S9>/Proportional'

  { 0.0F, 10.0F, 200.0F },

  // Computed Parameter: Integral_Value_k
  //  Referenced by: '<S9>/Integral'

  { 0.0F, 100.0F, 200.0F },

  // Computed Parameter: ProportionalRecovery_Value_g
  //  Referenced by: '<S9>/Proportional Recovery'

  { 0.0F, 10.0F, 200.0F },

  // Computed Parameter: IntegralRecovery_Value_b
  //  Referenced by: '<S9>/Integral Recovery'

  { 0.0F, 100.0F, 200.0F },

  // Computed Parameter: FeedForward_Value
  //  Referenced by: '<S10>/Feed-Forward'

  { 0.0F, 100.0F, 200.0F },

  // Computed Parameter: FeedForwardRecovery_Value
  //  Referenced by: '<S10>/Feed-Forward Recovery'

  { 0.0F, 100.0F, 200.0F },

  // Computed Parameter: PIPStepPerc_Value
  //  Referenced by: '<S10>/PIP Step Perc [%]'

  { 0.0F, 100.0F, 200.0F },

  // Computed Parameter: PIPRisePerc_Value
  //  Referenced by: '<S10>/PIP Rise Perc [%]'

  { 0.0F, 100.0F, 200.0F },

  // Computed Parameter: EnableTrigger_Value
  //  Referenced by: '<S8>/Enable Trigger'

  { 0.0F, 1.0F, 200.0F },

  // Computed Parameter: PressureThresholdcmH2O_Value
  //  Referenced by: '<S8>/Pressure Threshold [cmH2O]'

  { 0.0F, 20.0F, 200.0F },

  // Computed Parameter: FlowbyTarget_Value
  //  Referenced by: '<S8>/Flow-by Target'

  { 0.0F, 10.0F, 200.0F },

  // Computed Parameter: NotUsed1_Value
  //  Referenced by: '<S8>/Not Used 1'

  { 0.0F, 0.0F, 200.0F },

  // Computed Parameter: Test1Peak_Value
  //  Referenced by: '<S12>/Test1 Peak [%]'

  { 0.0F, 100.0F, 200.0F },

  // Computed Parameter: Test1Periods_Value
  //  Referenced by: '<S12>/Test1 Period [s]'

  { 0.0F, 10.0F, 200.0F },

  // Computed Parameter: Test2Periods_Value
  //  Referenced by: '<S12>/Test2 Period [s]'

  { 0.0F, 10.0F, 200.0F },

  // Computed Parameter: NotUsed1_Value_h
  //  Referenced by: '<S12>/Not Used 1'

  { 0.0F, 0.0F, 200.0F },

  // Computed Parameter: S7MincmH2O_Value
  //  Referenced by: '<S6>/S7 Min [cmH2O]'

  { 0.0F, 1000.0F, 200.0F },

  // Computed Parameter: S7MaxcmH2O_Value
  //  Referenced by: '<S6>/S7 Max [cmH2O]'

  { 500.0F, 5000.0F, 200.0F },

  // Computed Parameter: S1S3MaxcmH2O_Value
  //  Referenced by: '<S6>/S1,S3 Max [cmH2O]'

  { 50.0F, 1000.0F, 200.0F },

  // Computed Parameter: S1S3ReinstatecmH2O_Value
  //  Referenced by: '<S6>/S1,S3 Reinstate [cmH2O]'

  { 0.0F, 100.0F, 200.0F },

  // Computed Parameter: PEEPcmH2O_Value
  //  Referenced by: '<S7>/PEEP [cmH2O]'

  { -50.0F, 100.0F, 200.0F },

  // Computed Parameter: TDMinL_Value
  //  Referenced by: '<S7>/TD Min [L]'

  { -0.1F, 1.0F, 200.0F },

  // Computed Parameter: NotUsed1_Value_l
  //  Referenced by: '<S7>/Not Used 1'

  { 0.0F, 0.0F, 200.0F },

  // Computed Parameter: NotUsed2_Value
  //  Referenced by: '<S7>/Not Used 2'

  { 0.0F, 0.0F, 200.0F }
};

//
// File trailer for generated code.
//
// [EOF]
//
