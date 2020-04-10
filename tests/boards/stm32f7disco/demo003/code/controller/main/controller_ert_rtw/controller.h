//
// Non-Degree Granting Education License -- for use at non-degree
// granting, nonprofit, educational organizations only. Not for
// government, commercial, or other organizational use.
//
// File: controller.h
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
#ifndef RTW_HEADER_controller_h_
#define RTW_HEADER_controller_h_
#include <cmath>
#include "rtwtypes.h"
#include "zero_crossing_types.h"
#include "controller_types.h"
#include "rt_r32zcfcn.h"

// Macros for accessing real-time model data structure
#ifndef rtmGetErrorStatus
# define rtmGetErrorStatus(rtm)        ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
# define rtmSetErrorStatus(rtm, val)   ((rtm)->errorStatus = (val))
#endif

// Class declaration for model controller
class controllerModelClass {
  // public data and function members
 public:
  // Block signals (default storage)
  typedef struct {
    real32_T Gain1;                    // '<S5>/Gain1'
    real32_T cmH20;                    // '<S1>/cmH20'
    real32_T VolumetricFlowrate;       // '<S4>/Volumetric Flow rate'
    real32_T Integrator;               // '<S89>/Integrator'
    real32_T Saturation;               // '<S4>/Saturation'
    real32_T DataStoreRead;            // '<S56>/Data Store Read'
    real32_T Integrator_m;             // '<S38>/Integrator'
    real32_T Switch_n;                 // '<S3>/Switch'
    real32_T Switch1;                  // '<S3>/Switch1'
    real32_T HSCLP;                    // '<S1>/HSC LP'
  } B_controller_T;

  // Block states (default storage) for system '<Root>'
  typedef struct {
    b_dspcodegen_BiquadFilter_con_T gobj_1;// '<S1>/HSC LP'
    b_dspcodegen_BiquadFilter_con_T gobj_2;// '<S1>/HSC LP'
    dsp_LowpassFilter_controller_T obj;// '<S1>/HSC LP'
    real32_T UnitDelay_DSTATE;         // '<S1>/Unit Delay'
    real32_T UnitDelay1_DSTATE;        // '<S1>/Unit Delay1'
    real32_T UnitDelay_DSTATE_n;       // '<S4>/Unit Delay'
    real32_T Integrator_DSTATE;        // '<S89>/Integrator'
    real32_T InputShaping_states[3];   // '<S3>/Input Shaping'
    real32_T Integrator_DSTATE_p;      // '<S38>/Integrator'
    real32_T FFW;                      // '<S4>/Data Store Memory'
    real32_T InputShaping_tmp;         // '<S3>/Input Shaping'
    int32_T clockTickCounter;          // '<S1>/Pulse Generator'
    int8_T Integrator_PrevResetState;  // '<S89>/Integrator'
    int8_T Integrator_PrevResetState_f;// '<S38>/Integrator'
    uint8_T InputShaping_icLoad;       // '<S3>/Input Shaping'
    boolean_T Relay_Mode;              // '<S3>/Relay'
    boolean_T objisempty;              // '<S1>/HSC LP'
    boolean_T isInitialized;           // '<S1>/HSC LP'
    boolean_T Controller_MODE;         // '<Root>/Controller'
    boolean_T VolumeControl_MODE;      // '<S1>/Volume Control'
    boolean_T ReadFFW_MODE;            // '<S4>/Read FFW'
    boolean_T PressureControl_MODE;    // '<S1>/Pressure Control'
  } DW_controller_T;

  // Zero-crossing (trigger) state
  typedef struct {
    ZCSigState StoreFFW_Trig_ZCE;      // '<S4>/Store FFW'
    ZCSigState InputShaping_Reset_ZCE; // '<S3>/Input Shaping'
  } PrevZCX_controller_T;

