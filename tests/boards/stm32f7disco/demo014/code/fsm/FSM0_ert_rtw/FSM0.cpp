//
// Non-Degree Granting Education License -- for use at non-degree
// granting, nonprofit, educational organizations only. Not for
// government, commercial, or other organizational use.
//
// File: FSM0.cpp
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

// Named constants for Chart: '<S1>/FSM'
const int32_T FSM0_CALL_EVENT = -1;
const uint8_T FSM0_IN_CPAP = 1U;
const uint8_T FSM0_IN_DEFINITIVE_ALARM = 1U;
const uint8_T FSM0_IN_IDLE = 2U;
const uint8_T FSM0_IN_IDLE_n = 5U;
const uint8_T FSM0_IN_INIT = 1U;
const uint8_T FSM0_IN_INLET_OVERPRESSURE = 2U;
const uint8_T FSM0_IN_NO_ACTIVE_CHILD = 0U;
const uint8_T FSM0_IN_NO_ALARMS = 3U;
const uint8_T FSM0_IN_OFF = 3U;
const uint8_T FSM0_IN_OFF_c = 2U;
const uint8_T FSM0_IN_ON = 3U;
const uint8_T FSM0_IN_PRVC = 4U;
const uint8_T FSM0_IN_TEST_1 = 6U;
const uint8_T FSM0_IN_TEST_2 = 7U;
const uint8_T FSM0_IN_TRANSITIVE_ALARMS = 4U;
const uint8_T FSM0_IN_VCV = 8U;
const int32_T FSM0_event_blink_IDLE_off = 0;
const int32_T FSM0_event_blink_IDLE_on = 1;
const int32_T FSM0_event_idle_controller = 2;
const int32_T FSM0_event_switch_off = 3;
static void rate_scheduler(FSM0ModelClass::RT_MODEL_FSM0_T *const FSM0_M);

//
//   This function updates active task flag for each subrate.
// The function is called at model base rate, hence the
// generated code self-manages all its subrates.
//
static void rate_scheduler(FSM0ModelClass::RT_MODEL_FSM0_T *const FSM0_M)
{
  // Compute which subrates run during the next base time step.  Subrates
  //  are an integer multiple of the base rate counter.  Therefore, the subtask
  //  counter is reset when it reaches its limit (zero means run).

  (FSM0_M->Timing.TaskCounters.TID[1])++;
  if ((FSM0_M->Timing.TaskCounters.TID[1]) > 3) {// Sample time: [0.04s, 0.0s]
    FSM0_M->Timing.TaskCounters.TID[1] = 0;
  }
}

// Function for Chart: '<S1>/FSM'
real_T FSM0ModelClass::FSM0_unavailable_button(real_T state)
{
  return state == 1.0;
}

int32_T FSM0ModelClass::FSM0_safe_cast_to_StateLed(int32_T input)
{
  int32_T y;

  // Initialize output value to default value for StateLed (Off)
  y = 0;
  if ((input >= 0) && (input <= 1)) {
    // Set output value to input value if it is a member of StateLed
    y = input;
  }

  return y;
}

// Function for Chart: '<S1>/FSM'
void FSM0ModelClass::FSM0_CPAP(void)
{
  boolean_T condIsTrue;
  real_T tmp;
  int32_T condIsTrue_tmp;
  int32_T ledsSTATES_tmp;
  int32_T ledsSTATES_tmp_0;
  int32_T ledsSTATES_tmp_1;
  switch (FSM0_DW.sfEvent) {
   case FSM0_event_switch_off:
    FSM0_DW.is_STATES_HANDLING = FSM0_IN_OFF;

    // Outport: '<Root>/ledsSTATES'
    FSM0_Y.ledsSTATES[static_cast<int32_T>(IDLE)] = Off;
    FSM0_Y.ledsSTATES[static_cast<int32_T>(CPAP)] = Off;
    FSM0_Y.ledsSTATES[static_cast<int32_T>(VCV)] = Off;
    FSM0_Y.ledsSTATES[static_cast<int32_T>(PRVC)] = Off;

    // Outport: '<Root>/Controller_Mode'
    FSM0_Y.Controller_Mode = IDLE;
    break;

   case FSM0_event_idle_controller:
    FSM0_DW.enter_test = false;
    FSM0_DW.durationLastReferenceTick_6 = FSM0_DW.chartAbsoluteTimeCounter;
    FSM0_DW.durationLastReferenceTick_5 = FSM0_DW.chartAbsoluteTimeCounter;
    FSM0_DW.durationLastReferenceTick_4 = FSM0_DW.chartAbsoluteTimeCounter;
    FSM0_DW.durationLastReferenceTick_3 = FSM0_DW.chartAbsoluteTimeCounter;
    FSM0_DW.durationLastReferenceTick_2 = FSM0_DW.chartAbsoluteTimeCounter;
    FSM0_DW.durationLastReferenceTick_1_o = FSM0_DW.chartAbsoluteTimeCounter;
    FSM0_DW.is_STATES_HANDLING = FSM0_IN_IDLE;
    ledsSTATES_tmp = static_cast<int32_T>(IDLE);

    // Outport: '<Root>/ledsSTATES'
    FSM0_Y.ledsSTATES[ledsSTATES_tmp] = On;
    ledsSTATES_tmp_0 = static_cast<int32_T>(CPAP);

    // Outport: '<Root>/ledsSTATES'
    FSM0_Y.ledsSTATES[ledsSTATES_tmp_0] = Off;
    ledsSTATES_tmp_1 = static_cast<int32_T>(VCV);

    // Outport: '<Root>/ledsSTATES'
    FSM0_Y.ledsSTATES[ledsSTATES_tmp_1] = Off;
    condIsTrue_tmp = static_cast<int32_T>(PRVC);

    // Outport: '<Root>/ledsSTATES'
    FSM0_Y.ledsSTATES[condIsTrue_tmp] = Off;

    // Outport: '<Root>/Controller_Mode'
    FSM0_Y.Controller_Mode = IDLE;

    // Inport: '<Root>/buttons'
    FSM0_DW.condWasTrueAtLastTimeStep_1_g = (FSM0_U.buttons[ledsSTATES_tmp] ==
      Pressed);
    FSM0_DW.condWasTrueAtLastTimeStep_2 = (FSM0_U.buttons[ledsSTATES_tmp] ==
      Released);
    FSM0_DW.condWasTrueAtLastTimeStep_3 = (FSM0_U.buttons[ledsSTATES_tmp_0] ==
      Pressed);
    FSM0_DW.condWasTrueAtLastTimeStep_4 = (FSM0_U.buttons[ledsSTATES_tmp_1] ==
      Pressed);
    FSM0_DW.condWasTrueAtLastTimeStep_5 = (FSM0_U.buttons[condIsTrue_tmp] ==
      Pressed);

    // Inport: '<Root>/MultiSelector_Go'
    FSM0_DW.condWasTrueAtLastTimeStep_6 = (FSM0_U.MultiSelector_Go == Pressed);
    break;

   default:
    condIsTrue_tmp = static_cast<int32_T>(IDLE);

    // Inport: '<Root>/buttons'
    condIsTrue = (FSM0_U.buttons[condIsTrue_tmp] == Pressed);
    if ((!condIsTrue) || (!FSM0_DW.condWasTrueAtLastTimeStep_1_h)) {
      FSM0_DW.durationLastReferenceTick_1_l = FSM0_DW.chartAbsoluteTimeCounter;
    }

    FSM0_DW.condWasTrueAtLastTimeStep_1_h = condIsTrue;
    if (FSM0_DW.chartAbsoluteTimeCounter - FSM0_DW.durationLastReferenceTick_1_l
        >= FSM0_P.FSM_debounce_tmo * 100.0F) {
      FSM0_DW.enter_test = false;
      FSM0_DW.durationLastReferenceTick_6 = FSM0_DW.chartAbsoluteTimeCounter;
      FSM0_DW.durationLastReferenceTick_5 = FSM0_DW.chartAbsoluteTimeCounter;
      FSM0_DW.durationLastReferenceTick_4 = FSM0_DW.chartAbsoluteTimeCounter;
      FSM0_DW.durationLastReferenceTick_3 = FSM0_DW.chartAbsoluteTimeCounter;
      FSM0_DW.durationLastReferenceTick_2 = FSM0_DW.chartAbsoluteTimeCounter;
      FSM0_DW.durationLastReferenceTick_1_o = FSM0_DW.chartAbsoluteTimeCounter;
      FSM0_DW.is_STATES_HANDLING = FSM0_IN_IDLE;

      // Outport: '<Root>/ledsSTATES'
      FSM0_Y.ledsSTATES[condIsTrue_tmp] = On;
      ledsSTATES_tmp = static_cast<int32_T>(CPAP);

      // Outport: '<Root>/ledsSTATES'
      FSM0_Y.ledsSTATES[ledsSTATES_tmp] = Off;
      ledsSTATES_tmp_0 = static_cast<int32_T>(VCV);

      // Outport: '<Root>/ledsSTATES'
      FSM0_Y.ledsSTATES[ledsSTATES_tmp_0] = Off;
      ledsSTATES_tmp_1 = static_cast<int32_T>(PRVC);

      // Outport: '<Root>/ledsSTATES'
      FSM0_Y.ledsSTATES[ledsSTATES_tmp_1] = Off;

      // Outport: '<Root>/Controller_Mode'
      FSM0_Y.Controller_Mode = IDLE;

      // Inport: '<Root>/buttons'
      FSM0_DW.condWasTrueAtLastTimeStep_1_g = (FSM0_U.buttons[condIsTrue_tmp] ==
        Pressed);
      FSM0_DW.condWasTrueAtLastTimeStep_2 = (FSM0_U.buttons[condIsTrue_tmp] ==
        Released);
      FSM0_DW.condWasTrueAtLastTimeStep_3 = (FSM0_U.buttons[ledsSTATES_tmp] ==
        Pressed);
      FSM0_DW.condWasTrueAtLastTimeStep_4 = (FSM0_U.buttons[ledsSTATES_tmp_0] ==
        Pressed);
      FSM0_DW.condWasTrueAtLastTimeStep_5 = (FSM0_U.buttons[ledsSTATES_tmp_1] ==
        Pressed);

      // Inport: '<Root>/MultiSelector_Go'
      FSM0_DW.condWasTrueAtLastTimeStep_6 = (FSM0_U.MultiSelector_Go == Pressed);
    } else {
      ledsSTATES_tmp = static_cast<int32_T>(VCV);

      // Inport: '<Root>/buttons'
      tmp = FSM0_unavailable_button(static_cast<real_T>(static_cast<int32_T>
        (FSM0_U.buttons[ledsSTATES_tmp])));
      if (tmp < 2.147483648E+9) {
        if (tmp >= -2.147483648E+9) {
          // Outport: '<Root>/ledsSTATES'
          FSM0_Y.ledsSTATES[ledsSTATES_tmp] = static_cast<StateLed>
            (FSM0_safe_cast_to_StateLed(static_cast<int32_T>(tmp)));
        } else {
          // Outport: '<Root>/ledsSTATES'
          FSM0_Y.ledsSTATES[ledsSTATES_tmp] = static_cast<StateLed>
            (FSM0_safe_cast_to_StateLed(MIN_int32_T));
        }
      } else {
        // Outport: '<Root>/ledsSTATES'
        FSM0_Y.ledsSTATES[ledsSTATES_tmp] = static_cast<StateLed>
          (FSM0_safe_cast_to_StateLed(MAX_int32_T));
      }

      ledsSTATES_tmp = static_cast<int32_T>(PRVC);

      // Inport: '<Root>/buttons'
      tmp = FSM0_unavailable_button(static_cast<real_T>(static_cast<int32_T>
        (FSM0_U.buttons[ledsSTATES_tmp])));
      if (tmp < 2.147483648E+9) {
        if (tmp >= -2.147483648E+9) {
          // Outport: '<Root>/ledsSTATES'
          FSM0_Y.ledsSTATES[ledsSTATES_tmp] = static_cast<StateLed>
            (FSM0_safe_cast_to_StateLed(static_cast<int32_T>(tmp)));
        } else {
          // Outport: '<Root>/ledsSTATES'
          FSM0_Y.ledsSTATES[ledsSTATES_tmp] = static_cast<StateLed>
            (FSM0_safe_cast_to_StateLed(MIN_int32_T));
        }
      } else {
        // Outport: '<Root>/ledsSTATES'
        FSM0_Y.ledsSTATES[ledsSTATES_tmp] = static_cast<StateLed>
          (FSM0_safe_cast_to_StateLed(MAX_int32_T));
      }
    }
    break;
  }
}

real32_T rt_roundf_snf(real32_T u)
{
  real32_T y;
  if (std::abs(u) < 8.388608E+6F) {
    if (u >= 0.5F) {
      y = std::floor(u + 0.5F);
    } else if (u > -0.5F) {
      y = u * 0.0F;
    } else {
      y = std::ceil(u - 0.5F);
    }
  } else {
    y = u;
  }

  return y;
}

// Function for Chart: '<S1>/FSM'
void FSM0ModelClass::FSM0_apply_map(real_T n)
{
  real32_T q;
  int32_T q_tmp;
  real_T j_tmp;
  j_tmp = (n - 1.0) * 4.0;
  q_tmp = (static_cast<int32_T>(j_tmp + 1.0) - 1) * 3;
  q = 1.0F / FSM0_B.MatrixConcatenate[q_tmp + 2];

  // Outport: '<Root>/knobs_out'
  FSM0_Y.knobs_out[0] = (FSM0_B.MatrixConcatenate[q_tmp + 1] -
    FSM0_B.MatrixConcatenate[q_tmp]) * (rt_roundf_snf(FSM0_B.RateTransition[0] /
    q) * q) + FSM0_B.MatrixConcatenate[q_tmp];
  q_tmp = (static_cast<int32_T>(j_tmp + 2.0) - 1) * 3;
  q = 1.0F / FSM0_B.MatrixConcatenate[q_tmp + 2];

  // Outport: '<Root>/knobs_out'
  FSM0_Y.knobs_out[1] = (FSM0_B.MatrixConcatenate[q_tmp + 1] -
    FSM0_B.MatrixConcatenate[q_tmp]) * (rt_roundf_snf(FSM0_B.RateTransition[1] /
    q) * q) + FSM0_B.MatrixConcatenate[q_tmp];
  q_tmp = (static_cast<int32_T>(j_tmp + 3.0) - 1) * 3;
  q = 1.0F / FSM0_B.MatrixConcatenate[q_tmp + 2];

  // Outport: '<Root>/knobs_out'
  FSM0_Y.knobs_out[2] = (FSM0_B.MatrixConcatenate[q_tmp + 1] -
    FSM0_B.MatrixConcatenate[q_tmp]) * (rt_roundf_snf(FSM0_B.RateTransition[2] /
    q) * q) + FSM0_B.MatrixConcatenate[q_tmp];
  q_tmp = (static_cast<int32_T>(j_tmp + 4.0) - 1) * 3;
  q = 1.0F / FSM0_B.MatrixConcatenate[q_tmp + 2];

  // Outport: '<Root>/knobs_out'
  FSM0_Y.knobs_out[3] = (FSM0_B.MatrixConcatenate[q_tmp + 1] -
    FSM0_B.MatrixConcatenate[q_tmp]) * (rt_roundf_snf(FSM0_B.RateTransition[3] /
    q) * q) + FSM0_B.MatrixConcatenate[q_tmp];
}

int32_T FSM0ModelClass::FSM0_safe_cast_to_MapIndex(int32_T input)
{
  int32_T y;

  // Initialize output value to default value for MapIndex (MAP_Transfer_None)
  y = 0;
  if ((input >= 0) && (input <= 8)) {
    // Set output value to input value if it is a member of MapIndex
    y = input;
  }

  return y;
}

// Function for Chart: '<S1>/FSM'
void FSM0ModelClass::FSM0_tune_alarms(void)
{
  // Outport: '<Root>/knobs_transfer' incorporates:
  //   Outport: '<Root>/knobs_out'

  switch (FSM0_Y.knobs_transfer) {
   case MAP_ALARMS_1:
    FSM0_DW.alarms_S7_min = FSM0_Y.knobs_out[0];
    FSM0_DW.alarms_S7_max = FSM0_Y.knobs_out[1];
    FSM0_DW.alarms_S1S3_max = FSM0_Y.knobs_out[2];
    FSM0_DW.alarms_S1S3_reinstate = FSM0_Y.knobs_out[3];
    break;

   case MAP_ALARMS_2:
    FSM0_DW.alarms_PEEP = FSM0_Y.knobs_out[0];
    FSM0_DW.alarms_TD_min = FSM0_Y.knobs_out[1];
    break;
  }

  // End of Outport: '<Root>/knobs_transfer'
}

// Function for Chart: '<S1>/FSM'
void FSM0ModelClass::FSM0_latch_multiselector(void)
{
  // Inport: '<Root>/MultiSelector_Maps'
  if (FSM0_U.MultiSelector_Maps < 1U) {
    FSM0_DW.MultiSelector_Maps_latch = 1U;
  } else if (FSM0_U.MultiSelector_Maps > 8U) {
    FSM0_DW.MultiSelector_Maps_latch = 8U;
  } else {
    FSM0_DW.MultiSelector_Maps_latch = FSM0_U.MultiSelector_Maps;
  }

  // End of Inport: '<Root>/MultiSelector_Maps'
}

// Function for Chart: '<S1>/FSM'
void FSM0ModelClass::FSM0_IDLE_BLINKING(void)
{
  switch (FSM0_DW.is_IDLE_BLINKING) {
   case FSM0_IN_INIT:
    if (FSM0_DW.sfEvent == FSM0_event_blink_IDLE_on) {
      FSM0_DW.is_IDLE_BLINKING = FSM0_IN_ON;
      FSM0_DW.temporalCounter_i2 = 0U;

      // Outport: '<Root>/ledsSTATES'
      FSM0_Y.ledsSTATES[static_cast<int32_T>(IDLE)] = On;
    }
    break;

   case FSM0_IN_OFF_c:
    if (FSM0_DW.temporalCounter_i2 >= FSM0_P.FSM_blinking_period * 100.0F) {
      FSM0_DW.is_IDLE_BLINKING = FSM0_IN_ON;
      FSM0_DW.temporalCounter_i2 = 0U;

      // Outport: '<Root>/ledsSTATES'
      FSM0_Y.ledsSTATES[static_cast<int32_T>(IDLE)] = On;
    } else {
      if (FSM0_DW.sfEvent == FSM0_event_blink_IDLE_off) {
        FSM0_DW.is_IDLE_BLINKING = FSM0_IN_INIT;

        // Outport: '<Root>/ledsSTATES'
        FSM0_Y.ledsSTATES[static_cast<int32_T>(IDLE)] = Off;
      }
    }
    break;

   case FSM0_IN_ON:
    if (FSM0_DW.temporalCounter_i2 >= FSM0_P.FSM_blinking_period * 100.0F) {
      FSM0_DW.is_IDLE_BLINKING = FSM0_IN_OFF_c;
      FSM0_DW.temporalCounter_i2 = 0U;

      // Outport: '<Root>/ledsSTATES'
      FSM0_Y.ledsSTATES[static_cast<int32_T>(IDLE)] = Off;
    } else {
      if (FSM0_DW.sfEvent == FSM0_event_blink_IDLE_off) {
        FSM0_DW.is_IDLE_BLINKING = FSM0_IN_INIT;

        // Outport: '<Root>/ledsSTATES'
        FSM0_Y.ledsSTATES[static_cast<int32_T>(IDLE)] = Off;
      }
    }
    break;
  }
}

