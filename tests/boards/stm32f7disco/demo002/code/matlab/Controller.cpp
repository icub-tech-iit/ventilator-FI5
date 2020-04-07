//
// Non-Degree Granting Education License -- for use at non-degree
// granting, nonprofit, educational organizations only. Not for
// government, commercial, or other organizational use.
//
// File: Controller.cpp
//
// Code generated for Simulink model 'Controller'.
//
// Model version                  : 1.238
// Simulink Coder version         : 9.3 (R2020a) 18-Nov-2019
// C/C++ source code generated on : Sun Mar 29 15:51:14 2020
//
// Target selection: ert.tlc
// Embedded hardware selection: 32-bit Generic
// Code generation objectives: Unspecified
// Validation result: Not run
//
#include "Controller.h"
#include "Controller_private.h"

// Named constants for Chart: '<S1>/Compensator Handling'
const uint8_T Controller_IN_AutomaticOff = 1U;
const uint8_T Controller_IN_AutomaticOn = 2U;
const uint8_T Controller_IN_Select = 3U;

// System initialize for atomic system: '<S1>/Compensator'
void ControllerModelClass::Compensator_Init(DW_Compensator_T *localDW,
  P_Compensator_T *localP)
{
  // InitializeConditions for DiscreteIntegrator: '<S41>/Integrator'
  localDW->Integrator_DSTATE = localP->DiscretePIDController_InitialCo;
  localDW->Integrator_PrevResetState = 2;
}

// Output and update for atomic system: '<S1>/Compensator'
void ControllerModelClass::Compensator(real_T rtu_e, boolean_T rtu_reset,
  B_Compensator_T *localB, DW_Compensator_T *localDW, P_Compensator_T *localP,
  P_Controller_T *Controller_P)
{
  real_T Integrator;
  real_T Integrator_tmp;

  // DiscreteIntegrator: '<S41>/Integrator' incorporates:
  //   Gain: '<S38>/Integral Gain'

  if (rtu_reset && (localDW->Integrator_PrevResetState <= 0)) {
    localDW->Integrator_DSTATE = localP->DiscretePIDController_InitialCo;
  }

  Integrator_tmp = localP->Integrator_gainval * (Controller_P->Compensator_Ki *
    rtu_e);
  Integrator = Integrator_tmp + localDW->Integrator_DSTATE;

  // End of DiscreteIntegrator: '<S41>/Integrator'

  // Sum: '<S50>/Sum' incorporates:
  //   Gain: '<S46>/Proportional Gain'

  localB->Sum = Controller_P->Compensator_Kp * rtu_e + Integrator;

  // Update for DiscreteIntegrator: '<S41>/Integrator'
  localDW->Integrator_DSTATE = Integrator_tmp + Integrator;
  localDW->Integrator_PrevResetState = static_cast<int8_T>(rtu_reset);
}

// Output and update for atomic system: '<S1>/Error Statistics'
void ControllerModelClass::ErrorStatistics(real_T rtu_in, B_ErrorStatistics_T
  *localB, DW_ErrorStatistics_T *localDW)
{
  int32_T bIndx;
  int32_T idx1;
  int32_T k;
  real_T rtb_DelayLine[100];
  real_T Mean_AccVal;

  // Abs: '<S5>/Abs'
  localB->Abs = std::abs(rtu_in);

  // S-Function (sdspsreg2): '<S5>/Delay Line'
  for (k = 0; k < 100 - localDW->DelayLine_BUFF_OFFSET; k++) {
    rtb_DelayLine[k] = localDW->DelayLine_Buff[localDW->DelayLine_BUFF_OFFSET +
      k];
  }

  idx1 = 100 - localDW->DelayLine_BUFF_OFFSET;
  for (k = 0; k < localDW->DelayLine_BUFF_OFFSET; k++) {
    rtb_DelayLine[idx1 + k] = localDW->DelayLine_Buff[k];
  }

  // End of S-Function (sdspsreg2): '<S5>/Delay Line'

  // S-Function (sdspstatfcns): '<S5>/Mean'
  for (idx1 = 0; idx1 < 1; idx1++) {
    Mean_AccVal = rtb_DelayLine[idx1];
    bIndx = 1;
    for (k = 98; k >= 0; k--) {
      Mean_AccVal += rtb_DelayLine[idx1 + bIndx];
      bIndx++;
    }

    localB->Mean = Mean_AccVal / 100.0;
  }

  // End of S-Function (sdspstatfcns): '<S5>/Mean'

  // Update for S-Function (sdspsreg2): '<S5>/Delay Line'
  localDW->DelayLine_Buff[localDW->DelayLine_BUFF_OFFSET] = localB->Abs;
  localDW->DelayLine_BUFF_OFFSET++;
  while (localDW->DelayLine_BUFF_OFFSET >= 100) {
    localDW->DelayLine_BUFF_OFFSET -= 100;
  }

  // End of Update for S-Function (sdspsreg2): '<S5>/Delay Line'
}