  // External inputs (root inport signals with default storage)
  typedef struct {
    boolean_T enable;                  // '<Root>/enable'
    real32_T IV_Vdot_fbk;              // '<Root>/IV_Vdot_fbk'
    real32_T IV_P_fbk;                 // '<Root>/IV_P_fbk'
    real32_T params[2];                // '<Root>/params'
  } ExtU_controller_T;

  // External outputs (root outports fed by signals with default storage)
  typedef struct {
    real32_T IV_dc;                    // '<Root>/IV_dc'
    real32_T IV_Vdot_ref;              // '<Root>/IV_Vdot_ref'
    real32_T IV_P_ref;                 // '<Root>/IV_P_ref'
    real32_T OV_cmd;                   // '<Root>/OV_cmd'
    real32_T signals[3];               // '<Root>/signals'
  } ExtY_controller_T;

  // Parameters (default storage)
  struct P_controller_T {
    real_T Controller_IE_ratio;        // Mask Parameter: Controller_IE_ratio
                                          //  Referenced by: '<S1>/Pulse Generator'

    real_T Controller_respiratory_rate;
                                  // Mask Parameter: Controller_respiratory_rate
                                     //  Referenced by: '<S1>/Pulse Generator'

    real32_T PIDController_InitialConditionF;
                              // Mask Parameter: PIDController_InitialConditionF
                                 //  Referenced by: '<S38>/Integrator'

    real32_T PIDController_InitialConditio_c;
                              // Mask Parameter: PIDController_InitialConditio_c
                                 //  Referenced by: '<S89>/Integrator'

    real32_T Controller_PC_FFW;        // Mask Parameter: Controller_PC_FFW
                                          //  Referenced by: '<S3>/FFW'

    real32_T Controller_PC_I_dw;       // Mask Parameter: Controller_PC_I_dw
                                          //  Referenced by: '<S3>/Constant5'

    real32_T Controller_PC_I_up;       // Mask Parameter: Controller_PC_I_up
                                          //  Referenced by: '<S3>/Constant4'

    real32_T Controller_PC_P;          // Mask Parameter: Controller_PC_P
                                          //  Referenced by: '<S3>/Constant2'

    real32_T Controller_PEEP;          // Mask Parameter: Controller_PEEP
                                          //  Referenced by: '<S3>/Constant3'

    real32_T Controller_PIP;           // Mask Parameter: Controller_PIP
                                          //  Referenced by: '<S3>/Gain'

    real32_T Controller_VC_I;          // Mask Parameter: Controller_VC_I
                                          //  Referenced by: '<S86>/Integral Gain'

    real32_T Controller_VC_P;          // Mask Parameter: Controller_VC_P
                                          //  Referenced by: '<S94>/Proportional Gain'

    real_T PulseGenerator_Amp;         // Expression: 1
                                          //  Referenced by: '<S1>/Pulse Generator'

    real_T PulseGenerator_PhaseDelay;  // Expression: 0
                                          //  Referenced by: '<S1>/Pulse Generator'

    real32_T Constant_Value;           // Computed Parameter: Constant_Value
                                          //  Referenced by: '<S3>/Constant'

    real32_T Saturation_UpperSat;     // Computed Parameter: Saturation_UpperSat
                                         //  Referenced by: '<S3>/Saturation'

    real32_T Saturation_LowerSat;     // Computed Parameter: Saturation_LowerSat
                                         //  Referenced by: '<S3>/Saturation'

    real32_T IV_P_ref_Y0;              // Computed Parameter: IV_P_ref_Y0
                                          //  Referenced by: '<S3>/IV_P_ref'

    real32_T IV_dc_Y0;                 // Computed Parameter: IV_dc_Y0
                                          //  Referenced by: '<S3>/IV_dc'

    real32_T Constant1_Value;          // Computed Parameter: Constant1_Value
                                          //  Referenced by: '<S3>/Constant1'

    real32_T InputShaping_NumCoef[4];// Computed Parameter: InputShaping_NumCoef
                                        //  Referenced by: '<S3>/Input Shaping'