// Function for Chart: '<S1>/FSM'
void FSM0ModelClass::FSM0_IDLE_j(void)
{
  boolean_T condIsTrue;
  int32_T b_previousEvent;
  int32_T condIsTrue_tmp;
  int32_T condIsTrue_tmp_0;
  int32_T condIsTrue_tmp_1;
  int32_T condIsTrue_tmp_2;
  boolean_T guard1 = false;
  if (FSM0_DW.sfEvent == FSM0_event_switch_off) {
    // Outport: '<Root>/knobs_transfer'
    FSM0_Y.knobs_transfer = MAP_Transfer_None;
    FSM0_DW.is_STATES_HANDLING = FSM0_IN_OFF;

    // Outport: '<Root>/ledsSTATES'
    FSM0_Y.ledsSTATES[static_cast<int32_T>(IDLE)] = Off;
    FSM0_Y.ledsSTATES[static_cast<int32_T>(CPAP)] = Off;
    FSM0_Y.ledsSTATES[static_cast<int32_T>(VCV)] = Off;
    FSM0_Y.ledsSTATES[static_cast<int32_T>(PRVC)] = Off;

    // Outport: '<Root>/Controller_Mode'
    FSM0_Y.Controller_Mode = IDLE;
  } else {
    condIsTrue_tmp = static_cast<int32_T>(IDLE);

    // Inport: '<Root>/buttons'
    condIsTrue = (FSM0_U.buttons[condIsTrue_tmp] == Released);
    if ((!condIsTrue) || (!FSM0_DW.condWasTrueAtLastTimeStep_2)) {
      FSM0_DW.durationLastReferenceTick_2 = FSM0_DW.chartAbsoluteTimeCounter;
    }

    FSM0_DW.condWasTrueAtLastTimeStep_2 = condIsTrue;
    guard1 = false;
    if ((!FSM0_DW.enter_test) && (FSM0_DW.chartAbsoluteTimeCounter -
         FSM0_DW.durationLastReferenceTick_2 >= FSM0_P.FSM_debounce_tmo * 100.0F))
    {
      FSM0_DW.enter_test = true;
      guard1 = true;
    } else {
      condIsTrue_tmp_0 = static_cast<int32_T>(CPAP);

      // Inport: '<Root>/buttons'
      condIsTrue = (FSM0_U.buttons[condIsTrue_tmp_0] == Pressed);
      if ((!condIsTrue) || (!FSM0_DW.condWasTrueAtLastTimeStep_3)) {
        FSM0_DW.durationLastReferenceTick_3 = FSM0_DW.chartAbsoluteTimeCounter;
      }

      FSM0_DW.condWasTrueAtLastTimeStep_3 = condIsTrue;
      if ((FSM0_DW.chartAbsoluteTimeCounter -
           FSM0_DW.durationLastReferenceTick_3 >= FSM0_P.FSM_debounce_tmo *
           100.0F) || ((FSM0_DW.resume_mode == CPAP) && (FSM0_Y.idALARM ==
            ALARM_None))) {
        // Outport: '<Root>/knobs_transfer'
        FSM0_Y.knobs_transfer = MAP_Transfer_None;
        FSM0_DW.durationLastReferenceTick_1_l = FSM0_DW.chartAbsoluteTimeCounter;
        FSM0_DW.is_STATES_HANDLING = FSM0_IN_CPAP;

        // Outport: '<Root>/ledsSTATES'
        FSM0_Y.ledsSTATES[condIsTrue_tmp] = Off;
        FSM0_Y.ledsSTATES[condIsTrue_tmp_0] = On;

        // Outport: '<Root>/Controller_Mode'
        FSM0_Y.Controller_Mode = CPAP;
        FSM0_DW.resume_mode = IDLE;

        // Inport: '<Root>/buttons'
        FSM0_DW.condWasTrueAtLastTimeStep_1_h = (FSM0_U.buttons[condIsTrue_tmp] ==
          Pressed);
      } else {
        condIsTrue_tmp_1 = static_cast<int32_T>(VCV);

        // Inport: '<Root>/buttons'
        condIsTrue = (FSM0_U.buttons[condIsTrue_tmp_1] == Pressed);
        if ((!condIsTrue) || (!FSM0_DW.condWasTrueAtLastTimeStep_4)) {
          FSM0_DW.durationLastReferenceTick_4 = FSM0_DW.chartAbsoluteTimeCounter;
        }

        FSM0_DW.condWasTrueAtLastTimeStep_4 = condIsTrue;
        if ((FSM0_DW.chartAbsoluteTimeCounter -
             FSM0_DW.durationLastReferenceTick_4 >= FSM0_P.FSM_debounce_tmo *
             100.0F) || ((FSM0_DW.resume_mode == VCV) && (FSM0_Y.idALARM ==
              ALARM_None))) {
          // Outport: '<Root>/knobs_transfer'
          FSM0_Y.knobs_transfer = MAP_Transfer_None;
          FSM0_DW.durationLastReferenceTick_1_m =
            FSM0_DW.chartAbsoluteTimeCounter;
          FSM0_DW.is_STATES_HANDLING = FSM0_IN_VCV;

          // Outport: '<Root>/ledsSTATES'
          FSM0_Y.ledsSTATES[condIsTrue_tmp] = Off;
          FSM0_Y.ledsSTATES[condIsTrue_tmp_1] = On;

          // Outport: '<Root>/Controller_Mode'
          FSM0_Y.Controller_Mode = VCV;
          FSM0_DW.resume_mode = IDLE;

          // Inport: '<Root>/buttons'
          FSM0_DW.condWasTrueAtLastTimeStep_1_a = (FSM0_U.buttons[condIsTrue_tmp]
            == Pressed);
        } else {
          condIsTrue_tmp_2 = static_cast<int32_T>(PRVC);

          // Inport: '<Root>/buttons'
          condIsTrue = (FSM0_U.buttons[condIsTrue_tmp_2] == Pressed);
          if ((!condIsTrue) || (!FSM0_DW.condWasTrueAtLastTimeStep_5)) {
            FSM0_DW.durationLastReferenceTick_5 =
              FSM0_DW.chartAbsoluteTimeCounter;
          }

          FSM0_DW.condWasTrueAtLastTimeStep_5 = condIsTrue;
          if ((FSM0_DW.chartAbsoluteTimeCounter -
               FSM0_DW.durationLastReferenceTick_5 >= FSM0_P.FSM_debounce_tmo *
               100.0F) || ((FSM0_DW.resume_mode == PRVC) && (FSM0_Y.idALARM ==
                ALARM_None))) {
            // Outport: '<Root>/knobs_transfer'
            FSM0_Y.knobs_transfer = MAP_Transfer_None;
            FSM0_DW.durationLastReferenceTick_1_g =
              FSM0_DW.chartAbsoluteTimeCounter;
            FSM0_DW.is_STATES_HANDLING = FSM0_IN_PRVC;

            // Outport: '<Root>/ledsSTATES'
            FSM0_Y.ledsSTATES[condIsTrue_tmp] = Off;
            FSM0_Y.ledsSTATES[condIsTrue_tmp_2] = On;

            // Outport: '<Root>/Controller_Mode'
            FSM0_Y.Controller_Mode = PRVC;
            FSM0_DW.resume_mode = IDLE;

            // Inport: '<Root>/buttons'
            FSM0_DW.condWasTrueAtLastTimeStep_1_gp =
              (FSM0_U.buttons[condIsTrue_tmp] == Pressed);
          } else {
            // Inport: '<Root>/buttons'
            condIsTrue = (FSM0_U.buttons[condIsTrue_tmp] == Pressed);
            if ((!condIsTrue) || (!FSM0_DW.condWasTrueAtLastTimeStep_1_g)) {
              FSM0_DW.durationLastReferenceTick_1_o =
                FSM0_DW.chartAbsoluteTimeCounter;
            }

            FSM0_DW.condWasTrueAtLastTimeStep_1_g = condIsTrue;
            if (FSM0_DW.enter_test && (FSM0_DW.chartAbsoluteTimeCounter -
                 FSM0_DW.durationLastReferenceTick_1_o >= FSM0_P.FSM_test_tmo *
                 100.0F)) {
              FSM0_DW.enter_test = false;

              // Outport: '<Root>/knobs_transfer'
              FSM0_Y.knobs_transfer = MAP_Transfer_None;
              b_previousEvent = FSM0_DW.sfEvent;
              FSM0_DW.sfEvent = FSM0_event_blink_IDLE_on;
              if (FSM0_DW.is_active_IDLE_BLINKING != 0U) {
                FSM0_IDLE_BLINKING();
              }

              FSM0_DW.sfEvent = b_previousEvent;
              FSM0_DW.durationLastReferenceTick_5_d =
                FSM0_DW.chartAbsoluteTimeCounter;
              FSM0_DW.durationLastReferenceTick_4_o =
                FSM0_DW.chartAbsoluteTimeCounter;
              FSM0_DW.durationLastReferenceTick_3_j =
                FSM0_DW.chartAbsoluteTimeCounter;
              FSM0_DW.durationLastReferenceTick_2_h =
                FSM0_DW.chartAbsoluteTimeCounter;
              FSM0_DW.durationLastReferenceTick_1 =
                FSM0_DW.chartAbsoluteTimeCounter;
              FSM0_DW.is_STATES_HANDLING = FSM0_IN_IDLE_n;

              // Outport: '<Root>/ledsSTATES'
              FSM0_Y.ledsSTATES[condIsTrue_tmp_0] = Off;
              FSM0_Y.ledsSTATES[condIsTrue_tmp_1] = Off;
              FSM0_Y.ledsSTATES[condIsTrue_tmp_2] = Off;

              // Outport: '<Root>/test_IV_dc'
              FSM0_Y.test_IV_dc = 0.0F;

              // Outport: '<Root>/test_OV_cmd'
              FSM0_Y.test_OV_cmd = false;

              // Inport: '<Root>/buttons'
              FSM0_DW.condWasTrueAtLastTimeStep_1 =
                (FSM0_U.buttons[condIsTrue_tmp] == Pressed);

              // Inport: '<Root>/MultiSelector_Go'
              FSM0_DW.condWasTrueAtLastTimeStep_2_k = (FSM0_U.MultiSelector_Go ==
                Pressed);

              // Inport: '<Root>/buttons'
              FSM0_DW.condWasTrueAtLastTimeStep_3_n =
                (FSM0_U.buttons[condIsTrue_tmp] == Released);
              FSM0_DW.condWasTrueAtLastTimeStep_4_k =
                (FSM0_U.buttons[condIsTrue_tmp_0] == Pressed);
              FSM0_DW.condWasTrueAtLastTimeStep_5_p =
                (FSM0_U.buttons[condIsTrue_tmp_1] == Pressed);
            } else {
              // Inport: '<Root>/MultiSelector_Go'
              condIsTrue = (FSM0_U.MultiSelector_Go == Pressed);
              if ((!condIsTrue) || (!FSM0_DW.condWasTrueAtLastTimeStep_6)) {
                FSM0_DW.durationLastReferenceTick_6 =
                  FSM0_DW.chartAbsoluteTimeCounter;
              }

              FSM0_DW.condWasTrueAtLastTimeStep_6 = condIsTrue;
              if (FSM0_DW.chartAbsoluteTimeCounter -
                  FSM0_DW.durationLastReferenceTick_6 >= FSM0_P.FSM_debounce_tmo
                  * 100.0F) {
                FSM0_latch_multiselector();
                guard1 = true;
              } else {
                FSM0_apply_map(static_cast<real_T>
                               (FSM0_DW.MultiSelector_Maps_latch));

                // Outport: '<Root>/knobs_transfer'
                FSM0_Y.knobs_transfer = static_cast<MapIndex>
                  (FSM0_safe_cast_to_MapIndex(static_cast<int32_T>
                    (FSM0_DW.MultiSelector_Maps_latch)));
                FSM0_tune_alarms();
              }
            }
          }
        }
      }
    }

    if (guard1) {
      // Outport: '<Root>/knobs_transfer'
      FSM0_Y.knobs_transfer = MAP_Transfer_None;
      FSM0_DW.durationLastReferenceTick_6 = FSM0_DW.chartAbsoluteTimeCounter;
      FSM0_DW.durationLastReferenceTick_5 = FSM0_DW.chartAbsoluteTimeCounter;
      FSM0_DW.durationLastReferenceTick_4 = FSM0_DW.chartAbsoluteTimeCounter;
      FSM0_DW.durationLastReferenceTick_3 = FSM0_DW.chartAbsoluteTimeCounter;
      FSM0_DW.durationLastReferenceTick_2 = FSM0_DW.chartAbsoluteTimeCounter;
      FSM0_DW.durationLastReferenceTick_1_o = FSM0_DW.chartAbsoluteTimeCounter;
      FSM0_DW.is_STATES_HANDLING = FSM0_IN_IDLE;

      // Outport: '<Root>/ledsSTATES'
      FSM0_Y.ledsSTATES[condIsTrue_tmp] = On;
      condIsTrue_tmp_0 = static_cast<int32_T>(CPAP);

      // Outport: '<Root>/ledsSTATES'
      FSM0_Y.ledsSTATES[condIsTrue_tmp_0] = Off;
      condIsTrue_tmp_1 = static_cast<int32_T>(VCV);

      // Outport: '<Root>/ledsSTATES'
      FSM0_Y.ledsSTATES[condIsTrue_tmp_1] = Off;
      condIsTrue_tmp_2 = static_cast<int32_T>(PRVC);

      // Outport: '<Root>/ledsSTATES'
      FSM0_Y.ledsSTATES[condIsTrue_tmp_2] = Off;

      // Outport: '<Root>/Controller_Mode'
      FSM0_Y.Controller_Mode = IDLE;

      // Inport: '<Root>/buttons'
      FSM0_DW.condWasTrueAtLastTimeStep_1_g = (FSM0_U.buttons[condIsTrue_tmp] ==
        Pressed);
      FSM0_DW.condWasTrueAtLastTimeStep_2 = (FSM0_U.buttons[condIsTrue_tmp] ==
        Released);
      FSM0_DW.condWasTrueAtLastTimeStep_3 = (FSM0_U.buttons[condIsTrue_tmp_0] ==
        Pressed);
      FSM0_DW.condWasTrueAtLastTimeStep_4 = (FSM0_U.buttons[condIsTrue_tmp_1] ==
        Pressed);
      FSM0_DW.condWasTrueAtLastTimeStep_5 = (FSM0_U.buttons[condIsTrue_tmp_2] ==
        Pressed);

      // Inport: '<Root>/MultiSelector_Go'
      FSM0_DW.condWasTrueAtLastTimeStep_6 = (FSM0_U.MultiSelector_Go == Pressed);
    }
  }
}

// Function for Chart: '<S1>/FSM'
void FSM0ModelClass::FSM0_PRVC(void)
{
  boolean_T condIsTrue;
  real_T tmp;
  int32_T condIsTrue_tmp;
  int32_T ledsSTATES_tmp;
  int32_T ledsSTATES_tmp_0;
  int32_T ledsSTATES_tmp_1;
  switch (FSM0_DW.sfEvent) {
   case FSM0_event_switch_off:
    FSM0_DW.is_STATES_HANDLING = FSM0_IN_OFF;

    // Outport: '<Root>/ledsSTATES'
    FSM0_Y.ledsSTATES[static_cast<int32_T>(IDLE)] = Off;
    FSM0_Y.ledsSTATES[static_cast<int32_T>(CPAP)] = Off;
    FSM0_Y.ledsSTATES[static_cast<int32_T>(VCV)] = Off;
    FSM0_Y.ledsSTATES[static_cast<int32_T>(PRVC)] = Off;

    // Outport: '<Root>/Controller_Mode'
    FSM0_Y.Controller_Mode = IDLE;
    break;

   case FSM0_event_idle_controller:
    FSM0_DW.enter_test = false;
    FSM0_DW.durationLastReferenceTick_6 = FSM0_DW.chartAbsoluteTimeCounter;
    FSM0_DW.durationLastReferenceTick_5 = FSM0_DW.chartAbsoluteTimeCounter;
    FSM0_DW.durationLastReferenceTick_4 = FSM0_DW.chartAbsoluteTimeCounter;
    FSM0_DW.durationLastReferenceTick_3 = FSM0_DW.chartAbsoluteTimeCounter;
    FSM0_DW.durationLastReferenceTick_2 = FSM0_DW.chartAbsoluteTimeCounter;
    FSM0_DW.durationLastReferenceTick_1_o = FSM0_DW.chartAbsoluteTimeCounter;
    FSM0_DW.is_STATES_HANDLING = FSM0_IN_IDLE;
    ledsSTATES_tmp = static_cast<int32_T>(IDLE);

    // Outport: '<Root>/ledsSTATES'
    FSM0_Y.ledsSTATES[ledsSTATES_tmp] = On;
    ledsSTATES_tmp_0 = static_cast<int32_T>(CPAP);

    // Outport: '<Root>/ledsSTATES'
    FSM0_Y.ledsSTATES[ledsSTATES_tmp_0] = Off;
    ledsSTATES_tmp_1 = static_cast<int32_T>(VCV);

    // Outport: '<Root>/ledsSTATES'
    FSM0_Y.ledsSTATES[ledsSTATES_tmp_1] = Off;
    condIsTrue_tmp = static_cast<int32_T>(PRVC);

    // Outport: '<Root>/ledsSTATES'
    FSM0_Y.ledsSTATES[condIsTrue_tmp] = Off;

    // Outport: '<Root>/Controller_Mode'
    FSM0_Y.Controller_Mode = IDLE;

    // Inport: '<Root>/buttons'
    FSM0_DW.condWasTrueAtLastTimeStep_1_g = (FSM0_U.buttons[ledsSTATES_tmp] ==
      Pressed);
    FSM0_DW.condWasTrueAtLastTimeStep_2 = (FSM0_U.buttons[ledsSTATES_tmp] ==
      Released);
    FSM0_DW.condWasTrueAtLastTimeStep_3 = (FSM0_U.buttons[ledsSTATES_tmp_0] ==
      Pressed);
    FSM0_DW.condWasTrueAtLastTimeStep_4 = (FSM0_U.buttons[ledsSTATES_tmp_1] ==
      Pressed);
    FSM0_DW.condWasTrueAtLastTimeStep_5 = (FSM0_U.buttons[condIsTrue_tmp] ==
      Pressed);

    // Inport: '<Root>/MultiSelector_Go'
    FSM0_DW.condWasTrueAtLastTimeStep_6 = (FSM0_U.MultiSelector_Go == Pressed);
    break;

   default:
    condIsTrue_tmp = static_cast<int32_T>(IDLE);

    // Inport: '<Root>/buttons'
    condIsTrue = (FSM0_U.buttons[condIsTrue_tmp] == Pressed);
    if ((!condIsTrue) || (!FSM0_DW.condWasTrueAtLastTimeStep_1_gp)) {
      FSM0_DW.durationLastReferenceTick_1_g = FSM0_DW.chartAbsoluteTimeCounter;
    }

    FSM0_DW.condWasTrueAtLastTimeStep_1_gp = condIsTrue;
    if (FSM0_DW.chartAbsoluteTimeCounter - FSM0_DW.durationLastReferenceTick_1_g
        >= FSM0_P.FSM_debounce_tmo * 100.0F) {
      FSM0_DW.enter_test = false;
      FSM0_DW.durationLastReferenceTick_6 = FSM0_DW.chartAbsoluteTimeCounter;
      FSM0_DW.durationLastReferenceTick_5 = FSM0_DW.chartAbsoluteTimeCounter;
      FSM0_DW.durationLastReferenceTick_4 = FSM0_DW.chartAbsoluteTimeCounter;
      FSM0_DW.durationLastReferenceTick_3 = FSM0_DW.chartAbsoluteTimeCounter;
      FSM0_DW.durationLastReferenceTick_2 = FSM0_DW.chartAbsoluteTimeCounter;
      FSM0_DW.durationLastReferenceTick_1_o = FSM0_DW.chartAbsoluteTimeCounter;
      FSM0_DW.is_STATES_HANDLING = FSM0_IN_IDLE;

      // Outport: '<Root>/ledsSTATES'
      FSM0_Y.ledsSTATES[condIsTrue_tmp] = On;
      ledsSTATES_tmp = static_cast<int32_T>(CPAP);

      // Outport: '<Root>/ledsSTATES'
      FSM0_Y.ledsSTATES[ledsSTATES_tmp] = Off;
      ledsSTATES_tmp_0 = static_cast<int32_T>(VCV);

      // Outport: '<Root>/ledsSTATES'
      FSM0_Y.ledsSTATES[ledsSTATES_tmp_0] = Off;
      ledsSTATES_tmp_1 = static_cast<int32_T>(PRVC);

      // Outport: '<Root>/ledsSTATES'
      FSM0_Y.ledsSTATES[ledsSTATES_tmp_1] = Off;

      // Outport: '<Root>/Controller_Mode'
      FSM0_Y.Controller_Mode = IDLE;

      // Inport: '<Root>/buttons'
      FSM0_DW.condWasTrueAtLastTimeStep_1_g = (FSM0_U.buttons[condIsTrue_tmp] ==
        Pressed);
      FSM0_DW.condWasTrueAtLastTimeStep_2 = (FSM0_U.buttons[condIsTrue_tmp] ==
        Released);
      FSM0_DW.condWasTrueAtLastTimeStep_3 = (FSM0_U.buttons[ledsSTATES_tmp] ==
        Pressed);
      FSM0_DW.condWasTrueAtLastTimeStep_4 = (FSM0_U.buttons[ledsSTATES_tmp_0] ==
        Pressed);
      FSM0_DW.condWasTrueAtLastTimeStep_5 = (FSM0_U.buttons[ledsSTATES_tmp_1] ==
        Pressed);

      // Inport: '<Root>/MultiSelector_Go'
      FSM0_DW.condWasTrueAtLastTimeStep_6 = (FSM0_U.MultiSelector_Go == Pressed);
    } else {
      ledsSTATES_tmp = static_cast<int32_T>(CPAP);

      // Inport: '<Root>/buttons'
      tmp = FSM0_unavailable_button(static_cast<real_T>(static_cast<int32_T>
        (FSM0_U.buttons[ledsSTATES_tmp])));
      if (tmp < 2.147483648E+9) {
        if (tmp >= -2.147483648E+9) {
          // Outport: '<Root>/ledsSTATES'
          FSM0_Y.ledsSTATES[ledsSTATES_tmp] = static_cast<StateLed>
            (FSM0_safe_cast_to_StateLed(static_cast<int32_T>(tmp)));
        } else {
          // Outport: '<Root>/ledsSTATES'
          FSM0_Y.ledsSTATES[ledsSTATES_tmp] = static_cast<StateLed>
            (FSM0_safe_cast_to_StateLed(MIN_int32_T));
        }
      } else {
        // Outport: '<Root>/ledsSTATES'
        FSM0_Y.ledsSTATES[ledsSTATES_tmp] = static_cast<StateLed>
          (FSM0_safe_cast_to_StateLed(MAX_int32_T));
      }

      ledsSTATES_tmp = static_cast<int32_T>(VCV);

      // Inport: '<Root>/buttons'
      tmp = FSM0_unavailable_button(static_cast<real_T>(static_cast<int32_T>
        (FSM0_U.buttons[ledsSTATES_tmp])));
      if (tmp < 2.147483648E+9) {
        if (tmp >= -2.147483648E+9) {
          // Outport: '<Root>/ledsSTATES'
          FSM0_Y.ledsSTATES[ledsSTATES_tmp] = static_cast<StateLed>
            (FSM0_safe_cast_to_StateLed(static_cast<int32_T>(tmp)));
        } else {
          // Outport: '<Root>/ledsSTATES'
          FSM0_Y.ledsSTATES[ledsSTATES_tmp] = static_cast<StateLed>
            (FSM0_safe_cast_to_StateLed(MIN_int32_T));
        }
      } else {
        // Outport: '<Root>/ledsSTATES'
        FSM0_Y.ledsSTATES[ledsSTATES_tmp] = static_cast<StateLed>
          (FSM0_safe_cast_to_StateLed(MAX_int32_T));
      }
    }
    break;
  }
}