//
// System initialize for atomic system:
//    '<S1>/Filter'
//    '<S1>/Filter1'
//
void ControllerModelClass::Filter_Init(DW_Filter_T *localDW, P_Filter_T *localP)
{
  // InitializeConditions for DiscreteTransferFcn: '<S6>/Discrete Filter'
  localDW->DiscreteFilter_states[0] = localP->DiscreteFilter_InitialStates;
  localDW->DiscreteFilter_states[1] = localP->DiscreteFilter_InitialStates;
}

//
// Output and update for atomic system:
//    '<S1>/Filter'
//    '<S1>/Filter1'
//
void ControllerModelClass::Filter(real_T rtu_e, B_Filter_T *localB, DW_Filter_T *
  localDW, P_Filter_T *localP)
{
  real_T DiscreteFilter_tmp;

  // DiscreteTransferFcn: '<S6>/Discrete Filter'
  DiscreteFilter_tmp = ((rtu_e - localP->DiscreteFilter_DenCoef[1] *
    localDW->DiscreteFilter_states[0]) - localP->DiscreteFilter_DenCoef[2] *
                        localDW->DiscreteFilter_states[1]) /
    localP->DiscreteFilter_DenCoef[0];
  localB->DiscreteFilter = (localP->DiscreteFilter_NumCoef[0] *
    DiscreteFilter_tmp + localP->DiscreteFilter_NumCoef[1] *
    localDW->DiscreteFilter_states[0]) + localP->DiscreteFilter_NumCoef[2] *
    localDW->DiscreteFilter_states[1];

  // Update for DiscreteTransferFcn: '<S6>/Discrete Filter'
  localDW->DiscreteFilter_states[1] = localDW->DiscreteFilter_states[0];
  localDW->DiscreteFilter_states[0] = DiscreteFilter_tmp;
}

// System initialize for atomic system: '<S1>/Reference Plant'
void ControllerModelClass::ReferencePlant_Init(DW_ReferencePlant_T *localDW,
  P_Controller_T *Controller_P)
{
  // InitializeConditions for DiscreteIntegrator: '<S8>/Discrete Integrator'
  localDW->DiscreteIntegrator_DSTATE = Controller_P->Plant_IC;
}

// Output and update for atomic system: '<S1>/Reference Plant'
void ControllerModelClass::ReferencePlant(real_T rtu_u, B_ReferencePlant_T
  *localB, DW_ReferencePlant_T *localDW, P_ReferencePlant_T *localP,
  P_Controller_T *Controller_P)
{
  real_T DiscreteIntegrator_tmp;

  // DiscreteIntegrator: '<S8>/Discrete Integrator'
  DiscreteIntegrator_tmp = localP->DiscreteIntegrator_gainval * rtu_u;
  localB->DiscreteIntegrator = DiscreteIntegrator_tmp +
    localDW->DiscreteIntegrator_DSTATE;
  if (localB->DiscreteIntegrator >= Controller_P->Plant_Max) {
    localB->DiscreteIntegrator = Controller_P->Plant_Max;
  } else {
    if (localB->DiscreteIntegrator <= Controller_P->Plant_Min) {
      localB->DiscreteIntegrator = Controller_P->Plant_Min;
    }
  }

  // End of DiscreteIntegrator: '<S8>/Discrete Integrator'

  // Update for DiscreteIntegrator: '<S8>/Discrete Integrator'
  localDW->DiscreteIntegrator_DSTATE = DiscreteIntegrator_tmp +
    localB->DiscreteIntegrator;
  if (localDW->DiscreteIntegrator_DSTATE >= Controller_P->Plant_Max) {
    localDW->DiscreteIntegrator_DSTATE = Controller_P->Plant_Max;
  } else {
    if (localDW->DiscreteIntegrator_DSTATE <= Controller_P->Plant_Min) {
      localDW->DiscreteIntegrator_DSTATE = Controller_P->Plant_Min;
    }
  }
}

