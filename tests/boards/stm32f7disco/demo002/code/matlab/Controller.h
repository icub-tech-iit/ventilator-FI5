//
// Non-Degree Granting Education License -- for use at non-degree
// granting, nonprofit, educational organizations only. Not for
// government, commercial, or other organizational use.
//
// File: Controller.h
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
#ifndef RTW_HEADER_Controller_h_
#define RTW_HEADER_Controller_h_
#include <cmath>
#include "rtwtypes.h"
#include "Controller_types.h"

// Macros for accessing real-time model data structure
#ifndef rtmGetErrorStatus
# define rtmGetErrorStatus(rtm)        ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
# define rtmSetErrorStatus(rtm, val)   ((rtm)->errorStatus = (val))
#endif

// Class declaration for model Controller
class ControllerModelClass {
  // public data and function members
 public:
  // Block signals for system '<S1>/Compensator'
  typedef struct {
    real_T Sum;                        // '<S50>/Sum'
  } B_Compensator_T;

  // Block states (default storage) for system '<S1>/Compensator'
  typedef struct {
    real_T Integrator_DSTATE;          // '<S41>/Integrator'
    int8_T Integrator_PrevResetState;  // '<S41>/Integrator'
  } DW_Compensator_T;

  // Block signals for system '<S1>/Error Statistics'
  typedef struct {
    real_T Abs;                        // '<S5>/Abs'
    real_T Mean;                       // '<S5>/Mean'
  } B_ErrorStatistics_T;

  // Block states (default storage) for system '<S1>/Error Statistics'
  typedef struct {
    real_T DelayLine_Buff[100];        // '<S5>/Delay Line'
    int32_T DelayLine_BUFF_OFFSET;     // '<S5>/Delay Line'
  } DW_ErrorStatistics_T;

  // Block signals for system '<S1>/Filter'
  typedef struct {
    real_T DiscreteFilter;             // '<S6>/Discrete Filter'
  } B_Filter_T;

  // Block states (default storage) for system '<S1>/Filter'
  typedef struct {
    real_T DiscreteFilter_states[2];   // '<S6>/Discrete Filter'
  } DW_Filter_T;

  // Block signals for system '<S1>/Reference Plant'
  typedef struct {
    real_T DiscreteIntegrator;         // '<S8>/Discrete Integrator'
  } B_ReferencePlant_T;

  // Block states (default storage) for system '<S1>/Reference Plant'
  typedef struct {
    real_T DiscreteIntegrator_DSTATE;  // '<S8>/Discrete Integrator'
  } DW_ReferencePlant_T;

  // Block signals (default storage)
  typedef struct {
    boolean_T RelationalOperator1;     // '<S60>/Relational Operator1'
    boolean_T RelationalOperator1_n;   // '<S59>/Relational Operator1'
    B_ReferencePlant_T ReferencePlant_j;// '<S1>/Reference Plant'
    B_Filter_T Filter1;                // '<S1>/Filter1'
    B_Filter_T Filter_i;               // '<S1>/Filter'
    B_ErrorStatistics_T ErrorStatistics_p;// '<S1>/Error Statistics'
    B_Compensator_T Compensator_k;     // '<S1>/Compensator'
  } B_Controller_T;

  // Block states (default storage) for system '<Root>'
  typedef struct {
    real_T UnitDelay_DSTATE;           // '<S1>/Unit Delay'
    real_T UnitDelay2_DSTATE;          // '<S1>/Unit Delay2'
    boolean_T UnitDelay_DSTATE_e;      // '<S58>/Unit Delay'
    uint8_T is_active_c3_Controller;   // '<S1>/Compensator Handling'
    uint8_T is_c3_Controller;          // '<S1>/Compensator Handling'
    DW_ReferencePlant_T ReferencePlant_j;// '<S1>/Reference Plant'
    DW_Filter_T Filter1;               // '<S1>/Filter1'
    DW_Filter_T Filter_i;              // '<S1>/Filter'
    DW_ErrorStatistics_T ErrorStatistics_p;// '<S1>/Error Statistics'
    DW_Compensator_T Compensator_k;    // '<S1>/Compensator'
  } DW_Controller_T;

  // External inputs (root inport signals with default storage)
  typedef struct {
    real_T reference;                  // '<Root>/reference'
    CompensatorState compensator_state;// '<Root>/compensator_state'
    real_T plant_output;               // '<Root>/plant_output'
  } ExtU_Controller_T;