// Function for Chart: '<S1>/FSM'
void FSM0ModelClass::FSM0_IDLE(void)
{
  boolean_T condIsTrue;
  int32_T c_previousEvent;
  real_T tmp;
  int32_T condIsTrue_tmp;
  int32_T condIsTrue_tmp_0;
  int32_T condIsTrue_tmp_1;
  boolean_T guard1 = false;
  condIsTrue_tmp = static_cast<int32_T>(CPAP);

  // Inport: '<Root>/buttons'
  condIsTrue = (FSM0_U.buttons[condIsTrue_tmp] == Pressed);
  if ((!condIsTrue) || (!FSM0_DW.condWasTrueAtLastTimeStep_4_k)) {
    FSM0_DW.durationLastReferenceTick_4_o = FSM0_DW.chartAbsoluteTimeCounter;
  }

  FSM0_DW.condWasTrueAtLastTimeStep_4_k = condIsTrue;
  if (FSM0_DW.chartAbsoluteTimeCounter - FSM0_DW.durationLastReferenceTick_4_o >=
      FSM0_P.FSM_debounce_tmo * 100.0F) {
    // Outport: '<Root>/knobs_transfer'
    FSM0_Y.knobs_transfer = MAP_Transfer_None;
    FSM0_DW.durationLastReferenceTick_1_j = FSM0_DW.chartAbsoluteTimeCounter;
    FSM0_DW.is_STATES_HANDLING = FSM0_IN_TEST_1;
    FSM0_DW.temporalCounter_i1 = 0U;

    // Outport: '<Root>/test_IV_dc'
    FSM0_Y.test_IV_dc = 0.0F;

    // Outport: '<Root>/test_OV_cmd'
    FSM0_Y.test_OV_cmd = true;

    // Outport: '<Root>/ledsSTATES'
    FSM0_Y.ledsSTATES[condIsTrue_tmp] = On;

    // Inport: '<Root>/buttons'
    FSM0_DW.condWasTrueAtLastTimeStep_1_d = (FSM0_U.buttons[static_cast<int32_T>
      (IDLE)] == Pressed);
  } else {
    condIsTrue_tmp_0 = static_cast<int32_T>(IDLE);

    // Inport: '<Root>/buttons'
    condIsTrue = (FSM0_U.buttons[condIsTrue_tmp_0] == Pressed);
    if ((!condIsTrue) || (!FSM0_DW.condWasTrueAtLastTimeStep_1)) {
      FSM0_DW.durationLastReferenceTick_1 = FSM0_DW.chartAbsoluteTimeCounter;
    }

    FSM0_DW.condWasTrueAtLastTimeStep_1 = condIsTrue;
    if (FSM0_DW.exit_test && (FSM0_DW.chartAbsoluteTimeCounter -
         FSM0_DW.durationLastReferenceTick_1 >= FSM0_P.FSM_test_tmo * 100.0F)) {
      FSM0_DW.exit_test = false;

      // Outport: '<Root>/knobs_transfer'
      FSM0_Y.knobs_transfer = MAP_Transfer_None;
      c_previousEvent = FSM0_DW.sfEvent;
      FSM0_DW.sfEvent = FSM0_event_blink_IDLE_off;
      if (FSM0_DW.is_active_IDLE_BLINKING != 0U) {
        FSM0_IDLE_BLINKING();
      }

      FSM0_DW.sfEvent = c_previousEvent;
      FSM0_DW.durationLastReferenceTick_6 = FSM0_DW.chartAbsoluteTimeCounter;
      FSM0_DW.durationLastReferenceTick_5 = FSM0_DW.chartAbsoluteTimeCounter;
      FSM0_DW.durationLastReferenceTick_4 = FSM0_DW.chartAbsoluteTimeCounter;
      FSM0_DW.durationLastReferenceTick_3 = FSM0_DW.chartAbsoluteTimeCounter;
      FSM0_DW.durationLastReferenceTick_2 = FSM0_DW.chartAbsoluteTimeCounter;
      FSM0_DW.durationLastReferenceTick_1_o = FSM0_DW.chartAbsoluteTimeCounter;
      FSM0_DW.is_STATES_HANDLING = FSM0_IN_IDLE;

      // Outport: '<Root>/ledsSTATES'
      FSM0_Y.ledsSTATES[condIsTrue_tmp_0] = On;
      FSM0_Y.ledsSTATES[condIsTrue_tmp] = Off;
      condIsTrue_tmp_1 = static_cast<int32_T>(VCV);

      // Outport: '<Root>/ledsSTATES'
      FSM0_Y.ledsSTATES[condIsTrue_tmp_1] = Off;
      c_previousEvent = static_cast<int32_T>(PRVC);

      // Outport: '<Root>/ledsSTATES'
      FSM0_Y.ledsSTATES[c_previousEvent] = Off;

      // Outport: '<Root>/Controller_Mode'
      FSM0_Y.Controller_Mode = IDLE;

      // Inport: '<Root>/buttons'
      FSM0_DW.condWasTrueAtLastTimeStep_1_g = (FSM0_U.buttons[condIsTrue_tmp_0] ==
        Pressed);
      FSM0_DW.condWasTrueAtLastTimeStep_2 = (FSM0_U.buttons[condIsTrue_tmp_0] ==
        Released);
      FSM0_DW.condWasTrueAtLastTimeStep_3 = (FSM0_U.buttons[condIsTrue_tmp] ==
        Pressed);
      FSM0_DW.condWasTrueAtLastTimeStep_4 = (FSM0_U.buttons[condIsTrue_tmp_1] ==
        Pressed);
      FSM0_DW.condWasTrueAtLastTimeStep_5 = (FSM0_U.buttons[c_previousEvent] ==
        Pressed);

      // Inport: '<Root>/MultiSelector_Go'
      FSM0_DW.condWasTrueAtLastTimeStep_6 = (FSM0_U.MultiSelector_Go == Pressed);
    } else {
      condIsTrue_tmp_1 = static_cast<int32_T>(VCV);

      // Inport: '<Root>/buttons'
      condIsTrue = (FSM0_U.buttons[condIsTrue_tmp_1] == Pressed);
      if ((!condIsTrue) || (!FSM0_DW.condWasTrueAtLastTimeStep_5_p)) {
        FSM0_DW.durationLastReferenceTick_5_d = FSM0_DW.chartAbsoluteTimeCounter;
      }

      FSM0_DW.condWasTrueAtLastTimeStep_5_p = condIsTrue;
      if (FSM0_DW.chartAbsoluteTimeCounter -
          FSM0_DW.durationLastReferenceTick_5_d >= FSM0_P.FSM_debounce_tmo *
          100.0F) {
        // Outport: '<Root>/knobs_transfer'
        FSM0_Y.knobs_transfer = MAP_Transfer_None;
        FSM0_DW.durationLastReferenceTick_1_lo =
          FSM0_DW.chartAbsoluteTimeCounter;
        FSM0_DW.is_STATES_HANDLING = FSM0_IN_TEST_2;
        FSM0_DW.temporalCounter_i1 = 0U;

        // Outport: '<Root>/test_IV_dc'
        FSM0_Y.test_IV_dc = 100.0F;

        // Outport: '<Root>/ledsSTATES'
        FSM0_Y.ledsSTATES[condIsTrue_tmp_1] = On;

        // Inport: '<Root>/buttons'
        FSM0_DW.condWasTrueAtLastTimeStep_1_c = (FSM0_U.buttons[condIsTrue_tmp_0]
          == Pressed);
      } else {
        // Inport: '<Root>/buttons'
        condIsTrue = (FSM0_U.buttons[condIsTrue_tmp_0] == Released);
        if ((!condIsTrue) || (!FSM0_DW.condWasTrueAtLastTimeStep_3_n)) {
          FSM0_DW.durationLastReferenceTick_3_j =
            FSM0_DW.chartAbsoluteTimeCounter;
        }

        FSM0_DW.condWasTrueAtLastTimeStep_3_n = condIsTrue;
        guard1 = false;
        if ((!FSM0_DW.exit_test) && (FSM0_DW.chartAbsoluteTimeCounter -
             FSM0_DW.durationLastReferenceTick_3_j >= FSM0_P.FSM_debounce_tmo *
             100.0F)) {
          FSM0_DW.exit_test = true;
          guard1 = true;
        } else {
          // Inport: '<Root>/MultiSelector_Go'
          condIsTrue = (FSM0_U.MultiSelector_Go == Pressed);
          if ((!condIsTrue) || (!FSM0_DW.condWasTrueAtLastTimeStep_2_k)) {
            FSM0_DW.durationLastReferenceTick_2_h =
              FSM0_DW.chartAbsoluteTimeCounter;
          }

          FSM0_DW.condWasTrueAtLastTimeStep_2_k = condIsTrue;
          if (FSM0_DW.chartAbsoluteTimeCounter -
              FSM0_DW.durationLastReferenceTick_2_h >= FSM0_P.FSM_debounce_tmo *
              100.0F) {
            FSM0_latch_multiselector();
            guard1 = true;
          } else if (FSM0_DW.sfEvent == FSM0_event_switch_off) {
            // Outport: '<Root>/knobs_transfer'
            FSM0_Y.knobs_transfer = MAP_Transfer_None;
            c_previousEvent = FSM0_DW.sfEvent;
            FSM0_DW.sfEvent = FSM0_event_blink_IDLE_off;
            if (FSM0_DW.is_active_IDLE_BLINKING != 0U) {
              FSM0_IDLE_BLINKING();
            }

            FSM0_DW.sfEvent = c_previousEvent;
            FSM0_DW.is_STATES_HANDLING = FSM0_IN_OFF;

            // Outport: '<Root>/ledsSTATES'
            FSM0_Y.ledsSTATES[condIsTrue_tmp_0] = Off;
            FSM0_Y.ledsSTATES[condIsTrue_tmp] = Off;
            FSM0_Y.ledsSTATES[condIsTrue_tmp_1] = Off;
            FSM0_Y.ledsSTATES[static_cast<int32_T>(PRVC)] = Off;

            // Outport: '<Root>/Controller_Mode'
            FSM0_Y.Controller_Mode = IDLE;
          } else {
            FSM0_apply_map(static_cast<real_T>(FSM0_DW.MultiSelector_Maps_latch));

            // Outport: '<Root>/knobs_transfer'
            FSM0_Y.knobs_transfer = static_cast<MapIndex>
              (FSM0_safe_cast_to_MapIndex(static_cast<int32_T>
                (FSM0_DW.MultiSelector_Maps_latch)));

            // Outport: '<Root>/knobs_out'
            FSM0_DW.test1_dc_peak = FSM0_Y.knobs_out[0];
            FSM0_DW.test1_period = FSM0_Y.knobs_out[1];
            FSM0_DW.test2_period = FSM0_Y.knobs_out[2];
            condIsTrue_tmp = static_cast<int32_T>(PRVC);

            // Inport: '<Root>/buttons'
            tmp = FSM0_unavailable_button(static_cast<real_T>
              (static_cast<int32_T>(FSM0_U.buttons[condIsTrue_tmp])));
            if (tmp < 2.147483648E+9) {
              if (tmp >= -2.147483648E+9) {
                // Outport: '<Root>/ledsSTATES'
                FSM0_Y.ledsSTATES[condIsTrue_tmp] = static_cast<StateLed>
                  (FSM0_safe_cast_to_StateLed(static_cast<int32_T>(tmp)));
              } else {
                // Outport: '<Root>/ledsSTATES'
                FSM0_Y.ledsSTATES[condIsTrue_tmp] = static_cast<StateLed>
                  (FSM0_safe_cast_to_StateLed(MIN_int32_T));
              }
            } else {
              // Outport: '<Root>/ledsSTATES'
              FSM0_Y.ledsSTATES[condIsTrue_tmp] = static_cast<StateLed>
                (FSM0_safe_cast_to_StateLed(MAX_int32_T));
            }
          }
        }

        if (guard1) {
          // Outport: '<Root>/knobs_transfer'
          FSM0_Y.knobs_transfer = MAP_Transfer_None;
          FSM0_DW.durationLastReferenceTick_5_d =
            FSM0_DW.chartAbsoluteTimeCounter;
          FSM0_DW.durationLastReferenceTick_4_o =
            FSM0_DW.chartAbsoluteTimeCounter;
          FSM0_DW.durationLastReferenceTick_3_j =
            FSM0_DW.chartAbsoluteTimeCounter;
          FSM0_DW.durationLastReferenceTick_2_h =
            FSM0_DW.chartAbsoluteTimeCounter;
          FSM0_DW.durationLastReferenceTick_1 = FSM0_DW.chartAbsoluteTimeCounter;
          FSM0_DW.is_STATES_HANDLING = FSM0_IN_IDLE_n;

          // Outport: '<Root>/ledsSTATES'
          FSM0_Y.ledsSTATES[condIsTrue_tmp] = Off;
          FSM0_Y.ledsSTATES[condIsTrue_tmp_1] = Off;
          FSM0_Y.ledsSTATES[static_cast<int32_T>(PRVC)] = Off;

          // Outport: '<Root>/test_IV_dc'
          FSM0_Y.test_IV_dc = 0.0F;

          // Outport: '<Root>/test_OV_cmd'
          FSM0_Y.test_OV_cmd = false;

          // Inport: '<Root>/buttons'
          FSM0_DW.condWasTrueAtLastTimeStep_1 = (FSM0_U.buttons[condIsTrue_tmp_0]
            == Pressed);

          // Inport: '<Root>/MultiSelector_Go'
          FSM0_DW.condWasTrueAtLastTimeStep_2_k = (FSM0_U.MultiSelector_Go ==
            Pressed);

          // Inport: '<Root>/buttons'
          FSM0_DW.condWasTrueAtLastTimeStep_3_n =
            (FSM0_U.buttons[condIsTrue_tmp_0] == Released);
          FSM0_DW.condWasTrueAtLastTimeStep_4_k = (FSM0_U.buttons[condIsTrue_tmp]
            == Pressed);
          FSM0_DW.condWasTrueAtLastTimeStep_5_p =
            (FSM0_U.buttons[condIsTrue_tmp_1] == Pressed);
        }
      }
    }
  }
}

