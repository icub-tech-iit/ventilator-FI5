//
// Non-Degree Granting Education License -- for use at non-degree
// granting, nonprofit, educational organizations only. Not for
// government, commercial, or other organizational use.
//
// File: FSM0.cpp
//
// Code generated for Simulink model 'FSM0'.
//
// Model version                  : 1.838
// Simulink Coder version         : 9.3 (R2020a) 18-Nov-2019
// C/C++ source code generated on : Tue May 12 12:10:29 2020
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
const uint8_T FSM0_IN_NO_ACTIVE_CHILD = 0U;
const uint8_T FSM0_IN_NO_ALARMS = 2U;
const uint8_T FSM0_IN_OFF = 3U;
const uint8_T FSM0_IN_OFF_c = 2U;
const uint8_T FSM0_IN_ON = 3U;
const uint8_T FSM0_IN_PRVC = 4U;
const uint8_T FSM0_IN_TEST_1 = 6U;
const uint8_T FSM0_IN_TEST_2 = 7U;
const uint8_T FSM0_IN_TRANSITIVE_ALARMS = 3U;
const uint8_T FSM0_IN_VCV = 8U;
const int32_T FSM0_event_blink_IDLE_off = 0;
const int32_T FSM0_event_blink_IDLE_on = 1;
const int32_T FSM0_event_idle_controller = 2;
const int32_T FSM0_event_switch_off = 3;

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
    FSM0_DW.durationLastReferenceTick_1_g = FSM0_DW.chartAbsoluteTimeCounter;
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
    FSM0_DW.condWasTrueAtLastTimeStep_1_e = (FSM0_U.buttons[ledsSTATES_tmp] ==
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
    if ((!condIsTrue) || (!FSM0_DW.condWasTrueAtLastTimeStep_1_p)) {
      FSM0_DW.durationLastReferenceTick_1_h = FSM0_DW.chartAbsoluteTimeCounter;
    }

    FSM0_DW.condWasTrueAtLastTimeStep_1_p = condIsTrue;
    if (FSM0_DW.chartAbsoluteTimeCounter - FSM0_DW.durationLastReferenceTick_1_h
        >= FSM0_P.FSM_debounce_tmo * 100.0F) {
      FSM0_DW.enter_test = false;
      FSM0_DW.durationLastReferenceTick_6 = FSM0_DW.chartAbsoluteTimeCounter;
      FSM0_DW.durationLastReferenceTick_5 = FSM0_DW.chartAbsoluteTimeCounter;
      FSM0_DW.durationLastReferenceTick_4 = FSM0_DW.chartAbsoluteTimeCounter;
      FSM0_DW.durationLastReferenceTick_3 = FSM0_DW.chartAbsoluteTimeCounter;
      FSM0_DW.durationLastReferenceTick_2 = FSM0_DW.chartAbsoluteTimeCounter;
      FSM0_DW.durationLastReferenceTick_1_g = FSM0_DW.chartAbsoluteTimeCounter;
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
      FSM0_DW.condWasTrueAtLastTimeStep_1_e = (FSM0_U.buttons[condIsTrue_tmp] ==
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

// Function for Chart: '<S1>/FSM'
void FSM0ModelClass::FSM0_apply_map(real_T n)
{
  int32_T knobs_out_tmp;
  real_T j_tmp;
  j_tmp = (n - 1.0) * 4.0;
  knobs_out_tmp = (static_cast<int32_T>(j_tmp + 1.0) - 1) << 1;

  // Outport: '<Root>/knobs_out' incorporates:
  //   Inport: '<Root>/knobs_in'

  FSM0_Y.knobs_out[0] = (FSM0_B.MatrixConcatenate[knobs_out_tmp + 1] -
    FSM0_B.MatrixConcatenate[knobs_out_tmp]) * FSM0_U.knobs_in[0] +
    FSM0_B.MatrixConcatenate[knobs_out_tmp];
  knobs_out_tmp = (static_cast<int32_T>(j_tmp + 2.0) - 1) << 1;

  // Outport: '<Root>/knobs_out' incorporates:
  //   Inport: '<Root>/knobs_in'

  FSM0_Y.knobs_out[1] = (FSM0_B.MatrixConcatenate[knobs_out_tmp + 1] -
    FSM0_B.MatrixConcatenate[knobs_out_tmp]) * FSM0_U.knobs_in[1] +
    FSM0_B.MatrixConcatenate[knobs_out_tmp];
  knobs_out_tmp = (static_cast<int32_T>(j_tmp + 3.0) - 1) << 1;

  // Outport: '<Root>/knobs_out' incorporates:
  //   Inport: '<Root>/knobs_in'

  FSM0_Y.knobs_out[2] = (FSM0_B.MatrixConcatenate[knobs_out_tmp + 1] -
    FSM0_B.MatrixConcatenate[knobs_out_tmp]) * FSM0_U.knobs_in[2] +
    FSM0_B.MatrixConcatenate[knobs_out_tmp];
  knobs_out_tmp = (static_cast<int32_T>(j_tmp + 4.0) - 1) << 1;

  // Outport: '<Root>/knobs_out' incorporates:
  //   Inport: '<Root>/knobs_in'

  FSM0_Y.knobs_out[3] = (FSM0_B.MatrixConcatenate[knobs_out_tmp + 1] -
    FSM0_B.MatrixConcatenate[knobs_out_tmp]) * FSM0_U.knobs_in[3] +
    FSM0_B.MatrixConcatenate[knobs_out_tmp];
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
    FSM0_DW.alarms_PEEP = FSM0_Y.knobs_out[3];
    break;

   case MAP_ALARMS_2:
    FSM0_DW.alarms_TD_min = FSM0_Y.knobs_out[0];
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
        FSM0_DW.durationLastReferenceTick_1_h = FSM0_DW.chartAbsoluteTimeCounter;
        FSM0_DW.is_STATES_HANDLING = FSM0_IN_CPAP;

        // Outport: '<Root>/ledsSTATES'
        FSM0_Y.ledsSTATES[condIsTrue_tmp] = Off;
        FSM0_Y.ledsSTATES[condIsTrue_tmp_0] = On;

        // Outport: '<Root>/Controller_Mode'
        FSM0_Y.Controller_Mode = CPAP;
        FSM0_DW.resume_mode = IDLE;

        // Inport: '<Root>/buttons'
        FSM0_DW.condWasTrueAtLastTimeStep_1_p = (FSM0_U.buttons[condIsTrue_tmp] ==
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
          FSM0_DW.durationLastReferenceTick_1_k =
            FSM0_DW.chartAbsoluteTimeCounter;
          FSM0_DW.is_STATES_HANDLING = FSM0_IN_VCV;

          // Outport: '<Root>/ledsSTATES'
          FSM0_Y.ledsSTATES[condIsTrue_tmp] = Off;
          FSM0_Y.ledsSTATES[condIsTrue_tmp_1] = On;

          // Outport: '<Root>/Controller_Mode'
          FSM0_Y.Controller_Mode = VCV;
          FSM0_DW.resume_mode = IDLE;

          // Inport: '<Root>/buttons'
          FSM0_DW.condWasTrueAtLastTimeStep_1_h = (FSM0_U.buttons[condIsTrue_tmp]
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
            FSM0_DW.durationLastReferenceTick_1_i =
              FSM0_DW.chartAbsoluteTimeCounter;
            FSM0_DW.is_STATES_HANDLING = FSM0_IN_PRVC;

            // Outport: '<Root>/ledsSTATES'
            FSM0_Y.ledsSTATES[condIsTrue_tmp] = Off;
            FSM0_Y.ledsSTATES[condIsTrue_tmp_2] = On;

            // Outport: '<Root>/Controller_Mode'
            FSM0_Y.Controller_Mode = PRVC;
            FSM0_DW.resume_mode = IDLE;

            // Inport: '<Root>/buttons'
            FSM0_DW.condWasTrueAtLastTimeStep_1_g =
              (FSM0_U.buttons[condIsTrue_tmp] == Pressed);
          } else {
            // Inport: '<Root>/buttons'
            condIsTrue = (FSM0_U.buttons[condIsTrue_tmp] == Pressed);
            if ((!condIsTrue) || (!FSM0_DW.condWasTrueAtLastTimeStep_1_e)) {
              FSM0_DW.durationLastReferenceTick_1_g =
                FSM0_DW.chartAbsoluteTimeCounter;
            }

            FSM0_DW.condWasTrueAtLastTimeStep_1_e = condIsTrue;
            if (FSM0_DW.enter_test && (FSM0_DW.chartAbsoluteTimeCounter -
                 FSM0_DW.durationLastReferenceTick_1_g >= FSM0_P.FSM_test_tmo *
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
              FSM0_DW.durationLastReferenceTick_5_l =
                FSM0_DW.chartAbsoluteTimeCounter;
              FSM0_DW.durationLastReferenceTick_4_h =
                FSM0_DW.chartAbsoluteTimeCounter;
              FSM0_DW.durationLastReferenceTick_3_d =
                FSM0_DW.chartAbsoluteTimeCounter;
              FSM0_DW.durationLastReferenceTick_2_l =
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
              FSM0_DW.condWasTrueAtLastTimeStep_2_a = (FSM0_U.MultiSelector_Go ==
                Pressed);

              // Inport: '<Root>/buttons'
              FSM0_DW.condWasTrueAtLastTimeStep_3_a =
                (FSM0_U.buttons[condIsTrue_tmp] == Released);
              FSM0_DW.condWasTrueAtLastTimeStep_4_d =
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
      FSM0_DW.durationLastReferenceTick_1_g = FSM0_DW.chartAbsoluteTimeCounter;
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
      FSM0_DW.condWasTrueAtLastTimeStep_1_e = (FSM0_U.buttons[condIsTrue_tmp] ==
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
    FSM0_DW.durationLastReferenceTick_1_g = FSM0_DW.chartAbsoluteTimeCounter;
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
    FSM0_DW.condWasTrueAtLastTimeStep_1_e = (FSM0_U.buttons[ledsSTATES_tmp] ==
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
    if ((!condIsTrue) || (!FSM0_DW.condWasTrueAtLastTimeStep_1_g)) {
      FSM0_DW.durationLastReferenceTick_1_i = FSM0_DW.chartAbsoluteTimeCounter;
    }

    FSM0_DW.condWasTrueAtLastTimeStep_1_g = condIsTrue;
    if (FSM0_DW.chartAbsoluteTimeCounter - FSM0_DW.durationLastReferenceTick_1_i
        >= FSM0_P.FSM_debounce_tmo * 100.0F) {
      FSM0_DW.enter_test = false;
      FSM0_DW.durationLastReferenceTick_6 = FSM0_DW.chartAbsoluteTimeCounter;
      FSM0_DW.durationLastReferenceTick_5 = FSM0_DW.chartAbsoluteTimeCounter;
      FSM0_DW.durationLastReferenceTick_4 = FSM0_DW.chartAbsoluteTimeCounter;
      FSM0_DW.durationLastReferenceTick_3 = FSM0_DW.chartAbsoluteTimeCounter;
      FSM0_DW.durationLastReferenceTick_2 = FSM0_DW.chartAbsoluteTimeCounter;
      FSM0_DW.durationLastReferenceTick_1_g = FSM0_DW.chartAbsoluteTimeCounter;
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
      FSM0_DW.condWasTrueAtLastTimeStep_1_e = (FSM0_U.buttons[condIsTrue_tmp] ==
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
  if ((!condIsTrue) || (!FSM0_DW.condWasTrueAtLastTimeStep_4_d)) {
    FSM0_DW.durationLastReferenceTick_4_h = FSM0_DW.chartAbsoluteTimeCounter;
  }

  FSM0_DW.condWasTrueAtLastTimeStep_4_d = condIsTrue;
  if (FSM0_DW.chartAbsoluteTimeCounter - FSM0_DW.durationLastReferenceTick_4_h >=
      FSM0_P.FSM_debounce_tmo * 100.0F) {
    // Outport: '<Root>/knobs_transfer'
    FSM0_Y.knobs_transfer = MAP_Transfer_None;
    FSM0_DW.durationLastReferenceTick_1_b = FSM0_DW.chartAbsoluteTimeCounter;
    FSM0_DW.is_STATES_HANDLING = FSM0_IN_TEST_1;
    FSM0_DW.temporalCounter_i1 = 0U;

    // Outport: '<Root>/test_IV_dc'
    FSM0_Y.test_IV_dc = 0.0F;

    // Outport: '<Root>/test_OV_cmd'
    FSM0_Y.test_OV_cmd = true;

    // Outport: '<Root>/ledsSTATES'
    FSM0_Y.ledsSTATES[condIsTrue_tmp] = On;

    // Inport: '<Root>/buttons'
    FSM0_DW.condWasTrueAtLastTimeStep_1_o = (FSM0_U.buttons[static_cast<int32_T>
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
      FSM0_DW.durationLastReferenceTick_1_g = FSM0_DW.chartAbsoluteTimeCounter;
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
      FSM0_DW.condWasTrueAtLastTimeStep_1_e = (FSM0_U.buttons[condIsTrue_tmp_0] ==
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
        FSM0_DW.durationLastReferenceTick_5_l = FSM0_DW.chartAbsoluteTimeCounter;
      }

      FSM0_DW.condWasTrueAtLastTimeStep_5_p = condIsTrue;
      if (FSM0_DW.chartAbsoluteTimeCounter -
          FSM0_DW.durationLastReferenceTick_5_l >= FSM0_P.FSM_debounce_tmo *
          100.0F) {
        // Outport: '<Root>/knobs_transfer'
        FSM0_Y.knobs_transfer = MAP_Transfer_None;
        FSM0_DW.durationLastReferenceTick_1_n = FSM0_DW.chartAbsoluteTimeCounter;
        FSM0_DW.is_STATES_HANDLING = FSM0_IN_TEST_2;
        FSM0_DW.temporalCounter_i1 = 0U;

        // Outport: '<Root>/test_IV_dc'
        FSM0_Y.test_IV_dc = 100.0F;

        // Outport: '<Root>/ledsSTATES'
        FSM0_Y.ledsSTATES[condIsTrue_tmp_1] = On;

        // Inport: '<Root>/buttons'
        FSM0_DW.condWasTrueAtLastTimeStep_1_es =
          (FSM0_U.buttons[condIsTrue_tmp_0] == Pressed);
      } else {
        // Inport: '<Root>/buttons'
        condIsTrue = (FSM0_U.buttons[condIsTrue_tmp_0] == Released);
        if ((!condIsTrue) || (!FSM0_DW.condWasTrueAtLastTimeStep_3_a)) {
          FSM0_DW.durationLastReferenceTick_3_d =
            FSM0_DW.chartAbsoluteTimeCounter;
        }

        FSM0_DW.condWasTrueAtLastTimeStep_3_a = condIsTrue;
        guard1 = false;
        if ((!FSM0_DW.exit_test) && (FSM0_DW.chartAbsoluteTimeCounter -
             FSM0_DW.durationLastReferenceTick_3_d >= FSM0_P.FSM_debounce_tmo *
             100.0F)) {
          FSM0_DW.exit_test = true;
          guard1 = true;
        } else {
          // Inport: '<Root>/MultiSelector_Go'
          condIsTrue = (FSM0_U.MultiSelector_Go == Pressed);
          if ((!condIsTrue) || (!FSM0_DW.condWasTrueAtLastTimeStep_2_a)) {
            FSM0_DW.durationLastReferenceTick_2_l =
              FSM0_DW.chartAbsoluteTimeCounter;
          }

          FSM0_DW.condWasTrueAtLastTimeStep_2_a = condIsTrue;
          if (FSM0_DW.chartAbsoluteTimeCounter -
              FSM0_DW.durationLastReferenceTick_2_l >= FSM0_P.FSM_debounce_tmo *
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
          FSM0_DW.durationLastReferenceTick_5_l =
            FSM0_DW.chartAbsoluteTimeCounter;
          FSM0_DW.durationLastReferenceTick_4_h =
            FSM0_DW.chartAbsoluteTimeCounter;
          FSM0_DW.durationLastReferenceTick_3_d =
            FSM0_DW.chartAbsoluteTimeCounter;
          FSM0_DW.durationLastReferenceTick_2_l =
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
          FSM0_DW.condWasTrueAtLastTimeStep_2_a = (FSM0_U.MultiSelector_Go ==
            Pressed);

          // Inport: '<Root>/buttons'
          FSM0_DW.condWasTrueAtLastTimeStep_3_a =
            (FSM0_U.buttons[condIsTrue_tmp_0] == Released);
          FSM0_DW.condWasTrueAtLastTimeStep_4_d = (FSM0_U.buttons[condIsTrue_tmp]
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
    FSM0_DW.durationLastReferenceTick_1_g = FSM0_DW.chartAbsoluteTimeCounter;
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
    FSM0_DW.condWasTrueAtLastTimeStep_1_e = (FSM0_U.buttons[ledsSTATES_tmp] ==
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
      FSM0_DW.durationLastReferenceTick_1_k = FSM0_DW.chartAbsoluteTimeCounter;
    }

    FSM0_DW.condWasTrueAtLastTimeStep_1_h = condIsTrue;
    if (FSM0_DW.chartAbsoluteTimeCounter - FSM0_DW.durationLastReferenceTick_1_k
        >= FSM0_P.FSM_debounce_tmo * 100.0F) {
      FSM0_DW.enter_test = false;
      FSM0_DW.durationLastReferenceTick_6 = FSM0_DW.chartAbsoluteTimeCounter;
      FSM0_DW.durationLastReferenceTick_5 = FSM0_DW.chartAbsoluteTimeCounter;
      FSM0_DW.durationLastReferenceTick_4 = FSM0_DW.chartAbsoluteTimeCounter;
      FSM0_DW.durationLastReferenceTick_3 = FSM0_DW.chartAbsoluteTimeCounter;
      FSM0_DW.durationLastReferenceTick_2 = FSM0_DW.chartAbsoluteTimeCounter;
      FSM0_DW.durationLastReferenceTick_1_g = FSM0_DW.chartAbsoluteTimeCounter;
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
      FSM0_DW.condWasTrueAtLastTimeStep_1_e = (FSM0_U.buttons[condIsTrue_tmp] ==
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
    if ((!condIsTrue) || (!FSM0_DW.condWasTrueAtLastTimeStep_1_o)) {
      FSM0_DW.durationLastReferenceTick_1_b = FSM0_DW.chartAbsoluteTimeCounter;
    }

    FSM0_DW.condWasTrueAtLastTimeStep_1_o = condIsTrue;
    if (FSM0_DW.chartAbsoluteTimeCounter - FSM0_DW.durationLastReferenceTick_1_b
        >= FSM0_P.FSM_debounce_tmo * 100.0F) {
      FSM0_DW.exit_test = false;
      FSM0_DW.durationLastReferenceTick_5_l = FSM0_DW.chartAbsoluteTimeCounter;
      FSM0_DW.durationLastReferenceTick_4_h = FSM0_DW.chartAbsoluteTimeCounter;
      FSM0_DW.durationLastReferenceTick_3_d = FSM0_DW.chartAbsoluteTimeCounter;
      FSM0_DW.durationLastReferenceTick_2_l = FSM0_DW.chartAbsoluteTimeCounter;
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
      FSM0_DW.condWasTrueAtLastTimeStep_2_a = (FSM0_U.MultiSelector_Go ==
        Pressed);

      // Inport: '<Root>/buttons'
      FSM0_DW.condWasTrueAtLastTimeStep_3_a = (FSM0_U.buttons[condIsTrue_tmp] ==
        Released);
      FSM0_DW.condWasTrueAtLastTimeStep_4_d = (FSM0_U.buttons[b_previousEvent] ==
        Pressed);
      FSM0_DW.condWasTrueAtLastTimeStep_5_p = (FSM0_U.buttons[ledsSTATES_tmp] ==
        Pressed);
    } else if (FSM0_DW.temporalCounter_i1 >= FSM0_DW.test1_period * 100.0F) {
      FSM0_DW.durationLastReferenceTick_1_b = FSM0_DW.chartAbsoluteTimeCounter;
      FSM0_DW.is_STATES_HANDLING = FSM0_IN_TEST_1;
      FSM0_DW.temporalCounter_i1 = 0U;

      // Outport: '<Root>/test_IV_dc'
      FSM0_Y.test_IV_dc = 0.0F;

      // Outport: '<Root>/test_OV_cmd'
      FSM0_Y.test_OV_cmd = true;

      // Outport: '<Root>/ledsSTATES'
      FSM0_Y.ledsSTATES[static_cast<int32_T>(CPAP)] = On;

      // Inport: '<Root>/buttons'
      FSM0_DW.condWasTrueAtLastTimeStep_1_o = (FSM0_U.buttons[condIsTrue_tmp] ==
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
    if ((!condIsTrue) || (!FSM0_DW.condWasTrueAtLastTimeStep_1_es)) {
      FSM0_DW.durationLastReferenceTick_1_n = FSM0_DW.chartAbsoluteTimeCounter;
    }

    FSM0_DW.condWasTrueAtLastTimeStep_1_es = condIsTrue;
    if (FSM0_DW.chartAbsoluteTimeCounter - FSM0_DW.durationLastReferenceTick_1_n
        >= FSM0_P.FSM_debounce_tmo * 100.0F) {
      FSM0_DW.exit_test = false;
      FSM0_DW.durationLastReferenceTick_5_l = FSM0_DW.chartAbsoluteTimeCounter;
      FSM0_DW.durationLastReferenceTick_4_h = FSM0_DW.chartAbsoluteTimeCounter;
      FSM0_DW.durationLastReferenceTick_3_d = FSM0_DW.chartAbsoluteTimeCounter;
      FSM0_DW.durationLastReferenceTick_2_l = FSM0_DW.chartAbsoluteTimeCounter;
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
      FSM0_DW.condWasTrueAtLastTimeStep_2_a = (FSM0_U.MultiSelector_Go ==
        Pressed);

      // Inport: '<Root>/buttons'
      FSM0_DW.condWasTrueAtLastTimeStep_3_a = (FSM0_U.buttons[condIsTrue_tmp] ==
        Released);
      FSM0_DW.condWasTrueAtLastTimeStep_4_d = (FSM0_U.buttons[b_previousEvent] ==
        Pressed);
      FSM0_DW.condWasTrueAtLastTimeStep_5_p = (FSM0_U.buttons[ledsSTATES_tmp] ==
        Pressed);
    } else if (FSM0_DW.temporalCounter_i1 >= FSM0_DW.test2_period * 100.0F) {
      // Outport: '<Root>/test_OV_cmd'
      FSM0_Y.test_OV_cmd = !FSM0_Y.test_OV_cmd;
      FSM0_DW.durationLastReferenceTick_1_n = FSM0_DW.chartAbsoluteTimeCounter;
      FSM0_DW.is_STATES_HANDLING = FSM0_IN_TEST_2;
      FSM0_DW.temporalCounter_i1 = 0U;

      // Outport: '<Root>/test_IV_dc'
      FSM0_Y.test_IV_dc = 100.0F;

      // Outport: '<Root>/ledsSTATES'
      FSM0_Y.ledsSTATES[static_cast<int32_T>(VCV)] = On;

      // Inport: '<Root>/buttons'
      FSM0_DW.condWasTrueAtLastTimeStep_1_es = (FSM0_U.buttons[condIsTrue_tmp] ==
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
real_T FSM0ModelClass::FSM0_check_transitive_alarms(void)
{
  // Inport: '<Root>/Controller_Signals'
  if (FSM0_U.Controller_Signals[0] > FSM0_DW.alarms_S1S3_max) {
    // Outport: '<Root>/idALARM'
    FSM0_Y.idALARM = ALARM_OverPressureInlet;
  } else if (FSM0_U.Controller_Signals[2] > FSM0_DW.alarms_S1S3_max) {
    // Outport: '<Root>/idALARM'
    FSM0_Y.idALARM = ALARM_OverPressureInlet;
  } else if (FSM0_U.Controller_Signals[4] > FSM0_DW.alarms_PEEP) {
    // Outport: '<Root>/idALARM'
    FSM0_Y.idALARM = ALARM_PEEP;
  } else if (FSM0_U.Controller_Signals[8] < FSM0_DW.alarms_TD_min) {
    // Outport: '<Root>/idALARM'
    FSM0_Y.idALARM = ALARM_LowVolume;
  } else {
    // Outport: '<Root>/idALARM'
    FSM0_Y.idALARM = ALARM_None;
  }

  // End of Inport: '<Root>/Controller_Signals'

  // Outport: '<Root>/idALARM'
  return FSM0_Y.idALARM != ALARM_None;
}

// Model step function
void FSM0ModelClass::step()
{
  boolean_T condIsTrue;
  int32_T c_previousEvent;
  int32_T condIsTrue_tmp;
  int32_T condIsTrue_tmp_0;
  int32_T condIsTrue_tmp_1;

  // Outputs for Atomic SubSystem: '<Root>/FSM'
  // Constant: '<S9>/BPM [1//min]'
  FSM0_B.MatrixConcatenate[0] = FSM0_P.BPM1min_Value[0];

  // Constant: '<S9>/PIP [cmH2O]'
  FSM0_B.MatrixConcatenate[2] = FSM0_P.PIPcmH2O_Value[0];

  // Constant: '<S9>/IE_ratio [-]'
  FSM0_B.MatrixConcatenate[4] = FSM0_P.IE_ratio_Value[0];

  // Constant: '<S9>/TD [ml]'
  FSM0_B.MatrixConcatenate[6] = FSM0_P.TDml_Value[0];

  // Constant: '<S11>/Proportional'
  FSM0_B.MatrixConcatenate[8] = FSM0_P.Proportional_Value[0];

  // Constant: '<S11>/Integral'
  FSM0_B.MatrixConcatenate[10] = FSM0_P.Integral_Value[0];

  // Constant: '<S11>/Proportional Recovery'
  FSM0_B.MatrixConcatenate[12] = FSM0_P.ProportionalRecovery_Value[0];

  // Constant: '<S11>/Integral Recovery'
  FSM0_B.MatrixConcatenate[14] = FSM0_P.IntegralRecovery_Value[0];

  // Constant: '<S7>/Proportional'
  FSM0_B.MatrixConcatenate[16] = FSM0_P.Proportional_Value_j[0];

  // Constant: '<S7>/Integral'
  FSM0_B.MatrixConcatenate[18] = FSM0_P.Integral_Value_k[0];

  // Constant: '<S7>/Proportional Recovery'
  FSM0_B.MatrixConcatenate[20] = FSM0_P.ProportionalRecovery_Value_g[0];

  // Constant: '<S7>/Integral Recovery'
  FSM0_B.MatrixConcatenate[22] = FSM0_P.IntegralRecovery_Value_b[0];

  // Constant: '<S8>/Feed-Forward'
  FSM0_B.MatrixConcatenate[24] = FSM0_P.FeedForward_Value[0];

  // Constant: '<S8>/Feed-Forward Recovery'
  FSM0_B.MatrixConcatenate[26] = FSM0_P.FeedForwardRecovery_Value[0];

  // Constant: '<S8>/PIP Step Perc [%]'
  FSM0_B.MatrixConcatenate[28] = FSM0_P.PIPStepPerc_Value[0];

  // Constant: '<S8>/PIP Rise Perc [%]'
  FSM0_B.MatrixConcatenate[30] = FSM0_P.PIPRisePerc_Value[0];

  // Constant: '<S6>/Enable Trigger'
  FSM0_B.MatrixConcatenate[32] = FSM0_P.EnableTrigger_Value[0];

  // Constant: '<S6>/Pressure Threshold [cmH2O]'
  FSM0_B.MatrixConcatenate[34] = FSM0_P.PressureThresholdcmH2O_Value[0];

  // Constant: '<S6>/Flow-by Target'
  FSM0_B.MatrixConcatenate[36] = FSM0_P.FlowbyTarget_Value[0];

  // Constant: '<S6>/Not Used 2'
  FSM0_B.MatrixConcatenate[38] = FSM0_P.NotUsed2_Value[0];

  // Constant: '<S10>/Test1 Peak [%]'
  FSM0_B.MatrixConcatenate[40] = FSM0_P.Test1Peak_Value[0];

  // Constant: '<S10>/Test1 Period [s]'
  FSM0_B.MatrixConcatenate[42] = FSM0_P.Test1Periods_Value[0];

  // Constant: '<S10>/Test2 Period [s]'
  FSM0_B.MatrixConcatenate[44] = FSM0_P.Test2Periods_Value[0];

  // Constant: '<S10>/Not Used 1'
  FSM0_B.MatrixConcatenate[46] = FSM0_P.NotUsed1_Value[0];

  // Constant: '<S4>/S7 Min [cmH2O]'
  FSM0_B.MatrixConcatenate[48] = FSM0_P.S7MincmH2O_Value[0];

  // Constant: '<S4>/S7 Max [cmH2O]'
  FSM0_B.MatrixConcatenate[50] = FSM0_P.S7MaxcmH2O_Value[0];

  // Constant: '<S4>/S1,S3 Max [cmH2O]'
  FSM0_B.MatrixConcatenate[52] = FSM0_P.S1S3MaxcmH2O_Value[0];

  // Constant: '<S4>/PEEP [cmH2O]'
  FSM0_B.MatrixConcatenate[54] = FSM0_P.PEEPcmH2O_Value[0];

  // Constant: '<S5>/TD Min [L]'
  FSM0_B.MatrixConcatenate[56] = FSM0_P.TDMinL_Value[0];

  // Constant: '<S5>/Not Used 1'
  FSM0_B.MatrixConcatenate[58] = FSM0_P.NotUsed1_Value_l[0];

  // Constant: '<S5>/Not Used 2'
  FSM0_B.MatrixConcatenate[60] = FSM0_P.NotUsed2_Value_f[0];

  // Constant: '<S5>/Not Used 3'
  FSM0_B.MatrixConcatenate[62] = FSM0_P.NotUsed3_Value[0];

  // Constant: '<S9>/BPM [1//min]'
  FSM0_B.MatrixConcatenate[1] = FSM0_P.BPM1min_Value[1];

  // Constant: '<S9>/PIP [cmH2O]'
  FSM0_B.MatrixConcatenate[3] = FSM0_P.PIPcmH2O_Value[1];

  // Constant: '<S9>/IE_ratio [-]'
  FSM0_B.MatrixConcatenate[5] = FSM0_P.IE_ratio_Value[1];

  // Constant: '<S9>/TD [ml]'
  FSM0_B.MatrixConcatenate[7] = FSM0_P.TDml_Value[1];

  // Constant: '<S11>/Proportional'
  FSM0_B.MatrixConcatenate[9] = FSM0_P.Proportional_Value[1];

  // Constant: '<S11>/Integral'
  FSM0_B.MatrixConcatenate[11] = FSM0_P.Integral_Value[1];

  // Constant: '<S11>/Proportional Recovery'
  FSM0_B.MatrixConcatenate[13] = FSM0_P.ProportionalRecovery_Value[1];

  // Constant: '<S11>/Integral Recovery'
  FSM0_B.MatrixConcatenate[15] = FSM0_P.IntegralRecovery_Value[1];

  // Constant: '<S7>/Proportional'
  FSM0_B.MatrixConcatenate[17] = FSM0_P.Proportional_Value_j[1];

  // Constant: '<S7>/Integral'
  FSM0_B.MatrixConcatenate[19] = FSM0_P.Integral_Value_k[1];

  // Constant: '<S7>/Proportional Recovery'
  FSM0_B.MatrixConcatenate[21] = FSM0_P.ProportionalRecovery_Value_g[1];

  // Constant: '<S7>/Integral Recovery'
  FSM0_B.MatrixConcatenate[23] = FSM0_P.IntegralRecovery_Value_b[1];

  // Constant: '<S8>/Feed-Forward'
  FSM0_B.MatrixConcatenate[25] = FSM0_P.FeedForward_Value[1];

  // Constant: '<S8>/Feed-Forward Recovery'
  FSM0_B.MatrixConcatenate[27] = FSM0_P.FeedForwardRecovery_Value[1];

  // Constant: '<S8>/PIP Step Perc [%]'
  FSM0_B.MatrixConcatenate[29] = FSM0_P.PIPStepPerc_Value[1];

  // Constant: '<S8>/PIP Rise Perc [%]'
  FSM0_B.MatrixConcatenate[31] = FSM0_P.PIPRisePerc_Value[1];

  // Constant: '<S6>/Enable Trigger'
  FSM0_B.MatrixConcatenate[33] = FSM0_P.EnableTrigger_Value[1];

  // Constant: '<S6>/Pressure Threshold [cmH2O]'
  FSM0_B.MatrixConcatenate[35] = FSM0_P.PressureThresholdcmH2O_Value[1];

  // Constant: '<S6>/Flow-by Target'
  FSM0_B.MatrixConcatenate[37] = FSM0_P.FlowbyTarget_Value[1];

  // Constant: '<S6>/Not Used 2'
  FSM0_B.MatrixConcatenate[39] = FSM0_P.NotUsed2_Value[1];

  // Constant: '<S10>/Test1 Peak [%]'
  FSM0_B.MatrixConcatenate[41] = FSM0_P.Test1Peak_Value[1];

  // Constant: '<S10>/Test1 Period [s]'
  FSM0_B.MatrixConcatenate[43] = FSM0_P.Test1Periods_Value[1];

  // Constant: '<S10>/Test2 Period [s]'
  FSM0_B.MatrixConcatenate[45] = FSM0_P.Test2Periods_Value[1];

  // Constant: '<S10>/Not Used 1'
  FSM0_B.MatrixConcatenate[47] = FSM0_P.NotUsed1_Value[1];

  // Constant: '<S4>/S7 Min [cmH2O]'
  FSM0_B.MatrixConcatenate[49] = FSM0_P.S7MincmH2O_Value[1];

  // Constant: '<S4>/S7 Max [cmH2O]'
  FSM0_B.MatrixConcatenate[51] = FSM0_P.S7MaxcmH2O_Value[1];

  // Constant: '<S4>/S1,S3 Max [cmH2O]'
  FSM0_B.MatrixConcatenate[53] = FSM0_P.S1S3MaxcmH2O_Value[1];

  // Constant: '<S4>/PEEP [cmH2O]'
  FSM0_B.MatrixConcatenate[55] = FSM0_P.PEEPcmH2O_Value[1];

  // Constant: '<S5>/TD Min [L]'
  FSM0_B.MatrixConcatenate[57] = FSM0_P.TDMinL_Value[1];

  // Constant: '<S5>/Not Used 1'
  FSM0_B.MatrixConcatenate[59] = FSM0_P.NotUsed1_Value_l[1];

  // Constant: '<S5>/Not Used 2'
  FSM0_B.MatrixConcatenate[61] = FSM0_P.NotUsed2_Value_f[1];

  // Constant: '<S5>/Not Used 3'
  FSM0_B.MatrixConcatenate[63] = FSM0_P.NotUsed3_Value[1];

  // Chart: '<S1>/FSM' incorporates:
  //   Inport: '<Root>/MultiSelector_Go'
  //   Inport: '<Root>/buttons'
  //   Outport: '<Root>/Controller_Mode'
  //   Outport: '<Root>/idALARM'
  //   Outport: '<Root>/ledsSTATES'

  FSM0_DW.chartAbsoluteTimeCounter++;
  c_previousEvent = static_cast<int32_T>(IDLE);
  condIsTrue = (FSM0_U.buttons[c_previousEvent] == Pressed);
  if ((!condIsTrue) || (!FSM0_DW.condWasTrueAtLastTimeStep_1)) {
    FSM0_DW.durationLastReferenceTick_1 = FSM0_DW.chartAbsoluteTimeCounter;
  }

  FSM0_DW.condWasTrueAtLastTimeStep_1 = condIsTrue;
  condIsTrue = (FSM0_U.buttons[c_previousEvent] == Pressed);
  if ((!condIsTrue) || (!FSM0_DW.condWasTrueAtLastTimeStep_1_e)) {
    FSM0_DW.durationLastReferenceTick_1_g = FSM0_DW.chartAbsoluteTimeCounter;
  }

  FSM0_DW.condWasTrueAtLastTimeStep_1_e = condIsTrue;
  condIsTrue = (FSM0_U.buttons[c_previousEvent] == Released);
  if ((!condIsTrue) || (!FSM0_DW.condWasTrueAtLastTimeStep_2)) {
    FSM0_DW.durationLastReferenceTick_2 = FSM0_DW.chartAbsoluteTimeCounter;
  }

  FSM0_DW.condWasTrueAtLastTimeStep_2 = condIsTrue;
  condIsTrue_tmp = static_cast<int32_T>(CPAP);
  condIsTrue = (FSM0_U.buttons[condIsTrue_tmp] == Pressed);
  if ((!condIsTrue) || (!FSM0_DW.condWasTrueAtLastTimeStep_3)) {
    FSM0_DW.durationLastReferenceTick_3 = FSM0_DW.chartAbsoluteTimeCounter;
  }

  FSM0_DW.condWasTrueAtLastTimeStep_3 = condIsTrue;
  condIsTrue = (FSM0_U.buttons[c_previousEvent] == Pressed);
  if ((!condIsTrue) || (!FSM0_DW.condWasTrueAtLastTimeStep_1_h)) {
    FSM0_DW.durationLastReferenceTick_1_k = FSM0_DW.chartAbsoluteTimeCounter;
  }

  FSM0_DW.condWasTrueAtLastTimeStep_1_h = condIsTrue;
  condIsTrue = (FSM0_U.buttons[c_previousEvent] == Pressed);
  if ((!condIsTrue) || (!FSM0_DW.condWasTrueAtLastTimeStep_1_p)) {
    FSM0_DW.durationLastReferenceTick_1_h = FSM0_DW.chartAbsoluteTimeCounter;
  }

  FSM0_DW.condWasTrueAtLastTimeStep_1_p = condIsTrue;
  condIsTrue_tmp_0 = static_cast<int32_T>(VCV);
  condIsTrue = (FSM0_U.buttons[condIsTrue_tmp_0] == Pressed);
  if ((!condIsTrue) || (!FSM0_DW.condWasTrueAtLastTimeStep_4)) {
    FSM0_DW.durationLastReferenceTick_4 = FSM0_DW.chartAbsoluteTimeCounter;
  }

  FSM0_DW.condWasTrueAtLastTimeStep_4 = condIsTrue;
  condIsTrue_tmp_1 = static_cast<int32_T>(PRVC);
  condIsTrue = (FSM0_U.buttons[condIsTrue_tmp_1] == Pressed);
  if ((!condIsTrue) || (!FSM0_DW.condWasTrueAtLastTimeStep_5)) {
    FSM0_DW.durationLastReferenceTick_5 = FSM0_DW.chartAbsoluteTimeCounter;
  }

  FSM0_DW.condWasTrueAtLastTimeStep_5 = condIsTrue;
  condIsTrue = (FSM0_U.MultiSelector_Go == Pressed);
  if ((!condIsTrue) || (!FSM0_DW.condWasTrueAtLastTimeStep_6)) {
    FSM0_DW.durationLastReferenceTick_6 = FSM0_DW.chartAbsoluteTimeCounter;
  }

  FSM0_DW.condWasTrueAtLastTimeStep_6 = condIsTrue;
  condIsTrue = (FSM0_U.buttons[c_previousEvent] == Pressed);
  if ((!condIsTrue) || (!FSM0_DW.condWasTrueAtLastTimeStep_1_g)) {
    FSM0_DW.durationLastReferenceTick_1_i = FSM0_DW.chartAbsoluteTimeCounter;
  }

  FSM0_DW.condWasTrueAtLastTimeStep_1_g = condIsTrue;
  condIsTrue = (FSM0_U.MultiSelector_Go == Pressed);
  if ((!condIsTrue) || (!FSM0_DW.condWasTrueAtLastTimeStep_2_a)) {
    FSM0_DW.durationLastReferenceTick_2_l = FSM0_DW.chartAbsoluteTimeCounter;
  }

  FSM0_DW.condWasTrueAtLastTimeStep_2_a = condIsTrue;
  condIsTrue = (FSM0_U.buttons[c_previousEvent] == Released);
  if ((!condIsTrue) || (!FSM0_DW.condWasTrueAtLastTimeStep_3_a)) {
    FSM0_DW.durationLastReferenceTick_3_d = FSM0_DW.chartAbsoluteTimeCounter;
  }

  FSM0_DW.condWasTrueAtLastTimeStep_3_a = condIsTrue;
  condIsTrue = (FSM0_U.buttons[condIsTrue_tmp] == Pressed);
  if ((!condIsTrue) || (!FSM0_DW.condWasTrueAtLastTimeStep_4_d)) {
    FSM0_DW.durationLastReferenceTick_4_h = FSM0_DW.chartAbsoluteTimeCounter;
  }

  FSM0_DW.condWasTrueAtLastTimeStep_4_d = condIsTrue;
  condIsTrue = (FSM0_U.buttons[c_previousEvent] == Pressed);
  if ((!condIsTrue) || (!FSM0_DW.condWasTrueAtLastTimeStep_1_o)) {
    FSM0_DW.durationLastReferenceTick_1_b = FSM0_DW.chartAbsoluteTimeCounter;
  }

  FSM0_DW.condWasTrueAtLastTimeStep_1_o = condIsTrue;
  condIsTrue = (FSM0_U.buttons[condIsTrue_tmp_0] == Pressed);
  if ((!condIsTrue) || (!FSM0_DW.condWasTrueAtLastTimeStep_5_p)) {
    FSM0_DW.durationLastReferenceTick_5_l = FSM0_DW.chartAbsoluteTimeCounter;
  }

  FSM0_DW.condWasTrueAtLastTimeStep_5_p = condIsTrue;
  condIsTrue = (FSM0_U.buttons[c_previousEvent] == Pressed);
  if ((!condIsTrue) || (!FSM0_DW.condWasTrueAtLastTimeStep_1_es)) {
    FSM0_DW.durationLastReferenceTick_1_n = FSM0_DW.chartAbsoluteTimeCounter;
  }

  FSM0_DW.condWasTrueAtLastTimeStep_1_es = condIsTrue;
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
    FSM0_DW.durationLastReferenceTick_1_g = FSM0_DW.chartAbsoluteTimeCounter;
    FSM0_DW.is_STATES_HANDLING = FSM0_IN_IDLE;
    FSM0_Y.ledsSTATES[c_previousEvent] = On;
    FSM0_Y.ledsSTATES[condIsTrue_tmp] = Off;
    FSM0_Y.ledsSTATES[condIsTrue_tmp_0] = Off;
    FSM0_Y.ledsSTATES[condIsTrue_tmp_1] = Off;

    // Outport: '<Root>/Controller_Mode' incorporates:
    //   Outport: '<Root>/ledsSTATES'

    FSM0_Y.Controller_Mode = IDLE;
    FSM0_DW.condWasTrueAtLastTimeStep_1_e = (FSM0_U.buttons[c_previousEvent] ==
      Pressed);
    FSM0_DW.condWasTrueAtLastTimeStep_2 = (FSM0_U.buttons[c_previousEvent] ==
      Released);
    FSM0_DW.condWasTrueAtLastTimeStep_3 = (FSM0_U.buttons[condIsTrue_tmp] ==
      Pressed);
    FSM0_DW.condWasTrueAtLastTimeStep_4 = (FSM0_U.buttons[condIsTrue_tmp_0] ==
      Pressed);
    FSM0_DW.condWasTrueAtLastTimeStep_5 = (FSM0_U.buttons[condIsTrue_tmp_1] ==
      Pressed);
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
        } else if (!(FSM0_check_transitive_alarms() != 0.0)) {
          FSM0_DW.is_ALARMS_HANDLING = FSM0_IN_NO_ALARMS;

          // Outport: '<Root>/idALARM'
          FSM0_Y.idALARM = ALARM_None;

          // Outport: '<Root>/ledALARM'
          FSM0_Y.ledALARM = Off;

          // Outport: '<Root>/soundALARM'
          FSM0_Y.soundALARM = false;
        } else {
          if ((FSM0_Y.idALARM == ALARM_OverPressureInlet) &&
              (FSM0_Y.Controller_Mode != IDLE)) {
            FSM0_DW.resume_mode = FSM0_Y.Controller_Mode;
            c_previousEvent = FSM0_DW.sfEvent;
            FSM0_DW.sfEvent = FSM0_event_idle_controller;
            if (FSM0_DW.is_active_STATES_HANDLING != 0U) {
              FSM0_STATES_HANDLING();
            }

            FSM0_DW.sfEvent = c_previousEvent;
            FSM0_DW.is_ALARMS_HANDLING = FSM0_IN_TRANSITIVE_ALARMS;

            // Outport: '<Root>/ledALARM' incorporates:
            //   Outport: '<Root>/Controller_Mode'

            FSM0_Y.ledALARM = On;

            // Outport: '<Root>/soundALARM'
            FSM0_Y.soundALARM = true;
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
  // End of Outputs for SubSystem: '<Root>/FSM'
}

// Model initialize function
void FSM0ModelClass::initialize()
{
  // SystemInitialize for Atomic SubSystem: '<Root>/FSM'
  // SystemInitialize for Chart: '<S1>/FSM'
  FSM0_DW.sfEvent = FSM0_CALL_EVENT;
  FSM0_DW.MultiSelector_Maps_latch = 1U;
  FSM0_DW.alarms_S7_max = 1500.0F;
  FSM0_DW.alarms_S1S3_max = 100.0F;
  FSM0_DW.alarms_PEEP = 5.0F;
  FSM0_DW.resume_mode = IDLE;

  // InitializeConditions for Outport: '<Root>/ledsSTATES' incorporates:
  //   Chart: '<S1>/FSM'

  FSM0_Y.ledsSTATES[0] = Off;
  FSM0_Y.ledsSTATES[1] = Off;
  FSM0_Y.ledsSTATES[2] = Off;
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

  // End of SystemInitialize for SubSystem: '<Root>/FSM'
}

// Model terminate function
void FSM0ModelClass::terminate()
{
  // (no terminate code required)
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
