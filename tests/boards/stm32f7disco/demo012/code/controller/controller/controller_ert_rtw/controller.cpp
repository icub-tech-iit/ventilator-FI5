//
// Non-Degree Granting Education License -- for use at non-degree
// granting, nonprofit, educational organizations only. Not for
// government, commercial, or other organizational use.
//
// File: controller.cpp
//
// Code generated for Simulink model 'controller'.
//
// Model version                  : 1.491
// Simulink Coder version         : 9.3 (R2020a) 18-Nov-2019
// C/C++ source code generated on : Thu May  7 22:28:47 2020
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
  real32_T rtb_Switch3;
  real32_T rtb_PIP;
  boolean_T rtb_Compare;
  ZCEventType zcEvent;
  boolean_T rtb_Compare_k;
  boolean_T rtb_LogicalOperator1;
  boolean_T rtb_Compare_p;
  real32_T rtb_Divide1;
  int32_T i;
  real32_T rtb_TmpSignalConversionAtHSCL_0;
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

      // InitializeConditions for UnitDelay: '<S1>/Unit Delay'
      controller_DW.UnitDelay_DSTATE[0] =
        controller_P.UnitDelay_InitialCondition_g;
      controller_DW.UnitDelay_DSTATE[1] =
        controller_P.UnitDelay_InitialCondition_g;

      // InitializeConditions for UnitDelay: '<S1>/Unit Delay2'
      controller_DW.UnitDelay2_DSTATE = controller_P.UnitDelay2_InitialCondition;

      // InitializeConditions for DiscreteIntegrator: '<S1>/Discrete-Time Integrator' 
      controller_DW.DiscreteTimeIntegrator_DSTATE =
        controller_P.DiscreteTimeIntegrator_IC;
      controller_DW.DiscreteTimeIntegrator_PrevRese = 2;

      // InitializeConditions for UnitDelay: '<S1>/Unit Delay3'
      controller_DW.UnitDelay3_DSTATE = controller_P.UnitDelay3_InitialCondition;

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

      // Chart: '<S1>/Pulse Generator'
      controller_DW.is_c3_controller = controller_IN_INSPIRATION;
      controller_DW.temporalCounter_i1 = 0U;

      // Enable for Chart: '<S1>/Pulse Generator'
      controller_DW.presentTicks = (&controller_M)->Timing.clockTick0;
      controller_DW.previousTicks = controller_DW.presentTicks;
      controller_DW.Controller_MODE = true;
    }

    // SignalConversion generated from: '<S1>/HSC LP' incorporates:
    //   Bias: '<S14>/Bias'
    //   Bias: '<S15>/Bias'
    //   Gain: '<S14>/Gain'
    //   Gain: '<S14>/Gain1'
    //   Gain: '<S15>/Gain'
    //   Gain: '<S15>/Gain1'
    //   Gain: '<S1>/mabr_to_cmH2O'
    //   Gain: '<S1>/psi_cmH2O'
    //   Inport: '<Root>/params'
    //   Product: '<S14>/Product'
    //   Product: '<S15>/Product'
    //   Sum: '<S15>/Sum'
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
    rtb_Divide1 = controller_DW.obj.FilterObj->cSFunObject.P1_RTP1COEFF[0] *
      rtb_TmpSignalConversionAtHSCL_1;
    rtb_Divide1 += controller_DW.obj.FilterObj->cSFunObject.P1_RTP1COEFF[1] *
      controller_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES[0];
    rtb_Divide1 += controller_DW.obj.FilterObj->cSFunObject.P1_RTP1COEFF[2] *
      controller_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES[1];
    rtb_Switch3 = rtb_Divide1 -
      controller_DW.obj.FilterObj->cSFunObject.P2_RTP2COEFF[0] *
      controller_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES[0];
    rtb_Switch3 -= controller_DW.obj.FilterObj->cSFunObject.P2_RTP2COEFF[1] *
      controller_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES[1];
    controller_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES[1] =
      controller_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES[0];
    controller_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES[0] =
      rtb_TmpSignalConversionAtHSCL_1;
    controller_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES[1] =
      controller_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES[0];
    controller_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES[0] = rtb_Switch3;
    rtb_TmpSignalConversionAtHSCL_1 = rtb_Switch3;
    rtb_Divide1 = controller_DW.obj.FilterObj->cSFunObject.P1_RTP1COEFF[3] *
      rtb_Switch3;
    rtb_Divide1 += controller_DW.obj.FilterObj->cSFunObject.P1_RTP1COEFF[4] *
      controller_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES[2];
    rtb_Divide1 += controller_DW.obj.FilterObj->cSFunObject.P1_RTP1COEFF[5] *
      controller_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES[3];
    rtb_Switch3 = rtb_Divide1 -
      controller_DW.obj.FilterObj->cSFunObject.P2_RTP2COEFF[2] *
      controller_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES[2];
    rtb_Switch3 -= controller_DW.obj.FilterObj->cSFunObject.P2_RTP2COEFF[3] *
      controller_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES[3];
    controller_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES[3] =
      controller_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES[2];
    controller_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES[2] =
      rtb_TmpSignalConversionAtHSCL_1;
    controller_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES[3] =
      controller_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES[2];
    controller_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES[2] = rtb_Switch3;
    controller_B.HSCLP[0] = rtb_Switch3;
    rtb_Divide1 = controller_DW.obj.FilterObj->cSFunObject.P1_RTP1COEFF[0] *
      rtb_TmpSignalConversionAtHSCL_2;
    rtb_Divide1 += controller_DW.obj.FilterObj->cSFunObject.P1_RTP1COEFF[1] *
      controller_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES[4];
    rtb_Divide1 += controller_DW.obj.FilterObj->cSFunObject.P1_RTP1COEFF[2] *
      controller_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES[5];
    rtb_Switch3 = rtb_Divide1 -
      controller_DW.obj.FilterObj->cSFunObject.P2_RTP2COEFF[0] *
      controller_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES[4];
    rtb_Switch3 -= controller_DW.obj.FilterObj->cSFunObject.P2_RTP2COEFF[1] *
      controller_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES[5];
    controller_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES[5] =
      controller_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES[4];
    controller_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES[4] =
      rtb_TmpSignalConversionAtHSCL_2;
    controller_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES[5] =
      controller_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES[4];
    controller_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES[4] = rtb_Switch3;
    rtb_TmpSignalConversionAtHSCL_1 = rtb_Switch3;
    rtb_Divide1 = controller_DW.obj.FilterObj->cSFunObject.P1_RTP1COEFF[3] *
      rtb_Switch3;
    rtb_Divide1 += controller_DW.obj.FilterObj->cSFunObject.P1_RTP1COEFF[4] *
      controller_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES[6];
    rtb_Divide1 += controller_DW.obj.FilterObj->cSFunObject.P1_RTP1COEFF[5] *
      controller_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES[7];
    rtb_Switch3 = rtb_Divide1 -
      controller_DW.obj.FilterObj->cSFunObject.P2_RTP2COEFF[2] *
      controller_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES[6];
    rtb_Switch3 -= controller_DW.obj.FilterObj->cSFunObject.P2_RTP2COEFF[3] *
      controller_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES[7];
    controller_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES[7] =
      controller_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES[6];
    controller_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES[6] =
      rtb_TmpSignalConversionAtHSCL_1;
    controller_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES[7] =
      controller_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES[6];
    controller_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES[6] = rtb_Switch3;
    controller_B.HSCLP[1] = rtb_Switch3;
    rtb_Divide1 = controller_DW.obj.FilterObj->cSFunObject.P1_RTP1COEFF[0] *
      rtb_TmpSignalConversionAtHSCL_3;
    rtb_Divide1 += controller_DW.obj.FilterObj->cSFunObject.P1_RTP1COEFF[1] *
      controller_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES[8];
    rtb_Divide1 += controller_DW.obj.FilterObj->cSFunObject.P1_RTP1COEFF[2] *
      controller_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES[9];
    rtb_Switch3 = rtb_Divide1 -
      controller_DW.obj.FilterObj->cSFunObject.P2_RTP2COEFF[0] *
      controller_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES[8];
    rtb_Switch3 -= controller_DW.obj.FilterObj->cSFunObject.P2_RTP2COEFF[1] *
      controller_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES[9];
    controller_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES[9] =
      controller_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES[8];
    controller_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES[8] =
      rtb_TmpSignalConversionAtHSCL_3;
    controller_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES[9] =
      controller_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES[8];
    controller_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES[8] = rtb_Switch3;
    rtb_TmpSignalConversionAtHSCL_1 = rtb_Switch3;
    rtb_Divide1 = controller_DW.obj.FilterObj->cSFunObject.P1_RTP1COEFF[3] *
      rtb_Switch3;
    rtb_Divide1 += controller_DW.obj.FilterObj->cSFunObject.P1_RTP1COEFF[4] *
      controller_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES[10];
    rtb_Divide1 += controller_DW.obj.FilterObj->cSFunObject.P1_RTP1COEFF[5] *
      controller_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES[11];
    rtb_Switch3 = rtb_Divide1 -
      controller_DW.obj.FilterObj->cSFunObject.P2_RTP2COEFF[2] *
      controller_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES[10];
    rtb_Switch3 -= controller_DW.obj.FilterObj->cSFunObject.P2_RTP2COEFF[3] *
      controller_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES[11];
    controller_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES[11] =
      controller_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES[10];
    controller_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES[10] =
      rtb_TmpSignalConversionAtHSCL_1;
    controller_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES[11] =
      controller_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES[10];
    controller_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES[10] = rtb_Switch3;
    controller_B.HSCLP[2] = rtb_Switch3;
    rtb_Divide1 = controller_DW.obj.FilterObj->cSFunObject.P1_RTP1COEFF[0] *
      rtb_TmpSignalConversionAtHSCL_0;
    rtb_Divide1 += controller_DW.obj.FilterObj->cSFunObject.P1_RTP1COEFF[1] *
      controller_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES[12];
    rtb_Divide1 += controller_DW.obj.FilterObj->cSFunObject.P1_RTP1COEFF[2] *
      controller_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES[13];
    rtb_Switch3 = rtb_Divide1 -
      controller_DW.obj.FilterObj->cSFunObject.P2_RTP2COEFF[0] *
      controller_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES[12];
    rtb_Switch3 -= controller_DW.obj.FilterObj->cSFunObject.P2_RTP2COEFF[1] *
      controller_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES[13];
    controller_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES[13] =
      controller_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES[12];
    controller_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES[12] =
      rtb_TmpSignalConversionAtHSCL_0;
    controller_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES[13] =
      controller_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES[12];
    controller_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES[12] = rtb_Switch3;
    rtb_TmpSignalConversionAtHSCL_1 = rtb_Switch3;
    rtb_Divide1 = controller_DW.obj.FilterObj->cSFunObject.P1_RTP1COEFF[3] *
      rtb_Switch3;
    rtb_Divide1 += controller_DW.obj.FilterObj->cSFunObject.P1_RTP1COEFF[4] *
      controller_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES[14];
    rtb_Divide1 += controller_DW.obj.FilterObj->cSFunObject.P1_RTP1COEFF[5] *
      controller_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES[15];
    rtb_Switch3 = rtb_Divide1 -
      controller_DW.obj.FilterObj->cSFunObject.P2_RTP2COEFF[2] *
      controller_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES[14];
    rtb_Switch3 -= controller_DW.obj.FilterObj->cSFunObject.P2_RTP2COEFF[3] *
      controller_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES[15];
    controller_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES[15] =
      controller_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES[14];
    controller_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES[14] =
      rtb_TmpSignalConversionAtHSCL_1;
    controller_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES[15] =
      controller_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES[14];
    controller_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES[14] = rtb_Switch3;
    controller_B.HSCLP[3] = rtb_Switch3;

    // End of MATLABSystem: '<S1>/HSC LP'

    // Bias: '<S12>/Bias' incorporates:
    //   Gain: '<S12>/Gain'
    //   UnitDelay: '<S1>/Unit Delay2'

    controller_B.Bias = controller_P.Gain_Gain_m *
      controller_DW.UnitDelay2_DSTATE + controller_P.Bias_Bias_b;

    // Product: '<S2>/Divide' incorporates:
    //   Bias: '<S2>/K'
    //   Constant: '<S2>/Constant'

    rtb_TmpSignalConversionAtHSCL_1 = (controller_B.Bias + controller_P.K_Bias) /
      controller_P.Constant_Value_h;

    // Product: '<S2>/Divide1' incorporates:
    //   Bias: '<S2>/Bias'
    //   Constant: '<S2>/Constant1'
    //   Gain: '<S2>/cmH2O to Pa'

    rtb_Divide1 = controller_P.Constant1_Value_k / (controller_P.cmH2OtoPa_Gain *
      controller_B.HSCLP[1] + controller_P.Bias_Bias_j);

    // Product: '<S2>/Product1' incorporates:
    //   Bias: '<S13>/Bias'
    //   Gain: '<S13>/Gain'
    //   Gain: '<S13>/Gain1'
    //   Inport: '<Root>/params'
    //   Product: '<S13>/Product'
    //   Product: '<S2>/Product'
    //   UnitDelay: '<S1>/Unit Delay'

    controller_B.Product1[0] = (controller_P.Gain_Gain_f *
      controller_DW.UnitDelay_DSTATE[0] + controller_P.Bias_Bias_l) *
      controller_U.params[0] * controller_P.Gain1_Gain_e *
      rtb_TmpSignalConversionAtHSCL_1 * rtb_Divide1;
    controller_B.Product1[1] = (controller_P.Gain_Gain_f *
      controller_DW.UnitDelay_DSTATE[1] + controller_P.Bias_Bias_l) *
      controller_U.params[0] * controller_P.Gain1_Gain_e *
      rtb_TmpSignalConversionAtHSCL_1 * rtb_Divide1;

    // Gain: '<S1>/Gain'
    controller_B.Gain = controller_P.Gain_Gain_e * controller_B.Product1[1];

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

    rtb_TmpSignalConversionAtHSCL_2 =
      controller_P.DiscreteTimeIntegrator_gainval * (controller_B.Product1[0] +
      controller_B.Gain);
    controller_B.DiscreteTimeIntegrator = rtb_TmpSignalConversionAtHSCL_2 +
      controller_DW.DiscreteTimeIntegrator_DSTATE;

    // End of DiscreteIntegrator: '<S1>/Discrete-Time Integrator'

    // Outputs for Enabled SubSystem: '<S1>/Compute Trigger' incorporates:
    //   EnablePort: '<S8>/Enable'

    // Logic: '<S1>/Logical Operator' incorporates:
    //   UnitDelay: '<S1>/Unit Delay3'

    if (!(controller_DW.UnitDelay3_DSTATE != 0.0F)) {
      controller_DW.ComputeTrigger_MODE = true;

      // RelationalOperator: '<S17>/Compare' incorporates:
      //   Constant: '<S17>/Constant'

      controller_B.Compare_m = (controller_B.HSCLP[1] <=
        controller_P.pressure_thres_const);
    } else {
      if (controller_DW.ComputeTrigger_MODE) {
        // Disable for Outport: '<S8>/Out'
        controller_B.Compare_m = controller_P.Out_Y0;
        controller_DW.ComputeTrigger_MODE = false;
      }
    }

    // End of Logic: '<S1>/Logical Operator'
    // End of Outputs for SubSystem: '<S1>/Compute Trigger'

    // Chart: '<S1>/Pulse Generator' incorporates:
    //   Constant: '<S1>/IE_ratio'
    //   Constant: '<S1>/enable_trigger'
    //   Constant: '<S1>/respiratory_rate'

    controller_DW.presentTicks = (&controller_M)->Timing.clockTick0;
    controller_DW.elapsedTicks = controller_DW.presentTicks -
      controller_DW.previousTicks;
    controller_DW.previousTicks = controller_DW.presentTicks;
    controller_DW.temporalCounter_i1 += controller_DW.elapsedTicks;
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
          (controller_DW.temporalCounter_i1 >= 1.0F /
           (controller_P.IE_ratio_Value + 1.0F) * (60.0F /
            controller_P.respiratory_rate_Value) * 100.0F)) {
        controller_DW.is_c3_controller = controller_IN_INSPIRATION;
        controller_DW.temporalCounter_i1 = 0U;
        controller_B.pulse = 1.0F;
      }
      break;

     case controller_IN_INSPIRATION:
      controller_B.pulse = 1.0F;
      if (controller_DW.control_mode_prev != controller_DW.control_mode_start) {
        controller_DW.is_c3_controller = controller_IN_INSPIRATION;
        controller_DW.temporalCounter_i1 = 0U;
        controller_B.pulse = 1.0F;
      } else if (controller_DW.temporalCounter_i1 >= controller_P.IE_ratio_Value
                 / (controller_P.IE_ratio_Value + 1.0F) * (60.0F /
                  controller_P.respiratory_rate_Value) * 100.0F) {
        controller_DW.is_c3_controller = controller_IN_EXPIRATION;
        controller_DW.temporalCounter_i1 = 0U;
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
      if (controller_DW.temporalCounter_i1 >= controller_P.IE_ratio_Value /
          (controller_P.IE_ratio_Value + 1.0F) * (60.0F /
           controller_P.respiratory_rate_Value) * 100.0F) {
        controller_DW.is_c3_controller = controller_IN_TRIGGER_WAIT;
        controller_B.pulse = 0.0F;
      } else {
        if (!controller_P.enable_trigger_Value) {
          controller_DW.is_c3_controller = controller_IN_INSPIRATION;
          controller_DW.temporalCounter_i1 = 0U;
          controller_B.pulse = 1.0F;
        }
      }
      break;

     default:
      // case IN_TRIGGER_WAIT:
      controller_B.pulse = 0.0F;
      if (controller_B.Compare_m) {
        controller_DW.is_c3_controller = controll_IN_TRIGGER_INSPIRATION;
        controller_DW.temporalCounter_i1 = 0U;
        controller_B.pulse = 1.0F;
      } else {
        if (!controller_P.enable_trigger_Value) {
          controller_DW.is_c3_controller = controller_IN_INSPIRATION;
          controller_DW.temporalCounter_i1 = 0U;
          controller_B.pulse = 1.0F;
        }
      }
      break;
    }

    // Switch: '<S1>/Switch4' incorporates:
    //   Constant: '<S1>/Constant8'

    if (rtb_Compare_k) {
      rtb_Divide1 = controller_P.Constant8_Value;
    } else {
      rtb_Divide1 = controller_B.pulse;
    }

    // End of Switch: '<S1>/Switch4'

    // Outputs for Enabled SubSystem: '<S1>/Pressure Control' incorporates:
    //   EnablePort: '<S10>/Enable'

    // Logic: '<S1>/Logical Operator2' incorporates:
    //   Logic: '<S10>/Logical Operator'

    if (rtb_Compare_k || rtb_Compare_p) {
      if (!controller_DW.PressureControl_MODE) {
        // InitializeConditions for DiscreteFilter: '<S10>/Input Shaping'
        controller_DW.InputShaping_icLoad = 1U;

        // InitializeConditions for UnitDelay: '<S10>/Unit Delay'
        controller_DW.UnitDelay_DSTATE_o =
          controller_P.UnitDelay_InitialCondition;

        // InitializeConditions for DiscreteIntegrator: '<S52>/Integrator'
        controller_DW.Integrator_DSTATE_p =
          controller_P.PIDController_InitialConditionF;
        controller_DW.Integrator_PrevResetState_f = 2;
        controller_DW.PressureControl_MODE = true;
      }

      // Gain: '<S10>/PIP'
      rtb_PIP = controller_P.PIP_Gain * rtb_Divide1;

      // Gain: '<S18>/Gain' incorporates:
      //   Constant: '<S18>/Constant 1'
      //   Constant: '<S18>/PIP_rise_perc'
      //   Constant: '<S1>/IE_ratio'
      //   Constant: '<S1>/respiratory_rate'
      //   Fcn: '<S18>/fun1'
      //   Product: '<S18>/Product2'

      rtb_TmpSignalConversionAtHSCL_3 = controller_P.PIP_rise_perc_Value *
        controller_P.Constant1_Value / controller_P.respiratory_rate_Value *
        (controller_P.IE_ratio_Value / (controller_P.IE_ratio_Value + 1.0F)) *
        controller_P.Gain_Gain;

      // Math: '<S18>/Math Function'
      rtb_Switch3 = rtb_TmpSignalConversionAtHSCL_3 *
        rtb_TmpSignalConversionAtHSCL_3;

      // Product: '<S18>/Product1' incorporates:
      //   Constant: '<S18>/Constant'
      //   Constant: '<S18>/Constant1'
      //   Product: '<S18>/Divide'
      //   Product: '<S18>/Product'

      rtb_TmpSignalConversionAtHSCL_0 = controller_P.Constant_Value_e[0] /
        (rtb_TmpSignalConversionAtHSCL_3 * rtb_Switch3) *
        controller_P.Constant1_Value_n[0];
      rtb_TmpSignalConversionAtHSCL_1 = controller_P.Constant_Value_e[1] /
        rtb_Switch3 * controller_P.Constant1_Value_n[1];
      rtb_pnm_idx_2 = controller_P.Constant_Value_e[2] /
        rtb_TmpSignalConversionAtHSCL_3 * controller_P.Constant1_Value_n[2];

      // Fcn: '<S18>/a0'
      rtb_Divide5 = ((rtb_pnm_idx_2 + rtb_TmpSignalConversionAtHSCL_1) +
                     rtb_TmpSignalConversionAtHSCL_0) - 8.0F;

      // Product: '<S18>/Divide1' incorporates:
      //   Fcn: '<S18>/b0_3'

      rtb_TmpSignalConversionAtHSCL_3 = rtb_TmpSignalConversionAtHSCL_0 /
        rtb_Divide5;

      // Product: '<S18>/Divide2' incorporates:
      //   Fcn: '<S18>/b1_2'

      rtb_Switch3 = 3.0F * rtb_TmpSignalConversionAtHSCL_0 / rtb_Divide5;

      // Product: '<S18>/Divide3' incorporates:
      //   Fcn: '<S18>/a1'

      rtb_Divide3 = (((-rtb_pnm_idx_2 + rtb_TmpSignalConversionAtHSCL_1) + 3.0F *
                      rtb_TmpSignalConversionAtHSCL_0) + 24.0F) / rtb_Divide5;

      // Product: '<S18>/Divide4' incorporates:
      //   Fcn: '<S18>/a2'

      rtb_SumFdbk = (((-rtb_pnm_idx_2 - rtb_TmpSignalConversionAtHSCL_1) + 3.0F *
                      rtb_TmpSignalConversionAtHSCL_0) - 24.0F) / rtb_Divide5;

      // Product: '<S18>/Divide5' incorporates:
      //   Fcn: '<S18>/a3'

      rtb_Divide5 = (((rtb_pnm_idx_2 - rtb_TmpSignalConversionAtHSCL_1) +
                      rtb_TmpSignalConversionAtHSCL_0) + 8.0F) / rtb_Divide5;

      // SignalConversion generated from: '<S10>/Input Shaping'
      controller_B.TmpSignalConversionAtInputShapi[0] =
        rtb_TmpSignalConversionAtHSCL_3;
      controller_B.TmpSignalConversionAtInputShapi[1] = rtb_Switch3;
      controller_B.TmpSignalConversionAtInputShapi[2] = rtb_Switch3;
      controller_B.TmpSignalConversionAtInputShapi[3] =
        rtb_TmpSignalConversionAtHSCL_3;

      // SignalConversion generated from: '<S10>/Input Shaping' incorporates:
      //   Constant: '<S18>/Constant2'

      controller_B.TmpSignalConversionAtInputSha_h[0] =
        controller_P.Constant2_Value;
      controller_B.TmpSignalConversionAtInputSha_h[1] = rtb_Divide3;
      controller_B.TmpSignalConversionAtInputSha_h[2] = rtb_SumFdbk;
      controller_B.TmpSignalConversionAtInputSha_h[3] = rtb_Divide5;

      // Product: '<S10>/Product' incorporates:
      //   Fcn: '<S18>/fun'
      //   Gain: '<S10>/Gain3'
      //   Gain: '<S10>/PIP_step_perc'
      //   Sum: '<S10>/Sum3'
      //   Sum: '<S10>/Sum4'

      rtb_Divide5 = (rtb_Divide3 - rtb_TmpSignalConversionAtHSCL_3) /
        (((rtb_Divide3 + rtb_SumFdbk) + rtb_Divide5) * rtb_Switch3 + (2.0F *
          rtb_Switch3 + rtb_TmpSignalConversionAtHSCL_3) * rtb_Divide3) *
        ((rtb_PIP - controller_B.HSCLP[0]) * controller_P.PIP_step_perc_Gain *
         controller_P.Gain3_Gain + controller_B.HSCLP[0]);

      // DiscreteFilter: '<S10>/Input Shaping'
      if (rt_R32ZCFcn(RISING_ZERO_CROSSING,
                      &controller_PrevZCX.InputShaping_Reset_ZCE,
                      (rtb_Divide1)) != NO_ZCEVENT) {
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
      rtb_TmpSignalConversionAtHSCL_3 =
        ((controller_B.TmpSignalConversionAtInputShapi[0] *
          controller_DW.InputShaping_tmp +
          controller_B.TmpSignalConversionAtInputShapi[1] *
          controller_DW.InputShaping_states[0]) +
         controller_B.TmpSignalConversionAtInputShapi[2] *
         controller_DW.InputShaping_states[1]) +
        controller_B.TmpSignalConversionAtInputShapi[3] *
        controller_DW.InputShaping_states[2];

      // End of DiscreteFilter: '<S10>/Input Shaping'

      // Sum: '<S10>/Sum1'
      rtb_Divide5 = rtb_TmpSignalConversionAtHSCL_3 - controller_B.HSCLP[0];

      // Outputs for Enabled SubSystem: '<S10>/Params Selector' incorporates:
      //   EnablePort: '<S20>/Enable'

      if (!(rtb_Divide1 != 0.0F)) {
        // Switch: '<S20>/Switch2' incorporates:
        //   Constant: '<S20>/PC_FFW'
        //   Constant: '<S20>/PC_FFW_rec'
        //   Constant: '<S20>/PC_I'
        //   Constant: '<S20>/PC_I_rec'
        //   Constant: '<S20>/PC_P'
        //   Constant: '<S20>/PC_P_rec'

        if (controller_B.HSCLP[3] > controller_P.Switch2_Threshold) {
          controller_B.Switch2[0] = controller_P.PC_P_rec_Value;
          controller_B.Switch2[1] = controller_P.PC_I_rec_Value;
          controller_B.Switch2[2] = controller_P.PC_FFW_rec_Value;
        } else {
          controller_B.Switch2[0] = controller_P.PC_P_Value;
          controller_B.Switch2[1] = controller_P.PC_I_Value;
          controller_B.Switch2[2] = controller_P.PC_FFW_Value;
        }

        // End of Switch: '<S20>/Switch2'

        // SignalConversion generated from: '<S20>/FFW'
        controller_B.OutportBufferForFFW = controller_B.Switch2[2];

        // SignalConversion generated from: '<S20>/I'
        controller_B.OutportBufferForI = controller_B.Switch2[1];
      }

      // End of Outputs for SubSystem: '<S10>/Params Selector'

      // Product: '<S49>/IProd Out' incorporates:
      //   Logic: '<S10>/Logical Operator'

      rtb_PIP = rtb_Divide5 * controller_B.OutportBufferForI;

      // Product: '<S57>/PProd Out'
      rtb_Divide5 *= controller_B.Switch2[0];

      // Sum: '<S64>/SumI1' incorporates:
      //   Gain: '<S63>/Kt'
      //   Sum: '<S10>/Sum5'
      //   Sum: '<S62>/Sum Fdbk'
      //   Sum: '<S63>/SumI3'
      //   UnitDelay: '<S10>/Unit Delay'

      rtb_PIP += ((controller_DW.UnitDelay_DSTATE_o -
                   controller_B.OutportBufferForFFW) - (rtb_Divide5 +
        controller_DW.Integrator_DSTATE_p)) * controller_P.PIDController_Kt;

      // DiscreteIntegrator: '<S52>/Integrator'
      if ((rtb_Divide1 > 0.0F) && (controller_DW.Integrator_PrevResetState_f <=
           0)) {
        controller_DW.Integrator_DSTATE_p =
          controller_P.PIDController_InitialConditionF;
      }

      controller_B.Integrator_m = controller_P.Integrator_gainval * rtb_PIP +
        controller_DW.Integrator_DSTATE_p;

      // End of DiscreteIntegrator: '<S52>/Integrator'

      // Sum: '<S10>/Sum2' incorporates:
      //   Sum: '<S61>/Sum'

      rtb_Divide5 = (rtb_Divide5 + controller_B.Integrator_m) +
        controller_B.OutportBufferForFFW;

      // Saturate: '<S10>/Saturation'
      if (rtb_Divide5 > controller_P.Saturation_UpperSat) {
        rtb_Divide5 = controller_P.Saturation_UpperSat;
      } else {
        if (rtb_Divide5 < controller_P.Saturation_LowerSat) {
          rtb_Divide5 = controller_P.Saturation_LowerSat;
        }
      }

      // End of Saturate: '<S10>/Saturation'

      // Switch: '<S10>/Switch' incorporates:
      //   Constant: '<S10>/Constant'

      if (rtb_Divide1 > controller_P.Switch_Threshold) {
        controller_B.Switch_n = rtb_Divide5;
      } else {
        controller_B.Switch_n = controller_P.Constant_Value;
      }

      // End of Switch: '<S10>/Switch'

      // Switch: '<S10>/Switch1' incorporates:
      //   Constant: '<S10>/Constant'

      if (rtb_Divide1 > controller_P.Switch1_Threshold) {
        controller_B.Switch1 = rtb_TmpSignalConversionAtHSCL_3;
      } else {
        controller_B.Switch1 = controller_P.Constant_Value;
      }

      // End of Switch: '<S10>/Switch1'
    } else {
      if (controller_DW.PressureControl_MODE) {
        // Disable for Outport: '<S10>/P_ref'
        controller_B.Switch1 = controller_P.P_ref_Y0;

        // Disable for Outport: '<S10>/IV_dc'
        controller_B.Switch_n = controller_P.IV_dc_Y0;
        controller_DW.PressureControl_MODE = false;
      }
    }

    // End of Logic: '<S1>/Logical Operator2'
    // End of Outputs for SubSystem: '<S1>/Pressure Control'

    // Outputs for Enabled SubSystem: '<S1>/Volume Control' incorporates:
    //   EnablePort: '<S16>/Enable'

    if (rtb_Compare) {
      if (!controller_DW.VolumeControl_MODE) {
        // InitializeConditions for UnitDelay: '<S16>/Unit Delay'
        controller_DW.UnitDelay_DSTATE_n =
          controller_P.UnitDelay_InitialCondition_n;

        // InitializeConditions for DiscreteIntegrator: '<S104>/Integrator'
        controller_DW.Integrator_DSTATE =
          controller_P.PIDController_InitialConditio_c;
        controller_DW.Integrator_PrevResetState = 2;
        controller_DW.VolumeControl_MODE = true;
      }

      // Gain: '<S16>/Volumetric Flow rate' incorporates:
      //   Gain: '<S1>/Tidal Volume'

      controller_B.VolumetricFlowrate = controller_P.TidalVolume_Gain *
        rtb_Divide1 * controller_P.VolumetricFlowrate_Gain;

      // Outputs for Triggered SubSystem: '<S16>/Store FFW' incorporates:
      //   TriggerPort: '<S72>/Trigger'

      zcEvent = rt_R32ZCFcn(FALLING_ZERO_CROSSING,
                            &controller_PrevZCX.StoreFFW_Trig_ZCE,
                            (controller_B.VolumetricFlowrate));
      if (zcEvent != NO_ZCEVENT) {
        // DataStoreWrite: '<S72>/Data Store Write' incorporates:
        //   UnitDelay: '<S16>/Unit Delay'

        controller_DW.FFW = controller_DW.UnitDelay_DSTATE_n;
      }

      // End of Outputs for SubSystem: '<S16>/Store FFW'

      // Sum: '<S16>/Sum1'
      rtb_Switch3 = controller_B.VolumetricFlowrate - controller_B.Product1[0];

      // Switch: '<S70>/Switch2' incorporates:
      //   Constant: '<S70>/VC_I'
      //   Constant: '<S70>/VC_I_rec'
      //   Constant: '<S70>/VC_P'
      //   Constant: '<S70>/VC_P_rec'

      if (controller_B.HSCLP[3] > controller_P.Switch2_Threshold_b) {
        rtb_TmpSignalConversionAtHSCL_0 = controller_P.VC_P_rec_Value;
        rtb_TmpSignalConversionAtHSCL_3 = controller_P.VC_I_rec_Value;
      } else {
        rtb_TmpSignalConversionAtHSCL_0 = controller_P.VC_P_Value;
        rtb_TmpSignalConversionAtHSCL_3 = controller_P.VC_I_Value;
      }

      // End of Switch: '<S70>/Switch2'

      // Product: '<S101>/IProd Out'
      rtb_TmpSignalConversionAtHSCL_3 *= rtb_Switch3;

      // Product: '<S109>/PProd Out'
      rtb_Switch3 *= rtb_TmpSignalConversionAtHSCL_0;

      // Outputs for Enabled SubSystem: '<S16>/Read FFW' incorporates:
      //   EnablePort: '<S71>/Enable'

      if (controller_B.VolumetricFlowrate > 0.0F) {
        controller_DW.ReadFFW_MODE = true;

        // DataStoreRead: '<S71>/Data Store Read'
        controller_B.DataStoreRead = controller_DW.FFW;
      } else {
        if (controller_DW.ReadFFW_MODE) {
          // Disable for Outport: '<S71>/ffw'
          controller_B.DataStoreRead = controller_P.ffw_Y0;
          controller_DW.ReadFFW_MODE = false;
        }
      }

      // End of Outputs for SubSystem: '<S16>/Read FFW'

      // Sum: '<S116>/SumI1' incorporates:
      //   Gain: '<S115>/Kt'
      //   Sum: '<S114>/Sum Fdbk'
      //   Sum: '<S115>/SumI3'
      //   Sum: '<S16>/Sum3'
      //   UnitDelay: '<S16>/Unit Delay'

      rtb_TmpSignalConversionAtHSCL_3 += ((controller_DW.UnitDelay_DSTATE_n -
        controller_B.DataStoreRead) - (rtb_Switch3 +
        controller_DW.Integrator_DSTATE)) * controller_P.PIDController_Kt_j;

      // DiscreteIntegrator: '<S104>/Integrator'
      if ((controller_B.VolumetricFlowrate <= 0.0F) &&
          (controller_DW.Integrator_PrevResetState == 1)) {
        controller_DW.Integrator_DSTATE =
          controller_P.PIDController_InitialConditio_c;
      }

      controller_B.Integrator = controller_P.Integrator_gainval_m *
        rtb_TmpSignalConversionAtHSCL_3 + controller_DW.Integrator_DSTATE;

      // End of DiscreteIntegrator: '<S104>/Integrator'

      // Sum: '<S16>/Sum2' incorporates:
      //   Sum: '<S113>/Sum'

      rtb_Switch3 = (rtb_Switch3 + controller_B.Integrator) +
        controller_B.DataStoreRead;

      // Saturate: '<S16>/Saturation'
      if (rtb_Switch3 > controller_P.Saturation_UpperSat_b) {
        controller_B.Saturation = controller_P.Saturation_UpperSat_b;
      } else if (rtb_Switch3 < controller_P.Saturation_LowerSat_f) {
        controller_B.Saturation = controller_P.Saturation_LowerSat_f;
      } else {
        controller_B.Saturation = rtb_Switch3;
      }

      // End of Saturate: '<S16>/Saturation'

      // Update for UnitDelay: '<S16>/Unit Delay'
      controller_DW.UnitDelay_DSTATE_n = controller_B.Saturation;

      // Update for DiscreteIntegrator: '<S104>/Integrator'
      controller_DW.Integrator_DSTATE = controller_P.Integrator_gainval_m *
        rtb_TmpSignalConversionAtHSCL_3 + controller_B.Integrator;
      if (controller_B.VolumetricFlowrate > 0.0F) {
        controller_DW.Integrator_PrevResetState = 1;
      } else if (controller_B.VolumetricFlowrate < 0.0F) {
        controller_DW.Integrator_PrevResetState = -1;
      } else if (controller_B.VolumetricFlowrate == 0.0F) {
        controller_DW.Integrator_PrevResetState = 0;
      } else {
        controller_DW.Integrator_PrevResetState = 2;
      }

      // End of Update for DiscreteIntegrator: '<S104>/Integrator'

      // Switch: '<S1>/Switch' incorporates:
      //   Outport: '<Root>/IV_dc'

      controller_Y.IV_dc = controller_B.Saturation;
    } else {
      if (controller_DW.VolumeControl_MODE) {
        // Disable for Enabled SubSystem: '<S16>/Read FFW'
        if (controller_DW.ReadFFW_MODE) {
          // Disable for Outport: '<S71>/ffw'
          controller_B.DataStoreRead = controller_P.ffw_Y0;
          controller_DW.ReadFFW_MODE = false;
        }

        // End of Disable for SubSystem: '<S16>/Read FFW'

        // Disable for Outport: '<S16>/Q_ref'
        controller_B.VolumetricFlowrate = controller_P.Q_ref_Y0;

        // Disable for Outport: '<S16>/IV_dc'
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

    // Switch: '<S1>/Switch3' incorporates:
    //   Constant: '<S1>/Constant'
    //   Constant: '<S3>/Constant'
    //   Logic: '<S1>/Logical Operator3'
    //   RelationalOperator: '<S3>/Compare'
    //   Sum: '<S1>/Sum'

    if ((controller_P.Controller_control_mode ==
         controller_P.CompareToConstant_const) || rtb_Compare_k) {
      rtb_Switch3 = controller_P.Constant_Value_a;
    } else {
      rtb_Switch3 = controller_P.Constant_Value_a - rtb_Divide1;
    }

    // End of Switch: '<S1>/Switch3'

    // Outport: '<Root>/OV_cmd' incorporates:
    //   Constant: '<S7>/Constant'
    //   RelationalOperator: '<S7>/Compare'

    controller_Y.OV_cmd = (rtb_Switch3 > controller_P.Constant_Value_p);

    // DataTypeConversion: '<S1>/Data Type Conversion'
    controller_B.DataTypeConversion = controller_B.Compare_m;

    // Update for UnitDelay: '<S1>/Unit Delay1' incorporates:
    //   Inport: '<Root>/S1_Pi'
    //   Inport: '<Root>/S3_i'
    //   Inport: '<Root>/S4_Pi'
    //   Inport: '<Root>/S7_Pi'

    controller_DW.UnitDelay1_DSTATE[0] = controller_U.S1_Pi;
    controller_DW.UnitDelay1_DSTATE[1] = controller_U.S3_i[0];
    controller_DW.UnitDelay1_DSTATE[2] = controller_U.S4_Pi;
    controller_DW.UnitDelay1_DSTATE[3] = controller_U.S7_Pi;

    // Update for UnitDelay: '<S1>/Unit Delay' incorporates:
    //   Inport: '<Root>/S2_Qi'
    //   Inport: '<Root>/S5_Qi'

    controller_DW.UnitDelay_DSTATE[0] = controller_U.S2_Qi;
    controller_DW.UnitDelay_DSTATE[1] = controller_U.S5_Qi;

    // Update for UnitDelay: '<S1>/Unit Delay2' incorporates:
    //   Inport: '<Root>/S3_i'

    controller_DW.UnitDelay2_DSTATE = controller_U.S3_i[1];

    // Update for DiscreteIntegrator: '<S1>/Discrete-Time Integrator'
    controller_DW.DiscreteTimeIntegrator_DSTATE =
      rtb_TmpSignalConversionAtHSCL_2 + controller_B.DiscreteTimeIntegrator;
    controller_DW.DiscreteTimeIntegrator_PrevRese = static_cast<int8_T>
      (rtb_LogicalOperator1);

    // Update for UnitDelay: '<S1>/Unit Delay3'
    controller_DW.UnitDelay3_DSTATE = rtb_Divide1;

    // Update for Enabled SubSystem: '<S1>/Pressure Control' incorporates:
    //   EnablePort: '<S10>/Enable'

    if (controller_DW.PressureControl_MODE) {
      // Update for DiscreteFilter: '<S10>/Input Shaping'
      controller_DW.InputShaping_icLoad = 0U;
      controller_DW.InputShaping_states[2] = controller_DW.InputShaping_states[1];
      controller_DW.InputShaping_states[1] = controller_DW.InputShaping_states[0];
      controller_DW.InputShaping_states[0] = controller_DW.InputShaping_tmp;

      // Update for UnitDelay: '<S10>/Unit Delay'
      controller_DW.UnitDelay_DSTATE_o = rtb_Divide5;

      // Update for DiscreteIntegrator: '<S52>/Integrator'
      controller_DW.Integrator_DSTATE_p = controller_P.Integrator_gainval *
        rtb_PIP + controller_B.Integrator_m;
      if (rtb_Divide1 > 0.0F) {
        controller_DW.Integrator_PrevResetState_f = 1;
      } else if (rtb_Divide1 < 0.0F) {
        controller_DW.Integrator_PrevResetState_f = -1;
      } else if (rtb_Divide1 == 0.0F) {
        controller_DW.Integrator_PrevResetState_f = 0;
      } else {
        controller_DW.Integrator_PrevResetState_f = 2;
      }

      // End of Update for DiscreteIntegrator: '<S52>/Integrator'
    }

    // End of Update for SubSystem: '<S1>/Pressure Control'
  } else {
    if (controller_DW.Controller_MODE) {
      // Disable for Enabled SubSystem: '<S1>/Compute Trigger'
      if (controller_DW.ComputeTrigger_MODE) {
        // Disable for Outport: '<S8>/Out'
        controller_B.Compare_m = controller_P.Out_Y0;
        controller_DW.ComputeTrigger_MODE = false;
      }

      // End of Disable for SubSystem: '<S1>/Compute Trigger'

      // Disable for Chart: '<S1>/Pulse Generator'
      controller_DW.presentTicks = (&controller_M)->Timing.clockTick0;
      controller_DW.elapsedTicks = controller_DW.presentTicks -
        controller_DW.previousTicks;
      controller_DW.previousTicks = controller_DW.presentTicks;
      controller_DW.temporalCounter_i1 += controller_DW.elapsedTicks;

      // Disable for Enabled SubSystem: '<S1>/Pressure Control'
      if (controller_DW.PressureControl_MODE) {
        // Disable for Outport: '<S10>/P_ref'
        controller_B.Switch1 = controller_P.P_ref_Y0;

        // Disable for Outport: '<S10>/IV_dc'
        controller_B.Switch_n = controller_P.IV_dc_Y0;
        controller_DW.PressureControl_MODE = false;
      }

      // End of Disable for SubSystem: '<S1>/Pressure Control'

      // Disable for Enabled SubSystem: '<S1>/Volume Control'
      if (controller_DW.VolumeControl_MODE) {
        // Disable for Enabled SubSystem: '<S16>/Read FFW'
        if (controller_DW.ReadFFW_MODE) {
          // Disable for Outport: '<S71>/ffw'
          controller_B.DataStoreRead = controller_P.ffw_Y0;
          controller_DW.ReadFFW_MODE = false;
        }

        // End of Disable for SubSystem: '<S16>/Read FFW'

        // Disable for Outport: '<S16>/Q_ref'
        controller_B.VolumetricFlowrate = controller_P.Q_ref_Y0;

        // Disable for Outport: '<S16>/IV_dc'
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

      // Disable for Outport: '<S1>/signals'
      controller_B.HSCLP[0] = controller_P.signals_Y0;
      controller_B.Product1[0] = controller_P.signals_Y0;
      controller_B.HSCLP[1] = controller_P.signals_Y0;
      controller_B.Bias = controller_P.signals_Y0;
      controller_B.HSCLP[2] = controller_P.signals_Y0;
      controller_B.Gain = controller_P.signals_Y0;
      controller_B.HSCLP[3] = controller_P.signals_Y0;
      controller_B.DiscreteTimeIntegrator = controller_P.signals_Y0;
      controller_B.DataTypeConversion = controller_P.signals_Y0;
      controller_DW.Controller_MODE = false;
    }
  }

  // End of Inport: '<Root>/enable'
  // End of Outputs for SubSystem: '<Root>/Controller'

  // Outport: '<Root>/signals'
  controller_Y.signals[0] = controller_B.HSCLP[0];
  controller_Y.signals[1] = controller_B.Product1[0];
  controller_Y.signals[2] = controller_B.HSCLP[1];
  controller_Y.signals[3] = controller_B.Bias;
  controller_Y.signals[4] = controller_B.HSCLP[2];
  controller_Y.signals[5] = controller_B.Gain;
  controller_Y.signals[6] = controller_B.HSCLP[3];
  controller_Y.signals[7] = controller_B.DiscreteTimeIntegrator;
  controller_Y.signals[8] = controller_B.DataTypeConversion;

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

    // InitializeConditions for UnitDelay: '<S1>/Unit Delay'
    controller_DW.UnitDelay_DSTATE[0] =
      controller_P.UnitDelay_InitialCondition_g;
    controller_DW.UnitDelay_DSTATE[1] =
      controller_P.UnitDelay_InitialCondition_g;

    // InitializeConditions for UnitDelay: '<S1>/Unit Delay2'
    controller_DW.UnitDelay2_DSTATE = controller_P.UnitDelay2_InitialCondition;

    // InitializeConditions for DiscreteIntegrator: '<S1>/Discrete-Time Integrator' 
    controller_DW.DiscreteTimeIntegrator_DSTATE =
      controller_P.DiscreteTimeIntegrator_IC;
    controller_DW.DiscreteTimeIntegrator_PrevRese = 2;

    // InitializeConditions for UnitDelay: '<S1>/Unit Delay3'
    controller_DW.UnitDelay3_DSTATE = controller_P.UnitDelay3_InitialCondition;

    // SystemInitialize for Enabled SubSystem: '<S1>/Compute Trigger'
    // SystemInitialize for Outport: '<S8>/Out'
    controller_B.Compare_m = controller_P.Out_Y0;

    // End of SystemInitialize for SubSystem: '<S1>/Compute Trigger'

    // Chart: '<S1>/Pulse Generator'
    controller_DW.is_c3_controller = controller_IN_INSPIRATION;
    controller_B.pulse = 1.0F;

    // SystemInitialize for Enabled SubSystem: '<S1>/Pressure Control'
    // InitializeConditions for DiscreteFilter: '<S10>/Input Shaping'
    controller_DW.InputShaping_icLoad = 1U;

    // InitializeConditions for UnitDelay: '<S10>/Unit Delay'
    controller_DW.UnitDelay_DSTATE_o = controller_P.UnitDelay_InitialCondition;

    // InitializeConditions for DiscreteIntegrator: '<S52>/Integrator'
    controller_DW.Integrator_DSTATE_p =
      controller_P.PIDController_InitialConditionF;
    controller_DW.Integrator_PrevResetState_f = 2;

    // SystemInitialize for Enabled SubSystem: '<S10>/Params Selector'
    // SystemInitialize for Outport: '<S20>/P'
    controller_B.Switch2[0] = controller_P.P_Y0;

    // SystemInitialize for Outport: '<S20>/I'
    controller_B.OutportBufferForI = controller_P.I_Y0;

    // SystemInitialize for Outport: '<S20>/FFW'
    controller_B.OutportBufferForFFW = controller_P.FFW_Y0;

    // End of SystemInitialize for SubSystem: '<S10>/Params Selector'

    // SystemInitialize for Outport: '<S10>/P_ref'
    controller_B.Switch1 = controller_P.P_ref_Y0;

    // SystemInitialize for Outport: '<S10>/IV_dc'
    controller_B.Switch_n = controller_P.IV_dc_Y0;

    // End of SystemInitialize for SubSystem: '<S1>/Pressure Control'

    // SystemInitialize for Enabled SubSystem: '<S1>/Volume Control'
    // Start for DataStoreMemory: '<S16>/Data Store Memory'
    controller_DW.FFW = controller_P.DataStoreMemory_InitialValue;

    // InitializeConditions for UnitDelay: '<S16>/Unit Delay'
    controller_DW.UnitDelay_DSTATE_n = controller_P.UnitDelay_InitialCondition_n;

    // InitializeConditions for DiscreteIntegrator: '<S104>/Integrator'
    controller_DW.Integrator_DSTATE =
      controller_P.PIDController_InitialConditio_c;
    controller_DW.Integrator_PrevResetState = 2;

    // SystemInitialize for Enabled SubSystem: '<S16>/Read FFW'
    // SystemInitialize for Outport: '<S71>/ffw'
    controller_B.DataStoreRead = controller_P.ffw_Y0;

    // End of SystemInitialize for SubSystem: '<S16>/Read FFW'

    // SystemInitialize for Outport: '<S16>/Q_ref'
    controller_B.VolumetricFlowrate = controller_P.Q_ref_Y0;

    // SystemInitialize for Outport: '<S16>/IV_dc'
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

    // SystemInitialize for Outport: '<S1>/signals'
    controller_B.HSCLP[0] = controller_P.signals_Y0;
    controller_B.Product1[0] = controller_P.signals_Y0;
    controller_B.HSCLP[1] = controller_P.signals_Y0;
    controller_B.Bias = controller_P.signals_Y0;
    controller_B.HSCLP[2] = controller_P.signals_Y0;
    controller_B.Gain = controller_P.signals_Y0;
    controller_B.HSCLP[3] = controller_P.signals_Y0;
    controller_B.DiscreteTimeIntegrator = controller_P.signals_Y0;
    controller_B.DataTypeConversion = controller_P.signals_Y0;

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