// Function for Chart: '<S1>/FSM'
void FSM0ModelClass::FSM0_VCV(void)
{
  boolean_T condIsTrue;
  real_T tmp;
  int32_T condIsTrue_tmp;
  int32_T ledsSTATES_tmp;
  int32_T ledsSTATES_tmp_0;
  int32_T ledsSTATES_tmp_1;
  switch (FSM0_DW.sfEvent) {
   case FSM0_event_switch_off:
    FSM0_DW.is_STATES_HANDLING = FSM0_IN_OFF;

    // Outport: '<Root>/ledsSTATES'
    FSM0_Y.ledsSTATES[static_cast<int32_T>(IDLE)] = Off;
    FSM0_Y.ledsSTATES[static_cast<int32_T>(CPAP)] = Off;
    FSM0_Y.ledsSTATES[static_cast<int32_T>(VCV)] = Off;
    FSM0_Y.ledsSTATES[static_cast<int32_T>(PRVC)] = Off;

    // Outport: '<Root>/Controller_Mode'
    FSM0_Y.Controller_Mode = IDLE;
    break;

   case FSM0_event_idle_controller:
    FSM0_DW.enter_test = false;
    FSM0_DW.durationLastReferenceTick_6 = FSM0_DW.chartAbsoluteTimeCounter;
    FSM0_DW.durationLastReferenceTick_5 = FSM0_DW.chartAbsoluteTimeCounter;
    FSM0_DW.durationLastReferenceTick_4 = FSM0_DW.chartAbsoluteTimeCounter;
    FSM0_DW.durationLastReferenceTick_3 = FSM0_DW.chartAbsoluteTimeCounter;
    FSM0_DW.durationLastReferenceTick_2 = FSM0_DW.chartAbsoluteTimeCounter;
    FSM0_DW.durationLastReferenceTick_1_o = FSM0_DW.chartAbsoluteTimeCounter;
    FSM0_DW.is_STATES_HANDLING = FSM0_IN_IDLE;
    ledsSTATES_tmp = static_cast<int32_T>(IDLE);

    // Outport: '<Root>/ledsSTATES'
    FSM0_Y.ledsSTATES[ledsSTATES_tmp] = On;
    ledsSTATES_tmp_0 = static_cast<int32_T>(CPAP);

    // Outport: '<Root>/ledsSTATES'
    FSM0_Y.ledsSTATES[ledsSTATES_tmp_0] = Off;
    ledsSTATES_tmp_1 = static_cast<int32_T>(VCV);

    // Outport: '<Root>/ledsSTATES'
    FSM0_Y.ledsSTATES[ledsSTATES_tmp_1] = Off;
    condIsTrue_tmp = static_cast<int32_T>(PRVC);

    // Outport: '<Root>/ledsSTATES'
    FSM0_Y.ledsSTATES[condIsTrue_tmp] = Off;

    // Outport: '<Root>/Controller_Mode'
    FSM0_Y.Controller_Mode = IDLE;

    // Inport: '<Root>/buttons'
    FSM0_DW.condWasTrueAtLastTimeStep_1_g = (FSM0_U.buttons[ledsSTATES_tmp] ==
      Pressed);
    FSM0_DW.condWasTrueAtLastTimeStep_2 = (FSM0_U.buttons[ledsSTATES_tmp] ==
      Released);
    FSM0_DW.condWasTrueAtLastTimeStep_3 = (FSM0_U.buttons[ledsSTATES_tmp_0] ==
      Pressed);
    FSM0_DW.condWasTrueAtLastTimeStep_4 = (FSM0_U.buttons[ledsSTATES_tmp_1] ==
      Pressed);
    FSM0_DW.condWasTrueAtLastTimeStep_5 = (FSM0_U.buttons[condIsTrue_tmp] ==
      Pressed);

    // Inport: '<Root>/MultiSelector_Go'
    FSM0_DW.condWasTrueAtLastTimeStep_6 = (FSM0_U.MultiSelector_Go == Pressed);
    break;

   default:
    condIsTrue_tmp = static_cast<int32_T>(IDLE);

    // Inport: '<Root>/buttons'
    condIsTrue = (FSM0_U.buttons[condIsTrue_tmp] == Pressed);
    if ((!condIsTrue) || (!FSM0_DW.condWasTrueAtLastTimeStep_1_a)) {
      FSM0_DW.durationLastReferenceTick_1_m = FSM0_DW.chartAbsoluteTimeCounter;
    }

    FSM0_DW.condWasTrueAtLastTimeStep_1_a = condIsTrue;
    if (FSM0_DW.chartAbsoluteTimeCounter - FSM0_DW.durationLastReferenceTick_1_m
        >= FSM0_P.FSM_debounce_tmo * 100.0F) {
      FSM0_DW.enter_test = false;
      FSM0_DW.durationLastReferenceTick_6 = FSM0_DW.chartAbsoluteTimeCounter;
      FSM0_DW.durationLastReferenceTick_5 = FSM0_DW.chartAbsoluteTimeCounter;
      FSM0_DW.durationLastReferenceTick_4 = FSM0_DW.chartAbsoluteTimeCounter;
      FSM0_DW.durationLastReferenceTick_3 = FSM0_DW.chartAbsoluteTimeCounter;
      FSM0_DW.durationLastReferenceTick_2 = FSM0_DW.chartAbsoluteTimeCounter;
      FSM0_DW.durationLastReferenceTick_1_o = FSM0_DW.chartAbsoluteTimeCounter;
      FSM0_DW.is_STATES_HANDLING = FSM0_IN_IDLE;

      // Outport: '<Root>/ledsSTATES'
      FSM0_Y.ledsSTATES[condIsTrue_tmp] = On;
      ledsSTATES_tmp = static_cast<int32_T>(CPAP);

      // Outport: '<Root>/ledsSTATES'
      FSM0_Y.ledsSTATES[ledsSTATES_tmp] = Off;
      ledsSTATES_tmp_0 = static_cast<int32_T>(VCV);

      // Outport: '<Root>/ledsSTATES'
      FSM0_Y.ledsSTATES[ledsSTATES_tmp_0] = Off;
      ledsSTATES_tmp_1 = static_cast<int32_T>(PRVC);

      // Outport: '<Root>/ledsSTATES'
      FSM0_Y.ledsSTATES[ledsSTATES_tmp_1] = Off;

      // Outport: '<Root>/Controller_Mode'
      FSM0_Y.Controller_Mode = IDLE;

      // Inport: '<Root>/buttons'
      FSM0_DW.condWasTrueAtLastTimeStep_1_g = (FSM0_U.buttons[condIsTrue_tmp] ==
        Pressed);
      FSM0_DW.condWasTrueAtLastTimeStep_2 = (FSM0_U.buttons[condIsTrue_tmp] ==
        Released);
      FSM0_DW.condWasTrueAtLastTimeStep_3 = (FSM0_U.buttons[ledsSTATES_tmp] ==
        Pressed);
      FSM0_DW.condWasTrueAtLastTimeStep_4 = (FSM0_U.buttons[ledsSTATES_tmp_0] ==
        Pressed);
      FSM0_DW.condWasTrueAtLastTimeStep_5 = (FSM0_U.buttons[ledsSTATES_tmp_1] ==
        Pressed);

      // Inport: '<Root>/MultiSelector_Go'
      FSM0_DW.condWasTrueAtLastTimeStep_6 = (FSM0_U.MultiSelector_Go == Pressed);
    } else {
      ledsSTATES_tmp = static_cast<int32_T>(CPAP);

      // Inport: '<Root>/buttons'
      tmp = FSM0_unavailable_button(static_cast<real_T>(static_cast<int32_T>
        (FSM0_U.buttons[ledsSTATES_tmp])));
      if (tmp < 2.147483648E+9) {
        if (tmp >= -2.147483648E+9) {
          // Outport: '<Root>/ledsSTATES'
          FSM0_Y.ledsSTATES[ledsSTATES_tmp] = static_cast<StateLed>
            (FSM0_safe_cast_to_StateLed(static_cast<int32_T>(tmp)));
        } else {
          // Outport: '<Root>/ledsSTATES'
          FSM0_Y.ledsSTATES[ledsSTATES_tmp] = static_cast<StateLed>
            (FSM0_safe_cast_to_StateLed(MIN_int32_T));
        }
      } else {
        // Outport: '<Root>/ledsSTATES'
        FSM0_Y.ledsSTATES[ledsSTATES_tmp] = static_cast<StateLed>
          (FSM0_safe_cast_to_StateLed(MAX_int32_T));
      }

      ledsSTATES_tmp = static_cast<int32_T>(PRVC);

      // Inport: '<Root>/buttons'
      tmp = FSM0_unavailable_button(static_cast<real_T>(static_cast<int32_T>
        (FSM0_U.buttons[ledsSTATES_tmp])));
      if (tmp < 2.147483648E+9) {
        if (tmp >= -2.147483648E+9) {
          // Outport: '<Root>/ledsSTATES'
          FSM0_Y.ledsSTATES[ledsSTATES_tmp] = static_cast<StateLed>
            (FSM0_safe_cast_to_StateLed(static_cast<int32_T>(tmp)));
        } else {
          // Outport: '<Root>/ledsSTATES'
          FSM0_Y.ledsSTATES[ledsSTATES_tmp] = static_cast<StateLed>
            (FSM0_safe_cast_to_StateLed(MIN_int32_T));
        }
      } else {
        // Outport: '<Root>/ledsSTATES'
        FSM0_Y.ledsSTATES[ledsSTATES_tmp] = static_cast<StateLed>
          (FSM0_safe_cast_to_StateLed(MAX_int32_T));
      }
    }
    break;
  }
}

// Function for Chart: '<S1>/FSM'
void FSM0ModelClass::FSM0_STATES_HANDLING(void)
{
  boolean_T condIsTrue;
  int32_T b_previousEvent;
  real_T tmp;
  int32_T condIsTrue_tmp;
  int32_T ledsSTATES_tmp;
  switch (FSM0_DW.is_STATES_HANDLING) {
   case FSM0_IN_CPAP:
    FSM0_CPAP();
    break;

   case FSM0_IN_IDLE:
    FSM0_IDLE_j();
    break;

   case FSM0_IN_OFF:
    condIsTrue_tmp = static_cast<int32_T>(IDLE);

    // Inport: '<Root>/buttons'
    tmp = FSM0_unavailable_button(static_cast<real_T>(static_cast<int32_T>
      (FSM0_U.buttons[condIsTrue_tmp])));
    if (tmp < 2.147483648E+9) {
      if (tmp >= -2.147483648E+9) {
        // Outport: '<Root>/ledsSTATES'
        FSM0_Y.ledsSTATES[condIsTrue_tmp] = static_cast<StateLed>
          (FSM0_safe_cast_to_StateLed(static_cast<int32_T>(tmp)));
      } else {
        // Outport: '<Root>/ledsSTATES'
        FSM0_Y.ledsSTATES[condIsTrue_tmp] = static_cast<StateLed>
          (FSM0_safe_cast_to_StateLed(MIN_int32_T));
      }
    } else {
      // Outport: '<Root>/ledsSTATES'
      FSM0_Y.ledsSTATES[condIsTrue_tmp] = static_cast<StateLed>
        (FSM0_safe_cast_to_StateLed(MAX_int32_T));
    }

    condIsTrue_tmp = static_cast<int32_T>(CPAP);

    // Inport: '<Root>/buttons'
    tmp = FSM0_unavailable_button(static_cast<real_T>(static_cast<int32_T>
      (FSM0_U.buttons[condIsTrue_tmp])));
    if (tmp < 2.147483648E+9) {
      if (tmp >= -2.147483648E+9) {
        // Outport: '<Root>/ledsSTATES'
        FSM0_Y.ledsSTATES[condIsTrue_tmp] = static_cast<StateLed>
          (FSM0_safe_cast_to_StateLed(static_cast<int32_T>(tmp)));
      } else {
        // Outport: '<Root>/ledsSTATES'
        FSM0_Y.ledsSTATES[condIsTrue_tmp] = static_cast<StateLed>
          (FSM0_safe_cast_to_StateLed(MIN_int32_T));
      }
    } else {
      // Outport: '<Root>/ledsSTATES'
      FSM0_Y.ledsSTATES[condIsTrue_tmp] = static_cast<StateLed>
        (FSM0_safe_cast_to_StateLed(MAX_int32_T));
    }

    condIsTrue_tmp = static_cast<int32_T>(VCV);

    // Inport: '<Root>/buttons'
    tmp = FSM0_unavailable_button(static_cast<real_T>(static_cast<int32_T>
      (FSM0_U.buttons[condIsTrue_tmp])));
    if (tmp < 2.147483648E+9) {
      if (tmp >= -2.147483648E+9) {
        // Outport: '<Root>/ledsSTATES'
        FSM0_Y.ledsSTATES[condIsTrue_tmp] = static_cast<StateLed>
          (FSM0_safe_cast_to_StateLed(static_cast<int32_T>(tmp)));
      } else {
        // Outport: '<Root>/ledsSTATES'
        FSM0_Y.ledsSTATES[condIsTrue_tmp] = static_cast<StateLed>
          (FSM0_safe_cast_to_StateLed(MIN_int32_T));
      }
    } else {
      // Outport: '<Root>/ledsSTATES'
      FSM0_Y.ledsSTATES[condIsTrue_tmp] = static_cast<StateLed>
        (FSM0_safe_cast_to_StateLed(MAX_int32_T));
    }

    condIsTrue_tmp = static_cast<int32_T>(PRVC);

    // Inport: '<Root>/buttons'
    tmp = FSM0_unavailable_button(static_cast<real_T>(static_cast<int32_T>
      (FSM0_U.buttons[condIsTrue_tmp])));
    if (tmp < 2.147483648E+9) {
      if (tmp >= -2.147483648E+9) {
        // Outport: '<Root>/ledsSTATES'
        FSM0_Y.ledsSTATES[condIsTrue_tmp] = static_cast<StateLed>
          (FSM0_safe_cast_to_StateLed(static_cast<int32_T>(tmp)));
      } else {
        // Outport: '<Root>/ledsSTATES'
        FSM0_Y.ledsSTATES[condIsTrue_tmp] = static_cast<StateLed>
          (FSM0_safe_cast_to_StateLed(MIN_int32_T));
      }
    } else {
      // Outport: '<Root>/ledsSTATES'
      FSM0_Y.ledsSTATES[condIsTrue_tmp] = static_cast<StateLed>
        (FSM0_safe_cast_to_StateLed(MAX_int32_T));
    }
    break;

   case FSM0_IN_PRVC:
    FSM0_PRVC();
    break;

   case FSM0_IN_IDLE_n:
    FSM0_IDLE();
    break;

   case FSM0_IN_TEST_1:
    condIsTrue_tmp = static_cast<int32_T>(IDLE);

    // Inport: '<Root>/buttons'
    condIsTrue = (FSM0_U.buttons[condIsTrue_tmp] == Pressed);
    if ((!condIsTrue) || (!FSM0_DW.condWasTrueAtLastTimeStep_1_d)) {
      FSM0_DW.durationLastReferenceTick_1_j = FSM0_DW.chartAbsoluteTimeCounter;
    }

    FSM0_DW.condWasTrueAtLastTimeStep_1_d = condIsTrue;
    if (FSM0_DW.chartAbsoluteTimeCounter - FSM0_DW.durationLastReferenceTick_1_j
        >= FSM0_P.FSM_debounce_tmo * 100.0F) {
      FSM0_DW.exit_test = false;
      FSM0_DW.durationLastReferenceTick_5_d = FSM0_DW.chartAbsoluteTimeCounter;
      FSM0_DW.durationLastReferenceTick_4_o = FSM0_DW.chartAbsoluteTimeCounter;
      FSM0_DW.durationLastReferenceTick_3_j = FSM0_DW.chartAbsoluteTimeCounter;
      FSM0_DW.durationLastReferenceTick_2_h = FSM0_DW.chartAbsoluteTimeCounter;
      FSM0_DW.durationLastReferenceTick_1 = FSM0_DW.chartAbsoluteTimeCounter;
      FSM0_DW.is_STATES_HANDLING = FSM0_IN_IDLE_n;
      b_previousEvent = static_cast<int32_T>(CPAP);

      // Outport: '<Root>/ledsSTATES'
      FSM0_Y.ledsSTATES[b_previousEvent] = Off;
      ledsSTATES_tmp = static_cast<int32_T>(VCV);

      // Outport: '<Root>/ledsSTATES'
      FSM0_Y.ledsSTATES[ledsSTATES_tmp] = Off;
      FSM0_Y.ledsSTATES[static_cast<int32_T>(PRVC)] = Off;

      // Outport: '<Root>/test_IV_dc'
      FSM0_Y.test_IV_dc = 0.0F;

      // Outport: '<Root>/test_OV_cmd'
      FSM0_Y.test_OV_cmd = false;

      // Inport: '<Root>/buttons'
      FSM0_DW.condWasTrueAtLastTimeStep_1 = (FSM0_U.buttons[condIsTrue_tmp] ==
        Pressed);

      // Inport: '<Root>/MultiSelector_Go'
      FSM0_DW.condWasTrueAtLastTimeStep_2_k = (FSM0_U.MultiSelector_Go ==
        Pressed);

      // Inport: '<Root>/buttons'
      FSM0_DW.condWasTrueAtLastTimeStep_3_n = (FSM0_U.buttons[condIsTrue_tmp] ==
        Released);
      FSM0_DW.condWasTrueAtLastTimeStep_4_k = (FSM0_U.buttons[b_previousEvent] ==
        Pressed);
      FSM0_DW.condWasTrueAtLastTimeStep_5_p = (FSM0_U.buttons[ledsSTATES_tmp] ==
        Pressed);
    } else if (FSM0_DW.temporalCounter_i1 >= FSM0_DW.test1_period * 100.0F) {
      FSM0_DW.durationLastReferenceTick_1_j = FSM0_DW.chartAbsoluteTimeCounter;
      FSM0_DW.is_STATES_HANDLING = FSM0_IN_TEST_1;
      FSM0_DW.temporalCounter_i1 = 0U;

      // Outport: '<Root>/test_IV_dc'
      FSM0_Y.test_IV_dc = 0.0F;

      // Outport: '<Root>/test_OV_cmd'
      FSM0_Y.test_OV_cmd = true;

      // Outport: '<Root>/ledsSTATES'
      FSM0_Y.ledsSTATES[static_cast<int32_T>(CPAP)] = On;

      // Inport: '<Root>/buttons'
      FSM0_DW.condWasTrueAtLastTimeStep_1_d = (FSM0_U.buttons[condIsTrue_tmp] ==
        Pressed);
    } else if (FSM0_DW.sfEvent == FSM0_event_switch_off) {
      b_previousEvent = FSM0_DW.sfEvent;
      FSM0_DW.sfEvent = FSM0_event_blink_IDLE_off;
      if (FSM0_DW.is_active_IDLE_BLINKING != 0U) {
        FSM0_IDLE_BLINKING();
      }

      FSM0_DW.sfEvent = b_previousEvent;
      FSM0_DW.is_STATES_HANDLING = FSM0_IN_OFF;

      // Outport: '<Root>/ledsSTATES'
      FSM0_Y.ledsSTATES[condIsTrue_tmp] = Off;
      FSM0_Y.ledsSTATES[static_cast<int32_T>(CPAP)] = Off;
      FSM0_Y.ledsSTATES[static_cast<int32_T>(VCV)] = Off;
      FSM0_Y.ledsSTATES[static_cast<int32_T>(PRVC)] = Off;

      // Outport: '<Root>/Controller_Mode'
      FSM0_Y.Controller_Mode = IDLE;
    } else {
      // Outport: '<Root>/test_IV_dc'
      FSM0_Y.test_IV_dc += FSM0_DW.test1_dc_peak / FSM0_DW.test1_period *
        FSM0_P.FSM_Ts;
      condIsTrue_tmp = static_cast<int32_T>(VCV);

      // Inport: '<Root>/buttons'
      tmp = FSM0_unavailable_button(static_cast<real_T>(static_cast<int32_T>
        (FSM0_U.buttons[condIsTrue_tmp])));
      if (tmp < 2.147483648E+9) {
        if (tmp >= -2.147483648E+9) {
          // Outport: '<Root>/ledsSTATES'
          FSM0_Y.ledsSTATES[condIsTrue_tmp] = static_cast<StateLed>
            (FSM0_safe_cast_to_StateLed(static_cast<int32_T>(tmp)));
        } else {
          // Outport: '<Root>/ledsSTATES'
          FSM0_Y.ledsSTATES[condIsTrue_tmp] = static_cast<StateLed>
            (FSM0_safe_cast_to_StateLed(MIN_int32_T));
        }
      } else {
        // Outport: '<Root>/ledsSTATES'
        FSM0_Y.ledsSTATES[condIsTrue_tmp] = static_cast<StateLed>
          (FSM0_safe_cast_to_StateLed(MAX_int32_T));
      }

      condIsTrue_tmp = static_cast<int32_T>(PRVC);

      // Inport: '<Root>/buttons'
      tmp = FSM0_unavailable_button(static_cast<real_T>(static_cast<int32_T>
        (FSM0_U.buttons[condIsTrue_tmp])));
      if (tmp < 2.147483648E+9) {
        if (tmp >= -2.147483648E+9) {
          // Outport: '<Root>/ledsSTATES'
          FSM0_Y.ledsSTATES[condIsTrue_tmp] = static_cast<StateLed>
            (FSM0_safe_cast_to_StateLed(static_cast<int32_T>(tmp)));
        } else {
          // Outport: '<Root>/ledsSTATES'
          FSM0_Y.ledsSTATES[condIsTrue_tmp] = static_cast<StateLed>
            (FSM0_safe_cast_to_StateLed(MIN_int32_T));
        }
      } else {
        // Outport: '<Root>/ledsSTATES'
        FSM0_Y.ledsSTATES[condIsTrue_tmp] = static_cast<StateLed>
          (FSM0_safe_cast_to_StateLed(MAX_int32_T));
      }
    }
    break;

   case FSM0_IN_TEST_2:
    condIsTrue_tmp = static_cast<int32_T>(IDLE);

    // Inport: '<Root>/buttons'
    condIsTrue = (FSM0_U.buttons[condIsTrue_tmp] == Pressed);
    if ((!condIsTrue) || (!FSM0_DW.condWasTrueAtLastTimeStep_1_c)) {
      FSM0_DW.durationLastReferenceTick_1_lo = FSM0_DW.chartAbsoluteTimeCounter;
    }

    FSM0_DW.condWasTrueAtLastTimeStep_1_c = condIsTrue;
    if (FSM0_DW.chartAbsoluteTimeCounter -
        FSM0_DW.durationLastReferenceTick_1_lo >= FSM0_P.FSM_debounce_tmo *
        100.0F) {
      FSM0_DW.exit_test = false;
      FSM0_DW.durationLastReferenceTick_5_d = FSM0_DW.chartAbsoluteTimeCounter;
      FSM0_DW.durationLastReferenceTick_4_o = FSM0_DW.chartAbsoluteTimeCounter;
      FSM0_DW.durationLastReferenceTick_3_j = FSM0_DW.chartAbsoluteTimeCounter;
      FSM0_DW.durationLastReferenceTick_2_h = FSM0_DW.chartAbsoluteTimeCounter;
      FSM0_DW.durationLastReferenceTick_1 = FSM0_DW.chartAbsoluteTimeCounter;
      FSM0_DW.is_STATES_HANDLING = FSM0_IN_IDLE_n;
      b_previousEvent = static_cast<int32_T>(CPAP);

      // Outport: '<Root>/ledsSTATES'
      FSM0_Y.ledsSTATES[b_previousEvent] = Off;
      ledsSTATES_tmp = static_cast<int32_T>(VCV);

      // Outport: '<Root>/ledsSTATES'
      FSM0_Y.ledsSTATES[ledsSTATES_tmp] = Off;
      FSM0_Y.ledsSTATES[static_cast<int32_T>(PRVC)] = Off;

      // Outport: '<Root>/test_IV_dc'
      FSM0_Y.test_IV_dc = 0.0F;

      // Outport: '<Root>/test_OV_cmd'
      FSM0_Y.test_OV_cmd = false;

      // Inport: '<Root>/buttons'
      FSM0_DW.condWasTrueAtLastTimeStep_1 = (FSM0_U.buttons[condIsTrue_tmp] ==
        Pressed);

      // Inport: '<Root>/MultiSelector_Go'
      FSM0_DW.condWasTrueAtLastTimeStep_2_k = (FSM0_U.MultiSelector_Go ==
        Pressed);

      // Inport: '<Root>/buttons'
      FSM0_DW.condWasTrueAtLastTimeStep_3_n = (FSM0_U.buttons[condIsTrue_tmp] ==
        Released);
      FSM0_DW.condWasTrueAtLastTimeStep_4_k = (FSM0_U.buttons[b_previousEvent] ==
        Pressed);
      FSM0_DW.condWasTrueAtLastTimeStep_5_p = (FSM0_U.buttons[ledsSTATES_tmp] ==
        Pressed);
    } else if (FSM0_DW.temporalCounter_i1 >= FSM0_DW.test2_period * 100.0F) {
      // Outport: '<Root>/test_OV_cmd'
      FSM0_Y.test_OV_cmd = !FSM0_Y.test_OV_cmd;
      FSM0_DW.durationLastReferenceTick_1_lo = FSM0_DW.chartAbsoluteTimeCounter;
      FSM0_DW.is_STATES_HANDLING = FSM0_IN_TEST_2;
      FSM0_DW.temporalCounter_i1 = 0U;

      // Outport: '<Root>/test_IV_dc'
      FSM0_Y.test_IV_dc = 100.0F;

      // Outport: '<Root>/ledsSTATES'
      FSM0_Y.ledsSTATES[static_cast<int32_T>(VCV)] = On;

      // Inport: '<Root>/buttons'
      FSM0_DW.condWasTrueAtLastTimeStep_1_c = (FSM0_U.buttons[condIsTrue_tmp] ==
        Pressed);
    } else if (FSM0_DW.sfEvent == FSM0_event_switch_off) {
      b_previousEvent = FSM0_DW.sfEvent;
      FSM0_DW.sfEvent = FSM0_event_blink_IDLE_off;
      if (FSM0_DW.is_active_IDLE_BLINKING != 0U) {
        FSM0_IDLE_BLINKING();
      }

      FSM0_DW.sfEvent = b_previousEvent;
      FSM0_DW.is_STATES_HANDLING = FSM0_IN_OFF;

      // Outport: '<Root>/ledsSTATES'
      FSM0_Y.ledsSTATES[condIsTrue_tmp] = Off;
      FSM0_Y.ledsSTATES[static_cast<int32_T>(CPAP)] = Off;
      FSM0_Y.ledsSTATES[static_cast<int32_T>(VCV)] = Off;
      FSM0_Y.ledsSTATES[static_cast<int32_T>(PRVC)] = Off;

      // Outport: '<Root>/Controller_Mode'
      FSM0_Y.Controller_Mode = IDLE;
    } else {
      condIsTrue_tmp = static_cast<int32_T>(CPAP);

      // Inport: '<Root>/buttons'
      tmp = FSM0_unavailable_button(static_cast<real_T>(static_cast<int32_T>
        (FSM0_U.buttons[condIsTrue_tmp])));
      if (tmp < 2.147483648E+9) {
        if (tmp >= -2.147483648E+9) {
          // Outport: '<Root>/ledsSTATES'
          FSM0_Y.ledsSTATES[condIsTrue_tmp] = static_cast<StateLed>
            (FSM0_safe_cast_to_StateLed(static_cast<int32_T>(tmp)));
        } else {
          // Outport: '<Root>/ledsSTATES'
          FSM0_Y.ledsSTATES[condIsTrue_tmp] = static_cast<StateLed>
            (FSM0_safe_cast_to_StateLed(MIN_int32_T));
        }
      } else {
        // Outport: '<Root>/ledsSTATES'
        FSM0_Y.ledsSTATES[condIsTrue_tmp] = static_cast<StateLed>
          (FSM0_safe_cast_to_StateLed(MAX_int32_T));
      }

      condIsTrue_tmp = static_cast<int32_T>(PRVC);

      // Inport: '<Root>/buttons'
      tmp = FSM0_unavailable_button(static_cast<real_T>(static_cast<int32_T>
        (FSM0_U.buttons[condIsTrue_tmp])));
      if (tmp < 2.147483648E+9) {
        if (tmp >= -2.147483648E+9) {
          // Outport: '<Root>/ledsSTATES'
          FSM0_Y.ledsSTATES[condIsTrue_tmp] = static_cast<StateLed>
            (FSM0_safe_cast_to_StateLed(static_cast<int32_T>(tmp)));
        } else {
          // Outport: '<Root>/ledsSTATES'
          FSM0_Y.ledsSTATES[condIsTrue_tmp] = static_cast<StateLed>
            (FSM0_safe_cast_to_StateLed(MIN_int32_T));
        }
      } else {
        // Outport: '<Root>/ledsSTATES'
        FSM0_Y.ledsSTATES[condIsTrue_tmp] = static_cast<StateLed>
          (FSM0_safe_cast_to_StateLed(MAX_int32_T));
      }
    }
    break;

   case FSM0_IN_VCV:
    FSM0_VCV();
    break;
  }
}

