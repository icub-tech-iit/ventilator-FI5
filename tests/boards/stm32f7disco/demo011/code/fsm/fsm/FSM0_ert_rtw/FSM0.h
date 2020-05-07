//
// Non-Degree Granting Education License -- for use at non-degree
// granting, nonprofit, educational organizations only. Not for
// government, commercial, or other organizational use.
//
// File: FSM0.h
//
// Code generated for Simulink model 'FSM0'.
//
// Model version                  : 1.697
// Simulink Coder version         : 9.3 (R2020a) 18-Nov-2019
// C/C++ source code generated on : Thu May  7 10:44:33 2020
//
// Target selection: ert.tlc
// Embedded hardware selection: Intel->x86-64 (Windows64)
// Code generation objectives: Unspecified
// Validation result: Not run
//
#ifndef RTW_HEADER_FSM0_h_
#define RTW_HEADER_FSM0_h_
#include "rtwtypes.h"
#include "FSM0_types.h"

// Macros for accessing real-time model data structure
#ifndef rtmGetErrorStatus
# define rtmGetErrorStatus(rtm)        ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
# define rtmSetErrorStatus(rtm, val)   ((rtm)->errorStatus = (val))
#endif

// Class declaration for model FSM0
class FSM0ModelClass {
  // public data and function members
 public:
  // Block signals (default storage)
  typedef struct {
    real32_T MatrixConcatenate[48];    // '<S3>/Matrix Concatenate'
  } B_FSM0_T;

  // Block states (default storage) for system '<Root>'
  typedef struct {
    real32_T test1_dc_peak;            // '<S1>/FSM'
    real32_T test1_period;             // '<S1>/FSM'
    real32_T test2_period;             // '<S1>/FSM'
    int32_T sfEvent;                   // '<S1>/FSM'
    int32_T chartAbsoluteTimeCounter;  // '<S1>/FSM'
    int32_T durationLastReferenceTick_1;// '<S1>/FSM'
    int32_T durationLastReferenceTick_1_f;// '<S1>/FSM'
    int32_T durationLastReferenceTick_2;// '<S1>/FSM'
    int32_T durationLastReferenceTick_3;// '<S1>/FSM'
    int32_T durationLastReferenceTick_1_g;// '<S1>/FSM'
    int32_T durationLastReferenceTick_1_g5;// '<S1>/FSM'
    int32_T durationLastReferenceTick_4;// '<S1>/FSM'
    int32_T durationLastReferenceTick_5;// '<S1>/FSM'
    int32_T durationLastReferenceTick_6;// '<S1>/FSM'
    int32_T durationLastReferenceTick_1_h;// '<S1>/FSM'
    int32_T durationLastReferenceTick_2_b;// '<S1>/FSM'
    int32_T durationLastReferenceTick_3_b;// '<S1>/FSM'
    int32_T durationLastReferenceTick_4_g;// '<S1>/FSM'
    int32_T durationLastReferenceTick_1_i;// '<S1>/FSM'
    int32_T durationLastReferenceTick_1_gm;// '<S1>/FSM'
    int32_T durationLastReferenceTick_5_c;// '<S1>/FSM'
    uint32_T MultiSelector_Maps_latch; // '<S1>/FSM'
    uint32_T temporalCounter_i1;       // '<S1>/FSM'
    uint32_T temporalCounter_i2;       // '<S1>/FSM'
    uint8_T is_active_c3_FSM0;         // '<S1>/FSM'
    uint8_T is_STATES_HANDLING;        // '<S1>/FSM'
    uint8_T is_active_STATES_HANDLING; // '<S1>/FSM'
    uint8_T is_IDLE_BLINKING;          // '<S1>/FSM'
    uint8_T is_active_IDLE_BLINKING;   // '<S1>/FSM'
    boolean_T enter_test;              // '<S1>/FSM'
    boolean_T exit_test;               // '<S1>/FSM'
    boolean_T condWasTrueAtLastTimeStep_1;// '<S1>/FSM'
    boolean_T condWasTrueAtLastTimeStep_1_o;// '<S1>/FSM'
    boolean_T condWasTrueAtLastTimeStep_2;// '<S1>/FSM'
    boolean_T condWasTrueAtLastTimeStep_3;// '<S1>/FSM'
    boolean_T condWasTrueAtLastTimeStep_1_d;// '<S1>/FSM'
    boolean_T condWasTrueAtLastTimeStep_1_e;// '<S1>/FSM'
    boolean_T condWasTrueAtLastTimeStep_4;// '<S1>/FSM'
    boolean_T condWasTrueAtLastTimeStep_5;// '<S1>/FSM'
    boolean_T condWasTrueAtLastTimeStep_6;// '<S1>/FSM'
    boolean_T condWasTrueAtLastTimeStep_1_p;// '<S1>/FSM'
    boolean_T condWasTrueAtLastTimeStep_2_b;// '<S1>/FSM'
    boolean_T condWasTrueAtLastTimeStep_3_m;// '<S1>/FSM'
    boolean_T condWasTrueAtLastTimeStep_4_f;// '<S1>/FSM'
    boolean_T condWasTrueAtLastTimeStep_1_g;// '<S1>/FSM'
    boolean_T condWasTrueAtLastTimeStep_1_a;// '<S1>/FSM'
    boolean_T condWasTrueAtLastTimeStep_5_c;// '<S1>/FSM'
  } DW_FSM0_T;