    real32_T InputShaping_DenCoef[4];// Computed Parameter: InputShaping_DenCoef
                                        //  Referenced by: '<S3>/Input Shaping'

    real32_T Relay_OnVal;              // Computed Parameter: Relay_OnVal
                                          //  Referenced by: '<S3>/Relay'

    real32_T Relay_OffVal;             // Computed Parameter: Relay_OffVal
                                          //  Referenced by: '<S3>/Relay'

    real32_T Relay_YOn;                // Computed Parameter: Relay_YOn
                                          //  Referenced by: '<S3>/Relay'

    real32_T Relay_YOff;               // Computed Parameter: Relay_YOff
                                          //  Referenced by: '<S3>/Relay'

    real32_T Switch2_Threshold;        // Computed Parameter: Switch2_Threshold
                                          //  Referenced by: '<S3>/Switch2'

    real32_T Integrator_gainval;       // Computed Parameter: Integrator_gainval
                                          //  Referenced by: '<S38>/Integrator'

    real32_T Switch_Threshold;         // Computed Parameter: Switch_Threshold
                                          //  Referenced by: '<S3>/Switch'

    real32_T Switch1_Threshold;        // Computed Parameter: Switch1_Threshold
                                          //  Referenced by: '<S3>/Switch1'

    real32_T ffw_Y0;                   // Computed Parameter: ffw_Y0
                                          //  Referenced by: '<S56>/ffw'

    real32_T IV_Vdot_ref_Y0;           // Computed Parameter: IV_Vdot_ref_Y0
                                          //  Referenced by: '<S4>/IV_Vdot_ref'

    real32_T IV_dc_Y0_m;               // Computed Parameter: IV_dc_Y0_m
                                          //  Referenced by: '<S4>/IV_dc'

    real32_T UnitDelay_InitialCondition;
                               // Computed Parameter: UnitDelay_InitialCondition
                                  //  Referenced by: '<S4>/Unit Delay'

    real32_T VolumetricFlowrate_Gain;
                                  // Computed Parameter: VolumetricFlowrate_Gain
                                     //  Referenced by: '<S4>/Volumetric Flow rate'

    real32_T Integrator_gainval_m;   // Computed Parameter: Integrator_gainval_m
                                        //  Referenced by: '<S89>/Integrator'

    real32_T Saturation_UpperSat_b; // Computed Parameter: Saturation_UpperSat_b
                                       //  Referenced by: '<S4>/Saturation'

    real32_T Saturation_LowerSat_f; // Computed Parameter: Saturation_LowerSat_f
                                       //  Referenced by: '<S4>/Saturation'

    real32_T DataStoreMemory_InitialValue;
                             // Computed Parameter: DataStoreMemory_InitialValue
                                //  Referenced by: '<S4>/Data Store Memory'

    real32_T IV_dc_Y0_o;               // Computed Parameter: IV_dc_Y0_o
                                          //  Referenced by: '<S1>/IV_dc'

    real32_T IV_Vdot_ref_Y0_n;         // Computed Parameter: IV_Vdot_ref_Y0_n
                                          //  Referenced by: '<S1>/IV_Vdot_ref'

    real32_T IV_P_ref_Y0_f;            // Computed Parameter: IV_P_ref_Y0_f
                                          //  Referenced by: '<S1>/IV_P_ref'

    real32_T OV_cmd_Y0;                // Computed Parameter: OV_cmd_Y0
                                          //  Referenced by: '<S1>/OV_cmd'

    real32_T signals_Y0[3];            // Computed Parameter: signals_Y0
                                          //  Referenced by: '<S1>/signals'

    real32_T Constant_Value_a;         // Computed Parameter: Constant_Value_a
                                          //  Referenced by: '<S1>/Constant'

    real32_T UnitDelay_InitialCondition_g;
                             // Computed Parameter: UnitDelay_InitialCondition_g
                                //  Referenced by: '<S1>/Unit Delay'