// Model step function
void ControllerModelClass::step()
{
  real_T rtb_Switch;
  boolean_T rtb_UnitDelay;
  real_T rtb_e_comp;
  real_T rtb_MultiportSwitch_idx_0;
  real_T rtb_MultiportSwitch_idx_1;

  // Outputs for Atomic SubSystem: '<Root>/Controller'
  // UnitDelay: '<S1>/Unit Delay2'
  rtb_Switch = Controller_DW.UnitDelay2_DSTATE;

  // Sum: '<S1>/Sum1' incorporates:
  //   UnitDelay: '<S1>/Unit Delay'

  rtb_e_comp = rtb_Switch - Controller_DW.UnitDelay_DSTATE;

  // Outputs for Atomic SubSystem: '<S1>/Error Statistics'
  ErrorStatistics(rtb_e_comp, &Controller_B.ErrorStatistics_p,
                  &Controller_DW.ErrorStatistics_p);

  // End of Outputs for SubSystem: '<S1>/Error Statistics'

  // Chart: '<S1>/Compensator Handling' incorporates:
  //   Inport: '<Root>/compensator_state'

  // Gateway: Controller/Compensator Handling
  // During: Controller/Compensator Handling
  if (Controller_DW.is_active_c3_Controller == 0U) {
    // Entry: Controller/Compensator Handling
    Controller_DW.is_active_c3_Controller = 1U;

    // Entry Internal: Controller/Compensator Handling
    // Transition: '<S3>:9'
    Controller_DW.is_c3_Controller = Controller_IN_Select;
  } else {
    switch (Controller_DW.is_c3_Controller) {
     case Controller_IN_AutomaticOff:
      // During 'AutomaticOff': '<S3>:21'
      if (Controller_U.compensator_state != Auto) {
        // Transition: '<S3>:22'
        Controller_DW.is_c3_Controller = Controller_IN_Select;
      } else {
        if (Controller_B.ErrorStatistics_p.Mean >
            Controller_P.AutoCompensator_ThresHystMax) {
          // Transition: '<S3>:24'
          Controller_DW.is_c3_Controller = Controller_IN_AutomaticOn;

          // Outport: '<Root>/enable_compensation'
          // Entry 'AutomaticOn': '<S3>:19'
          Controller_Y.enable_compensation = true;
        }
      }
      break;

     case Controller_IN_AutomaticOn:
      // During 'AutomaticOn': '<S3>:19'
      if (Controller_U.compensator_state != Auto) {
        // Transition: '<S3>:20'
        Controller_DW.is_c3_Controller = Controller_IN_Select;
      } else {
        if (Controller_B.ErrorStatistics_p.Mean <
            Controller_P.AutoCompensator_ThresHystMin) {
          // Transition: '<S3>:23'
          Controller_DW.is_c3_Controller = Controller_IN_AutomaticOff;

          // Outport: '<Root>/enable_compensation'
          // Entry 'AutomaticOff': '<S3>:21'
          Controller_Y.enable_compensation = false;
        }
      }
      break;

     default:
      // During 'Select': '<S3>:3'
      // Transition: '<S3>:15'
      if (Controller_U.compensator_state == Off) {
        // Outport: '<Root>/enable_compensation'
        // Transition: '<S3>:12'
        Controller_Y.enable_compensation = false;
        Controller_DW.is_c3_Controller = Controller_IN_Select;
      } else if (Controller_U.compensator_state == On) {
        // Outport: '<Root>/enable_compensation'
        // Transition: '<S3>:11'
        Controller_Y.enable_compensation = true;
        Controller_DW.is_c3_Controller = Controller_IN_Select;
      } else {
        // Transition: '<S3>:17'
        if (Controller_B.ErrorStatistics_p.Mean >
            Controller_P.AutoCompensator_ThresHystMax) {
          // Transition: '<S3>:26'
          Controller_DW.is_c3_Controller = Controller_IN_AutomaticOn;

          // Outport: '<Root>/enable_compensation'
          // Entry 'AutomaticOn': '<S3>:19'
          Controller_Y.enable_compensation = true;
        } else {
          // Transition: '<S3>:27'
          Controller_DW.is_c3_Controller = Controller_IN_AutomaticOff;

          // Outport: '<Root>/enable_compensation'
          // Entry 'AutomaticOff': '<S3>:21'
          Controller_Y.enable_compensation = false;
        }
      }
      break;
    }
  }

  // End of Chart: '<S1>/Compensator Handling'

  // UnitDelay: '<S58>/Unit Delay'
  rtb_UnitDelay = Controller_DW.UnitDelay_DSTATE_e;

  // MultiPortSwitch: '<S58>/Multiport Switch' incorporates:
  //   Constant: '<S58>/Constant1'
  //   Constant: '<S58>/either edge'
  //   Constant: '<S58>/neg. edge'
  //   Constant: '<S58>/pos. edge'

  switch (static_cast<int32_T>(Controller_P.EdgeDetector_model)) {
   case 1:
    rtb_MultiportSwitch_idx_0 = Controller_P.posedge_Value[0];
    rtb_MultiportSwitch_idx_1 = Controller_P.posedge_Value[1];
    break;

   case 2:
    rtb_MultiportSwitch_idx_0 = Controller_P.negedge_Value[0];
    rtb_MultiportSwitch_idx_1 = Controller_P.negedge_Value[1];
    break;

   default:
    rtb_MultiportSwitch_idx_0 = Controller_P.eitheredge_Value[0];
    rtb_MultiportSwitch_idx_1 = Controller_P.eitheredge_Value[1];
    break;
  }

  // End of MultiPortSwitch: '<S58>/Multiport Switch'

  // Outputs for Enabled SubSystem: '<S58>/POSITIVE Edge' incorporates:
  //   EnablePort: '<S60>/Enable'

  if (rtb_MultiportSwitch_idx_0 > 0.0) {
    // RelationalOperator: '<S60>/Relational Operator1' incorporates:
    //   Outport: '<Root>/enable_compensation'

    Controller_B.RelationalOperator1 = (static_cast<int32_T>
      (Controller_Y.enable_compensation) > static_cast<int32_T>(rtb_UnitDelay));
  }

  // End of Outputs for SubSystem: '<S58>/POSITIVE Edge'

  // Outputs for Enabled SubSystem: '<S58>/NEGATIVE Edge' incorporates:
  //   EnablePort: '<S59>/Enable'

  if (rtb_MultiportSwitch_idx_1 > 0.0) {
    // RelationalOperator: '<S59>/Relational Operator1' incorporates:
    //   Outport: '<Root>/enable_compensation'

    Controller_B.RelationalOperator1_n = (static_cast<int32_T>(rtb_UnitDelay) >
      static_cast<int32_T>(Controller_Y.enable_compensation));
  }

  // End of Outputs for SubSystem: '<S58>/NEGATIVE Edge'

  // Outputs for Atomic SubSystem: '<S1>/Compensator'
  // Logic: '<S58>/Logical Operator1'
  Compensator(rtb_e_comp, Controller_B.RelationalOperator1 ||
              Controller_B.RelationalOperator1_n, &Controller_B.Compensator_k,
              &Controller_DW.Compensator_k, &Controller_P.Compensator_k,
              &Controller_P);

  // End of Outputs for SubSystem: '<S1>/Compensator'

  // Outputs for Atomic SubSystem: '<S1>/Filter'
  // Sum: '<S1>/Sum3' incorporates:
  //   Inport: '<Root>/reference'

  Filter(Controller_U.reference - rtb_Switch, &Controller_B.Filter_i,
         &Controller_DW.Filter_i, &Controller_P.Filter_i);

  // End of Outputs for SubSystem: '<S1>/Filter'

  // Outputs for Atomic SubSystem: '<S1>/Filter1'
  // Sum: '<S1>/Sum2' incorporates:
  //   Inport: '<Root>/reference'
  //   UnitDelay: '<S1>/Unit Delay'

  Filter(Controller_U.reference - Controller_DW.UnitDelay_DSTATE,
         &Controller_B.Filter1, &Controller_DW.Filter1, &Controller_P.Filter1);

  // End of Outputs for SubSystem: '<S1>/Filter1'

  // Outputs for Atomic SubSystem: '<S1>/Reference Plant'
  ReferencePlant(Controller_B.Filter_i.DiscreteFilter,
                 &Controller_B.ReferencePlant_j, &Controller_DW.ReferencePlant_j,
                 &Controller_P.ReferencePlant_j, &Controller_P);

  // End of Outputs for SubSystem: '<S1>/Reference Plant'

  // Update for UnitDelay: '<S1>/Unit Delay' incorporates:
  //   Inport: '<Root>/plant_output'

  Controller_DW.UnitDelay_DSTATE = Controller_U.plant_output;

  // Update for UnitDelay: '<S1>/Unit Delay2'
  Controller_DW.UnitDelay2_DSTATE =
    Controller_B.ReferencePlant_j.DiscreteIntegrator;

  // Update for UnitDelay: '<S58>/Unit Delay' incorporates:
  //   Outport: '<Root>/enable_compensation'

  Controller_DW.UnitDelay_DSTATE_e = Controller_Y.enable_compensation;

  // Switch: '<S1>/Switch' incorporates:
  //   Constant: '<S1>/Constant'
  //   Outport: '<Root>/enable_compensation'

  if (Controller_Y.enable_compensation) {
    rtb_Switch = Controller_B.Compensator_k.Sum;
  } else {
    rtb_Switch = Controller_P.Constant_Value;
  }

  // End of Switch: '<S1>/Switch'

  // Outport: '<Root>/controller_output' incorporates:
  //   Sum: '<S1>/Sum4'

  Controller_Y.controller_output = Controller_B.Filter1.DiscreteFilter +
    rtb_Switch;

  // End of Outputs for SubSystem: '<Root>/Controller'

  // Outport: '<Root>/controller_reference'
  Controller_Y.controller_reference = Controller_B.Filter_i.DiscreteFilter;

  // Outport: '<Root>/plant_reference'
  Controller_Y.plant_reference =
    Controller_B.ReferencePlant_j.DiscreteIntegrator;

  // Outport: '<Root>/error_statistics'
  Controller_Y.error_statistics = Controller_B.ErrorStatistics_p.Mean;
}

