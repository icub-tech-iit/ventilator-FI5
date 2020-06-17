//
// Non-Degree Granting Education License -- for use at non-degree
// granting, nonprofit, educational organizations only. Not for
// government, commercial, or other organizational use.
//
// File: controller.h
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
#ifndef RTW_HEADER_controller_h_
#define RTW_HEADER_controller_h_
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
    real32_T Bias;                     // '<S12>/Bias'
    real32_T Product1[2];              // '<S2>/Product1'
    real32_T Gain;                     // '<S1>/Gain'
    real32_T DiscreteTimeIntegrator;   // '<S1>/Discrete-Time Integrator'
    real32_T DataTypeConversion;       // '<S1>/Data Type Conversion'
    real32_T VolumetricFlowrate;       // '<S16>/Volumetric Flow rate'
    real32_T Integrator;               // '<S104>/Integrator'
    real32_T Saturation;               // '<S16>/Saturation'
    real32_T DataStoreRead;            // '<S71>/Data Store Read'
    real32_T pulse;                    // '<S1>/Pulse Generator'
    real32_T TmpSignalConversionAtInputShapi[4];// '<S10>/Init IS'
    real32_T TmpSignalConversionAtInputSha_h[4];// '<S10>/Init IS'
    real32_T Integrator_m;             // '<S52>/Integrator'
    real32_T Switch_n;                 // '<S10>/Switch'
    real32_T Switch1;                  // '<S10>/Switch1'
    real32_T Switch2[3];               // '<S20>/Switch2'
    real32_T OutportBufferForFFW;
    real32_T OutportBufferForI;
    real32_T HSCLP[4];                 // '<S1>/HSC LP'
    boolean_T Compare_m;               // '<S17>/Compare'
  } B_controller_T;

  // Block states (default storage) for system '<Root>'
  typedef struct {
    b_dspcodegen_BiquadFilter_con_T gobj_1;// '<S1>/HSC LP'
    b_dspcodegen_BiquadFilter_con_T gobj_2;// '<S1>/HSC LP'
    dsp_LowpassFilter_controller_T obj;// '<S1>/HSC LP'
    real32_T UnitDelay1_DSTATE[4];     // '<S1>/Unit Delay1'
    real32_T UnitDelay_DSTATE[2];      // '<S1>/Unit Delay'
    real32_T UnitDelay2_DSTATE;        // '<S1>/Unit Delay2'
    real32_T DiscreteTimeIntegrator_DSTATE;// '<S1>/Discrete-Time Integrator'
    real32_T UnitDelay3_DSTATE;        // '<S1>/Unit Delay3'
    real32_T UnitDelay_DSTATE_n;       // '<S16>/Unit Delay'
    real32_T Integrator_DSTATE;        // '<S104>/Integrator'
    real32_T InputShaping_states[3];   // '<S10>/Input Shaping'
    real32_T UnitDelay_DSTATE_o;       // '<S10>/Unit Delay'
    real32_T Integrator_DSTATE_p;      // '<S52>/Integrator'
    real32_T FFW;                      // '<S16>/Data Store Memory'
    real32_T InputShaping_tmp;         // '<S10>/Input Shaping'
    uint32_T temporalCounter_i1;       // '<S1>/Pulse Generator'
    uint32_T presentTicks;             // '<S1>/Pulse Generator'
    uint32_T elapsedTicks;             // '<S1>/Pulse Generator'
    uint32_T previousTicks;            // '<S1>/Pulse Generator'
    int8_T DiscreteTimeIntegrator_PrevRese;// '<S1>/Discrete-Time Integrator'
    int8_T Integrator_PrevResetState;  // '<S104>/Integrator'
    int8_T Integrator_PrevResetState_f;// '<S52>/Integrator'
    uint8_T is_c3_controller;          // '<S1>/Pulse Generator'
    uint8_T control_mode_prev;         // '<S1>/Pulse Generator'
    uint8_T control_mode_start;        // '<S1>/Pulse Generator'
    uint8_T InputShaping_icLoad;       // '<S10>/Input Shaping'
    boolean_T doneDoubleBufferReInit;  // '<S1>/Pulse Generator'
    boolean_T objisempty;              // '<S1>/HSC LP'
    boolean_T isInitialized;           // '<S1>/HSC LP'
    boolean_T Controller_MODE;         // '<Root>/Controller'
    boolean_T VolumeControl_MODE;      // '<S1>/Volume Control'
    boolean_T ReadFFW_MODE;            // '<S16>/Read FFW'
    boolean_T PressureControl_MODE;    // '<S1>/Pressure Control'
    boolean_T ComputeTrigger_MODE;     // '<S1>/Compute Trigger'
  } DW_controller_T;

  // Zero-crossing (trigger) state
  typedef struct {
    ZCSigState StoreFFW_Trig_ZCE;      // '<S16>/Store FFW'
    ZCSigState InputShaping_Reset_ZCE; // '<S10>/Input Shaping'
  } PrevZCX_controller_T;

  // External inputs (root inport signals with default storage)
  typedef struct {
    boolean_T enable;                  // '<Root>/enable'
    real32_T S2_Qi;                    // '<Root>/S2_Qi'
    real32_T S5_Qi;                    // '<Root>/S5_Qi'
    real32_T S1_Pi;                    // '<Root>/S1_Pi'
    real32_T S4_Pi;                    // '<Root>/S4_Pi'
    real32_T S3_i[2];                  // '<Root>/S3_i'
    real32_T S7_Pi;                    // '<Root>/S7_Pi'
    real32_T params[3];                // '<Root>/params'
  } ExtU_controller_T;

  // External outputs (root outports fed by signals with default storage)
  typedef struct {
    real32_T IV_dc;                    // '<Root>/IV_dc'
    real32_T Q_ref;                    // '<Root>/Q_ref'
    real32_T P_ref;                    // '<Root>/P_ref'
    boolean_T OV_cmd;                  // '<Root>/OV_cmd'
    real32_T signals[9];               // '<Root>/signals'
  } ExtY_controller_T;

  // Parameters (default storage)
  struct P_controller_T {
    real32_T PIDController_InitialConditionF;
                              // Mask Parameter: PIDController_InitialConditionF
                                 //  Referenced by: '<S52>/Integrator'

    real32_T PIDController_InitialConditio_c;
                              // Mask Parameter: PIDController_InitialConditio_c
                                 //  Referenced by: '<S104>/Integrator'

    real32_T PIDController_Kt;         // Mask Parameter: PIDController_Kt
                                          //  Referenced by: '<S63>/Kt'

    real32_T PIDController_Kt_j;       // Mask Parameter: PIDController_Kt_j
                                          //  Referenced by: '<S115>/Kt'

    real32_T pressure_thres_const;     // Mask Parameter: pressure_thres_const
                                          //  Referenced by: '<S17>/Constant'

    uint8_T CompareToConstant1_const;// Mask Parameter: CompareToConstant1_const
                                        //  Referenced by: '<S4>/Constant'

    uint8_T CompareToConstant2_const;// Mask Parameter: CompareToConstant2_const
                                        //  Referenced by: '<S5>/Constant'

    uint8_T CompareToConstant3_const;// Mask Parameter: CompareToConstant3_const
                                        //  Referenced by: '<S6>/Constant'

    uint8_T CompareToConstant_const;  // Mask Parameter: CompareToConstant_const
                                         //  Referenced by: '<S3>/Constant'

    uint8_T Controller_control_mode;  // Mask Parameter: Controller_control_mode
                                         //  Referenced by: '<S1>/Constant13'

    real32_T Constant8_Value;          // Computed Parameter: Constant8_Value
                                          //  Referenced by: '<S1>/Constant8'

    real32_T PC_P_Value;               // Computed Parameter: PC_P_Value
                                          //  Referenced by: '<S20>/PC_P'

    real32_T PC_I_Value;               // Computed Parameter: PC_I_Value
                                          //  Referenced by: '<S20>/PC_I'

    real32_T PC_FFW_Value;             // Computed Parameter: PC_FFW_Value
                                          //  Referenced by: '<S20>/PC_FFW'

    real32_T PC_P_rec_Value;           // Computed Parameter: PC_P_rec_Value
                                          //  Referenced by: '<S20>/PC_P_rec'

    real32_T PC_I_rec_Value;           // Computed Parameter: PC_I_rec_Value
                                          //  Referenced by: '<S20>/PC_I_rec'

    real32_T PC_FFW_rec_Value;         // Computed Parameter: PC_FFW_rec_Value
                                          //  Referenced by: '<S20>/PC_FFW_rec'

    real32_T P_Y0;                     // Computed Parameter: P_Y0
                                          //  Referenced by: '<S20>/P'

    real32_T I_Y0;                     // Computed Parameter: I_Y0
                                          //  Referenced by: '<S20>/I'

    real32_T FFW_Y0;                   // Computed Parameter: FFW_Y0
                                          //  Referenced by: '<S20>/FFW'

    real32_T Switch2_Threshold;        // Computed Parameter: Switch2_Threshold
                                          //  Referenced by: '<S20>/Switch2'

    real32_T P_ref_Y0;                 // Computed Parameter: P_ref_Y0
                                          //  Referenced by: '<S10>/P_ref'

    real32_T IV_dc_Y0;                 // Computed Parameter: IV_dc_Y0
                                          //  Referenced by: '<S10>/IV_dc'

    real32_T Constant_Value;           // Computed Parameter: Constant_Value
                                          //  Referenced by: '<S10>/Constant'

    real32_T PIP_Gain;                 // Computed Parameter: PIP_Gain
                                          //  Referenced by: '<S10>/PIP'

    real32_T PIP_step_perc_Gain;       // Computed Parameter: PIP_step_perc_Gain
                                          //  Referenced by: '<S10>/PIP_step_perc'

    real32_T Gain3_Gain;               // Computed Parameter: Gain3_Gain
                                          //  Referenced by: '<S10>/Gain3'

    real32_T Constant_Value_e[3];      // Computed Parameter: Constant_Value_e
                                          //  Referenced by: '<S18>/Constant'

    real32_T Constant1_Value;          // Computed Parameter: Constant1_Value
                                          //  Referenced by: '<S18>/Constant 1'

    real32_T Constant1_Value_n[3];     // Computed Parameter: Constant1_Value_n
                                          //  Referenced by: '<S18>/Constant1'

    real32_T Constant2_Value;          // Computed Parameter: Constant2_Value
                                          //  Referenced by: '<S18>/Constant2'

    real32_T PIP_rise_perc_Value;     // Computed Parameter: PIP_rise_perc_Value
                                         //  Referenced by: '<S18>/PIP_rise_perc'

    real32_T Gain_Gain;                // Computed Parameter: Gain_Gain
                                          //  Referenced by: '<S18>/Gain'

    real32_T UnitDelay_InitialCondition;
                               // Computed Parameter: UnitDelay_InitialCondition
                                  //  Referenced by: '<S10>/Unit Delay'

    real32_T Integrator_gainval;       // Computed Parameter: Integrator_gainval
                                          //  Referenced by: '<S52>/Integrator'

    real32_T Saturation_UpperSat;     // Computed Parameter: Saturation_UpperSat
                                         //  Referenced by: '<S10>/Saturation'

    real32_T Saturation_LowerSat;     // Computed Parameter: Saturation_LowerSat
                                         //  Referenced by: '<S10>/Saturation'

    real32_T Switch_Threshold;         // Computed Parameter: Switch_Threshold
                                          //  Referenced by: '<S10>/Switch'

    real32_T Switch1_Threshold;        // Computed Parameter: Switch1_Threshold
                                          //  Referenced by: '<S10>/Switch1'

    real32_T VC_P_Value;               // Computed Parameter: VC_P_Value
                                          //  Referenced by: '<S70>/VC_P'

    real32_T VC_I_Value;               // Computed Parameter: VC_I_Value
                                          //  Referenced by: '<S70>/VC_I'

    real32_T VC_P_rec_Value;           // Computed Parameter: VC_P_rec_Value
                                          //  Referenced by: '<S70>/VC_P_rec'

    real32_T VC_I_rec_Value;           // Computed Parameter: VC_I_rec_Value
                                          //  Referenced by: '<S70>/VC_I_rec'

    real32_T ffw_Y0;                   // Computed Parameter: ffw_Y0
                                          //  Referenced by: '<S71>/ffw'

    real32_T Q_ref_Y0;                 // Computed Parameter: Q_ref_Y0
                                          //  Referenced by: '<S16>/Q_ref'

    real32_T IV_dc_Y0_m;               // Computed Parameter: IV_dc_Y0_m
                                          //  Referenced by: '<S16>/IV_dc'

    real32_T UnitDelay_InitialCondition_n;
                             // Computed Parameter: UnitDelay_InitialCondition_n
                                //  Referenced by: '<S16>/Unit Delay'

    real32_T VolumetricFlowrate_Gain;
                                  // Computed Parameter: VolumetricFlowrate_Gain
                                     //  Referenced by: '<S16>/Volumetric Flow rate'

    real32_T Switch2_Threshold_b;     // Computed Parameter: Switch2_Threshold_b
                                         //  Referenced by: '<S70>/Switch2'

    real32_T Integrator_gainval_m;   // Computed Parameter: Integrator_gainval_m
                                        //  Referenced by: '<S104>/Integrator'

    real32_T Saturation_UpperSat_b; // Computed Parameter: Saturation_UpperSat_b
                                       //  Referenced by: '<S16>/Saturation'

    real32_T Saturation_LowerSat_f; // Computed Parameter: Saturation_LowerSat_f
                                       //  Referenced by: '<S16>/Saturation'

    real32_T DataStoreMemory_InitialValue;
                             // Computed Parameter: DataStoreMemory_InitialValue
                                //  Referenced by: '<S16>/Data Store Memory'

    real32_T IV_dc_Y0_o;               // Computed Parameter: IV_dc_Y0_o
                                          //  Referenced by: '<S1>/IV_dc'

    real32_T Q_ref_Y0_n;               // Computed Parameter: Q_ref_Y0_n
                                          //  Referenced by: '<S1>/Q_ref'

    real32_T P_ref_Y0_f;               // Computed Parameter: P_ref_Y0_f
                                          //  Referenced by: '<S1>/P_ref'

    real32_T signals_Y0;               // Computed Parameter: signals_Y0
                                          //  Referenced by: '<S1>/signals'

    real32_T Constant_Value_p;         // Computed Parameter: Constant_Value_p
                                          //  Referenced by: '<S7>/Constant'

    real32_T UnitDelay1_InitialCondition[4];
                              // Computed Parameter: UnitDelay1_InitialCondition
                                 //  Referenced by: '<S1>/Unit Delay1'

    real32_T Gain_Gain_h;              // Computed Parameter: Gain_Gain_h
                                          //  Referenced by: '<S15>/Gain'

    real32_T Bias_Bias;                // Computed Parameter: Bias_Bias
                                          //  Referenced by: '<S15>/Bias'

    real32_T Gain1_Gain;               // Computed Parameter: Gain1_Gain
                                          //  Referenced by: '<S15>/Gain1'

    real32_T mabr_to_cmH2O_Gain;       // Computed Parameter: mabr_to_cmH2O_Gain
                                          //  Referenced by: '<S1>/mabr_to_cmH2O'

    real32_T Gain_Gain_i;              // Computed Parameter: Gain_Gain_i
                                          //  Referenced by: '<S14>/Gain'

    real32_T Bias_Bias_h;              // Computed Parameter: Bias_Bias_h
                                          //  Referenced by: '<S14>/Bias'

    real32_T Gain1_Gain_d;             // Computed Parameter: Gain1_Gain_d
                                          //  Referenced by: '<S14>/Gain1'

    real32_T psi_cmH2O_Gain;           // Computed Parameter: psi_cmH2O_Gain
                                          //  Referenced by: '<S1>/psi_cmH2O'

    real32_T UnitDelay_InitialCondition_g;
                             // Computed Parameter: UnitDelay_InitialCondition_g
                                //  Referenced by: '<S1>/Unit Delay'

    real32_T Gain_Gain_f;              // Computed Parameter: Gain_Gain_f
                                          //  Referenced by: '<S13>/Gain'

    real32_T Bias_Bias_l;              // Computed Parameter: Bias_Bias_l
                                          //  Referenced by: '<S13>/Bias'

    real32_T Gain1_Gain_e;             // Computed Parameter: Gain1_Gain_e
                                          //  Referenced by: '<S13>/Gain1'

    real32_T Constant_Value_h;         // Computed Parameter: Constant_Value_h
                                          //  Referenced by: '<S2>/Constant'

    real32_T UnitDelay2_InitialCondition;
                              // Computed Parameter: UnitDelay2_InitialCondition
                                 //  Referenced by: '<S1>/Unit Delay2'

    real32_T Gain_Gain_m;              // Computed Parameter: Gain_Gain_m
                                          //  Referenced by: '<S12>/Gain'

    real32_T Bias_Bias_b;              // Computed Parameter: Bias_Bias_b
                                          //  Referenced by: '<S12>/Bias'

    real32_T K_Bias;                   // Computed Parameter: K_Bias
                                          //  Referenced by: '<S2>/K'

    real32_T cmH2OtoPa_Gain;           // Computed Parameter: cmH2OtoPa_Gain
                                          //  Referenced by: '<S2>/cmH2O to Pa'

    real32_T Bias_Bias_j;              // Computed Parameter: Bias_Bias_j
                                          //  Referenced by: '<S2>/Bias'

    real32_T Constant1_Value_k;        // Computed Parameter: Constant1_Value_k
                                          //  Referenced by: '<S2>/Constant1'

    real32_T Gain_Gain_e;              // Computed Parameter: Gain_Gain_e
                                          //  Referenced by: '<S1>/Gain'

    real32_T DiscreteTimeIntegrator_gainval;
                           // Computed Parameter: DiscreteTimeIntegrator_gainval
                              //  Referenced by: '<S1>/Discrete-Time Integrator'

    real32_T DiscreteTimeIntegrator_IC;
                                // Computed Parameter: DiscreteTimeIntegrator_IC
                                   //  Referenced by: '<S1>/Discrete-Time Integrator'

    real32_T respiratory_rate_Value;
                                   // Computed Parameter: respiratory_rate_Value
                                      //  Referenced by: '<S1>/respiratory_rate'

    real32_T IE_ratio_Value;           // Computed Parameter: IE_ratio_Value
                                          //  Referenced by: '<S1>/IE_ratio'

    real32_T UnitDelay3_InitialCondition;
                              // Computed Parameter: UnitDelay3_InitialCondition
                                 //  Referenced by: '<S1>/Unit Delay3'

    real32_T TidalVolume_Gain;         // Computed Parameter: TidalVolume_Gain
                                          //  Referenced by: '<S1>/Tidal Volume'

    real32_T Constant_Value_a;         // Computed Parameter: Constant_Value_a
                                          //  Referenced by: '<S1>/Constant'

    boolean_T Out_Y0;                  // Computed Parameter: Out_Y0
                                          //  Referenced by: '<S8>/Out'

    boolean_T OV_cmd_Y0;               // Computed Parameter: OV_cmd_Y0
                                          //  Referenced by: '<S1>/OV_cmd'

    boolean_T enable_trigger_Value;  // Computed Parameter: enable_trigger_Value
                                        //  Referenced by: '<S1>/enable_trigger'

  };

  // Real-time Model Data Structure
  struct RT_MODEL_controller_T {
    const char_T * volatile errorStatus;

    //
    //  Timing:
    //  The following substructure contains information regarding
    //  the timing information for the model.

    struct {
      uint32_T clockTick0;
    } Timing;
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
//  Block '<S9>/Reshape' : Reshape block reduction
//  Block '<S1>/Zero-Order Hold' : Eliminated since input and output rates are identical
//  Block '<S1>/Zero-Order Hold1' : Eliminated since input and output rates are identical
//  Block '<S1>/Zero-Order Hold2' : Eliminated since input and output rates are identical


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
//  '<S2>'   : 'controller/Controller/Apply Actual Conditions'
//  '<S3>'   : 'controller/Controller/Compare To Constant'
//  '<S4>'   : 'controller/Controller/Compare To Constant1'
//  '<S5>'   : 'controller/Controller/Compare To Constant2'
//  '<S6>'   : 'controller/Controller/Compare To Constant3'
//  '<S7>'   : 'controller/Controller/Compare To Zero'
//  '<S8>'   : 'controller/Controller/Compute Trigger'
//  '<S9>'   : 'controller/Controller/Convert 1-D to 2-D'
//  '<S10>'  : 'controller/Controller/Pressure Control'
//  '<S11>'  : 'controller/Controller/Pulse Generator'
//  '<S12>'  : 'controller/Controller/Subsystem Reference'
//  '<S13>'  : 'controller/Controller/Subsystem Reference1'
//  '<S14>'  : 'controller/Controller/Subsystem Reference2'
//  '<S15>'  : 'controller/Controller/Subsystem Reference3'
//  '<S16>'  : 'controller/Controller/Volume Control'
//  '<S17>'  : 'controller/Controller/Compute Trigger/pressure_thres'
//  '<S18>'  : 'controller/Controller/Pressure Control/Init IS'
//  '<S19>'  : 'controller/Controller/Pressure Control/PID Controller'
//  '<S20>'  : 'controller/Controller/Pressure Control/Params Selector'
//  '<S21>'  : 'controller/Controller/Pressure Control/PID Controller/Anti-windup'
//  '<S22>'  : 'controller/Controller/Pressure Control/PID Controller/D Gain'
//  '<S23>'  : 'controller/Controller/Pressure Control/PID Controller/Filter'
//  '<S24>'  : 'controller/Controller/Pressure Control/PID Controller/Filter ICs'
//  '<S25>'  : 'controller/Controller/Pressure Control/PID Controller/I Gain'
//  '<S26>'  : 'controller/Controller/Pressure Control/PID Controller/Ideal P Gain'
//  '<S27>'  : 'controller/Controller/Pressure Control/PID Controller/Ideal P Gain Fdbk'
//  '<S28>'  : 'controller/Controller/Pressure Control/PID Controller/Integrator'
//  '<S29>'  : 'controller/Controller/Pressure Control/PID Controller/Integrator ICs'
//  '<S30>'  : 'controller/Controller/Pressure Control/PID Controller/N Copy'
//  '<S31>'  : 'controller/Controller/Pressure Control/PID Controller/N Gain'
//  '<S32>'  : 'controller/Controller/Pressure Control/PID Controller/P Copy'
//  '<S33>'  : 'controller/Controller/Pressure Control/PID Controller/Parallel P Gain'
//  '<S34>'  : 'controller/Controller/Pressure Control/PID Controller/Reset Signal'
//  '<S35>'  : 'controller/Controller/Pressure Control/PID Controller/Saturation'
//  '<S36>'  : 'controller/Controller/Pressure Control/PID Controller/Saturation Fdbk'
//  '<S37>'  : 'controller/Controller/Pressure Control/PID Controller/Sum'
//  '<S38>'  : 'controller/Controller/Pressure Control/PID Controller/Sum Fdbk'
//  '<S39>'  : 'controller/Controller/Pressure Control/PID Controller/Tracking Mode'
//  '<S40>'  : 'controller/Controller/Pressure Control/PID Controller/Tracking Mode Sum'
//  '<S41>'  : 'controller/Controller/Pressure Control/PID Controller/Tsamp - Integral'
//  '<S42>'  : 'controller/Controller/Pressure Control/PID Controller/Tsamp - Ngain'
//  '<S43>'  : 'controller/Controller/Pressure Control/PID Controller/postSat Signal'
//  '<S44>'  : 'controller/Controller/Pressure Control/PID Controller/preSat Signal'
//  '<S45>'  : 'controller/Controller/Pressure Control/PID Controller/Anti-windup/Passthrough'
//  '<S46>'  : 'controller/Controller/Pressure Control/PID Controller/D Gain/Disabled'
//  '<S47>'  : 'controller/Controller/Pressure Control/PID Controller/Filter/Disabled'
//  '<S48>'  : 'controller/Controller/Pressure Control/PID Controller/Filter ICs/Disabled'
//  '<S49>'  : 'controller/Controller/Pressure Control/PID Controller/I Gain/External Parameters'
//  '<S50>'  : 'controller/Controller/Pressure Control/PID Controller/Ideal P Gain/Passthrough'
//  '<S51>'  : 'controller/Controller/Pressure Control/PID Controller/Ideal P Gain Fdbk/Passthrough'
//  '<S52>'  : 'controller/Controller/Pressure Control/PID Controller/Integrator/Discrete'
//  '<S53>'  : 'controller/Controller/Pressure Control/PID Controller/Integrator ICs/Internal IC'
//  '<S54>'  : 'controller/Controller/Pressure Control/PID Controller/N Copy/Disabled wSignal Specification'
//  '<S55>'  : 'controller/Controller/Pressure Control/PID Controller/N Gain/Disabled'
//  '<S56>'  : 'controller/Controller/Pressure Control/PID Controller/P Copy/Disabled'
//  '<S57>'  : 'controller/Controller/Pressure Control/PID Controller/Parallel P Gain/External Parameters'
//  '<S58>'  : 'controller/Controller/Pressure Control/PID Controller/Reset Signal/External Reset'
//  '<S59>'  : 'controller/Controller/Pressure Control/PID Controller/Saturation/Passthrough'
//  '<S60>'  : 'controller/Controller/Pressure Control/PID Controller/Saturation Fdbk/Passthrough'
//  '<S61>'  : 'controller/Controller/Pressure Control/PID Controller/Sum/Sum_PI'
//  '<S62>'  : 'controller/Controller/Pressure Control/PID Controller/Sum Fdbk/Enabled'
//  '<S63>'  : 'controller/Controller/Pressure Control/PID Controller/Tracking Mode/Enabled'
//  '<S64>'  : 'controller/Controller/Pressure Control/PID Controller/Tracking Mode Sum/Tracking Mode'
//  '<S65>'  : 'controller/Controller/Pressure Control/PID Controller/Tsamp - Integral/Passthrough'
//  '<S66>'  : 'controller/Controller/Pressure Control/PID Controller/Tsamp - Ngain/Passthrough'
//  '<S67>'  : 'controller/Controller/Pressure Control/PID Controller/postSat Signal/Feedback_Path'
//  '<S68>'  : 'controller/Controller/Pressure Control/PID Controller/preSat Signal/Feedback_Path'
//  '<S69>'  : 'controller/Controller/Volume Control/PID Controller'
//  '<S70>'  : 'controller/Controller/Volume Control/Params Selector'
//  '<S71>'  : 'controller/Controller/Volume Control/Read FFW'
//  '<S72>'  : 'controller/Controller/Volume Control/Store FFW'
//  '<S73>'  : 'controller/Controller/Volume Control/PID Controller/Anti-windup'
//  '<S74>'  : 'controller/Controller/Volume Control/PID Controller/D Gain'
//  '<S75>'  : 'controller/Controller/Volume Control/PID Controller/Filter'
//  '<S76>'  : 'controller/Controller/Volume Control/PID Controller/Filter ICs'
//  '<S77>'  : 'controller/Controller/Volume Control/PID Controller/I Gain'
//  '<S78>'  : 'controller/Controller/Volume Control/PID Controller/Ideal P Gain'
//  '<S79>'  : 'controller/Controller/Volume Control/PID Controller/Ideal P Gain Fdbk'
//  '<S80>'  : 'controller/Controller/Volume Control/PID Controller/Integrator'
//  '<S81>'  : 'controller/Controller/Volume Control/PID Controller/Integrator ICs'
//  '<S82>'  : 'controller/Controller/Volume Control/PID Controller/N Copy'
//  '<S83>'  : 'controller/Controller/Volume Control/PID Controller/N Gain'
//  '<S84>'  : 'controller/Controller/Volume Control/PID Controller/P Copy'
//  '<S85>'  : 'controller/Controller/Volume Control/PID Controller/Parallel P Gain'
//  '<S86>'  : 'controller/Controller/Volume Control/PID Controller/Reset Signal'
//  '<S87>'  : 'controller/Controller/Volume Control/PID Controller/Saturation'
//  '<S88>'  : 'controller/Controller/Volume Control/PID Controller/Saturation Fdbk'
//  '<S89>'  : 'controller/Controller/Volume Control/PID Controller/Sum'
//  '<S90>'  : 'controller/Controller/Volume Control/PID Controller/Sum Fdbk'
//  '<S91>'  : 'controller/Controller/Volume Control/PID Controller/Tracking Mode'
//  '<S92>'  : 'controller/Controller/Volume Control/PID Controller/Tracking Mode Sum'
//  '<S93>'  : 'controller/Controller/Volume Control/PID Controller/Tsamp - Integral'
//  '<S94>'  : 'controller/Controller/Volume Control/PID Controller/Tsamp - Ngain'
//  '<S95>'  : 'controller/Controller/Volume Control/PID Controller/postSat Signal'
//  '<S96>'  : 'controller/Controller/Volume Control/PID Controller/preSat Signal'
//  '<S97>'  : 'controller/Controller/Volume Control/PID Controller/Anti-windup/Passthrough'
//  '<S98>'  : 'controller/Controller/Volume Control/PID Controller/D Gain/Disabled'
//  '<S99>'  : 'controller/Controller/Volume Control/PID Controller/Filter/Disabled'
//  '<S100>' : 'controller/Controller/Volume Control/PID Controller/Filter ICs/Disabled'
//  '<S101>' : 'controller/Controller/Volume Control/PID Controller/I Gain/External Parameters'
//  '<S102>' : 'controller/Controller/Volume Control/PID Controller/Ideal P Gain/Passthrough'
//  '<S103>' : 'controller/Controller/Volume Control/PID Controller/Ideal P Gain Fdbk/Passthrough'
//  '<S104>' : 'controller/Controller/Volume Control/PID Controller/Integrator/Discrete'
//  '<S105>' : 'controller/Controller/Volume Control/PID Controller/Integrator ICs/Internal IC'
//  '<S106>' : 'controller/Controller/Volume Control/PID Controller/N Copy/Disabled wSignal Specification'
//  '<S107>' : 'controller/Controller/Volume Control/PID Controller/N Gain/Disabled'
//  '<S108>' : 'controller/Controller/Volume Control/PID Controller/P Copy/Disabled'
//  '<S109>' : 'controller/Controller/Volume Control/PID Controller/Parallel P Gain/External Parameters'
//  '<S110>' : 'controller/Controller/Volume Control/PID Controller/Reset Signal/External Reset'
//  '<S111>' : 'controller/Controller/Volume Control/PID Controller/Saturation/Passthrough'
//  '<S112>' : 'controller/Controller/Volume Control/PID Controller/Saturation Fdbk/Passthrough'
//  '<S113>' : 'controller/Controller/Volume Control/PID Controller/Sum/Sum_PI'
//  '<S114>' : 'controller/Controller/Volume Control/PID Controller/Sum Fdbk/Enabled'
//  '<S115>' : 'controller/Controller/Volume Control/PID Controller/Tracking Mode/Enabled'
//  '<S116>' : 'controller/Controller/Volume Control/PID Controller/Tracking Mode Sum/Tracking Mode'
//  '<S117>' : 'controller/Controller/Volume Control/PID Controller/Tsamp - Integral/Passthrough'
//  '<S118>' : 'controller/Controller/Volume Control/PID Controller/Tsamp - Ngain/Passthrough'
//  '<S119>' : 'controller/Controller/Volume Control/PID Controller/postSat Signal/Feedback_Path'
//  '<S120>' : 'controller/Controller/Volume Control/PID Controller/preSat Signal/Feedback_Path'

#endif                                 // RTW_HEADER_controller_h_

//
// File trailer for generated code.
//
// [EOF]
//