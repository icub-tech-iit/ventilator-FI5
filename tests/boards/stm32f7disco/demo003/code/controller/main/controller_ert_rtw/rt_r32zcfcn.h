//
// Non-Degree Granting Education License -- for use at non-degree
// granting, nonprofit, educational organizations only. Not for
// government, commercial, or other organizational use.
//
// File: rt_r32zcfcn.h
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
#ifndef RTW_HEADER_rt_r32zcfcn_h_
#define RTW_HEADER_rt_r32zcfcn_h_
#include "rtwtypes.h"
#include "solver_zc.h"
#include "zero_crossing_types.h"
#ifndef slZcHadEvent
#define slZcHadEvent(ev, zcsDir)       (((ev) & (zcsDir)) != 0x00 )
#endif

#ifndef slZcUnAliasEvents
#define slZcUnAliasEvents(evL, evR)    ((((slZcHadEvent((evL), (SL_ZCS_EVENT_N2Z)) && slZcHadEvent((evR), (SL_ZCS_EVENT_Z2P))) || (slZcHadEvent((evL), (SL_ZCS_EVENT_P2Z)) && slZcHadEvent((evR), (SL_ZCS_EVENT_Z2N)))) ? (SL_ZCS_EVENT_NUL) : (evR)))
#endif

#ifdef __cplusplus

extern "C" {

#endif

  extern ZCEventType rt_R32ZCFcn(ZCDirection zcDir, ZCSigState *prevZc, real32_T
    currValue);

#ifdef __cplusplus

}                                      // extern "C"
#endif
#endif                                 // RTW_HEADER_rt_r32zcfcn_h_

//
// File trailer for generated code.
//
// [EOF]
//