  // External outputs (root outports fed by signals with default storage)
  typedef struct {
    real_T controller_output;          // '<Root>/controller_output'
    real_T controller_reference;       // '<Root>/controller_reference'
    real_T plant_reference;            // '<Root>/plant_reference'
    real_T error_statistics;           // '<Root>/error_statistics'
    boolean_T enable_compensation;     // '<Root>/enable_compensation'
  } ExtY_Controller_T;

  // Parameters for system: '<S1>/Compensator'
  struct P_Compensator_T {
    real_T DiscretePIDController_InitialCo;
                              // Mask Parameter: DiscretePIDController_InitialCo
                                 //  Referenced by: '<S41>/Integrator'

    real_T Integrator_gainval;         // Computed Parameter: Integrator_gainval
                                          //  Referenced by: '<S41>/Integrator'

  };

  // Parameters for system: '<S1>/Filter'
  struct P_Filter_T {
    real_T DiscreteFilter_NumCoef[3];  // Expression: [4.5e-4 9e-4 4.5e-4]
                                          //  Referenced by: '<S6>/Discrete Filter'

    real_T DiscreteFilter_DenCoef[3];  // Expression: [1 -1.921 0.9231]
                                          //  Referenced by: '<S6>/Discrete Filter'

    real_T DiscreteFilter_InitialStates;// Expression: 0
                                           //  Referenced by: '<S6>/Discrete Filter'

  };

  // Parameters for system: '<S1>/Reference Plant'
  struct P_ReferencePlant_T {
    real_T DiscreteIntegrator_gainval;
                               // Computed Parameter: DiscreteIntegrator_gainval
                                  //  Referenced by: '<S8>/Discrete Integrator'

  };

  // Parameters (default storage)
  struct P_Controller_T {
    real_T AutoCompensator_ThresHystMax;// Variable: AutoCompensator_ThresHystMax
                                           //  Referenced by: '<S1>/Compensator Handling'

    real_T AutoCompensator_ThresHystMin;// Variable: AutoCompensator_ThresHystMin
                                           //  Referenced by: '<S1>/Compensator Handling'

    real_T Compensator_Ki;             // Variable: Compensator_Ki
                                          //  Referenced by: '<S38>/Integral Gain'

    real_T Compensator_Kp;             // Variable: Compensator_Kp
                                          //  Referenced by: '<S46>/Proportional Gain'

    real_T Plant_IC;                   // Variable: Plant_IC
                                          //  Referenced by:
                                          //    '<S1>/Unit Delay'
                                          //    '<S1>/Unit Delay2'
                                          //    '<S8>/Discrete Integrator'

    real_T Plant_Max;                  // Variable: Plant_Max
                                          //  Referenced by: '<S8>/Discrete Integrator'

    real_T Plant_Min;                  // Variable: Plant_Min
                                          //  Referenced by: '<S8>/Discrete Integrator'

    real_T EdgeDetector_model;         // Mask Parameter: EdgeDetector_model
                                          //  Referenced by: '<S58>/Constant1'

    boolean_T EdgeDetector_ic;         // Mask Parameter: EdgeDetector_ic
                                          //  Referenced by: '<S58>/Unit Delay'

    real_T Constant_Value;             // Expression: 0
                                          //  Referenced by: '<S1>/Constant'

    real_T eitheredge_Value[2];        // Expression: [1 1]
                                          //  Referenced by: '<S58>/either edge'

    real_T negedge_Value[2];           // Expression: [0 1]
                                          //  Referenced by: '<S58>/neg. edge'

    real_T posedge_Value[2];           // Expression: [1 0]
                                          //  Referenced by: '<S58>/pos. edge'

    boolean_T OUT_Y0;                  // Computed Parameter: OUT_Y0
                                          //  Referenced by: '<S59>/OUT'

    boolean_T OUT_Y0_d;                // Computed Parameter: OUT_Y0_d
                                          //  Referenced by: '<S60>/OUT'

    P_ReferencePlant_T ReferencePlant_j;// '<S1>/Reference Plant'
    P_Filter_T Filter1;                // '<S1>/Filter1'
    P_Filter_T Filter_i;               // '<S1>/Filter'
    P_Compensator_T Compensator_k;     // '<S1>/Compensator'
  };

  // Real-time Model Data Structure
  struct RT_MODEL_Controller_T {
    const char_T * volatile errorStatus;
  };

  // model initialize function
  void initialize();

  // model step function
  void step();

  // model terminate function
  void terminate();

  // Constructor
  ControllerModelClass();

  // Destructor
  ~ControllerModelClass();

  // Root-level structure-based inputs set method

