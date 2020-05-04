//
// Non-Degree Granting Education License -- for use at non-degree
// granting, nonprofit, educational organizations only. Not for
// government, commercial, or other organizational use.
//
// File: rt_r32zcfcn.cpp
//
// Code generated for Simulink model 'controller'.
//
// Model version                  : 1.444
// Simulink Coder version         : 9.3 (R2020a) 18-Nov-2019
// C/C++ source code generated on : Sat May  2 11:21:40 2020
//
// Target selection: ert.tlc
// Embedded hardware selection: Intel->x86-64 (Windows64)
// Code generation objectives: Unspecified
// Validation result: Not run
//

#include "rt_r32zcfcn.h"

extern "C" {
  // Detect zero crossings events.
  ZCEventType rt_R32ZCFcn(ZCDirection zcDir, ZCSigState* prevZc, real32_T
    currValue)
  {
    slZcEventType zcsDir;
    slZcEventType tempEv;
    ZCEventType zcEvent = NO_ZCEVENT;  // assume

    // zcEvent matrix
    static const slZcEventType eventMatrix[4][4] = {
      //          ZER              POS              NEG              UNK
      { SL_ZCS_EVENT_NUL, SL_ZCS_EVENT_Z2P, SL_ZCS_EVENT_Z2N, SL_ZCS_EVENT_NUL },// ZER

      { SL_ZCS_EVENT_P2Z, SL_ZCS_EVENT_NUL, SL_ZCS_EVENT_P2N, SL_ZCS_EVENT_NUL },// POS

      { SL_ZCS_EVENT_N2Z, SL_ZCS_EVENT_N2P, SL_ZCS_EVENT_NUL, SL_ZCS_EVENT_NUL },// NEG

      { SL_ZCS_EVENT_NUL, SL_ZCS_EVENT_NUL, SL_ZCS_EVENT_NUL, SL_ZCS_EVENT_NUL }// UNK
    };

    // get prevZcEvent and prevZcSign from prevZc
    slZcEventType prevEv = (slZcEventType)(((uint8_T)(*prevZc)) >> 2);
    slZcSignalSignType prevSign = (slZcSignalSignType)(((uint8_T)(*prevZc)) &
      (uint8_T)0x03);

    // get current zcSignal sign from current zcSignal value
    slZcSignalSignType currSign = (slZcSignalSignType)((currValue) > 0.0 ?
      SL_ZCS_SIGN_POS :
      ((currValue) < 0.0 ? SL_ZCS_SIGN_NEG : SL_ZCS_SIGN_ZERO));

    // get current zcEvent based on prev and current zcSignal value
    slZcEventType currEv = eventMatrix[prevSign][currSign];

    // get slZcEventType from ZCDirection
    switch (zcDir) {
     case ANY_ZERO_CROSSING:
      zcsDir = SL_ZCS_EVENT_ALL;
      break;

     case FALLING_ZERO_CROSSING:
      zcsDir = SL_ZCS_EVENT_ALL_DN;
      break;

     case RISING_ZERO_CROSSING:
      zcsDir = SL_ZCS_EVENT_ALL_UP;
      break;

     default:
      zcsDir = SL_ZCS_EVENT_NUL;
      break;
    }

    //had event, check if double zc happend remove double detection.
    if (slZcHadEvent(currEv, zcsDir)) {
      currEv = (slZcEventType)(slZcUnAliasEvents(prevEv, currEv));
    } else {
      currEv = SL_ZCS_EVENT_NUL;
    }

    // Update prevZc
    tempEv = (slZcEventType)(currEv << 2);// shift left by 2 bits
    *prevZc = (ZCSigState)((currSign) | (tempEv));
    if ((currEv & SL_ZCS_EVENT_ALL_DN) != 0) {
      zcEvent = FALLING_ZCEVENT;
    } else if ((currEv & SL_ZCS_EVENT_ALL_UP) != 0) {
      zcEvent = RISING_ZCEVENT;
    } else {
      zcEvent = NO_ZCEVENT;
    }

    return zcEvent;
  }                                    // rt_R32ZCFcn
}
//
// File trailer for generated code.
//
// [EOF]
//
