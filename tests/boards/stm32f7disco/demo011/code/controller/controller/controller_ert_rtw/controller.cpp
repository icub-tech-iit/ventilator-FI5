//
// Non-Degree Granting Education License -- for use at non-degree
// granting, nonprofit, educational organizations only. Not for
// government, commercial, or other organizational use.
//
// File: controller.cpp
//
// Code generated for Simulink model 'controller'.
//
// Model version                  : 1.477
// Simulink Coder version         : 9.3 (R2020a) 18-Nov-2019
// C/C++ source code generated on : Thu May  7 16:01:19 2020
//
// Target selection: ert.tlc
// Embedded hardware selection: Intel->x86-64 (Windows64)
// Code generation objectives: Unspecified
// Validation result: Not run
//
#include "controller.h"
#include "controller_private.h"

// Named constants for Chart: '<S1>/Pulse Generator'
const uint8_T controller_IN_DOWN = 1U;
const uint8_T controller_IN_NO_ACTIVE_CHILD = 0U;
const uint8_T controller_IN_UP = 2U;
void controllerModelClass::controller_SystemCore_release
  (b_dspcodegen_BiquadFilter_con_T *obj)
{
  if (obj->isInitialized == 1) {
    obj->isInitialized = 2;
  }
}

void controllerModelClass::cont_LPHPFilterBase_releaseImpl
  (dsp_LowpassFilter_controller_T *obj)
{
  controller_SystemCore_release(obj->FilterObj);
  obj->NumChannels = -1;
}

void controllerModelClass::contr_SystemCore_releaseWrapper
  (dsp_LowpassFilter_controller_T *obj)
{
  if (obj->isSetupComplete) {
    cont_LPHPFilterBase_releaseImpl(obj);
  }
}

void controllerModelClass::controller_SystemCore_release_j
  (dsp_LowpassFilter_controller_T *obj)
{
  if (obj->isInitialized == 1) {
    contr_SystemCore_releaseWrapper(obj);
  }
}

void controllerModelClass::controller_SystemCore_delete_j
  (dsp_LowpassFilter_controller_T *obj)
{
  controller_SystemCore_release_j(obj);
}

void controllerModelClass::matlabCodegenHandle_matlabCod_j
  (dsp_LowpassFilter_controller_T *obj)
{
  if (!obj->matlabCodegenIsDeleted) {
    obj->matlabCodegenIsDeleted = true;
    controller_SystemCore_delete_j(obj);
  }
}

void controllerModelClass::controller_SystemCore_delete
  (b_dspcodegen_BiquadFilter_con_T *obj)
{
  controller_SystemCore_release(obj);
}

void controllerModelClass::matlabCodegenHandle_matlabCodeg
  (b_dspcodegen_BiquadFilter_con_T *obj)
{
  if (!obj->matlabCodegenIsDeleted) {
    obj->matlabCodegenIsDeleted = true;
    controller_SystemCore_delete(obj);
  }
}