  // Root inports set method
  void setExternalInputs(const ExtU_Controller_T* pExtU_Controller_T)
  {
    Controller_U = *pExtU_Controller_T;
  }

  // Root-level structure-based outputs get method

  // Root outports get method
  const ControllerModelClass::ExtY_Controller_T & getExternalOutputs() const
  {
    return Controller_Y;
  }

  // Real-Time Model get method
  ControllerModelClass::RT_MODEL_Controller_T * getRTM();

  // private data and function members
 private:
  // Tunable parameters
  static P_Controller_T Controller_P;

  // Block signals
  B_Controller_T Controller_B;

  // Block states
  DW_Controller_T Controller_DW;

  // External inputs
  ExtU_Controller_T Controller_U;

  // External outputs
  ExtY_Controller_T Controller_Y;

  // Real-Time Model
  RT_MODEL_Controller_T Controller_M;

  // private member function(s) for subsystem '<S1>/Compensator'
  void Compensator_Init(DW_Compensator_T *localDW, P_Compensator_T *localP);
  void Compensator(real_T rtu_e, boolean_T rtu_reset, B_Compensator_T *localB,
                   DW_Compensator_T *localDW, P_Compensator_T *localP,
                   P_Controller_T *Controller_P);

  // private member function(s) for subsystem '<S1>/Error Statistics'
  void ErrorStatistics(real_T rtu_in, B_ErrorStatistics_T *localB,
                       DW_ErrorStatistics_T *localDW);

  // private member function(s) for subsystem '<S1>/Filter'
  void Filter_Init(DW_Filter_T *localDW, P_Filter_T *localP);
  void Filter(real_T rtu_e, B_Filter_T *localB, DW_Filter_T *localDW, P_Filter_T
              *localP);