    real32_T Gain_Gain;                // Computed Parameter: Gain_Gain
                                          //  Referenced by: '<S5>/Gain'

    real32_T Bias_Bias;                // Computed Parameter: Bias_Bias
                                          //  Referenced by: '<S5>/Bias'

    real32_T Gain1_Gain;               // Computed Parameter: Gain1_Gain
                                          //  Referenced by: '<S5>/Gain1'

    real32_T UnitDelay1_InitialCondition;
                              // Computed Parameter: UnitDelay1_InitialCondition
                                 //  Referenced by: '<S1>/Unit Delay1'

    real32_T Gain_Gain_p;              // Computed Parameter: Gain_Gain_p
                                          //  Referenced by: '<S2>/Gain'

    real32_T Bias_Bias_e;              // Computed Parameter: Bias_Bias_e
                                          //  Referenced by: '<S2>/Bias'

    real32_T Gain1_Gain_c;             // Computed Parameter: Gain1_Gain_c
                                          //  Referenced by: '<S2>/Gain1'

    real32_T cmH20_Gain;               // Computed Parameter: cmH20_Gain
                                          //  Referenced by: '<S1>/cmH20'

    boolean_T Constant1_Value_p;       // Expression: VC_mode
                                          //  Referenced by: '<S1>/Constant1'

    boolean_T Constant3_Value;         // Expression: VC_mode
                                          //  Referenced by: '<S1>/Constant3'

    boolean_T Constant2_Value;         // Expression: PC_mode
                                          //  Referenced by: '<S1>/Constant2'

  };

  // Real-time Model Data Structure
  struct RT_MODEL_controller_T {
    const char_T * volatile errorStatus;
  };

  // Tunable parameters
  static P_controller_T controller_P;

  // model initialize function
  void initialize();

  // model step function
  void step();

  // model terminate function
  void terminate();

  // Constructor
  controllerModelClass();

  // Destructor
  ~controllerModelClass();

  // Block parameters get method
  const controllerModelClass::P_controller_T & getBlockParameters() const;

  // Block parameters set method
  void setBlockParameters(const P_controller_T *pcontroller_P);

  // Root-level structure-based inputs set method

  // Root inports set method
  void setExternalInputs(const ExtU_controller_T* pExtU_controller_T)
  {
    controller_U = *pExtU_controller_T;
  }

  // Root-level structure-based outputs get method

  // Root outports get method
  const controllerModelClass::ExtY_controller_T & getExternalOutputs() const
  {
    return controller_Y;
  }

  // Real-Time Model get method
  controllerModelClass::RT_MODEL_controller_T * getRTM();

  // private data and function members
 private:
  // Block signals
  B_controller_T controller_B;

  // Block states
  DW_controller_T controller_DW;
  PrevZCX_controller_T controller_PrevZCX;// Triggered events

  // External inputs
  ExtU_controller_T controller_U;

  // External outputs
  ExtY_controller_T controller_Y;

  // Real-Time Model
  RT_MODEL_controller_T controller_M;

  // private member function(s) for subsystem '<Root>'
  void controller_SystemCore_release(b_dspcodegen_BiquadFilter_con_T *obj);
  void cont_LPHPFilterBase_releaseImpl(dsp_LowpassFilter_controller_T *obj);
  void contr_SystemCore_releaseWrapper(dsp_LowpassFilter_controller_T *obj);
  void controller_SystemCore_release_j(dsp_LowpassFilter_controller_T *obj);
  void controller_SystemCore_delete_j(dsp_LowpassFilter_controller_T *obj);
  void matlabCodegenHandle_matlabCod_j(dsp_LowpassFilter_controller_T *obj);
  void controller_SystemCore_delete(b_dspcodegen_BiquadFilter_con_T *obj);
  void matlabCodegenHandle_matlabCodeg(b_dspcodegen_BiquadFilter_con_T *obj);
};