// Model step function
void controllerModelClass::step()
{
  real32_T rtb_Divide5;
  real32_T rtb_SumFdbk;
  real32_T rtb_Divide3;
  real32_T rtb_IProdOut_d;
  real32_T rtb_psi_cmH2O;
  boolean_T rtb_pulse;
  boolean_T rtb_RelationalOperator;
  real32_T rtb_PIP;
  boolean_T rtb_Compare;
  ZCEventType zcEvent;
  boolean_T rtb_Compare_k;
  boolean_T rtb_Compare_p;
  boolean_T rtb_LogicalOperator1;
  real32_T rtb_Divide;
  int32_T i;
  real32_T rtb_TmpSignalConversionAtHSCL_0;
  real_T rtb_MultiportSwitch_idx_0;
  real_T rtb_MultiportSwitch_idx_1;
  real32_T rtb_pnm_idx_0;
  real32_T rtb_pnm_idx_1;
  real32_T rtb_pnm_idx_2;
  real32_T rtb_TmpSignalConversionAtHSCL_1;
  real32_T rtb_TmpSignalConversionAtHSCL_2;
  real32_T rtb_TmpSignalConversionAtHSCL_3;

  // Outputs for Enabled SubSystem: '<Root>/Controller' incorporates:
  //   EnablePort: '<S1>/Enable'

  // Inport: '<Root>/enable'
  if (controller_U.enable) {
    if (!controller_DW.Controller_MODE) {
      // InitializeConditions for UnitDelay: '<S1>/Unit Delay1'
      controller_DW.UnitDelay1_DSTATE[0] =
        controller_P.UnitDelay1_InitialCondition[0];
      controller_DW.UnitDelay1_DSTATE[1] =
        controller_P.UnitDelay1_InitialCondition[1];
      controller_DW.UnitDelay1_DSTATE[2] =
        controller_P.UnitDelay1_InitialCondition[2];
      controller_DW.UnitDelay1_DSTATE[3] =
        controller_P.UnitDelay1_InitialCondition[3];

      // InitializeConditions for UnitDelay: '<S1>/Unit Delay2'
      controller_DW.UnitDelay2_DSTATE = controller_P.UnitDelay2_InitialCondition;

      // InitializeConditions for UnitDelay: '<S1>/Unit Delay'
      controller_DW.UnitDelay_DSTATE_m[0] =
        controller_P.UnitDelay_InitialCondition_g;
      controller_DW.UnitDelay_DSTATE_m[1] =
        controller_P.UnitDelay_InitialCondition_g;

      // InitializeConditions for UnitDelay: '<S1>/Unit Delay3'
      controller_DW.UnitDelay3_DSTATE = controller_P.UnitDelay3_InitialCondition;

      // InitializeConditions for DiscreteIntegrator: '<S1>/Discrete-Time Integrator' 
      controller_DW.DiscreteTimeIntegrator_DSTATE =
        controller_P.DiscreteTimeIntegrator_IC;
      controller_DW.DiscreteTimeIntegrator_PrevRese = 2;

      // InitializeConditions for UnitDelay: '<S20>/Unit Delay'
      controller_DW.UnitDelay_DSTATE_d = controller_P.Monostable1_ic;

      // InitializeConditions for UnitDelay: '<S19>/delay'
      controller_DW.delay_DSTATE = controller_P.delay_InitialCondition;

      // InitializeConditions for UnitDelay: '<S19>/Unit Delay'
      controller_DW.UnitDelay_DSTATE = controller_P.UnitDelay_InitialCondition;

      // InitializeConditions for MATLABSystem: '<S1>/HSC LP'
      if (controller_DW.obj.FilterObj->isInitialized == 1) {
        // System object Initialization function: dsp.BiquadFilter
        for (i = 0; i < 16; i++) {
          controller_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES[i] =
            controller_DW.obj.FilterObj->cSFunObject.P0_ICRTP;
        }

        for (i = 0; i < 16; i++) {
          controller_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES[i] =
            controller_DW.obj.FilterObj->cSFunObject.P5_IC2RTP;
        }
      }

      // End of InitializeConditions for MATLABSystem: '<S1>/HSC LP'

      // SystemReset for Chart: '<S1>/Pulse Generator'
      controller_DW.temporalCounter_i1 = 0U;
      controller_DW.is_active_c3_controller = 0U;
      controller_DW.is_c3_controller = controller_IN_NO_ACTIVE_CHILD;
      controller_DW.Controller_MODE = true;
    }

    // SignalConversion generated from: '<S1>/HSC LP' incorporates:
    //   Bias: '<S15>/Bias'
    //   Bias: '<S16>/Bias'
    //   Gain: '<S15>/Gain'
    //   Gain: '<S15>/Gain1'
    //   Gain: '<S16>/Gain'
    //   Gain: '<S16>/Gain1'
    //   Gain: '<S1>/mabr_to_cmH2O'
    //   Gain: '<S1>/psi_cmH2O'
    //   Inport: '<Root>/params'
    //   Product: '<S15>/Product'
    //   Product: '<S16>/Product'
    //   Sum: '<S16>/Sum'
    //   UnitDelay: '<S1>/Unit Delay1'

    rtb_TmpSignalConversionAtHSCL_1 = ((controller_P.Gain_Gain_h *
      controller_DW.UnitDelay1_DSTATE[0] + controller_P.Bias_Bias) *
      controller_U.params[1] * controller_P.Gain1_Gain - controller_U.params[1])
      * controller_P.mabr_to_cmH2O_Gain;
    rtb_TmpSignalConversionAtHSCL_2 = ((controller_P.Gain_Gain_h *
      controller_DW.UnitDelay1_DSTATE[1] + controller_P.Bias_Bias) *
      controller_U.params[1] * controller_P.Gain1_Gain - controller_U.params[1])
      * controller_P.mabr_to_cmH2O_Gain;
    rtb_TmpSignalConversionAtHSCL_3 = ((controller_P.Gain_Gain_h *
      controller_DW.UnitDelay1_DSTATE[2] + controller_P.Bias_Bias) *
      controller_U.params[1] * controller_P.Gain1_Gain - controller_U.params[1])
      * controller_P.mabr_to_cmH2O_Gain;
    rtb_TmpSignalConversionAtHSCL_0 = (controller_P.Gain_Gain_i *
      controller_DW.UnitDelay1_DSTATE[3] + controller_P.Bias_Bias_h) *
      controller_U.params[2] * controller_P.Gain1_Gain_d *
      controller_P.psi_cmH2O_Gain;

    // MATLABSystem: '<S1>/HSC LP'
    if (controller_DW.obj.FilterObj->isInitialized != 1) {
      controller_DW.obj.FilterObj->isSetupComplete = false;
      controller_DW.obj.FilterObj->isInitialized = 1;
      controller_DW.obj.FilterObj->isSetupComplete = true;

      // System object Initialization function: dsp.BiquadFilter
      for (i = 0; i < 16; i++) {
        controller_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES[i] =
          controller_DW.obj.FilterObj->cSFunObject.P0_ICRTP;
      }

      for (i = 0; i < 16; i++) {
        controller_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES[i] =
          controller_DW.obj.FilterObj->cSFunObject.P5_IC2RTP;
      }
    }

    // System object Outputs function: dsp.BiquadFilter
    rtb_Divide = controller_DW.obj.FilterObj->cSFunObject.P1_RTP1COEFF[0] *
      rtb_TmpSignalConversionAtHSCL_1;
    rtb_Divide += controller_DW.obj.FilterObj->cSFunObject.P1_RTP1COEFF[1] *
      controller_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES[0];
    rtb_Divide += controller_DW.obj.FilterObj->cSFunObject.P1_RTP1COEFF[2] *
      controller_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES[1];
    rtb_IProdOut_d = rtb_Divide -
      controller_DW.obj.FilterObj->cSFunObject.P2_RTP2COEFF[0] *
      controller_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES[0];
    rtb_IProdOut_d -= controller_DW.obj.FilterObj->cSFunObject.P2_RTP2COEFF[1] *
      controller_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES[1];
    controller_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES[1] =
      controller_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES[0];
    controller_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES[0] =
      rtb_TmpSignalConversionAtHSCL_1;
    controller_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES[1] =
      controller_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES[0];
    controller_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES[0] = rtb_IProdOut_d;
    rtb_psi_cmH2O = rtb_IProdOut_d;
    rtb_Divide = controller_DW.obj.FilterObj->cSFunObject.P1_RTP1COEFF[3] *
      rtb_IProdOut_d;
    rtb_Divide += controller_DW.obj.FilterObj->cSFunObject.P1_RTP1COEFF[4] *
      controller_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES[2];
    rtb_Divide += controller_DW.obj.FilterObj->cSFunObject.P1_RTP1COEFF[5] *
      controller_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES[3];
    rtb_IProdOut_d = rtb_Divide -
      controller_DW.obj.FilterObj->cSFunObject.P2_RTP2COEFF[2] *
      controller_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES[2];
    rtb_IProdOut_d -= controller_DW.obj.FilterObj->cSFunObject.P2_RTP2COEFF[3] *
      controller_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES[3];
    controller_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES[3] =
      controller_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES[2];
    controller_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES[2] = rtb_psi_cmH2O;
    controller_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES[3] =
      controller_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES[2];
    controller_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES[2] = rtb_IProdOut_d;
    rtb_TmpSignalConversionAtHSCL_1 = rtb_IProdOut_d;
    rtb_Divide = controller_DW.obj.FilterObj->cSFunObject.P1_RTP1COEFF[0] *
      rtb_TmpSignalConversionAtHSCL_2;
    rtb_Divide += controller_DW.obj.FilterObj->cSFunObject.P1_RTP1COEFF[1] *
      controller_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES[4];
    rtb_Divide += controller_DW.obj.FilterObj->cSFunObject.P1_RTP1COEFF[2] *
      controller_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES[5];
    rtb_IProdOut_d = rtb_Divide -
      controller_DW.obj.FilterObj->cSFunObject.P2_RTP2COEFF[0] *
      controller_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES[4];
    rtb_IProdOut_d -= controller_DW.obj.FilterObj->cSFunObject.P2_RTP2COEFF[1] *
      controller_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES[5];
    controller_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES[5] =
      controller_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES[4];
    controller_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES[4] =
      rtb_TmpSignalConversionAtHSCL_2;
    controller_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES[5] =
      controller_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES[4];
    controller_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES[4] = rtb_IProdOut_d;
    rtb_psi_cmH2O = rtb_IProdOut_d;
    rtb_Divide = controller_DW.obj.FilterObj->cSFunObject.P1_RTP1COEFF[3] *
      rtb_IProdOut_d;
    rtb_Divide += controller_DW.obj.FilterObj->cSFunObject.P1_RTP1COEFF[4] *
      controller_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES[6];
    rtb_Divide += controller_DW.obj.FilterObj->cSFunObject.P1_RTP1COEFF[5] *
      controller_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES[7];
    rtb_IProdOut_d = rtb_Divide -
      controller_DW.obj.FilterObj->cSFunObject.P2_RTP2COEFF[2] *
      controller_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES[6];
    rtb_IProdOut_d -= controller_DW.obj.FilterObj->cSFunObject.P2_RTP2COEFF[3] *
      controller_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES[7];
    controller_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES[7] =
      controller_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES[6];
    controller_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES[6] = rtb_psi_cmH2O;
    controller_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES[7] =
      controller_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES[6];
    controller_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES[6] = rtb_IProdOut_d;
    rtb_pnm_idx_0 = rtb_IProdOut_d;
    rtb_Divide = controller_DW.obj.FilterObj->cSFunObject.P1_RTP1COEFF[0] *
      rtb_TmpSignalConversionAtHSCL_3;
    rtb_Divide += controller_DW.obj.FilterObj->cSFunObject.P1_RTP1COEFF[1] *
      controller_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES[8];
    rtb_Divide += controller_DW.obj.FilterObj->cSFunObject.P1_RTP1COEFF[2] *
      controller_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES[9];
    rtb_IProdOut_d = rtb_Divide -
      controller_DW.obj.FilterObj->cSFunObject.P2_RTP2COEFF[0] *
      controller_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES[8];
    rtb_IProdOut_d -= controller_DW.obj.FilterObj->cSFunObject.P2_RTP2COEFF[1] *
      controller_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES[9];
    controller_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES[9] =
      controller_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES[8];
    controller_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES[8] =
      rtb_TmpSignalConversionAtHSCL_3;
    controller_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES[9] =
      controller_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES[8];
    controller_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES[8] = rtb_IProdOut_d;
    rtb_psi_cmH2O = rtb_IProdOut_d;
    rtb_Divide = controller_DW.obj.FilterObj->cSFunObject.P1_RTP1COEFF[3] *
      rtb_IProdOut_d;
    rtb_Divide += controller_DW.obj.FilterObj->cSFunObject.P1_RTP1COEFF[4] *
      controller_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES[10];
    rtb_Divide += controller_DW.obj.FilterObj->cSFunObject.P1_RTP1COEFF[5] *
      controller_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES[11];
    rtb_IProdOut_d = rtb_Divide -
      controller_DW.obj.FilterObj->cSFunObject.P2_RTP2COEFF[2] *
      controller_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES[10];
    rtb_IProdOut_d -= controller_DW.obj.FilterObj->cSFunObject.P2_RTP2COEFF[3] *
      controller_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES[11];
    controller_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES[11] =
      controller_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES[10];
    controller_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES[10] = rtb_psi_cmH2O;
    controller_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES[11] =
      controller_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES[10];
    controller_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES[10] = rtb_IProdOut_d;

    // Outport: '<Root>/signals' incorporates:
    //   MATLABSystem: '<S1>/HSC LP'
    //   SignalConversion generated from: '<S1>/signals'

    controller_Y.signals[4] = rtb_IProdOut_d;

    // MATLABSystem: '<S1>/HSC LP'
    rtb_Divide = controller_DW.obj.FilterObj->cSFunObject.P1_RTP1COEFF[0] *
      rtb_TmpSignalConversionAtHSCL_0;
    rtb_Divide += controller_DW.obj.FilterObj->cSFunObject.P1_RTP1COEFF[1] *
      controller_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES[12];
    rtb_Divide += controller_DW.obj.FilterObj->cSFunObject.P1_RTP1COEFF[2] *
      controller_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES[13];
    rtb_IProdOut_d = rtb_Divide -
      controller_DW.obj.FilterObj->cSFunObject.P2_RTP2COEFF[0] *
      controller_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES[12];
    rtb_IProdOut_d -= controller_DW.obj.FilterObj->cSFunObject.P2_RTP2COEFF[1] *
      controller_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES[13];
    controller_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES[13] =
      controller_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES[12];
    controller_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES[12] =
      rtb_TmpSignalConversionAtHSCL_0;
    controller_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES[13] =
      controller_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES[12];
    controller_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES[12] = rtb_IProdOut_d;
    rtb_psi_cmH2O = rtb_IProdOut_d;
    rtb_Divide = controller_DW.obj.FilterObj->cSFunObject.P1_RTP1COEFF[3] *
      rtb_IProdOut_d;
    rtb_Divide += controller_DW.obj.FilterObj->cSFunObject.P1_RTP1COEFF[4] *
      controller_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES[14];
    rtb_Divide += controller_DW.obj.FilterObj->cSFunObject.P1_RTP1COEFF[5] *
      controller_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES[15];
    rtb_IProdOut_d = rtb_Divide -
      controller_DW.obj.FilterObj->cSFunObject.P2_RTP2COEFF[2] *
      controller_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES[14];
    rtb_IProdOut_d -= controller_DW.obj.FilterObj->cSFunObject.P2_RTP2COEFF[3] *
      controller_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES[15];
    controller_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES[15] =
      controller_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES[14];
    controller_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES[14] = rtb_psi_cmH2O;
    controller_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES[15] =
      controller_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES[14];
    controller_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES[14] = rtb_IProdOut_d;
    rtb_TmpSignalConversionAtHSCL_0 = rtb_IProdOut_d;

    // Bias: '<S13>/Bias' incorporates:
    //   Gain: '<S13>/Gain'
    //   UnitDelay: '<S1>/Unit Delay2'

    rtb_IProdOut_d = controller_P.Gain_Gain_m * controller_DW.UnitDelay2_DSTATE
      + controller_P.Bias_Bias_b;

    // Product: '<S2>/Divide1' incorporates:
    //   Bias: '<S2>/Bias'
    //   Constant: '<S2>/Constant1'
    //   Gain: '<S2>/cmH2O to Pa'
    //   MATLABSystem: '<S1>/HSC LP'

    rtb_psi_cmH2O = controller_P.Constant1_Value_k /
      (controller_P.cmH2OtoPa_Gain * rtb_pnm_idx_0 + controller_P.Bias_Bias_j);

    // Product: '<S2>/Divide' incorporates:
    //   Bias: '<S2>/K'
    //   Constant: '<S2>/Constant'

    rtb_Divide = (rtb_IProdOut_d + controller_P.K_Bias) /
      controller_P.Constant_Value_h;

    // Product: '<S2>/Product1' incorporates:
    //   Bias: '<S14>/Bias'
    //   Gain: '<S14>/Gain'
    //   Gain: '<S14>/Gain1'
    //   Inport: '<Root>/params'
    //   Product: '<S14>/Product'
    //   Product: '<S2>/Product'
    //   UnitDelay: '<S1>/Unit Delay'

    rtb_TmpSignalConversionAtHSCL_3 = (controller_P.Gain_Gain_f *
      controller_DW.UnitDelay_DSTATE_m[0] + controller_P.Bias_Bias_l) *
      controller_U.params[0] * controller_P.Gain1_Gain_e * rtb_Divide *
      rtb_psi_cmH2O;

    // Outputs for Enabled SubSystem: '<S1>/Compute Trigger' incorporates:
    //   EnablePort: '<S8>/Enable'

    // Logic: '<S1>/Logical Operator' incorporates:
    //   UnitDelay: '<S1>/Unit Delay3'

    if (!(controller_DW.UnitDelay3_DSTATE != 0.0F)) {
      controller_DW.ComputeTrigger_MODE = true;

      // DataTypeConversion: '<S8>/Data Type Conversion' incorporates:
      //   Constant: '<S18>/Constant'
      //   MATLABSystem: '<S1>/HSC LP'
      //   RelationalOperator: '<S18>/Compare'

      controller_B.DataTypeConversion = (rtb_pnm_idx_0 <=
        controller_P.pressure_thres_const);
    } else {
      if (controller_DW.ComputeTrigger_MODE) {
        // Disable for Outport: '<S8>/Out'
        controller_B.DataTypeConversion = controller_P.Out_Y0;
        controller_DW.ComputeTrigger_MODE = false;
      }
    }

    // End of Logic: '<S1>/Logical Operator'
    // End of Outputs for SubSystem: '<S1>/Compute Trigger'

    // Gain: '<S1>/Gain' incorporates:
    //   Bias: '<S14>/Bias'
    //   Gain: '<S14>/Gain'
    //   Gain: '<S14>/Gain1'
    //   Inport: '<Root>/params'
    //   Product: '<S14>/Product'
    //   Product: '<S2>/Product'
    //   Product: '<S2>/Product1'
    //   UnitDelay: '<S1>/Unit Delay'

    rtb_TmpSignalConversionAtHSCL_2 = (controller_P.Gain_Gain_f *
      controller_DW.UnitDelay_DSTATE_m[1] + controller_P.Bias_Bias_l) *
      controller_U.params[0] * controller_P.Gain1_Gain_e * rtb_Divide *
      rtb_psi_cmH2O * controller_P.Gain_Gain_e;

    // RelationalOperator: '<S4>/Compare' incorporates:
    //   Constant: '<S4>/Constant'

    rtb_Compare_k = (controller_P.Controller_control_mode ==
                     controller_P.CompareToConstant1_const);

    // RelationalOperator: '<S5>/Compare' incorporates:
    //   Constant: '<S5>/Constant'

    rtb_Compare = (controller_P.Controller_control_mode ==
                   controller_P.CompareToConstant2_const);

    // RelationalOperator: '<S6>/Compare' incorporates:
    //   Constant: '<S6>/Constant'

    rtb_Compare_p = (controller_P.Controller_control_mode ==
                     controller_P.CompareToConstant3_const);

    // Logic: '<S1>/Logical Operator1'
    rtb_LogicalOperator1 = (rtb_Compare_k || rtb_Compare || rtb_Compare_p);

    // DiscreteIntegrator: '<S1>/Discrete-Time Integrator' incorporates:
    //   Sum: '<S1>/Sum1'

    if (rtb_LogicalOperator1 && (controller_DW.DiscreteTimeIntegrator_PrevRese <=
         0)) {
      controller_DW.DiscreteTimeIntegrator_DSTATE =
        controller_P.DiscreteTimeIntegrator_IC;
    }

    rtb_psi_cmH2O = controller_P.DiscreteTimeIntegrator_gainval *
      (rtb_TmpSignalConversionAtHSCL_3 + rtb_TmpSignalConversionAtHSCL_2);
    controller_B.DiscreteTimeIntegrator = rtb_psi_cmH2O +
      controller_DW.DiscreteTimeIntegrator_DSTATE;

    // End of DiscreteIntegrator: '<S1>/Discrete-Time Integrator'

    // Outport: '<Root>/signals' incorporates:
    //   MATLABSystem: '<S1>/HSC LP'
    //   SignalConversion generated from: '<S1>/signals'

    controller_Y.signals[0] = rtb_TmpSignalConversionAtHSCL_1;
    controller_Y.signals[1] = rtb_TmpSignalConversionAtHSCL_3;
    controller_Y.signals[2] = rtb_pnm_idx_0;
    controller_Y.signals[3] = rtb_IProdOut_d;
    controller_Y.signals[5] = rtb_TmpSignalConversionAtHSCL_2;
    controller_Y.signals[6] = rtb_TmpSignalConversionAtHSCL_0;
    controller_Y.signals[7] = controller_B.DiscreteTimeIntegrator;
    controller_Y.signals[8] = controller_B.DataTypeConversion;

    // MultiPortSwitch: '<S20>/Multiport Switch' incorporates:
    //   Constant: '<S20>/Constant1'
    //   Constant: '<S20>/either edge'
    //   Constant: '<S20>/neg. edge'
    //   Constant: '<S20>/pos. edge'

    switch (static_cast<int32_T>(controller_P.Constant1_Value)) {
     case 1:
      rtb_MultiportSwitch_idx_0 = controller_P.posedge_Value[0];
      rtb_MultiportSwitch_idx_1 = controller_P.posedge_Value[1];
      break;

     case 2:
      rtb_MultiportSwitch_idx_0 = controller_P.negedge_Value[0];
      rtb_MultiportSwitch_idx_1 = controller_P.negedge_Value[1];
      break;

     default:
      rtb_MultiportSwitch_idx_0 = controller_P.eitheredge_Value[0];
      rtb_MultiportSwitch_idx_1 = controller_P.eitheredge_Value[1];
      break;
    }

    // End of MultiPortSwitch: '<S20>/Multiport Switch'

    // Outputs for Enabled SubSystem: '<S20>/POSITIVE Edge' incorporates:
    //   EnablePort: '<S22>/Enable'

    if (rtb_MultiportSwitch_idx_0 > 0.0) {
      // RelationalOperator: '<S22>/Relational Operator1' incorporates:
      //   DataTypeConversion: '<S19>/Data Type Conversion2'
      //   UnitDelay: '<S20>/Unit Delay'

      controller_B.RelationalOperator1 = (static_cast<int32_T>
        (controller_B.DataTypeConversion != 0.0F) > static_cast<int32_T>
        (controller_DW.UnitDelay_DSTATE_d));
    }

    // End of Outputs for SubSystem: '<S20>/POSITIVE Edge'

    // Outputs for Enabled SubSystem: '<S20>/NEGATIVE Edge' incorporates:
    //   EnablePort: '<S21>/Enable'

    if (rtb_MultiportSwitch_idx_1 > 0.0) {
      // RelationalOperator: '<S21>/Relational Operator1' incorporates:
      //   DataTypeConversion: '<S19>/Data Type Conversion2'
      //   UnitDelay: '<S20>/Unit Delay'

      controller_B.RelationalOperator1_h = (static_cast<int32_T>
        (controller_DW.UnitDelay_DSTATE_d) > static_cast<int32_T>
        (controller_B.DataTypeConversion != 0.0F));
    }

    // End of Outputs for SubSystem: '<S20>/NEGATIVE Edge'

    // Switch: '<S19>/Switch' incorporates:
    //   Constant: '<S19>/T_monostable'
    //   Logic: '<S19>/Logical Operator1'
    //   Logic: '<S19>/Logical Operator2'
    //   Logic: '<S20>/Logical Operator1'
    //   UnitDelay: '<S19>/Unit Delay'
    //   UnitDelay: '<S19>/delay'

    if ((!controller_DW.delay_DSTATE) && (controller_B.RelationalOperator1 ||
         controller_B.RelationalOperator1_h)) {
      rtb_MultiportSwitch_idx_0 = controller_P.Monostable1_PulseDuration -
        2.2204460492503131E-16;
    } else {
      rtb_MultiportSwitch_idx_0 = controller_DW.UnitDelay_DSTATE;
    }

    // End of Switch: '<S19>/Switch'

    // Sum: '<S19>/Sum1' incorporates:
    //   Constant: '<S19>/Constant3'

    rtb_MultiportSwitch_idx_0 -= controller_P.Monostable1_Ts;

    // RelationalOperator: '<S19>/Relational Operator' incorporates:
    //   Constant: '<S19>/Constant2'

    rtb_RelationalOperator = (rtb_MultiportSwitch_idx_0 >
      -controller_P.Monostable1_Ts);

    // Chart: '<S1>/Pulse Generator' incorporates:
    //   Constant: '<S1>/IE_ratio'
    //   Constant: '<S1>/respiratory_rate'

    if (controller_DW.temporalCounter_i1 < MAX_uint32_T) {
      controller_DW.temporalCounter_i1++;
    }

    if (controller_DW.is_active_c3_controller == 0U) {
      controller_DW.is_active_c3_controller = 1U;
      controller_DW.is_c3_controller = controller_IN_UP;
      controller_DW.temporalCounter_i1 = 0U;
      rtb_pulse = true;
    } else if (controller_DW.is_c3_controller == 1) {
      rtb_pulse = false;
      if (controller_DW.temporalCounter_i1 >= 1.0F /
          (controller_P.IE_ratio_Value + 1.0F) * (60.0F /
           controller_P.respiratory_rate_Value) * 100.0F) {
        controller_DW.is_c3_controller = controller_IN_UP;
        controller_DW.temporalCounter_i1 = 0U;
        rtb_pulse = true;
      }
    } else {
      // case IN_UP:
      rtb_pulse = true;
      if (controller_DW.temporalCounter_i1 >= controller_P.IE_ratio_Value /
          (controller_P.IE_ratio_Value + 1.0F) * (60.0F /
           controller_P.respiratory_rate_Value) * 100.0F) {
        controller_DW.is_c3_controller = controller_IN_DOWN;
        controller_DW.temporalCounter_i1 = 0U;
        rtb_pulse = false;
      }
    }

    // End of Chart: '<S1>/Pulse Generator'

    // Switch: '<S1>/Switch4' incorporates:
    //   Constant: '<S1>/Constant8'
    //   Constant: '<S1>/enable_trigger'
    //   Switch: '<S1>/Switch2'

    if (rtb_Compare_k) {
      rtb_Divide = controller_P.Constant8_Value;
    } else if (controller_P.enable_trigger_Value) {
      // Switch: '<S1>/Switch2'
      rtb_Divide = rtb_RelationalOperator;
    } else {
      rtb_Divide = rtb_pulse;
    }

    // End of Switch: '<S1>/Switch4'

    // Switch: '<S1>/Switch3' incorporates:
    //   Constant: '<S1>/Constant'
    //   Constant: '<S3>/Constant'
    //   Logic: '<S1>/Logical Operator3'
    //   RelationalOperator: '<S3>/Compare'
    //   Sum: '<S1>/Sum'

    if ((controller_P.Controller_control_mode ==
         controller_P.CompareToConstant_const) || rtb_Compare_k) {
      rtb_IProdOut_d = controller_P.Constant_Value_a;
    } else {
      rtb_IProdOut_d = controller_P.Constant_Value_a - rtb_Divide;
    }

    // End of Switch: '<S1>/Switch3'

    // Outport: '<Root>/OV_cmd' incorporates:
    //   Constant: '<S7>/Constant'
    //   RelationalOperator: '<S7>/Compare'

    controller_Y.OV_cmd = (rtb_IProdOut_d > controller_P.Constant_Value_p);

    // Outputs for Enabled SubSystem: '<S1>/Pressure Control' incorporates:
    //   EnablePort: '<S11>/Enable'

    // Logic: '<S1>/Logical Operator2' incorporates:
    //   Logic: '<S11>/Logical Operator'

    if (rtb_Compare_k || rtb_Compare_p) {
      if (!controller_DW.PressureControl_MODE) {
        // InitializeConditions for DiscreteFilter: '<S11>/Input Shaping'
        controller_DW.InputShaping_icLoad = 1U;

        // InitializeConditions for UnitDelay: '<S11>/Unit Delay'
        controller_DW.UnitDelay_DSTATE_o =
          controller_P.UnitDelay_InitialCondition_m;

        // InitializeConditions for DiscreteIntegrator: '<S57>/Integrator'
        controller_DW.Integrator_DSTATE_p =
          controller_P.PIDController_InitialConditionF;
        controller_DW.Integrator_PrevResetState_f = 2;
        controller_DW.PressureControl_MODE = true;
      }

      // Gain: '<S11>/PIP'
      rtb_PIP = controller_P.PIP_Gain * rtb_Divide;

      // Gain: '<S23>/Gain' incorporates:
      //   Constant: '<S1>/IE_ratio'
      //   Constant: '<S1>/respiratory_rate'
      //   Constant: '<S23>/Constant 1'
      //   Constant: '<S23>/PIP_rise_perc'
      //   Fcn: '<S23>/fun1'
      //   Product: '<S23>/Product2'

      rtb_TmpSignalConversionAtHSCL_2 = controller_P.PIP_rise_perc_Value *
        controller_P.Constant1_Value_i / controller_P.respiratory_rate_Value *
        (controller_P.IE_ratio_Value / (controller_P.IE_ratio_Value + 1.0F)) *
        controller_P.Gain_Gain;

      // Math: '<S23>/Math Function'
      rtb_IProdOut_d = rtb_TmpSignalConversionAtHSCL_2 *
        rtb_TmpSignalConversionAtHSCL_2;

      // Product: '<S23>/Product1' incorporates:
      //   Constant: '<S23>/Constant'
      //   Constant: '<S23>/Constant1'
      //   Product: '<S23>/Divide'
      //   Product: '<S23>/Product'

      rtb_pnm_idx_0 = controller_P.Constant_Value_e[0] /
        (rtb_TmpSignalConversionAtHSCL_2 * rtb_IProdOut_d) *
        controller_P.Constant1_Value_n[0];
      rtb_pnm_idx_1 = controller_P.Constant_Value_e[1] / rtb_IProdOut_d *
        controller_P.Constant1_Value_n[1];
      rtb_pnm_idx_2 = controller_P.Constant_Value_e[2] /
        rtb_TmpSignalConversionAtHSCL_2 * controller_P.Constant1_Value_n[2];

      // Fcn: '<S23>/a0'
      rtb_Divide5 = ((rtb_pnm_idx_2 + rtb_pnm_idx_1) + rtb_pnm_idx_0) - 8.0F;

      // Product: '<S23>/Divide1' incorporates:
      //   Fcn: '<S23>/b0_3'

      rtb_TmpSignalConversionAtHSCL_2 = rtb_pnm_idx_0 / rtb_Divide5;

      // Product: '<S23>/Divide2' incorporates:
      //   Fcn: '<S23>/b1_2'

      rtb_IProdOut_d = 3.0F * rtb_pnm_idx_0 / rtb_Divide5;

      // Product: '<S23>/Divide3' incorporates:
      //   Fcn: '<S23>/a1'

      rtb_Divide3 = (((-rtb_pnm_idx_2 + rtb_pnm_idx_1) + 3.0F * rtb_pnm_idx_0) +
                     24.0F) / rtb_Divide5;

      // Product: '<S23>/Divide4' incorporates:
      //   Fcn: '<S23>/a2'

      rtb_SumFdbk = (((-rtb_pnm_idx_2 - rtb_pnm_idx_1) + 3.0F * rtb_pnm_idx_0) -
                     24.0F) / rtb_Divide5;

      // Product: '<S23>/Divide5' incorporates:
      //   Fcn: '<S23>/a3'

      rtb_Divide5 = (((rtb_pnm_idx_2 - rtb_pnm_idx_1) + rtb_pnm_idx_0) + 8.0F) /
        rtb_Divide5;

      // SignalConversion generated from: '<S11>/Input Shaping'
      controller_B.TmpSignalConversionAtInputShapi[0] =
        rtb_TmpSignalConversionAtHSCL_2;
      controller_B.TmpSignalConversionAtInputShapi[1] = rtb_IProdOut_d;
      controller_B.TmpSignalConversionAtInputShapi[2] = rtb_IProdOut_d;
      controller_B.TmpSignalConversionAtInputShapi[3] =
        rtb_TmpSignalConversionAtHSCL_2;

      // SignalConversion generated from: '<S11>/Input Shaping' incorporates:
      //   Constant: '<S23>/Constant2'

      controller_B.TmpSignalConversionAtInputSha_h[0] =
        controller_P.Constant2_Value;
      controller_B.TmpSignalConversionAtInputSha_h[1] = rtb_Divide3;
      controller_B.TmpSignalConversionAtInputSha_h[2] = rtb_SumFdbk;
      controller_B.TmpSignalConversionAtInputSha_h[3] = rtb_Divide5;

      // Product: '<S11>/Product' incorporates:
      //   Fcn: '<S23>/fun'
      //   Gain: '<S11>/Gain3'
      //   Gain: '<S11>/PIP_step_perc'
      //   MATLABSystem: '<S1>/HSC LP'
      //   Sum: '<S11>/Sum3'
      //   Sum: '<S11>/Sum4'

      rtb_Divide5 = (rtb_Divide3 - rtb_TmpSignalConversionAtHSCL_2) /
        (((rtb_Divide3 + rtb_SumFdbk) + rtb_Divide5) * rtb_IProdOut_d + (2.0F *
          rtb_IProdOut_d + rtb_TmpSignalConversionAtHSCL_2) * rtb_Divide3) *
        ((rtb_PIP - rtb_TmpSignalConversionAtHSCL_1) *
         controller_P.PIP_step_perc_Gain * controller_P.Gain3_Gain +
         rtb_TmpSignalConversionAtHSCL_1);

      // DiscreteFilter: '<S11>/Input Shaping'
      if (rt_R32ZCFcn(RISING_ZERO_CROSSING,
                      &controller_PrevZCX.InputShaping_Reset_ZCE,
                      (rtb_Divide)) != NO_ZCEVENT) {
        controller_DW.InputShaping_icLoad = 1U;
      }

      if (controller_DW.InputShaping_icLoad != 0) {
        controller_DW.InputShaping_states[0] = rtb_Divide5;
        controller_DW.InputShaping_states[1] = rtb_Divide5;
        controller_DW.InputShaping_states[2] = rtb_Divide5;
        controller_DW.InputShaping_icLoad = 0U;
      }

      controller_DW.InputShaping_tmp = ((rtb_PIP -
        controller_B.TmpSignalConversionAtInputSha_h[1] *
        controller_DW.InputShaping_states[0]) -
        controller_B.TmpSignalConversionAtInputSha_h[2] *
        controller_DW.InputShaping_states[1]) -
        controller_B.TmpSignalConversionAtInputSha_h[3] *
        controller_DW.InputShaping_states[2];
      rtb_IProdOut_d = ((controller_B.TmpSignalConversionAtInputShapi[0] *
                         controller_DW.InputShaping_tmp +
                         controller_B.TmpSignalConversionAtInputShapi[1] *
                         controller_DW.InputShaping_states[0]) +
                        controller_B.TmpSignalConversionAtInputShapi[2] *
                        controller_DW.InputShaping_states[1]) +
        controller_B.TmpSignalConversionAtInputShapi[3] *
        controller_DW.InputShaping_states[2];

      // End of DiscreteFilter: '<S11>/Input Shaping'

      // Sum: '<S11>/Sum1' incorporates:
      //   MATLABSystem: '<S1>/HSC LP'

      rtb_Divide5 = rtb_IProdOut_d - rtb_TmpSignalConversionAtHSCL_1;

      // Outputs for Enabled SubSystem: '<S11>/Params Selector' incorporates:
      //   EnablePort: '<S25>/Enable'

      if (!(rtb_Divide != 0.0F)) {
        // Switch: '<S25>/Switch2' incorporates:
        //   Constant: '<S25>/PC_FFW'
        //   Constant: '<S25>/PC_FFW_rec'
        //   Constant: '<S25>/PC_I'
        //   Constant: '<S25>/PC_I_rec'
        //   Constant: '<S25>/PC_P'
        //   Constant: '<S25>/PC_P_rec'
        //   MATLABSystem: '<S1>/HSC LP'

        if (rtb_TmpSignalConversionAtHSCL_0 > controller_P.Switch2_Threshold) {
          controller_B.Switch2[0] = controller_P.PC_P_rec_Value;
          controller_B.Switch2[1] = controller_P.PC_I_rec_Value;
          controller_B.Switch2[2] = controller_P.PC_FFW_rec_Value;
        } else {
          controller_B.Switch2[0] = controller_P.PC_P_Value;
          controller_B.Switch2[1] = controller_P.PC_I_Value;
          controller_B.Switch2[2] = controller_P.PC_FFW_Value;
        }

        // End of Switch: '<S25>/Switch2'

        // SignalConversion generated from: '<S25>/FFW'
        controller_B.OutportBufferForFFW = controller_B.Switch2[2];

        // SignalConversion generated from: '<S25>/I'
        controller_B.OutportBufferForI = controller_B.Switch2[1];
      }

      // End of Outputs for SubSystem: '<S11>/Params Selector'

      // Product: '<S54>/IProd Out' incorporates:
      //   Logic: '<S11>/Logical Operator'

      rtb_PIP = rtb_Divide5 * controller_B.OutportBufferForI;

      // Product: '<S62>/PProd Out'
      rtb_Divide5 *= controller_B.Switch2[0];

      // Sum: '<S69>/SumI1' incorporates:
      //   Gain: '<S68>/Kt'
      //   Sum: '<S11>/Sum5'
      //   Sum: '<S67>/Sum Fdbk'
      //   Sum: '<S68>/SumI3'
      //   UnitDelay: '<S11>/Unit Delay'

      rtb_PIP += ((controller_DW.UnitDelay_DSTATE_o -
                   controller_B.OutportBufferForFFW) - (rtb_Divide5 +
        controller_DW.Integrator_DSTATE_p)) * controller_P.PIDController_Kt;

      // DiscreteIntegrator: '<S57>/Integrator'
      if ((rtb_Divide > 0.0F) && (controller_DW.Integrator_PrevResetState_f <= 0))
      {
        controller_DW.Integrator_DSTATE_p =
          controller_P.PIDController_InitialConditionF;
      }

      controller_B.Integrator_m = controller_P.Integrator_gainval * rtb_PIP +
        controller_DW.Integrator_DSTATE_p;

      // End of DiscreteIntegrator: '<S57>/Integrator'

      // Sum: '<S11>/Sum2' incorporates:
      //   Sum: '<S66>/Sum'

      rtb_Divide5 = (rtb_Divide5 + controller_B.Integrator_m) +
        controller_B.OutportBufferForFFW;

      // Saturate: '<S11>/Saturation'
      if (rtb_Divide5 > controller_P.Saturation_UpperSat) {
        rtb_Divide5 = controller_P.Saturation_UpperSat;
      } else {
        if (rtb_Divide5 < controller_P.Saturation_LowerSat) {
          rtb_Divide5 = controller_P.Saturation_LowerSat;
        }
      }

      // End of Saturate: '<S11>/Saturation'

      // Switch: '<S11>/Switch' incorporates:
      //   Constant: '<S11>/Constant'

      if (rtb_Divide > controller_P.Switch_Threshold) {
        controller_B.Switch_n = rtb_Divide5;
      } else {
        controller_B.Switch_n = controller_P.Constant_Value;
      }

      // End of Switch: '<S11>/Switch'

      // Switch: '<S11>/Switch1' incorporates:
      //   Constant: '<S11>/Constant'

      if (rtb_Divide > controller_P.Switch1_Threshold) {
        controller_B.Switch1 = rtb_IProdOut_d;
      } else {
        controller_B.Switch1 = controller_P.Constant_Value;
      }

      // End of Switch: '<S11>/Switch1'
    } else {
      if (controller_DW.PressureControl_MODE) {
        // Disable for Outport: '<S11>/P_ref'
        controller_B.Switch1 = controller_P.P_ref_Y0;

        // Disable for Outport: '<S11>/IV_dc'
        controller_B.Switch_n = controller_P.IV_dc_Y0;
        controller_DW.PressureControl_MODE = false;
      }
    }

    // End of Logic: '<S1>/Logical Operator2'
    // End of Outputs for SubSystem: '<S1>/Pressure Control'

    // Outputs for Enabled SubSystem: '<S1>/Volume Control' incorporates:
    //   EnablePort: '<S17>/Enable'

    if (rtb_Compare) {
      if (!controller_DW.VolumeControl_MODE) {
        // InitializeConditions for UnitDelay: '<S17>/Unit Delay'
        controller_DW.UnitDelay_DSTATE_n =
          controller_P.UnitDelay_InitialCondition_n;

        // InitializeConditions for DiscreteIntegrator: '<S109>/Integrator'
        controller_DW.Integrator_DSTATE =
          controller_P.PIDController_InitialConditio_c;
        controller_DW.Integrator_PrevResetState = 2;
        controller_DW.VolumeControl_MODE = true;
      }

      // Gain: '<S17>/Volumetric Flow rate' incorporates:
      //   Gain: '<S1>/Tidal Volume'

      controller_B.VolumetricFlowrate = controller_P.TidalVolume_Gain *
        rtb_Divide * controller_P.VolumetricFlowrate_Gain;

      // Outputs for Triggered SubSystem: '<S17>/Store FFW' incorporates:
      //   TriggerPort: '<S77>/Trigger'

      zcEvent = rt_R32ZCFcn(FALLING_ZERO_CROSSING,
                            &controller_PrevZCX.StoreFFW_Trig_ZCE,
                            (controller_B.VolumetricFlowrate));
      if (zcEvent != NO_ZCEVENT) {
        // DataStoreWrite: '<S77>/Data Store Write' incorporates:
        //   UnitDelay: '<S17>/Unit Delay'

        controller_DW.FFW = controller_DW.UnitDelay_DSTATE_n;
      }

      // End of Outputs for SubSystem: '<S17>/Store FFW'

      // Sum: '<S17>/Sum1'
      rtb_TmpSignalConversionAtHSCL_2 = controller_B.VolumetricFlowrate -
        rtb_TmpSignalConversionAtHSCL_3;

      // Switch: '<S75>/Switch2' incorporates:
      //   Constant: '<S75>/VC_I'
      //   Constant: '<S75>/VC_I_rec'
      //   Constant: '<S75>/VC_P'
      //   Constant: '<S75>/VC_P_rec'
      //   MATLABSystem: '<S1>/HSC LP'

      if (rtb_TmpSignalConversionAtHSCL_0 > controller_P.Switch2_Threshold_b) {
        rtb_TmpSignalConversionAtHSCL_3 = controller_P.VC_P_rec_Value;
        rtb_TmpSignalConversionAtHSCL_1 = controller_P.VC_I_rec_Value;
      } else {
        rtb_TmpSignalConversionAtHSCL_3 = controller_P.VC_P_Value;
        rtb_TmpSignalConversionAtHSCL_1 = controller_P.VC_I_Value;
      }

      // End of Switch: '<S75>/Switch2'

      // Product: '<S106>/IProd Out'
      rtb_IProdOut_d = rtb_TmpSignalConversionAtHSCL_2 *
        rtb_TmpSignalConversionAtHSCL_1;

      // Product: '<S114>/PProd Out'
      rtb_TmpSignalConversionAtHSCL_2 *= rtb_TmpSignalConversionAtHSCL_3;

      // Outputs for Enabled SubSystem: '<S17>/Read FFW' incorporates:
      //   EnablePort: '<S76>/Enable'

      if (controller_B.VolumetricFlowrate > 0.0F) {
        controller_DW.ReadFFW_MODE = true;

        // DataStoreRead: '<S76>/Data Store Read'
        controller_B.DataStoreRead = controller_DW.FFW;
      } else {
        if (controller_DW.ReadFFW_MODE) {
          // Disable for Outport: '<S76>/ffw'
          controller_B.DataStoreRead = controller_P.ffw_Y0;
          controller_DW.ReadFFW_MODE = false;
        }
      }

      // End of Outputs for SubSystem: '<S17>/Read FFW'

      // Sum: '<S121>/SumI1' incorporates:
      //   Gain: '<S120>/Kt'
      //   Sum: '<S119>/Sum Fdbk'
      //   Sum: '<S120>/SumI3'
      //   Sum: '<S17>/Sum3'
      //   UnitDelay: '<S17>/Unit Delay'

      rtb_IProdOut_d += ((controller_DW.UnitDelay_DSTATE_n -
                          controller_B.DataStoreRead) -
                         (rtb_TmpSignalConversionAtHSCL_2 +
                          controller_DW.Integrator_DSTATE)) *
        controller_P.PIDController_Kt_j;

      // DiscreteIntegrator: '<S109>/Integrator'
      if ((controller_B.VolumetricFlowrate <= 0.0F) &&
          (controller_DW.Integrator_PrevResetState == 1)) {
        controller_DW.Integrator_DSTATE =
          controller_P.PIDController_InitialConditio_c;
      }

      controller_B.Integrator = controller_P.Integrator_gainval_m *
        rtb_IProdOut_d + controller_DW.Integrator_DSTATE;

      // End of DiscreteIntegrator: '<S109>/Integrator'

      // Sum: '<S17>/Sum2' incorporates:
      //   Sum: '<S118>/Sum'

      rtb_TmpSignalConversionAtHSCL_1 = (rtb_TmpSignalConversionAtHSCL_2 +
        controller_B.Integrator) + controller_B.DataStoreRead;

      // Saturate: '<S17>/Saturation'
      if (rtb_TmpSignalConversionAtHSCL_1 > controller_P.Saturation_UpperSat_b)
      {
        controller_B.Saturation = controller_P.Saturation_UpperSat_b;
      } else if (rtb_TmpSignalConversionAtHSCL_1 <
                 controller_P.Saturation_LowerSat_f) {
        controller_B.Saturation = controller_P.Saturation_LowerSat_f;
      } else {
        controller_B.Saturation = rtb_TmpSignalConversionAtHSCL_1;
      }

      // End of Saturate: '<S17>/Saturation'

      // Update for UnitDelay: '<S17>/Unit Delay'
      controller_DW.UnitDelay_DSTATE_n = controller_B.Saturation;

      // Update for DiscreteIntegrator: '<S109>/Integrator'
      controller_DW.Integrator_DSTATE = controller_P.Integrator_gainval_m *
        rtb_IProdOut_d + controller_B.Integrator;
      if (controller_B.VolumetricFlowrate > 0.0F) {
        controller_DW.Integrator_PrevResetState = 1;
      } else if (controller_B.VolumetricFlowrate < 0.0F) {
        controller_DW.Integrator_PrevResetState = -1;
      } else if (controller_B.VolumetricFlowrate == 0.0F) {
        controller_DW.Integrator_PrevResetState = 0;
      } else {
        controller_DW.Integrator_PrevResetState = 2;
      }

      // End of Update for DiscreteIntegrator: '<S109>/Integrator'

      // Switch: '<S1>/Switch' incorporates:
      //   Outport: '<Root>/IV_dc'

      controller_Y.IV_dc = controller_B.Saturation;
    } else {
      if (controller_DW.VolumeControl_MODE) {
        // Disable for Enabled SubSystem: '<S17>/Read FFW'
        if (controller_DW.ReadFFW_MODE) {
          // Disable for Outport: '<S76>/ffw'
          controller_B.DataStoreRead = controller_P.ffw_Y0;
          controller_DW.ReadFFW_MODE = false;
        }

        // End of Disable for SubSystem: '<S17>/Read FFW'

        // Disable for Outport: '<S17>/Q_ref'
        controller_B.VolumetricFlowrate = controller_P.Q_ref_Y0;

        // Disable for Outport: '<S17>/IV_dc'
        controller_B.Saturation = controller_P.IV_dc_Y0_m;
        controller_DW.VolumeControl_MODE = false;
      }

      // Outport: '<Root>/IV_dc' incorporates:
      //   Switch: '<S1>/Switch'

      controller_Y.IV_dc = controller_B.Switch_n;
    }

    // End of Outputs for SubSystem: '<S1>/Volume Control'

    // Outport: '<Root>/Q_ref' incorporates:
    //   SignalConversion generated from: '<S1>/Q_ref'

    controller_Y.Q_ref = controller_B.VolumetricFlowrate;

    // Outport: '<Root>/P_ref' incorporates:
    //   SignalConversion generated from: '<S1>/P_ref'

    controller_Y.P_ref = controller_B.Switch1;

    // Update for UnitDelay: '<S1>/Unit Delay1' incorporates:
    //   Inport: '<Root>/S1_Pi'
    //   Inport: '<Root>/S3_i'
    //   Inport: '<Root>/S4_Pi'
    //   Inport: '<Root>/S7_Pi'

    controller_DW.UnitDelay1_DSTATE[0] = controller_U.S1_Pi;
    controller_DW.UnitDelay1_DSTATE[1] = controller_U.S3_i[0];
    controller_DW.UnitDelay1_DSTATE[2] = controller_U.S4_Pi;
    controller_DW.UnitDelay1_DSTATE[3] = controller_U.S7_Pi;

    // Update for UnitDelay: '<S1>/Unit Delay2' incorporates:
    //   Inport: '<Root>/S3_i'

    controller_DW.UnitDelay2_DSTATE = controller_U.S3_i[1];

    // Update for UnitDelay: '<S1>/Unit Delay' incorporates:
    //   Inport: '<Root>/S2_Qi'
    //   Inport: '<Root>/S5_Qi'

    controller_DW.UnitDelay_DSTATE_m[0] = controller_U.S2_Qi;
    controller_DW.UnitDelay_DSTATE_m[1] = controller_U.S5_Qi;

    // Update for UnitDelay: '<S1>/Unit Delay3'
    controller_DW.UnitDelay3_DSTATE = rtb_Divide;

    // Update for DiscreteIntegrator: '<S1>/Discrete-Time Integrator'
    controller_DW.DiscreteTimeIntegrator_DSTATE = rtb_psi_cmH2O +
      controller_B.DiscreteTimeIntegrator;
    controller_DW.DiscreteTimeIntegrator_PrevRese = static_cast<int8_T>
      (rtb_LogicalOperator1);

    // Update for UnitDelay: '<S20>/Unit Delay' incorporates:
    //   DataTypeConversion: '<S19>/Data Type Conversion2'

    controller_DW.UnitDelay_DSTATE_d = (controller_B.DataTypeConversion != 0.0F);

    // Update for UnitDelay: '<S19>/delay'
    controller_DW.delay_DSTATE = rtb_RelationalOperator;

    // Update for UnitDelay: '<S19>/Unit Delay'
    controller_DW.UnitDelay_DSTATE = rtb_MultiportSwitch_idx_0;

    // Update for Enabled SubSystem: '<S1>/Pressure Control' incorporates:
    //   EnablePort: '<S11>/Enable'

    if (controller_DW.PressureControl_MODE) {
      // Update for DiscreteFilter: '<S11>/Input Shaping'
      controller_DW.InputShaping_icLoad = 0U;
      controller_DW.InputShaping_states[2] = controller_DW.InputShaping_states[1];
      controller_DW.InputShaping_states[1] = controller_DW.InputShaping_states[0];
      controller_DW.InputShaping_states[0] = controller_DW.InputShaping_tmp;

      // Update for UnitDelay: '<S11>/Unit Delay'
      controller_DW.UnitDelay_DSTATE_o = rtb_Divide5;

      // Update for DiscreteIntegrator: '<S57>/Integrator'
      controller_DW.Integrator_DSTATE_p = controller_P.Integrator_gainval *
        rtb_PIP + controller_B.Integrator_m;
      if (rtb_Divide > 0.0F) {
        controller_DW.Integrator_PrevResetState_f = 1;
      } else if (rtb_Divide < 0.0F) {
        controller_DW.Integrator_PrevResetState_f = -1;
      } else if (rtb_Divide == 0.0F) {
        controller_DW.Integrator_PrevResetState_f = 0;
      } else {
        controller_DW.Integrator_PrevResetState_f = 2;
      }

      // End of Update for DiscreteIntegrator: '<S57>/Integrator'
    }

    // End of Update for SubSystem: '<S1>/Pressure Control'
  } else {
    if (controller_DW.Controller_MODE) {
      // Disable for Enabled SubSystem: '<S1>/Compute Trigger'
      if (controller_DW.ComputeTrigger_MODE) {
        // Disable for Outport: '<S8>/Out'
        controller_B.DataTypeConversion = controller_P.Out_Y0;
        controller_DW.ComputeTrigger_MODE = false;
      }

      // End of Disable for SubSystem: '<S1>/Compute Trigger'

      // Disable for Enabled SubSystem: '<S1>/Pressure Control'
      if (controller_DW.PressureControl_MODE) {
        // Disable for Outport: '<S11>/P_ref'
        controller_B.Switch1 = controller_P.P_ref_Y0;

        // Disable for Outport: '<S11>/IV_dc'
        controller_B.Switch_n = controller_P.IV_dc_Y0;
        controller_DW.PressureControl_MODE = false;
      }

      // End of Disable for SubSystem: '<S1>/Pressure Control'

      // Disable for Enabled SubSystem: '<S1>/Volume Control'
      if (controller_DW.VolumeControl_MODE) {
        // Disable for Enabled SubSystem: '<S17>/Read FFW'
        if (controller_DW.ReadFFW_MODE) {
          // Disable for Outport: '<S76>/ffw'
          controller_B.DataStoreRead = controller_P.ffw_Y0;
          controller_DW.ReadFFW_MODE = false;
        }

        // End of Disable for SubSystem: '<S17>/Read FFW'

        // Disable for Outport: '<S17>/Q_ref'
        controller_B.VolumetricFlowrate = controller_P.Q_ref_Y0;

        // Disable for Outport: '<S17>/IV_dc'
        controller_B.Saturation = controller_P.IV_dc_Y0_m;
        controller_DW.VolumeControl_MODE = false;
      }

      // End of Disable for SubSystem: '<S1>/Volume Control'

      // Disable for Outport: '<Root>/IV_dc' incorporates:
      //   Outport: '<S1>/IV_dc'

      controller_Y.IV_dc = controller_P.IV_dc_Y0_o;

      // Disable for Outport: '<Root>/Q_ref' incorporates:
      //   Outport: '<S1>/Q_ref'

      controller_Y.Q_ref = controller_P.Q_ref_Y0_n;

      // Disable for Outport: '<Root>/P_ref' incorporates:
      //   Outport: '<S1>/P_ref'

      controller_Y.P_ref = controller_P.P_ref_Y0_f;

      // Disable for Outport: '<Root>/OV_cmd' incorporates:
      //   Outport: '<S1>/OV_cmd'

      controller_Y.OV_cmd = controller_P.OV_cmd_Y0;
      for (i = 0; i < 9; i++) {
        // Disable for Outport: '<Root>/signals' incorporates:
        //   Outport: '<S1>/signals'

        controller_Y.signals[i] = controller_P.signals_Y0;
      }

      controller_DW.Controller_MODE = false;
    }
  }

  // End of Inport: '<Root>/enable'
  // End of Outputs for SubSystem: '<Root>/Controller'
}

