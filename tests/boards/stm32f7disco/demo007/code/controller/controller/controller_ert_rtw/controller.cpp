//
// Non-Degree Granting Education License -- for use at non-degree
// granting, nonprofit, educational organizations only. Not for
// government, commercial, or other organizational use.
//
// File: controller.cpp
//
// Code generated for Simulink model 'controller'.
//
// Model version                  : 1.171
// Simulink Coder version         : 9.3 (R2020a) 18-Nov-2019
// C/C++ source code generated on : Sun Apr 12 12:02:58 2020
//
// Target selection: ert.tlc
// Embedded hardware selection: Intel->x86-64 (Windows64)
// Code generation objectives: Unspecified
// Validation result: Not run
//
#include "controller.h"
#include "controller_private.h"

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
  real32_T rtb_Sum1;
  real32_T rtb_IntegralGain_h;
  ZCEventType zcEvent;
  real_T rtb_PulseGenerator;
  real32_T Integrator_m_tmp;

  // Outputs for Enabled SubSystem: '<Root>/Controller' incorporates:
  //   EnablePort: '<S1>/Enable'

  // Inport: '<Root>/enable'
  if (controller_U.enable) {
    controller_DW.Controller_MODE = true;

    // DiscretePulseGenerator: '<S1>/Pulse Generator'
    rtb_PulseGenerator = (controller_DW.clockTickCounter < std::floor
                          (controller_P.Controller_IE_ratio /
      (controller_P.Controller_IE_ratio + 1.0) * (60.0 /
      controller_P.Controller_respiratory_rate) / 0.01)) &&
      (controller_DW.clockTickCounter >= 0) ? controller_P.PulseGenerator_Amp :
      0.0;
    if (controller_DW.clockTickCounter >= std::floor(60.0 /
         controller_P.Controller_respiratory_rate / 0.01) - 1.0) {
      controller_DW.clockTickCounter = 0;
    } else {
      controller_DW.clockTickCounter++;
    }

    // End of DiscretePulseGenerator: '<S1>/Pulse Generator'

    // Outport: '<Root>/OV_cmd' incorporates:
    //   Constant: '<S1>/Constant'
    //   DataTypeConversion: '<S1>/Data Type Conversion'
    //   Sum: '<S1>/Sum'

    controller_Y.OV_cmd = controller_P.Constant_Value_a - static_cast<real32_T>
      (rtb_PulseGenerator);

    // Gain: '<S5>/Gain1' incorporates:
    //   Bias: '<S5>/Bias'
    //   Gain: '<S5>/Gain'
    //   Inport: '<Root>/params'
    //   Product: '<S5>/Product'
    //   UnitDelay: '<S1>/Unit Delay'

    controller_B.Gain1 = (controller_P.Gain_Gain *
                          controller_DW.UnitDelay_DSTATE +
                          controller_P.Bias_Bias) * controller_U.params[0] *
      controller_P.Gain1_Gain;

    // Outputs for Enabled SubSystem: '<S1>/Volume Control' incorporates:
    //   EnablePort: '<S4>/Enable'

    // Constant: '<S1>/Constant1'
    if (controller_P.Constant1_Value_p) {
      controller_DW.VolumeControl_MODE = true;

      // Gain: '<S4>/Volumetric Flow rate' incorporates:
      //   DataTypeConversion: '<S1>/Data Type Conversion'

      controller_B.VolumetricFlowrate = controller_P.VolumetricFlowrate_Gain *
        static_cast<real32_T>(rtb_PulseGenerator);

      // Outputs for Triggered SubSystem: '<S4>/Store FFW' incorporates:
      //   TriggerPort: '<S57>/Trigger'

      zcEvent = rt_R32ZCFcn(FALLING_ZERO_CROSSING,
                            &controller_PrevZCX.StoreFFW_Trig_ZCE,
                            (controller_B.VolumetricFlowrate));
      if (zcEvent != NO_ZCEVENT) {
        // DataStoreWrite: '<S57>/Data Store Write' incorporates:
        //   UnitDelay: '<S4>/Unit Delay'

        controller_DW.FFW = controller_DW.UnitDelay_DSTATE_n;
      }

      // End of Outputs for SubSystem: '<S4>/Store FFW'

      // Outputs for Enabled SubSystem: '<S4>/Read FFW' incorporates:
      //   EnablePort: '<S56>/Enable'

      if (controller_B.VolumetricFlowrate > 0.0F) {
        controller_DW.ReadFFW_MODE = true;

        // DataStoreRead: '<S56>/Data Store Read'
        controller_B.DataStoreRead = controller_DW.FFW;
      } else {
        if (controller_DW.ReadFFW_MODE) {
          // Disable for Outport: '<S56>/ffw'
          controller_B.DataStoreRead = controller_P.ffw_Y0;
          controller_DW.ReadFFW_MODE = false;
        }
      }

      // End of Outputs for SubSystem: '<S4>/Read FFW'

      // Sum: '<S4>/Sum1'
      rtb_Sum1 = controller_B.VolumetricFlowrate - controller_B.Gain1;

      // DiscreteIntegrator: '<S89>/Integrator' incorporates:
      //   Gain: '<S86>/Integral Gain'

      if ((controller_B.VolumetricFlowrate <= 0.0F) &&
          (controller_DW.Integrator_PrevResetState == 1)) {
        controller_DW.Integrator_DSTATE =
          controller_P.PIDController_InitialConditio_c;
      }

      rtb_IntegralGain_h = controller_P.Integrator_gainval_m *
        (controller_P.Controller_VC_I * rtb_Sum1);
      controller_B.Integrator = rtb_IntegralGain_h +
        controller_DW.Integrator_DSTATE;

      // End of DiscreteIntegrator: '<S89>/Integrator'

      // Sum: '<S4>/Sum2' incorporates:
      //   Gain: '<S94>/Proportional Gain'
      //   Sum: '<S98>/Sum'

      rtb_Sum1 = (controller_P.Controller_VC_P * rtb_Sum1 +
                  controller_B.Integrator) + controller_B.DataStoreRead;

      // Saturate: '<S4>/Saturation'
      if (rtb_Sum1 > controller_P.Saturation_UpperSat_b) {
        controller_B.Saturation = controller_P.Saturation_UpperSat_b;
      } else if (rtb_Sum1 < controller_P.Saturation_LowerSat_f) {
        controller_B.Saturation = controller_P.Saturation_LowerSat_f;
      } else {
        controller_B.Saturation = rtb_Sum1;
      }

      // End of Saturate: '<S4>/Saturation'

      // Update for UnitDelay: '<S4>/Unit Delay'
      controller_DW.UnitDelay_DSTATE_n = controller_B.Saturation;

      // Update for DiscreteIntegrator: '<S89>/Integrator'
      controller_DW.Integrator_DSTATE = rtb_IntegralGain_h +
        controller_B.Integrator;
      if (controller_B.VolumetricFlowrate > 0.0F) {
        controller_DW.Integrator_PrevResetState = 1;
      } else if (controller_B.VolumetricFlowrate < 0.0F) {
        controller_DW.Integrator_PrevResetState = -1;
      } else if (controller_B.VolumetricFlowrate == 0.0F) {
        controller_DW.Integrator_PrevResetState = 0;
      } else {
        controller_DW.Integrator_PrevResetState = 2;
      }
    } else {
      if (controller_DW.VolumeControl_MODE) {
        // Disable for Enabled SubSystem: '<S4>/Read FFW'
        if (controller_DW.ReadFFW_MODE) {
          // Disable for Outport: '<S56>/ffw'
          controller_B.DataStoreRead = controller_P.ffw_Y0;
          controller_DW.ReadFFW_MODE = false;
        }

        // End of Disable for SubSystem: '<S4>/Read FFW'

        // Disable for Outport: '<S4>/IV_Vdot_ref'
        controller_B.VolumetricFlowrate = controller_P.IV_Vdot_ref_Y0;

        // Disable for Outport: '<S4>/IV_dc'
        controller_B.Saturation = controller_P.IV_dc_Y0_m;
        controller_DW.VolumeControl_MODE = false;
      }
    }

    // End of Constant: '<S1>/Constant1'
    // End of Outputs for SubSystem: '<S1>/Volume Control'

    // Gain: '<S1>/cmH2O' incorporates:
    //   Bias: '<S2>/Bias'
    //   Gain: '<S2>/Gain'
    //   Gain: '<S2>/Gain1'
    //   Inport: '<Root>/params'
    //   Product: '<S2>/Product'
    //   UnitDelay: '<S1>/Unit Delay1'

    controller_B.cmH2O = (controller_P.Gain_Gain_p *
                          controller_DW.UnitDelay1_DSTATE +
                          controller_P.Bias_Bias_e) * controller_U.params[1] *
      controller_P.Gain1_Gain_c * controller_P.cmH2O_Gain;

    // MATLABSystem: '<S1>/HSC LP'
    if (controller_DW.obj.FilterObj->isInitialized != 1) {
      controller_DW.obj.FilterObj->isSetupComplete = false;
      controller_DW.obj.FilterObj->isInitialized = 1;
      controller_DW.obj.FilterObj->isSetupComplete = true;

      // System object Initialization function: dsp.BiquadFilter
      controller_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES[0] =
        controller_DW.obj.FilterObj->cSFunObject.P0_ICRTP;
      controller_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES[1] =
        controller_DW.obj.FilterObj->cSFunObject.P0_ICRTP;
      controller_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES[2] =
        controller_DW.obj.FilterObj->cSFunObject.P0_ICRTP;
      controller_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES[3] =
        controller_DW.obj.FilterObj->cSFunObject.P0_ICRTP;
      controller_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES[0] =
        controller_DW.obj.FilterObj->cSFunObject.P5_IC2RTP;
      controller_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES[1] =
        controller_DW.obj.FilterObj->cSFunObject.P5_IC2RTP;
      controller_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES[2] =
        controller_DW.obj.FilterObj->cSFunObject.P5_IC2RTP;
      controller_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES[3] =
        controller_DW.obj.FilterObj->cSFunObject.P5_IC2RTP;
    }

    // System object Outputs function: dsp.BiquadFilter
    rtb_IntegralGain_h = controller_DW.obj.FilterObj->cSFunObject.P1_RTP1COEFF[0]
      * controller_B.cmH2O;
    rtb_IntegralGain_h += controller_DW.obj.FilterObj->cSFunObject.P1_RTP1COEFF
      [1] * controller_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES[0];
    rtb_IntegralGain_h += controller_DW.obj.FilterObj->cSFunObject.P1_RTP1COEFF
      [2] * controller_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES[1];
    rtb_IntegralGain_h -= controller_DW.obj.FilterObj->cSFunObject.P2_RTP2COEFF
      [0] * controller_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES[0];
    rtb_IntegralGain_h -= controller_DW.obj.FilterObj->cSFunObject.P2_RTP2COEFF
      [1] * controller_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES[1];
    controller_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES[1] =
      controller_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES[0];
    controller_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES[0] =
      controller_B.cmH2O;
    controller_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES[1] =
      controller_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES[0];
    controller_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES[0] =
      rtb_IntegralGain_h;
    rtb_Sum1 = rtb_IntegralGain_h;
    rtb_IntegralGain_h *= controller_DW.obj.FilterObj->cSFunObject.P1_RTP1COEFF
      [3];
    rtb_IntegralGain_h += controller_DW.obj.FilterObj->cSFunObject.P1_RTP1COEFF
      [4] * controller_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES[2];
    rtb_IntegralGain_h += controller_DW.obj.FilterObj->cSFunObject.P1_RTP1COEFF
      [5] * controller_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES[3];
    rtb_IntegralGain_h -= controller_DW.obj.FilterObj->cSFunObject.P2_RTP2COEFF
      [2] * controller_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES[2];
    rtb_IntegralGain_h -= controller_DW.obj.FilterObj->cSFunObject.P2_RTP2COEFF
      [3] * controller_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES[3];
    controller_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES[3] =
      controller_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES[2];
    controller_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES[2] = rtb_Sum1;
    controller_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES[3] =
      controller_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES[2];
    controller_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES[2] =
      rtb_IntegralGain_h;
    controller_B.HSCLP = rtb_IntegralGain_h;

    // End of MATLABSystem: '<S1>/HSC LP'

    // Outputs for Enabled SubSystem: '<S1>/Pressure Control' incorporates:
    //   EnablePort: '<S3>/Enable'

    // Constant: '<S1>/Constant2'
    if (controller_P.Constant2_Value) {
      controller_DW.PressureControl_MODE = true;

      // DiscreteFilter: '<S3>/Input Shaping' incorporates:
      //   Constant: '<S3>/Constant1'
      //   DataTypeConversion: '<S1>/Data Type Conversion'
      //   Gain: '<S3>/Gain'

      if (rt_R32ZCFcn(RISING_ZERO_CROSSING,
                      &controller_PrevZCX.InputShaping_Reset_ZCE,
                      (static_cast<real32_T>(rtb_PulseGenerator))) != NO_ZCEVENT)
      {
        controller_DW.InputShaping_icLoad = 1U;
      }

      if (controller_DW.InputShaping_icLoad != 0) {
        controller_DW.InputShaping_states[0] = controller_P.Constant1_Value;
        controller_DW.InputShaping_states[1] = controller_P.Constant1_Value;
        controller_DW.InputShaping_states[2] = controller_P.Constant1_Value;
      }

      controller_DW.InputShaping_tmp = (((controller_P.Controller_PIP *
        static_cast<real32_T>(rtb_PulseGenerator) -
        controller_P.InputShaping_DenCoef[1] *
        controller_DW.InputShaping_states[0]) -
        controller_P.InputShaping_DenCoef[2] *
        controller_DW.InputShaping_states[1]) -
        controller_P.InputShaping_DenCoef[3] *
        controller_DW.InputShaping_states[2]) /
        controller_P.InputShaping_DenCoef[0];
      rtb_IntegralGain_h = ((controller_P.InputShaping_NumCoef[0] *
        controller_DW.InputShaping_tmp + controller_P.InputShaping_NumCoef[1] *
        controller_DW.InputShaping_states[0]) +
                            controller_P.InputShaping_NumCoef[2] *
                            controller_DW.InputShaping_states[1]) +
        controller_P.InputShaping_NumCoef[3] *
        controller_DW.InputShaping_states[2];

      // End of DiscreteFilter: '<S3>/Input Shaping'

      // Sum: '<S3>/Sum1'
      rtb_Sum1 = rtb_IntegralGain_h - controller_B.HSCLP;

      // DiscreteIntegrator: '<S38>/Integrator' incorporates:
      //   DataTypeConversion: '<S1>/Data Type Conversion'
      //   Gain: '<S35>/Integral Gain'

      if ((static_cast<real32_T>(rtb_PulseGenerator) > 0.0F) &&
          (controller_DW.Integrator_PrevResetState_f <= 0)) {
        controller_DW.Integrator_DSTATE_p =
          controller_P.PIDController_InitialConditionF;
      }

      Integrator_m_tmp = controller_P.Integrator_gainval *
        (controller_P.Controller_PC_I * rtb_Sum1);
      controller_B.Integrator_m = Integrator_m_tmp +
        controller_DW.Integrator_DSTATE_p;

      // End of DiscreteIntegrator: '<S38>/Integrator'

      // Switch: '<S3>/Switch' incorporates:
      //   Constant: '<S3>/Constant'
      //   DataTypeConversion: '<S1>/Data Type Conversion'

      if (static_cast<real32_T>(rtb_PulseGenerator) >
          controller_P.Switch_Threshold) {
        // Sum: '<S3>/Sum2' incorporates:
        //   Gain: '<S43>/Proportional Gain'
        //   Sum: '<S47>/Sum'

        rtb_Sum1 = (controller_P.Controller_PC_P * rtb_Sum1 +
                    controller_B.Integrator_m) + controller_P.Controller_PC_FFW;

        // Saturate: '<S3>/Saturation'
        if (rtb_Sum1 > controller_P.Saturation_UpperSat) {
          controller_B.Switch_n = controller_P.Saturation_UpperSat;
        } else if (rtb_Sum1 < controller_P.Saturation_LowerSat) {
          controller_B.Switch_n = controller_P.Saturation_LowerSat;
        } else {
          controller_B.Switch_n = rtb_Sum1;
        }

        // End of Saturate: '<S3>/Saturation'
      } else {
        controller_B.Switch_n = controller_P.Constant_Value;
      }

      // End of Switch: '<S3>/Switch'

      // Switch: '<S3>/Switch1' incorporates:
      //   DataTypeConversion: '<S1>/Data Type Conversion'

      if (static_cast<real32_T>(rtb_PulseGenerator) >
          controller_P.Switch1_Threshold) {
        controller_B.Switch1 = rtb_IntegralGain_h;
      } else {
        controller_B.Switch1 = controller_P.Controller_PEEP;
      }

      // End of Switch: '<S3>/Switch1'

      // Update for DiscreteFilter: '<S3>/Input Shaping'
      controller_DW.InputShaping_icLoad = 0U;
      controller_DW.InputShaping_states[2] = controller_DW.InputShaping_states[1];
      controller_DW.InputShaping_states[1] = controller_DW.InputShaping_states[0];
      controller_DW.InputShaping_states[0] = controller_DW.InputShaping_tmp;

      // Update for DiscreteIntegrator: '<S38>/Integrator' incorporates:
      //   DataTypeConversion: '<S1>/Data Type Conversion'

      controller_DW.Integrator_DSTATE_p = Integrator_m_tmp +
        controller_B.Integrator_m;
      if (static_cast<real32_T>(rtb_PulseGenerator) > 0.0F) {
        controller_DW.Integrator_PrevResetState_f = 1;
      } else if (static_cast<real32_T>(rtb_PulseGenerator) < 0.0F) {
        controller_DW.Integrator_PrevResetState_f = -1;
      } else if (static_cast<real32_T>(rtb_PulseGenerator) == 0.0F) {
        controller_DW.Integrator_PrevResetState_f = 0;
      } else {
        controller_DW.Integrator_PrevResetState_f = 2;
      }
    } else {
      if (controller_DW.PressureControl_MODE) {
        // Disable for Outport: '<S3>/IV_P_ref'
        controller_B.Switch1 = controller_P.IV_P_ref_Y0;

        // Disable for Outport: '<S3>/IV_dc'
        controller_B.Switch_n = controller_P.IV_dc_Y0;
        controller_DW.PressureControl_MODE = false;
      }
    }

    // End of Constant: '<S1>/Constant2'
    // End of Outputs for SubSystem: '<S1>/Pressure Control'

    // Switch: '<S1>/Switch' incorporates:
    //   Constant: '<S1>/Constant3'

    if (controller_P.Constant3_Value) {
      // Outport: '<Root>/IV_dc'
      controller_Y.IV_dc = controller_B.Saturation;
    } else {
      // Outport: '<Root>/IV_dc'
      controller_Y.IV_dc = controller_B.Switch_n;
    }

    // End of Switch: '<S1>/Switch'

    // Outport: '<Root>/IV_P_ref' incorporates:
    //   SignalConversion generated from: '<S1>/IV_P_ref'

    controller_Y.IV_P_ref = controller_B.Switch1;

    // Outport: '<Root>/IV_Vdot_ref' incorporates:
    //   SignalConversion generated from: '<S1>/IV_Vdot_ref'

    controller_Y.IV_Vdot_ref = controller_B.VolumetricFlowrate;

    // Update for UnitDelay: '<S1>/Unit Delay' incorporates:
    //   Inport: '<Root>/IV_Vdot_fbk'

    controller_DW.UnitDelay_DSTATE = controller_U.IV_Vdot_fbk;

    // Update for UnitDelay: '<S1>/Unit Delay1' incorporates:
    //   Inport: '<Root>/IV_P_fbk'

    controller_DW.UnitDelay1_DSTATE = controller_U.IV_P_fbk;
  } else {
    if (controller_DW.Controller_MODE) {
      // Disable for Enabled SubSystem: '<S1>/Volume Control'
      if (controller_DW.VolumeControl_MODE) {
        // Disable for Enabled SubSystem: '<S4>/Read FFW'
        if (controller_DW.ReadFFW_MODE) {
          // Disable for Outport: '<S56>/ffw'
          controller_B.DataStoreRead = controller_P.ffw_Y0;
          controller_DW.ReadFFW_MODE = false;
        }

        // End of Disable for SubSystem: '<S4>/Read FFW'

        // Disable for Outport: '<S4>/IV_Vdot_ref'
        controller_B.VolumetricFlowrate = controller_P.IV_Vdot_ref_Y0;

        // Disable for Outport: '<S4>/IV_dc'
        controller_B.Saturation = controller_P.IV_dc_Y0_m;
        controller_DW.VolumeControl_MODE = false;
      }

      // End of Disable for SubSystem: '<S1>/Volume Control'

      // Disable for Enabled SubSystem: '<S1>/Pressure Control'
      if (controller_DW.PressureControl_MODE) {
        // Disable for Outport: '<S3>/IV_P_ref'
        controller_B.Switch1 = controller_P.IV_P_ref_Y0;

        // Disable for Outport: '<S3>/IV_dc'
        controller_B.Switch_n = controller_P.IV_dc_Y0;
        controller_DW.PressureControl_MODE = false;
      }

      // End of Disable for SubSystem: '<S1>/Pressure Control'

      // Disable for Outport: '<Root>/IV_dc' incorporates:
      //   Outport: '<S1>/IV_dc'

      controller_Y.IV_dc = controller_P.IV_dc_Y0_o;

      // Disable for Outport: '<Root>/IV_Vdot_ref' incorporates:
      //   Outport: '<S1>/IV_Vdot_ref'

      controller_Y.IV_Vdot_ref = controller_P.IV_Vdot_ref_Y0_n;

      // Disable for Outport: '<Root>/IV_P_ref' incorporates:
      //   Outport: '<S1>/IV_P_ref'

      controller_Y.IV_P_ref = controller_P.IV_P_ref_Y0_f;

      // Disable for Outport: '<Root>/OV_cmd' incorporates:
      //   Outport: '<S1>/OV_cmd'

      controller_Y.OV_cmd = controller_P.OV_cmd_Y0;

      // Disable for Outport: '<S1>/signals'
      controller_B.Gain1 = controller_P.signals_Y0[0];
      controller_B.cmH2O = controller_P.signals_Y0[1];
      controller_B.HSCLP = controller_P.signals_Y0[2];
      controller_DW.Controller_MODE = false;
    }
  }

  // End of Inport: '<Root>/enable'
  // End of Outputs for SubSystem: '<Root>/Controller'

  // Outport: '<Root>/signals'
  controller_Y.signals[0] = controller_B.Gain1;
  controller_Y.signals[1] = controller_B.cmH2O;
  controller_Y.signals[2] = controller_B.HSCLP;
}