// Function for Chart: '<S1>/FSM'
real_T FSM0ModelClass::FSM0_check_definitive_alarms(void)
{
  real_T fault;
  fault = 0.0;

  // Outport: '<Root>/Controller_Mode' incorporates:
  //   Inport: '<Root>/Controller_Signals'
  //   Outport: '<Root>/idALARM'

  if (FSM0_Y.Controller_Mode != IDLE) {
    if (FSM0_U.Controller_Signals[6] < FSM0_DW.alarms_S7_min) {
      FSM0_Y.idALARM = ALARM_PressureDrop;
      fault = 1.0;
    } else {
      if (FSM0_U.Controller_Signals[6] > FSM0_DW.alarms_S7_max) {
        FSM0_Y.idALARM = ALARM_BreakPR1;
        fault = 1.0;
      }
    }
  }

  // End of Outport: '<Root>/Controller_Mode'
  return fault;
}

// Function for Chart: '<S1>/FSM'
real_T FSM0ModelClass::FSM0_check_inlet_overpressure(void)
{
  // Inport: '<Root>/Controller_Signals'
  if (FSM0_U.Controller_Signals[0] > FSM0_DW.alarms_S1S3_max) {
    // Outport: '<Root>/idALARM'
    FSM0_Y.idALARM = ALARM_OverPressureInlet;
  } else if (FSM0_U.Controller_Signals[2] > FSM0_DW.alarms_S1S3_max) {
    // Outport: '<Root>/idALARM'
    FSM0_Y.idALARM = ALARM_OverPressureInlet;
  } else {
    // Outport: '<Root>/idALARM'
    FSM0_Y.idALARM = ALARM_None;
  }

  // End of Inport: '<Root>/Controller_Signals'

  // Outport: '<Root>/idALARM'
  return FSM0_Y.idALARM != ALARM_None;
}

// Function for Chart: '<S1>/FSM'
real_T FSM0ModelClass::FSM0_check_transitive_alarms(void)
{
  // Inport: '<Root>/Controller_Signals'
  if (FSM0_U.Controller_Signals[4] < FSM0_DW.alarms_PEEP) {
    // Outport: '<Root>/idALARM'
    FSM0_Y.idALARM = ALARM_PEEP;
  } else if ((!rtIsInfF(FSM0_U.Controller_Signals[8])) && (!rtIsNaNF
              (FSM0_U.Controller_Signals[8]))) {
    if (FSM0_U.Controller_Signals[8] < FSM0_DW.alarms_TD_min) {
      // Outport: '<Root>/idALARM'
      FSM0_Y.idALARM = ALARM_LowVolume;
    } else {
      // Outport: '<Root>/idALARM'
      FSM0_Y.idALARM = ALARM_None;
    }
  } else {
    // Outport: '<Root>/idALARM'
    FSM0_Y.idALARM = ALARM_None;
  }

  // End of Inport: '<Root>/Controller_Signals'

  // Outport: '<Root>/idALARM'
  return FSM0_Y.idALARM != ALARM_None;
}

void FSM0ModelClass::FSM0_MedianFilterCG_resetImpl
  (c_dsp_private_MedianFilterCG__T *obj)
{
  real32_T cnt1;
  real32_T cnt2;
  int32_T c_index;
  int32_T obj_0;
  std::memset(&obj->pBuf[0], 0, sizeof(real32_T) << 6U);
  std::memset(&obj->pPos[0], 0, sizeof(real32_T) << 6U);
  std::memset(&obj->pHeap[0], 0, sizeof(real32_T) << 6U);
  obj->pWinLen = 16.0F;
  obj->pIdx[0] = obj->pWinLen;
  obj->pIdx[1] = obj->pWinLen;
  obj->pIdx[2] = obj->pWinLen;
  obj->pIdx[3] = obj->pWinLen;
  obj->pMidHeap = std::ceil((obj->pWinLen + 1.0F) / 2.0F);
  cnt1 = (obj->pWinLen - 1.0F) / 2.0F;
  if (cnt1 < 0.0F) {
    obj->pMinHeapLength = std::ceil(cnt1);
  } else {
    obj->pMinHeapLength = std::floor(cnt1);
  }

  cnt1 = obj->pWinLen / 2.0F;
  if (cnt1 < 0.0F) {
    obj->pMaxHeapLength = std::ceil(cnt1);
  } else {
    obj->pMaxHeapLength = std::floor(cnt1);
  }

  cnt1 = 1.0F;
  cnt2 = obj->pWinLen;
  for (c_index = 0; c_index < 16; c_index++) {
    if (std::fmod(-static_cast<real32_T>(c_index) + 16.0F, 2.0F) == 0.0F) {
      obj_0 = 16 - c_index;
      obj->pPos[obj_0 - 1] = cnt1;
      obj->pPos[obj_0 + 15] = cnt1;
      obj->pPos[obj_0 + 31] = cnt1;
      obj->pPos[obj_0 + 47] = cnt1;
      cnt1++;
    } else {
      obj_0 = 16 - c_index;
      obj->pPos[obj_0 - 1] = cnt2;
      obj->pPos[obj_0 + 15] = cnt2;
      obj->pPos[obj_0 + 31] = cnt2;
      obj->pPos[obj_0 + 47] = cnt2;
      cnt2--;
    }

    obj_0 = static_cast<int32_T>(obj->pPos[15 - c_index]);
    obj->pHeap[obj_0 - 1] = -static_cast<real32_T>(c_index) + 16.0F;
    obj->pHeap[obj_0 + 15] = -static_cast<real32_T>(c_index) + 16.0F;
    obj->pHeap[obj_0 + 31] = -static_cast<real32_T>(c_index) + 16.0F;
    obj->pHeap[obj_0 + 47] = -static_cast<real32_T>(c_index) + 16.0F;
  }
}

void FSM0ModelClass::F_MedianFilterCG_trickleDownMax
  (c_dsp_private_MedianFilterCG__T *obj, real32_T i, int32_T chanIdx)
{
  real32_T temp;
  real32_T u;
  int32_T tmp;
  real32_T ind2;
  int32_T temp_tmp;
  int32_T tmp_0;
  boolean_T exitg1;
  exitg1 = false;
  while ((!exitg1) && (i >= -obj->pMaxHeapLength)) {
    if ((i < -1.0F) && (i > -obj->pMaxHeapLength)) {
      tmp = (chanIdx - 1) << 4;
      if (obj->pBuf[(static_cast<int32_T>(obj->pHeap[(tmp + static_cast<int32_T>
             (i + obj->pMidHeap)) - 1]) + tmp) - 1] < obj->pBuf
          [(static_cast<int32_T>(obj->pHeap[(static_cast<int32_T>((i - 1.0F) +
              obj->pMidHeap) + tmp) - 1]) + tmp) - 1]) {
        i--;
      }
    }

    u = i / 2.0F;
    if (u < 0.0F) {
      u = std::ceil(u);
    } else {
      u = std::floor(u);
    }

    tmp = (chanIdx - 1) << 4;
    ind2 = i + obj->pMidHeap;
    if (!(obj->pBuf[(static_cast<int32_T>(obj->pHeap[(tmp + static_cast<int32_T>
            (u + obj->pMidHeap)) - 1]) + tmp) - 1] < obj->pBuf
          [(static_cast<int32_T>(obj->pHeap[(tmp + static_cast<int32_T>(ind2)) -
           1]) + tmp) - 1])) {
      exitg1 = true;
    } else {
      u = i / 2.0F;
      if (u < 0.0F) {
        u = std::ceil(u);
      } else {
        u = std::floor(u);
      }

      u += obj->pMidHeap;
      temp_tmp = (tmp + static_cast<int32_T>(u)) - 1;
      temp = obj->pHeap[temp_tmp];
      tmp_0 = (((chanIdx - 1) << 4) + static_cast<int32_T>(ind2)) - 1;
      obj->pHeap[temp_tmp] = obj->pHeap[tmp_0];
      obj->pHeap[tmp_0] = temp;
      obj->pPos[(static_cast<int32_T>(obj->pHeap[temp_tmp]) + tmp) - 1] = u;
      obj->pPos[(static_cast<int32_T>(obj->pHeap[tmp_0]) + ((chanIdx - 1) << 4))
        - 1] = ind2;
      i *= 2.0F;
    }
  }
}

void FSM0ModelClass::F_MedianFilterCG_trickleDownMin
  (c_dsp_private_MedianFilterCG__T *obj, real32_T i, int32_T chanIdx)
{
  real32_T temp;
  real32_T u;
  int32_T tmp;
  real32_T ind1;
  int32_T temp_tmp;
  int32_T tmp_0;
  boolean_T exitg1;
  exitg1 = false;
  while ((!exitg1) && (i <= obj->pMinHeapLength)) {
    if ((i > 1.0F) && (i < obj->pMinHeapLength)) {
      tmp = (chanIdx - 1) << 4;
      if (obj->pBuf[(static_cast<int32_T>(obj->pHeap[(static_cast<int32_T>((i +
               1.0F) + obj->pMidHeap) + tmp) - 1]) + tmp) - 1] < obj->pBuf[(
           static_cast<int32_T>(obj->pHeap[(tmp + static_cast<int32_T>(i +
              obj->pMidHeap)) - 1]) + tmp) - 1]) {
        i++;
      }
    }

    u = i / 2.0F;
    if (u < 0.0F) {
      u = std::ceil(u);
    } else {
      u = std::floor(u);
    }

    tmp = (chanIdx - 1) << 4;
    ind1 = i + obj->pMidHeap;
    if (!(obj->pBuf[(static_cast<int32_T>(obj->pHeap[(tmp + static_cast<int32_T>
            (ind1)) - 1]) + tmp) - 1] < obj->pBuf[(static_cast<int32_T>
          (obj->pHeap[(tmp + static_cast<int32_T>(u + obj->pMidHeap)) - 1]) +
          tmp) - 1])) {
      exitg1 = true;
    } else {
      u = i / 2.0F;
      if (u < 0.0F) {
        u = std::ceil(u);
      } else {
        u = std::floor(u);
      }

      u += obj->pMidHeap;
      temp_tmp = (((chanIdx - 1) << 4) + static_cast<int32_T>(ind1)) - 1;
      temp = obj->pHeap[temp_tmp];
      tmp_0 = (tmp + static_cast<int32_T>(u)) - 1;
      obj->pHeap[temp_tmp] = obj->pHeap[tmp_0];
      obj->pHeap[tmp_0] = temp;
      obj->pPos[(static_cast<int32_T>(obj->pHeap[temp_tmp]) + ((chanIdx - 1) <<
                  4)) - 1] = ind1;
      obj->pPos[(static_cast<int32_T>(obj->pHeap[tmp_0]) + tmp) - 1] = u;
      i *= 2.0F;
    }
  }
}

void FSM0ModelClass::FSM0_SystemCore_release(dsp_MedianFilter_FSM0_T *obj)
{
  if ((obj->isInitialized == 1) && obj->isSetupComplete) {
    obj->NumChannels = -1;
    if (obj->pMID.isInitialized == 1) {
      obj->pMID.isInitialized = 2;
    }
  }
}

void FSM0ModelClass::FSM0_SystemCore_delete(dsp_MedianFilter_FSM0_T *obj)
{
  FSM0_SystemCore_release(obj);
}

void FSM0ModelClass::matlabCodegenHandle_matlabCodeg(dsp_MedianFilter_FSM0_T
  *obj)
{
  if (!obj->matlabCodegenIsDeleted) {
    obj->matlabCodegenIsDeleted = true;
    FSM0_SystemCore_delete(obj);
  }
}