// Model initialize function
void controllerModelClass::initialize()
{
  {
    b_dspcodegen_BiquadFilter_con_T *iobj_0;
    int32_T i;
    static const real32_T tmp[6] = { 0.312298179F, 0.407353371F, 0.312298179F,
      0.68379283F, 0.68379283F, 0.0F };

    controller_PrevZCX.InputShaping_Reset_ZCE = UNINITIALIZED_ZCSIG;
    controller_PrevZCX.StoreFFW_Trig_ZCE = UNINITIALIZED_ZCSIG;

    // SystemInitialize for Enabled SubSystem: '<Root>/Controller'
    // InitializeConditions for UnitDelay: '<S1>/Unit Delay1'
    controller_DW.UnitDelay1_DSTATE[0] =
      controller_P.UnitDelay1_InitialCondition[0];
    controller_DW.UnitDelay1_DSTATE[1] =
      controller_P.UnitDelay1_InitialCondition[1];
    controller_DW.UnitDelay1_DSTATE[2] =
      controller_P.UnitDelay1_InitialCondition[2];
    controller_DW.UnitDelay1_DSTATE[3] =
      controller_P.UnitDelay1_InitialCondition[3];

    // InitializeConditions for UnitDelay: '<S1>/Unit Delay2'
    controller_DW.UnitDelay2_DSTATE = controller_P.UnitDelay2_InitialCondition;

    // InitializeConditions for UnitDelay: '<S1>/Unit Delay'
    controller_DW.UnitDelay_DSTATE_m[0] =
      controller_P.UnitDelay_InitialCondition_g;
    controller_DW.UnitDelay_DSTATE_m[1] =
      controller_P.UnitDelay_InitialCondition_g;

    // InitializeConditions for UnitDelay: '<S1>/Unit Delay3'
    controller_DW.UnitDelay3_DSTATE = controller_P.UnitDelay3_InitialCondition;

    // InitializeConditions for DiscreteIntegrator: '<S1>/Discrete-Time Integrator' 
    controller_DW.DiscreteTimeIntegrator_DSTATE =
      controller_P.DiscreteTimeIntegrator_IC;
    controller_DW.DiscreteTimeIntegrator_PrevRese = 2;

    // InitializeConditions for UnitDelay: '<S20>/Unit Delay'
    controller_DW.UnitDelay_DSTATE_d = controller_P.Monostable1_ic;

    // InitializeConditions for UnitDelay: '<S19>/delay'
    controller_DW.delay_DSTATE = controller_P.delay_InitialCondition;

    // InitializeConditions for UnitDelay: '<S19>/Unit Delay'
    controller_DW.UnitDelay_DSTATE = controller_P.UnitDelay_InitialCondition;

    // SystemInitialize for Enabled SubSystem: '<S1>/Compute Trigger'
    // SystemInitialize for Outport: '<S8>/Out'
    controller_B.DataTypeConversion = controller_P.Out_Y0;

    // End of SystemInitialize for SubSystem: '<S1>/Compute Trigger'

    // SystemInitialize for Enabled SubSystem: '<S20>/POSITIVE Edge'
    // SystemInitialize for Outport: '<S22>/OUT'
    controller_B.RelationalOperator1 = controller_P.OUT_Y0_h;

    // End of SystemInitialize for SubSystem: '<S20>/POSITIVE Edge'

    // SystemInitialize for Enabled SubSystem: '<S20>/NEGATIVE Edge'
    // SystemInitialize for Outport: '<S21>/OUT'
    controller_B.RelationalOperator1_h = controller_P.OUT_Y0;

    // End of SystemInitialize for SubSystem: '<S20>/NEGATIVE Edge'

    // SystemInitialize for Enabled SubSystem: '<S1>/Pressure Control'
    // InitializeConditions for DiscreteFilter: '<S11>/Input Shaping'
    controller_DW.InputShaping_icLoad = 1U;

    // InitializeConditions for UnitDelay: '<S11>/Unit Delay'
    controller_DW.UnitDelay_DSTATE_o = controller_P.UnitDelay_InitialCondition_m;

    // InitializeConditions for DiscreteIntegrator: '<S57>/Integrator'
    controller_DW.Integrator_DSTATE_p =
      controller_P.PIDController_InitialConditionF;
    controller_DW.Integrator_PrevResetState_f = 2;

    // SystemInitialize for Enabled SubSystem: '<S11>/Params Selector'
    // SystemInitialize for Outport: '<S25>/P'
    controller_B.Switch2[0] = controller_P.P_Y0;

    // SystemInitialize for Outport: '<S25>/I'
    controller_B.OutportBufferForI = controller_P.I_Y0;

    // SystemInitialize for Outport: '<S25>/FFW'
    controller_B.OutportBufferForFFW = controller_P.FFW_Y0;

    // End of SystemInitialize for SubSystem: '<S11>/Params Selector'

    // SystemInitialize for Outport: '<S11>/P_ref'
    controller_B.Switch1 = controller_P.P_ref_Y0;

    // SystemInitialize for Outport: '<S11>/IV_dc'
    controller_B.Switch_n = controller_P.IV_dc_Y0;

    // End of SystemInitialize for SubSystem: '<S1>/Pressure Control'

    // SystemInitialize for Enabled SubSystem: '<S1>/Volume Control'
    // Start for DataStoreMemory: '<S17>/Data Store Memory'
    controller_DW.FFW = controller_P.DataStoreMemory_InitialValue;

    // InitializeConditions for UnitDelay: '<S17>/Unit Delay'
    controller_DW.UnitDelay_DSTATE_n = controller_P.UnitDelay_InitialCondition_n;

    // InitializeConditions for DiscreteIntegrator: '<S109>/Integrator'
    controller_DW.Integrator_DSTATE =
      controller_P.PIDController_InitialConditio_c;
    controller_DW.Integrator_PrevResetState = 2;

    // SystemInitialize for Enabled SubSystem: '<S17>/Read FFW'
    // SystemInitialize for Outport: '<S76>/ffw'
    controller_B.DataStoreRead = controller_P.ffw_Y0;

    // End of SystemInitialize for SubSystem: '<S17>/Read FFW'

    // SystemInitialize for Outport: '<S17>/Q_ref'
    controller_B.VolumetricFlowrate = controller_P.Q_ref_Y0;

    // SystemInitialize for Outport: '<S17>/IV_dc'
    controller_B.Saturation = controller_P.IV_dc_Y0_m;

    // End of SystemInitialize for SubSystem: '<S1>/Volume Control'

    // Start for MATLABSystem: '<S1>/HSC LP'
    controller_DW.gobj_1.matlabCodegenIsDeleted = true;
    controller_DW.gobj_2.matlabCodegenIsDeleted = true;
    iobj_0 = &controller_DW.gobj_2;
    controller_DW.obj.TunablePropsChanged = false;
    controller_DW.obj.matlabCodegenIsDeleted = false;
    controller_DW.objisempty = true;
    controller_DW.obj.isInitialized = 1;
    controller_DW.gobj_2.isInitialized = 0;

    // System object Constructor function: dsp.BiquadFilter
    iobj_0->cSFunObject.P0_ICRTP = 0.0F;
    for (i = 0; i < 6; i++) {
      iobj_0->cSFunObject.P1_RTP1COEFF[i] = tmp[i];
    }

    iobj_0->cSFunObject.P2_RTP2COEFF[0] = 0.111092761F;
    iobj_0->cSFunObject.P2_RTP2COEFF[1] = 0.604452431F;
    iobj_0->cSFunObject.P2_RTP2COEFF[2] = -0.177357942F;
    iobj_0->cSFunObject.P2_RTP2COEFF[3] = 0.0F;
    iobj_0->cSFunObject.P3_RTP3COEFF[0] = 0.0F;
    iobj_0->cSFunObject.P3_RTP3COEFF[1] = 0.0F;
    iobj_0->cSFunObject.P3_RTP3COEFF[2] = 0.0F;
    iobj_0->cSFunObject.P4_RTP_COEFF3_BOOL[0] = false;
    iobj_0->cSFunObject.P4_RTP_COEFF3_BOOL[1] = false;
    iobj_0->cSFunObject.P4_RTP_COEFF3_BOOL[2] = false;
    iobj_0->cSFunObject.P5_IC2RTP = 0.0F;
    controller_DW.gobj_2.matlabCodegenIsDeleted = false;
    controller_DW.obj.FilterObj = &controller_DW.gobj_2;
    controller_DW.obj.NumChannels = 4;
    controller_DW.obj.isSetupComplete = true;

    // End of Start for MATLABSystem: '<S1>/HSC LP'

    // InitializeConditions for MATLABSystem: '<S1>/HSC LP'
    if (controller_DW.obj.FilterObj->isInitialized == 1) {
      // System object Initialization function: dsp.BiquadFilter
      for (i = 0; i < 16; i++) {
        controller_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES[i] =
          controller_DW.obj.FilterObj->cSFunObject.P0_ICRTP;
      }

      for (i = 0; i < 16; i++) {
        controller_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES[i] =
          controller_DW.obj.FilterObj->cSFunObject.P5_IC2RTP;
      }
    }

    // End of InitializeConditions for MATLABSystem: '<S1>/HSC LP'

    // SystemInitialize for Outport: '<Root>/IV_dc' incorporates:
    //   Outport: '<S1>/IV_dc'

    controller_Y.IV_dc = controller_P.IV_dc_Y0_o;

    // SystemInitialize for Outport: '<Root>/Q_ref' incorporates:
    //   Outport: '<S1>/Q_ref'

    controller_Y.Q_ref = controller_P.Q_ref_Y0_n;

    // SystemInitialize for Outport: '<Root>/P_ref' incorporates:
    //   Outport: '<S1>/P_ref'

    controller_Y.P_ref = controller_P.P_ref_Y0_f;

    // SystemInitialize for Outport: '<Root>/OV_cmd' incorporates:
    //   Outport: '<S1>/OV_cmd'

    controller_Y.OV_cmd = controller_P.OV_cmd_Y0;
    for (i = 0; i < 9; i++) {
      // SystemInitialize for Outport: '<Root>/signals' incorporates:
      //   Outport: '<S1>/signals'

      controller_Y.signals[i] = controller_P.signals_Y0;
    }

    // End of SystemInitialize for SubSystem: '<Root>/Controller'
  }
}