//-
//  These blocks were eliminated from the model due to optimizations:
//
//  Block '<S1>/Zero-Order Hold' : Eliminated since input and output rates are identical
//  Block '<S1>/Zero-Order Hold1' : Eliminated since input and output rates are identical


//-
//  The generated code includes comments that allow you to trace directly
//  back to the appropriate location in the model.  The basic format
//  is <system>/block_name, where system is the system number (uniquely
//  assigned by Simulink) and block_name is the name of the block.
//
//  Use the MATLAB hilite_system command to trace the generated code back
//  to the model.  For example,
//
//  hilite_system('<S3>')    - opens system 3
//  hilite_system('<S3>/Kp') - opens and selects block Kp which resides in S3
//
//  Here is the system hierarchy for this model
//
//  '<Root>' : 'controller'
//  '<S1>'   : 'controller/Controller'
//  '<S2>'   : 'controller/Controller/HSC Converter'
//  '<S3>'   : 'controller/Controller/Pressure Control'
//  '<S4>'   : 'controller/Controller/Volume Control'
//  '<S5>'   : 'controller/Controller/Zephyr Converter'
//  '<S6>'   : 'controller/Controller/Pressure Control/PID Controller'
//  '<S7>'   : 'controller/Controller/Pressure Control/PID Controller/Anti-windup'
//  '<S8>'   : 'controller/Controller/Pressure Control/PID Controller/D Gain'
//  '<S9>'   : 'controller/Controller/Pressure Control/PID Controller/Filter'
//  '<S10>'  : 'controller/Controller/Pressure Control/PID Controller/Filter ICs'
//  '<S11>'  : 'controller/Controller/Pressure Control/PID Controller/I Gain'
//  '<S12>'  : 'controller/Controller/Pressure Control/PID Controller/Ideal P Gain'
//  '<S13>'  : 'controller/Controller/Pressure Control/PID Controller/Ideal P Gain Fdbk'
//  '<S14>'  : 'controller/Controller/Pressure Control/PID Controller/Integrator'
//  '<S15>'  : 'controller/Controller/Pressure Control/PID Controller/Integrator ICs'
//  '<S16>'  : 'controller/Controller/Pressure Control/PID Controller/N Copy'
//  '<S17>'  : 'controller/Controller/Pressure Control/PID Controller/N Gain'
//  '<S18>'  : 'controller/Controller/Pressure Control/PID Controller/P Copy'
//  '<S19>'  : 'controller/Controller/Pressure Control/PID Controller/Parallel P Gain'
//  '<S20>'  : 'controller/Controller/Pressure Control/PID Controller/Reset Signal'
//  '<S21>'  : 'controller/Controller/Pressure Control/PID Controller/Saturation'
//  '<S22>'  : 'controller/Controller/Pressure Control/PID Controller/Saturation Fdbk'
//  '<S23>'  : 'controller/Controller/Pressure Control/PID Controller/Sum'
//  '<S24>'  : 'controller/Controller/Pressure Control/PID Controller/Sum Fdbk'
//  '<S25>'  : 'controller/Controller/Pressure Control/PID Controller/Tracking Mode'
//  '<S26>'  : 'controller/Controller/Pressure Control/PID Controller/Tracking Mode Sum'
//  '<S27>'  : 'controller/Controller/Pressure Control/PID Controller/Tsamp - Integral'
//  '<S28>'  : 'controller/Controller/Pressure Control/PID Controller/Tsamp - Ngain'
//  '<S29>'  : 'controller/Controller/Pressure Control/PID Controller/postSat Signal'
//  '<S30>'  : 'controller/Controller/Pressure Control/PID Controller/preSat Signal'
//  '<S31>'  : 'controller/Controller/Pressure Control/PID Controller/Anti-windup/Passthrough'
//  '<S32>'  : 'controller/Controller/Pressure Control/PID Controller/D Gain/Disabled'
//  '<S33>'  : 'controller/Controller/Pressure Control/PID Controller/Filter/Disabled'
//  '<S34>'  : 'controller/Controller/Pressure Control/PID Controller/Filter ICs/Disabled'
//  '<S35>'  : 'controller/Controller/Pressure Control/PID Controller/I Gain/External Parameters'
//  '<S36>'  : 'controller/Controller/Pressure Control/PID Controller/Ideal P Gain/Passthrough'
//  '<S37>'  : 'controller/Controller/Pressure Control/PID Controller/Ideal P Gain Fdbk/Disabled'
//  '<S38>'  : 'controller/Controller/Pressure Control/PID Controller/Integrator/Discrete'
//  '<S39>'  : 'controller/Controller/Pressure Control/PID Controller/Integrator ICs/Internal IC'
//  '<S40>'  : 'controller/Controller/Pressure Control/PID Controller/N Copy/Disabled wSignal Specification'
//  '<S41>'  : 'controller/Controller/Pressure Control/PID Controller/N Gain/Disabled'
//  '<S42>'  : 'controller/Controller/Pressure Control/PID Controller/P Copy/Disabled'
//  '<S43>'  : 'controller/Controller/Pressure Control/PID Controller/Parallel P Gain/External Parameters'
//  '<S44>'  : 'controller/Controller/Pressure Control/PID Controller/Reset Signal/External Reset'
//  '<S45>'  : 'controller/Controller/Pressure Control/PID Controller/Saturation/Passthrough'
//  '<S46>'  : 'controller/Controller/Pressure Control/PID Controller/Saturation Fdbk/Disabled'
//  '<S47>'  : 'controller/Controller/Pressure Control/PID Controller/Sum/Sum_PI'
//  '<S48>'  : 'controller/Controller/Pressure Control/PID Controller/Sum Fdbk/Disabled'
//  '<S49>'  : 'controller/Controller/Pressure Control/PID Controller/Tracking Mode/Disabled'
//  '<S50>'  : 'controller/Controller/Pressure Control/PID Controller/Tracking Mode Sum/Passthrough'
//  '<S51>'  : 'controller/Controller/Pressure Control/PID Controller/Tsamp - Integral/Passthrough'
//  '<S52>'  : 'controller/Controller/Pressure Control/PID Controller/Tsamp - Ngain/Passthrough'
//  '<S53>'  : 'controller/Controller/Pressure Control/PID Controller/postSat Signal/Forward_Path'
//  '<S54>'  : 'controller/Controller/Pressure Control/PID Controller/preSat Signal/Forward_Path'
//  '<S55>'  : 'controller/Controller/Volume Control/PID Controller'
//  '<S56>'  : 'controller/Controller/Volume Control/Read FFW'
//  '<S57>'  : 'controller/Controller/Volume Control/Store FFW'
//  '<S58>'  : 'controller/Controller/Volume Control/PID Controller/Anti-windup'
//  '<S59>'  : 'controller/Controller/Volume Control/PID Controller/D Gain'
//  '<S60>'  : 'controller/Controller/Volume Control/PID Controller/Filter'
//  '<S61>'  : 'controller/Controller/Volume Control/PID Controller/Filter ICs'
//  '<S62>'  : 'controller/Controller/Volume Control/PID Controller/I Gain'
//  '<S63>'  : 'controller/Controller/Volume Control/PID Controller/Ideal P Gain'
//  '<S64>'  : 'controller/Controller/Volume Control/PID Controller/Ideal P Gain Fdbk'
//  '<S65>'  : 'controller/Controller/Volume Control/PID Controller/Integrator'
//  '<S66>'  : 'controller/Controller/Volume Control/PID Controller/Integrator ICs'
//  '<S67>'  : 'controller/Controller/Volume Control/PID Controller/N Copy'
//  '<S68>'  : 'controller/Controller/Volume Control/PID Controller/N Gain'
//  '<S69>'  : 'controller/Controller/Volume Control/PID Controller/P Copy'
//  '<S70>'  : 'controller/Controller/Volume Control/PID Controller/Parallel P Gain'
//  '<S71>'  : 'controller/Controller/Volume Control/PID Controller/Reset Signal'
//  '<S72>'  : 'controller/Controller/Volume Control/PID Controller/Saturation'
//  '<S73>'  : 'controller/Controller/Volume Control/PID Controller/Saturation Fdbk'
//  '<S74>'  : 'controller/Controller/Volume Control/PID Controller/Sum'
//  '<S75>'  : 'controller/Controller/Volume Control/PID Controller/Sum Fdbk'
//  '<S76>'  : 'controller/Controller/Volume Control/PID Controller/Tracking Mode'
//  '<S77>'  : 'controller/Controller/Volume Control/PID Controller/Tracking Mode Sum'
//  '<S78>'  : 'controller/Controller/Volume Control/PID Controller/Tsamp - Integral'
//  '<S79>'  : 'controller/Controller/Volume Control/PID Controller/Tsamp - Ngain'
//  '<S80>'  : 'controller/Controller/Volume Control/PID Controller/postSat Signal'
//  '<S81>'  : 'controller/Controller/Volume Control/PID Controller/preSat Signal'
//  '<S82>'  : 'controller/Controller/Volume Control/PID Controller/Anti-windup/Passthrough'
//  '<S83>'  : 'controller/Controller/Volume Control/PID Controller/D Gain/Disabled'
//  '<S84>'  : 'controller/Controller/Volume Control/PID Controller/Filter/Disabled'
//  '<S85>'  : 'controller/Controller/Volume Control/PID Controller/Filter ICs/Disabled'
//  '<S86>'  : 'controller/Controller/Volume Control/PID Controller/I Gain/Internal Parameters'
//  '<S87>'  : 'controller/Controller/Volume Control/PID Controller/Ideal P Gain/Passthrough'
//  '<S88>'  : 'controller/Controller/Volume Control/PID Controller/Ideal P Gain Fdbk/Disabled'
//  '<S89>'  : 'controller/Controller/Volume Control/PID Controller/Integrator/Discrete'
//  '<S90>'  : 'controller/Controller/Volume Control/PID Controller/Integrator ICs/Internal IC'
//  '<S91>'  : 'controller/Controller/Volume Control/PID Controller/N Copy/Disabled wSignal Specification'
//  '<S92>'  : 'controller/Controller/Volume Control/PID Controller/N Gain/Disabled'
//  '<S93>'  : 'controller/Controller/Volume Control/PID Controller/P Copy/Disabled'
//  '<S94>'  : 'controller/Controller/Volume Control/PID Controller/Parallel P Gain/Internal Parameters'
//  '<S95>'  : 'controller/Controller/Volume Control/PID Controller/Reset Signal/External Reset'
//  '<S96>'  : 'controller/Controller/Volume Control/PID Controller/Saturation/Passthrough'
//  '<S97>'  : 'controller/Controller/Volume Control/PID Controller/Saturation Fdbk/Disabled'
//  '<S98>'  : 'controller/Controller/Volume Control/PID Controller/Sum/Sum_PI'
//  '<S99>'  : 'controller/Controller/Volume Control/PID Controller/Sum Fdbk/Disabled'
//  '<S100>' : 'controller/Controller/Volume Control/PID Controller/Tracking Mode/Disabled'
//  '<S101>' : 'controller/Controller/Volume Control/PID Controller/Tracking Mode Sum/Passthrough'
//  '<S102>' : 'controller/Controller/Volume Control/PID Controller/Tsamp - Integral/Passthrough'
//  '<S103>' : 'controller/Controller/Volume Control/PID Controller/Tsamp - Ngain/Passthrough'
//  '<S104>' : 'controller/Controller/Volume Control/PID Controller/postSat Signal/Forward_Path'
//  '<S105>' : 'controller/Controller/Volume Control/PID Controller/preSat Signal/Forward_Path'

#endif                                 // RTW_HEADER_controller_h_

//
// File trailer for generated code.
//
// [EOF]
//
