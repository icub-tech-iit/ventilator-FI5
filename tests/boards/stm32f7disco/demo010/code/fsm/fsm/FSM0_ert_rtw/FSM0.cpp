//
// Non-Degree Granting Education License -- for use at non-degree
// granting, nonprofit, educational organizations only. Not for
// government, commercial, or other organizational use.
//
// File: FSM0.cpp
//
// Code generated for Simulink model 'FSM0'.
//
// Model version                  : 1.694
// Simulink Coder version         : 9.3 (R2020a) 18-Nov-2019
// C/C++ source code generated on : Wed May  6 09:08:23 2020
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
const uint8_T FSM0_IN_IDLE = 2U;
const uint8_T FSM0_IN_IDLE_n = 5U;
const uint8_T FSM0_IN_INIT = 1U;
const uint8_T FSM0_IN_INIT_d = 3U;
const uint8_T FSM0_IN_NO_ACTIVE_CHILD = 0U;
const uint8_T FSM0_IN_OFF = 2U;
const uint8_T FSM0_IN_ON = 3U;
const uint8_T FSM0_IN_PRVC = 4U;
const uint8_T FSM0_IN_TEST_1 = 6U;
const uint8_T FSM0_IN_TEST_2 = 7U;
const uint8_T FSM0_IN_VCV = 8U;
const int32_T FSM0_event_blink_IDLE_off = 0;
const int32_T FSM0_event_blink_IDLE_on = 1;
const int32_T FSM0_event_idle_controller = 2;

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
  int32_T ledsSTATES_tmp;
  int32_T ledsSTATES_tmp_0;
  int32_T ledsSTATES_tmp_1;
  int32_T ledsSTATES_tmp_2;
  boolean_T guard1 = false;

  // Outport: '<Root>/Controller_Mode'
  FSM0_Y.Controller_Mode = CPAP;
  guard1 = false;
  if (FSM0_DW.sfEvent == FSM0_event_idle_controller) {
    guard1 = true;
  } else {
    // Inport: '<Root>/buttons'
    condIsTrue = (FSM0_U.buttons[static_cast<int32_T>(IDLE)] == Pressed);
    if ((!condIsTrue) || (!FSM0_DW.condWasTrueAtLastTimeStep_1_e)) {
      FSM0_DW.durationLastReferenceTick_1_g5 = FSM0_DW.chartAbsoluteTimeCounter;
    }

    FSM0_DW.condWasTrueAtLastTimeStep_1_e = condIsTrue;
    if (FSM0_DW.chartAbsoluteTimeCounter -
        FSM0_DW.durationLastReferenceTick_1_g5 >= FSM0_P.FSM_debounce_tmo *
        100.0F) {
      guard1 = true;
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
  }

  if (guard1) {
    FSM0_DW.durationLastReferenceTick_6 = FSM0_DW.chartAbsoluteTimeCounter;
    FSM0_DW.durationLastReferenceTick_5 = FSM0_DW.chartAbsoluteTimeCounter;
    FSM0_DW.durationLastReferenceTick_4 = FSM0_DW.chartAbsoluteTimeCounter;
    FSM0_DW.durationLastReferenceTick_3 = FSM0_DW.chartAbsoluteTimeCounter;
    FSM0_DW.durationLastReferenceTick_2 = FSM0_DW.chartAbsoluteTimeCounter;
    FSM0_DW.durationLastReferenceTick_1 = FSM0_DW.chartAbsoluteTimeCounter;
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
    ledsSTATES_tmp_2 = static_cast<int32_T>(PRVC);

    // Outport: '<Root>/ledsSTATES'
    FSM0_Y.ledsSTATES[ledsSTATES_tmp_2] = Off;

    // Outport: '<Root>/Controller_Mode'
    FSM0_Y.Controller_Mode = IDLE;

    // Inport: '<Root>/buttons'
    FSM0_DW.condWasTrueAtLastTimeStep_1 = (FSM0_U.buttons[ledsSTATES_tmp] ==
      Pressed);
    FSM0_DW.condWasTrueAtLastTimeStep_2 = (FSM0_U.buttons[ledsSTATES_tmp] ==
      Released);
    FSM0_DW.condWasTrueAtLastTimeStep_3 = (FSM0_U.buttons[ledsSTATES_tmp_0] ==
      Pressed);
    FSM0_DW.condWasTrueAtLastTimeStep_4 = (FSM0_U.buttons[ledsSTATES_tmp_1] ==
      Pressed);

    // Inport: '<Root>/MultiSelector_Go'
    FSM0_DW.condWasTrueAtLastTimeStep_5 = (FSM0_U.MultiSelector_Go == Pressed);

    // Inport: '<Root>/buttons'
    FSM0_DW.condWasTrueAtLastTimeStep_6 = (FSM0_U.buttons[ledsSTATES_tmp_2] ==
      Pressed);
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
  if ((input >= 0) && (input <= 5)) {
    // Set output value to input value if it is a member of MapIndex
    y = input;
  }

  return y;
}

// Function for Chart: '<S1>/FSM'
void FSM0ModelClass::FSM0_latch_multiselector(void)
{
  // Inport: '<Root>/MultiSelector_Maps'
  if (FSM0_U.MultiSelector_Maps < 1U) {
    FSM0_DW.MultiSelector_Maps_latch = 1U;
  } else if (FSM0_U.MultiSelector_Maps > 5U) {
    FSM0_DW.MultiSelector_Maps_latch = 5U;
  } else {
    FSM0_DW.MultiSelector_Maps_latch = FSM0_U.MultiSelector_Maps;
  }

  // End of Inport: '<Root>/MultiSelector_Maps'
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

  // Outport: '<Root>/Controller_Mode'
  FSM0_Y.Controller_Mode = IDLE;
  condIsTrue_tmp = static_cast<int32_T>(CPAP);

  // Inport: '<Root>/buttons'
  condIsTrue = (FSM0_U.buttons[condIsTrue_tmp] == Pressed);
  if ((!condIsTrue) || (!FSM0_DW.condWasTrueAtLastTimeStep_3)) {
    FSM0_DW.durationLastReferenceTick_3 = FSM0_DW.chartAbsoluteTimeCounter;
  }

  FSM0_DW.condWasTrueAtLastTimeStep_3 = condIsTrue;
  if (FSM0_DW.chartAbsoluteTimeCounter - FSM0_DW.durationLastReferenceTick_3 >=
      FSM0_P.FSM_debounce_tmo * 100.0F) {
    FSM0_DW.enter_test = false;

    // Outport: '<Root>/knobs_transfer'
    FSM0_Y.knobs_transfer = MAP_Transfer_None;
    FSM0_DW.durationLastReferenceTick_1_g5 = FSM0_DW.chartAbsoluteTimeCounter;
    FSM0_DW.is_STATES_HANDLING = FSM0_IN_CPAP;
    condIsTrue_tmp_2 = static_cast<int32_T>(IDLE);

    // Outport: '<Root>/ledsSTATES'
    FSM0_Y.ledsSTATES[condIsTrue_tmp_2] = Off;
    FSM0_Y.ledsSTATES[condIsTrue_tmp] = On;

    // Outport: '<Root>/Controller_Mode'
    FSM0_Y.Controller_Mode = CPAP;

    // Inport: '<Root>/buttons'
    FSM0_DW.condWasTrueAtLastTimeStep_1_e = (FSM0_U.buttons[condIsTrue_tmp_2] ==
      Pressed);
  } else {
    condIsTrue_tmp_0 = static_cast<int32_T>(VCV);

    // Inport: '<Root>/buttons'
    condIsTrue = (FSM0_U.buttons[condIsTrue_tmp_0] == Pressed);
    if ((!condIsTrue) || (!FSM0_DW.condWasTrueAtLastTimeStep_4)) {
      FSM0_DW.durationLastReferenceTick_4 = FSM0_DW.chartAbsoluteTimeCounter;
    }

    FSM0_DW.condWasTrueAtLastTimeStep_4 = condIsTrue;
    if (FSM0_DW.chartAbsoluteTimeCounter - FSM0_DW.durationLastReferenceTick_4 >=
        FSM0_P.FSM_debounce_tmo * 100.0F) {
      // Outport: '<Root>/knobs_transfer'
      FSM0_Y.knobs_transfer = MAP_Transfer_None;
      FSM0_DW.durationLastReferenceTick_1_g = FSM0_DW.chartAbsoluteTimeCounter;
      FSM0_DW.is_STATES_HANDLING = FSM0_IN_VCV;
      condIsTrue_tmp_2 = static_cast<int32_T>(IDLE);

      // Outport: '<Root>/ledsSTATES'
      FSM0_Y.ledsSTATES[condIsTrue_tmp_2] = Off;
      FSM0_Y.ledsSTATES[condIsTrue_tmp_0] = On;

      // Outport: '<Root>/Controller_Mode'
      FSM0_Y.Controller_Mode = VCV;

      // Inport: '<Root>/buttons'
      FSM0_DW.condWasTrueAtLastTimeStep_1_d = (FSM0_U.buttons[condIsTrue_tmp_2] ==
        Pressed);
    } else {
      condIsTrue_tmp_1 = static_cast<int32_T>(PRVC);

      // Inport: '<Root>/buttons'
      condIsTrue = (FSM0_U.buttons[condIsTrue_tmp_1] == Pressed);
      if ((!condIsTrue) || (!FSM0_DW.condWasTrueAtLastTimeStep_6)) {
        FSM0_DW.durationLastReferenceTick_6 = FSM0_DW.chartAbsoluteTimeCounter;
      }

      FSM0_DW.condWasTrueAtLastTimeStep_6 = condIsTrue;
      if (FSM0_DW.chartAbsoluteTimeCounter - FSM0_DW.durationLastReferenceTick_6
          >= FSM0_P.FSM_debounce_tmo * 100.0F) {
        // Outport: '<Root>/knobs_transfer'
        FSM0_Y.knobs_transfer = MAP_Transfer_None;
        FSM0_DW.durationLastReferenceTick_1_h = FSM0_DW.chartAbsoluteTimeCounter;
        FSM0_DW.is_STATES_HANDLING = FSM0_IN_PRVC;
        condIsTrue_tmp_2 = static_cast<int32_T>(IDLE);

        // Outport: '<Root>/ledsSTATES'
        FSM0_Y.ledsSTATES[condIsTrue_tmp_2] = Off;
        FSM0_Y.ledsSTATES[condIsTrue_tmp_1] = On;

        // Outport: '<Root>/Controller_Mode'
        FSM0_Y.Controller_Mode = PRVC;

        // Inport: '<Root>/buttons'
        FSM0_DW.condWasTrueAtLastTimeStep_1_p = (FSM0_U.buttons[condIsTrue_tmp_2]
          == Pressed);
      } else {
        condIsTrue_tmp_2 = static_cast<int32_T>(IDLE);

        // Inport: '<Root>/buttons'
        condIsTrue = (FSM0_U.buttons[condIsTrue_tmp_2] == Pressed);
        if ((!condIsTrue) || (!FSM0_DW.condWasTrueAtLastTimeStep_1)) {
          FSM0_DW.durationLastReferenceTick_1 = FSM0_DW.chartAbsoluteTimeCounter;
        }

        FSM0_DW.condWasTrueAtLastTimeStep_1 = condIsTrue;
        if (FSM0_DW.enter_test && (FSM0_DW.chartAbsoluteTimeCounter -
             FSM0_DW.durationLastReferenceTick_1 >= FSM0_P.FSM_test_tmo * 100.0F))
        {
          FSM0_DW.enter_test = false;

          // Outport: '<Root>/knobs_transfer'
          FSM0_Y.knobs_transfer = MAP_Transfer_None;
          FSM0_DW.is_STATES_HANDLING = FSM0_IN_NO_ACTIVE_CHILD;
          b_previousEvent = FSM0_DW.sfEvent;
          FSM0_DW.sfEvent = FSM0_event_blink_IDLE_on;

          // Chart: '<S1>/FSM'
          FSM0_c3_FSM0();
          FSM0_DW.sfEvent = b_previousEvent;
          if ((FSM0_DW.is_STATES_HANDLING == 0) &&
              (FSM0_DW.is_active_STATES_HANDLING != 0U)) {
            FSM0_DW.durationLastReferenceTick_5_c =
              FSM0_DW.chartAbsoluteTimeCounter;
            FSM0_DW.durationLastReferenceTick_4_g =
              FSM0_DW.chartAbsoluteTimeCounter;
            FSM0_DW.durationLastReferenceTick_3_b =
              FSM0_DW.chartAbsoluteTimeCounter;
            FSM0_DW.durationLastReferenceTick_2_b =
              FSM0_DW.chartAbsoluteTimeCounter;
            FSM0_DW.durationLastReferenceTick_1_f =
              FSM0_DW.chartAbsoluteTimeCounter;
            FSM0_DW.is_STATES_HANDLING = FSM0_IN_IDLE_n;

            // Outport: '<Root>/ledsSTATES'
            FSM0_Y.ledsSTATES[condIsTrue_tmp] = Off;
            FSM0_Y.ledsSTATES[condIsTrue_tmp_0] = Off;
            FSM0_Y.ledsSTATES[condIsTrue_tmp_1] = Off;

            // Outport: '<Root>/test_IV_dc'
            FSM0_Y.test_IV_dc = 0.0F;

            // Outport: '<Root>/test_OV_cmd'
            FSM0_Y.test_OV_cmd = false;

            // Inport: '<Root>/buttons'
            FSM0_DW.condWasTrueAtLastTimeStep_1_o =
              (FSM0_U.buttons[condIsTrue_tmp_2] == Pressed);

            // Inport: '<Root>/MultiSelector_Go'
            FSM0_DW.condWasTrueAtLastTimeStep_2_b = (FSM0_U.MultiSelector_Go ==
              Pressed);

            // Inport: '<Root>/buttons'
            FSM0_DW.condWasTrueAtLastTimeStep_3_m =
              (FSM0_U.buttons[condIsTrue_tmp_2] == Released);
            FSM0_DW.condWasTrueAtLastTimeStep_4_f =
              (FSM0_U.buttons[condIsTrue_tmp] == Pressed);
            FSM0_DW.condWasTrueAtLastTimeStep_5_c =
              (FSM0_U.buttons[condIsTrue_tmp_0] == Pressed);
          }
        } else {
          // Inport: '<Root>/buttons'
          condIsTrue = (FSM0_U.buttons[condIsTrue_tmp_2] == Released);
          if ((!condIsTrue) || (!FSM0_DW.condWasTrueAtLastTimeStep_2)) {
            FSM0_DW.durationLastReferenceTick_2 =
              FSM0_DW.chartAbsoluteTimeCounter;
          }

          FSM0_DW.condWasTrueAtLastTimeStep_2 = condIsTrue;
          guard1 = false;
          if ((!FSM0_DW.enter_test) && (FSM0_DW.chartAbsoluteTimeCounter -
               FSM0_DW.durationLastReferenceTick_2 >= FSM0_P.FSM_debounce_tmo *
               100.0F)) {
            FSM0_DW.enter_test = true;
            guard1 = true;
          } else {
            // Inport: '<Root>/MultiSelector_Go'
            condIsTrue = (FSM0_U.MultiSelector_Go == Pressed);
            if ((!condIsTrue) || (!FSM0_DW.condWasTrueAtLastTimeStep_5)) {
              FSM0_DW.durationLastReferenceTick_5 =
                FSM0_DW.chartAbsoluteTimeCounter;
            }

            FSM0_DW.condWasTrueAtLastTimeStep_5 = condIsTrue;
            if (FSM0_DW.chartAbsoluteTimeCounter -
                FSM0_DW.durationLastReferenceTick_5 >= FSM0_P.FSM_debounce_tmo *
                100.0F) {
              FSM0_latch_multiselector();
              guard1 = true;
            } else {
              FSM0_apply_map(static_cast<real_T>
                             (FSM0_DW.MultiSelector_Maps_latch));

              // Outport: '<Root>/knobs_transfer'
              FSM0_Y.knobs_transfer = static_cast<MapIndex>
                (FSM0_safe_cast_to_MapIndex(static_cast<int32_T>
                  (FSM0_DW.MultiSelector_Maps_latch)));
            }
          }

          if (guard1) {
            // Outport: '<Root>/knobs_transfer'
            FSM0_Y.knobs_transfer = MAP_Transfer_None;
            FSM0_DW.durationLastReferenceTick_6 =
              FSM0_DW.chartAbsoluteTimeCounter;
            FSM0_DW.durationLastReferenceTick_5 =
              FSM0_DW.chartAbsoluteTimeCounter;
            FSM0_DW.durationLastReferenceTick_4 =
              FSM0_DW.chartAbsoluteTimeCounter;
            FSM0_DW.durationLastReferenceTick_3 =
              FSM0_DW.chartAbsoluteTimeCounter;
            FSM0_DW.durationLastReferenceTick_2 =
              FSM0_DW.chartAbsoluteTimeCounter;
            FSM0_DW.durationLastReferenceTick_1 =
              FSM0_DW.chartAbsoluteTimeCounter;
            FSM0_DW.is_STATES_HANDLING = FSM0_IN_IDLE;

            // Outport: '<Root>/ledsSTATES'
            FSM0_Y.ledsSTATES[condIsTrue_tmp_2] = On;
            FSM0_Y.ledsSTATES[condIsTrue_tmp] = Off;
            FSM0_Y.ledsSTATES[condIsTrue_tmp_0] = Off;
            FSM0_Y.ledsSTATES[condIsTrue_tmp_1] = Off;

            // Outport: '<Root>/Controller_Mode'
            FSM0_Y.Controller_Mode = IDLE;

            // Inport: '<Root>/buttons'
            FSM0_DW.condWasTrueAtLastTimeStep_1 =
              (FSM0_U.buttons[condIsTrue_tmp_2] == Pressed);
            FSM0_DW.condWasTrueAtLastTimeStep_2 =
              (FSM0_U.buttons[condIsTrue_tmp_2] == Released);
            FSM0_DW.condWasTrueAtLastTimeStep_3 = (FSM0_U.buttons[condIsTrue_tmp]
              == Pressed);
            FSM0_DW.condWasTrueAtLastTimeStep_4 =
              (FSM0_U.buttons[condIsTrue_tmp_0] == Pressed);

            // Inport: '<Root>/MultiSelector_Go'
            FSM0_DW.condWasTrueAtLastTimeStep_5 = (FSM0_U.MultiSelector_Go ==
              Pressed);

            // Inport: '<Root>/buttons'
            FSM0_DW.condWasTrueAtLastTimeStep_6 =
              (FSM0_U.buttons[condIsTrue_tmp_1] == Pressed);
          }
        }
      }
    }
  }
}

// Function for Chart: '<S1>/FSM'
void FSM0ModelClass::FSM0_INIT(void)
{
  int32_T ledsSTATES_tmp;
  int32_T ledsSTATES_tmp_0;
  int32_T ledsSTATES_tmp_1;
  int32_T ledsSTATES_tmp_2;
  FSM0_DW.durationLastReferenceTick_6 = FSM0_DW.chartAbsoluteTimeCounter;
  FSM0_DW.durationLastReferenceTick_5 = FSM0_DW.chartAbsoluteTimeCounter;
  FSM0_DW.durationLastReferenceTick_4 = FSM0_DW.chartAbsoluteTimeCounter;
  FSM0_DW.durationLastReferenceTick_3 = FSM0_DW.chartAbsoluteTimeCounter;
  FSM0_DW.durationLastReferenceTick_2 = FSM0_DW.chartAbsoluteTimeCounter;
  FSM0_DW.durationLastReferenceTick_1 = FSM0_DW.chartAbsoluteTimeCounter;
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
  ledsSTATES_tmp_2 = static_cast<int32_T>(PRVC);

  // Outport: '<Root>/ledsSTATES'
  FSM0_Y.ledsSTATES[ledsSTATES_tmp_2] = Off;

  // Outport: '<Root>/Controller_Mode'
  FSM0_Y.Controller_Mode = IDLE;

  // Inport: '<Root>/buttons'
  FSM0_DW.condWasTrueAtLastTimeStep_1 = (FSM0_U.buttons[ledsSTATES_tmp] ==
    Pressed);
  FSM0_DW.condWasTrueAtLastTimeStep_2 = (FSM0_U.buttons[ledsSTATES_tmp] ==
    Released);
  FSM0_DW.condWasTrueAtLastTimeStep_3 = (FSM0_U.buttons[ledsSTATES_tmp_0] ==
    Pressed);
  FSM0_DW.condWasTrueAtLastTimeStep_4 = (FSM0_U.buttons[ledsSTATES_tmp_1] ==
    Pressed);

  // Inport: '<Root>/MultiSelector_Go'
  FSM0_DW.condWasTrueAtLastTimeStep_5 = (FSM0_U.MultiSelector_Go == Pressed);

  // Inport: '<Root>/buttons'
  FSM0_DW.condWasTrueAtLastTimeStep_6 = (FSM0_U.buttons[ledsSTATES_tmp_2] ==
    Pressed);
}

// Function for Chart: '<S1>/FSM'
void FSM0ModelClass::FSM0_PRVC(void)
{
  boolean_T condIsTrue;
  real_T tmp;
  int32_T ledsSTATES_tmp;
  int32_T ledsSTATES_tmp_0;
  int32_T ledsSTATES_tmp_1;
  int32_T ledsSTATES_tmp_2;
  boolean_T guard1 = false;

  // Outport: '<Root>/Controller_Mode'
  FSM0_Y.Controller_Mode = PRVC;
  guard1 = false;
  if (FSM0_DW.sfEvent == FSM0_event_idle_controller) {
    guard1 = true;
  } else {
    // Inport: '<Root>/buttons'
    condIsTrue = (FSM0_U.buttons[static_cast<int32_T>(IDLE)] == Pressed);
    if ((!condIsTrue) || (!FSM0_DW.condWasTrueAtLastTimeStep_1_p)) {
      FSM0_DW.durationLastReferenceTick_1_h = FSM0_DW.chartAbsoluteTimeCounter;
    }

    FSM0_DW.condWasTrueAtLastTimeStep_1_p = condIsTrue;
    if (FSM0_DW.chartAbsoluteTimeCounter - FSM0_DW.durationLastReferenceTick_1_h
        >= FSM0_P.FSM_debounce_tmo * 100.0F) {
      FSM0_DW.enter_test = false;
      guard1 = true;
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
  }

  if (guard1) {
    FSM0_DW.durationLastReferenceTick_6 = FSM0_DW.chartAbsoluteTimeCounter;
    FSM0_DW.durationLastReferenceTick_5 = FSM0_DW.chartAbsoluteTimeCounter;
    FSM0_DW.durationLastReferenceTick_4 = FSM0_DW.chartAbsoluteTimeCounter;
    FSM0_DW.durationLastReferenceTick_3 = FSM0_DW.chartAbsoluteTimeCounter;
    FSM0_DW.durationLastReferenceTick_2 = FSM0_DW.chartAbsoluteTimeCounter;
    FSM0_DW.durationLastReferenceTick_1 = FSM0_DW.chartAbsoluteTimeCounter;
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
    ledsSTATES_tmp_2 = static_cast<int32_T>(PRVC);

    // Outport: '<Root>/ledsSTATES'
    FSM0_Y.ledsSTATES[ledsSTATES_tmp_2] = Off;

    // Outport: '<Root>/Controller_Mode'
    FSM0_Y.Controller_Mode = IDLE;

    // Inport: '<Root>/buttons'
    FSM0_DW.condWasTrueAtLastTimeStep_1 = (FSM0_U.buttons[ledsSTATES_tmp] ==
      Pressed);
    FSM0_DW.condWasTrueAtLastTimeStep_2 = (FSM0_U.buttons[ledsSTATES_tmp] ==
      Released);
    FSM0_DW.condWasTrueAtLastTimeStep_3 = (FSM0_U.buttons[ledsSTATES_tmp_0] ==
      Pressed);
    FSM0_DW.condWasTrueAtLastTimeStep_4 = (FSM0_U.buttons[ledsSTATES_tmp_1] ==
      Pressed);

    // Inport: '<Root>/MultiSelector_Go'
    FSM0_DW.condWasTrueAtLastTimeStep_5 = (FSM0_U.MultiSelector_Go == Pressed);

    // Inport: '<Root>/buttons'
    FSM0_DW.condWasTrueAtLastTimeStep_6 = (FSM0_U.buttons[ledsSTATES_tmp_2] ==
      Pressed);
  }
}

// Function for Chart: '<S1>/FSM'
void FSM0ModelClass::FSM0_IDLE(void)
{
  boolean_T condIsTrue;
  int32_T b_previousEvent;
  real_T tmp;
  int32_T condIsTrue_tmp;
  int32_T condIsTrue_tmp_0;
  int32_T ledsSTATES_tmp;
  boolean_T guard1 = false;
  condIsTrue_tmp = static_cast<int32_T>(CPAP);

  // Inport: '<Root>/buttons'
  condIsTrue = (FSM0_U.buttons[condIsTrue_tmp] == Pressed);
  if ((!condIsTrue) || (!FSM0_DW.condWasTrueAtLastTimeStep_4_f)) {
    FSM0_DW.durationLastReferenceTick_4_g = FSM0_DW.chartAbsoluteTimeCounter;
  }

  FSM0_DW.condWasTrueAtLastTimeStep_4_f = condIsTrue;
  if (FSM0_DW.chartAbsoluteTimeCounter - FSM0_DW.durationLastReferenceTick_4_g >=
      FSM0_P.FSM_debounce_tmo * 100.0F) {
    // Outport: '<Root>/knobs_transfer'
    FSM0_Y.knobs_transfer = MAP_Transfer_None;
    FSM0_DW.durationLastReferenceTick_1_i = FSM0_DW.chartAbsoluteTimeCounter;
    FSM0_DW.is_STATES_HANDLING = FSM0_IN_TEST_1;
    FSM0_DW.temporalCounter_i1 = 0U;

    // Outport: '<Root>/test_IV_dc'
    FSM0_Y.test_IV_dc = 0.0F;

    // Outport: '<Root>/test_OV_cmd'
    FSM0_Y.test_OV_cmd = true;

    // Outport: '<Root>/ledsSTATES'
    FSM0_Y.ledsSTATES[condIsTrue_tmp] = On;

    // Inport: '<Root>/buttons'
    FSM0_DW.condWasTrueAtLastTimeStep_1_g = (FSM0_U.buttons[static_cast<int32_T>
      (IDLE)] == Pressed);
  } else {
    condIsTrue_tmp_0 = static_cast<int32_T>(IDLE);

    // Inport: '<Root>/buttons'
    condIsTrue = (FSM0_U.buttons[condIsTrue_tmp_0] == Pressed);
    if ((!condIsTrue) || (!FSM0_DW.condWasTrueAtLastTimeStep_1_o)) {
      FSM0_DW.durationLastReferenceTick_1_f = FSM0_DW.chartAbsoluteTimeCounter;
    }

    FSM0_DW.condWasTrueAtLastTimeStep_1_o = condIsTrue;
    if (FSM0_DW.exit_test && (FSM0_DW.chartAbsoluteTimeCounter -
         FSM0_DW.durationLastReferenceTick_1_f >= FSM0_P.FSM_test_tmo * 100.0F))
    {
      FSM0_DW.exit_test = false;

      // Outport: '<Root>/knobs_transfer'
      FSM0_Y.knobs_transfer = MAP_Transfer_None;
      FSM0_DW.is_STATES_HANDLING = FSM0_IN_NO_ACTIVE_CHILD;
      b_previousEvent = FSM0_DW.sfEvent;
      FSM0_DW.sfEvent = FSM0_event_blink_IDLE_off;

      // Chart: '<S1>/FSM'
      FSM0_c3_FSM0();
      FSM0_DW.sfEvent = b_previousEvent;
      if ((FSM0_DW.is_STATES_HANDLING == 0) &&
          (FSM0_DW.is_active_STATES_HANDLING != 0U)) {
        FSM0_DW.durationLastReferenceTick_6 = FSM0_DW.chartAbsoluteTimeCounter;
        FSM0_DW.durationLastReferenceTick_5 = FSM0_DW.chartAbsoluteTimeCounter;
        FSM0_DW.durationLastReferenceTick_4 = FSM0_DW.chartAbsoluteTimeCounter;
        FSM0_DW.durationLastReferenceTick_3 = FSM0_DW.chartAbsoluteTimeCounter;
        FSM0_DW.durationLastReferenceTick_2 = FSM0_DW.chartAbsoluteTimeCounter;
        FSM0_DW.durationLastReferenceTick_1 = FSM0_DW.chartAbsoluteTimeCounter;
        FSM0_DW.is_STATES_HANDLING = FSM0_IN_IDLE;

        // Outport: '<Root>/ledsSTATES'
        FSM0_Y.ledsSTATES[condIsTrue_tmp_0] = On;
        FSM0_Y.ledsSTATES[condIsTrue_tmp] = Off;
        b_previousEvent = static_cast<int32_T>(VCV);

        // Outport: '<Root>/ledsSTATES'
        FSM0_Y.ledsSTATES[b_previousEvent] = Off;
        ledsSTATES_tmp = static_cast<int32_T>(PRVC);

        // Outport: '<Root>/ledsSTATES'
        FSM0_Y.ledsSTATES[ledsSTATES_tmp] = Off;

        // Outport: '<Root>/Controller_Mode'
        FSM0_Y.Controller_Mode = IDLE;

        // Inport: '<Root>/buttons'
        FSM0_DW.condWasTrueAtLastTimeStep_1 = (FSM0_U.buttons[condIsTrue_tmp_0] ==
          Pressed);
        FSM0_DW.condWasTrueAtLastTimeStep_2 = (FSM0_U.buttons[condIsTrue_tmp_0] ==
          Released);
        FSM0_DW.condWasTrueAtLastTimeStep_3 = (FSM0_U.buttons[condIsTrue_tmp] ==
          Pressed);
        FSM0_DW.condWasTrueAtLastTimeStep_4 = (FSM0_U.buttons[b_previousEvent] ==
          Pressed);

        // Inport: '<Root>/MultiSelector_Go'
        FSM0_DW.condWasTrueAtLastTimeStep_5 = (FSM0_U.MultiSelector_Go ==
          Pressed);

        // Inport: '<Root>/buttons'
        FSM0_DW.condWasTrueAtLastTimeStep_6 = (FSM0_U.buttons[ledsSTATES_tmp] ==
          Pressed);
      }
    } else {
      b_previousEvent = static_cast<int32_T>(VCV);

      // Inport: '<Root>/buttons'
      condIsTrue = (FSM0_U.buttons[b_previousEvent] == Pressed);
      if ((!condIsTrue) || (!FSM0_DW.condWasTrueAtLastTimeStep_5_c)) {
        FSM0_DW.durationLastReferenceTick_5_c = FSM0_DW.chartAbsoluteTimeCounter;
      }

      FSM0_DW.condWasTrueAtLastTimeStep_5_c = condIsTrue;
      if (FSM0_DW.chartAbsoluteTimeCounter -
          FSM0_DW.durationLastReferenceTick_5_c >= FSM0_P.FSM_debounce_tmo *
          100.0F) {
        // Outport: '<Root>/knobs_transfer'
        FSM0_Y.knobs_transfer = MAP_Transfer_None;
        FSM0_DW.durationLastReferenceTick_1_gm =
          FSM0_DW.chartAbsoluteTimeCounter;
        FSM0_DW.is_STATES_HANDLING = FSM0_IN_TEST_2;
        FSM0_DW.temporalCounter_i1 = 0U;

        // Outport: '<Root>/test_IV_dc'
        FSM0_Y.test_IV_dc = 100.0F;

        // Outport: '<Root>/ledsSTATES'
        FSM0_Y.ledsSTATES[b_previousEvent] = On;

        // Inport: '<Root>/buttons'
        FSM0_DW.condWasTrueAtLastTimeStep_1_a = (FSM0_U.buttons[condIsTrue_tmp_0]
          == Pressed);
      } else {
        // Inport: '<Root>/buttons'
        condIsTrue = (FSM0_U.buttons[condIsTrue_tmp_0] == Released);
        if ((!condIsTrue) || (!FSM0_DW.condWasTrueAtLastTimeStep_3_m)) {
          FSM0_DW.durationLastReferenceTick_3_b =
            FSM0_DW.chartAbsoluteTimeCounter;
        }

        FSM0_DW.condWasTrueAtLastTimeStep_3_m = condIsTrue;
        guard1 = false;
        if ((!FSM0_DW.exit_test) && (FSM0_DW.chartAbsoluteTimeCounter -
             FSM0_DW.durationLastReferenceTick_3_b >= FSM0_P.FSM_debounce_tmo *
             100.0F)) {
          FSM0_DW.exit_test = true;
          guard1 = true;
        } else {
          // Inport: '<Root>/MultiSelector_Go'
          condIsTrue = (FSM0_U.MultiSelector_Go == Pressed);
          if ((!condIsTrue) || (!FSM0_DW.condWasTrueAtLastTimeStep_2_b)) {
            FSM0_DW.durationLastReferenceTick_2_b =
              FSM0_DW.chartAbsoluteTimeCounter;
          }

          FSM0_DW.condWasTrueAtLastTimeStep_2_b = condIsTrue;
          if (FSM0_DW.chartAbsoluteTimeCounter -
              FSM0_DW.durationLastReferenceTick_2_b >= FSM0_P.FSM_debounce_tmo *
              100.0F) {
            FSM0_latch_multiselector();
            guard1 = true;
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
          FSM0_DW.durationLastReferenceTick_5_c =
            FSM0_DW.chartAbsoluteTimeCounter;
          FSM0_DW.durationLastReferenceTick_4_g =
            FSM0_DW.chartAbsoluteTimeCounter;
          FSM0_DW.durationLastReferenceTick_3_b =
            FSM0_DW.chartAbsoluteTimeCounter;
          FSM0_DW.durationLastReferenceTick_2_b =
            FSM0_DW.chartAbsoluteTimeCounter;
          FSM0_DW.durationLastReferenceTick_1_f =
            FSM0_DW.chartAbsoluteTimeCounter;
          FSM0_DW.is_STATES_HANDLING = FSM0_IN_IDLE_n;

          // Outport: '<Root>/ledsSTATES'
          FSM0_Y.ledsSTATES[condIsTrue_tmp] = Off;
          FSM0_Y.ledsSTATES[b_previousEvent] = Off;
          FSM0_Y.ledsSTATES[static_cast<int32_T>(PRVC)] = Off;

          // Outport: '<Root>/test_IV_dc'
          FSM0_Y.test_IV_dc = 0.0F;

          // Outport: '<Root>/test_OV_cmd'
          FSM0_Y.test_OV_cmd = false;

          // Inport: '<Root>/buttons'
          FSM0_DW.condWasTrueAtLastTimeStep_1_o =
            (FSM0_U.buttons[condIsTrue_tmp_0] == Pressed);

          // Inport: '<Root>/MultiSelector_Go'
          FSM0_DW.condWasTrueAtLastTimeStep_2_b = (FSM0_U.MultiSelector_Go ==
            Pressed);

          // Inport: '<Root>/buttons'
          FSM0_DW.condWasTrueAtLastTimeStep_3_m =
            (FSM0_U.buttons[condIsTrue_tmp_0] == Released);
          FSM0_DW.condWasTrueAtLastTimeStep_4_f = (FSM0_U.buttons[condIsTrue_tmp]
            == Pressed);
          FSM0_DW.condWasTrueAtLastTimeStep_5_c =
            (FSM0_U.buttons[b_previousEvent] == Pressed);
        }
      }
    }
  }
}

// Function for Chart: '<S1>/FSM'
void FSM0ModelClass::FSM0_STATES_HANDLING(void)
{
  boolean_T condIsTrue;
  real_T tmp;
  int32_T condIsTrue_tmp;
  int32_T ledsSTATES_tmp;
  int32_T ledsSTATES_tmp_0;
  int32_T ledsSTATES_tmp_1;
  boolean_T guard1 = false;
  guard1 = false;
  switch (FSM0_DW.is_STATES_HANDLING) {
   case FSM0_IN_CPAP:
    FSM0_CPAP();
    break;

   case FSM0_IN_IDLE:
    FSM0_IDLE_j();
    break;

   case FSM0_IN_INIT_d:
    FSM0_INIT();
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
    if ((!condIsTrue) || (!FSM0_DW.condWasTrueAtLastTimeStep_1_g)) {
      FSM0_DW.durationLastReferenceTick_1_i = FSM0_DW.chartAbsoluteTimeCounter;
    }

    FSM0_DW.condWasTrueAtLastTimeStep_1_g = condIsTrue;
    if (FSM0_DW.chartAbsoluteTimeCounter - FSM0_DW.durationLastReferenceTick_1_i
        >= FSM0_P.FSM_debounce_tmo * 100.0F) {
      FSM0_DW.exit_test = false;
      FSM0_DW.durationLastReferenceTick_5_c = FSM0_DW.chartAbsoluteTimeCounter;
      FSM0_DW.durationLastReferenceTick_4_g = FSM0_DW.chartAbsoluteTimeCounter;
      FSM0_DW.durationLastReferenceTick_3_b = FSM0_DW.chartAbsoluteTimeCounter;
      FSM0_DW.durationLastReferenceTick_2_b = FSM0_DW.chartAbsoluteTimeCounter;
      FSM0_DW.durationLastReferenceTick_1_f = FSM0_DW.chartAbsoluteTimeCounter;
      FSM0_DW.is_STATES_HANDLING = FSM0_IN_IDLE_n;
      ledsSTATES_tmp = static_cast<int32_T>(CPAP);

      // Outport: '<Root>/ledsSTATES'
      FSM0_Y.ledsSTATES[ledsSTATES_tmp] = Off;
      ledsSTATES_tmp_1 = static_cast<int32_T>(VCV);

      // Outport: '<Root>/ledsSTATES'
      FSM0_Y.ledsSTATES[ledsSTATES_tmp_1] = Off;
      FSM0_Y.ledsSTATES[static_cast<int32_T>(PRVC)] = Off;

      // Outport: '<Root>/test_IV_dc'
      FSM0_Y.test_IV_dc = 0.0F;

      // Outport: '<Root>/test_OV_cmd'
      FSM0_Y.test_OV_cmd = false;

      // Inport: '<Root>/buttons'
      FSM0_DW.condWasTrueAtLastTimeStep_1_o = (FSM0_U.buttons[condIsTrue_tmp] ==
        Pressed);

      // Inport: '<Root>/MultiSelector_Go'
      FSM0_DW.condWasTrueAtLastTimeStep_2_b = (FSM0_U.MultiSelector_Go ==
        Pressed);

      // Inport: '<Root>/buttons'
      FSM0_DW.condWasTrueAtLastTimeStep_3_m = (FSM0_U.buttons[condIsTrue_tmp] ==
        Released);
      FSM0_DW.condWasTrueAtLastTimeStep_4_f = (FSM0_U.buttons[ledsSTATES_tmp] ==
        Pressed);
      FSM0_DW.condWasTrueAtLastTimeStep_5_c = (FSM0_U.buttons[ledsSTATES_tmp_1] ==
        Pressed);
    } else if (FSM0_DW.temporalCounter_i1 >= FSM0_DW.test1_period * 100.0F) {
      FSM0_DW.durationLastReferenceTick_1_i = FSM0_DW.chartAbsoluteTimeCounter;
      FSM0_DW.is_STATES_HANDLING = FSM0_IN_TEST_1;
      FSM0_DW.temporalCounter_i1 = 0U;

      // Outport: '<Root>/test_IV_dc'
      FSM0_Y.test_IV_dc = 0.0F;

      // Outport: '<Root>/test_OV_cmd'
      FSM0_Y.test_OV_cmd = true;

      // Outport: '<Root>/ledsSTATES'
      FSM0_Y.ledsSTATES[static_cast<int32_T>(CPAP)] = On;

      // Inport: '<Root>/buttons'
      FSM0_DW.condWasTrueAtLastTimeStep_1_g = (FSM0_U.buttons[condIsTrue_tmp] ==
        Pressed);
    } else {
      // Outport: '<Root>/test_IV_dc'
      FSM0_Y.test_IV_dc += FSM0_DW.test1_dc_peak / FSM0_DW.test1_period *
        FSM0_P.FSM_Ts;
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

   case FSM0_IN_TEST_2:
    condIsTrue_tmp = static_cast<int32_T>(IDLE);

    // Inport: '<Root>/buttons'
    condIsTrue = (FSM0_U.buttons[condIsTrue_tmp] == Pressed);
    if ((!condIsTrue) || (!FSM0_DW.condWasTrueAtLastTimeStep_1_a)) {
      FSM0_DW.durationLastReferenceTick_1_gm = FSM0_DW.chartAbsoluteTimeCounter;
    }

    FSM0_DW.condWasTrueAtLastTimeStep_1_a = condIsTrue;
    if (FSM0_DW.chartAbsoluteTimeCounter -
        FSM0_DW.durationLastReferenceTick_1_gm >= FSM0_P.FSM_debounce_tmo *
        100.0F) {
      FSM0_DW.exit_test = false;
      FSM0_DW.durationLastReferenceTick_5_c = FSM0_DW.chartAbsoluteTimeCounter;
      FSM0_DW.durationLastReferenceTick_4_g = FSM0_DW.chartAbsoluteTimeCounter;
      FSM0_DW.durationLastReferenceTick_3_b = FSM0_DW.chartAbsoluteTimeCounter;
      FSM0_DW.durationLastReferenceTick_2_b = FSM0_DW.chartAbsoluteTimeCounter;
      FSM0_DW.durationLastReferenceTick_1_f = FSM0_DW.chartAbsoluteTimeCounter;
      FSM0_DW.is_STATES_HANDLING = FSM0_IN_IDLE_n;
      ledsSTATES_tmp = static_cast<int32_T>(CPAP);

      // Outport: '<Root>/ledsSTATES'
      FSM0_Y.ledsSTATES[ledsSTATES_tmp] = Off;
      ledsSTATES_tmp_1 = static_cast<int32_T>(VCV);

      // Outport: '<Root>/ledsSTATES'
      FSM0_Y.ledsSTATES[ledsSTATES_tmp_1] = Off;
      FSM0_Y.ledsSTATES[static_cast<int32_T>(PRVC)] = Off;

      // Outport: '<Root>/test_IV_dc'
      FSM0_Y.test_IV_dc = 0.0F;

      // Outport: '<Root>/test_OV_cmd'
      FSM0_Y.test_OV_cmd = false;

      // Inport: '<Root>/buttons'
      FSM0_DW.condWasTrueAtLastTimeStep_1_o = (FSM0_U.buttons[condIsTrue_tmp] ==
        Pressed);

      // Inport: '<Root>/MultiSelector_Go'
      FSM0_DW.condWasTrueAtLastTimeStep_2_b = (FSM0_U.MultiSelector_Go ==
        Pressed);

      // Inport: '<Root>/buttons'
      FSM0_DW.condWasTrueAtLastTimeStep_3_m = (FSM0_U.buttons[condIsTrue_tmp] ==
        Released);
      FSM0_DW.condWasTrueAtLastTimeStep_4_f = (FSM0_U.buttons[ledsSTATES_tmp] ==
        Pressed);
      FSM0_DW.condWasTrueAtLastTimeStep_5_c = (FSM0_U.buttons[ledsSTATES_tmp_1] ==
        Pressed);
    } else if (FSM0_DW.temporalCounter_i1 >= FSM0_DW.test2_period * 100.0F) {
      // Outport: '<Root>/test_OV_cmd'
      FSM0_Y.test_OV_cmd = !FSM0_Y.test_OV_cmd;
      FSM0_DW.durationLastReferenceTick_1_gm = FSM0_DW.chartAbsoluteTimeCounter;
      FSM0_DW.is_STATES_HANDLING = FSM0_IN_TEST_2;
      FSM0_DW.temporalCounter_i1 = 0U;

      // Outport: '<Root>/test_IV_dc'
      FSM0_Y.test_IV_dc = 100.0F;

      // Outport: '<Root>/ledsSTATES'
      FSM0_Y.ledsSTATES[static_cast<int32_T>(VCV)] = On;

      // Inport: '<Root>/buttons'
      FSM0_DW.condWasTrueAtLastTimeStep_1_a = (FSM0_U.buttons[condIsTrue_tmp] ==
        Pressed);
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

   case FSM0_IN_VCV:
    // Outport: '<Root>/Controller_Mode'
    FSM0_Y.Controller_Mode = VCV;
    if (FSM0_DW.sfEvent == FSM0_event_idle_controller) {
      guard1 = true;
    } else {
      // Inport: '<Root>/buttons'
      condIsTrue = (FSM0_U.buttons[static_cast<int32_T>(IDLE)] == Pressed);
      if ((!condIsTrue) || (!FSM0_DW.condWasTrueAtLastTimeStep_1_d)) {
        FSM0_DW.durationLastReferenceTick_1_g = FSM0_DW.chartAbsoluteTimeCounter;
      }

      FSM0_DW.condWasTrueAtLastTimeStep_1_d = condIsTrue;
      if (FSM0_DW.chartAbsoluteTimeCounter -
          FSM0_DW.durationLastReferenceTick_1_g >= FSM0_P.FSM_debounce_tmo *
          100.0F) {
        FSM0_DW.enter_test = false;
        guard1 = true;
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
    }
    break;
  }

  if (guard1) {
    FSM0_DW.durationLastReferenceTick_6 = FSM0_DW.chartAbsoluteTimeCounter;
    FSM0_DW.durationLastReferenceTick_5 = FSM0_DW.chartAbsoluteTimeCounter;
    FSM0_DW.durationLastReferenceTick_4 = FSM0_DW.chartAbsoluteTimeCounter;
    FSM0_DW.durationLastReferenceTick_3 = FSM0_DW.chartAbsoluteTimeCounter;
    FSM0_DW.durationLastReferenceTick_2 = FSM0_DW.chartAbsoluteTimeCounter;
    FSM0_DW.durationLastReferenceTick_1 = FSM0_DW.chartAbsoluteTimeCounter;
    FSM0_DW.is_STATES_HANDLING = FSM0_IN_IDLE;
    ledsSTATES_tmp = static_cast<int32_T>(IDLE);

    // Outport: '<Root>/ledsSTATES'
    FSM0_Y.ledsSTATES[ledsSTATES_tmp] = On;
    ledsSTATES_tmp_1 = static_cast<int32_T>(CPAP);

    // Outport: '<Root>/ledsSTATES'
    FSM0_Y.ledsSTATES[ledsSTATES_tmp_1] = Off;
    condIsTrue_tmp = static_cast<int32_T>(VCV);

    // Outport: '<Root>/ledsSTATES'
    FSM0_Y.ledsSTATES[condIsTrue_tmp] = Off;
    ledsSTATES_tmp_0 = static_cast<int32_T>(PRVC);

    // Outport: '<Root>/ledsSTATES'
    FSM0_Y.ledsSTATES[ledsSTATES_tmp_0] = Off;

    // Outport: '<Root>/Controller_Mode'
    FSM0_Y.Controller_Mode = IDLE;

    // Inport: '<Root>/buttons'
    FSM0_DW.condWasTrueAtLastTimeStep_1 = (FSM0_U.buttons[ledsSTATES_tmp] ==
      Pressed);
    FSM0_DW.condWasTrueAtLastTimeStep_2 = (FSM0_U.buttons[ledsSTATES_tmp] ==
      Released);
    FSM0_DW.condWasTrueAtLastTimeStep_3 = (FSM0_U.buttons[ledsSTATES_tmp_1] ==
      Pressed);
    FSM0_DW.condWasTrueAtLastTimeStep_4 = (FSM0_U.buttons[condIsTrue_tmp] ==
      Pressed);

    // Inport: '<Root>/MultiSelector_Go'
    FSM0_DW.condWasTrueAtLastTimeStep_5 = (FSM0_U.MultiSelector_Go == Pressed);

    // Inport: '<Root>/buttons'
    FSM0_DW.condWasTrueAtLastTimeStep_6 = (FSM0_U.buttons[ledsSTATES_tmp_0] ==
      Pressed);
  }
}

// Function for Chart: '<S1>/FSM'
void FSM0ModelClass::FSM0_c3_FSM0(void)
{
  // Chart: '<S1>/FSM' incorporates:
  //   Outport: '<Root>/ledsSTATES'

  if (FSM0_DW.is_active_c3_FSM0 == 0U) {
    FSM0_DW.chartAbsoluteTimeCounter = 0;
    FSM0_DW.is_active_c3_FSM0 = 1U;
    FSM0_DW.is_active_STATES_HANDLING = 1U;
    FSM0_DW.enter_test = false;
    FSM0_DW.exit_test = false;
    FSM0_DW.is_STATES_HANDLING = FSM0_IN_INIT_d;
    FSM0_DW.is_active_IDLE_BLINKING = 1U;
    FSM0_DW.is_IDLE_BLINKING = FSM0_IN_INIT;
    FSM0_Y.ledsSTATES[static_cast<int32_T>(IDLE)] = Off;
  } else {
    if (FSM0_DW.is_active_STATES_HANDLING != 0U) {
      FSM0_STATES_HANDLING();
    }

    if (FSM0_DW.is_active_IDLE_BLINKING != 0U) {
      switch (FSM0_DW.is_IDLE_BLINKING) {
       case FSM0_IN_INIT:
        if (FSM0_DW.sfEvent == FSM0_event_blink_IDLE_on) {
          FSM0_DW.is_IDLE_BLINKING = FSM0_IN_ON;
          FSM0_DW.temporalCounter_i2 = 0U;
          FSM0_Y.ledsSTATES[static_cast<int32_T>(IDLE)] = On;
        }
        break;

       case FSM0_IN_OFF:
        if (FSM0_DW.temporalCounter_i2 >= FSM0_P.FSM_blinking_period * 100.0F) {
          FSM0_DW.is_IDLE_BLINKING = FSM0_IN_ON;
          FSM0_DW.temporalCounter_i2 = 0U;
          FSM0_Y.ledsSTATES[static_cast<int32_T>(IDLE)] = On;
        } else {
          if (FSM0_DW.sfEvent == FSM0_event_blink_IDLE_off) {
            FSM0_DW.is_IDLE_BLINKING = FSM0_IN_INIT;
            FSM0_Y.ledsSTATES[static_cast<int32_T>(IDLE)] = Off;
          }
        }
        break;

       case FSM0_IN_ON:
        if (FSM0_DW.temporalCounter_i2 >= FSM0_P.FSM_blinking_period * 100.0F) {
          FSM0_DW.is_IDLE_BLINKING = FSM0_IN_OFF;
          FSM0_DW.temporalCounter_i2 = 0U;
          FSM0_Y.ledsSTATES[static_cast<int32_T>(IDLE)] = Off;
        } else {
          if (FSM0_DW.sfEvent == FSM0_event_blink_IDLE_off) {
            FSM0_DW.is_IDLE_BLINKING = FSM0_IN_INIT;
            FSM0_Y.ledsSTATES[static_cast<int32_T>(IDLE)] = Off;
          }
        }
        break;
      }
    }
  }

  // End of Chart: '<S1>/FSM'
}

// Model step function
void FSM0ModelClass::step()
{
  boolean_T condIsTrue;
  int32_T condIsTrue_tmp;
  int32_T condIsTrue_tmp_0;
  int32_T condIsTrue_tmp_1;

  // Outputs for Atomic SubSystem: '<Root>/FSM'
  // Constant: '<S6>/button_1'
  FSM0_B.MatrixConcatenate[0] = FSM0_P.button_1_Value[0];

  // Constant: '<S6>/button_2'
  FSM0_B.MatrixConcatenate[2] = FSM0_P.button_2_Value[0];

  // Constant: '<S6>/button_3'
  FSM0_B.MatrixConcatenate[4] = FSM0_P.button_3_Value[0];

  // Constant: '<S6>/button_4'
  FSM0_B.MatrixConcatenate[6] = FSM0_P.button_4_Value[0];

  // Constant: '<S8>/button_1'
  FSM0_B.MatrixConcatenate[8] = FSM0_P.button_1_Value_h[0];

  // Constant: '<S8>/button_2'
  FSM0_B.MatrixConcatenate[10] = FSM0_P.button_2_Value_d[0];

  // Constant: '<S8>/button_3'
  FSM0_B.MatrixConcatenate[12] = FSM0_P.button_3_Value_f[0];

  // Constant: '<S8>/button_4'
  FSM0_B.MatrixConcatenate[14] = FSM0_P.button_4_Value_j[0];

  // Constant: '<S4>/button_1'
  FSM0_B.MatrixConcatenate[16] = FSM0_P.button_1_Value_j[0];

  // Constant: '<S4>/button_2'
  FSM0_B.MatrixConcatenate[18] = FSM0_P.button_2_Value_k[0];

  // Constant: '<S4>/button_3'
  FSM0_B.MatrixConcatenate[20] = FSM0_P.button_3_Value_g[0];

  // Constant: '<S4>/button_4'
  FSM0_B.MatrixConcatenate[22] = FSM0_P.button_4_Value_b[0];

  // Constant: '<S5>/button_1'
  FSM0_B.MatrixConcatenate[24] = FSM0_P.button_1_Value_g[0];

  // Constant: '<S5>/button_2'
  FSM0_B.MatrixConcatenate[26] = FSM0_P.button_2_Value_h[0];

  // Constant: '<S5>/button_3'
  FSM0_B.MatrixConcatenate[28] = FSM0_P.button_3_Value_d[0];

  // Constant: '<S5>/button_4'
  FSM0_B.MatrixConcatenate[30] = FSM0_P.button_4_Value_f[0];

  // Constant: '<S7>/button_1'
  FSM0_B.MatrixConcatenate[32] = FSM0_P.button_1_Value_l[0];

  // Constant: '<S7>/button_2'
  FSM0_B.MatrixConcatenate[34] = FSM0_P.button_2_Value_m[0];

  // Constant: '<S7>/button_3'
  FSM0_B.MatrixConcatenate[36] = FSM0_P.button_3_Value_e[0];

  // Constant: '<S7>/button_4'
  FSM0_B.MatrixConcatenate[38] = FSM0_P.button_4_Value_h[0];

  // Constant: '<S6>/button_1'
  FSM0_B.MatrixConcatenate[1] = FSM0_P.button_1_Value[1];

  // Constant: '<S6>/button_2'
  FSM0_B.MatrixConcatenate[3] = FSM0_P.button_2_Value[1];

  // Constant: '<S6>/button_3'
  FSM0_B.MatrixConcatenate[5] = FSM0_P.button_3_Value[1];

  // Constant: '<S6>/button_4'
  FSM0_B.MatrixConcatenate[7] = FSM0_P.button_4_Value[1];

  // Constant: '<S8>/button_1'
  FSM0_B.MatrixConcatenate[9] = FSM0_P.button_1_Value_h[1];

  // Constant: '<S8>/button_2'
  FSM0_B.MatrixConcatenate[11] = FSM0_P.button_2_Value_d[1];

  // Constant: '<S8>/button_3'
  FSM0_B.MatrixConcatenate[13] = FSM0_P.button_3_Value_f[1];

  // Constant: '<S8>/button_4'
  FSM0_B.MatrixConcatenate[15] = FSM0_P.button_4_Value_j[1];

  // Constant: '<S4>/button_1'
  FSM0_B.MatrixConcatenate[17] = FSM0_P.button_1_Value_j[1];

  // Constant: '<S4>/button_2'
  FSM0_B.MatrixConcatenate[19] = FSM0_P.button_2_Value_k[1];

  // Constant: '<S4>/button_3'
  FSM0_B.MatrixConcatenate[21] = FSM0_P.button_3_Value_g[1];

  // Constant: '<S4>/button_4'
  FSM0_B.MatrixConcatenate[23] = FSM0_P.button_4_Value_b[1];

  // Constant: '<S5>/button_1'
  FSM0_B.MatrixConcatenate[25] = FSM0_P.button_1_Value_g[1];

  // Constant: '<S5>/button_2'
  FSM0_B.MatrixConcatenate[27] = FSM0_P.button_2_Value_h[1];

  // Constant: '<S5>/button_3'
  FSM0_B.MatrixConcatenate[29] = FSM0_P.button_3_Value_d[1];

  // Constant: '<S5>/button_4'
  FSM0_B.MatrixConcatenate[31] = FSM0_P.button_4_Value_f[1];

  // Constant: '<S7>/button_1'
  FSM0_B.MatrixConcatenate[33] = FSM0_P.button_1_Value_l[1];

  // Constant: '<S7>/button_2'
  FSM0_B.MatrixConcatenate[35] = FSM0_P.button_2_Value_m[1];

  // Constant: '<S7>/button_3'
  FSM0_B.MatrixConcatenate[37] = FSM0_P.button_3_Value_e[1];

  // Constant: '<S7>/button_4'
  FSM0_B.MatrixConcatenate[39] = FSM0_P.button_4_Value_h[1];

  // Chart: '<S1>/FSM' incorporates:
  //   Inport: '<Root>/MultiSelector_Go'
  //   Inport: '<Root>/buttons'

  FSM0_DW.chartAbsoluteTimeCounter++;
  condIsTrue_tmp = static_cast<int32_T>(IDLE);
  condIsTrue = (FSM0_U.buttons[condIsTrue_tmp] == Pressed);
  if ((!condIsTrue) || (!FSM0_DW.condWasTrueAtLastTimeStep_1)) {
    FSM0_DW.durationLastReferenceTick_1 = FSM0_DW.chartAbsoluteTimeCounter;
  }

  FSM0_DW.condWasTrueAtLastTimeStep_1 = condIsTrue;
  condIsTrue = (FSM0_U.buttons[condIsTrue_tmp] == Pressed);
  if ((!condIsTrue) || (!FSM0_DW.condWasTrueAtLastTimeStep_1_o)) {
    FSM0_DW.durationLastReferenceTick_1_f = FSM0_DW.chartAbsoluteTimeCounter;
  }

  FSM0_DW.condWasTrueAtLastTimeStep_1_o = condIsTrue;
  condIsTrue = (FSM0_U.buttons[condIsTrue_tmp] == Released);
  if ((!condIsTrue) || (!FSM0_DW.condWasTrueAtLastTimeStep_2)) {
    FSM0_DW.durationLastReferenceTick_2 = FSM0_DW.chartAbsoluteTimeCounter;
  }

  FSM0_DW.condWasTrueAtLastTimeStep_2 = condIsTrue;
  condIsTrue_tmp_0 = static_cast<int32_T>(CPAP);
  condIsTrue = (FSM0_U.buttons[condIsTrue_tmp_0] == Pressed);
  if ((!condIsTrue) || (!FSM0_DW.condWasTrueAtLastTimeStep_3)) {
    FSM0_DW.durationLastReferenceTick_3 = FSM0_DW.chartAbsoluteTimeCounter;
  }

  FSM0_DW.condWasTrueAtLastTimeStep_3 = condIsTrue;
  condIsTrue = (FSM0_U.buttons[condIsTrue_tmp] == Pressed);
  if ((!condIsTrue) || (!FSM0_DW.condWasTrueAtLastTimeStep_1_d)) {
    FSM0_DW.durationLastReferenceTick_1_g = FSM0_DW.chartAbsoluteTimeCounter;
  }

  FSM0_DW.condWasTrueAtLastTimeStep_1_d = condIsTrue;
  condIsTrue = (FSM0_U.buttons[condIsTrue_tmp] == Pressed);
  if ((!condIsTrue) || (!FSM0_DW.condWasTrueAtLastTimeStep_1_e)) {
    FSM0_DW.durationLastReferenceTick_1_g5 = FSM0_DW.chartAbsoluteTimeCounter;
  }

  FSM0_DW.condWasTrueAtLastTimeStep_1_e = condIsTrue;
  condIsTrue_tmp_1 = static_cast<int32_T>(VCV);
  condIsTrue = (FSM0_U.buttons[condIsTrue_tmp_1] == Pressed);
  if ((!condIsTrue) || (!FSM0_DW.condWasTrueAtLastTimeStep_4)) {
    FSM0_DW.durationLastReferenceTick_4 = FSM0_DW.chartAbsoluteTimeCounter;
  }

  FSM0_DW.condWasTrueAtLastTimeStep_4 = condIsTrue;
  condIsTrue = (FSM0_U.MultiSelector_Go == Pressed);
  if ((!condIsTrue) || (!FSM0_DW.condWasTrueAtLastTimeStep_5)) {
    FSM0_DW.durationLastReferenceTick_5 = FSM0_DW.chartAbsoluteTimeCounter;
  }

  FSM0_DW.condWasTrueAtLastTimeStep_5 = condIsTrue;
  condIsTrue = (FSM0_U.buttons[static_cast<int32_T>(PRVC)] == Pressed);
  if ((!condIsTrue) || (!FSM0_DW.condWasTrueAtLastTimeStep_6)) {
    FSM0_DW.durationLastReferenceTick_6 = FSM0_DW.chartAbsoluteTimeCounter;
  }

  FSM0_DW.condWasTrueAtLastTimeStep_6 = condIsTrue;
  condIsTrue = (FSM0_U.buttons[condIsTrue_tmp] == Pressed);
  if ((!condIsTrue) || (!FSM0_DW.condWasTrueAtLastTimeStep_1_p)) {
    FSM0_DW.durationLastReferenceTick_1_h = FSM0_DW.chartAbsoluteTimeCounter;
  }

  FSM0_DW.condWasTrueAtLastTimeStep_1_p = condIsTrue;
  condIsTrue = (FSM0_U.MultiSelector_Go == Pressed);
  if ((!condIsTrue) || (!FSM0_DW.condWasTrueAtLastTimeStep_2_b)) {
    FSM0_DW.durationLastReferenceTick_2_b = FSM0_DW.chartAbsoluteTimeCounter;
  }

  FSM0_DW.condWasTrueAtLastTimeStep_2_b = condIsTrue;
  condIsTrue = (FSM0_U.buttons[condIsTrue_tmp] == Released);
  if ((!condIsTrue) || (!FSM0_DW.condWasTrueAtLastTimeStep_3_m)) {
    FSM0_DW.durationLastReferenceTick_3_b = FSM0_DW.chartAbsoluteTimeCounter;
  }

  FSM0_DW.condWasTrueAtLastTimeStep_3_m = condIsTrue;
  condIsTrue = (FSM0_U.buttons[condIsTrue_tmp_0] == Pressed);
  if ((!condIsTrue) || (!FSM0_DW.condWasTrueAtLastTimeStep_4_f)) {
    FSM0_DW.durationLastReferenceTick_4_g = FSM0_DW.chartAbsoluteTimeCounter;
  }

  FSM0_DW.condWasTrueAtLastTimeStep_4_f = condIsTrue;
  condIsTrue = (FSM0_U.buttons[condIsTrue_tmp] == Pressed);
  if ((!condIsTrue) || (!FSM0_DW.condWasTrueAtLastTimeStep_1_g)) {
    FSM0_DW.durationLastReferenceTick_1_i = FSM0_DW.chartAbsoluteTimeCounter;
  }

  FSM0_DW.condWasTrueAtLastTimeStep_1_g = condIsTrue;
  condIsTrue = (FSM0_U.buttons[condIsTrue_tmp] == Pressed);
  if ((!condIsTrue) || (!FSM0_DW.condWasTrueAtLastTimeStep_1_a)) {
    FSM0_DW.durationLastReferenceTick_1_gm = FSM0_DW.chartAbsoluteTimeCounter;
  }

  FSM0_DW.condWasTrueAtLastTimeStep_1_a = condIsTrue;
  condIsTrue = (FSM0_U.buttons[condIsTrue_tmp_1] == Pressed);
  if ((!condIsTrue) || (!FSM0_DW.condWasTrueAtLastTimeStep_5_c)) {
    FSM0_DW.durationLastReferenceTick_5_c = FSM0_DW.chartAbsoluteTimeCounter;
  }

  FSM0_DW.condWasTrueAtLastTimeStep_5_c = condIsTrue;
  if (FSM0_DW.temporalCounter_i1 < MAX_uint32_T) {
    FSM0_DW.temporalCounter_i1++;
  }

  if (FSM0_DW.temporalCounter_i2 < MAX_uint32_T) {
    FSM0_DW.temporalCounter_i2++;
  }

  FSM0_DW.sfEvent = FSM0_CALL_EVENT;
  FSM0_c3_FSM0();

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