// Model step function
void FSM0ModelClass::step()
{
  int32_T c_previousEvent;
  dsp_MedianFilter_FSM0_T *obj;
  c_dsp_private_MedianFilterCG__T *obj_0;
  real32_T vprev;
  real32_T p;
  real32_T i;
  boolean_T flag;
  real32_T temp;
  real32_T rtb_MedianFilter[4];
  int32_T flag_tmp;
  int32_T flag_tmp_0;
  int32_T flag_tmp_1;
  boolean_T exitg1;

  // Outputs for Atomic SubSystem: '<Root>/FSM'
  // RateTransition: '<S1>/Rate Transition'
  if ((&FSM0_M)->Timing.TaskCounters.TID[1] == 0) {
    FSM0_B.RateTransition[0] = FSM0_DW.RateTransition_Buffer0[0];
    FSM0_B.RateTransition[1] = FSM0_DW.RateTransition_Buffer0[1];
    FSM0_B.RateTransition[2] = FSM0_DW.RateTransition_Buffer0[2];
    FSM0_B.RateTransition[3] = FSM0_DW.RateTransition_Buffer0[3];
  }

  // End of RateTransition: '<S1>/Rate Transition'

  // Constant: '<S11>/BPM [1//min]'
  FSM0_B.MatrixConcatenate[0] = FSM0_P.BPM1min_Value[0];

  // Constant: '<S11>/PIP [cmH2O]'
  FSM0_B.MatrixConcatenate[3] = FSM0_P.PIPcmH2O_Value[0];

  // Constant: '<S11>/IE_ratio [-]'
  FSM0_B.MatrixConcatenate[6] = FSM0_P.IE_ratio_Value[0];

  // Constant: '<S11>/TD [ml]'
  FSM0_B.MatrixConcatenate[9] = FSM0_P.TDml_Value[0];

  // Constant: '<S13>/Proportional'
  FSM0_B.MatrixConcatenate[12] = FSM0_P.Proportional_Value[0];

  // Constant: '<S13>/Integral'
  FSM0_B.MatrixConcatenate[15] = FSM0_P.Integral_Value[0];

  // Constant: '<S13>/Proportional Recovery'
  FSM0_B.MatrixConcatenate[18] = FSM0_P.ProportionalRecovery_Value[0];

  // Constant: '<S13>/Integral Recovery'
  FSM0_B.MatrixConcatenate[21] = FSM0_P.IntegralRecovery_Value[0];

  // Constant: '<S9>/Proportional'
  FSM0_B.MatrixConcatenate[24] = FSM0_P.Proportional_Value_j[0];

  // Constant: '<S9>/Integral'
  FSM0_B.MatrixConcatenate[27] = FSM0_P.Integral_Value_k[0];

  // Constant: '<S9>/Proportional Recovery'
  FSM0_B.MatrixConcatenate[30] = FSM0_P.ProportionalRecovery_Value_g[0];

  // Constant: '<S9>/Integral Recovery'
  FSM0_B.MatrixConcatenate[33] = FSM0_P.IntegralRecovery_Value_b[0];

  // Constant: '<S10>/Feed-Forward'
  FSM0_B.MatrixConcatenate[36] = FSM0_P.FeedForward_Value[0];

  // Constant: '<S10>/Feed-Forward Recovery'
  FSM0_B.MatrixConcatenate[39] = FSM0_P.FeedForwardRecovery_Value[0];

  // Constant: '<S10>/PIP Step Perc [%]'
  FSM0_B.MatrixConcatenate[42] = FSM0_P.PIPStepPerc_Value[0];

  // Constant: '<S10>/PIP Rise Perc [%]'
  FSM0_B.MatrixConcatenate[45] = FSM0_P.PIPRisePerc_Value[0];

  // Constant: '<S8>/Enable Trigger'
  FSM0_B.MatrixConcatenate[48] = FSM0_P.EnableTrigger_Value[0];

  // Constant: '<S8>/Pressure Threshold [cmH2O]'
  FSM0_B.MatrixConcatenate[51] = FSM0_P.PressureThresholdcmH2O_Value[0];

  // Constant: '<S8>/Flow-by Target'
  FSM0_B.MatrixConcatenate[54] = FSM0_P.FlowbyTarget_Value[0];

  // Constant: '<S8>/Not Used 1'
  FSM0_B.MatrixConcatenate[57] = FSM0_P.NotUsed1_Value[0];

  // Constant: '<S12>/Test1 Peak [%]'
  FSM0_B.MatrixConcatenate[60] = FSM0_P.Test1Peak_Value[0];

  // Constant: '<S12>/Test1 Period [s]'
  FSM0_B.MatrixConcatenate[63] = FSM0_P.Test1Periods_Value[0];

  // Constant: '<S12>/Test2 Period [s]'
  FSM0_B.MatrixConcatenate[66] = FSM0_P.Test2Periods_Value[0];

  // Constant: '<S12>/Not Used 1'
  FSM0_B.MatrixConcatenate[69] = FSM0_P.NotUsed1_Value_h[0];

  // Constant: '<S6>/S7 Min [cmH2O]'
  FSM0_B.MatrixConcatenate[72] = FSM0_P.S7MincmH2O_Value[0];

  // Constant: '<S6>/S7 Max [cmH2O]'
  FSM0_B.MatrixConcatenate[75] = FSM0_P.S7MaxcmH2O_Value[0];

  // Constant: '<S6>/S1,S3 Max [cmH2O]'
  FSM0_B.MatrixConcatenate[78] = FSM0_P.S1S3MaxcmH2O_Value[0];

  // Constant: '<S6>/S1,S3 Reinstate [cmH2O]'
  FSM0_B.MatrixConcatenate[81] = FSM0_P.S1S3ReinstatecmH2O_Value[0];

  // Constant: '<S7>/PEEP [cmH2O]'
  FSM0_B.MatrixConcatenate[84] = FSM0_P.PEEPcmH2O_Value[0];

  // Constant: '<S7>/TD Min [L]'
  FSM0_B.MatrixConcatenate[87] = FSM0_P.TDMinL_Value[0];

  // Constant: '<S7>/Not Used 1'
  FSM0_B.MatrixConcatenate[90] = FSM0_P.NotUsed1_Value_l[0];

  // Constant: '<S7>/Not Used 2'
  FSM0_B.MatrixConcatenate[93] = FSM0_P.NotUsed2_Value[0];

  // Constant: '<S11>/BPM [1//min]'
  FSM0_B.MatrixConcatenate[1] = FSM0_P.BPM1min_Value[1];

  // Constant: '<S11>/PIP [cmH2O]'
  FSM0_B.MatrixConcatenate[4] = FSM0_P.PIPcmH2O_Value[1];

  // Constant: '<S11>/IE_ratio [-]'
  FSM0_B.MatrixConcatenate[7] = FSM0_P.IE_ratio_Value[1];

  // Constant: '<S11>/TD [ml]'
  FSM0_B.MatrixConcatenate[10] = FSM0_P.TDml_Value[1];

  // Constant: '<S13>/Proportional'
  FSM0_B.MatrixConcatenate[13] = FSM0_P.Proportional_Value[1];

  // Constant: '<S13>/Integral'
  FSM0_B.MatrixConcatenate[16] = FSM0_P.Integral_Value[1];

  // Constant: '<S13>/Proportional Recovery'
  FSM0_B.MatrixConcatenate[19] = FSM0_P.ProportionalRecovery_Value[1];

  // Constant: '<S13>/Integral Recovery'
  FSM0_B.MatrixConcatenate[22] = FSM0_P.IntegralRecovery_Value[1];

  // Constant: '<S9>/Proportional'
  FSM0_B.MatrixConcatenate[25] = FSM0_P.Proportional_Value_j[1];

  // Constant: '<S9>/Integral'
  FSM0_B.MatrixConcatenate[28] = FSM0_P.Integral_Value_k[1];

  // Constant: '<S9>/Proportional Recovery'
  FSM0_B.MatrixConcatenate[31] = FSM0_P.ProportionalRecovery_Value_g[1];

  // Constant: '<S9>/Integral Recovery'
  FSM0_B.MatrixConcatenate[34] = FSM0_P.IntegralRecovery_Value_b[1];

  // Constant: '<S10>/Feed-Forward'
  FSM0_B.MatrixConcatenate[37] = FSM0_P.FeedForward_Value[1];

  // Constant: '<S10>/Feed-Forward Recovery'
  FSM0_B.MatrixConcatenate[40] = FSM0_P.FeedForwardRecovery_Value[1];

  // Constant: '<S10>/PIP Step Perc [%]'
  FSM0_B.MatrixConcatenate[43] = FSM0_P.PIPStepPerc_Value[1];

  // Constant: '<S10>/PIP Rise Perc [%]'
  FSM0_B.MatrixConcatenate[46] = FSM0_P.PIPRisePerc_Value[1];

  // Constant: '<S8>/Enable Trigger'
  FSM0_B.MatrixConcatenate[49] = FSM0_P.EnableTrigger_Value[1];

  // Constant: '<S8>/Pressure Threshold [cmH2O]'
  FSM0_B.MatrixConcatenate[52] = FSM0_P.PressureThresholdcmH2O_Value[1];

  // Constant: '<S8>/Flow-by Target'
  FSM0_B.MatrixConcatenate[55] = FSM0_P.FlowbyTarget_Value[1];

  // Constant: '<S8>/Not Used 1'
  FSM0_B.MatrixConcatenate[58] = FSM0_P.NotUsed1_Value[1];

  // Constant: '<S12>/Test1 Peak [%]'
  FSM0_B.MatrixConcatenate[61] = FSM0_P.Test1Peak_Value[1];

  // Constant: '<S12>/Test1 Period [s]'
  FSM0_B.MatrixConcatenate[64] = FSM0_P.Test1Periods_Value[1];

  // Constant: '<S12>/Test2 Period [s]'
  FSM0_B.MatrixConcatenate[67] = FSM0_P.Test2Periods_Value[1];

  // Constant: '<S12>/Not Used 1'
  FSM0_B.MatrixConcatenate[70] = FSM0_P.NotUsed1_Value_h[1];

  // Constant: '<S6>/S7 Min [cmH2O]'
  FSM0_B.MatrixConcatenate[73] = FSM0_P.S7MincmH2O_Value[1];

  // Constant: '<S6>/S7 Max [cmH2O]'
  FSM0_B.MatrixConcatenate[76] = FSM0_P.S7MaxcmH2O_Value[1];

  // Constant: '<S6>/S1,S3 Max [cmH2O]'
  FSM0_B.MatrixConcatenate[79] = FSM0_P.S1S3MaxcmH2O_Value[1];

  // Constant: '<S6>/S1,S3 Reinstate [cmH2O]'
  FSM0_B.MatrixConcatenate[82] = FSM0_P.S1S3ReinstatecmH2O_Value[1];

  // Constant: '<S7>/PEEP [cmH2O]'
  FSM0_B.MatrixConcatenate[85] = FSM0_P.PEEPcmH2O_Value[1];

  // Constant: '<S7>/TD Min [L]'
  FSM0_B.MatrixConcatenate[88] = FSM0_P.TDMinL_Value[1];

  // Constant: '<S7>/Not Used 1'
  FSM0_B.MatrixConcatenate[91] = FSM0_P.NotUsed1_Value_l[1];

  // Constant: '<S7>/Not Used 2'
  FSM0_B.MatrixConcatenate[94] = FSM0_P.NotUsed2_Value[1];

  // Constant: '<S11>/BPM [1//min]'
  FSM0_B.MatrixConcatenate[2] = FSM0_P.BPM1min_Value[2];

  // Constant: '<S11>/PIP [cmH2O]'
  FSM0_B.MatrixConcatenate[5] = FSM0_P.PIPcmH2O_Value[2];

  // Constant: '<S11>/IE_ratio [-]'
  FSM0_B.MatrixConcatenate[8] = FSM0_P.IE_ratio_Value[2];

  // Constant: '<S11>/TD [ml]'
  FSM0_B.MatrixConcatenate[11] = FSM0_P.TDml_Value[2];

  // Constant: '<S13>/Proportional'
  FSM0_B.MatrixConcatenate[14] = FSM0_P.Proportional_Value[2];

  // Constant: '<S13>/Integral'
  FSM0_B.MatrixConcatenate[17] = FSM0_P.Integral_Value[2];

  // Constant: '<S13>/Proportional Recovery'
  FSM0_B.MatrixConcatenate[20] = FSM0_P.ProportionalRecovery_Value[2];

  // Constant: '<S13>/Integral Recovery'
  FSM0_B.MatrixConcatenate[23] = FSM0_P.IntegralRecovery_Value[2];

  // Constant: '<S9>/Proportional'
  FSM0_B.MatrixConcatenate[26] = FSM0_P.Proportional_Value_j[2];

  // Constant: '<S9>/Integral'
  FSM0_B.MatrixConcatenate[29] = FSM0_P.Integral_Value_k[2];

  // Constant: '<S9>/Proportional Recovery'
  FSM0_B.MatrixConcatenate[32] = FSM0_P.ProportionalRecovery_Value_g[2];

  // Constant: '<S9>/Integral Recovery'
  FSM0_B.MatrixConcatenate[35] = FSM0_P.IntegralRecovery_Value_b[2];

  // Constant: '<S10>/Feed-Forward'
  FSM0_B.MatrixConcatenate[38] = FSM0_P.FeedForward_Value[2];

  // Constant: '<S10>/Feed-Forward Recovery'
  FSM0_B.MatrixConcatenate[41] = FSM0_P.FeedForwardRecovery_Value[2];

  // Constant: '<S10>/PIP Step Perc [%]'
  FSM0_B.MatrixConcatenate[44] = FSM0_P.PIPStepPerc_Value[2];

  // Constant: '<S10>/PIP Rise Perc [%]'
  FSM0_B.MatrixConcatenate[47] = FSM0_P.PIPRisePerc_Value[2];

  // Constant: '<S8>/Enable Trigger'
  FSM0_B.MatrixConcatenate[50] = FSM0_P.EnableTrigger_Value[2];

  // Constant: '<S8>/Pressure Threshold [cmH2O]'
  FSM0_B.MatrixConcatenate[53] = FSM0_P.PressureThresholdcmH2O_Value[2];

  // Constant: '<S8>/Flow-by Target'
  FSM0_B.MatrixConcatenate[56] = FSM0_P.FlowbyTarget_Value[2];

  // Constant: '<S8>/Not Used 1'
  FSM0_B.MatrixConcatenate[59] = FSM0_P.NotUsed1_Value[2];

  // Constant: '<S12>/Test1 Peak [%]'
  FSM0_B.MatrixConcatenate[62] = FSM0_P.Test1Peak_Value[2];

  // Constant: '<S12>/Test1 Period [s]'
  FSM0_B.MatrixConcatenate[65] = FSM0_P.Test1Periods_Value[2];

  // Constant: '<S12>/Test2 Period [s]'
  FSM0_B.MatrixConcatenate[68] = FSM0_P.Test2Periods_Value[2];

  // Constant: '<S12>/Not Used 1'
  FSM0_B.MatrixConcatenate[71] = FSM0_P.NotUsed1_Value_h[2];

  // Constant: '<S6>/S7 Min [cmH2O]'
  FSM0_B.MatrixConcatenate[74] = FSM0_P.S7MincmH2O_Value[2];

  // Constant: '<S6>/S7 Max [cmH2O]'
  FSM0_B.MatrixConcatenate[77] = FSM0_P.S7MaxcmH2O_Value[2];

  // Constant: '<S6>/S1,S3 Max [cmH2O]'
  FSM0_B.MatrixConcatenate[80] = FSM0_P.S1S3MaxcmH2O_Value[2];

  // Constant: '<S6>/S1,S3 Reinstate [cmH2O]'
  FSM0_B.MatrixConcatenate[83] = FSM0_P.S1S3ReinstatecmH2O_Value[2];

  // Constant: '<S7>/PEEP [cmH2O]'
  FSM0_B.MatrixConcatenate[86] = FSM0_P.PEEPcmH2O_Value[2];

  // Constant: '<S7>/TD Min [L]'
  FSM0_B.MatrixConcatenate[89] = FSM0_P.TDMinL_Value[2];

  // Constant: '<S7>/Not Used 1'
  FSM0_B.MatrixConcatenate[92] = FSM0_P.NotUsed1_Value_l[2];

  // Constant: '<S7>/Not Used 2'
  FSM0_B.MatrixConcatenate[95] = FSM0_P.NotUsed2_Value[2];

  // Chart: '<S1>/FSM' incorporates:
  //   Inport: '<Root>/Controller_Signals'
  //   Inport: '<Root>/MultiSelector_Go'
  //   Inport: '<Root>/buttons'
  //   Outport: '<Root>/Controller_Mode'
  //   Outport: '<Root>/ledsSTATES'

  FSM0_DW.chartAbsoluteTimeCounter++;
  c_previousEvent = static_cast<int32_T>(IDLE);
  flag = (FSM0_U.buttons[c_previousEvent] == Pressed);
  if ((!flag) || (!FSM0_DW.condWasTrueAtLastTimeStep_1)) {
    FSM0_DW.durationLastReferenceTick_1 = FSM0_DW.chartAbsoluteTimeCounter;
  }

  FSM0_DW.condWasTrueAtLastTimeStep_1 = flag;
  flag = (FSM0_U.buttons[c_previousEvent] == Pressed);
  if ((!flag) || (!FSM0_DW.condWasTrueAtLastTimeStep_1_g)) {
    FSM0_DW.durationLastReferenceTick_1_o = FSM0_DW.chartAbsoluteTimeCounter;
  }

  FSM0_DW.condWasTrueAtLastTimeStep_1_g = flag;
  flag = (FSM0_U.buttons[c_previousEvent] == Released);
  if ((!flag) || (!FSM0_DW.condWasTrueAtLastTimeStep_2)) {
    FSM0_DW.durationLastReferenceTick_2 = FSM0_DW.chartAbsoluteTimeCounter;
  }

  FSM0_DW.condWasTrueAtLastTimeStep_2 = flag;
  flag_tmp = static_cast<int32_T>(CPAP);
  flag = (FSM0_U.buttons[flag_tmp] == Pressed);
  if ((!flag) || (!FSM0_DW.condWasTrueAtLastTimeStep_3)) {
    FSM0_DW.durationLastReferenceTick_3 = FSM0_DW.chartAbsoluteTimeCounter;
  }

  FSM0_DW.condWasTrueAtLastTimeStep_3 = flag;
  flag = (FSM0_U.buttons[c_previousEvent] == Pressed);
  if ((!flag) || (!FSM0_DW.condWasTrueAtLastTimeStep_1_a)) {
    FSM0_DW.durationLastReferenceTick_1_m = FSM0_DW.chartAbsoluteTimeCounter;
  }

  FSM0_DW.condWasTrueAtLastTimeStep_1_a = flag;
  flag = (FSM0_U.buttons[c_previousEvent] == Pressed);
  if ((!flag) || (!FSM0_DW.condWasTrueAtLastTimeStep_1_h)) {
    FSM0_DW.durationLastReferenceTick_1_l = FSM0_DW.chartAbsoluteTimeCounter;
  }

  FSM0_DW.condWasTrueAtLastTimeStep_1_h = flag;
  flag_tmp_0 = static_cast<int32_T>(VCV);
  flag = (FSM0_U.buttons[flag_tmp_0] == Pressed);
  if ((!flag) || (!FSM0_DW.condWasTrueAtLastTimeStep_4)) {
    FSM0_DW.durationLastReferenceTick_4 = FSM0_DW.chartAbsoluteTimeCounter;
  }

  FSM0_DW.condWasTrueAtLastTimeStep_4 = flag;
  flag_tmp_1 = static_cast<int32_T>(PRVC);
  flag = (FSM0_U.buttons[flag_tmp_1] == Pressed);
  if ((!flag) || (!FSM0_DW.condWasTrueAtLastTimeStep_5)) {
    FSM0_DW.durationLastReferenceTick_5 = FSM0_DW.chartAbsoluteTimeCounter;
  }

  FSM0_DW.condWasTrueAtLastTimeStep_5 = flag;
  flag = (FSM0_U.MultiSelector_Go == Pressed);
  if ((!flag) || (!FSM0_DW.condWasTrueAtLastTimeStep_6)) {
    FSM0_DW.durationLastReferenceTick_6 = FSM0_DW.chartAbsoluteTimeCounter;
  }

  FSM0_DW.condWasTrueAtLastTimeStep_6 = flag;
  flag = (FSM0_U.buttons[c_previousEvent] == Pressed);
  if ((!flag) || (!FSM0_DW.condWasTrueAtLastTimeStep_1_gp)) {
    FSM0_DW.durationLastReferenceTick_1_g = FSM0_DW.chartAbsoluteTimeCounter;
  }

  FSM0_DW.condWasTrueAtLastTimeStep_1_gp = flag;
  flag = (FSM0_U.MultiSelector_Go == Pressed);
  if ((!flag) || (!FSM0_DW.condWasTrueAtLastTimeStep_2_k)) {
    FSM0_DW.durationLastReferenceTick_2_h = FSM0_DW.chartAbsoluteTimeCounter;
  }

  FSM0_DW.condWasTrueAtLastTimeStep_2_k = flag;
  flag = (FSM0_U.buttons[c_previousEvent] == Released);
  if ((!flag) || (!FSM0_DW.condWasTrueAtLastTimeStep_3_n)) {
    FSM0_DW.durationLastReferenceTick_3_j = FSM0_DW.chartAbsoluteTimeCounter;
  }

  FSM0_DW.condWasTrueAtLastTimeStep_3_n = flag;
  flag = (FSM0_U.buttons[flag_tmp] == Pressed);
  if ((!flag) || (!FSM0_DW.condWasTrueAtLastTimeStep_4_k)) {
    FSM0_DW.durationLastReferenceTick_4_o = FSM0_DW.chartAbsoluteTimeCounter;
  }

  FSM0_DW.condWasTrueAtLastTimeStep_4_k = flag;
  flag = (FSM0_U.buttons[c_previousEvent] == Pressed);
  if ((!flag) || (!FSM0_DW.condWasTrueAtLastTimeStep_1_d)) {
    FSM0_DW.durationLastReferenceTick_1_j = FSM0_DW.chartAbsoluteTimeCounter;
  }

  FSM0_DW.condWasTrueAtLastTimeStep_1_d = flag;
  flag = (FSM0_U.buttons[flag_tmp_0] == Pressed);
  if ((!flag) || (!FSM0_DW.condWasTrueAtLastTimeStep_5_p)) {
    FSM0_DW.durationLastReferenceTick_5_d = FSM0_DW.chartAbsoluteTimeCounter;
  }

  FSM0_DW.condWasTrueAtLastTimeStep_5_p = flag;
  flag = (FSM0_U.buttons[c_previousEvent] == Pressed);
  if ((!flag) || (!FSM0_DW.condWasTrueAtLastTimeStep_1_c)) {
    FSM0_DW.durationLastReferenceTick_1_lo = FSM0_DW.chartAbsoluteTimeCounter;
  }

  FSM0_DW.condWasTrueAtLastTimeStep_1_c = flag;
  if (FSM0_DW.temporalCounter_i1 < MAX_uint32_T) {
    FSM0_DW.temporalCounter_i1++;
  }

  if (FSM0_DW.temporalCounter_i2 < MAX_uint32_T) {
    FSM0_DW.temporalCounter_i2++;
  }

  FSM0_DW.sfEvent = FSM0_CALL_EVENT;
  if (FSM0_DW.is_active_c3_FSM0 == 0U) {
    FSM0_DW.chartAbsoluteTimeCounter = 0;
    FSM0_DW.is_active_c3_FSM0 = 1U;
    FSM0_DW.is_active_STATES_HANDLING = 1U;
    FSM0_DW.enter_test = false;
    FSM0_DW.exit_test = false;
    FSM0_DW.durationLastReferenceTick_6 = FSM0_DW.chartAbsoluteTimeCounter;
    FSM0_DW.durationLastReferenceTick_5 = FSM0_DW.chartAbsoluteTimeCounter;
    FSM0_DW.durationLastReferenceTick_4 = FSM0_DW.chartAbsoluteTimeCounter;
    FSM0_DW.durationLastReferenceTick_3 = FSM0_DW.chartAbsoluteTimeCounter;
    FSM0_DW.durationLastReferenceTick_2 = FSM0_DW.chartAbsoluteTimeCounter;
    FSM0_DW.durationLastReferenceTick_1_o = FSM0_DW.chartAbsoluteTimeCounter;
    FSM0_DW.is_STATES_HANDLING = FSM0_IN_IDLE;
    FSM0_Y.ledsSTATES[c_previousEvent] = On;
    FSM0_Y.ledsSTATES[flag_tmp] = Off;
    FSM0_Y.ledsSTATES[flag_tmp_0] = Off;
    FSM0_Y.ledsSTATES[flag_tmp_1] = Off;

    // Outport: '<Root>/Controller_Mode' incorporates:
    //   Outport: '<Root>/ledsSTATES'

    FSM0_Y.Controller_Mode = IDLE;
    FSM0_DW.condWasTrueAtLastTimeStep_1_g = (FSM0_U.buttons[c_previousEvent] ==
      Pressed);
    FSM0_DW.condWasTrueAtLastTimeStep_2 = (FSM0_U.buttons[c_previousEvent] ==
      Released);
    FSM0_DW.condWasTrueAtLastTimeStep_3 = (FSM0_U.buttons[flag_tmp] == Pressed);
    FSM0_DW.condWasTrueAtLastTimeStep_4 = (FSM0_U.buttons[flag_tmp_0] == Pressed);
    FSM0_DW.condWasTrueAtLastTimeStep_5 = (FSM0_U.buttons[flag_tmp_1] == Pressed);
    FSM0_DW.condWasTrueAtLastTimeStep_6 = (FSM0_U.MultiSelector_Go == Pressed);
    FSM0_DW.is_active_ALARMS_HANDLING = 1U;
    FSM0_DW.is_ALARMS_HANDLING = FSM0_IN_NO_ALARMS;

    // Outport: '<Root>/idALARM'
    FSM0_Y.idALARM = ALARM_None;

    // Outport: '<Root>/ledALARM'
    FSM0_Y.ledALARM = Off;

    // Outport: '<Root>/soundALARM'
    FSM0_Y.soundALARM = false;
    FSM0_DW.is_active_IDLE_BLINKING = 1U;
    FSM0_DW.is_IDLE_BLINKING = FSM0_IN_INIT;
    FSM0_Y.ledsSTATES[c_previousEvent] = Off;
  } else {
    if (FSM0_DW.is_active_STATES_HANDLING != 0U) {
      FSM0_STATES_HANDLING();
    }

    if (FSM0_DW.is_active_ALARMS_HANDLING != 0U) {
      switch (FSM0_DW.is_ALARMS_HANDLING) {
       case FSM0_IN_DEFINITIVE_ALARM:
        // Outport: '<Root>/ledALARM'
        FSM0_Y.ledALARM = On;

        // Outport: '<Root>/soundALARM'
        FSM0_Y.soundALARM = true;
        break;

       case FSM0_IN_INLET_OVERPRESSURE:
        // Outport: '<Root>/ledALARM'
        FSM0_Y.ledALARM = On;

        // Outport: '<Root>/soundALARM'
        FSM0_Y.soundALARM = true;
        if (FSM0_check_definitive_alarms() != 0.0) {
          FSM0_DW.is_ALARMS_HANDLING = FSM0_IN_DEFINITIVE_ALARM;

          // Outport: '<Root>/ledALARM'
          FSM0_Y.ledALARM = On;

          // Outport: '<Root>/soundALARM'
          FSM0_Y.soundALARM = true;
          c_previousEvent = FSM0_DW.sfEvent;
          FSM0_DW.sfEvent = FSM0_event_switch_off;
          if (FSM0_DW.is_active_STATES_HANDLING != 0U) {
            FSM0_STATES_HANDLING();
          }

          FSM0_DW.sfEvent = c_previousEvent;
        } else {
          if ((FSM0_U.Controller_Signals[0] < FSM0_DW.alarms_S1S3_reinstate) &&
              (FSM0_U.Controller_Signals[2] < FSM0_DW.alarms_S1S3_reinstate)) {
            FSM0_DW.is_ALARMS_HANDLING = FSM0_IN_NO_ALARMS;

            // Outport: '<Root>/idALARM'
            FSM0_Y.idALARM = ALARM_None;

            // Outport: '<Root>/ledALARM'
            FSM0_Y.ledALARM = Off;

            // Outport: '<Root>/soundALARM'
            FSM0_Y.soundALARM = false;
          }
        }
        break;

       case FSM0_IN_NO_ALARMS:
        // Outport: '<Root>/ledALARM'
        FSM0_Y.ledALARM = Off;

        // Outport: '<Root>/soundALARM'
        FSM0_Y.soundALARM = false;
        if (FSM0_check_definitive_alarms() != 0.0) {
          FSM0_DW.is_ALARMS_HANDLING = FSM0_IN_DEFINITIVE_ALARM;

          // Outport: '<Root>/ledALARM'
          FSM0_Y.ledALARM = On;

          // Outport: '<Root>/soundALARM'
          FSM0_Y.soundALARM = true;
          c_previousEvent = FSM0_DW.sfEvent;
          FSM0_DW.sfEvent = FSM0_event_switch_off;
          if (FSM0_DW.is_active_STATES_HANDLING != 0U) {
            FSM0_STATES_HANDLING();
          }

          FSM0_DW.sfEvent = c_previousEvent;
        } else if (FSM0_check_inlet_overpressure() != 0.0) {
          FSM0_DW.resume_mode = FSM0_Y.Controller_Mode;
          c_previousEvent = FSM0_DW.sfEvent;
          FSM0_DW.sfEvent = FSM0_event_idle_controller;
          if (FSM0_DW.is_active_STATES_HANDLING != 0U) {
            FSM0_STATES_HANDLING();
          }

          FSM0_DW.sfEvent = c_previousEvent;
          FSM0_DW.is_ALARMS_HANDLING = FSM0_IN_INLET_OVERPRESSURE;

          // Outport: '<Root>/ledALARM' incorporates:
          //   Outport: '<Root>/Controller_Mode'

          FSM0_Y.ledALARM = On;

          // Outport: '<Root>/soundALARM'
          FSM0_Y.soundALARM = true;
        } else {
          if (FSM0_check_transitive_alarms() != 0.0) {
            FSM0_DW.is_ALARMS_HANDLING = FSM0_IN_TRANSITIVE_ALARMS;

            // Outport: '<Root>/ledALARM'
            FSM0_Y.ledALARM = On;

            // Outport: '<Root>/soundALARM'
            FSM0_Y.soundALARM = true;
          }
        }
        break;

       case FSM0_IN_TRANSITIVE_ALARMS:
        // Outport: '<Root>/ledALARM'
        FSM0_Y.ledALARM = On;

        // Outport: '<Root>/soundALARM'
        FSM0_Y.soundALARM = true;
        if (FSM0_check_definitive_alarms() != 0.0) {
          FSM0_DW.is_ALARMS_HANDLING = FSM0_IN_DEFINITIVE_ALARM;

          // Outport: '<Root>/ledALARM'
          FSM0_Y.ledALARM = On;

          // Outport: '<Root>/soundALARM'
          FSM0_Y.soundALARM = true;
          c_previousEvent = FSM0_DW.sfEvent;
          FSM0_DW.sfEvent = FSM0_event_switch_off;
          if (FSM0_DW.is_active_STATES_HANDLING != 0U) {
            FSM0_STATES_HANDLING();
          }

          FSM0_DW.sfEvent = c_previousEvent;
        } else {
          if (!(FSM0_check_transitive_alarms() != 0.0)) {
            FSM0_DW.is_ALARMS_HANDLING = FSM0_IN_NO_ALARMS;

            // Outport: '<Root>/idALARM'
            FSM0_Y.idALARM = ALARM_None;

            // Outport: '<Root>/ledALARM'
            FSM0_Y.ledALARM = Off;

            // Outport: '<Root>/soundALARM'
            FSM0_Y.soundALARM = false;
          }
        }
        break;
      }
    }

    if (FSM0_DW.is_active_IDLE_BLINKING != 0U) {
      FSM0_IDLE_BLINKING();
    }
  }

  // End of Chart: '<S1>/FSM'

  // ZeroOrderHold: '<S1>/Zero-Order Hold'
  if ((&FSM0_M)->Timing.TaskCounters.TID[1] == 0) {
    // MATLABSystem: '<S1>/Median Filter' incorporates:
    //   Inport: '<Root>/knobs_in'

    obj = &FSM0_DW.obj;
    obj_0 = &FSM0_DW.obj.pMID;
    if (obj->pMID.isInitialized != 1) {
      obj->pMID.isSetupComplete = false;
      obj->pMID.isInitialized = 1;
      obj->pMID.isSetupComplete = true;
      FSM0_MedianFilterCG_resetImpl(&obj->pMID);
    }

    vprev = obj_0->pBuf[static_cast<int32_T>(obj_0->pIdx[0]) - 1];
    obj_0->pBuf[static_cast<int32_T>(obj_0->pIdx[0]) - 1] = FSM0_U.knobs_in[0];
    p = obj_0->pPos[static_cast<int32_T>(obj_0->pIdx[0]) - 1];
    obj_0->pIdx[0]++;
    if (obj_0->pWinLen + 1.0F == obj_0->pIdx[0]) {
      obj_0->pIdx[0] = 1.0F;
    }

    if (p > obj_0->pMidHeap) {
      if (vprev < FSM0_U.knobs_in[0]) {
        vprev = p - obj_0->pMidHeap;
        F_MedianFilterCG_trickleDownMin(obj_0, vprev * 2.0F, 1);
      } else {
        i = p - obj_0->pMidHeap;
        exitg1 = false;
        while ((!exitg1) && (i > 0.0F)) {
          vprev = std::floor(i / 2.0F);
          flag = (obj_0->pBuf[static_cast<int32_T>(obj_0->pHeap
                   [static_cast<int32_T>(i + obj_0->pMidHeap) - 1]) - 1] <
                  obj_0->pBuf[static_cast<int32_T>(obj_0->pHeap
                   [static_cast<int32_T>(vprev + obj_0->pMidHeap) - 1]) - 1]);
          if (!flag) {
            exitg1 = true;
          } else {
            p = i + obj_0->pMidHeap;
            i = std::floor(i / 2.0F) + obj_0->pMidHeap;
            c_previousEvent = static_cast<int32_T>(p) - 1;
            temp = obj_0->pHeap[c_previousEvent];
            flag_tmp = static_cast<int32_T>(i) - 1;
            obj_0->pHeap[c_previousEvent] = obj_0->pHeap[flag_tmp];
            obj_0->pHeap[flag_tmp] = temp;
            obj_0->pPos[static_cast<int32_T>(obj_0->pHeap[static_cast<int32_T>(p)
              - 1]) - 1] = p;
            obj_0->pPos[static_cast<int32_T>(obj_0->pHeap[flag_tmp]) - 1] = i;
            i = vprev;
          }
        }

        if (i == 0.0F) {
          F_MedianFilterCG_trickleDownMax(obj_0, -1.0F, 1);
        }
      }
    } else if (p < obj_0->pMidHeap) {
      if (FSM0_U.knobs_in[0] < vprev) {
        vprev = p - obj_0->pMidHeap;
        F_MedianFilterCG_trickleDownMax(obj_0, vprev * 2.0F, 1);
      } else {
        i = p - obj_0->pMidHeap;
        exitg1 = false;
        while ((!exitg1) && (i < 0.0F)) {
          p = i / 2.0F;
          if (p < 0.0F) {
            vprev = std::ceil(p);
          } else {
            vprev = -0.0F;
          }

          flag = (obj_0->pBuf[static_cast<int32_T>(obj_0->pHeap
                   [static_cast<int32_T>(vprev + obj_0->pMidHeap) - 1]) - 1] <
                  obj_0->pBuf[static_cast<int32_T>(obj_0->pHeap
                   [static_cast<int32_T>(i + obj_0->pMidHeap) - 1]) - 1]);
          if (!flag) {
            exitg1 = true;
          } else {
            p = i / 2.0F;
            if (p < 0.0F) {
              p = std::ceil(p);
            } else {
              p = -0.0F;
            }

            p += obj_0->pMidHeap;
            i += obj_0->pMidHeap;
            c_previousEvent = static_cast<int32_T>(p) - 1;
            temp = obj_0->pHeap[c_previousEvent];
            obj_0->pHeap[c_previousEvent] = obj_0->pHeap[static_cast<int32_T>(i)
              - 1];
            flag_tmp = static_cast<int32_T>(i) - 1;
            obj_0->pHeap[flag_tmp] = temp;
            obj_0->pPos[static_cast<int32_T>(obj_0->pHeap[c_previousEvent]) - 1]
              = p;
            obj_0->pPos[static_cast<int32_T>(obj_0->pHeap[flag_tmp]) - 1] = i;
            i = vprev;
          }
        }

        if (i == 0.0F) {
          F_MedianFilterCG_trickleDownMin(obj_0, 1.0F, 1);
        }
      }
    } else {
      if (obj_0->pMaxHeapLength != 0.0F) {
        F_MedianFilterCG_trickleDownMax(obj_0, -1.0F, 1);
      }

      if (obj_0->pMinHeapLength > 0.0F) {
        F_MedianFilterCG_trickleDownMin(obj_0, 1.0F, 1);
      }
    }

    vprev = obj_0->pBuf[static_cast<int32_T>(obj_0->pHeap[static_cast<int32_T>
      (obj_0->pMidHeap) - 1]) - 1];
    vprev += obj_0->pBuf[static_cast<int32_T>(obj_0->pHeap[static_cast<int32_T>
      (obj_0->pMidHeap - 1.0F) - 1]) - 1];
    rtb_MedianFilter[0] = vprev / 2.0F;
    vprev = obj_0->pBuf[static_cast<int32_T>(obj_0->pIdx[1]) + 15];
    obj_0->pBuf[static_cast<int32_T>(obj_0->pIdx[1]) + 15] = FSM0_U.knobs_in[1];
    p = obj_0->pPos[static_cast<int32_T>(obj_0->pIdx[1]) + 15];
    obj_0->pIdx[1]++;
    if (obj_0->pWinLen + 1.0F == obj_0->pIdx[1]) {
      obj_0->pIdx[1] = 1.0F;
    }

    if (p > obj_0->pMidHeap) {
      if (vprev < FSM0_U.knobs_in[1]) {
        vprev = p - obj_0->pMidHeap;
        F_MedianFilterCG_trickleDownMin(obj_0, vprev * 2.0F, 2);
      } else {
        i = p - obj_0->pMidHeap;
        exitg1 = false;
        while ((!exitg1) && (i > 0.0F)) {
          vprev = std::floor(i / 2.0F);
          flag = (obj_0->pBuf[static_cast<int32_T>(obj_0->pHeap
                   [static_cast<int32_T>(i + obj_0->pMidHeap) + 15]) + 15] <
                  obj_0->pBuf[static_cast<int32_T>(obj_0->pHeap
                   [static_cast<int32_T>(vprev + obj_0->pMidHeap) + 15]) + 15]);
          if (!flag) {
            exitg1 = true;
          } else {
            p = i + obj_0->pMidHeap;
            i = std::floor(i / 2.0F) + obj_0->pMidHeap;
            c_previousEvent = static_cast<int32_T>(p) + 15;
            temp = obj_0->pHeap[c_previousEvent];
            flag_tmp = static_cast<int32_T>(i) + 15;
            obj_0->pHeap[c_previousEvent] = obj_0->pHeap[flag_tmp];
            obj_0->pHeap[flag_tmp] = temp;
            obj_0->pPos[static_cast<int32_T>(obj_0->pHeap[static_cast<int32_T>(p)
              + 15]) + 15] = p;
            obj_0->pPos[static_cast<int32_T>(obj_0->pHeap[flag_tmp]) + 15] = i;
            i = vprev;
          }
        }

        if (i == 0.0F) {
          F_MedianFilterCG_trickleDownMax(obj_0, -1.0F, 2);
        }
      }
    } else if (p < obj_0->pMidHeap) {
      if (FSM0_U.knobs_in[1] < vprev) {
        vprev = p - obj_0->pMidHeap;
        F_MedianFilterCG_trickleDownMax(obj_0, vprev * 2.0F, 2);
      } else {
        i = p - obj_0->pMidHeap;
        exitg1 = false;
        while ((!exitg1) && (i < 0.0F)) {
          p = i / 2.0F;
          if (p < 0.0F) {
            vprev = std::ceil(p);
          } else {
            vprev = -0.0F;
          }

          flag = (obj_0->pBuf[static_cast<int32_T>(obj_0->pHeap
                   [static_cast<int32_T>(vprev + obj_0->pMidHeap) + 15]) + 15] <
                  obj_0->pBuf[static_cast<int32_T>(obj_0->pHeap
                   [static_cast<int32_T>(i + obj_0->pMidHeap) + 15]) + 15]);
          if (!flag) {
            exitg1 = true;
          } else {
            p = i / 2.0F;
            if (p < 0.0F) {
              p = std::ceil(p);
            } else {
              p = -0.0F;
            }

            p += obj_0->pMidHeap;
            i += obj_0->pMidHeap;
            c_previousEvent = static_cast<int32_T>(p) + 15;
            temp = obj_0->pHeap[c_previousEvent];
            obj_0->pHeap[c_previousEvent] = obj_0->pHeap[static_cast<int32_T>(i)
              + 15];
            flag_tmp = static_cast<int32_T>(i) + 15;
            obj_0->pHeap[flag_tmp] = temp;
            obj_0->pPos[static_cast<int32_T>(obj_0->pHeap[c_previousEvent]) + 15]
              = p;
            obj_0->pPos[static_cast<int32_T>(obj_0->pHeap[flag_tmp]) + 15] = i;
            i = vprev;
          }
        }

        if (i == 0.0F) {
          F_MedianFilterCG_trickleDownMin(obj_0, 1.0F, 2);
        }
      }
    } else {
      if (obj_0->pMaxHeapLength != 0.0F) {
        F_MedianFilterCG_trickleDownMax(obj_0, -1.0F, 2);
      }

      if (obj_0->pMinHeapLength > 0.0F) {
        F_MedianFilterCG_trickleDownMin(obj_0, 1.0F, 2);
      }
    }

    vprev = obj_0->pBuf[static_cast<int32_T>(obj_0->pHeap[static_cast<int32_T>
      (obj_0->pMidHeap) + 15]) + 15];
    vprev += obj_0->pBuf[static_cast<int32_T>(obj_0->pHeap[static_cast<int32_T>
      (obj_0->pMidHeap - 1.0F) + 15]) + 15];
    rtb_MedianFilter[1] = vprev / 2.0F;
    vprev = obj_0->pBuf[static_cast<int32_T>(obj_0->pIdx[2]) + 31];
    obj_0->pBuf[static_cast<int32_T>(obj_0->pIdx[2]) + 31] = FSM0_U.knobs_in[2];
    p = obj_0->pPos[static_cast<int32_T>(obj_0->pIdx[2]) + 31];
    obj_0->pIdx[2]++;
    if (obj_0->pWinLen + 1.0F == obj_0->pIdx[2]) {
      obj_0->pIdx[2] = 1.0F;
    }

    if (p > obj_0->pMidHeap) {
      if (vprev < FSM0_U.knobs_in[2]) {
        vprev = p - obj_0->pMidHeap;
        F_MedianFilterCG_trickleDownMin(obj_0, vprev * 2.0F, 3);
      } else {
        i = p - obj_0->pMidHeap;
        exitg1 = false;
        while ((!exitg1) && (i > 0.0F)) {
          vprev = std::floor(i / 2.0F);
          flag = (obj_0->pBuf[static_cast<int32_T>(obj_0->pHeap
                   [static_cast<int32_T>(i + obj_0->pMidHeap) + 31]) + 31] <
                  obj_0->pBuf[static_cast<int32_T>(obj_0->pHeap
                   [static_cast<int32_T>(vprev + obj_0->pMidHeap) + 31]) + 31]);
          if (!flag) {
            exitg1 = true;
          } else {
            p = i + obj_0->pMidHeap;
            i = std::floor(i / 2.0F) + obj_0->pMidHeap;
            c_previousEvent = static_cast<int32_T>(p) + 31;
            temp = obj_0->pHeap[c_previousEvent];
            flag_tmp = static_cast<int32_T>(i) + 31;
            obj_0->pHeap[c_previousEvent] = obj_0->pHeap[flag_tmp];
            obj_0->pHeap[flag_tmp] = temp;
            obj_0->pPos[static_cast<int32_T>(obj_0->pHeap[static_cast<int32_T>(p)
              + 31]) + 31] = p;
            obj_0->pPos[static_cast<int32_T>(obj_0->pHeap[flag_tmp]) + 31] = i;
            i = vprev;
          }
        }

        if (i == 0.0F) {
          F_MedianFilterCG_trickleDownMax(obj_0, -1.0F, 3);
        }
      }
    } else if (p < obj_0->pMidHeap) {
      if (FSM0_U.knobs_in[2] < vprev) {
        vprev = p - obj_0->pMidHeap;
        F_MedianFilterCG_trickleDownMax(obj_0, vprev * 2.0F, 3);
      } else {
        i = p - obj_0->pMidHeap;
        exitg1 = false;
        while ((!exitg1) && (i < 0.0F)) {
          p = i / 2.0F;
          if (p < 0.0F) {
            vprev = std::ceil(p);
          } else {
            vprev = -0.0F;
          }

          flag = (obj_0->pBuf[static_cast<int32_T>(obj_0->pHeap
                   [static_cast<int32_T>(vprev + obj_0->pMidHeap) + 31]) + 31] <
                  obj_0->pBuf[static_cast<int32_T>(obj_0->pHeap
                   [static_cast<int32_T>(i + obj_0->pMidHeap) + 31]) + 31]);
          if (!flag) {
            exitg1 = true;
          } else {
            p = i / 2.0F;
            if (p < 0.0F) {
              p = std::ceil(p);
            } else {
              p = -0.0F;
            }

            p += obj_0->pMidHeap;
            i += obj_0->pMidHeap;
            c_previousEvent = static_cast<int32_T>(p) + 31;
            temp = obj_0->pHeap[c_previousEvent];
            obj_0->pHeap[c_previousEvent] = obj_0->pHeap[static_cast<int32_T>(i)
              + 31];
            flag_tmp = static_cast<int32_T>(i) + 31;
            obj_0->pHeap[flag_tmp] = temp;
            obj_0->pPos[static_cast<int32_T>(obj_0->pHeap[c_previousEvent]) + 31]
              = p;
            obj_0->pPos[static_cast<int32_T>(obj_0->pHeap[flag_tmp]) + 31] = i;
            i = vprev;
          }
        }

        if (i == 0.0F) {
          F_MedianFilterCG_trickleDownMin(obj_0, 1.0F, 3);
        }
      }
    } else {
      if (obj_0->pMaxHeapLength != 0.0F) {
        F_MedianFilterCG_trickleDownMax(obj_0, -1.0F, 3);
      }

      if (obj_0->pMinHeapLength > 0.0F) {
        F_MedianFilterCG_trickleDownMin(obj_0, 1.0F, 3);
      }
    }

    vprev = obj_0->pBuf[static_cast<int32_T>(obj_0->pHeap[static_cast<int32_T>
      (obj_0->pMidHeap) + 31]) + 31];
    vprev += obj_0->pBuf[static_cast<int32_T>(obj_0->pHeap[static_cast<int32_T>
      (obj_0->pMidHeap - 1.0F) + 31]) + 31];
    rtb_MedianFilter[2] = vprev / 2.0F;
    vprev = obj_0->pBuf[static_cast<int32_T>(obj_0->pIdx[3]) + 47];
    obj_0->pBuf[static_cast<int32_T>(obj_0->pIdx[3]) + 47] = FSM0_U.knobs_in[3];
    p = obj_0->pPos[static_cast<int32_T>(obj_0->pIdx[3]) + 47];
    obj_0->pIdx[3]++;
    if (obj_0->pWinLen + 1.0F == obj_0->pIdx[3]) {
      obj_0->pIdx[3] = 1.0F;
    }

    if (p > obj_0->pMidHeap) {
      if (vprev < FSM0_U.knobs_in[3]) {
        vprev = p - obj_0->pMidHeap;
        F_MedianFilterCG_trickleDownMin(obj_0, vprev * 2.0F, 4);
      } else {
        i = p - obj_0->pMidHeap;
        exitg1 = false;
        while ((!exitg1) && (i > 0.0F)) {
          vprev = std::floor(i / 2.0F);
          flag = (obj_0->pBuf[static_cast<int32_T>(obj_0->pHeap
                   [static_cast<int32_T>(i + obj_0->pMidHeap) + 47]) + 47] <
                  obj_0->pBuf[static_cast<int32_T>(obj_0->pHeap
                   [static_cast<int32_T>(vprev + obj_0->pMidHeap) + 47]) + 47]);
          if (!flag) {
            exitg1 = true;
          } else {
            p = i + obj_0->pMidHeap;
            i = std::floor(i / 2.0F) + obj_0->pMidHeap;
            c_previousEvent = static_cast<int32_T>(p) + 47;
            temp = obj_0->pHeap[c_previousEvent];
            flag_tmp = static_cast<int32_T>(i) + 47;
            obj_0->pHeap[c_previousEvent] = obj_0->pHeap[flag_tmp];
            obj_0->pHeap[flag_tmp] = temp;
            obj_0->pPos[static_cast<int32_T>(obj_0->pHeap[c_previousEvent]) + 47]
              = p;
            obj_0->pPos[static_cast<int32_T>(obj_0->pHeap[flag_tmp]) + 47] = i;
            i = vprev;
          }
        }

        if (i == 0.0F) {
          F_MedianFilterCG_trickleDownMax(obj_0, -1.0F, 4);
        }
      }
    } else if (p < obj_0->pMidHeap) {
      if (FSM0_U.knobs_in[3] < vprev) {
        vprev = p - obj_0->pMidHeap;
        F_MedianFilterCG_trickleDownMax(obj_0, vprev * 2.0F, 4);
      } else {
        i = p - obj_0->pMidHeap;
        exitg1 = false;
        while ((!exitg1) && (i < 0.0F)) {
          p = i / 2.0F;
          if (p < 0.0F) {
            vprev = std::ceil(p);
          } else {
            vprev = -0.0F;
          }

          flag = (obj_0->pBuf[static_cast<int32_T>(obj_0->pHeap
                   [static_cast<int32_T>(vprev + obj_0->pMidHeap) + 47]) + 47] <
                  obj_0->pBuf[static_cast<int32_T>(obj_0->pHeap
                   [static_cast<int32_T>(i + obj_0->pMidHeap) + 47]) + 47]);
          if (!flag) {
            exitg1 = true;
          } else {
            p = i / 2.0F;
            if (p < 0.0F) {
              p = std::ceil(p);
            } else {
              p = -0.0F;
            }

            p += obj_0->pMidHeap;
            i += obj_0->pMidHeap;
            c_previousEvent = static_cast<int32_T>(p) + 47;
            temp = obj_0->pHeap[c_previousEvent];
            flag_tmp = static_cast<int32_T>(i) + 47;
            obj_0->pHeap[c_previousEvent] = obj_0->pHeap[flag_tmp];
            obj_0->pHeap[flag_tmp] = temp;
            obj_0->pPos[static_cast<int32_T>(obj_0->pHeap[c_previousEvent]) + 47]
              = p;
            obj_0->pPos[static_cast<int32_T>(obj_0->pHeap[flag_tmp]) + 47] = i;
            i = vprev;
          }
        }

        if (i == 0.0F) {
          F_MedianFilterCG_trickleDownMin(obj_0, 1.0F, 4);
        }
      }
    } else {
      if (obj_0->pMaxHeapLength != 0.0F) {
        F_MedianFilterCG_trickleDownMax(obj_0, -1.0F, 4);
      }

      if (obj_0->pMinHeapLength > 0.0F) {
        F_MedianFilterCG_trickleDownMin(obj_0, 1.0F, 4);
      }
    }

    vprev = obj_0->pBuf[static_cast<int32_T>(obj_0->pHeap[static_cast<int32_T>
      (obj_0->pMidHeap) + 47]) + 47];
    vprev += obj_0->pBuf[static_cast<int32_T>(obj_0->pHeap[static_cast<int32_T>
      (obj_0->pMidHeap - 1.0F) + 47]) + 47];
    rtb_MedianFilter[3] = vprev / 2.0F;

    // Update for RateTransition: '<S1>/Rate Transition' incorporates:
    //   MATLABSystem: '<S1>/Median Filter'

    FSM0_DW.RateTransition_Buffer0[0] = rtb_MedianFilter[0];
    FSM0_DW.RateTransition_Buffer0[1] = rtb_MedianFilter[1];
    FSM0_DW.RateTransition_Buffer0[2] = rtb_MedianFilter[2];
    FSM0_DW.RateTransition_Buffer0[3] = rtb_MedianFilter[3];
  }

  // End of ZeroOrderHold: '<S1>/Zero-Order Hold'
  // End of Outputs for SubSystem: '<Root>/FSM'
  rate_scheduler((&FSM0_M));
}