  // External inputs (root inport signals with default storage)
  typedef struct {
    StateButton buttons[4];            // '<Root>/buttons'
    real32_T knobs_in[4];              // '<Root>/knobs_in'
    real32_T Controller_Signals[9];    // '<Root>/Controller_Signals'
    uint32_T MultiSelector_Maps;       // '<Root>/MultiSelector_Maps'
    StateButton MultiSelector_Go;      // '<Root>/MultiSelector_Go'
  } ExtU_FSM0_T;

  // External outputs (root outports fed by signals with default storage)
  typedef struct {
    real32_T knobs_out[4];             // '<Root>/knobs_out'
    MapIndex knobs_transfer;           // '<Root>/knobs_transfer'
    real32_T test_IV_dc;               // '<Root>/test_IV_dc'
    boolean_T test_OV_cmd;             // '<Root>/test_OV_cmd'
    StateLed ledsSTATES[4];            // '<Root>/ledsSTATES'
    StateLed ledALARM;                 // '<Root>/ledALARM'
    ControlMode Controller_Mode;       // '<Root>/Controller_Mode'
  } ExtY_FSM0_T;

  // Parameters (default storage)
  struct P_FSM0_T {
    real32_T FSM_Ts;                   // Computed Parameter: FSM_Ts
                                          //  Referenced by: '<S1>/FSM'

    real32_T FSM_blinking_period;     // Computed Parameter: FSM_blinking_period
                                         //  Referenced by: '<S1>/FSM'

    real32_T FSM_debounce_tmo;         // Computed Parameter: FSM_debounce_tmo
                                          //  Referenced by: '<S1>/FSM'

    real32_T FSM_test_tmo;             // Computed Parameter: FSM_test_tmo
                                          //  Referenced by: '<S1>/FSM'

    real32_T BPM1min_Value[2];         // Computed Parameter: BPM1min_Value
                                          //  Referenced by: '<S7>/BPM [1//min]'

    real32_T PIPcmH2O_Value[2];        // Computed Parameter: PIPcmH2O_Value
                                          //  Referenced by: '<S7>/PIP [cmH2O]'

    real32_T IE_ratio_Value[2];        // Computed Parameter: IE_ratio_Value
                                          //  Referenced by: '<S7>/IE_ratio [-]'

    real32_T TDml_Value[2];            // Computed Parameter: TDml_Value
                                          //  Referenced by: '<S7>/TD [ml]'

    real32_T Proportional_Value[2];    // Computed Parameter: Proportional_Value
                                          //  Referenced by: '<S9>/Proportional'

    real32_T Integral_Value[2];        // Computed Parameter: Integral_Value
                                          //  Referenced by: '<S9>/Integral'

    real32_T ProportionalRecovery_Value[2];
                               // Computed Parameter: ProportionalRecovery_Value
                                  //  Referenced by: '<S9>/Proportional Recovery'

    real32_T IntegralRecovery_Value[2];
                                   // Computed Parameter: IntegralRecovery_Value
                                      //  Referenced by: '<S9>/Integral Recovery'

    real32_T Proportional_Value_j[2];// Computed Parameter: Proportional_Value_j
                                        //  Referenced by: '<S5>/Proportional'

    real32_T Integral_Value_k[2];      // Computed Parameter: Integral_Value_k
                                          //  Referenced by: '<S5>/Integral'

    real32_T ProportionalRecovery_Value_g[2];
                             // Computed Parameter: ProportionalRecovery_Value_g
                                //  Referenced by: '<S5>/Proportional Recovery'

    real32_T IntegralRecovery_Value_b[2];
                                 // Computed Parameter: IntegralRecovery_Value_b
                                    //  Referenced by: '<S5>/Integral Recovery'

    real32_T FeedForward_Value[2];     // Computed Parameter: FeedForward_Value
                                          //  Referenced by: '<S6>/Feed-Forward'

    real32_T FeedForwardRecovery_Value[2];
                                // Computed Parameter: FeedForwardRecovery_Value
                                   //  Referenced by: '<S6>/Feed-Forward Recovery'

    real32_T PIPStepPerc_Value[2];     // Computed Parameter: PIPStepPerc_Value
                                          //  Referenced by: '<S6>/PIP Step Perc [%]'

    real32_T PIPRisePerc_Value[2];     // Computed Parameter: PIPRisePerc_Value
                                          //  Referenced by: '<S6>/PIP Rise Perc [%]'