  // private member function(s) for subsystem '<S1>/Reference Plant'
  void ReferencePlant_Init(DW_ReferencePlant_T *localDW, P_Controller_T
    *Controller_P);
  void ReferencePlant(real_T rtu_u, B_ReferencePlant_T *localB,
                      DW_ReferencePlant_T *localDW, P_ReferencePlant_T *localP,
                      P_Controller_T *Controller_P);
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
//  hilite_system('minJerk/Controller')    - opens subsystem minJerk/Controller
//  hilite_system('minJerk/Controller/Kp') - opens and selects block Kp
//
//  Here is the system hierarchy for this model
//
//  '<Root>' : 'minJerk'
//  '<S1>'   : 'minJerk/Controller'
//  '<S2>'   : 'minJerk/Controller/Compensator'
//  '<S3>'   : 'minJerk/Controller/Compensator Handling'
//  '<S4>'   : 'minJerk/Controller/Edge Detector'
//  '<S5>'   : 'minJerk/Controller/Error Statistics'
//  '<S6>'   : 'minJerk/Controller/Filter'
//  '<S7>'   : 'minJerk/Controller/Filter1'
//  '<S8>'   : 'minJerk/Controller/Reference Plant'
//  '<S9>'   : 'minJerk/Controller/Compensator/Discrete PID Controller'
//  '<S10>'  : 'minJerk/Controller/Compensator/Discrete PID Controller/Anti-windup'
//  '<S11>'  : 'minJerk/Controller/Compensator/Discrete PID Controller/D Gain'
//  '<S12>'  : 'minJerk/Controller/Compensator/Discrete PID Controller/Filter'
//  '<S13>'  : 'minJerk/Controller/Compensator/Discrete PID Controller/Filter ICs'
//  '<S14>'  : 'minJerk/Controller/Compensator/Discrete PID Controller/I Gain'
//  '<S15>'  : 'minJerk/Controller/Compensator/Discrete PID Controller/Ideal P Gain'
//  '<S16>'  : 'minJerk/Controller/Compensator/Discrete PID Controller/Ideal P Gain Fdbk'
//  '<S17>'  : 'minJerk/Controller/Compensator/Discrete PID Controller/Integrator'
//  '<S18>'  : 'minJerk/Controller/Compensator/Discrete PID Controller/Integrator ICs'
//  '<S19>'  : 'minJerk/Controller/Compensator/Discrete PID Controller/N Copy'
//  '<S20>'  : 'minJerk/Controller/Compensator/Discrete PID Controller/N Gain'
//  '<S21>'  : 'minJerk/Controller/Compensator/Discrete PID Controller/P Copy'
//  '<S22>'  : 'minJerk/Controller/Compensator/Discrete PID Controller/Parallel P Gain'
//  '<S23>'  : 'minJerk/Controller/Compensator/Discrete PID Controller/Reset Signal'
//  '<S24>'  : 'minJerk/Controller/Compensator/Discrete PID Controller/Saturation'
//  '<S25>'  : 'minJerk/Controller/Compensator/Discrete PID Controller/Saturation Fdbk'
//  '<S26>'  : 'minJerk/Controller/Compensator/Discrete PID Controller/Sum'
//  '<S27>'  : 'minJerk/Controller/Compensator/Discrete PID Controller/Sum Fdbk'
//  '<S28>'  : 'minJerk/Controller/Compensator/Discrete PID Controller/Tracking Mode'
//  '<S29>'  : 'minJerk/Controller/Compensator/Discrete PID Controller/Tracking Mode Sum'
//  '<S30>'  : 'minJerk/Controller/Compensator/Discrete PID Controller/Tsamp - Integral'
//  '<S31>'  : 'minJerk/Controller/Compensator/Discrete PID Controller/Tsamp - Ngain'
//  '<S32>'  : 'minJerk/Controller/Compensator/Discrete PID Controller/postSat Signal'
//  '<S33>'  : 'minJerk/Controller/Compensator/Discrete PID Controller/preSat Signal'
//  '<S34>'  : 'minJerk/Controller/Compensator/Discrete PID Controller/Anti-windup/Passthrough'
//  '<S35>'  : 'minJerk/Controller/Compensator/Discrete PID Controller/D Gain/Disabled'
//  '<S36>'  : 'minJerk/Controller/Compensator/Discrete PID Controller/Filter/Disabled'
//  '<S37>'  : 'minJerk/Controller/Compensator/Discrete PID Controller/Filter ICs/Disabled'
//  '<S38>'  : 'minJerk/Controller/Compensator/Discrete PID Controller/I Gain/Internal Parameters'
//  '<S39>'  : 'minJerk/Controller/Compensator/Discrete PID Controller/Ideal P Gain/Passthrough'
//  '<S40>'  : 'minJerk/Controller/Compensator/Discrete PID Controller/Ideal P Gain Fdbk/Disabled'
//  '<S41>'  : 'minJerk/Controller/Compensator/Discrete PID Controller/Integrator/Discrete'
//  '<S42>'  : 'minJerk/Controller/Compensator/Discrete PID Controller/Integrator ICs/Internal IC'
//  '<S43>'  : 'minJerk/Controller/Compensator/Discrete PID Controller/N Copy/Disabled wSignal Specification'
//  '<S44>'  : 'minJerk/Controller/Compensator/Discrete PID Controller/N Gain/Disabled'
//  '<S45>'  : 'minJerk/Controller/Compensator/Discrete PID Controller/P Copy/Disabled'
//  '<S46>'  : 'minJerk/Controller/Compensator/Discrete PID Controller/Parallel P Gain/Internal Parameters'
//  '<S47>'  : 'minJerk/Controller/Compensator/Discrete PID Controller/Reset Signal/External Reset'
//  '<S48>'  : 'minJerk/Controller/Compensator/Discrete PID Controller/Saturation/Passthrough'
//  '<S49>'  : 'minJerk/Controller/Compensator/Discrete PID Controller/Saturation Fdbk/Disabled'
//  '<S50>'  : 'minJerk/Controller/Compensator/Discrete PID Controller/Sum/Sum_PI'
//  '<S51>'  : 'minJerk/Controller/Compensator/Discrete PID Controller/Sum Fdbk/Disabled'
//  '<S52>'  : 'minJerk/Controller/Compensator/Discrete PID Controller/Tracking Mode/Disabled'
//  '<S53>'  : 'minJerk/Controller/Compensator/Discrete PID Controller/Tracking Mode Sum/Passthrough'
//  '<S54>'  : 'minJerk/Controller/Compensator/Discrete PID Controller/Tsamp - Integral/Passthrough'
//  '<S55>'  : 'minJerk/Controller/Compensator/Discrete PID Controller/Tsamp - Ngain/Passthrough'
//  '<S56>'  : 'minJerk/Controller/Compensator/Discrete PID Controller/postSat Signal/Forward_Path'
//  '<S57>'  : 'minJerk/Controller/Compensator/Discrete PID Controller/preSat Signal/Forward_Path'
//  '<S58>'  : 'minJerk/Controller/Edge Detector/Model'
//  '<S59>'  : 'minJerk/Controller/Edge Detector/Model/NEGATIVE Edge'
//  '<S60>'  : 'minJerk/Controller/Edge Detector/Model/POSITIVE Edge'

#endif                                 // RTW_HEADER_Controller_h_

//
// File trailer for generated code.
//
// [EOF]
//