// Model initialize function
void controllerModelClass::initialize()
{
    controller_U.enable =  true;
  {
    b_dspcodegen_BiquadFilter_con_T *iobj_0;
    int32_T i;
    static const real32_T tmp[6] = { 0.157646582F, -0.16491501F, 0.157646582F,
      0.252874941F, 0.252874941F, 0.0F };

    controller_PrevZCX.InputShaping_Reset_ZCE = UNINITIALIZED_ZCSIG;
    controller_PrevZCX.StoreFFW_Trig_ZCE = UNINITIALIZED_ZCSIG;

    // SystemInitialize for Enabled SubSystem: '<Root>/Controller'
    // InitializeConditions for UnitDelay: '<S1>/Unit Delay'
    controller_DW.UnitDelay_DSTATE = controller_P.UnitDelay_InitialCondition_g;

    // InitializeConditions for UnitDelay: '<S1>/Unit Delay1'
    controller_DW.UnitDelay1_DSTATE = controller_P.UnitDelay1_InitialCondition;

    // SystemInitialize for Enabled SubSystem: '<S1>/Volume Control'
    // Start for DataStoreMemory: '<S4>/Data Store Memory'
    controller_DW.FFW = controller_P.DataStoreMemory_InitialValue;

    // InitializeConditions for UnitDelay: '<S4>/Unit Delay'
    controller_DW.UnitDelay_DSTATE_n = controller_P.UnitDelay_InitialCondition;

    // InitializeConditions for DiscreteIntegrator: '<S89>/Integrator'
    controller_DW.Integrator_DSTATE =
      controller_P.PIDController_InitialConditio_c;
    controller_DW.Integrator_PrevResetState = 2;

    // SystemInitialize for Enabled SubSystem: '<S4>/Read FFW'
    // SystemInitialize for Outport: '<S56>/ffw'
    controller_B.DataStoreRead = controller_P.ffw_Y0;

    // End of SystemInitialize for SubSystem: '<S4>/Read FFW'

    // SystemInitialize for Outport: '<S4>/IV_Vdot_ref'
    controller_B.VolumetricFlowrate = controller_P.IV_Vdot_ref_Y0;

    // SystemInitialize for Outport: '<S4>/IV_dc'
    controller_B.Saturation = controller_P.IV_dc_Y0_m;

    // End of SystemInitialize for SubSystem: '<S1>/Volume Control'

    // SystemInitialize for Enabled SubSystem: '<S1>/Pressure Control'
    // InitializeConditions for DiscreteFilter: '<S3>/Input Shaping'
    controller_DW.InputShaping_icLoad = 1U;

    // InitializeConditions for DiscreteIntegrator: '<S38>/Integrator'
    controller_DW.Integrator_DSTATE_p =
      controller_P.PIDController_InitialConditionF;
    controller_DW.Integrator_PrevResetState_f = 2;

    // SystemInitialize for Outport: '<S3>/IV_P_ref'
    controller_B.Switch1 = controller_P.IV_P_ref_Y0;

    // SystemInitialize for Outport: '<S3>/IV_dc'
    controller_B.Switch_n = controller_P.IV_dc_Y0;

    // End of SystemInitialize for SubSystem: '<S1>/Pressure Control'

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

    iobj_0->cSFunObject.P2_RTP2COEFF[0] = -1.52551329F;
    iobj_0->cSFunObject.P2_RTP2COEFF[1] = 0.775506556F;
    iobj_0->cSFunObject.P2_RTP2COEFF[2] = -0.69577682F;
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
    controller_DW.obj.NumChannels = 1;
    controller_DW.obj.isSetupComplete = true;

    // End of Start for MATLABSystem: '<S1>/HSC LP'

    // InitializeConditions for MATLABSystem: '<S1>/HSC LP'
    if (controller_DW.obj.FilterObj->isInitialized == 1) {
      // System object Initialization function: dsp.BiquadFilter
      controller_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES[0] =
        controller_DW.obj.FilterObj->cSFunObject.P0_ICRTP;
      controller_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES[1] =
        controller_DW.obj.FilterObj->cSFunObject.P0_ICRTP;
      controller_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES[2] =
        controller_DW.obj.FilterObj->cSFunObject.P0_ICRTP;
      controller_DW.obj.FilterObj->cSFunObject.W0_ZERO_STATES[3] =
        controller_DW.obj.FilterObj->cSFunObject.P0_ICRTP;
      controller_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES[0] =
        controller_DW.obj.FilterObj->cSFunObject.P5_IC2RTP;
      controller_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES[1] =
        controller_DW.obj.FilterObj->cSFunObject.P5_IC2RTP;
      controller_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES[2] =
        controller_DW.obj.FilterObj->cSFunObject.P5_IC2RTP;
      controller_DW.obj.FilterObj->cSFunObject.W1_POLE_STATES[3] =
        controller_DW.obj.FilterObj->cSFunObject.P5_IC2RTP;
    }

    // End of InitializeConditions for MATLABSystem: '<S1>/HSC LP'

    // SystemInitialize for Outport: '<Root>/IV_dc' incorporates:
    //   Outport: '<S1>/IV_dc'

    controller_Y.IV_dc = controller_P.IV_dc_Y0_o;

    // SystemInitialize for Outport: '<Root>/IV_Vdot_ref' incorporates:
    //   Outport: '<S1>/IV_Vdot_ref'

    controller_Y.IV_Vdot_ref = controller_P.IV_Vdot_ref_Y0_n;

    // SystemInitialize for Outport: '<Root>/IV_P_ref' incorporates:
    //   Outport: '<S1>/IV_P_ref'

    controller_Y.IV_P_ref = controller_P.IV_P_ref_Y0_f;

    // SystemInitialize for Outport: '<Root>/OV_cmd' incorporates:
    //   Outport: '<S1>/OV_cmd'

    controller_Y.OV_cmd = controller_P.OV_cmd_Y0;

    // SystemInitialize for Outport: '<S1>/signals'
    controller_B.Gain1 = controller_P.signals_Y0[0];
    controller_B.cmH2O = controller_P.signals_Y0[1];
    controller_B.HSCLP = controller_P.signals_Y0[2];

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