// Model initialize function
void FSM0ModelClass::initialize()
{
  // Registration code

  // initialize non-finites
  rt_InitInfAndNaN(sizeof(real_T));

  {
    dsp_MedianFilter_FSM0_T *obj;

    // SystemInitialize for Atomic SubSystem: '<Root>/FSM'
    // SystemInitialize for Chart: '<S1>/FSM'
    FSM0_DW.sfEvent = FSM0_CALL_EVENT;
    FSM0_DW.MultiSelector_Maps_latch = 1U;
    FSM0_DW.alarms_S7_max = 1500.0F;
    FSM0_DW.alarms_S1S3_max = 100.0F;
    FSM0_DW.alarms_PEEP = -10.0F;
    FSM0_DW.resume_mode = IDLE;
    FSM0_DW.alarms_TD_min = -0.1F;
    FSM0_DW.alarms_S1S3_reinstate = 20.0F;

    // Start for RateTransition: '<S1>/Rate Transition'
    FSM0_B.RateTransition[0] = FSM0_P.RateTransition_InitialCondition;

    // InitializeConditions for RateTransition: '<S1>/Rate Transition'
    FSM0_DW.RateTransition_Buffer0[0] = FSM0_P.RateTransition_InitialCondition;

    // InitializeConditions for Outport: '<Root>/ledsSTATES' incorporates:
    //   Chart: '<S1>/FSM'

    FSM0_Y.ledsSTATES[0] = Off;

    // Start for RateTransition: '<S1>/Rate Transition'
    FSM0_B.RateTransition[1] = FSM0_P.RateTransition_InitialCondition;

    // InitializeConditions for RateTransition: '<S1>/Rate Transition'
    FSM0_DW.RateTransition_Buffer0[1] = FSM0_P.RateTransition_InitialCondition;

    // InitializeConditions for Outport: '<Root>/ledsSTATES' incorporates:
    //   Chart: '<S1>/FSM'

    FSM0_Y.ledsSTATES[1] = Off;

    // Start for RateTransition: '<S1>/Rate Transition'
    FSM0_B.RateTransition[2] = FSM0_P.RateTransition_InitialCondition;

    // InitializeConditions for RateTransition: '<S1>/Rate Transition'
    FSM0_DW.RateTransition_Buffer0[2] = FSM0_P.RateTransition_InitialCondition;

    // InitializeConditions for Outport: '<Root>/ledsSTATES' incorporates:
    //   Chart: '<S1>/FSM'

    FSM0_Y.ledsSTATES[2] = Off;

    // Start for RateTransition: '<S1>/Rate Transition'
    FSM0_B.RateTransition[3] = FSM0_P.RateTransition_InitialCondition;

    // InitializeConditions for RateTransition: '<S1>/Rate Transition'
    FSM0_DW.RateTransition_Buffer0[3] = FSM0_P.RateTransition_InitialCondition;

    // InitializeConditions for Outport: '<Root>/ledsSTATES' incorporates:
    //   Chart: '<S1>/FSM'

    FSM0_Y.ledsSTATES[3] = Off;

    // InitializeConditions for Outport: '<Root>/Controller_Mode' incorporates:
    //   Chart: '<S1>/FSM'

    FSM0_Y.Controller_Mode = IDLE;

    // InitializeConditions for Outport: '<Root>/ledALARM' incorporates:
    //   Chart: '<S1>/FSM'

    FSM0_Y.ledALARM = Off;

    // InitializeConditions for Outport: '<Root>/knobs_transfer' incorporates:
    //   Chart: '<S1>/FSM'

    FSM0_Y.knobs_transfer = MAP_Transfer_None;

    // InitializeConditions for Outport: '<Root>/idALARM' incorporates:
    //   Chart: '<S1>/FSM'

    FSM0_Y.idALARM = ALARM_None;

    // Start for MATLABSystem: '<S1>/Median Filter'
    FSM0_DW.obj.matlabCodegenIsDeleted = true;
    FSM0_DW.obj.isInitialized = 0;
    FSM0_DW.obj.NumChannels = -1;
    FSM0_DW.obj.matlabCodegenIsDeleted = false;
    obj = &FSM0_DW.obj;
    FSM0_DW.obj.isSetupComplete = false;
    FSM0_DW.obj.isInitialized = 1;
    FSM0_DW.obj.NumChannels = 4;
    obj->pMID.isInitialized = 0;
    FSM0_DW.obj.isSetupComplete = true;

    // InitializeConditions for MATLABSystem: '<S1>/Median Filter'
    if (FSM0_DW.obj.pMID.isInitialized == 1) {
      FSM0_MedianFilterCG_resetImpl(&FSM0_DW.obj.pMID);
    }

    // End of InitializeConditions for MATLABSystem: '<S1>/Median Filter'
    // End of SystemInitialize for SubSystem: '<Root>/FSM'
  }
}

