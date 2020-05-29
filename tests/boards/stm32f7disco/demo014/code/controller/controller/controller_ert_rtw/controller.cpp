//
// Non-Degree Granting Education License -- for use at non-degree
// granting, nonprofit, educational organizations only. Not for
// government, commercial, or other organizational use.
//
// File: controller.cpp
//
// Code generated for Simulink model 'controller'.
//
// Model version                  : 1.524
// Simulink Coder version         : 9.3 (R2020a) 18-Nov-2019
// C/C++ source code generated on : Sun May 24 20:41:38 2020
//
// Target selection: ert.tlc
// Embedded hardware selection: Intel->x86-64 (Windows64)
// Code generation objectives: Unspecified
// Validation result: Not run
//
#include "controller.h"
#include "controller_private.h"

// Named constants for Chart: '<S1>/Pulse Generator'
const uint8_T controll_IN_TRIGGER_INSPIRATION = 3U;
const uint8_T controller_IN_EXPIRATION = 1U;
const uint8_T controller_IN_INSPIRATION = 2U;
const uint8_T controller_IN_TRIGGER_WAIT = 4U;

// Named constants for Chart: '<S1>/TD Max'
const uint8_T controller_IN_COMPUTE_MAX = 1U;
const uint8_T controller_IN_IDLE = 2U;
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
  real32_T rtb_IProdOut_d;
  real32_T rtb_SumFdbk_i;
  real32_T rtb_PIP;
  ZCEventType zcEvent;
  boolean_T rtb_Compare_k;
  boolean_T rtb_Compare_m;
  boolean_T rtb_Compare_f;
  real32_T rtb_Divide;
  int32_T i;
  real32_T rtb_TmpSignalConversionAtHSCL_0;
  real32_T rtb_pnm_idx_0;
  real32_T rtb_pnm_idx_1;
  real32_T rtb_pnm_idx_2;
  real32_T tmp;
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
      controller_DW.UnitDelay_DSTATE[0] =
        controller_P.UnitDelay_InitialCondition_g;
      controller_DW.UnitDelay_DSTATE[1] =
        controller_P.UnitDelay_InitialCondition_g;

      // InitializeConditions for UnitDelay: '<S1>/Unit Delay3'
      controller_DW.UnitDelay3_DSTATE = controller_P.UnitDelay3_InitialCondition;

      // InitializeConditions for DiscreteIntegrator: '<S1>/Discrete-Time Integrator' 
      controller_DW.DiscreteTimeIntegrator_DSTATE =
        controller_P.DiscreteTimeIntegrator_IC;
      controller_DW.DiscreteTimeIntegrator_PrevRese = 2;

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
      controller_DW.doneDoubleBufferReInit = false;
      controller_DW.elapsedTicks_i = 0U;

      // Chart: '<S1>/Pulse Generator'
      controller_DW.is_c3_controller = controller_IN_INSPIRATION;
      controller_DW.temporalCounter_i1_c = 0U;
      controller_B.pulse = 1.0F;

      // SystemReset for Chart: '<S1>/TD Max'
      controller_DW.temporalCounter_i1 = 0U;
      controller_DW.elapsedTicks = 0U;

      // Chart: '<S1>/TD Max'
      controller_DW.is_c1_controller = controller_IN_IDLE;
      controller_B.TD_max = (rtNaNF);

      // Enable for Chart: '<S1>/Pulse Generator'
      controller_DW.presentTicks_j = (&controller_M)->Timing.clockTick0;
      controller_DW.previousTicks_j = controller_DW.presentTicks_j;

      // Enable for Chart: '<S1>/TD Max'
      controller_DW.presentTicks = (&controller_M)->Timing.clockTick0;
      controller_DW.previousTicks = controller_DW.presentTicks;
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
    rtb_SumFdbk_i = rtb_Divide -
      controller_DW.obj.FilterObj->cSFunObject.P2_RTP2COEFF[0] *
      controller_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES[0];
    rtb_SumFdbk_i -= controller_DW.obj.FilterObj->cSFunObject.P2_RTP2COEFF[1] *
      controller_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES[1];
    controller_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES[1] =
      controller_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES[0];
    controller_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES[0] =
      rtb_TmpSignalConversionAtHSCL_1;
    controller_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES[1] =
      controller_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES[0];
    controller_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES[0] = rtb_SumFdbk_i;
    rtb_TmpSignalConversionAtHSCL_1 = rtb_SumFdbk_i;
    rtb_Divide = controller_DW.obj.FilterObj->cSFunObject.P1_RTP1COEFF[3] *
      rtb_SumFdbk_i;
    rtb_Divide += controller_DW.obj.FilterObj->cSFunObject.P1_RTP1COEFF[4] *
      controller_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES[2];
    rtb_Divide += controller_DW.obj.FilterObj->cSFunObject.P1_RTP1COEFF[5] *
      controller_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES[3];
    rtb_SumFdbk_i = rtb_Divide -
      controller_DW.obj.FilterObj->cSFunObject.P2_RTP2COEFF[2] *
      controller_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES[2];
    rtb_SumFdbk_i -= controller_DW.obj.FilterObj->cSFunObject.P2_RTP2COEFF[3] *
      controller_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES[3];
    controller_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES[3] =
      controller_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES[2];
    controller_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES[2] =
      rtb_TmpSignalConversionAtHSCL_1;
    controller_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES[3] =
      controller_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES[2];
    controller_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES[2] = rtb_SumFdbk_i;
    tmp = rtb_SumFdbk_i;
    rtb_Divide = controller_DW.obj.FilterObj->cSFunObject.P1_RTP1COEFF[0] *
      rtb_TmpSignalConversionAtHSCL_2;
    rtb_Divide += controller_DW.obj.FilterObj->cSFunObject.P1_RTP1COEFF[1] *
      controller_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES[4];
    rtb_Divide += controller_DW.obj.FilterObj->cSFunObject.P1_RTP1COEFF[2] *
      controller_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES[5];
    rtb_SumFdbk_i = rtb_Divide -
      controller_DW.obj.FilterObj->cSFunObject.P2_RTP2COEFF[0] *
      controller_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES[4];
    rtb_SumFdbk_i -= controller_DW.obj.FilterObj->cSFunObject.P2_RTP2COEFF[1] *
      controller_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES[5];
    controller_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES[5] =
      controller_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES[4];
    controller_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES[4] =
      rtb_TmpSignalConversionAtHSCL_2;
    controller_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES[5] =
      controller_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES[4];
    controller_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES[4] = rtb_SumFdbk_i;
    rtb_TmpSignalConversionAtHSCL_1 = rtb_SumFdbk_i;
    rtb_Divide = controller_DW.obj.FilterObj->cSFunObject.P1_RTP1COEFF[3] *
      rtb_SumFdbk_i;
    rtb_Divide += controller_DW.obj.FilterObj->cSFunObject.P1_RTP1COEFF[4] *
      controller_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES[6];
    rtb_Divide += controller_DW.obj.FilterObj->cSFunObject.P1_RTP1COEFF[5] *
      controller_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES[7];
    rtb_SumFdbk_i = rtb_Divide -
      controller_DW.obj.FilterObj->cSFunObject.P2_RTP2COEFF[2] *
      controller_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES[6];
    rtb_SumFdbk_i -= controller_DW.obj.FilterObj->cSFunObject.P2_RTP2COEFF[3] *
      controller_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES[7];
    controller_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES[7] =
      controller_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES[6];
    controller_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES[6] =
      rtb_TmpSignalConversionAtHSCL_1;
    controller_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES[7] =
      controller_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES[6];
    controller_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES[6] = rtb_SumFdbk_i;
    rtb_IProdOut_d = rtb_SumFdbk_i;
    rtb_Divide = controller_DW.obj.FilterObj->cSFunObject.P1_RTP1COEFF[0] *
      rtb_TmpSignalConversionAtHSCL_3;
    rtb_Divide += controller_DW.obj.FilterObj->cSFunObject.P1_RTP1COEFF[1] *
      controller_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES[8];
    rtb_Divide += controller_DW.obj.FilterObj->cSFunObject.P1_RTP1COEFF[2] *
      controller_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES[9];
    rtb_SumFdbk_i = rtb_Divide -
      controller_DW.obj.FilterObj->cSFunObject.P2_RTP2COEFF[0] *
      controller_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES[8];
    rtb_SumFdbk_i -= controller_DW.obj.FilterObj->cSFunObject.P2_RTP2COEFF[1] *
      controller_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES[9];
    controller_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES[9] =
      controller_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES[8];
    controller_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES[8] =
      rtb_TmpSignalConversionAtHSCL_3;
    controller_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES[9] =
      controller_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES[8];
    controller_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES[8] = rtb_SumFdbk_i;
    rtb_TmpSignalConversionAtHSCL_1 = rtb_SumFdbk_i;
    rtb_Divide = controller_DW.obj.FilterObj->cSFunObject.P1_RTP1COEFF[3] *
      rtb_SumFdbk_i;
    rtb_Divide += controller_DW.obj.FilterObj->cSFunObject.P1_RTP1COEFF[4] *
      controller_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES[10];
    rtb_Divide += controller_DW.obj.FilterObj->cSFunObject.P1_RTP1COEFF[5] *
      controller_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES[11];
    rtb_SumFdbk_i = rtb_Divide -
      controller_DW.obj.FilterObj->cSFunObject.P2_RTP2COEFF[2] *
      controller_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES[10];
    rtb_SumFdbk_i -= controller_DW.obj.FilterObj->cSFunObject.P2_RTP2COEFF[3] *
      controller_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES[11];
    controller_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES[11] =
      controller_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES[10];
    controller_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES[10] =
      rtb_TmpSignalConversionAtHSCL_1;
    controller_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES[11] =
      controller_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES[10];
    controller_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES[10] = rtb_SumFdbk_i;

    // Outport: '<Root>/signals' incorporates:
    //   MATLABSystem: '<S1>/HSC LP'
    //   SignalConversion generated from: '<S1>/signals'

    controller_Y.signals[4] = rtb_SumFdbk_i;

    // MATLABSystem: '<S1>/HSC LP'
    rtb_Divide = controller_DW.obj.FilterObj->cSFunObject.P1_RTP1COEFF[0] *
      rtb_TmpSignalConversionAtHSCL_0;
    rtb_Divide += controller_DW.obj.FilterObj->cSFunObject.P1_RTP1COEFF[1] *
      controller_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES[12];
    rtb_Divide += controller_DW.obj.FilterObj->cSFunObject.P1_RTP1COEFF[2] *
      controller_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES[13];
    rtb_SumFdbk_i = rtb_Divide -
      controller_DW.obj.FilterObj->cSFunObject.P2_RTP2COEFF[0] *
      controller_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES[12];
    rtb_SumFdbk_i -= controller_DW.obj.FilterObj->cSFunObject.P2_RTP2COEFF[1] *
      controller_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES[13];
    controller_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES[13] =
      controller_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES[12];
    controller_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES[12] =
      rtb_TmpSignalConversionAtHSCL_0;
    controller_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES[13] =
      controller_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES[12];
    controller_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES[12] = rtb_SumFdbk_i;
    rtb_TmpSignalConversionAtHSCL_1 = rtb_SumFdbk_i;
    rtb_Divide = controller_DW.obj.FilterObj->cSFunObject.P1_RTP1COEFF[3] *
      rtb_SumFdbk_i;
    rtb_Divide += controller_DW.obj.FilterObj->cSFunObject.P1_RTP1COEFF[4] *
      controller_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES[14];
    rtb_Divide += controller_DW.obj.FilterObj->cSFunObject.P1_RTP1COEFF[5] *
      controller_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES[15];
    rtb_SumFdbk_i = rtb_Divide -
      controller_DW.obj.FilterObj->cSFunObject.P2_RTP2COEFF[2] *
      controller_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES[14];
    rtb_SumFdbk_i -= controller_DW.obj.FilterObj->cSFunObject.P2_RTP2COEFF[3] *
      controller_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES[15];
    controller_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES[15] =
      controller_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES[14];
    controller_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES[14] =
      rtb_TmpSignalConversionAtHSCL_1;
    controller_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES[15] =
      controller_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES[14];
    controller_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES[14] = rtb_SumFdbk_i;
    rtb_TmpSignalConversionAtHSCL_0 = rtb_SumFdbk_i;

    // Bias: '<S13>/Bias' incorporates:
    //   Gain: '<S13>/Gain'
    //   UnitDelay: '<S1>/Unit Delay2'

    rtb_SumFdbk_i = controller_P.Gain_Gain_m * controller_DW.UnitDelay2_DSTATE +
      controller_P.Bias_Bias_b;

    // Product: '<S2>/Divide1' incorporates:
    //   Bias: '<S2>/Bias'
    //   Constant: '<S2>/Constant1'
    //   Gain: '<S2>/cmH2O to Pa'
    //   MATLABSystem: '<S1>/HSC LP'

    rtb_TmpSignalConversionAtHSCL_1 = controller_P.Constant1_Value_k /
      (controller_P.cmH2OtoPa_Gain * rtb_IProdOut_d + controller_P.Bias_Bias_j);

    // Product: '<S2>/Divide' incorporates:
    //   Bias: '<S2>/K'
    //   Constant: '<S2>/Constant'

    rtb_Divide = (rtb_SumFdbk_i + controller_P.K_Bias) /
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
      controller_DW.UnitDelay_DSTATE[0] + controller_P.Bias_Bias_l) *
      controller_U.params[0] * controller_P.Gain1_Gain_e * rtb_Divide *
      rtb_TmpSignalConversionAtHSCL_1;

    // Outputs for Enabled SubSystem: '<S1>/Compute Trigger' incorporates:
    //   EnablePort: '<S9>/Enable'

    // Logic: '<S1>/Logical Operator' incorporates:
    //   UnitDelay: '<S1>/Unit Delay3'

    if (!(controller_DW.UnitDelay3_DSTATE != 0.0F)) {
      controller_DW.ComputeTrigger_MODE = true;

      // RelationalOperator: '<S19>/Compare' incorporates:
      //   Constant: '<S19>/Constant'
      //   MATLABSystem: '<S1>/HSC LP'

      controller_B.Compare_m = (rtb_IProdOut_d <=
        controller_P.pressure_thres_const);
    } else {
      if (controller_DW.ComputeTrigger_MODE) {
        // Disable for Outport: '<S9>/Out'
        controller_B.Compare_m = controller_P.Out_Y0;
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
      controller_DW.UnitDelay_DSTATE[1] + controller_P.Bias_Bias_l) *
      controller_U.params[0] * controller_P.Gain1_Gain_e * rtb_Divide *
      rtb_TmpSignalConversionAtHSCL_1 * controller_P.Gain_Gain_e;

    // RelationalOperator: '<S4>/Compare' incorporates:
    //   Constant: '<S4>/Constant'

    rtb_Compare_k = (controller_P.Controller_control_mode ==
                     controller_P.CompareToConstant1_const);

    // Chart: '<S1>/Pulse Generator' incorporates:
    //   Constant: '<S1>/IE_ratio'
    //   Constant: '<S1>/enable_trigger'
    //   Constant: '<S1>/respiratory_rate'

    controller_DW.presentTicks_j = (&controller_M)->Timing.clockTick0;
    controller_DW.elapsedTicks_i = controller_DW.presentTicks_j -
      controller_DW.previousTicks_j;
    controller_DW.previousTicks_j = controller_DW.presentTicks_j;
    controller_DW.temporalCounter_i1_c += controller_DW.elapsedTicks_i;
    controller_DW.control_mode_prev = controller_DW.control_mode_start;
    controller_DW.control_mode_start = controller_P.Controller_control_mode;
    if (!controller_DW.doneDoubleBufferReInit) {
      controller_DW.doneDoubleBufferReInit = true;
      controller_DW.control_mode_prev = controller_P.Controller_control_mode;
    }

    switch (controller_DW.is_c3_controller) {
     case controller_IN_EXPIRATION:
      controller_B.pulse = 0.0F;
      if ((controller_DW.control_mode_prev != controller_DW.control_mode_start) ||
          (controller_DW.temporalCounter_i1_c >= 1.0F /
           (controller_P.IE_ratio_Value + 1.0F) * (60.0F /
            controller_P.respiratory_rate_Value) * 100.0F)) {
        controller_DW.is_c3_controller = controller_IN_INSPIRATION;
        controller_DW.temporalCounter_i1_c = 0U;
        controller_B.pulse = 1.0F;
      }
      break;

     case controller_IN_INSPIRATION:
      controller_B.pulse = 1.0F;
      if (controller_DW.control_mode_prev != controller_DW.control_mode_start) {
        controller_DW.is_c3_controller = controller_IN_INSPIRATION;
        controller_DW.temporalCounter_i1_c = 0U;
        controller_B.pulse = 1.0F;
      } else if (controller_DW.temporalCounter_i1_c >=
                 controller_P.IE_ratio_Value / (controller_P.IE_ratio_Value +
                  1.0F) * (60.0F / controller_P.respiratory_rate_Value) * 100.0F)
      {
        controller_DW.is_c3_controller = controller_IN_EXPIRATION;
        controller_DW.temporalCounter_i1_c = 0U;
        controller_B.pulse = 0.0F;
      } else {
        if (controller_P.enable_trigger_Value) {
          controller_DW.is_c3_controller = controller_IN_TRIGGER_WAIT;
          controller_B.pulse = 0.0F;
        }
      }
      break;

     case controll_IN_TRIGGER_INSPIRATION:
      controller_B.pulse = 1.0F;
      if (controller_DW.temporalCounter_i1_c >= controller_P.IE_ratio_Value /
          (controller_P.IE_ratio_Value + 1.0F) * (60.0F /
           controller_P.respiratory_rate_Value) * 100.0F) {
        controller_DW.is_c3_controller = controller_IN_TRIGGER_WAIT;
        controller_B.pulse = 0.0F;
      } else {
        if (!controller_P.enable_trigger_Value) {
          controller_DW.is_c3_controller = controller_IN_INSPIRATION;
          controller_DW.temporalCounter_i1_c = 0U;
          controller_B.pulse = 1.0F;
        }
      }
      break;

     default:
      // case IN_TRIGGER_WAIT:
      controller_B.pulse = 0.0F;
      if (controller_B.Compare_m) {
        controller_DW.is_c3_controller = controll_IN_TRIGGER_INSPIRATION;
        controller_DW.temporalCounter_i1_c = 0U;
        controller_B.pulse = 1.0F;
      } else {
        if (!controller_P.enable_trigger_Value) {
          controller_DW.is_c3_controller = controller_IN_INSPIRATION;
          controller_DW.temporalCounter_i1_c = 0U;
          controller_B.pulse = 1.0F;
        }
      }
      break;
    }

    // Switch: '<S1>/Switch4' incorporates:
    //   Constant: '<S1>/Constant8'

    if (rtb_Compare_k) {
      rtb_TmpSignalConversionAtHSCL_1 = controller_P.Constant8_Value;
    } else {
      rtb_TmpSignalConversionAtHSCL_1 = controller_B.pulse;
    }

    // End of Switch: '<S1>/Switch4'

    // DiscreteIntegrator: '<S1>/Discrete-Time Integrator' incorporates:
    //   Sum: '<S1>/Sum1'

    if ((rtb_TmpSignalConversionAtHSCL_1 > 0.0F) &&
        (controller_DW.DiscreteTimeIntegrator_PrevRese <= 0)) {
      controller_DW.DiscreteTimeIntegrator_DSTATE =
        controller_P.DiscreteTimeIntegrator_IC;
    }

    rtb_Divide = controller_P.DiscreteTimeIntegrator_gainval *
      (rtb_TmpSignalConversionAtHSCL_3 + rtb_TmpSignalConversionAtHSCL_2);
    controller_B.DiscreteTimeIntegrator = rtb_Divide +
      controller_DW.DiscreteTimeIntegrator_DSTATE;

    // End of DiscreteIntegrator: '<S1>/Discrete-Time Integrator'

    // RelationalOperator: '<S5>/Compare' incorporates:
    //   Constant: '<S5>/Constant'

    rtb_Compare_m = (controller_P.Controller_control_mode ==
                     controller_P.CompareToConstant2_const);

    // RelationalOperator: '<S6>/Compare' incorporates:
    //   Constant: '<S6>/Constant'

    rtb_Compare_f = (controller_P.Controller_control_mode ==
                     controller_P.CompareToConstant3_const);

    // Logic: '<S1>/Logical Operator6'
    controller_B.LogicalOperator6 = (rtb_Compare_m || rtb_Compare_f);

    // Chart: '<S1>/TD Max' incorporates:
    //   Constant: '<S1>/respiratory_rate'

    controller_DW.presentTicks = (&controller_M)->Timing.clockTick0;
    controller_DW.elapsedTicks = controller_DW.presentTicks -
      controller_DW.previousTicks;
    controller_DW.previousTicks = controller_DW.presentTicks;
    controller_DW.temporalCounter_i1 += controller_DW.elapsedTicks;
    if (controller_DW.is_c1_controller == 1) {
      if (!controller_B.LogicalOperator6) {
        controller_DW.is_c1_controller = controller_IN_IDLE;
        controller_B.TD_max = (rtNaNF);
      } else if (controller_DW.temporalCounter_i1 >= 60.0F /
                 controller_P.respiratory_rate_Value * 2.0F * 100.0F) {
        controller_B.TD_max = controller_DW.tmp_max;
        controller_DW.is_c1_controller = controller_IN_COMPUTE_MAX;
        controller_DW.temporalCounter_i1 = 0U;
        controller_DW.tmp_max = controller_B.DiscreteTimeIntegrator;
      } else {
        if ((!(controller_DW.tmp_max > controller_B.DiscreteTimeIntegrator)) &&
            (!rtIsNaNF(controller_B.DiscreteTimeIntegrator))) {
          controller_DW.tmp_max = controller_B.DiscreteTimeIntegrator;
        }
      }
    } else {
      // case IN_IDLE:
      if (controller_B.LogicalOperator6) {
        controller_DW.is_c1_controller = controller_IN_COMPUTE_MAX;
        controller_DW.temporalCounter_i1 = 0U;
        controller_DW.tmp_max = controller_B.DiscreteTimeIntegrator;
      }
    }

    // Outport: '<Root>/signals' incorporates:
    //   DataTypeConversion: '<S1>/Data Type Conversion'
    //   MATLABSystem: '<S1>/HSC LP'
    //   SignalConversion generated from: '<S1>/signals'

    controller_Y.signals[0] = tmp;
    controller_Y.signals[1] = rtb_TmpSignalConversionAtHSCL_3;
    controller_Y.signals[2] = rtb_IProdOut_d;
    controller_Y.signals[3] = rtb_SumFdbk_i;
    controller_Y.signals[5] = rtb_TmpSignalConversionAtHSCL_2;
    controller_Y.signals[6] = rtb_TmpSignalConversionAtHSCL_0;
    controller_Y.signals[7] = controller_B.DiscreteTimeIntegrator;
    controller_Y.signals[8] = controller_B.TD_max;
    controller_Y.signals[9] = controller_B.Compare_m;

    // Outputs for Enabled SubSystem: '<S1>/Pressure Control' incorporates:
    //   EnablePort: '<S11>/Enable'

    // Logic: '<S1>/Logical Operator2' incorporates:
    //   Logic: '<S11>/Logical Operator'

    if (rtb_Compare_k || rtb_Compare_f) {
      if (!controller_DW.PressureControl_MODE) {
        // InitializeConditions for DiscreteFilter: '<S11>/Input Shaping'
        controller_DW.InputShaping_icLoad = 1U;

        // InitializeConditions for UnitDelay: '<S11>/Unit Delay'
        controller_DW.UnitDelay_DSTATE_o =
          controller_P.UnitDelay_InitialCondition;

        // InitializeConditions for DiscreteIntegrator: '<S54>/Integrator'
        controller_DW.Integrator_DSTATE_p =
          controller_P.PIDController_InitialConditionF;
        controller_DW.Integrator_PrevResetState_f = 2;
        controller_DW.PressureControl_MODE = true;
      }

      // Gain: '<S11>/PIP'
      rtb_PIP = controller_P.PIP_Gain * rtb_TmpSignalConversionAtHSCL_1;

      // Gain: '<S20>/Gain' incorporates:
      //   Constant: '<S1>/IE_ratio'
      //   Constant: '<S1>/respiratory_rate'
      //   Constant: '<S20>/Constant 1'
      //   Constant: '<S20>/PIP_rise_perc'
      //   Fcn: '<S20>/fun1'
      //   Product: '<S20>/Product2'

      rtb_IProdOut_d = controller_P.PIP_rise_perc_Value *
        controller_P.Constant1_Value / controller_P.respiratory_rate_Value *
        (controller_P.IE_ratio_Value / (controller_P.IE_ratio_Value + 1.0F)) *
        controller_P.Gain_Gain;

      // Math: '<S20>/Math Function'
      rtb_SumFdbk_i = rtb_IProdOut_d * rtb_IProdOut_d;

      // Product: '<S20>/Product1' incorporates:
      //   Constant: '<S20>/Constant'
      //   Constant: '<S20>/Constant1'
      //   Product: '<S20>/Divide'
      //   Product: '<S20>/Product'

      rtb_pnm_idx_0 = controller_P.Constant_Value_e[0] / (rtb_IProdOut_d *
        rtb_SumFdbk_i) * controller_P.Constant1_Value_n[0];
      rtb_pnm_idx_1 = controller_P.Constant_Value_e[1] / rtb_SumFdbk_i *
        controller_P.Constant1_Value_n[1];
      rtb_pnm_idx_2 = controller_P.Constant_Value_e[2] / rtb_IProdOut_d *
        controller_P.Constant1_Value_n[2];

      // Fcn: '<S20>/a0'
      rtb_Divide5 = ((rtb_pnm_idx_2 + rtb_pnm_idx_1) + rtb_pnm_idx_0) - 8.0F;

      // Product: '<S20>/Divide1' incorporates:
      //   Fcn: '<S20>/b0_3'

      rtb_IProdOut_d = rtb_pnm_idx_0 / rtb_Divide5;

      // Product: '<S20>/Divide2' incorporates:
      //   Fcn: '<S20>/b1_2'

      rtb_SumFdbk_i = 3.0F * rtb_pnm_idx_0 / rtb_Divide5;

      // Product: '<S20>/Divide3' incorporates:
      //   Fcn: '<S20>/a1'

      rtb_TmpSignalConversionAtHSCL_2 = (((-rtb_pnm_idx_2 + rtb_pnm_idx_1) +
        3.0F * rtb_pnm_idx_0) + 24.0F) / rtb_Divide5;

      // Product: '<S20>/Divide4' incorporates:
      //   Fcn: '<S20>/a2'

      rtb_SumFdbk = (((-rtb_pnm_idx_2 - rtb_pnm_idx_1) + 3.0F * rtb_pnm_idx_0) -
                     24.0F) / rtb_Divide5;

      // Product: '<S20>/Divide5' incorporates:
      //   Fcn: '<S20>/a3'

      rtb_Divide5 = (((rtb_pnm_idx_2 - rtb_pnm_idx_1) + rtb_pnm_idx_0) + 8.0F) /
        rtb_Divide5;

      // SignalConversion generated from: '<S11>/Input Shaping'
      controller_B.TmpSignalConversionAtInputShapi[0] = rtb_IProdOut_d;
      controller_B.TmpSignalConversionAtInputShapi[1] = rtb_SumFdbk_i;
      controller_B.TmpSignalConversionAtInputShapi[2] = rtb_SumFdbk_i;
      controller_B.TmpSignalConversionAtInputShapi[3] = rtb_IProdOut_d;

      // SignalConversion generated from: '<S11>/Input Shaping' incorporates:
      //   Constant: '<S20>/Constant2'

      controller_B.TmpSignalConversionAtInputSha_h[0] =
        controller_P.Constant2_Value;
      controller_B.TmpSignalConversionAtInputSha_h[1] =
        rtb_TmpSignalConversionAtHSCL_2;
      controller_B.TmpSignalConversionAtInputSha_h[2] = rtb_SumFdbk;
      controller_B.TmpSignalConversionAtInputSha_h[3] = rtb_Divide5;

      // Product: '<S11>/Product' incorporates:
      //   Fcn: '<S20>/fun'
      //   Gain: '<S11>/Gain3'
      //   Gain: '<S11>/PIP_step_perc'
      //   MATLABSystem: '<S1>/HSC LP'
      //   Sum: '<S11>/Sum3'
      //   Sum: '<S11>/Sum4'

      rtb_Divide5 = (rtb_TmpSignalConversionAtHSCL_2 - rtb_IProdOut_d) /
        (((rtb_TmpSignalConversionAtHSCL_2 + rtb_SumFdbk) + rtb_Divide5) *
         rtb_SumFdbk_i + (2.0F * rtb_SumFdbk_i + rtb_IProdOut_d) *
         rtb_TmpSignalConversionAtHSCL_2) * ((rtb_PIP - tmp) *
        controller_P.PIP_step_perc_Gain * controller_P.Gain3_Gain + tmp);

      // DiscreteFilter: '<S11>/Input Shaping'
      if (rt_R32ZCFcn(RISING_ZERO_CROSSING,
                      &controller_PrevZCX.InputShaping_Reset_ZCE,
                      (rtb_TmpSignalConversionAtHSCL_1)) != NO_ZCEVENT) {
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
      rtb_SumFdbk_i = ((controller_B.TmpSignalConversionAtInputShapi[0] *
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

      rtb_Divide5 = rtb_SumFdbk_i - tmp;

      // Outputs for Enabled SubSystem: '<S11>/Params Selector' incorporates:
      //   EnablePort: '<S22>/Enable'

      if (!(rtb_TmpSignalConversionAtHSCL_1 != 0.0F)) {
        // Switch: '<S22>/Switch2' incorporates:
        //   Constant: '<S22>/PC_FFW'
        //   Constant: '<S22>/PC_FFW_rec'
        //   Constant: '<S22>/PC_I'
        //   Constant: '<S22>/PC_I_rec'
        //   Constant: '<S22>/PC_P'
        //   Constant: '<S22>/PC_P_rec'
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

        // End of Switch: '<S22>/Switch2'

        // SignalConversion generated from: '<S22>/FFW'
        controller_B.OutportBufferForFFW = controller_B.Switch2[2];

        // SignalConversion generated from: '<S22>/I'
        controller_B.OutportBufferForI = controller_B.Switch2[1];
      }

      // End of Outputs for SubSystem: '<S11>/Params Selector'

      // Product: '<S51>/IProd Out' incorporates:
      //   Logic: '<S11>/Logical Operator'

      rtb_PIP = rtb_Divide5 * controller_B.OutportBufferForI;

      // Product: '<S59>/PProd Out'
      rtb_Divide5 *= controller_B.Switch2[0];

      // Sum: '<S66>/SumI1' incorporates:
      //   Gain: '<S65>/Kt'
      //   Sum: '<S11>/Sum5'
      //   Sum: '<S64>/Sum Fdbk'
      //   Sum: '<S65>/SumI3'
      //   UnitDelay: '<S11>/Unit Delay'

      rtb_PIP += ((controller_DW.UnitDelay_DSTATE_o -
                   controller_B.OutportBufferForFFW) - (rtb_Divide5 +
        controller_DW.Integrator_DSTATE_p)) * controller_P.PIDController_Kt;

      // DiscreteIntegrator: '<S54>/Integrator'
      if ((rtb_TmpSignalConversionAtHSCL_1 > 0.0F) &&
          (controller_DW.Integrator_PrevResetState_f <= 0)) {
        controller_DW.Integrator_DSTATE_p =
          controller_P.PIDController_InitialConditionF;
      }

      controller_B.Integrator_m = controller_P.Integrator_gainval * rtb_PIP +
        controller_DW.Integrator_DSTATE_p;

      // End of DiscreteIntegrator: '<S54>/Integrator'

      // Sum: '<S11>/Sum2' incorporates:
      //   Sum: '<S63>/Sum'

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

      if (rtb_TmpSignalConversionAtHSCL_1 > controller_P.Switch_Threshold) {
        controller_B.Switch_n = rtb_Divide5;
      } else {
        controller_B.Switch_n = controller_P.Constant_Value;
      }

      // End of Switch: '<S11>/Switch'

      // Switch: '<S11>/Switch1' incorporates:
      //   Constant: '<S11>/Constant'

      if (rtb_TmpSignalConversionAtHSCL_1 > controller_P.Switch1_Threshold) {
        controller_B.Switch1 = rtb_SumFdbk_i;
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

    // Gain: '<S1>/Tidal Volume'
    rtb_SumFdbk_i = controller_P.TidalVolume_Gain *
      rtb_TmpSignalConversionAtHSCL_1;

    // Logic: '<S1>/Logical Operator4' incorporates:
    //   Constant: '<S8>/Constant'
    //   Logic: '<S1>/Logical Operator5'
    //   RelationalOperator: '<S8>/Compare'

    rtb_Compare_m = ((rtb_Compare_f && (rtb_SumFdbk_i ==
      controller_P.Constant_Value_d)) || rtb_Compare_m);

    // Outputs for Enabled SubSystem: '<S1>/Volume Control' incorporates:
    //   EnablePort: '<S18>/Enable'

    if (rtb_Compare_m) {
      controller_DW.VolumeControl_MODE = true;

      // Switch: '<S18>/Switch' incorporates:
      //   Constant: '<S1>/flowby_target'
      //   Constant: '<S71>/Constant'
      //   Gain: '<S18>/Volumetric Flow rate'
      //   RelationalOperator: '<S71>/Compare'

      if (rtb_SumFdbk_i == controller_P.Constant_Value_b) {
        controller_B.Switch_c = controller_P.flowby_target_Value;
      } else {
        controller_B.Switch_c = controller_P.VolumetricFlowrate_Gain *
          rtb_SumFdbk_i;
      }

      // End of Switch: '<S18>/Switch'

      // Outputs for Triggered SubSystem: '<S18>/Store FFW' incorporates:
      //   TriggerPort: '<S75>/Trigger'

      zcEvent = rt_R32ZCFcn(FALLING_ZERO_CROSSING,
                            &controller_PrevZCX.StoreFFW_Trig_ZCE,
                            (controller_B.Switch_c));
      if (zcEvent != NO_ZCEVENT) {
        // DataStoreWrite: '<S75>/Data Store Write' incorporates:
        //   UnitDelay: '<S18>/Unit Delay'

        controller_DW.FFW = controller_DW.UnitDelay_DSTATE_n;
      }

      // End of Outputs for SubSystem: '<S18>/Store FFW'

      // Sum: '<S18>/Sum1'
      rtb_TmpSignalConversionAtHSCL_2 = controller_B.Switch_c -
        rtb_TmpSignalConversionAtHSCL_3;

      // Switch: '<S73>/Switch2' incorporates:
      //   Constant: '<S73>/VC_I'
      //   Constant: '<S73>/VC_I_rec'
      //   Constant: '<S73>/VC_P'
      //   Constant: '<S73>/VC_P_rec'
      //   MATLABSystem: '<S1>/HSC LP'

      if (rtb_TmpSignalConversionAtHSCL_0 > controller_P.Switch2_Threshold_b) {
        rtb_TmpSignalConversionAtHSCL_3 = controller_P.VC_P_rec_Value;
        rtb_SumFdbk_i = controller_P.VC_I_rec_Value;
      } else {
        rtb_TmpSignalConversionAtHSCL_3 = controller_P.VC_P_Value;
        rtb_SumFdbk_i = controller_P.VC_I_Value;
      }

      // End of Switch: '<S73>/Switch2'

      // Product: '<S104>/IProd Out'
      rtb_IProdOut_d = rtb_TmpSignalConversionAtHSCL_2 * rtb_SumFdbk_i;

      // Product: '<S112>/PProd Out'
      rtb_TmpSignalConversionAtHSCL_2 *= rtb_TmpSignalConversionAtHSCL_3;

      // Outputs for Enabled SubSystem: '<S18>/Read FFW' incorporates:
      //   EnablePort: '<S74>/Enable'

      if (controller_B.Switch_c > 0.0F) {
        controller_DW.ReadFFW_MODE = true;

        // DataStoreRead: '<S74>/Data Store Read'
        controller_B.DataStoreRead = controller_DW.FFW;
      } else {
        if (controller_DW.ReadFFW_MODE) {
          // Disable for Outport: '<S74>/ffw'
          controller_B.DataStoreRead = controller_P.ffw_Y0;
          controller_DW.ReadFFW_MODE = false;
        }
      }

      // End of Outputs for SubSystem: '<S18>/Read FFW'

      // Sum: '<S119>/SumI1' incorporates:
      //   Gain: '<S118>/Kt'
      //   Sum: '<S117>/Sum Fdbk'
      //   Sum: '<S118>/SumI3'
      //   Sum: '<S18>/Sum3'
      //   UnitDelay: '<S18>/Unit Delay'

      rtb_SumFdbk_i = ((controller_DW.UnitDelay_DSTATE_n -
                        controller_B.DataStoreRead) -
                       (rtb_TmpSignalConversionAtHSCL_2 +
                        controller_DW.Integrator_DSTATE)) *
        controller_P.PIDController_Kt_j + rtb_IProdOut_d;

      // DiscreteIntegrator: '<S107>/Integrator'
      if (((controller_B.Switch_c > 0.0F) &&
           (controller_DW.Integrator_PrevResetState <= 0)) ||
          ((controller_B.Switch_c <= 0.0F) &&
           (controller_DW.Integrator_PrevResetState == 1))) {
        controller_DW.Integrator_DSTATE =
          controller_P.PIDController_InitialConditio_c;
      }

      controller_B.Integrator = controller_P.Integrator_gainval_m *
        rtb_SumFdbk_i + controller_DW.Integrator_DSTATE;

      // End of DiscreteIntegrator: '<S107>/Integrator'

      // Sum: '<S18>/Sum2' incorporates:
      //   Sum: '<S116>/Sum'

      tmp = (rtb_TmpSignalConversionAtHSCL_2 + controller_B.Integrator) +
        controller_B.DataStoreRead;

      // Saturate: '<S18>/Saturation'
      if (tmp > controller_P.Saturation_UpperSat_b) {
        controller_B.Saturation = controller_P.Saturation_UpperSat_b;
      } else if (tmp < controller_P.Saturation_LowerSat_f) {
        controller_B.Saturation = controller_P.Saturation_LowerSat_f;
      } else {
        controller_B.Saturation = tmp;
      }

      // End of Saturate: '<S18>/Saturation'

      // Update for UnitDelay: '<S18>/Unit Delay'
      controller_DW.UnitDelay_DSTATE_n = controller_B.Saturation;

      // Update for DiscreteIntegrator: '<S107>/Integrator'
      controller_DW.Integrator_DSTATE = controller_P.Integrator_gainval_m *
        rtb_SumFdbk_i + controller_B.Integrator;
      if (controller_B.Switch_c > 0.0F) {
        controller_DW.Integrator_PrevResetState = 1;
      } else if (controller_B.Switch_c < 0.0F) {
        controller_DW.Integrator_PrevResetState = -1;
      } else if (controller_B.Switch_c == 0.0F) {
        controller_DW.Integrator_PrevResetState = 0;
      } else {
        controller_DW.Integrator_PrevResetState = 2;
      }

      // End of Update for DiscreteIntegrator: '<S107>/Integrator'

      // Switch: '<S1>/Switch' incorporates:
      //   Outport: '<Root>/IV_dc'

      controller_Y.IV_dc = controller_B.Saturation;
    } else {
      if (controller_DW.VolumeControl_MODE) {
        // Disable for Enabled SubSystem: '<S18>/Read FFW'
        if (controller_DW.ReadFFW_MODE) {
          // Disable for Outport: '<S74>/ffw'
          controller_B.DataStoreRead = controller_P.ffw_Y0;
          controller_DW.ReadFFW_MODE = false;
        }

        // End of Disable for SubSystem: '<S18>/Read FFW'

        // Disable for Outport: '<S18>/Q_ref'
        controller_B.Switch_c = controller_P.Q_ref_Y0;

        // Disable for Outport: '<S18>/IV_dc'
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

    controller_Y.Q_ref = controller_B.Switch_c;

    // Outport: '<Root>/P_ref' incorporates:
    //   SignalConversion generated from: '<S1>/P_ref'

    controller_Y.P_ref = controller_B.Switch1;

    // Switch: '<S1>/Switch3' incorporates:
    //   Constant: '<S1>/Constant'
    //   Constant: '<S3>/Constant'
    //   Logic: '<S1>/Logical Operator3'
    //   RelationalOperator: '<S3>/Compare'
    //   Sum: '<S1>/Sum'

    if ((controller_P.Controller_control_mode ==
         controller_P.CompareToConstant_const) || rtb_Compare_k) {
      rtb_SumFdbk_i = controller_P.Constant_Value_a;
    } else {
      rtb_SumFdbk_i = controller_P.Constant_Value_a -
        rtb_TmpSignalConversionAtHSCL_1;
    }

    // End of Switch: '<S1>/Switch3'

    // Outport: '<Root>/OV_cmd' incorporates:
    //   Constant: '<S7>/Constant'
    //   RelationalOperator: '<S7>/Compare'

    controller_Y.OV_cmd = (rtb_SumFdbk_i > controller_P.Constant_Value_p);

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

    controller_DW.UnitDelay_DSTATE[0] = controller_U.S2_Qi;
    controller_DW.UnitDelay_DSTATE[1] = controller_U.S5_Qi;

    // Update for UnitDelay: '<S1>/Unit Delay3'
    controller_DW.UnitDelay3_DSTATE = rtb_TmpSignalConversionAtHSCL_1;

    // Update for DiscreteIntegrator: '<S1>/Discrete-Time Integrator'
    controller_DW.DiscreteTimeIntegrator_DSTATE = rtb_Divide +
      controller_B.DiscreteTimeIntegrator;
    if (rtb_TmpSignalConversionAtHSCL_1 > 0.0F) {
      controller_DW.DiscreteTimeIntegrator_PrevRese = 1;
    } else if (rtb_TmpSignalConversionAtHSCL_1 < 0.0F) {
      controller_DW.DiscreteTimeIntegrator_PrevRese = -1;
    } else if (rtb_TmpSignalConversionAtHSCL_1 == 0.0F) {
      controller_DW.DiscreteTimeIntegrator_PrevRese = 0;
    } else {
      controller_DW.DiscreteTimeIntegrator_PrevRese = 2;
    }

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

      // Update for DiscreteIntegrator: '<S54>/Integrator'
      controller_DW.Integrator_DSTATE_p = controller_P.Integrator_gainval *
        rtb_PIP + controller_B.Integrator_m;
      if (rtb_TmpSignalConversionAtHSCL_1 > 0.0F) {
        controller_DW.Integrator_PrevResetState_f = 1;
      } else if (rtb_TmpSignalConversionAtHSCL_1 < 0.0F) {
        controller_DW.Integrator_PrevResetState_f = -1;
      } else if (rtb_TmpSignalConversionAtHSCL_1 == 0.0F) {
        controller_DW.Integrator_PrevResetState_f = 0;
      } else {
        controller_DW.Integrator_PrevResetState_f = 2;
      }

      // End of Update for DiscreteIntegrator: '<S54>/Integrator'
    }

    // End of Update for SubSystem: '<S1>/Pressure Control'
  } else {
    if (controller_DW.Controller_MODE) {
      // Disable for Enabled SubSystem: '<S1>/Compute Trigger'
      if (controller_DW.ComputeTrigger_MODE) {
        // Disable for Outport: '<S9>/Out'
        controller_B.Compare_m = controller_P.Out_Y0;
        controller_DW.ComputeTrigger_MODE = false;
      }

      // End of Disable for SubSystem: '<S1>/Compute Trigger'

      // Disable for Chart: '<S1>/Pulse Generator'
      controller_DW.presentTicks_j = (&controller_M)->Timing.clockTick0;
      controller_DW.elapsedTicks_i = controller_DW.presentTicks_j -
        controller_DW.previousTicks_j;
      controller_DW.previousTicks_j = controller_DW.presentTicks_j;
      controller_DW.temporalCounter_i1_c += controller_DW.elapsedTicks_i;

      // Disable for Chart: '<S1>/TD Max'
      controller_DW.presentTicks = (&controller_M)->Timing.clockTick0;
      controller_DW.elapsedTicks = controller_DW.presentTicks -
        controller_DW.previousTicks;
      controller_DW.previousTicks = controller_DW.presentTicks;
      controller_DW.temporalCounter_i1 += controller_DW.elapsedTicks;

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
        // Disable for Enabled SubSystem: '<S18>/Read FFW'
        if (controller_DW.ReadFFW_MODE) {
          // Disable for Outport: '<S74>/ffw'
          controller_B.DataStoreRead = controller_P.ffw_Y0;
          controller_DW.ReadFFW_MODE = false;
        }

        // End of Disable for SubSystem: '<S18>/Read FFW'

        // Disable for Outport: '<S18>/Q_ref'
        controller_B.Switch_c = controller_P.Q_ref_Y0;

        // Disable for Outport: '<S18>/IV_dc'
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
      for (i = 0; i < 10; i++) {
        // Disable for Outport: '<Root>/signals' incorporates:
        //   Outport: '<S1>/signals'

        controller_Y.signals[i] = controller_P.signals_Y0;
      }

      controller_DW.Controller_MODE = false;
    }
  }

  // End of Inport: '<Root>/enable'
  // End of Outputs for SubSystem: '<Root>/Controller'

  // Update absolute time for base rate
  // The "clockTick0" counts the number of times the code of this task has
  //  been executed. The resolution of this integer timer is 0.01, which is the step size
  //  of the task. Size of "clockTick0" ensures timer will not overflow during the
  //  application lifespan selected.

  (&controller_M)->Timing.clockTick0++;
}

// Model initialize function
void controllerModelClass::initialize()
{
  // Registration code

  // initialize non-finites
  rt_InitInfAndNaN(sizeof(real_T));

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
    controller_DW.UnitDelay_DSTATE[0] =
      controller_P.UnitDelay_InitialCondition_g;
    controller_DW.UnitDelay_DSTATE[1] =
      controller_P.UnitDelay_InitialCondition_g;

    // InitializeConditions for UnitDelay: '<S1>/Unit Delay3'
    controller_DW.UnitDelay3_DSTATE = controller_P.UnitDelay3_InitialCondition;

    // InitializeConditions for DiscreteIntegrator: '<S1>/Discrete-Time Integrator' 
    controller_DW.DiscreteTimeIntegrator_DSTATE =
      controller_P.DiscreteTimeIntegrator_IC;
    controller_DW.DiscreteTimeIntegrator_PrevRese = 2;

    // SystemInitialize for Enabled SubSystem: '<S1>/Compute Trigger'
    // SystemInitialize for Outport: '<S9>/Out'
    controller_B.Compare_m = controller_P.Out_Y0;

    // End of SystemInitialize for SubSystem: '<S1>/Compute Trigger'

    // Chart: '<S1>/Pulse Generator'
    controller_DW.is_c3_controller = controller_IN_INSPIRATION;
    controller_B.pulse = 1.0F;

    // Chart: '<S1>/TD Max'
    controller_DW.is_c1_controller = controller_IN_IDLE;
    controller_B.TD_max = (rtNaNF);

    // SystemInitialize for Enabled SubSystem: '<S1>/Pressure Control'
    // InitializeConditions for DiscreteFilter: '<S11>/Input Shaping'
    controller_DW.InputShaping_icLoad = 1U;

    // InitializeConditions for UnitDelay: '<S11>/Unit Delay'
    controller_DW.UnitDelay_DSTATE_o = controller_P.UnitDelay_InitialCondition;

    // InitializeConditions for DiscreteIntegrator: '<S54>/Integrator'
    controller_DW.Integrator_DSTATE_p =
      controller_P.PIDController_InitialConditionF;
    controller_DW.Integrator_PrevResetState_f = 2;

    // SystemInitialize for Enabled SubSystem: '<S11>/Params Selector'
    // SystemInitialize for Outport: '<S22>/P'
    controller_B.Switch2[0] = controller_P.P_Y0;

    // SystemInitialize for Outport: '<S22>/I'
    controller_B.OutportBufferForI = controller_P.I_Y0;

    // SystemInitialize for Outport: '<S22>/FFW'
    controller_B.OutportBufferForFFW = controller_P.FFW_Y0;

    // End of SystemInitialize for SubSystem: '<S11>/Params Selector'

    // SystemInitialize for Outport: '<S11>/P_ref'
    controller_B.Switch1 = controller_P.P_ref_Y0;

    // SystemInitialize for Outport: '<S11>/IV_dc'
    controller_B.Switch_n = controller_P.IV_dc_Y0;

    // End of SystemInitialize for SubSystem: '<S1>/Pressure Control'

    // SystemInitialize for Enabled SubSystem: '<S1>/Volume Control'
    // Start for DataStoreMemory: '<S18>/Data Store Memory'
    controller_DW.FFW = controller_P.DataStoreMemory_InitialValue;

    // InitializeConditions for UnitDelay: '<S18>/Unit Delay'
    controller_DW.UnitDelay_DSTATE_n = controller_P.UnitDelay_InitialCondition_n;

    // InitializeConditions for DiscreteIntegrator: '<S107>/Integrator'
    controller_DW.Integrator_DSTATE =
      controller_P.PIDController_InitialConditio_c;
    controller_DW.Integrator_PrevResetState = 2;

    // SystemInitialize for Enabled SubSystem: '<S18>/Read FFW'
    // SystemInitialize for Outport: '<S74>/ffw'
    controller_B.DataStoreRead = controller_P.ffw_Y0;

    // End of SystemInitialize for SubSystem: '<S18>/Read FFW'

    // SystemInitialize for Outport: '<S18>/Q_ref'
    controller_B.Switch_c = controller_P.Q_ref_Y0;

    // SystemInitialize for Outport: '<S18>/IV_dc'
    controller_B.Saturation = controller_P.IV_dc_Y0_m;

    // End of SystemInitialize for SubSystem: '<S1>/Volume Control'

    // Start for MATLABSystem: '<S1>/HSC LP'
    controller_DW.gobj_1.matlabCodegenIsDeleted = true;
    controller_DW.gobj_2.matlabCodegenIsDeleted = true;
    controller_DW.obj.matlabCodegenIsDeleted = true;
    iobj_0 = &controller_DW.gobj_2;
    controller_DW.obj.isInitialized = 0;
    controller_DW.obj.TunablePropsChanged = false;
    controller_DW.obj.NumChannels = -1;
    controller_DW.obj.matlabCodegenIsDeleted = false;
    controller_DW.objisempty = true;
    controller_DW.obj.isSetupComplete = false;
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
    for (i = 0; i < 10; i++) {
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