// Model terminate function
void controllerModelClass::terminate()
{
  // Terminate for Enabled SubSystem: '<Root>/Controller'
  // Terminate for MATLABSystem: '<S1>/HSC LP'
  matlabCodegenHandle_matlabCod_j(&controller_DW.obj);
  matlabCodegenHandle_matlabCodeg(&controller_DW.gobj_2);
  matlabCodegenHandle_matlabCodeg(&controller_DW.gobj_1);

  // End of Terminate for SubSystem: '<Root>/Controller'
}

// Constructor
controllerModelClass::controllerModelClass():
  controller_B()
  ,controller_DW()
  ,controller_PrevZCX()
  ,controller_U()
  ,controller_Y()
  ,controller_M()
{
  // Currently there is no constructor body generated.
}

// Destructor
controllerModelClass::~controllerModelClass()
{
  // Currently there is no destructor body generated.
}

// Block parameters get method
const controllerModelClass::P_controller_T & controllerModelClass::
  getBlockParameters() const
{
  return controller_P;
}

// Block parameters set method
void controllerModelClass::setBlockParameters(const P_controller_T
  *pcontroller_P)
{
  controller_P = *pcontroller_P;
}

// Real-Time Model get method
controllerModelClass::RT_MODEL_controller_T * controllerModelClass::getRTM()
{
  return (&controller_M);
}

//
// File trailer for generated code.
//
// [EOF]
//