// Model initialize function
void ControllerModelClass::initialize()
{
  // SystemInitialize for Atomic SubSystem: '<Root>/Controller'
  // InitializeConditions for UnitDelay: '<S1>/Unit Delay'
  Controller_DW.UnitDelay_DSTATE = Controller_P.Plant_IC;

  // InitializeConditions for UnitDelay: '<S1>/Unit Delay2'
  Controller_DW.UnitDelay2_DSTATE = Controller_P.Plant_IC;

  // InitializeConditions for UnitDelay: '<S58>/Unit Delay'
  Controller_DW.UnitDelay_DSTATE_e = Controller_P.EdgeDetector_ic;

  // SystemInitialize for Enabled SubSystem: '<S58>/POSITIVE Edge'
  // SystemInitialize for Outport: '<S60>/OUT'
  Controller_B.RelationalOperator1 = Controller_P.OUT_Y0_d;

  // End of SystemInitialize for SubSystem: '<S58>/POSITIVE Edge'

  // SystemInitialize for Enabled SubSystem: '<S58>/NEGATIVE Edge'
  // SystemInitialize for Outport: '<S59>/OUT'
  Controller_B.RelationalOperator1_n = Controller_P.OUT_Y0;

  // End of SystemInitialize for SubSystem: '<S58>/NEGATIVE Edge'

  // SystemInitialize for Atomic SubSystem: '<S1>/Compensator'
  Compensator_Init(&Controller_DW.Compensator_k, &Controller_P.Compensator_k);

  // End of SystemInitialize for SubSystem: '<S1>/Compensator'

  // SystemInitialize for Atomic SubSystem: '<S1>/Filter'
  Filter_Init(&Controller_DW.Filter_i, &Controller_P.Filter_i);

  // End of SystemInitialize for SubSystem: '<S1>/Filter'

  // SystemInitialize for Atomic SubSystem: '<S1>/Filter1'
  Filter_Init(&Controller_DW.Filter1, &Controller_P.Filter1);

  // End of SystemInitialize for SubSystem: '<S1>/Filter1'

  // SystemInitialize for Atomic SubSystem: '<S1>/Reference Plant'
  ReferencePlant_Init(&Controller_DW.ReferencePlant_j, &Controller_P);

  // End of SystemInitialize for SubSystem: '<S1>/Reference Plant'
  // End of SystemInitialize for SubSystem: '<Root>/Controller'
}

// Model terminate function
void ControllerModelClass::terminate()
{
  // (no terminate code required)
}

// Constructor
ControllerModelClass::ControllerModelClass():
  Controller_B()
  ,Controller_DW()
  ,Controller_U()
  ,Controller_Y()
  ,Controller_M()
{
  // Currently there is no constructor body generated.
}

// Destructor
ControllerModelClass::~ControllerModelClass()
{
  // Currently there is no destructor body generated.
}

// Real-Time Model get method
ControllerModelClass::RT_MODEL_Controller_T * ControllerModelClass::getRTM()
{
  return (&Controller_M);
}

//
// File trailer for generated code.
//
// [EOF]
//