    real32_T EnableTrigger_Value[2];  // Computed Parameter: EnableTrigger_Value
                                         //  Referenced by: '<S4>/Enable Trigger'

    real32_T PressureThresholdcmH2O_Value[2];
                             // Computed Parameter: PressureThresholdcmH2O_Value
                                //  Referenced by: '<S4>/Pressure Threshold [cmH2O]'

    real32_T NotUsed1_Value[2];        // Computed Parameter: NotUsed1_Value
                                          //  Referenced by: '<S4>/Not Used 1'

    real32_T NotUsed2_Value[2];        // Computed Parameter: NotUsed2_Value
                                          //  Referenced by: '<S4>/Not Used 2'

    real32_T Test1Peak_Value[2];       // Computed Parameter: Test1Peak_Value
                                          //  Referenced by: '<S8>/Test1 Peak [%]'

    real32_T Test1Periods_Value[2];    // Computed Parameter: Test1Periods_Value
                                          //  Referenced by: '<S8>/Test1 Period [s]'

    real32_T Test2Periods_Value[2];    // Computed Parameter: Test2Periods_Value
                                          //  Referenced by: '<S8>/Test2 Period [s]'

    real32_T NotUsed1_Value_h[2];      // Computed Parameter: NotUsed1_Value_h
                                          //  Referenced by: '<S8>/Not Used 1'

  };

  // Real-time Model Data Structure
  struct RT_MODEL_FSM0_T {
    const char_T * volatile errorStatus;
  };

  // Tunable parameters
  static P_FSM0_T FSM0_P;

  // model initialize function
  void initialize();

  // model step function
  void step();

  // model terminate function
  void terminate();

  // Constructor
  FSM0ModelClass();

  // Destructor
  ~FSM0ModelClass();

  // Block parameters get method
  const FSM0ModelClass::P_FSM0_T & getBlockParameters() const;

  // Block parameters set method
  void setBlockParameters(const P_FSM0_T *pFSM0_P);

  // Root-level structure-based inputs set method

  // Root inports set method
  void setExternalInputs(const ExtU_FSM0_T* pExtU_FSM0_T)
  {
    FSM0_U = *pExtU_FSM0_T;
  }

  // Root-level structure-based outputs get method

  // Root outports get method
  const FSM0ModelClass::ExtY_FSM0_T & getExternalOutputs() const
  {
    return FSM0_Y;
  }

  // Real-Time Model get method
  FSM0ModelClass::RT_MODEL_FSM0_T * getRTM();

  // private data and function members
 private:
  // Block signals
  B_FSM0_T FSM0_B;

  // Block states
  DW_FSM0_T FSM0_DW;

  // External inputs
  ExtU_FSM0_T FSM0_U;

  // External outputs
  ExtY_FSM0_T FSM0_Y;

  // Real-Time Model
  RT_MODEL_FSM0_T FSM0_M;

  // private member function(s) for subsystem '<Root>'
  real_T FSM0_unavailable_button(real_T state);
  int32_T FSM0_safe_cast_to_StateLed(int32_T input);
  void FSM0_CPAP(void);
  void FSM0_apply_map(real_T n);
  int32_T FSM0_safe_cast_to_MapIndex(int32_T input);
  void FSM0_latch_multiselector(void);
  void FSM0_IDLE_BLINKING(void);
  void FSM0_IDLE_j(void);
  void FSM0_INIT(void);
  void FSM0_PRVC(void);
  void FSM0_IDLE(void);
  void FSM0_STATES_HANDLING(void);
};

//-
//  The generated code includes comments that allow you to trace directly
//  back to the appropriate location in the model.  The basic format
//  is <system>/block_name, where system is the system number (uniquely
//  assigned by Simulink) and block_name is the name of the block.
//
//  Note that this particular code originates from a subsystem build,
//  and has its own system numbers different from the parent model.
//  Refer to the system hierarchy for this subsystem below, and use the
//  MATLAB hilite_system command to trace the generated code back
//  to the parent model.  For example,
//
//  hilite_system('fsm/FSM')    - opens subsystem fsm/FSM
//  hilite_system('fsm/FSM/Kp') - opens and selects block Kp
//
//  Here is the system hierarchy for this model
//
//  '<Root>' : 'fsm'
//  '<S1>'   : 'fsm/FSM'
//  '<S2>'   : 'fsm/FSM/FSM'
//  '<S3>'   : 'fsm/FSM/MAPS'
//  '<S4>'   : 'fsm/FSM/MAPS/AutoTrigger'
//  '<S5>'   : 'fsm/FSM/MAPS/PRVC_1'
//  '<S6>'   : 'fsm/FSM/MAPS/PRVC_2'
//  '<S7>'   : 'fsm/FSM/MAPS/Standard'
//  '<S8>'   : 'fsm/FSM/MAPS/Test'
//  '<S9>'   : 'fsm/FSM/MAPS/VCV'

#endif                                 // RTW_HEADER_FSM0_h_

//
// File trailer for generated code.
//
// [EOF]
//