// Model terminate function
void FSM0ModelClass::terminate()
{
  // Terminate for Atomic SubSystem: '<Root>/FSM'
  // Terminate for MATLABSystem: '<S1>/Median Filter'
  matlabCodegenHandle_matlabCodeg(&FSM0_DW.obj);

  // End of Terminate for SubSystem: '<Root>/FSM'
}

// Constructor
FSM0ModelClass::FSM0ModelClass():
  FSM0_B()
  ,FSM0_DW()
  ,FSM0_U()
  ,FSM0_Y()
  ,FSM0_M()
{
  // Currently there is no constructor body generated.
}

// Destructor
FSM0ModelClass::~FSM0ModelClass()
{
  // Currently there is no destructor body generated.
}

// Block parameters get method
const FSM0ModelClass::P_FSM0_T & FSM0ModelClass::getBlockParameters() const
{
  return FSM0_P;
}

// Block parameters set method
void FSM0ModelClass::setBlockParameters(const P_FSM0_T *pFSM0_P)
{
  FSM0_P = *pFSM0_P;
}

// Real-Time Model get method
FSM0ModelClass::RT_MODEL_FSM0_T * FSM0ModelClass::getRTM()
{
  return (&FSM0_M);
}

//
// File trailer for generated code.
//
// [EOF]
//
